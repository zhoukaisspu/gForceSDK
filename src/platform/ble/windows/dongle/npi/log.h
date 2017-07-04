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
#ifndef _LOG_H_
#define _LOG_H_

extern void setColor(UINT8 red);
extern void setData(WCHAR *buf);
extern void setMessageBox(WCHAR *buf);
#define COLOR_GREEN 0
#define COLOR_RED   1
#define COLOR_BLUE  2

#define	LOGTYPE_CONSOLE 1
#define	LOGTYPE_MFC     2
#define	LOGTYPE_FILE    3
#define	LOGTYPE_MAX     3
typedef UINT8 GF_LogType;

class Log: public Runnable
{
public:
	Log(HANDLE hdl, GF_LogType type);
	~Log();
	void Run();
	void Analyze_TX(PUINT8 buf, UINT8 size);
	void Analyze_RX(PUINT8 buf, UINT8 size);

private:
	HANDLE comHdl;
	void Sprintf_HexData(PUINT8 pBuf, INT16 len);
	void Sprintf_DumpRx(PUINT8 pBuf, INT16 len);
	void Sprintf_DumpTx(PUINT8 pBuf, INT16 len);
	void Analyze_LL_Event(PUINT8 buf, UINT8 size);
	void Analyze_L2CAP_Event(PUINT8 buf, UINT8 size);
	void Analyze_ATT_Event(PUINT8 buf, UINT8 size);
	void Analyze_GATT_Event(PUINT8 buf, UINT8 size);
	void Analyze_GAP_Event(PUINT8 buf, UINT8 size);
	void Analyze_LE_Hci_Event(PUINT8 buf, UINT8 size);
	size_t Log_printf(const wchar_t* fmt, ...);
};

size_t LogE(const wchar_t* fmt, ...);
size_t LogW(const wchar_t* fmt, ...);
size_t LogI(const wchar_t* fmt, ...);
UINT16 GetLogMsg(void);
#endif // !_LOG_H_