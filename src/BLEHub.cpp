#include "LogUtils.h"
#include "BLEHub.h"
#include "HubListener.h"
#include "GForceDevice.h"
#include "Utils.h"

using namespace gf;

BLEHub::BLEHub(const tstring& sIdentifier)
{
	GF_LOGD("BLEHub++: identfiler: %s", utils::tostring(sIdentifier).c_str());
}


BLEHub::~BLEHub()
{
	mAM = nullptr;
	GF_LOGD("BLEHub--");
}

GF_UINT32 BLEHub::executeCommand(gfsPtr<HubMsg> msg)
{
	msg->executed = false;
	unique_lock<mutex> lock(mTaskMutex);
	mMsgQ.push(msg);
	msg->syncCallCond.wait(lock, [&msg]()->bool{ return msg->executed; });
	return msg->ret;
}
void BLEHub::commandTask()
{
	while (true)
	{
		auto msg = mMsgQ.pop();
		if (nullptr == msg)
		{
			GF_LOGD("Wake me up and exit!");
			break;
		}
				{
					lock_guard<mutex> lock(mTaskMutex);
					msg->ret = msg->fun();
				}
				GF_LOGD("Msg received and executed: ret = %u", msg->ret);
				msg->executed = true;
				msg->syncCallCond.notify_all();
	}
}

// module management
GF_RET_CODE BLEHub::init(GF_UINT8 comPort)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_SUCCESS;
	GF_LOGD(__FUNCTION__);

	lock_guard<mutex> lock(const_cast<mutex&>(mTaskMutex));
	if (nullptr != mAM) {
		return ret;
	}
	mAM = GF_CAdapterManagerInterface::GetInstance();
	if (nullptr == mAM) {
		GF_LOGD("Memory insurfficient.");
		return GF_RET_CODE::GF_ERR_NO_RESOURCE;
	}
	if (GF_OK != mAM->Init(comPort)) {
		GF_LOGD("Hub init failed.");
		mAM = nullptr;
		// TODO: check return value for reason
		return GF_RET_CODE::GF_ERROR;
	}

	mAM->RegisterClientCallback(this);

	mCommandThread = thread(std::mem_fn(&BLEHub::commandTask), this);

	return ret;
}

GF_RET_CODE BLEHub::deinit()
{
	GF_RET_CODE ret = GF_RET_CODE::GF_SUCCESS;
	GF_LOGD(__FUNCTION__);

	// get dongle status and stop scan
	if (HubState::Scanning == getStatus())
	{
		stopScan();
	}

	unique_lock<mutex> lock(mTaskMutex);
	auto disconDevices = mDisconnDevices;
	auto connedDevices = mConnectedDevices;
	lock.unlock();
	for (auto& itor : disconDevices)
	{
		if (DeviceConnectionStatus::Disconnected != itor->getConnectionStatus())
			itor->disconnect();
	}
	for (auto& itor : connedDevices)
	{
		if (DeviceConnectionStatus::Disconnected != itor->getConnectionStatus())
			itor->disconnect();
	}
	// send empty message to exit command thread
	gfsPtr<HubMsg> emptyEvent;
	mMsgQ.push(emptyEvent);
	if (mCommandThread.joinable())
	{
		mCommandThread.join();
	}

	lock.lock();
	mMsgQ.clear();
	mDisconnDevices.clear();
	mConnectedDevices.clear();

	if (nullptr == mAM) {
		return ret;
	}
	if (GF_OK != mAM->Deinit()) {
		GF_LOGE("Hub deinit failed.");
	}
	mAM->UnregisterClientCallback();
	mAM = nullptr;

	return ret;
}

WorkMode BLEHub::setWorkMode(WorkMode newMode)
{
	lock_guard<mutex> lock(mTaskMutex);
	auto oldMode = mWorkMode;
	mWorkMode = newMode;
	return oldMode;
}

