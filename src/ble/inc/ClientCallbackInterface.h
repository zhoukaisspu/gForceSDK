#ifndef ICLIENTCALLBADK
#define ICLIENTCALLBADK
#include "GFBLETypes.h"

class GF_CClientCallback
{
public:
	virtual void onScanResult(GF_BLEDevice* device) = 0;
	virtual void onScanFinished() = 0;
	virtual void onDeviceConnected(GF_STATUS status, GF_ConnectedDevice *device) = 0;
	virtual void onDeviceDisonnected(GF_STATUS status, GF_ConnectedDevice *device, GF_UINT8 reason) = 0;

	virtual void onMTUSizeChanged(GF_STATUS status, GF_UINT16 handle, GF_UINT16 mtu_size) = 0;
	virtual void onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 handle, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency) = 0;
	virtual void onCharacteristicValueRead(GF_STATUS status, GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data) = 0;

	/*Notification format: data length(1 byte N) + data(N Bytes)*/
	virtual void onNotificationReceived(GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data) = 0;

	virtual void onComDestory() = 0;
};
#endif