#include "BLEDevice.h"
#include "utils.h"

using namespace oym;


GF_RET_CODE BLEDevice::registerListener(const gfwPtr<DeviceListener>& listener)
{
	tcout << __FUNCTION__ << endl;
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

	tcout << "New device: address type: " << mAddrType << ", address: " << getAddress() << ", name: " << mName << endl;
}

GF_RET_CODE BLEDevice::identifyDevice(int msec)
{
	tcout << __FUNCTION__ << _T(": Opps, not implemented yet. ") << getName().c_str() << endl;
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
	char str[sizeof(mAddress)*(3)+1];
	for (int i = 0; i < sizeof(mAddress); ++i)
	{
		sprintf_s(str+(3*i), sizeof(str)-(3*i), "%2.2X:", mAddress[i]);
	}
	str[sizeof(str)-2] = '\0';
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
		tcout << __FUNCTION__ << _T(": failed because two objects are not the same device.")
			<< _T("\nself--> addr is ") << mAddrType << _T(": ") << getAddress().c_str()
			<< _T("\nold--> addr is ") << from.mAddrType << _T(": ") << from.getAddress().c_str() << endl;
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
	tcout << __FUNCTION__ << endl;
	mPosition = pos;
	return GF_SUCCESS;
}

GF_RET_CODE BLEDevice::connect(bool directConn)
{
	tcout << __FUNCTION__ << endl;
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
	tcout << __FUNCTION__ << endl;
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
	tcout << __FUNCTION__ << endl;
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
			tcout << "Connection cancelling..." << endl;
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
		tcout << "Update device name:: " << mName << endl;
	}
	mRssi = bleDev.rssi;

	tcout << "Update device rssi: " << mRssi << endl;

	// TODO: notify client.
}

void BLEDevice::onConnected(const GF_ConnectedDevice& connedDevice)
{
	tcout << "Device connected: previous state is: " << static_cast<int>(mCnntStatus) << endl;
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
			tcout << "Trying to disconnect as client sent a cancel order." << endl;
			return;
		}
	}
	mCnntStatus = DeviceConnectionStatus::Connected;

	// TODO: notify client.
}
	
void BLEDevice::onDisconnected(GF_UINT8 reason)
{
	tcout << "Device connected: previous state is: " << static_cast<int>(mCnntStatus)
		<< ", reason is: " << reason << endl;
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
	tcout << __FUNCTION__ << endl;
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
	tcout << __FUNCTION__ << endl;
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
	tcout << __FUNCTION__ << endl;
	OYM_STATUS ret = OYM_FAIL;
	if (mHandle != 0)
	{
		ret = mAM->WriteCharacteristic(mHandle, attribute_handle, data_length, data);
	}
	return (ret == OYM_SUCCESS) ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDevice::ReadCharacteristic(GF_UINT16 attribute_handle)
{
	tcout << __FUNCTION__ << endl;
	OYM_STATUS ret = OYM_FAIL;
	if (mHandle != 0)
	{
		ret = mAM->ReadCharacteristic(mHandle, attribute_handle);
	}
	return (ret == OYM_SUCCESS) ? GF_SUCCESS : GF_ERROR;
}

void BLEDevice::onMTUSizeChanged(GF_STATUS status, GF_UINT16 mtu_size)
{
	tcout << __FUNCTION__ << ": status: " << status << ", new MTU size: " << mtu_size << endl;
	mMTUsize = mtu_size;
	// TODO: notify client
}

void BLEDevice::onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency)
{
	tcout << __FUNCTION__ << endl;
	tcout << "status: " << status << endl;
	tcout << "conn_int: " << conn_int << endl;
	tcout << "superTO: " << superTO << endl;
	tcout << "slavelatency: " << slavelatency << endl;
	mConnInt = conn_int;
	mSuperTO = superTO;
	mSlavelatency = slavelatency;
	// TODO: notify client
}

void BLEDevice::onChracteristicValueRead(GF_STATUS status, GF_UINT8 length, GF_PUINT8 data)
{
	tcout << __FUNCTION__ << endl;
	tcout << "status: " << status << endl;
	tcout << "length: " << length << endl;
	tcout << "&data: " << (int)data << endl;
	// TODO: notify client
}
