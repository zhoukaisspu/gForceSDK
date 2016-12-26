#include"stdafx.h"

#include"npi_evt.h"
#include "CommonDef.h"
#include "com.h"
Com::Com()
{
}

Com::Com(UINT8 nPort, DWORD nBaud, UINT8 nParity, UINT8 nByteSize, UINT8 nStopBit)
{
	com_threadID = GetCurrentThreadId();
	port = nPort;
	dcb.BaudRate = nBaud;
	dcb.Parity = nParity;
	dcb.ByteSize = nByteSize;
	dcb.StopBits = nStopBit;
	logThread = NULL;
	rxThread = NULL;
	evtThread = NULL;
	memset(&osWait, 0, sizeof(OVERLAPPED));
}
Com::~Com()
{
	delete[] logThread;
	delete[] rxThread;
	delete[] evtThread;
	logThread = NULL;
	rxThread = NULL;
	evtThread = NULL;
}

int Com::Connect()
{
	DCB ndcb;
	CString sCom;
	sCom.Format(_T("\\\\.\\COM%d"), port);
	/*----------------Open Com port----------------*/
	com_file = CreateFile(sCom.GetBuffer(50),
		GENERIC_READ | GENERIC_WRITE,
		0,/* do not share*/
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,// | FILE_FLAG_OVERLAPPED,
		NULL);

	if (com_file == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile() error:%d", GetLastError());
		return false;
	}

	/*---------------Configure Com port---------------*/
	/*set timeouts*/
	GetCommTimeouts(com_file, &timeouts);
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(com_file, &timeouts);

	/*set com state*/
	if (!GetCommState(com_file, &ndcb))
	{
		printf(("GetCommState() failed!\n"));
		CloseHandle(com_file);
		return false;
	}
	ndcb.DCBlength = sizeof(DCB);
	ndcb.BaudRate = dcb.BaudRate;
	ndcb.Parity = dcb.Parity;
	ndcb.ByteSize = dcb.ByteSize;
	ndcb.StopBits = dcb.StopBits;
	if (!SetCommState(com_file, &ndcb))
	{
		printf(("SetCommState() failed!\n"));
		CloseHandle(com_file);
		return false;
	}
	/*set buffer size*/
	//static const int g_buffMax = 32768;
	//if (!SetupComm(com_file, g_buffMax, g_buffMax))
	//{
	//	printf(("SetupComm() failed"));
	//	return false;	
	//}
	/*clear buffer*/
	PurgeComm(com_file, PURGE_RXCLEAR | PURGE_TXCLEAR);

	/*clear error*/
	DWORD dwError;
	COMSTAT cs;
	if (!ClearCommError(com_file, &dwError, &cs))
	{
		printf(("ClearCommError() failed"));
		CloseHandle(com_file);
		return false;
	}

	/*set mask*/
	//SetCommMask(com_file, EV_RXFLAG | EV_TXEMPTY);

	/*Cread event thread*/
	m_evt = new NPI_EVT();
	evtThread = new CThread(m_evt);
	evtThread->Start();
	evtThread->Join(100);
	evtThreadID = evtThread->GetThreadID();

	/*Creat log thread*/
	Log *log = new Log(com_threadID, evtThreadID);
	logThread = new CThread(log);
	logThread->Start();
	logThread->Join(100);
	logThreadID = logThread->GetThreadID();
	
	/*Creat rx thread*/
	rxThread = new CThread(this);
	rxThread->Start();
	rxThread->Join(100);
	rxThreadID = rxThread->GetThreadID();

	printf("Connect sucessfully!\n");
	//MessageBoxA(0, "Connect sucessfully!","UART", MB_OK);
	return true;
}

int Com::DisConnect(){
	CloseHandle(com_file);
	rxThread->Terminate(0);
	return 0;
}

DWORD Com::write(PUINT8 buf, UINT8 size) {
	/*write something*/
	BOOL bErrorFlag = FALSE;
	DWORD dwBytesWritten = 0;
	PUINT8 pBuf = new UINT8[size];
	memcpy(pBuf, buf, size);

	PurgeComm(com_file, PURGE_TXCLEAR);

	bErrorFlag = WriteFile(
		com_file,           // open file handle
		buf,      // start of data to write
		size,  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);

	if (FALSE == bErrorFlag)
	{
		LogE(L"WriteFile()\n");
		delete buf;
	}
	else {
		while (!PostThreadMessage(logThreadID, GetLogMsg(), (WPARAM)pBuf, size)) {
			printf("Post LOG_MSG Err:%d\n", GetLastError());
			Sleep(500);
		}
	}
	return dwBytesWritten;
}

void Com::Run()
{
	enum {
		READ_TYPE_STATE,
		READ_EVT_CODE,
		READ_DATA_LEN,
		READ_OTHERS
	};
	PUINT8 pBuf = NULL;
	DWORD nLenOut = 0;
	UINT8 state = READ_TYPE_STATE;
	UINT8 msgLen = 0;
	UINT8 type;
	UINT8 evtCode;
	UINT8 dataLen;
	DWORD offset;
	DWORD remaining;
	while (1)
	{
		switch (state) {
			case READ_TYPE_STATE:
				//printf("ST:%d\n", state);
				if (ReadFile(com_file, &type, 1, &nLenOut, NULL)) {
					if (nLenOut) {
						state = READ_EVT_CODE;
					}
				}
				else {
					::LogE(L"READ_TYPE_STATE!\n");
				}
				break;
			case READ_EVT_CODE:
				//printf("ST:%d\n", state);
				if (ReadFile(com_file, &evtCode, 1, &nLenOut, NULL)) {
					if (nLenOut) {
						state = READ_DATA_LEN;
					}
				}
				else {
					::LogE(L"READ_EVT_CODE\n");
					state = READ_TYPE_STATE;
				}
				break;
			case READ_DATA_LEN:
				//printf("ST:%d\n", state);
				if (ReadFile(com_file, &dataLen, 1, &nLenOut, NULL)) {
					if (nLenOut) {
						state = READ_OTHERS;
						pBuf = new UINT8[dataLen + EVT_HEADER_LEN];
						pBuf[0] = type;
						pBuf[1] = evtCode;
						pBuf[2] = dataLen;
					}
				}
				else {
					::LogE(L"READ_DATA_LEN\n");
					state = READ_TYPE_STATE;
				}
				break;
			case READ_OTHERS:
				//printf("ST:%d\n",state);
				remaining = dataLen;
				offset = EVT_HEADER_LEN;
				while ( remaining > 0 )
				{
					if (ReadFile(com_file, pBuf+offset, remaining, &nLenOut, NULL)) {
						remaining -= nLenOut;
						offset += nLenOut;
					}
					else {
						::LogE(L"read file error, continue reading!\n");
					}
				}

				while (!PostThreadMessage(logThreadID, GetLogMsg(), (WPARAM)pBuf, dataLen + EVT_HEADER_LEN)) {
						printf("Post LOG_MSG Err:%d\n", GetLastError());
						Sleep(500);
				}
				state = READ_TYPE_STATE;
				break;
			default:
				::LogE(L"Read File Err!\n");
				break;
		}
	}
}