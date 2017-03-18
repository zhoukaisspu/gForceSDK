#include "LogUtils.h"
#include "BLEDevice.h"
#include "utils.h"
#include <cstdio>


using namespace oym;


GF_RET_CODE BLEDevice::registerListener(const gfwPtr<DeviceListener>& listener)
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == listener.lock())
		return GF_ERROR_BAD_PARAM;

	mListeners.insert(listener);
	cleanInvalidWeakP(mListeners);
	return GF_SUCCESS;
}

BLEDevice::BLEDevice(gfsPtr<OYM_AdapterManager>& am, const BLE_DEVICE& bleDev)
	: mAM(am)
	, mAddrType(bleDev.addr_type)
	, mRssi(bleDev.rssi)
{
	memcpy(mAddress, bleDev.addr, sizeof(mAddress));

	GF_CHAR name[BLE_DEVICE_NAME_LENGTH + 1] = "";
	memcpy(name, bleDev.dev_name, sizeof(bleDev.dev_name));
	name[BLE_DEVICE_NAME_LENGTH] = '\0';
	mName = totstring(string(name));

	GF_LOGD("New device: address type: %d, address: %s, name: %s", mAddrType, tostring(getAddress()).c_str(), tostring(mName).c_str());
}

GF_RET_CODE BLEDevice::identifyDevice(int msec)
{
	msec;
	GF_LOGI("%s: Opps, not implemented yet.", __FUNCTION__);
	return GF_ERROR_NOT_SUPPORT;
}

GF_RET_CODE BLEDevice::getAddress(GF_UINT8* addr, GF_SIZE bufLen) const
{
	if (nullptr == addr || bufLen < sizeof(mAddress))
		return GF_ERROR_BAD_PARAM;
	memcpy(addr, mAddress, sizeof(mAddress));
	return GF_SUCCESS;
}

tstring BLEDevice::getAddress() const
{
	char str[sizeof(mAddress)*(3) + 1];
	for (int i = 0; i < sizeof(mAddress); ++i)
	{
#ifdef WIN32
		sprintf_s(str + (3 * i), sizeof(str) - (3 * i), "%2.2X:", mAddress[i]);
#else
		snprintf(str + (3 * i), sizeof(str) - (3 * i), "%2.2X:", mAddress[i]);
#endif
	}
	str[sizeof(str) - 2] = '\0';
	return totstring(string(str));
}

bool BLEDevice::isMyself(GF_UINT8 addrType, GF_UINT8 addr[]) const
{
	if (nullptr == addr)
		return false;
	if (addrType == mAddrType)
	{
		if (0 == memcmp(addr, mAddress, sizeof(mAddress)))
		{
			return true;
		}
	}
	return false;
}

bool BLEDevice::takeover(BLEDevice& from)
{
	if ((mAddrType != from.mAddrType) ||
		memcmp(mAddress, from.mAddress, sizeof(mAddress)))
	{
		GF_LOGD("%s: failed because two objects are not the same device.\nself--> addr is %u:%s.\nold--> addr is :%u:%s",
			__FUNCTION__, mAddrType, tostring(getAddress()).c_str(), from.mAddrType, tostring(from.getAddress()).c_str());
		return false;
	}
	/* copy needed
	ConnectionStatus	mCnntStatus;
	DevicePosition		mPosition;
	GF_UINT8	mAddrType;
	GF_UINT8	mAddress[BT_ADDRESS_SIZE];
	tstring		mName;
	GF_UINT8	mRssi;

	GF_UINT16	mHandle;
	GF_UINT16	mConnInt;
	GF_UINT16	mSuperTO;
	GF_UINT16	mSlavelatency;
	GF_UINT16	mMTUsize;
	*/
	mListeners = from.mListeners;
	return true;
}

GF_RET_CODE BLEDevice::setPostion(DevicePosition pos)
{
	GF_LOGD(__FUNCTION__);
	mPosition = pos;
	return GF_SUCCESS;
}

GF_RET_CODE BLEDevice::connect(bool directConn)
{
	GF_LOGD(__FUNCTION__);
	GF_RET_CODE ret = GF_ERROR;
	switch (mCnntStatus)
	{
	case DeviceConnectionStatus::Connected:
		return GF_SUCCESS;
	case DeviceConnectionStatus::Disconnecting:
		break;
	case DeviceConnectionStatus::Disconnected:
	case DeviceConnectionStatus::Connecting:
		if (OYM_SUCCESS == mAM->Connect(mAddress, mAddrType, directConn))
			ret = GF_SUCCESS;
		break;
	default:;
	}
	if (ret == GF_SUCCESS)
		mCnntStatus = DeviceConnectionStatus::Connecting;
	return ret;
}

GF_RET_CODE BLEDevice::disconnect()
{
	GF_LOGD(__FUNCTION__);
	GF_RET_CODE ret = GF_ERROR;
	switch (mCnntStatus)
	{
	case DeviceConnectionStatus::Connected:
		if (OYM_SUCCESS == mAM->Disconnect(mHandle))
			ret = GF_SUCCESS;
		break;
	case DeviceConnectionStatus::Disconnected:
	case DeviceConnectionStatus::Disconnecting:
		return GF_SUCCESS;
	case DeviceConnectionStatus::Connecting:
		break;
	default:;
	}
	if (ret == GF_SUCCESS)
		mCnntStatus = DeviceConnectionStatus::Disconnecting;
	return ret;
}

