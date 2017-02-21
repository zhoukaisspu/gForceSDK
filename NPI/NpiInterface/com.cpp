#include"stdafx.h"
#include "npi_cmd.h"
#include"npi_evt.h"
#include"CommonDef.h"
#include"com.h"
#include"log.h"
Com::Com()
{
}

Com::Com(UINT8 nPort, DWORD nBaud, UINT8 nParity, UINT8 nByteSize,
         UINT8 nStopBit)
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
	if (logThread) {
		delete[] logThread;
	}
	if (rxThread) {
		delete[] rxThread;
	}
	if (evtThread) {
		delete[] evtThread;
	}
	if (m_rx) {
		delete[] m_rx;
	}
	if (m_tx) {
		delete[] m_tx;
	}
	if (m_log) {
		delete[] m_log;
	}

	logThread = NULL;
	txThread = NULL;
	rxThread = NULL;
	evtThread = NULL;
	m_log = NULL;
	m_rx = NULL;
	m_tx = NULL;
}

NPI_Queue<sEvt*, EVT_QUEUE_SIZE>* Com::Connect(HANDLE evtHdl)
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
	                      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
	                      NULL);

	if (com_file == INVALID_HANDLE_VALUE) {
		LogE(L"CreateFile() error:%d", GetLastError());
		return NULL;
	}

	/*---------------Configure Com port---------------*/
	/*set timeouts*/
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 5000;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(com_file, &timeouts);

	/*set com state*/
	if (!GetCommState(com_file, &ndcb)) {
		LogE(L"GetCommState() failed!\n");
		CloseHandle(com_file);
		return NULL;
	}
	ndcb.DCBlength = sizeof(DCB);
	ndcb.BaudRate = dcb.BaudRate;
	ndcb.Parity = dcb.Parity;
	ndcb.ByteSize = dcb.ByteSize;
	ndcb.StopBits = dcb.StopBits;
	ndcb.fRtsControl = RTS_CONTROL_DISABLE;
	ndcb.fDtrControl = DTR_CONTROL_ENABLE;
	ndcb.fOutxCtsFlow = FALSE;
	ndcb.fOutxDsrFlow = FALSE;
	ndcb.fOutX = FALSE;
	ndcb.fInX = FALSE;
	if (!SetCommState(com_file, &ndcb)) {
		LogE(L"SetCommState() failed!\n");
		CloseHandle(com_file);
		return NULL;
	}
	/*set buffer size*/
	//const int g_buffMax = 32768;
	//if (!SetupComm(com_file, g_buffMax, g_buffMax))
	//{
	//      LogE(L"SetupComm() failed");
	//      return false;
	//}
	/*clear buffer*/
	PurgeComm(com_file, PURGE_RXCLEAR | PURGE_TXCLEAR);

	/*clear error*/
	DWORD dwError;
	COMSTAT cs;
	if (!ClearCommError(com_file, &dwError, &cs)) {
		LogE(L"ClearCommError() failed");
		CloseHandle(com_file);
		return false;
	}

	/*set mask*/
	SetCommMask(com_file, EV_RXCHAR);

	/*Creat log thread*/
	m_log = new Log(this);
	logThread = new CThread((Log*)m_log);
	logThread->Start();
	logThread->Join(100);
	logThreadID = logThread->GetThreadID();

	/*Creat Tx thread*/
	m_tx = new NPI_TX(this);
	txThread = new CThread((NPI_TX*)m_tx);
	txThread->Start();
	txThread->Join(100);
	txThreadID = txThread->GetThreadID();

	/*Creat Rx thread*/
	m_rx = new NPI_RX(this);
	rxThread = new CThread((NPI_RX*)m_rx);
	rxThread->Start();
	rxThread->Join(100);
	rxThreadID = rxThread->GetThreadID();

	/*Event thread*/
	m_evt = evtHdl;

	return ((NPI_RX*)m_rx)->Get_Queue();
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
	                      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
	                      NULL);

	if (com_file == INVALID_HANDLE_VALUE) {
		LogE(L"CreateFile() error:%d", GetLastError());
		return false;
	}

	/*---------------Configure Com port---------------*/
	/*set timeouts*/
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 5000;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(com_file, &timeouts);

	/*set com state*/
	if (!GetCommState(com_file, &ndcb)) {
		LogE(L"GetCommState() failed!\n");
		CloseHandle(com_file);
		return false;
	}
	ndcb.DCBlength = sizeof(DCB);
	ndcb.BaudRate = dcb.BaudRate;
	ndcb.Parity = dcb.Parity;
	ndcb.ByteSize = dcb.ByteSize;
	ndcb.StopBits = dcb.StopBits;
	ndcb.fRtsControl = RTS_CONTROL_DISABLE;
	ndcb.fDtrControl = DTR_CONTROL_ENABLE;
	ndcb.fOutxCtsFlow = FALSE;
	ndcb.fOutxDsrFlow = FALSE;
	ndcb.fOutX = FALSE;
	ndcb.fInX = FALSE;
	if (!SetCommState(com_file, &ndcb)) {
		LogE(L"SetCommState() failed!\n");
		CloseHandle(com_file);
		return false;
	}
	/*set buffer size*/
	//const int g_buffMax = 32768;
	//if (!SetupComm(com_file, g_buffMax, g_buffMax))
	//{
	//      LogE(L"SetupComm() failed");
	//      return false;
	//}
	/*clear buffer*/
	PurgeComm(com_file, PURGE_RXCLEAR | PURGE_TXCLEAR);

	/*clear error*/
	DWORD dwError;
	COMSTAT cs;
	if (!ClearCommError(com_file, &dwError, &cs)) {
		LogE(L"ClearCommError() failed");
		CloseHandle(com_file);
		return false;
	}

	/*set mask*/
	SetCommMask(com_file, EV_RXCHAR);

	/*Creat log thread*/
	m_log = new Log(this);
	logThread = new CThread((Log*)m_log);
	logThread->Start();
	logThread->Join(100);
	logThreadID = logThread->GetThreadID();

	/*Creat Tx thread*/
	m_tx = new NPI_TX(this);
	txThread = new CThread((NPI_TX*)m_tx);
	txThread->Start();
	txThread->Join(100);
	txThreadID = txThread->GetThreadID();

	/*Creat Rx thread*/
	m_rx = new NPI_RX(this);
	rxThread = new CThread((NPI_RX*)m_rx);
	rxThread->Start();
	rxThread->Join(100);
	rxThreadID = rxThread->GetThreadID();

	/*Cread event thread*/
	m_evt = new NPI_EVT(this);
	evtThread = new CThread((NPI_EVT*)m_evt);
	evtThread->Start();
	evtThread->Join(100);
	evtThreadID = evtThread->GetThreadID();

	return true;
}

int Com::DisConnect()
{
	if (com_file) {
		evtThread->Terminate(0);
		logThread->Terminate(0);
		txThread->Terminate(0);
		rxThread->Terminate(0);
		/*Send exit code to exit tx thread.*/
		sCMD* pcmd = (sCMD*)new UINT8[CMD_HEAD_LEN];
		pcmd->type = HCI_EXIT_PACKET;
		pcmd->len = 0;
		((NPI_TX*)m_tx)->Get_Queue()->Push(pcmd);
		/*Send exit code to exit event thread.*/
		sEvt* pEvt = (sEvt*)new UINT8[EVT_HEADER_LEN];
		pEvt->type = HCI_EXIT_PACKET;		
		pcmd->len = 0;
		((NPI_RX*)m_rx)->Get_Queue()->Push(pEvt);
		CloseHandle(com_file);
		com_file = NULL;
	}
	return 0;
}
