/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */
#include"npi_cmd.h"
#include"npi_tl.h"
#include"log.h"

extern HANDLE hSerialPortEvent;
extern CString mComName;
extern HANDLE g_semhdl_NPI_RX;

/*---------*/
/*Serial Port Detect THREAD
/*---------*/

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

NPI_SerialPortDetect::NPI_SerialPortDetect(HANDLE hdl)
{
	comHdl = hdl;
	mNeedCheck = TRUE;
}

NPI_SerialPortDetect::~NPI_SerialPortDetect()
{
	comHdl = NULL;
}

void NPI_SerialPortDetect::Run(void)
{
	HANDLE hEvent = ((Com *)comHdl)->spDetectThread->GetEvent();

	while (1)
	{
		if (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0){
			CloseHandle(hEvent);
			return;
		}

		Sleep(100);

		if (mNeedCheck == TRUE && CheckPortAvailable() == TRUE)
		{
			::SetEvent(hSerialPortEvent);
			mNeedCheck = FALSE;
		}
	}
}

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
			if (ERROR_INVALID_THREAD_ID == GetLastError())
			{
				break;
			}
			Sleep(0);
		}
		if (pCmd->type == HCI_EXIT_PACKET){
			CloseHandle(hEvent);
			/*when using _beginthread and _endthread, do not explicitly close the thread handle by calling the Win32 CloseHandle API.*/
			//CloseHandle(hThread);
			delete pCmd;
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
			CloseHandle(hEvent);
			return;
		}

		if (WaitForSingleObject(hSerialPortEvent, 0) == WAIT_OBJECT_0){
			::LogE(L"hSerialPortEvent received!\n");
			CloseHandle(hSerialPortEvent);
			pEvt = (sEvt*)new UINT8[dataLen + EVT_HEADER_LEN];
			pEvt->type = HCI_PORT_CLOSE_PACKET;
			pEvt->evtCode = 0x00;
			pEvt->len = 0x00;
			m_evtQue.Push(pEvt);
		}

		switch (state) {
		case READ_TYPE_STATE:
			if (ReadFile(((Com*)comHdl)->com_file, &type, 1, &nLenOut, &osRead)) {
				if (nLenOut) {
					state = READ_EVT_CODE;
				}
			} else {
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
			} else {
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
			} else {
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
				} else {
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
				if (ERROR_INVALID_THREAD_ID == GetLastError())
				{
					break;
				}
				Sleep(0);
			}
			if((pEvt->type != HCI_CMD_PACKET)
			 && (pEvt->type != HCI_ACL_DATA_PACKET)
			 && (pEvt->type != HCI_SCO_DATA_PACKET)
			 && (pEvt->type != HCI_EVENT_PACKET)){
			   ::LogW(L"Wrong type (%d) and len(%d)\n", pEvt->type, pEvt->len);
			   delete pEvt;
		    }else{
			   m_evtQue.Push(pEvt);
		    }
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