GF_RET_CODE BLEDevice::cancelConnect()
{
	GF_LOGD(__FUNCTION__);
	GF_RET_CODE ret = GF_ERROR;
	switch (mCnntStatus)
	{
	case DeviceConnectionStatus::Connected:
	case DeviceConnectionStatus::Disconnected:
	case DeviceConnectionStatus::Disconnecting:
		break;
	case DeviceConnectionStatus::Connecting:
		if (OYM_SUCCESS == mAM->CancelConnect(mAddress, mAddrType))
		{
			ret = GF_SUCCESS;
			GF_LOGD("Connection cancelling...");
		}
		break;
	default:;
	}
	if (ret == GF_SUCCESS)
		mCnntStatus = DeviceConnectionStatus::Disconnecting;
	return ret;
}

void BLEDevice::updateData(const BLE_DEVICE& bleDev)
{
	// update name and rssi
	GF_CHAR name[BLE_DEVICE_NAME_LENGTH + 1] = "";
	memcpy(name, bleDev.dev_name, sizeof(bleDev.dev_name));
	name[BLE_DEVICE_NAME_LENGTH] = '\0';
	tstring newName = totstring(string(name));
	if (newName != mName)
	{
		GF_LOGD("Update device name: %s", tostring(mName).c_str());
	}
	mRssi = bleDev.rssi;

	GF_LOGD("Update device rssi: %u", mRssi);

	// TODO: notify client.
}

void BLEDevice::onConnected(const GF_ConnectedDevice& connedDevice)
{
	GF_LOGD("Device connected: previous state is: %u", static_cast<int>(mCnntStatus));
	mHandle = connedDevice.handle;
	mConnInt = connedDevice.conn_int;
	mSuperTO = connedDevice.superTO;
	mSlavelatency = connedDevice.slavelatency;
	mMTUsize = connedDevice.MTUsize;

	if (DeviceConnectionStatus::Disconnecting == mCnntStatus)
	{
		if (OYM_SUCCESS == mAM->Disconnect(mHandle))
		{
			// if user selected to cancel connection, make it and no notifiction
			mCnntStatus = DeviceConnectionStatus::Disconnecting;
			GF_LOGD("Trying to disconnect as client sent a cancel order.");
			return;
		}
	}
	mCnntStatus = DeviceConnectionStatus::Connected;

	// TODO: notify client.
}

void BLEDevice::onDisconnected(GF_UINT8 reason)
{
	GF_LOGD("Device connected: previous state is: %u, reason is: %u",
		static_cast<int>(mCnntStatus), reason);
	mCnntStatus = DeviceConnectionStatus::Disconnected;
	mHandle = 0;
	mConnInt = 0;
	mSuperTO = 0;
	mSlavelatency = 0;
	mMTUsize = 0;

	// TODO: notify client.
}

GF_RET_CODE BLEDevice::configMtuSize(GF_UINT16 mtuSize)
{
	GF_LOGD(__FUNCTION__);
	OYM_STATUS ret = OYM_FAIL;
	if (mHandle != 0)
	{
		ret = mAM->ConfigMtuSize(mHandle, mtuSize);
	}
	return (ret == OYM_SUCCESS) ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDevice::connectionParameterUpdate(GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max,
	GF_UINT16 slave_latence, GF_UINT16 supervision_timeout)
{
	GF_LOGD(__FUNCTION__);
	OYM_STATUS ret = OYM_FAIL;
	if (mHandle != 0)
	{
		ret = mAM->ConnectionParameterUpdate(mHandle, conn_interval_min, conn_interval_max,
			slave_latence, supervision_timeout);
	}
	return (ret == OYM_SUCCESS) ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDevice::WriteCharacteristic(GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data)
{
	GF_LOGD(__FUNCTION__);
	OYM_STATUS ret = OYM_FAIL;
	if (mHandle != 0)
	{
		ret = mAM->WriteCharacteristic(mHandle, attribute_handle, data_length, data);
	}
	return (ret == OYM_SUCCESS) ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDevice::ReadCharacteristic(GF_UINT16 attribute_handle)
{
	GF_LOGD(__FUNCTION__);
	OYM_STATUS ret = OYM_FAIL;
	if (mHandle != 0)
	{
		ret = mAM->ReadCharacteristic(mHandle, attribute_handle);
	}
	return (ret == OYM_SUCCESS) ? GF_SUCCESS : GF_ERROR;
}

void BLEDevice::onMTUSizeChanged(GF_STATUS status, GF_UINT16 mtu_size)
{
	GF_LOGD("%s: status: %u, new MTU size: %u", __FUNCTION__, status, mtu_size);
	mMTUsize = mtu_size;
	// TODO: notify client
}

void BLEDevice::onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency)
{
	GF_LOGD(__FUNCTION__);
	GF_LOGD("status: %u", status);
	GF_LOGD("conn_int: %u", conn_int);
	GF_LOGD("superTO: %u", superTO);
	GF_LOGD("slavelatency: %u", slavelatency);
	mConnInt = conn_int;
	mSuperTO = superTO;
	mSlavelatency = slavelatency;
	// TODO: notify client
}

void BLEDevice::onChracteristicValueRead(GF_STATUS status, GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD(__FUNCTION__);
	GF_LOGD("status: ", status);
	GF_LOGD("length: ", length);
	GF_LOGD("&data: ", (int)data);
	// TODO: notify client
}
