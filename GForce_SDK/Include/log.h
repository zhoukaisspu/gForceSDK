#include"windows.h"
#include"Thread.h"

#ifndef _LOG_H_
#define	_LOG_H_

#define BUILD_UINT16(loByte, hiByte) \
          ((UINT16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

size_t LogE(const wchar_t *fmt, ...);
size_t LogW(const wchar_t *fmt, ...);
size_t LogI(const wchar_t *fmt, ...);
UINT16 GetLogMsg(void);

class Log:public Runnable
{
public:
	Log(DWORD com_id, DWORD evt_id);
	~Log();
	void Run();
	void Analyze_TX(PUINT8 buf, UINT8 size);
	void Analyze_RX(PUINT8 buf, UINT8 size);

private:
	DWORD comThreadID;
	DWORD evtThreadID;
	void Sprintf_HexData(PUINT8 pBuf, INT16 len);
	void Sprintf_DumpRx(PUINT8 pBuf, INT16 len);
	void Sprintf_DumpTx(PUINT8 pBuf, INT16 len);
	void Analyze_LL_Event(PUINT8 buf, UINT8 size);
	void Analyze_L2CAP_Event(PUINT8 buf, UINT8 size);
	void Analyze_ATT_Event(PUINT8 buf, UINT8 size);
	void Analyze_GATT_Event(PUINT8 buf, UINT8 size);
	void Analyze_GAP_Event(PUINT8 buf, UINT8 size);
	void Analyze_LE_Hci_Event(PUINT8 buf, UINT8 size);
};

#endif // !_LOG_H_