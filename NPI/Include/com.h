#ifndef _COM_H_
#define _COM_H_

#include <atlstr.h>
#include "CommonDef.h"
#include "npi_tl.h"
class Com
{
public:
	DWORD com_threadID;
	DWORD logThreadID;
	DWORD txThreadID;
	DWORD rxThreadID;
	DWORD evtThreadID;
	HANDLE com_file;
	HANDLE m_evt;
	HANDLE m_tx;
	HANDLE m_rx;
	HANDLE m_log;
	CThread* logThread;
	CThread* txThread;
	CThread* rxThread;
	CThread* evtThread;
	UINT8 ret_status;
	Com();
	Com(UINT8 nPort, DWORD nBaud = 115200, UINT8 nParity = 0, UINT8 nByteSize = 8,
	    UINT8 nStopBit = ONESTOPBIT);
	~Com();
	NPI_Queue<sEvt*, EVT_QUEUE_SIZE>* Connect(HANDLE evtHdl);
	int Connect();
	int DisConnect();
	DWORD write(PUINT8 buf, UINT8 size);

private:
	OVERLAPPED osWait;
	DCB dcb;
	COMMTIMEOUTS timeouts;
	int port;
	HANDLE hThread;
};


#endif