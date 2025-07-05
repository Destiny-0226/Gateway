#include "Int_CAN.h"

void Int_CAN_Init(void)
{

    // 1. 配置过滤器
    CAN_FilterTypeDef sFilterConfig;

    // 过滤器编号
    sFilterConfig.FilterBank           = 0;                     // 0-13
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;      // 绑定到队列0
    sFilterConfig.FilterMode           = CAN_FILTERMODE_IDMASK; // 标识符屏蔽位模式
    sFilterConfig.FilterScale          = CAN_FILTERSCALE_32BIT; // 32位宽
    sFilterConfig.FilterIdHigh         = 0x0000;                // 32位ID高位
    sFilterConfig.FilterIdLow          = 0x0000;                // 32位ID低位
    sFilterConfig.FilterMaskIdHigh     = 0x0000;                // 32位MASK高位
    sFilterConfig.FilterMaskIdLow      = 0x0000;                // 32位MASK低位
    sFilterConfig.FilterActivation     = ENABLE;                // 使能过滤器

    HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);

    // 2. 启动can
    HAL_CAN_Start(&hcan);
}

void Int_CAN_Send(uint32_t id, uint8_t *data, uint16_t len)
{
    // 1. CAN外设一共有3个发送邮箱
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0) {
        HAL_Delay(100);
    }

    // 2. 调用发送数据
    CAN_TxHeaderTypeDef TxHeader;
    // TxHeader.StdId = id; // 消息ID
    TxHeader.ExtId = id; // 消息ID
    TxHeader.IDE   = CAN_ID_EXT; // 扩展模式
    // TxHeader.IDE = CAN_ID_STD;   // 标准格式
    TxHeader.RTR = CAN_RTR_DATA; // 数据帧
    TxHeader.DLC = len;          // 数据长度
    uint32_t TxMailbox;
    HAL_CAN_AddTxMessage(&hcan, &TxHeader, data, &TxMailbox); // 数据
}

void Int_CAN_Receive(CAN_MSG *msgs, uint8_t *msg_count)
{
    // 1. 查询收件箱中是否有消息
    *msg_count = HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0);

    // 2. 收到消息存放到msgs
    for (uint8_t i = 0; i < *msg_count; i++) {
        CAN_RxHeaderTypeDef Rxheader;
        HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &Rxheader, msgs[i].data);
        // 统一使用标准格式的数据帧
        msgs[i].id  = Rxheader.StdId;
        msgs[i].len = Rxheader.DLC;
    }
}
