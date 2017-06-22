/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between you and
 * OYMotion.  You may not use this Software unless you agree to abide by the
 * terms of the License. The License limits your use, and you acknowledge,
 * that the Software may not be modified, copied or distributed unless used
 * solely and exclusively in conjunction with an OYMotion product.  Other
 * than for the foregoing purpose, you may not use, reproduce, copy, prepare
 * derivative works of, modify, distribute, perform, display or sell this
 * Software and/or its documentation for any purpose.
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
#ifndef _COM_H_
#define _COM_H_

#include <atlstr.h>
#include "CommonDef.h"
#include "npi_tl.h"
#include "log.h"
#define NAME_LEN 100
extern HANDLE  g_semhdl;
class Com
{
public:
	DWORD com_threadID;
	DWORD logThreadID;
	DWORD txThreadID;
	DWORD rxThreadID;
	DWORD evtThreadID;
	DWORD spDetectThreadID;
	HANDLE com_file;
	HANDLE m_evt;
	HANDLE m_tx;
	HANDLE m_rx;
	HANDLE m_log;
	HANDLE m_SPDetect;
	CThread* logThread;
	CThread* txThread;
	CThread* rxThread;
	CThread* evtThread;
	CThread* spDetectThread;
	UINT8 ret_status;
	Com();
	Com(UINT8 nPort, DWORD nBaud = 115200, UINT8 nParity = 0, UINT8 nByteSize = 8,
	    UINT8 nStopBit = ONESTOPBIT);
	~Com();
	NPI_Queue<sEvt*, EVT_QUEUE_SIZE>* Connect(HANDLE evtHdl, GF_LogType logType);
	int Connect(GF_LogType logType);
	int DisConnect();
	DWORD write(PUINT8 buf, UINT8 size);
	BOOL EnumSerialPort();
	BOOL SetupSerialPort();
	BOOL ReadEventFromSerialPort();
	BOOL OpenSerialPort();
	void TerminateDetectingThread();
	void TerminateNPIRxThread();
	void TerminateLogThread();
	void TerminateNPITxThread();
	void TerminateEvtThread();

private:
	OVERLAPPED osWait;
	DCB dcb;
	COMMTIMEOUTS timeouts;
	int port;
	HANDLE hThread;
};


#endif