HubState BLEHub::getStatus()
{
	HubState ret = HubState::Unknown;
	if (nullptr == mAM) {
		return ret;
	}
	auto am = mAM;
	GF_HubState state = static_cast<GF_HubState>(executeCommand(make_shared<HubMsg>([&am]()
	{
		return static_cast<GF_UINT32>(am->GetHubState());
	})));

	switch (state)
	{
	case State_Idle:
		ret = HubState::Idle;
		break;
	case State_Scanning:
		ret = HubState::Scanning;
		break;
	case State_Connecting:
		ret = HubState::Connecting;
		break;
	default:
		ret = HubState::Unknown;
	}

	return ret;
}

tstring BLEHub::getDescString() const
{
	return tstring(_T("The first version of hw and sw."));
}

// setup listener
GF_RET_CODE BLEHub::registerListener(const gfwPtr<HubListener>& listener)
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == listener.lock())
		return GF_RET_CODE::GF_ERROR_BAD_PARAM;

	lock_guard<mutex> lock(mTaskMutex);
	mListeners.insert(listener);
	cleanInvalidWeakP(mListeners);
	return GF_RET_CODE::GF_SUCCESS;
}

GF_RET_CODE BLEHub::unRegisterListener(const gfwPtr<HubListener>& listener)
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == listener.lock())
		return GF_RET_CODE::GF_ERROR_BAD_PARAM;

	lock_guard<mutex> lock(mTaskMutex);
	mListeners.erase(listener);
	cleanInvalidWeakP(mListeners);
	return GF_RET_CODE::GF_SUCCESS;
}

GF_RET_CODE BLEHub::startScan(GF_UINT8 rssiThreshold)
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == mAM)
		return GF_RET_CODE::GF_ERROR_BAD_STATE;

	auto am = mAM;
	decltype(mListeners)& ls = mListeners;
	decltype(mDisconnDevices)& dd = mDisconnDevices;
	GF_UINT32 ret = static_cast<GF_UINT32>(executeCommand(make_shared<HubMsg>([&am, &ls, &dd, rssiThreshold]()
	{
		cleanInvalidWeakP(ls);
		GF_UINT32 ret = static_cast<GF_UINT32>(am->StartScan(rssiThreshold));
		if (GF_OK == ret)
		{
			// if scan started, remove all disconnected devices
			for (auto itor = dd.begin(); itor != dd.end();)
			{
				if ((*itor)->getConnectionStatus() == DeviceConnectionStatus::Disconnected)
				{
					// notify client that the device will be erased.
					for (auto& listener : ls)
					{
						auto ptr = listener.lock();
						if (nullptr != ptr)
							ptr->onDeviceDiscard(*itor);
					}
					GF_LOGD("Disconnected device removed.");
					dd.erase(itor++);
				}
				else
				{
					itor++;
				}
			}
		}
		return ret;
	})));
	if (GF_OK == ret)
		return GF_RET_CODE::GF_SUCCESS;
	else
		return GF_RET_CODE::GF_ERROR;
}

GF_RET_CODE BLEHub::stopScan()
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == mAM)
		return GF_RET_CODE::GF_ERROR_BAD_STATE;

	auto am = mAM;
	decltype(mListeners)& ls = mListeners;
	GF_UINT32 ret = static_cast<GF_UINT32>(executeCommand(make_shared<HubMsg>([&am, &ls]()
	{
		GF_UINT32 ret = static_cast<GF_UINT32>(am->StopScan());
		if (GF_OK == ret)
		{
			cleanInvalidWeakP(ls);
			GF_LOGD("Scan stopped.");
			for (auto& itor : ls)
			{
				auto ptr = itor.lock();
				if (nullptr != ptr)
					ptr->onScanfinished();
			}
		}
		return ret;
	})));
	if (GF_OK == ret)
		return GF_RET_CODE::GF_SUCCESS;
	else
		return GF_RET_CODE::GF_ERROR;
}

