#pragma once


#include <iostream>
#include <stdarg.h>
using namespace std;

#define GF_LOG_BUF_SIZE 1024

const char LevelPrompt[GF_LOG_MAX] = { 'V', 'D', 'I', 'W', 'E', 'F' };

struct win_log_print {
	static int print(GF_LOG_LEVEL level, const char* tag, const char* fmt, ...)
	{
		va_list ap;
		char buf[GF_LOG_BUF_SIZE];
		buf[0] = '\0';

		if (CURRENT_LOG_LEVEL > level)
			return 0;

		int offset = sprintf_s(buf, "[%c/%s]: ", LevelPrompt[level], tag);
		if (offset < 0)
			offset = 0;
		va_start(ap, fmt);
		vsnprintf_s(buf + offset, size_t(GF_LOG_BUF_SIZE - offset), _TRUNCATE, fmt, ap);
		va_end(ap);

		cout << buf << endl;
		return 0;//strlen(buf)
	}
};

#if defined(LOG_TO_CONSOLE)

#define gf_log_print_verbose(fmt, ...)	win_log_print::print(GF_LOG_VERBOSE, TAG, fmt, __VA_ARGS__)
#define gf_log_print_debug(fmt, ...)	win_log_print::print(GF_LOG_DEBUG, TAG, fmt, __VA_ARGS__)
#define gf_log_print_info(fmt, ...)		win_log_print::print(GF_LOG_INFO, TAG, fmt, __VA_ARGS__)
#define gf_log_print_warn(fmt, ...)		win_log_print::print(GF_LOG_WARN, TAG, fmt, __VA_ARGS__)
#define gf_log_print_error(fmt, ...)	win_log_print::print(GF_LOG_ERROR, TAG, fmt, __VA_ARGS__)
#define gf_log_print_fatal(fmt, ...)	win_log_print::print(GF_LOG_FATAL, TAG, fmt, __VA_ARGS__)

#elif defined(LOG_TO_FILE)

#define gf_log_print_verbose(fmt, ...)
#define gf_log_print_debug(fmt, ...)
#define gf_log_print_info(fmt, ...)
#define gf_log_print_warn(fmt, ...)
#define gf_log_print_error(fmt, ...)
#define gf_log_print_fatal(fmt, ...)

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
