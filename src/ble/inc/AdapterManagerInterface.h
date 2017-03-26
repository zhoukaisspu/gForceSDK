#ifndef __ADAPTERMANAGERINTERFACE_H__
#define __ADAPTERMANAGERINTERFACE_H__
#include "GFBLETypes.h"

class GF_CClientCallback;
class GF_CAdapterManager;

class GF_CAdapterManagerInterface
{
public:
	static GF_CAdapterManagerInterface* GetInstance();
	virtual GF_STATUS Init(GF_UINT8 com_num) = 0;
	virtual GF_STATUS Deinit() = 0;
	virtual GF_STATUS StartScan(GF_UINT8 RSSI_Threshold) = 0;
	virtual GF_STATUS StopScan() = 0;
	virtual GF_STATUS Connect(GF_PUINT8 addr, UINT8 addr_type, GF_BOOL is_direct_conn) = 0;
	virtual GF_STATUS CancelConnect(GF_PUINT8 addr, GF_UINT8 addr_type) = 0;
	virtual GF_STATUS Disconnect(GF_UINT16 handle) = 0;
	virtual GF_STATUS RegisterClientCallback(GF_CClientCallback* callback) = 0;
	virtual GF_STATUS UnregisterClientCallback() = 0;

	virtual GF_STATUS ConfigMtuSize(GF_UINT16 conn_handle, GF_UINT16 MTU_Size) = 0;
	virtual GF_STATUS ConnectionParameterUpdate(GF_UINT16 conn_handle, GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max, GF_UINT16 slave_latence, GF_UINT16 supervision_timeout) = 0;
	virtual GF_STATUS WriteCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data) = 0;
	virtual GF_STATUS ReadCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle) = 0;

    virtual GF_HubState GetHubState() = 0;
	virtual GF_UINT8 GetConnectedDeviceNum() = 0;
	/*connected_device is output result*/
	virtual GF_STATUS GetConnectedDeviceByIndex(GF_UINT8 index, GF_ConnectedDevice* connected_device) = 0;
};
#endif