void BLEHub::enumDevices(FunEnumDevice funEnum, bool bConnectedOnly)
{
	GF_LOGD(__FUNCTION__);

	lock_guard<mutex> lock(mTaskMutex);
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

WPDEVICE BLEHub::findDevice(GF_UINT8 addrType, tstring address)
{
	GF_LOGD(__FUNCTION__);
	gfsPtr<Device> ret;

	lock_guard<mutex> lock(mTaskMutex);
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

GF_RET_CODE BLEHub::createVirtualDevice(int numDevices, vector<WPDEVICE> realDevices, WPDEVICE& newDevice)
{
	numDevices;
	realDevices;
	newDevice;
	GF_LOGD(__FUNCTION__);
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; // not implemented
}

void BLEHub::onScanResult(GF_BLEDevice* device)
{
	GF_LOGD(__FUNCTION__);
	if (device == nullptr)
		return;

	// newly scanned should not be connected
	auto newItem = createDeviceBeforeConnect(*device);
	if (nullptr == newItem)
		return;

	lock_guard<mutex> lock(mTaskMutex);

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

void BLEHub::onScanFinished()
{
	GF_LOGD(__FUNCTION__);
	lock_guard<mutex> lock(mTaskMutex);
	for (auto& itor : mListeners)
	{
		auto ptr = itor.lock();
		if (nullptr != ptr)
			ptr->onScanfinished();
	}
}

void BLEHub::onDeviceConnected(GF_STATUS status, GF_ConnectedDevice *device)
{
	GF_LOGD(__FUNCTION__);
	ASSERT_VALID_PTR(device);
	if (nullptr == device)
		return;

	lock_guard<mutex> lock(mTaskMutex);
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

		// TODO: add new device using GF_ConnectedDevice
		// need more info to do it
		return;
	}
	mConnectedDevices.insert(dev);
	dev->onConnected(status, *device);
	// TODO: when a device is found, no later than it is connected, need to find
	// it's detail information, then specialize it.
	/*
	for (GF_UINT16 i = static_cast<GF_UINT16>(AttributeHandle::GATTPrimServiceDeclaration1);
	i < static_cast<GF_UINT16>(AttributeHandle::Max); ++i)
	{
	dev->readCharacteristic(static_cast<AttributeHandle>(i));
	}
	*/
}

void BLEHub::onDeviceDisonnected(GF_STATUS status, GF_ConnectedDevice *device, GF_UINT8 reason)
{
	status;
	GF_LOGD(__FUNCTION__);
	ASSERT_VALID_PTR(device);
	if (nullptr == device)
		return;

	lock_guard<mutex> lock(mTaskMutex);
	gfsPtr<BLEDevice> dev;
	for (auto& itor : mConnectedDevices)
	{
		if (device->handle == itor->getHandle())
		{
			dev = itor;
			mConnectedDevices.erase(itor);
			break;
		}
	}
	if (nullptr == dev)
	{
		GF_LOGW("No device found in connect_list. handle is %u, %u:%s, reason is %u",
			(GF_UINT)device->handle, (GF_UINT)device->address_type,
			utils::tostring(utils::deviceAddressToString(device->address, sizeof(device->address))).c_str(),
			(GF_UINT)reason);
		for (auto& itor : mDisconnDevices)
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
		GF_LOGW("No device found in disconnect_list. either handle is %u, %u:%s, reason is %u",
			(GF_UINT)device->handle, (GF_UINT)device->address_type,
			utils::tostring(utils::deviceAddressToString(device->address, sizeof(device->address))).c_str(),
			(GF_UINT)reason);
		return;
	}
	mDisconnDevices.insert(dev);
	dev->onDisconnected(status, reason);
}


void BLEHub::onMTUSizeChanged(GF_STATUS status, GF_UINT16 handle, GF_UINT16 mtu_size)
{
	GF_LOGD(__FUNCTION__);

	lock_guard<mutex> lock(mTaskMutex);
	for (auto& itor : mConnectedDevices)
	{
		if (itor->getHandle() == handle)
		{
			itor->onMTUSizeChanged(status, mtu_size);
		}
	}
}

void BLEHub::onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 handle, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency)
{
	GF_LOGD(__FUNCTION__);

	lock_guard<mutex> lock(mTaskMutex);
	for (auto& itor : mConnectedDevices)
	{
		if (itor->getHandle() == handle)
		{
			itor->onConnectionParmeterUpdated(status, conn_int, superTO, slavelatency);
		}
	}
}

void BLEHub::onCharacteristicValueRead(GF_STATUS status, GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD(__FUNCTION__);

	lock_guard<mutex> lock(mTaskMutex);
	for (auto& itor : mConnectedDevices)
	{
		if (itor->getHandle() == handle)
		{
			itor->onCharacteristicValueRead(status, length, data);
		}
	}
}

#define ATTRIB_HANDLE_GEVENT (0x27)

/*Notification format: data length(1 byte N) + data(N Bytes)*/
void BLEHub::onNotificationReceived(GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data)
{
	// parse data header
	if (length <= 3)
		return;
	GF_UINT16 attrib_handle = data[1] | ((GF_UINT16)data[2] << 8);
	if (ATTRIB_HANDLE_GEVENT != attrib_handle)
	{
		GF_LOGD("%s: not a event type. attrib_handle is %u", attrib_handle);
		return;
	}
	unique_lock<mutex> lock(mTaskMutex);
	gfsPtr<BLEDevice> dev;
	for (auto& itor : mConnectedDevices)
	{
		if (itor->getHandle() == handle)
		{
			dev = itor;
		}
	}
	lock.unlock();
	if (nullptr != dev)
		dev->onData(length - 3, data + 3);
}

void BLEHub::onComDestory()
{
	GF_LOGD(__FUNCTION__);
	// TODO: do something after com destoryed

	lock_guard<mutex> lock(mTaskMutex);
	for (auto& itor : mListeners)
	{
		auto ptr = itor.lock();
		if (nullptr != ptr)
			ptr->onStateChanged(HubState::Unknown);
	}
}


GF_RET_CODE BLEHub::connect(BLEDevice& dev, bool directConn)
{
	if (nullptr == mAM)
		return GF_RET_CODE::GF_ERROR_BAD_STATE;

	GF_UINT8 addr[BT_ADDRESS_SIZE];
	GF_RET_CODE ret = dev.getAddress(addr, sizeof(addr));
	if (GF_RET_CODE::GF_SUCCESS != ret)
		return ret;
	GF_UINT8 type = dev.getAddrType();
	auto am = mAM;
	GF_UINT32 status = static_cast<GF_UINT32>(executeCommand(make_shared<HubMsg>([&am, &addr, type, directConn]()
	{
		GF_STATUS status = am->Connect(addr, type, directConn ? GF_TRUE : GF_FALSE);
		return static_cast<GF_UINT32>(status);
	})));
	if (GF_OK == status)
		return GF_RET_CODE::GF_SUCCESS;
	else
		return GF_RET_CODE::GF_ERROR;
}

GF_RET_CODE BLEHub::cancelConnect(BLEDevice& dev)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_SUCCESS;
	if (nullptr == mAM)
		return GF_RET_CODE::GF_ERROR_BAD_STATE;
	GF_UINT8 addr[BT_ADDRESS_SIZE];
	ret = dev.getAddress(addr, sizeof(addr));
	if (GF_RET_CODE::GF_SUCCESS != ret)
		return ret;
	auto type = dev.getAddrType();
	auto am = mAM;
	GF_UINT32 status = static_cast<GF_UINT32>(executeCommand(make_shared<HubMsg>([&am, &addr, type]()
	{
		GF_STATUS status = am->CancelConnect(addr, type);
		return static_cast<GF_UINT32>(status);
	})));
	if (GF_OK == status)
		return GF_RET_CODE::GF_SUCCESS;
	else
		return GF_RET_CODE::GF_ERROR;
}

