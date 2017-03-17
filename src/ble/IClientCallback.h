#ifndef ICLIENTCALLBADK
#define ICLIENTCALLBADK
#include "oym_types.h"

class IClientCallback
{
public:
	virtual void onScanResult(BLE_DEVICE* device) = 0;
	virtual void onScanFinished() = 0;
	virtual void onDeviceConnected(OYM_STATUS status, GF_ConnectedDevice *device) = 0;
	virtual void onDeviceDisonnected(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT8 reason) = 0;

	virtual void onMTUSizeChanged(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 mtu_size) = 0;
	virtual void onConnectionParmeterUpdated(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 conn_int, OYM_UINT16 superTO, OYM_UINT16 slavelatency) = 0;
	virtual void onChracteristicValueRead(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT8 length, OYM_PUINT8 data) = 0;

	/*Notification format: data length(1 byte N) + data(N Bytes)*/
	virtual void onNotificationReceived(OYM_UINT16 handle, OYM_UINT8 length, OYM_PUINT8 data) = 0;

	virtual void onComDestory() = 0;
};
#endif