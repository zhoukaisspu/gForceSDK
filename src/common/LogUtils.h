/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */
#pragma once

#ifndef TAG
#define TAG "gForceSDK"
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

#ifdef GF_OPTION_LOGLEVEL // allow config log level from compiler option
const GF_LOG_LEVEL CURRENT_LOG_LEVEL = GF_OPTION_LOGLEVEL;
#else
const GF_LOG_LEVEL CURRENT_LOG_LEVEL = GF_LOG_DEBUG;
#endif

#include "utils/LogUtil_Win.h"

#elif defined (ANDROID)

#include "utils/LogUtil_Android.h"

#else

#include "utils/LogUtil_Linux.h"

#endif
