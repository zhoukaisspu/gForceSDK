#ifndef _LOG_H_
#define	_LOG_H_

class Log:public Runnable
{
public:
	Log(DWORD com_id, DWORD evt_id);
	~Log();
	void Run();
	void Analyze_TX(PUINT8 buf, UINT8 size);
	void Analyze_RX(PUINT8 buf, UINT8 size);
	CHARFORMAT2 cf;

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
	size_t Log_printf(const wchar_t* fmt, ...);
};

size_t LogE(const wchar_t *fmt, ...);
size_t LogW(const wchar_t *fmt, ...);
size_t LogI(const wchar_t *fmt, ...);
UINT16 GetLogMsg(void);
#endif // !_LOG_H_