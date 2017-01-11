#include"stdafx.h"

#include"npi_tl.h"
/*---------*/
/*TX THREAD
/*---------*/
NPI_TX::NPI_TX(HANDLE hdl, DWORD logThread)
{
	m_com = hdl;
	m_log = logThread;
}

NPI_TX::~NPI_TX()
{
}

void NPI_TX::Run(void)
{
	enum {
		WR_TYPE_STATE,
		WR_OPCODE_STATE,
		WR_LEN_STATE,
		WR_OTHERS
	};
	UINT8 state = WR_TYPE_STATE;
	UINT8 buf[MAX_TX_SIZE];
	INT16 i;
	while (1) {
		if (m_cmdQue.isEmpty()) {
			continue;
		}
		switch (state) {
		case WR_TYPE_STATE:
			buf[0] = m_cmdQue.Pop();
			state = WR_OPCODE_STATE;
			break;
		case WR_OPCODE_STATE:
			buf[1] = m_cmdQue.Pop();
			buf[2] = m_cmdQue.Pop();
			state = WR_LEN_STATE;
			break;
		case WR_LEN_STATE:
			buf[3] = m_cmdQue.Pop();
			state = WR_OTHERS;
			break;
		case WR_OTHERS:
			for (i = 0; i < buf[3]; i++) {
				buf[4 + i] = m_cmdQue.Pop();
			}
			this->Write(buf, buf[3] + CMD_HEAD_LEN);
			state = WR_TYPE_STATE;
			break;
		default:
			break;
		}
	}
}

DWORD NPI_TX::Write(PUINT8 buf, UINT8 size)
{
	/*write something*/
	BOOL bErrorFlag = FALSE;
	DWORD dwBytesWritten = 0;
	PUINT8 pBuf = new UINT8[size];
	memcpy(pBuf, buf, size);

	PurgeComm(m_com, PURGE_TXCLEAR);

	bErrorFlag = WriteFile(
	                     m_com,           // open file handle
	                     buf,      // start of data to write
	                     size,  // number of bytes to write
	                     &dwBytesWritten, // number of bytes that were written
	                     NULL);

	if (FALSE == bErrorFlag) {
		LogE(L"WriteFile()\n");
		delete buf;
	} else {
		while (!PostThreadMessage(m_log, GetLogMsg(), (WPARAM)pBuf, size)) {
			LogE(L"Post LOG_MSG Err:%d\n", GetLastError());
			Sleep(500);
		}
	}
	return dwBytesWritten;
}


GForceQueue<UINT8, CMD_BUF_SIZE>* NPI_TX::Get_Queue(void)
{
	return &m_cmdQue;
}


/*---------*/
/*RX THREAD
/*---------*/

NPI_RX::NPI_RX(HANDLE hdl, DWORD logThread)
{
	m_com = hdl;
	m_log = logThread;
}

NPI_RX::~NPI_RX()
{
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
	while (1) {
		switch (state) {
		case READ_TYPE_STATE:
			if (ReadFile(m_com, &type, 1, &nLenOut, NULL)) {
				if (nLenOut) {
					state = READ_EVT_CODE;
				}
			} else {
				::LogE(L"READ_TYPE_STATE!\n");
			}
			break;
		case READ_EVT_CODE:
			if (ReadFile(m_com, &evtCode, 1, &nLenOut, NULL)) {
				if (nLenOut) {
					state = READ_DATA_LEN;
				}
			} else {
				::LogE(L"READ_EVT_CODE\n");
				state = READ_TYPE_STATE;
			}
			break;
		case READ_DATA_LEN:
			if (ReadFile(m_com, &dataLen, 1, &nLenOut, NULL)) {
				if (nLenOut) {
					state = READ_OTHERS;
					pBuf = new UINT8[dataLen + EVT_HEADER_LEN];
					pBuf[0] = type;
					pBuf[1] = evtCode;
					pBuf[2] = dataLen;
				}
			} else {
				::LogE(L"READ_DATA_LEN\n");
				state = READ_TYPE_STATE;
			}
			break;
		case READ_OTHERS:
			//printf("ST:%d\n",state);
			remaining = dataLen;
			offset = EVT_HEADER_LEN;
			while (remaining > 0) {
				if (ReadFile(m_com, pBuf + offset, remaining, &nLenOut, NULL)) {
					remaining -= nLenOut;
					offset += nLenOut;
				} else {
					::LogE(L"read file error, continue reading!\n");
				}
			}

			while (!PostThreadMessage(m_log, GetLogMsg(), (WPARAM)pBuf,
			                          dataLen + EVT_HEADER_LEN)) {
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