#ifndef __OYM_LOG_H__
#define __OYM_LOG_H__

#include "stdio.h"
#include "oym_types.h"

#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_OFF 4

#define LOG_BUFFER_LENGTH 256
#define LOG_MODUAL_NAME_LENGTH 50
#define LOG_LOG_FILE_NAME_LENGTH 20
#define LOG_OFFSET 50
static FILE *mLogFile;
static OYM_UINT8 mReference;

/* Attention!!! different usage of VA_ARGS on windows and linux platform, be careful!!*/

/*others module use LOGDEBUG/LOGWARNING/LOGERROR function to print log*/
#ifdef WIN32
#define LOGDEBUG(fmt,...)   (mLog->PrintLog(LOG_LEVEL_DEBUG, fmt, __VA_ARGS__))
#define LOGWARNING(fmt,...) (mLog->PrintLog(LOG_LEVEL_WARNING, fmt, __VA_ARGS__))
#define LOGERROR(fmt,...)   (mLog->PrintLog(LOG_LEVEL_ERROR, fmt, __VA_ARGS__))
#else
#define LOGDEBUG(fmt,...)   (mLog->PrintLog(LOG_LEVEL_DEBUG, fmt, ##VA_ARGS))
#define LOGWARNING(fmt,...) (mLog->PrintLog(LOG_LEVEL_DEBUG, fmt, ##VA_ARGS))
#define LOGERROR(fmt,...)   (mLog->PrintLog(LOG_LEVEL_DEBUG, fmt, ##VA_ARGS))
#endif

class OYM_Log
{
public:
	OYM_Log(OYM_CPCHAR modual_name, OYM_UINT8 length);
	~OYM_Log();
	
	OYM_INT PrintLog(OYM_UINT8 mLogLevel, OYM_CPCHAR fmt, ...);
	void SetLogLevel(OYM_UINT8 mLogLevel);
	void GetFileName(OYM_PCHAR filename);
	
	OYM_INT LogDebug(const OYM_CHAR *fmt, ...);
	OYM_INT LogWarning(const OYM_CHAR *fmt, ...);
	OYM_INT LogError(const OYM_CHAR *fmt, ...);
private:
	OYM_CHAR mFileName[LOG_LOG_FILE_NAME_LENGTH];
	
	OYM_UINT8 mLogLevel;
	OYM_PCHAR mTag; //modual name
	OYM_CHAR mBuffer[LOG_BUFFER_LENGTH];
};
#endif
