#pragma once
#include "oym_types.h"
//define com port number
#define COM_PORT_NUM 3

#define LOCAL_GATT_CLIENT_MTU_SIZE 23

#define STD_OUT

class IClientCallback
{
public:
	virtual void onScanResult(BLE_DEVICE* device) = 0;
	virtual void onScanFinished() = 0;
	virtual void onDeviceConnected(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 conn_int, OYM_UINT16 superTO, OYM_UINT16 slavelatency, OYM_UINT16 MTU_Size) = 0;
	virtual void onDeviceDisonnected(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT8 reason) = 0;

	virtual void onMTUSizeChanged(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 mtu_size) = 0;
	virtual void onConnectionParmeterUpdated(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 conn_int, OYM_UINT16 superTO, OYM_UINT16 slavelatency) = 0;
	virtual void onChracteristicValueRead(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT8 length, OYM_PUINT8 data) = 0;

	/*Notification format: data length(1 byte N) + data(N Bytes)*/
	virtual void onNotificationReceived(OYM_UINT16 handle, OYM_UINT8 length, OYM_PUINT8 data) = 0;

	virtual void onComDestory() = 0;
};

class Device
{
public:
	OYM_UINT8 address_type;
	OYM_UINT8 address[6];
	OYM_UINT8 name[50];
	OYM_UINT8 cod;
	OYM_UINT8 state;
};
