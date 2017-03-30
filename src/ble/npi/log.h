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