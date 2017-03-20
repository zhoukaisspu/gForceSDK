#pragma once


#include <android/log.h>

#if defined(LOG_TO_CONSOLE) || defined(LOG_TO_FILE)
#define gf_log_print_verbose(fmt, ...)	__android_log_print(ANDROID_LOG_VERBOSE, TAG, fmt, __VA_ARGS__)
#define gf_log_print_debug(fmt, ...)	__android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, __VA_ARGS__)
#define gf_log_print_info(fmt, ...)	__android_log_print(ANDROID_LOG_INFO, TAG, fmt, __VA_ARGS__)
#define gf_log_print_warn(fmt, ...)	__android_log_print(ANDROID_LOG_WARN, TAG, fmt, __VA_ARGS__)
#define gf_log_print_error(fmt, ...)	__android_log_print(ANDROID_LOG_ERROR, TAG, fmt, __VA_ARGS__)
#define gf_log_print_fatal(fmt, ...)	__android_log_print(ANDROID_LOG_FATAL, TAG, fmt, __VA_ARGS__)
#else
#define gf_log_print_verbose(fmt, ...)
#define gf_log_print_debug(fmt, ...)
#define gf_log_print_info(fmt, ...)
#define gf_log_print_warn(fmt, ...)
#define gf_log_print_error(fmt, ...)
#define gf_log_print_fatal(fmt, ...)
#endif


#define GF_LOGV(fmt, ...) gf_log_print_verbose(fmt, ##__VA_ARGS__)
#define GF_LOGD(fmt, ...) gf_log_print_debug(fmt, ##__VA_ARGS__)
#define GF_LOGI(fmt, ...) gf_log_print_info(fmt, ##__VA_ARGS__)
#define GF_LOGW(fmt, ...) gf_log_print_warn(fmt, ##__VA_ARGS__)
#define GF_LOGE(fmt, ...) gf_log_print_error(fmt, ##__VA_ARGS__)
#define GF_LOGF(fmt, ...) gf_log_print_fatal(fmt, ##__VA_ARGS__)
