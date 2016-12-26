#ifndef _COM_H_
#define _COM_H_

#include <atlstr.h>
#include "CommonDef.h"
#include "npi_evt.h"
class Com: public Runnable
{
public:
	DWORD com_threadID;
	DWORD logThreadID;
	DWORD rxThreadID;
	DWORD evtThreadID;
	HANDLE com_file;
	NPI_EVT *m_evt;
	UINT8 ret_status;
	Com();
	Com(UINT8 nPort, DWORD nBaud = 115200, UINT8 nParity = 0, UINT8 nByteSize = 8,
		UINT8 nStopBit = ONESTOPBIT);
	~Com();
	int Connect();
	int DisConnect();
	DWORD write(PUINT8 buf, UINT8 size);
	void Run();

private:
	OVERLAPPED osWait;
	DCB dcb;
	COMMTIMEOUTS timeouts;
	int port;
	HANDLE hThread;
	CThread* logThread;
	CThread* rxThread;
	CThread* evtThread;
};


#endif