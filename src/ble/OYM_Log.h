#ifndef __OYM_LOG_H__
#define __OYM_LOG_H__

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
static OYM_BOOL mFileOpened = OYM_FALSE;
static OYM_CHAR mFileName[LOG_LOG_FILE_NAME_LENGTH];
static OYM_UINT8 mLogLevel;
static OYM_CHAR mBuffer[LOG_BUFFER_LENGTH];
static FILE *mLogFile;

/* Attention!!! different usage of VA_ARGS on windows and linux platform, be careful!!*/

/*others module use LOGDEBUG/LOGWARNING/LOGERROR function to print log*/
#ifdef WIN32
#define LOGDEBUG(tag, fmt, ...)   (OYM_Log::PrintLog(LOG_LEVEL_DEBUG, tag, fmt, __VA_ARGS__))
#define LOGWARNING(tag, fmt, ...) (OYM_Log::PrintLog(LOG_LEVEL_WARNING, tag, fmt, __VA_ARGS__))
#define LOGERROR(tag, fmt, ...)   (OYM_Log::PrintLog(LOG_LEVEL_ERROR, tag, fmt, __VA_ARGS__))
#else
#define LOGDEBUG(tag, fmt,...)   (mLog->PrintLog(LOG_LEVEL_DEBUG, tag, fmt, ##VA_ARGS))
#define LOGWARNING(tag, fmt,...) (mLog->PrintLog(LOG_LEVEL_WARNING, tag, fmt, ##VA_ARGS))
#define LOGERROR(tag, fmt,...)   (mLog->PrintLog(LOG_LEVEL_ERROR, tag, fmt, ##VA_ARGS))
#endif

class OYM_Log
{
public:
	void SetLogLevel(OYM_UINT8 mLogLevel);
	static OYM_INT PrintLog(OYM_UINT8 mLogLevel, OYM_CPCHAR tag, OYM_CPCHAR fmt, ...);

private:
	static void GetFileName(OYM_PCHAR filename);
	static OYM_BOOL OpenFile();
	OYM_Log();
	~OYM_Log();
};
#endif
