#include "LogUtils.h"
#include "BLEDongle.h"
#include "DongleListener.h"

using namespace oym;

BLEDongle::BLEDongle(const tstring& sIdentifier)
{
	GF_LOGD("BLEDongle++: identfiler: %s", utils::tostring(sIdentifier).c_str());
}


BLEDongle::~BLEDongle()
{
	mAM = nullptr;
	GF_LOGD("BLEDongle--");
}

// module management
GF_RET_CODE BLEDongle::init(GF_UINT8 comPort)
{
	GF_RET_CODE ret = GF_SUCCESS;
	GF_LOGD(__FUNCTION__);

	if (nullptr != mAM) {
		return ret;
	}
	mAM = make_shared<OYM_AdapterManager>();
	if (nullptr == mAM) {
		GF_LOGD("Memory insurfficient.");
		return GF_ERR_NO_RESOURCE;
	}
	OYM_STATUS result = OYM_FAIL;
	if (OYM_SUCCESS != (result = mAM->Init(comPort))) {
		GF_LOGD("Adapter init failed.");
		mAM = nullptr;
		// TODO: check return value
		return GF_ERROR;
	}

	mAM->RegisterClientCallback(this);

	return ret;
}

GF_RET_CODE BLEDongle::deinit()
{
	GF_RET_CODE ret = GF_SUCCESS;
	GF_LOGD(__FUNCTION__);

	for (auto& itor : mDisconnDevices)
	{
		if (DeviceConnectionStatus::Disconnected != itor->getConnectionStatus())
			itor->disconnect();
	}
	mDisconnDevices.clear();

	for (auto& itor : mConnectedDevices)
	{
		if (DeviceConnectionStatus::Disconnected != itor->getConnectionStatus())
			itor->disconnect();
	}
	mConnectedDevices.clear();

	if (nullptr == mAM) {
		return ret;
	}
	OYM_STATUS result = OYM_FAIL;
	if (OYM_SUCCESS != (result = mAM->Deinit())) {
		GF_LOGE("Adapter deinit failed.");
	}
	mAM->UnregisterClientCallback();
	mAM = nullptr;

	return ret;
}

DongleState BLEDongle::getStatus() const
{
	DongleState ret = DongleState::Unknown;

	if (nullptr == mAM) {
		return ret;
	}
	switch (mAM->GetDongleState())
	{
	case State_Idle:
		ret = DongleState::Idle;
		break;
	case State_Scanning:
		ret = DongleState::Scanning;
		break;
	case State_Connecting:
		ret = DongleState::Connecting;
		break;
	default:
		ret = DongleState::Unknown;
	}

	return ret;
}

tstring BLEDongle::getDescString() const
{
	return tstring(_T("The first version of hw and sw."));
}

// setup listener
GF_RET_CODE BLEDongle::registerListener(const gfwPtr<DongleListener>& listener)
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == listener.lock())
		return GF_ERROR_BAD_PARAM;

	mListeners.insert(listener);
	cleanInvalidWeakP(mListeners);
	return GF_SUCCESS;
}

GF_RET_CODE BLEDongle::unRegisterListener(const gfwPtr<DongleListener>& listener)
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == listener.lock())
		return GF_ERROR_BAD_PARAM;

	mListeners.erase(listener);
	cleanInvalidWeakP(mListeners);
	return GF_SUCCESS;
}

GF_RET_CODE BLEDongle::startScan(OYM_UINT8 rssiThreshold)
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == mAM)
		return GF_ERROR_BAD_STATE;

	cleanInvalidWeakP(mListeners);
	OYM_STATUS ret = mAM->StartScan(rssiThreshold);

	// TODO: thread safe
	if (OYM_SUCCESS == ret)
	{
		// if scan started, remove all disconnected devices
		for (auto itor = mDisconnDevices.begin(); itor != mDisconnDevices.end();)
		{
			if ((*itor)->getConnectionStatus() == DeviceConnectionStatus::Disconnected)
			{
				// notify client that the device will be erased.
				for (auto& listener : mListeners)
				{
					auto ptr = listener.lock();
					if (nullptr != ptr)
						ptr->onDeviceDiscard(*itor);
				}
				GF_LOGD("Disconnected device removed.");
				mDisconnDevices.erase(itor++);
			}
			else
			{
				itor++;
			}
		}

		return GF_SUCCESS;
	}
	return GF_ERROR;
}

GF_RET_CODE BLEDongle::stopScan()
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == mAM)
		return GF_ERROR_BAD_STATE;

	OYM_STATUS ret = mAM->StopScan();
	if (OYM_SUCCESS == ret)
	{
		GF_LOGD("Scan stopped.");
		for (auto& itor : mListeners)
		{
			auto ptr = itor.lock();
			if (nullptr != ptr)
				// TODO: add cleaning invalid gfwPtr from list in somewhere
				ptr->onScanfinished();
		}
	}
	return GF_SUCCESS;
}

