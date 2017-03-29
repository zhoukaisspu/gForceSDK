#include"npi_cmd.h"
#include"npi_evt.h"
#include"CommonDef.h"
#include"com.h"
#include"log.h"

HANDLE  g_semhdl;
UINT_PTR mTimerID;
CString mComName;
HANDLE hSerialPortEvent;

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
	spDetectThread = NULL;
	m_SPDetect = NULL;
	m_log = NULL;
	m_rx = NULL;
	m_tx = NULL;
	memset(&osWait, 0, sizeof(OVERLAPPED));
}
Com::~Com()
{
	if (logThread) {
		delete[] logThread;
	}
	if (txThread) {
		delete[] txThread;
	}
	if (rxThread) {
		delete[] rxThread;
	}
	if (evtThread) {
		delete[] evtThread;
		delete[] m_evt;
	}

	if (spDetectThread) {
		delete[] spDetectThread;
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
	if (m_SPDetect){
		delete[] m_SPDetect;
	}
	logThread = NULL;
	txThread = NULL;
	rxThread = NULL;
	evtThread = NULL;
	m_log = NULL;
	m_rx = NULL;
	m_tx = NULL;
}

BOOL Com::ReadEventFromSerialPort()
{
	DWORD nLenOut = 0;
	UINT8 event[9] = { 0x00 };
	UINT8 event_expected[9] = { 0x04, 0xFF, 0x06, 0x7F, 0x06, 0x01, 0x80, 0xFE, 0x00 };
	DWORD remaining = 9;
	DWORD offset = 0;
	OVERLAPPED osRead;
	memset(&osRead, 0, sizeof(OVERLAPPED));
	osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (com_file == NULL || com_file == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	printf("start to read event for SerialPort!!! \n");
	while (remaining > 0) {
		if (ReadFile(com_file, (PUINT8)event + offset, remaining,
			&nLenOut, &osRead)) {
			remaining -= nLenOut;
			offset += nLenOut;
		}
		else {
			if (GetLastError() == ERROR_IO_PENDING) {
				if (WaitForSingleObject(osRead.hEvent, 1000) == WAIT_OBJECT_0)
				{
					printf("ReadEvent success!!! \n");
				}
				else
				{
					printf("ReadEvent timeout!!! \n");
					return FALSE;
				}

				GetOverlappedResult(com_file, &osRead, &nLenOut, true);
				printf("ReadFile data read = %d !!! \n", nLenOut);
				if (nLenOut) {
					remaining -= nLenOut;
					offset += nLenOut;
				}
			}
		}

	}

	printf("----------------->>read end !!! \n");
	for (UINT8 i = 0; i < 9; i++)
	{
		printf("event[%d]= 0x%02x !!! \n", i, event[i]);
	}

	if (memcmp(event, event_expected, 9) == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL Com::SetupSerialPort()
{
	DCB ndcb;
	DCB dcb;
	dcb.BaudRate = 115200;
	dcb.Parity = 0;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	COMMTIMEOUTS timeouts;
	/*set timeouts*/
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 5000;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(com_file, &timeouts);

	/*set com state*/
	if (!GetCommState(com_file, &ndcb)) {
		printf("GetCommState() failed!\n");
		return FALSE;
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
		printf("SetCommState() failed!\n");
		return FALSE;
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
		printf("ClearCommError() failed");
		return FALSE;
	}

	/*set mask*/
	SetCommMask(com_file, EV_RXCHAR);

	return TRUE;
}

BOOL Com::EnumSerialPort()
{
	UINT8 result = 0;
	UINT8 data[5] = { 0x01, 0x80, 0xFE, 0x01, 0x00 };

	CString sCom;

	DWORD dwBytesWritten = 0;
	OVERLAPPED osWrite;
	memset(&osWrite, 0, sizeof(OVERLAPPED));

	dcb.BaudRate = 115200;
	dcb.Parity = 0;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	HKEY hKey;
	LPCTSTR lpSubKey = (LPCTSTR)"HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
	printf("start to enum all serial port by register \n");

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\"), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		printf("RegOpenKeyEx fail \n");
		return false;
	}

	char szValueName[NAME_LEN];
	BYTE szPortName[NAME_LEN];
	TCHAR *wStr = new TCHAR[NAME_LEN];
	LONG status;
	DWORD dwIndex = 0;
	DWORD dwSizeValueName = 100;
	DWORD dwSizeofPortName = 100;
	DWORD Type;
	dwSizeValueName = NAME_LEN;
	dwSizeofPortName = NAME_LEN;

	do
	{
		status = RegEnumValue(hKey, dwIndex++, (LPWSTR)szValueName, &dwSizeValueName, NULL, &Type,
			szPortName, &dwSizeofPortName);

		if ((status == ERROR_SUCCESS))
		{
			sCom.Format(_T("\\\\.\\%s"), szPortName);
			wprintf(L"%s \n", (const char*)sCom.GetBuffer(50));
			com_file = CreateFile(sCom.GetBuffer(50),
				GENERIC_READ | GENERIC_WRITE,
				0,/* do not share*/
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
				NULL);

			if (com_file == INVALID_HANDLE_VALUE)
			{
				printf("open com%d error:%d \n", GetLastError());
				if (ERROR_ACCESS_DENIED == GetLastError())
				{
					printf("com%d is opened by others applications!! \n", GetLastError());
				}
				continue;
			}
			else
			{
				//CloseHandle(com_file);
				/*---------------Configure Com port---------------*/
				if (FALSE == SetupSerialPort())
				{
					CloseHandle(com_file);
					com_file = INVALID_HANDLE_VALUE;
					continue;
				}

				if (!WriteFile(com_file, data, 5, &dwBytesWritten, &osWrite))
				{
					printf("after WriteFile GetLastError() = %d!!! \n", GetLastError());
					if (GetLastError() == ERROR_IO_PENDING) {
						GetOverlappedResult(com_file, &osWrite, &dwBytesWritten, true);
						if (dwBytesWritten != 5){
							printf("WriteFile failed dwBytesWritten = %d!!! \n", dwBytesWritten);
							CloseHandle(com_file);
							com_file = INVALID_HANDLE_VALUE;
							continue;
						}
						else
						{
							printf("WriteFile success dwBytesWritten = %d!!! \n", dwBytesWritten);
							if (TRUE == ReadEventFromSerialPort())
							{
								mComName = sCom;
								return TRUE;
							}
							else
							{
								CloseHandle(com_file);
								com_file = INVALID_HANDLE_VALUE;
								continue;
							}
						}
					}
					else
					{
						printf("<<<<<<<WriteFile failed dwBytesWritten = %d!!! \n", dwBytesWritten);
						CloseHandle(com_file);
						com_file = INVALID_HANDLE_VALUE;
						continue;
					}
				}
				else
				{
					CloseHandle(com_file);
					com_file = INVALID_HANDLE_VALUE;
					continue;
				}
			}

		}

		dwSizeValueName = NAME_LEN;
		dwSizeofPortName = NAME_LEN;
	} while ((status != ERROR_NO_MORE_ITEMS));

	RegCloseKey(hKey);

	return FALSE;
}

#if 0
BOOL CheckPortAvailable()
{
	HANDLE com_file;
	/*----------------Open Com port----------------*/
	com_file = CreateFile(mComName.GetBuffer(50),
		GENERIC_READ | GENERIC_WRITE,
		0,/* do not share*/
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (com_file == INVALID_HANDLE_VALUE)
	{
		if (ERROR_ACCESS_DENIED == GetLastError())
		{
			return FALSE;
		}
		else if (ERROR_FILE_NOT_FOUND == GetLastError())
		{
			printf("*****************************WARNING************************************* \n");
			printf("******Serial Port is not available any more!!!********* \n");
			printf("*****************************WARNING************************************* \n");
			return TRUE;
		}
	}
	else
	{
		printf("SerialPort is available now!! \n");
		CloseHandle(com_file);
		return TRUE;
	}

	return FALSE;
}

void CALLBACK timerCallback(
	HWND hWnd,
	UINT uMsg,
	UINT idEvent,
	DWORD dwTime
	)
{
	if (CheckPortAvailable() == TRUE)
	{
		KillTimer(NULL, mTimerID);
		::SetEvent(hSerialPortEvent);
	}
}
#endif
BOOL Com::OpenSerialPort()
{
	if (port != 0)
	{
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

		if (com_file == INVALID_HANDLE_VALUE)
		{
			LogE(L"CreateFile() with error:%d \n", GetLastError());
			/*Open specify serial port fail, try to enmulate all serial port*/
			//if (EnumSerialPort() == FALSE)
			{
				return false;
			}
		}
		else
		{
			if (SetupSerialPort() == false)
			{
				return false;
			}
			else
			{
				mComName = sCom;
			}
		}
	}
	else
	{
		if (EnumSerialPort() == FALSE)
		{
			return false;
		}
	}

	return true;
}

NPI_Queue<sEvt*, EVT_QUEUE_SIZE>* Com::Connect(HANDLE evtHdl, GF_LogType logType)
{
	if (false == OpenSerialPort())
	{
		printf("*****************************WARNING************************************* \n");
		printf("******Serial Port is not available, please plguin the dondle !!!********* \n");
		printf("*****************************WARNING************************************* \n");
		return NULL;
	}

	/*Creat log thread*/
	m_log = new Log(this, logType);
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

	m_SPDetect = new NPI_SerialPortDetect(this);
	spDetectThread = new CThread((NPI_SerialPortDetect*)m_SPDetect);
	spDetectThread->Start();
	spDetectThread->Join(100);

	//mTimerID = SetTimer(NULL, 0, 1000, timerCallback);
	hSerialPortEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	return ((NPI_RX*)m_rx)->Get_Queue();
}

int Com::Connect(GF_LogType logType)
{
	if (false == OpenSerialPort())
	{
		printf("*****************************WARNING************************************* \n");
		printf("******Serial Port is not available, please plguin the dondle !!!********* \n");
		printf("*****************************WARNING************************************* \n");
		return false;
	}

	/*Creat log thread*/
	m_log = new Log(this, logType);
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

	m_SPDetect = new NPI_SerialPortDetect(this);
	spDetectThread = new CThread((NPI_SerialPortDetect*)m_SPDetect);
	spDetectThread->Start();
	spDetectThread->Join(100);

	//mTimerID = SetTimer(NULL, 0, 1000, timerCallback);
	hSerialPortEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	return true;
}

int Com::DisConnect()
{
	HANDLE handle[5] = { 0 };
	DWORD count = 0;
	//CRITICAL_SECTION  threadCode;
	//InitializeCriticalSection(&threadCode);

	g_semhdl = CreateSemaphore(NULL, 0, 5, NULL);

	if (com_file) {
		//EnterCriticalSection(&threadCode);
		if (evtThread){
			evtThread->Terminate(0);
			handle[count++] = evtThread;
		}
		if (logThread){
			logThread->Terminate(0);
			handle[count++] = logThread;
		}
		if (txThread){
			txThread->Terminate(0);
			handle[count++] = txThread;
		}
		if (rxThread){
			rxThread->Terminate(0);
			handle[count++] = rxThread;
		}
		if (spDetectThread){
			spDetectThread->Terminate(0);
			handle[count++] = spDetectThread;
		}
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
		LogI(_T("%d threads need to exit \n"), count);
		/*wait until all threads exit*/
		while (count--){
			WaitForSingleObject(g_semhdl, INFINITE);
		}
		LogI(_T("all threads exit with err=%d\n"), GetLastError());
		CloseHandle(g_semhdl);
		g_semhdl = NULL;
		CloseHandle(com_file);
		com_file = NULL;
	}
	return 0;
}
