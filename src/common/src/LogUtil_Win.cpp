/*
* Copyright 2017, OYMotion Inc.
* All rights reserved.
*
* IMPORTANT: Your use of this Software is limited to those specific rights
* granted under the terms of a software license agreement between you and
* OYMotion.  You may not use this Software unless you agree to abide by the
* terms of the License. The License limits your use, and you acknowledge,
* that the Software may not be modified, copied or distributed unless used
* solely and exclusively in conjunction with an OYMotion product.  Other
* than for the foregoing purpose, you may not use, reproduce, copy, prepare
* derivative works of, modify, distribute, perform, display or sell this
* Software and/or its documentation for any purpose.
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

#include "LogUtils.h"

using namespace gf;

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

	int offset = sprintf_s(buf, "[%c/%s]: ", LevelPrompt[level], tag);
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
