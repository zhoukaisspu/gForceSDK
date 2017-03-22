#ifndef __ADAPTERMANAGER_H__
#define __ADAPTERMANAGER_H__
#include "oym_types.h"
#include "AdapterManagerInterface.h"
#include <list>
using namespace std;

class GF_CDiscoveryService;
class GF_CClientCallback;
class GF_CRemoteDevice;
class GF_CNpiInterface;
class GF_CClientCallback;
class GF_CCallBack;
static GF_BOOL mInitialized = GF_FALSE;

class GF_CAdapterManager : public GF_CCallBack, public GF_CAdapterManagerInterface
{
public:
	GF_STATUS Init(GF_UINT8 com_num);
	GF_STATUS Deinit();
	GF_STATUS StartScan(GF_UINT8 RSSI_Threshold);
	GF_STATUS StopScan();
	GF_STATUS Connect(GF_PUINT8 addr, UINT8 addr_type, GF_BOOL is_direct_conn);
	GF_STATUS CancelConnect(GF_PUINT8 addr, GF_UINT8 addr_type);
	GF_STATUS Disconnect(GF_UINT16 handle);
	GF_STATUS RegisterClientCallback(GF_CClientCallback* callback)
	{
		mClientCallback = callback;
		return GF_SUCCESS;
	}

	GF_STATUS UnregisterClientCallback()
	{
		mClientCallback = NULL;
		return GF_SUCCESS;
	}

	GF_STATUS ConfigMtuSize(GF_UINT16 conn_handle, GF_UINT16 MTU_Size);
	GF_STATUS ConnectionParameterUpdate(GF_UINT16 conn_handle, GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max, GF_UINT16 slave_latence, GF_UINT16 supervision_timeout);
	GF_STATUS WriteCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data);
	GF_STATUS ReadCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle);

    GF_HubState GetHubState();
	GF_UINT8 GetConnectedDeviceNum();
	/*connected_device is output result*/
	GF_STATUS GetConnectedDeviceByIndex(GF_UINT8 index, GF_ConnectedDevice* connected_device);

	GF_STATUS OnEvent(GF_UINT32 event, GF_PUINT8 data, GF_UINT16 length);
	GF_STATUS OnDeviceFound(GF_BLEDevice new_device);
	static GF_CAdapterManager* mInstance;
	GF_CAdapterManager();
	~GF_CAdapterManager();
private:
	GF_STATUS ConnectComplete(GF_UINT16 handle);
	GF_STATUS ScanFinished();
	GF_STATUS OnConnectEvent(GF_PUINT8 data, GF_UINT16 length);
	GF_CRemoteDevice* GetDeviceByHandle(GF_UINT16 handle);
	GF_STATUS mIsConnecting;
	GF_STATUS mIsScanning;
	GF_CNpiInterface* mInterface;
	GF_CPCHAR mTag;
	GF_CDiscoveryService* mDS;

	list<GF_CRemoteDevice*> mAvailabeDevice;
	list<GF_CRemoteDevice*> mConnectingDevice;
	list<GF_CRemoteDevice*> mConnectedDevice;

	GF_CClientCallback* mClientCallback;
};
#endif