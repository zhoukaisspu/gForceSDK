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

#include <Windows.h>
#include "LogUtils.h"

using namespace gf;
using namespace std;

#define GF_LOG_BUF_SIZE 1024

const char LevelPrompt[GF_LOG_MAX] = { 'V', 'D', 'I', 'W', 'E', 'F' };

LogMethod win_log_print::_logfn = nullptr;

void log_set_output_function(LogMethod logmethod)
{
	win_log_print::_logfn = logmethod;
}

int win_log_print::print(GF_LOG_LEVEL level, const char* tag, const char* fmt, ...)
{
	va_list ap;
	char buf[GF_LOG_BUF_SIZE];
	buf[0] = '\0';

	if (CURRENT_LOG_LEVEL > level)
		return 0;
	SYSTEMTIME st;
	GetLocalTime(&st);

	int offset = sprintf_s(buf, "(%4.4u-%u-%u %u:%2.2u:%2.2u:%3.3u)[%c/%s]: ",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
		LevelPrompt[level], tag);
	if (offset < 0)
		offset = 0;
	va_start(ap, fmt);
	vsnprintf_s(buf + offset, size_t(GF_LOG_BUF_SIZE - offset), _TRUNCATE, fmt, ap);
	va_end(ap);

	if (nullptr != _logfn)
	{
		try {
			_logfn(level, buf);
		}
		catch (...) {
			cout << buf << endl;
		}
	}
	else
	{
		cout << buf << endl;
	}
	return 0;//strlen(buf)
}