GF_RET_CODE BLEHub::disconnect(BLEDevice& dev)
{
	if (nullptr == mAM || INVALID_HANDLE == dev.getHandle())
		return GF_RET_CODE::GF_ERROR_BAD_STATE;

	auto handle = dev.getHandle();
	auto am = mAM;
	GF_UINT32 status = static_cast<GF_UINT32>(executeCommand(make_shared<HubMsg>([&am, handle]()
	{
		GF_STATUS status = am->Disconnect(handle);
		return static_cast<GF_UINT32>(status);
	})));
	if (GF_OK == status)
		return GF_RET_CODE::GF_SUCCESS;
	else
		return GF_RET_CODE::GF_ERROR;
}

GF_RET_CODE BLEHub::configMtuSize(BLEDevice& dev, GF_UINT16 mtuSize)
{
	if (nullptr == mAM || INVALID_HANDLE == dev.getHandle())
		return GF_RET_CODE::GF_ERROR_BAD_STATE;

	auto handle = dev.getHandle();
	auto am = mAM;
	GF_UINT32 status = static_cast<GF_UINT32>(executeCommand(make_shared<HubMsg>([&am, handle, mtuSize]()
	{
		GF_STATUS status = am->ConfigMtuSize(handle, mtuSize);
		return static_cast<GF_UINT32>(status);
	})));
	if (GF_OK == status)
		return GF_RET_CODE::GF_SUCCESS;
	else
		return GF_RET_CODE::GF_ERROR;
}

