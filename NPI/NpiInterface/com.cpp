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
	txThread = NULL;
	rxThread = NULL;
	evtThread = NULL;
	memset(&osWait, 0, sizeof(OVERLAPPED));
}
Com::~Com()
{
	delete[] logThread;
	delete[] rxThread;
	delete[] evtThread;
	delete[] m_rx;
	delete[] m_tx;
	logThread = NULL;
	txThread = NULL;
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
	
	/*Creat Tx thread*/
	m_tx = new NPI_TX(com_file, logThreadID);
	txThread = new CThread(m_tx);
	txThread->Start();
	txThread->Join(100);
	txThreadID = txThread->GetThreadID();

	/*Creat Rx thread*/
	m_rx = new NPI_RX(com_file,logThreadID);
	rxThread = new CThread(m_rx);
	rxThread->Start();
	rxThread->Join(100);
	rxThreadID = rxThread->GetThreadID();

	return true;
}

int Com::DisConnect(){
	CloseHandle(com_file);
	rxThread->Terminate(0);
	return 0;
}
