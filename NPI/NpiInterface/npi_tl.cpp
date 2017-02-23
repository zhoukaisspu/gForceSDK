#include"stdafx.h"
#include"npi_cmd.h"
#include"npi_tl.h"
#include"log.h"
/*---------*/
/*TX THREAD
/*---------*/
NPI_TX::NPI_TX(HANDLE hdl)
{
	comHdl = hdl;
}

NPI_TX::~NPI_TX()
{
	comHdl = NULL;
}

void NPI_TX::Run(void)
{
	INT8 totalLen;
	DWORD dwBytesWritten = 0;
	sCMD* pCmd = NULL;
	HANDLE hThread = ((Com *)comHdl)->txThread;
	HANDLE hEvent = ((Com *)comHdl)->txThread->GetEvent();

	OVERLAPPED osWrite;
	memset(&osWrite, 0, sizeof(OVERLAPPED));
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	while (1) {
		pCmd = m_cmdQue.Pop();
		totalLen = pCmd->len + CMD_HEAD_LEN;
		/*send to log thread*/
		PUINT8 pLog = new UINT8[totalLen];
		memcpy_s(pLog, totalLen, pCmd, totalLen);
		while (!PostThreadMessage(((Com*)comHdl)->logThreadID, GetLogMsg(),
			(WPARAM)pLog, totalLen)) {
			LogE(L"TX:Post LOG_MSG Err:%d\n", GetLastError());
			Sleep(500);
		}

		if (pCmd->type == HCI_EXIT_PACKET){
			::LogI(_T("TX Thread Exit!\n"));
			delete pCmd;
			CloseHandle(hEvent);
			//CloseHandle(hThread);
			return;
		}

		/*write to uart*/
		if (!WriteFile(((Com*)comHdl)->com_file, pCmd, totalLen, &dwBytesWritten,
		               &osWrite)) {
			if (GetLastError() == ERROR_IO_PENDING) {
				GetOverlappedResult(((Com*)comHdl)->com_file, &osWrite, &dwBytesWritten, true);
				if (dwBytesWritten != totalLen){
					LogW(_T("Written Bytes != Total Len\n"));
				}
			}				
		}
		delete pCmd;
	}
}

NPI_Queue<sCMD*, CMD_QUEUE_SIZE>* NPI_TX::Get_Queue(void)
{
	return &m_cmdQue;
}


/*---------*/
/*RX THREAD
/*---------*/

NPI_RX::NPI_RX(HANDLE hdl)
{
	comHdl = hdl;
}

NPI_RX::~NPI_RX()
{
	comHdl = NULL;
}

void NPI_RX::Run(void)
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
	sEvt*  pEvt = NULL;
	HANDLE hThread = ((Com *)comHdl)->rxThread;
	HANDLE hEvent = ((Com *)comHdl)->rxThread->GetEvent();
	OVERLAPPED osRead;
	memset(&osRead, 0, sizeof(OVERLAPPED));
	osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	
		while (1) {
			if (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0){
				::LogI(_T("RX Thread Exit! 111 \n"));
				CloseHandle(hEvent);
				//CloseHandle(hThread);
				::LogI(_T("RX Thread Exit 222 !\n"));
				return;
			}

			switch (state) {
			case READ_TYPE_STATE:
				if (ReadFile(((Com*)comHdl)->com_file, &type, 1, &nLenOut, &osRead)) {
					if (nLenOut) {
						state = READ_EVT_CODE;
					}
				}
				else {
					if (GetLastError() == ERROR_IO_PENDING) {
						GetOverlappedResult(((Com*)comHdl)->com_file, &osRead, &nLenOut, true);
						if (nLenOut) {
							state = READ_EVT_CODE;
						}
					}
				}
				break;
			case READ_EVT_CODE:
				if (ReadFile(((Com*)comHdl)->com_file, &evtCode, 1, &nLenOut, &osRead)) {
					if (nLenOut) {
						state = READ_DATA_LEN;
					}
				}
				else {
					if (GetLastError() == ERROR_IO_PENDING) {
						GetOverlappedResult(((Com*)comHdl)->com_file, &osRead, &nLenOut, true);
						if (nLenOut) {
							state = READ_DATA_LEN;
						}
					}
				}
				break;
			case READ_DATA_LEN:
				if (ReadFile(((Com*)comHdl)->com_file, &dataLen, 1, &nLenOut, &osRead)) {
					if (nLenOut) {
						state = READ_OTHERS;
						pEvt = (sEvt*)new UINT8[dataLen + EVT_HEADER_LEN];
						pEvt->type = type;
						pEvt->evtCode = evtCode;
						pEvt->len = dataLen;
					}
				}
				else {
					if (GetLastError() == ERROR_IO_PENDING) {
						GetOverlappedResult(((Com*)comHdl)->com_file, &osRead, &nLenOut, true);
						if (nLenOut) {
							state = READ_OTHERS;
							pEvt = (sEvt*)new UINT8[dataLen + EVT_HEADER_LEN];
							pEvt->type = type;
							pEvt->evtCode = evtCode;
							pEvt->len = dataLen;
						}
					}
				}
				break;
			case READ_OTHERS:
				remaining = dataLen;
				offset = EVT_HEADER_LEN;
				while (remaining > 0) {
					if (ReadFile(((Com*)comHdl)->com_file, (PUINT8)pEvt + offset, remaining,
						&nLenOut, &osRead)) {
						remaining -= nLenOut;
						offset += nLenOut;
					}
					else {
						if (GetLastError() == ERROR_IO_PENDING) {
							GetOverlappedResult(((Com*)comHdl)->com_file, &osRead, &nLenOut, true);
							if (nLenOut) {
								remaining -= nLenOut;
								offset += nLenOut;
							}
						}
					}
				}

				pBuf = new UINT8[dataLen + EVT_HEADER_LEN];
				memcpy_s(pBuf, dataLen + EVT_HEADER_LEN, pEvt, dataLen + EVT_HEADER_LEN);
				while (!PostThreadMessage(((Com*)comHdl)->logThreadID, GetLogMsg(),
					(WPARAM)pBuf,
					dataLen + EVT_HEADER_LEN)) {
					LogW(L"RX:Post LOG_MSG Err:%d\n", GetLastError());
					Sleep(500);
				}
				m_evtQue.Push(pEvt);
				state = READ_TYPE_STATE;
				break;
			default:
				::LogE(L"Read File Err!\n");
				break;
			}
		}

}

NPI_Queue<sEvt*, EVT_QUEUE_SIZE>* NPI_RX::Get_Queue(void)
{
	return &m_evtQue;
}