GF_RET_CODE BLEHub::connectionParameterUpdate(BLEDevice& dev, GF_UINT16 conn_interval_min,
	GF_UINT16 conn_interval_max, GF_UINT16 slave_latence, GF_UINT16 supervision_timeout)
{
	if (nullptr == mAM || INVALID_HANDLE == dev.getHandle())
		return GF_RET_CODE::GF_ERROR_BAD_STATE;

	auto handle = dev.getHandle();
	auto am = mAM;
	GF_UINT32 status = static_cast<GF_UINT32>(executeCommand(make_shared<HubMsg>([&am, handle,
		conn_interval_min, conn_interval_max, slave_latence, supervision_timeout]()
	{
		GF_STATUS status = am->ConnectionParameterUpdate(handle, conn_interval_min,
			conn_interval_max, slave_latence, supervision_timeout);
		return static_cast<GF_UINT32>(status);
	})));
	if (GF_OK == status)
		return GF_RET_CODE::GF_SUCCESS;
	else
		return GF_RET_CODE::GF_ERROR;
}

GF_RET_CODE BLEHub::writeCharacteristic(BLEDevice& dev,
	AttributeHandle attribute_handle, GF_UINT8 data_length, GF_PUINT8 data)
{
	if (nullptr == mAM || INVALID_HANDLE == dev.getHandle())
		return GF_RET_CODE::GF_ERROR_BAD_STATE;

	auto handle = dev.getHandle();
	auto am = mAM;
	GF_UINT32 status = static_cast<GF_UINT32>(executeCommand(make_shared<HubMsg>([&am, handle,
		attribute_handle, data_length, data]()
	{
		GF_STATUS status = am->WriteCharacteristic(handle, static_cast<GF_UINT16>(attribute_handle),
			data_length, data);
		return static_cast<GF_UINT32>(status);
	})));
	if (GF_OK == status)
		return GF_RET_CODE::GF_SUCCESS;
	else
		return GF_RET_CODE::GF_ERROR;
}

GF_RET_CODE BLEHub::readCharacteristic(BLEDevice& dev, AttributeHandle attribute_handle)
{
	if (nullptr == mAM || INVALID_HANDLE == dev.getHandle())
		return GF_RET_CODE::GF_ERROR_BAD_STATE;

	auto handle = dev.getHandle();
	auto am = mAM;
	GF_UINT32 status = static_cast<GF_UINT32>(executeCommand(make_shared<HubMsg>([&am, handle, attribute_handle]()
	{
		GF_STATUS status = am->ReadCharacteristic(handle, static_cast<GF_UINT16>(attribute_handle));
		return static_cast<GF_UINT32>(status);
	})));
	if (GF_OK == status)
		return GF_RET_CODE::GF_SUCCESS;
	else
		return GF_RET_CODE::GF_ERROR;
}

const char* gForcePrefix = "gForceBLE";

gfsPtr<BLEDevice> BLEHub::createDeviceBeforeConnect(const GF_BLEDevice& bleDev)
{
	if (nullptr != strstr(bleDev.dev_name, gForcePrefix))
	{
		return dynamic_pointer_cast<BLEDevice>(make_shared<GForceDevice>(this, bleDev));
	}
	else
	{
#if ONLY_SCAN_GFORCE_DEVICE
		GF_LOGI("Device unmatch. %s", bleDev.dev_name);
		return nullptr;
#else
		return make_shared<BLEDevice>(this, bleDev);
#endif
	}
}

