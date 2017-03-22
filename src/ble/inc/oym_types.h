#ifndef __GF_TYPES_H__
#define __GF_TYPES_H__
#pragma once

#include <Windows.h>

#include "gfTypes.h"
using namespace gf;

typedef void				GF_VOID;
typedef long				GF_LONG;
typedef unsigned long		GF_ULONG;
typedef GF_INT				GF_BOOL;
#define GF_FALSE			((GF_BOOL)0)
#define GF_TRUE			(GF_BOOL)(!(GF_FALSE))

#define GF_INNER_SUCCESS		((GF_INT)0)
#define GF_FAIL			((GF_INT)1)
#define GF_SUCCEEDED		(GF_INNER_SUCCESS == status)

#define BT_ADDRESS_SIZE		6

typedef enum {
	GF_ERR_CODE_SCAN_BUSY = 0x60,
	GF_ERR_CODE_NO_RESOURCE = 0x61,
} GF_ERROR_CODE;

#define BLE_DEVICE_NAME_LENGTH 30
struct GF_BLEDevice{
	GF_UINT8   addr_type;
	GF_UINT8	addr[BT_ADDRESS_SIZE];
	GF_CHAR	dev_name[BLE_DEVICE_NAME_LENGTH];
	GF_UINT8   rssi;
	GF_BLEDevice(GF_UINT8 bytes[], GF_UINT8 type)
	{
		for (int i = BT_ADDRESS_SIZE - 1; i >= 0; --i) {
			this->addr[i] = bytes[i];
		}
		addr_type = type;
	}

};

class GF_CCallBack
{
public:
	GF_CCallBack(GF_LONG eventmask, GF_UINT8 index) :mEventMask(eventmask), mIndex(index){}
	virtual GF_STATUS OnDeviceFound(GF_BLEDevice new_device) = 0;
	virtual GF_STATUS OnEvent(GF_UINT32 event, GF_PUINT8 data, GF_UINT16 length) = 0;

	GF_LONG GetEventMask()
	{
		return mEventMask;
	}

	GF_UINT8 GetIndex()
	{
		return mIndex;
	}

private:
	GF_LONG mEventMask;//which event is intent to process.
	GF_UINT8 mIndex;
};

typedef enum {
	State_Idle = 0,
	State_Scanning = 1,
	State_Connecting = 2,
}GF_HubState;

struct GF_ConnectedDevice
{
	GF_UINT8 address_type;
	GF_UINT8 address[BT_ADDRESS_SIZE];
	GF_UINT16 handle;
	GF_UINT16 conn_int;
	GF_UINT16 superTO;
	GF_UINT16 slavelatency;
	GF_UINT16 MTUsize;
};
#endif