void BLEDongle::enumDevices(FunEnumDevice funEnum, bool bConnectedOnly)
{
	GF_LOGD(__FUNCTION__);

	for (auto& itor : mConnectedDevices)
	{
		funEnum(dynamic_pointer_cast<Device>(itor));
	}
	if (bConnectedOnly)
		return;

	for (auto& itor : mDisconnDevices)
	{
		funEnum(dynamic_pointer_cast<Device>(itor));
	}
}

WPDEVICE BLEDongle::findDevice(OYM_UINT8 addrType, tstring address)
{
	GF_LOGD(__FUNCTION__);
	gfsPtr<Device> ret;
	for (auto& itor : mConnectedDevices)
	{
		if (itor->isMyself(addrType, address))
			ret = dynamic_pointer_cast<Device>(itor);
	}
	if (nullptr != ret)
		return ret;
	for (auto& itor : mDisconnDevices)
	{
		if (itor->isMyself(addrType, address))
			ret = dynamic_pointer_cast<Device>(itor);
	}
	return ret;
}

GF_RET_CODE BLEDongle::createVirtualDevice(int numDevices, vector<WPDEVICE> realDevices, WPDEVICE& newDevice)
{
	numDevices;
	realDevices;
	newDevice;
	GF_LOGD(__FUNCTION__);
	return GF_ERROR_NOT_SUPPORT; // not implemented
}

void BLEDongle::onScanResult(BLE_DEVICE* device)
{
	GF_LOGD(__FUNCTION__);
	if (device == nullptr)
		return;

	// newly scanned should not be connected
	auto newItem = make_shared<BLEDevice>(this, *device);
	pair<decltype(mDisconnDevices.begin()), bool> ret = mDisconnDevices.insert(newItem);
	auto exists = mDisconnDevices.find(newItem);
	if (ret.second == false)
	{
		(*exists)->updateData(*device);
	}
	for (auto& itor : mListeners)
	{
		auto ptr = itor.lock();
		if (nullptr != ptr)
			ptr->onDeviceFound(*exists);
	}
}

void BLEDongle::onScanFinished()
{
	GF_LOGD(__FUNCTION__);
	for (auto& itor : mListeners)
	{
		auto ptr = itor.lock();
		if (nullptr != ptr)
			ptr->onScanfinished();
	}
}

void BLEDongle::onDeviceConnected(OYM_STATUS status, GF_ConnectedDevice *device)
{
	status;
	GF_LOGD(__FUNCTION__);

	gfsPtr<BLEDevice> dev;
	for (auto& itor : mDisconnDevices)
	{
		if (itor->isMyself(device->address_type, device->address))
		{
			dev = itor;
			mDisconnDevices.erase(itor);
			break;
		}
	}
	if (nullptr == dev)
	{
		GF_LOGW("No device found in disconnect_list. %u:%s",
			(GF_UINT)device->address_type,
			utils::tostring(utils::deviceAddressToString(device->address, sizeof(device->address)).c_str()));
		for (auto& itor : mConnectedDevices)
		{
			if (itor->isMyself(device->address_type, device->address))
			{
				dev = itor;
				break;
			}
		}
	}
	if (nullptr == dev)
	{
		GF_LOGW("No device found in connect_list either. %u:%s",
			(GF_UINT)device->address_type,
			utils::tostring(utils::deviceAddressToString(device->address, sizeof(device->address)).c_str()));
		return;
	}
	mConnectedDevices.insert(dev);
	dev->onConnected(*device);
	// TODO: when a device is found, no later than it is connected, need to find
	// it's detail information, then specialize it.
}

void BLEDongle::onDeviceDisonnected(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT8 reason)
{
	status;
	GF_LOGD(__FUNCTION__);

	gfsPtr<BLEDevice> dev;
	for (auto& itor : mConnectedDevices)
	{
		if (handle == itor->getHandle())
		{
			dev = itor;
			mConnectedDevices.erase(itor);
			break;
		}
	}
	if (nullptr == dev)
	{
		GF_LOGW("No device found in connect_list. handle is %u, reason is %u",
			(GF_UINT)handle, (GF_UINT)reason);
		for (auto& itor : mDisconnDevices)
		{
			if (handle == itor->getHandle())
			{
				dev = itor;
				break;
			}
		}
	}
	if (nullptr == dev)
	{
		GF_LOGW("No device found in disconnect_list either. handle is %u, reason is %u",
			(GF_UINT)handle, (GF_UINT)reason);
		return;
	}
	mDisconnDevices.insert(dev);
	dev->onDisconnected(reason);
}


