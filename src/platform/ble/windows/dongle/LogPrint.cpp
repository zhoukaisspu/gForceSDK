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
#include <LogPrint.h>
/*
* GF_CLog is used to print log to a file, which use timestamp as file name.
* other modules create a instant of GF_CLog by new GF_CLog(modual_name, length_of_modual_name);
* then the log will print with modual_name as a tag.
*/

#ifdef WIN32

#else
#include<sys/time.h>
#endif

GF_VOID GF_CLog::GetFileName(GF_PCHAR filename)
{
#ifdef WIN32
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf_s(filename, LOG_LOG_FILE_NAME_LENGTH, "%02d_%02d_%02d_%03d.txt", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
#else
	struct timeval tv;
	struct timezone tz;
	struct tm *tm;
	time_t t;
	
	gettimeofday(&tv, &tz);
	time(&t);
	tm = localtime(&t);
	
	offset = sprintf(mFileName, "%02d:%02d:%02d.%03d.txt", tm->tm_hour, tm->tm_min, tm->tm_sec, int(tv.tv_usec / 1000));
	total += offset;
#endif
}

GF_BOOL GF_CLog::OpenFile()
{
	if (mFileOpened == GF_FALSE)
	{
		GetFileName(mFileName);
#ifdef WIN32
		fopen_s(&mLogFile, (GF_CPCHAR)mFileName, "wt+");
		if (mLogFile == NULL)
		{
			printf("open file failed!!!");
			mFileOpened = GF_FALSE;
			return GF_FALSE;
		}
		mFileOpened = GF_TRUE;
#else
		mLogFile = fopen(mFileName, "wt+");
#endif
	}

	return GF_TRUE;
}

GF_CLog::~GF_CLog()
{
	if (mFileOpened == GF_TRUE)
	{
		fclose(mLogFile);
		mFileOpened = GF_FALSE;
	}
}

void GF_CLog::SetLogLevel(GF_UINT8 logLevel)
{
	if (logLevel >= LOG_LEVEL_OFF)
	{
		logLevel = LOG_LEVEL_OFF;
	}
	mLogLevel = logLevel;
}

GF_INT GF_CLog::PrintLog(GF_UINT8 loglevel, GF_CPCHAR tag, GF_CPCHAR fmt, ...)
{
	va_list argp;
	GF_INT offset = 0;
	GF_INT total = 0;
	if (mLogLevel > loglevel)
	{
		return total;
	}

	if (OpenFile() == GF_FALSE)
	{
		return 0;
	}

	//get time of log
#ifdef WIN32
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	offset = sprintf_s((GF_PCHAR)mBuffer, LOG_BUFFER_LENGTH, "%02d:%02d:%02d.%03d", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	total += offset;
#else
	struct timeval tv;
	struct timezone tz;
	struct tm *tm;
	time_t t;
	
	gettimeofday(&tv, &tz);
	time(&t);
	tm = localtime(&t);
	
	offset = sprintf(mBuffer, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, int(tv.tv_usec / 1000));
	total += offset;
#endif

	if (loglevel == LOG_LEVEL_DEBUG)
	{
#ifdef WIN32
		offset = sprintf_s((GF_PCHAR)mBuffer + offset, LOG_BUFFER_LENGTH - total, "[Debug]/%s:", tag);
#else
		offset = sprintf((GF_PCHAR)mBuffer + offset, "[Debug]/%s:", modual);
#endif
	} 
	else if (loglevel == LOG_LEVEL_WARNING)
	{
#ifdef WIN32
		offset = sprintf_s((GF_PCHAR)mBuffer + offset, LOG_BUFFER_LENGTH - total, "[Warning]/%s:", tag);
#else
		offset = sprintf((GF_PCHAR)mBuffer + offset, "[Debug]/%s:", modual);
#endif
	}
	else if (loglevel == LOG_LEVEL_ERROR)
	{
#ifdef WIN32
		offset = sprintf_s((GF_PCHAR)mBuffer + offset, LOG_BUFFER_LENGTH - total, "[Error]/%s:", tag);
#else
		offset = sprintf((GF_PCHAR)mBuffer + offset, "[Debug]/%s:", modual);
#endif
	}
	total += offset;
	
	va_start(argp, fmt);
#ifdef WIN32
	offset = vsprintf_s((GF_PCHAR)mBuffer + total, LOG_BUFFER_LENGTH - total, (GF_CPCHAR)fmt, argp);
#else
	offset = vsnprintf((GF_PCHAR)mBuffer + total, LOG_BUFFER_LENGTH - total, fmt, argp);
#endif
	va_end(argp);

	mBuffer[offset + total] = '\n';

	fwrite(mBuffer, offset + total, 1, mLogFile);
	
	return offset + total + 1;
}

