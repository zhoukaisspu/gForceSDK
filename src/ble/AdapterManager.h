#ifndef __ADAPTERMANAGER_H__
#define __ADAPTERMANAGER_H__
#include "oym_types.h"
#include <list>
using namespace std;

class OYM_Discovery_Service;
class IClientCallback;
class OYM_RemoteDevice;
class OYM_NPI_Interface;
class OYM_Log;
class OYM_CallBack;

class OYM_AdapterManager: public OYM_CallBack
{
public:
	OYM_AdapterManager();
	~OYM_AdapterManager();

	OYM_STATUS Init(OYM_UINT8 com_num);
	OYM_STATUS Deinit();
	OYM_STATUS StartScan(OYM_UINT8 RSSI_Threshold);
	OYM_STATUS StopScan();
	OYM_STATUS Connect(OYM_PUINT8 addr, UINT8 addr_type, OYM_BOOL is_direct_conn);
	OYM_STATUS CancelConnect(OYM_PUINT8 addr, OYM_UINT8 addr_type);
	OYM_STATUS Disconnect(OYM_UINT16 handle);
	OYM_STATUS RegisterClientCallback(IClientCallback* callback)
	{
		mClientCallback = callback;
		return OYM_SUCCESS;
	}

	OYM_STATUS UnregisterClientCallback()
	{
		mClientCallback = NULL;
		return OYM_SUCCESS;
	}

	OYM_STATUS ConfigMtuSize(OYM_UINT16 conn_handle, OYM_UINT16 MTU_Size);
	OYM_STATUS ConnectionParameterUpdate(OYM_UINT16 conn_handle, OYM_UINT16 conn_interval_min, OYM_UINT16 conn_interval_max, OYM_UINT16 slave_latence, OYM_UINT16 supervision_timeout);
	OYM_STATUS WriteCharacteristic(OYM_UINT16 conn_handle, OYM_UINT16 attribute_handle, OYM_UINT8 data_length, OYM_PUINT8 data);
	OYM_STATUS ReadCharacteristic(OYM_UINT16 conn_handle, OYM_UINT16 attribute_handle);

    GF_DondleState GetDongleState();
	OYM_UINT8 GetConnectedDeviceNum();
	/*connected_device is output result*/
	OYM_STATUS GetConnectedDeviceByIndex(OYM_UINT8 index, GF_ConnectedDevice* connected_device);

	OYM_STATUS OnEvent(OYM_UINT32 event, OYM_PUINT8 data, OYM_UINT16 length);
	OYM_STATUS OnDeviceFound(BLE_DEVICE new_device);
private:
	OYM_STATUS ConnectComplete(OYM_UINT16 handle);
	OYM_STATUS ScanFinished();
	OYM_STATUS OnConnectEvent(OYM_PUINT8 data, OYM_UINT16 length);
	OYM_RemoteDevice* GetDeviceByHandle(OYM_UINT16 handle);
	OYM_STATUS mIsConnecting;
	OYM_STATUS mIsScanning;
	OYM_NPI_Interface* mInterface;
	OYM_CPCHAR mTag;
	OYM_Discovery_Service* mDS;

	list<OYM_RemoteDevice*> mAvailabeDevice;
	list<OYM_RemoteDevice*> mConnectingDevice;
	list<OYM_RemoteDevice*> mConnectedDevice;

	IClientCallback* mClientCallback;
};
#endif