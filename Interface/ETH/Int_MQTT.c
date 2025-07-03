#include "Int_mqtt.h"
#include "Com_log.h"
#include <usart.h>
#include <stdlib.h>
#include <string.h>

#include "MQTTClient.h"
#include "mqtt_interface.h"
#include "socket.h"
#include "Com_cJSON.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "w5500.h"

#define TCP_SOCKET  0
#define BUFFER_SIZE 1024
#define TARGET_IP   {192, 168, 43, 32}
#define TARGET_PORT 1883 // mqtt server port

#define PULL_TOPIC  "Pull-Destiny"
#define PUSH_TOPIC  "Push-Destiny"
#define MAC_ADDR    {110, 119, 20, 02, 02, 26}
#define IP_ADDR     {192, 168, 43, 226}
#define IP_MASK     {255, 255, 255, 0}
#define IP_GATEWAY  {192, 168, 43, 1}

static unsigned char recvBuf[BUFFER_SIZE] = {0};
static unsigned char sendBuf[BUFFER_SIZE] = {0};

static MQTTClient c;
Network n;

static void (*message_callback)(void *data, size_t data_len);

void messageArrived(MessageData *md)
{
    MQTTMessage *message = md->message;
    // 选择是否打印
    log_debug("%.*s", (int)message->payloadlen, (char *)message->payload);
    // 调用回调
    message_callback(message->payload, message->payloadlen);
}

static void ETH_Reset(void)
{

    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(800);

    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(800);
    log_info("W5500 init success\r\n");
}

static void Int_ETH_Init(void)
{
    uint8_t mac[6]     = MAC_ADDR;   // myself MAC
    uint8_t ip[4]      = IP_ADDR;    // myself IP
    uint8_t submask[4] = IP_MASK;    // myself submask
    uint8_t gateway[4] = IP_GATEWAY; // myself gateway

    user_register_function();

    ETH_Reset();

    setSHAR(mac);

    setSIPR(ip);

    setSUBR(submask);

    setGAR(gateway);
}

// mqtt初始化

void Int_Mqtt_Init(void)
{
    int rc                      = 0;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    uint8_t targetIP[4]         = TARGET_IP; // mqtt server IP
    Int_ETH_Init();                          // 初始化网络接口并配置 IP 地址
    HAL_Delay(3000);                         // 等待网络稳定

    log_info("W5500 init success\r\n");
    // 选择对应的 socket
    NewNetwork(&n, TCP_SOCKET);
    rc = ConnectNetwork(&n, targetIP, TARGET_PORT);
    if (rc != SOCK_OK) {
        log_error("Error connecting network\n");
    }
    log_info("success connected network\n");

    MQTTClientInit(&c, &n, 1000, sendBuf, 1024, recvBuf, 1024);

    data.willFlag         = 0;
    data.MQTTVersion      = 3;
    data.clientID.cstring = (char *)"Destiny32"; //"stdout-subscriber-zc";
    data.username.cstring = NULL;
    data.password.cstring = NULL;

    data.keepAliveInterval = 60;
    data.cleansession      = 1;

    rc = MQTTConnect(&c, &data);
    if (rc != SUCCESSS) {
        log_error("Error connecting MQTT.\n");
    }
    log_info("Connected %d\r\n", rc);

    log_info("Subscribing to %s\r\n", PULL_TOPIC);
    // 接收来自这个的信息 PULL_TOPIC
    rc = MQTTSubscribe(&c, PULL_TOPIC, QOS0, messageArrived);
    log_info("Subscribed %d\r\n", rc);
}

// 循环调用的函数，用来处理接收回调
void Int_Mqtt_Yield()
{
    MQTTYield(&c, 100);
}

// 通过mqtt发送数据到服务器
void Int_Mqtt_SendData(uint8_t *data)
{

    MQTTMessage message;
    // message.dup        = 0;
    message.qos = QOS0;
    // message.retained   = 0;
    message.payload    = data;
    message.payloadlen = strlen((char *)data);
    MQTTPublish(&c, PUSH_TOPIC, &message);
}

void Int_Mqtt_RegisterRecvCallback(void (*callback)(void *data, size_t data_len))
{
    message_callback = callback;
}
