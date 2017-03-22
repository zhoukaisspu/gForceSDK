#ifndef __GF_CLog_H__
#define __GF_CLog_H__

#include "stdio.h"
#include "oym_types.h"

#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_OFF 4

#define TAG_NAME_LENGTH 50
#define LOG_BUFFER_LENGTH 256
#define LOG_MODUAL_NAME_LENGTH 50
#define LOG_LOG_FILE_NAME_LENGTH 20
#define LOG_OFFSET 50
static GF_BOOL mFileOpened = GF_FALSE;
static GF_CHAR mFileName[LOG_LOG_FILE_NAME_LENGTH];
static GF_UINT8 mLogLevel;
static GF_CHAR mBuffer[LOG_BUFFER_LENGTH];
static FILE *mLogFile;

/* Attention!!! different usage of VA_ARGS on windows and linux platform, be careful!!*/

/*others module use LOGDEBUG/LOGWARNING/LOGERROR function to print log*/
#ifdef WIN32
#define LOGDEBUG(tag, fmt, ...)   (GF_CLog::PrintLog(LOG_LEVEL_DEBUG, tag, fmt, __VA_ARGS__))
#define LOGWARNING(tag, fmt, ...) (GF_CLog::PrintLog(LOG_LEVEL_WARNING, tag, fmt, __VA_ARGS__))
#define LOGERROR(tag, fmt, ...)   (GF_CLog::PrintLog(LOG_LEVEL_ERROR, tag, fmt, __VA_ARGS__))
#else
#define LOGDEBUG(tag, fmt,...)   (mLog->PrintLog(LOG_LEVEL_DEBUG, tag, fmt, ##VA_ARGS))
#define LOGWARNING(tag, fmt,...) (mLog->PrintLog(LOG_LEVEL_WARNING, tag, fmt, ##VA_ARGS))
#define LOGERROR(tag, fmt,...)   (mLog->PrintLog(LOG_LEVEL_ERROR, tag, fmt, ##VA_ARGS))
#endif

class GF_CLog
{
public:
	void SetLogLevel(GF_UINT8 mLogLevel);
	static GF_INT PrintLog(GF_UINT8 mLogLevel, GF_CPCHAR tag, GF_CPCHAR fmt, ...);

private:
	static void GetFileName(GF_PCHAR filename);
	static GF_BOOL OpenFile();
	GF_CLog();
	~GF_CLog();
};
#endif