void BLEDongle::onMTUSizeChanged(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 mtu_size)
{
	GF_LOGD(__FUNCTION__);

	for (auto& itor : mConnectedDevices)
	{
		if (itor->getHandle() == handle)
		{
			itor->onMTUSizeChanged(status, mtu_size);
		}
	}
}

void BLEDongle::onConnectionParmeterUpdated(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 conn_int, OYM_UINT16 superTO, OYM_UINT16 slavelatency)
{
	GF_LOGD(__FUNCTION__);

	for (auto& itor : mConnectedDevices)
	{
		if (itor->getHandle() == handle)
		{
			itor->onConnectionParmeterUpdated(status, conn_int, superTO, slavelatency);
		}
	}
}

void BLEDongle::onChracteristicValueRead(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT8 length, OYM_PUINT8 data)
{
	GF_LOGD(__FUNCTION__);

	for (auto& itor : mConnectedDevices)
	{
		if (itor->getHandle() == handle)
		{
			itor->onChracteristicValueRead(status, length, data);
		}
	}
}

/*Notification format: data length(1 byte N) + data(N Bytes)*/
void BLEDongle::onNotificationReceived(OYM_UINT16 handle, OYM_UINT8 length, OYM_PUINT8 data)
{
	handle; length; data;
	//GF_LOGD(__FUNCTION__);
}

void BLEDongle::onComDestory()
{
	GF_LOGD(__FUNCTION__);
	// TODO: do something after com destoryed

	for (auto& itor : mListeners)
	{
		auto ptr = itor.lock();
		if (nullptr != ptr)
			ptr->onStateChanged(DongleState::Unknown);
	}
}


GF_RET_CODE BLEDongle::connect(BLEDevice& dev, bool directConn)
{
	GF_RET_CODE ret = GF_SUCCESS;
	if (nullptr == mAM)
		return GF_ERROR_BAD_STATE;
	GF_UINT8 addr[BT_ADDRESS_SIZE];
	ret = dev.getAddress(addr, sizeof(addr));
	if (GF_SUCCESS != ret)
		return ret;
	OYM_STATUS status = mAM->Connect(addr, dev.getAddrType(),
		directConn ? OYM_TRUE : OYM_FALSE);
	return status == OYM_SUCCESS ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDongle::cancelConnect(BLEDevice& dev)
{
	GF_RET_CODE ret = GF_SUCCESS;
	if (nullptr == mAM)
		return GF_ERROR_BAD_STATE;
	GF_UINT8 addr[BT_ADDRESS_SIZE];
	ret = dev.getAddress(addr, sizeof(addr));
	if (GF_SUCCESS != ret)
		return ret;
	OYM_STATUS status = mAM->CancelConnect(addr, dev.getAddrType());
	return status == OYM_SUCCESS ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDongle::disconnect(BLEDevice& dev)
{
	if (nullptr == mAM || INVALID_HANDLE == dev.getHandle())
		return GF_ERROR_BAD_STATE;
	OYM_STATUS status = mAM->Disconnect(dev.getHandle());
	return status == OYM_SUCCESS ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDongle::configMtuSize(BLEDevice& dev, GF_UINT16 mtuSize)
{
	if (nullptr == mAM || INVALID_HANDLE == dev.getHandle())
		return GF_ERROR_BAD_STATE;
	OYM_STATUS status = mAM->ConfigMtuSize(dev.getHandle(), mtuSize);
	return status == OYM_SUCCESS ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDongle::connectionParameterUpdate(BLEDevice& dev, GF_UINT16 conn_interval_min,
	GF_UINT16 conn_interval_max, GF_UINT16 slave_latence, GF_UINT16 supervision_timeout)
{
	if (nullptr == mAM || INVALID_HANDLE == dev.getHandle())
		return GF_ERROR_BAD_STATE;
	OYM_STATUS status = mAM->ConnectionParameterUpdate(dev.getHandle(), conn_interval_min,
		conn_interval_max, slave_latence, supervision_timeout);
	return status == OYM_SUCCESS ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDongle::writeCharacteristic(BLEDevice& dev,
	GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data)
{
	if (nullptr == mAM || INVALID_HANDLE == dev.getHandle())
		return GF_ERROR_BAD_STATE;
	OYM_STATUS status = mAM->WriteCharacteristic(dev.getHandle(), attribute_handle,
		data_length, data);
	return status == OYM_SUCCESS ? GF_SUCCESS : GF_ERROR;
}

GF_RET_CODE BLEDongle::readCharacteristic(BLEDevice& dev, GF_UINT16 attribute_handle)
{
	if (nullptr == mAM || INVALID_HANDLE == dev.getHandle())
		return GF_ERROR_BAD_STATE;
	OYM_STATUS status = mAM->ReadCharacteristic(dev.getHandle(), attribute_handle);
	return status == OYM_SUCCESS ? GF_SUCCESS : GF_ERROR;
}


