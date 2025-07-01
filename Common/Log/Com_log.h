#ifndef __COM_LOG_H__
#define __COM_LOG_H__

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "usart.h"

// 日志级别定义
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_FATAL 4

// 当前日志级别，默认为INFO
#ifndef CURRENT_LOG_LEVEL
#define CURRENT_LOG_LEVEL LOG_LEVEL_DEBUG
#endif

// 替换__FILE__ 只保留文件名称  取出路径名称
// FILE__NAME => 去除\\的路径名称
#define FILE__NAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// 最终调用__FILE__NAME__  => 去除带有/路径名称  内部包含FILE__NAME
#define __FILE__NAME__ (strrchr(FILE__NAME, '/') ? strrchr(FILE__NAME, '/') + 1 : FILE__NAME)

// 日志输出宏
#define LOG(level, format, ...)                                                          \
    do {                                                                                 \
        printf(level " [%s]-%d: " format "\n", __FILE__NAME__, __LINE__, ##__VA_ARGS__); \
    } while (0)

// 各日志级别宏
#if CURRENT_LOG_LEVEL <= LOG_LEVEL_DEBUG
#define log_debug(format, ...) LOG("<DEBUG>", format, ##__VA_ARGS__)
#else
#define log_debug(format, ...)
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_INFO
#define log_info(format, ...) LOG("<INFO>", format, ##__VA_ARGS__)
#else
#define log_info(format, ...)
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_WARN
#define log_warn(format, ...) LOG("<WARN>", format, ##__VA_ARGS__)
#else
#define log_warn(format, ...)
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_ERROR
#define log_error(format, ...) LOG("<ERROR>", format, ##__VA_ARGS__)
#else
#define log_error(format, ...)
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_FATAL
#define log_fatal(format, ...) LOG("<FATAL>", format, ##__VA_ARGS__)
#else
#define log_fatal(format, ...)
#endif

#endif /* __COM_LOG_H__ */