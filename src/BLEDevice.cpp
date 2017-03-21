#include "LogUtils.h"
#include "BLEDevice.h"
#include "Utils.h"
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

GF_RET_CODE BLEDevice::unRegisterListener(const gfwPtr<DeviceListener>& listener)
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == listener.lock())
		return GF_ERROR_BAD_PARAM;

	mListeners.erase(listener);
	cleanInvalidWeakP(mListeners);
	return GF_SUCCESS;
}

BLEDevice::BLEDevice(IAdapter* adapter, const BLE_DEVICE& bleDev)
	: mAdapter(adapter)
	, mAddrType(bleDev.addr_type)
	, mRssi(bleDev.rssi)
{
	ASSERT_VALID_PTR(mAdapter);
	memcpy(mAddress, bleDev.addr, sizeof(mAddress));

	GF_CHAR name[BLE_DEVICE_NAME_LENGTH + 1] = "";
	memcpy(name, bleDev.dev_name, sizeof(bleDev.dev_name));
	name[BLE_DEVICE_NAME_LENGTH] = '\0';
	mName = utils::totstring(string(name));

	GF_LOGD("New device: address type: %u, address: %s, name: %s", (GF_UINT)mAddrType,
		utils::tostring(getAddress()).c_str(), utils::tostring(mName).c_str());
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
	return utils::deviceAddressToString(mAddress, sizeof(mAddress));
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

bool BLEDevice::isMyself(GF_UINT8 addrType, tstring addr) const
{
	return ((addrType == mAddrType && getAddress() == addr) ?
		true : false);
}

bool BLEDevice::takeover(BLEDevice& from)
{
	if ((mAddrType != from.mAddrType) ||
		memcmp(mAddress, from.mAddress, sizeof(mAddress)))
	{
		GF_LOGD("%s: failed because two objects are not the same device.\nself--> addr is %u:%s.\nold--> addr is :%u:%s",
			__FUNCTION__, (GF_UINT)mAddrType, utils::tostring(getAddress()).c_str(),
			(GF_UINT)from.mAddrType, utils::tostring(from.getAddress()).c_str());
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
		if (OYM_SUCCESS == mAdapter->connect(*this, directConn))
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
		if (OYM_SUCCESS == mAdapter->disconnect(*this))
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
		if (OYM_SUCCESS == mAdapter->cancelConnect(*this))
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
	tstring newName = utils::totstring(string(name));
	if (newName != mName)
	{
		GF_LOGD("Update device name: %s", utils::tostring(mName).c_str());
	}
	mRssi = bleDev.rssi;

	GF_LOGD("Update device rssi: %u", (GF_UINT)mRssi);

	// TODO: notify client.
}

void BLEDevice::onConnected(const GF_ConnectedDevice& connedDevice)
{
	GF_LOGD("Device connected: previous state is: %u", static_cast<GF_UINT>(mCnntStatus));
	mHandle = connedDevice.handle;
	mConnInt = connedDevice.conn_int;
	mSuperTO = connedDevice.superTO;
	mSlavelatency = connedDevice.slavelatency;
	mMTUsize = connedDevice.MTUsize;

	if (DeviceConnectionStatus::Disconnecting == mCnntStatus)
	{
		if (OYM_SUCCESS == mAdapter->disconnect(*this))
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
		static_cast<GF_UINT>(mCnntStatus), (GF_UINT)reason);
	mCnntStatus = DeviceConnectionStatus::Disconnected;
	mHandle = INVALID_HANDLE;
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
	if (mHandle != INVALID_HANDLE)
	{
		ret = mAdapter->configMtuSize(*this, mtuSize);
	}
	return (ret == OYM_SUCCESS) ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDevice::connectionParameterUpdate(GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max,
	GF_UINT16 slave_latence, GF_UINT16 supervision_timeout)
{
	GF_LOGD(__FUNCTION__);
	OYM_STATUS ret = OYM_FAIL;
	if (mHandle != INVALID_HANDLE)
	{
		ret = mAdapter->connectionParameterUpdate(*this, conn_interval_min, conn_interval_max,
			slave_latence, supervision_timeout);
	}
	return (ret == OYM_SUCCESS) ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDevice::writeCharacteristic(AttributeHandle attribute_handle, GF_UINT8 dataLen, GF_PUINT8 data)
{
	GF_LOGD(__FUNCTION__);
	OYM_STATUS ret = OYM_FAIL;
	if (mHandle != INVALID_HANDLE)
	{
		ret = mAdapter->writeCharacteristic(*this, attribute_handle, dataLen, data);
	}
	return (ret == OYM_SUCCESS) ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDevice::readCharacteristic(AttributeHandle attribute_handle)
{
	GF_LOGD(__FUNCTION__);
	OYM_STATUS ret = OYM_FAIL;
	if (mHandle != INVALID_HANDLE)
	{
		ret = mAdapter->readCharacteristic(*this, attribute_handle);
	}
	return (ret == OYM_SUCCESS) ? GF_SUCCESS : GF_ERROR;
}

void BLEDevice::onMTUSizeChanged(GF_STATUS status, GF_UINT16 mtu_size)
{
	GF_LOGD("%s: status: %u, new MTU size: %u", __FUNCTION__, (GF_UINT)status, (GF_UINT)mtu_size);
	mMTUsize = mtu_size;
	// TODO: notify client
}

void BLEDevice::onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency)
{
	GF_LOGD(__FUNCTION__);
	GF_LOGD("status: %u", (GF_UINT)status);
	GF_LOGD("conn_int: %u", (GF_UINT)conn_int);
	GF_LOGD("superTO: %u", (GF_UINT)superTO);
	GF_LOGD("slavelatency: %u", (GF_UINT)slavelatency);
	mConnInt = conn_int;
	mSuperTO = superTO;
	mSlavelatency = slavelatency;
	// TODO: notify client
}

void BLEDevice::onCharacteristicValueRead(GF_STATUS status, GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD(__FUNCTION__);
	GF_LOGD("status: %u", (GF_UINT)status);
	GF_LOGD("length: %u", (GF_UINT)length);
	GF_LOGD("&data: %p", data);
	// TODO: notify client
}

void BLEDevice::onData(GF_UINT8 length, GF_PUINT8 data)
{
	length; data;
	// nothing to do
}
