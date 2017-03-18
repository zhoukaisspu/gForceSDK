#include "LogUtils.h"
#include "BLEDongle.h"
#include "DongleListener.h"

using namespace oym;

#define BLE_COMM 3

BLEDongle::BLEDongle(const tstring& sIdentifier)
{
	GF_LOGD("BLEDongle++: identfiler: %s", tostring(sIdentifier).c_str());
}


BLEDongle::~BLEDongle()
{
	mAM = nullptr;
	GF_LOGD("BLEDongle--");
}

// module management
GF_RET_CODE BLEDongle::init()
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
	if (OYM_SUCCESS != (result = mAM->Init(BLE_COMM))) {
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

	for (auto& itor : mDevices)
	{
		if (DeviceConnectionStatus::Disconnected != itor->getConnectionStatus())
			itor->disconnect();
	}
	mDevices.clear();

	if (nullptr == mAM) {
		return ret;
	}
	OYM_STATUS result = OYM_FAIL;
	if (OYM_SUCCESS != (result = mAM->Deinit())) {
		GF_LOGD("Adapter deinit failed.");
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

GF_RET_CODE BLEDongle::startScan(OYM_UINT8 rssiThreshold)
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == mAM)
		return GF_ERROR_BAD_STATE;

	OYM_STATUS ret = mAM->StartScan(rssiThreshold);

	// TODO: thread safe
	if (OYM_SUCCESS == ret)
	{
		// if scan started, remove all disconnected devices
		for (auto itor = mDevices.begin(); itor != mDevices.end();)
		{
			if ((*itor)->getConnectionStatus() == DeviceConnectionStatus::Disconnected)
			{
				// TODO: notify client that the device will be erased.

				GF_LOGD("Disconnected device removed.");
				mDevices.erase(itor++);
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

	for (auto& itor : mDevices)
	{
		if (bConnectedOnly &&
			DeviceConnectionStatus::Connected != itor->getConnectionStatus())
			continue;
		funEnum(dynamic_pointer_cast<Device>(itor));
	}
}

int BLEDongle::getNumOfConnectedDevices() const
{
	GF_LOGD(__FUNCTION__);
	int cnt = 0;
	for (auto& itor : mDevices)
	{
		if (DeviceConnectionStatus::Connected == itor->getConnectionStatus())
			++cnt;
	}
	return cnt;
}

WPDEVICE BLEDongle::findDevice(OYM_UINT8 addrType, tstring address)
{
	GF_LOGD(__FUNCTION__);
	WPDEVICE ret;
	for (auto& itor : mDevices)
	{
		if (itor->getAddrType() == addrType && itor->getAddress() == address)
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

	auto newItem = make_shared<BLEDevice>(mAM, *device);
	pair<SET_DEVICE::iterator, bool> ret = mDevices.insert(newItem);
	auto exists = mDevices.find(newItem);
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
	cleanInvalidWeakP(mListeners);
}

void BLEDongle::onDeviceConnected(OYM_STATUS status, GF_ConnectedDevice *device)
{
	status;
	GF_LOGD(__FUNCTION__);

	for (auto itor : mDevices)
	{
		if (itor->isMyself(device->address_type, device->address))
		{
			itor->onConnected(*device);
		}
	}

	// TODO: when a device is found, no later than it is connected, need to find
	// it's detail information, then specialize it.
}

void BLEDongle::onDeviceDisonnected(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT8 reason)
{
	status;
	GF_LOGD(__FUNCTION__);

	for (auto itor : mDevices)
	{
		if (itor->getHandle() == handle)
		{
			itor->onDisconnected(reason);
		}
	}
}


void BLEDongle::onMTUSizeChanged(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 mtu_size)
{
	GF_LOGD(__FUNCTION__);

	for (auto itor : mDevices)
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

	for (auto itor : mDevices)
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

	for (auto itor : mDevices)
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