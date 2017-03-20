#pragma once

#ifndef TAG
#define TAG "GForceSDK"
#endif


#define LOG_TO_CONSOLE
//#define LOG_TO_FILE


#if defined (WIN32)

typedef enum {
	GF_LOG_VERBOSE,
	GF_LOG_DEBUG,
	GF_LOG_INFO,
	GF_LOG_WARN,
	GF_LOG_ERROR,
	GF_LOG_FATAL,
	GF_LOG_MAX
} GF_LOG_LEVEL;
const GF_LOG_LEVEL CURRENT_LOG_LEVEL = GF_LOG_DEBUG;

#include "utils/LogUtil_Win.h"

#elif defined (ANDROID)

#include "utils/LogUtil_Android.h"

#else

#include "utils/LogUtil_Linux.h"

#endif
