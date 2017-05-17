/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between you and
 * OYMotion.  You may not use this Software unless you agree to abide by the
 * terms of the License. The License limits your use, and you acknowledge,
 * that the Software may not be modified, copied or distributed unless used
 * solely and exclusively in conjunction with an OYMotion product.  Other
 * than for the foregoing purpose, you may not use, reproduce, copy, prepare
 * derivative works of, modify, distribute, perform, display or sell this
 * Software and/or its documentation for any purpose.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */
#include "LogUtils.h"
#include "BLEHub.h"
#include "HubListener.h"
#include "GForceDevice.h"
#include "Utils.h"
#include "GFBLETypes.h"

using namespace gf;

BLEHub::BLEHub(const tstring& sIdentifier)
	: mNotifHelper(*this)
{
	GF_LOGD("BLEHub++: identfiler: %s", utils::tostring(sIdentifier).c_str());
}


BLEHub::~BLEHub()
{
	mAM = nullptr;
	GF_LOGD("BLEHub--");
}

GF_UINT32 BLEHub::executeCommand(gfsPtr<HubMsg>& msg)
{
	msg->executed = false;
	unique_lock<mutex> lock;
	try {
		lock = unique_lock<mutex>(mTaskMutex);
	}
	catch (const system_error& e) {
		// duplicate lock in the same thread.
		// try polling mode instead.
		GF_LOGD("%s: system_error with code %d, meaning %s",
			__FUNCTION__, e.code().value(), e.what());
		throw e;
		return 0;
	}
	mMsgQ.push(msg);
	msg->syncCallCond.wait(lock, [&msg]()->bool{ return msg->executed; });
	return msg->ret;
}
void BLEHub::commandTask()
{
	bool loop = true;
	while (loop)
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
	if (GF_OK != mAM->Init(comPort, LOGTYPE_MFC)) {
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

	if (nullptr == mAM)
		return ret;

	// get dongle status and stop scan
	if (HubState::Scanning == getState())
	{
		stopScan();
	}

	// TODO: mAM is not thread-safe
	mAM->UnregisterClientCallback();

	unique_lock<mutex> lock(mTaskMutex);
	auto devices = mDisconnDevices;
	devices.insert(mConnectedDevices.begin(), mConnectedDevices.end());
	lock.unlock();
	for (auto& itor : devices)
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
	for (auto& itor : devices)
	{
		mNotifHelper.onDeviceDiscard(itor);
	}
	devices.clear();

	if (GF_OK != mAM->Deinit()) {
		GF_LOGE("Hub deinit failed.");
	}
	mAM = nullptr;

	mListeners.clear();

	return ret;
}

HubState BLEHub::getState()
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
		ret = HubState::Disconnected;
	}

	return ret;
}

tstring BLEHub::getDescString() const
{
	return tstring(_T("Welcome to the gForce world."));
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
	decltype(mNotifHelper)& notif = mNotifHelper;
	decltype(mDisconnDevices)& dd = mDisconnDevices;
	GF_UINT32 ret = executeCommand(make_shared<HubMsg>([&am, &notif, &dd, rssiThreshold]()
	{
		GF_UINT32 ret = static_cast<GF_UINT32>(am->StartScan(rssiThreshold));
		if (GF_OK == ret)
		{
			// if scan started, remove all disconnected devices
			for (auto itor = dd.begin(); itor != dd.end();)
			{
				if ((*itor)->getConnectionStatus() == DeviceConnectionStatus::Disconnected)
				{
					// notify client that the device will be erased.
					notif.onDeviceDiscard(*itor);
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
	}));
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
	decltype(mNotifHelper)& notif = mNotifHelper;
	GF_UINT32 ret = executeCommand(make_shared<HubMsg>([&am, &notif]()
	{
		GF_UINT32 ret = static_cast<GF_UINT32>(am->StopScan());
		if (GF_OK == ret)
		{
			GF_LOGD("Scan stopped.");
			notif.onScanfinished();
		}
		return ret;
	}));
	if (GF_OK == ret)
		return GF_RET_CODE::GF_SUCCESS;
	else
		return GF_RET_CODE::GF_ERROR;
}

void BLEHub::enumDevices(std::function<bool(WPDEVICE)>& funEnum, bool bConnectedOnly)
{
	GF_LOGD(__FUNCTION__);
	if (nullptr == funEnum)
	{
		GF_LOGD("%s: Bad ptr.", __FUNCTION__);
		return;
	}

	lock_guard<mutex> lock(mTaskMutex);
	for (auto& itor : mConnectedDevices)
	{
		if (!funEnum(static_pointer_cast<Device>(itor)))
			return; // if client wants to stop, it will return false in funEnum
	}
	if (bConnectedOnly)
		return;

	for (auto& itor : mDisconnDevices)
	{
		if (!funEnum(static_pointer_cast<Device>(itor)))
			return; // if client wants to stop, it will return false in funEnum
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
			ret = static_pointer_cast<Device>(itor);
	}
	if (nullptr != ret)
		return ret;
	for (auto& itor : mDisconnDevices)
	{
		if (itor->isMyself(addrType, address))
			ret = static_pointer_cast<Device>(itor);
	}
	return ret;
}

GF_RET_CODE BLEHub::createVirtualDevice(vector<WPDEVICE> realDevices, WPDEVICE& newDevice)
{
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
	mNotifHelper.onDeviceFound(*exists);
}

void BLEHub::onScanFinished()
{
	GF_LOGD(__FUNCTION__);
	lock_guard<mutex> lock(mTaskMutex);
	mNotifHelper.onScanfinished();
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
			utils::deviceAddressToString(device->address, sizeof(device->address)).c_str());
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
			utils::deviceAddressToString(device->address, sizeof(device->address)).c_str());

		// TODO: add new device using GF_ConnectedDevice
		// need more info to do it
		return;
	}
	mConnectedDevices.insert(dev);
	dev->onConnected(status, *device);
	mNotifHelper.onDeviceConnected(dev);
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

void BLEHub::onDeviceDisconnected(GF_STATUS status, GF_ConnectedDevice *device, GF_UINT8 reason)
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
			utils::deviceAddressToString(device->address, sizeof(device->address)).c_str(),
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
			utils::deviceAddressToString(device->address, sizeof(device->address)).c_str(),
			(GF_UINT)reason);
		return;
	}
	mDisconnDevices.insert(dev);
	dev->onDisconnected(status, reason);
	mNotifHelper.onDeviceDisconnected(dev, reason);
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
	lock_guard<mutex> lock(mTaskMutex);
	gfsPtr<BLEDevice> dev;
	for (auto& itor : mConnectedDevices)
	{
		if (itor->getHandle() == handle)
		{
			dev = itor;
			break;
		}
	}
	if (nullptr != dev)
		dev->onData(length - 3, data + 3);
}

void BLEHub::onComDestory()
{
	GF_LOGD(__FUNCTION__);
	// TODO: do something after com destoryed

	lock_guard<mutex> lock(mTaskMutex);
	mNotifHelper.onStateChanged(HubState::Disconnected);
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
	GF_UINT32 status = executeCommand(make_shared<HubMsg>([&am, &addr, type, directConn]()
	{
		GF_STATUS status = am->Connect(addr, type, directConn ? GF_TRUE : GF_FALSE);
		return static_cast<GF_UINT32>(status);
	}));
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
	GF_UINT32 status = executeCommand(make_shared<HubMsg>([&am, &addr, type]()
	{
		GF_STATUS status = am->CancelConnect(addr, type);
		return static_cast<GF_UINT32>(status);
	}));
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
	GF_UINT32 status = executeCommand(make_shared<HubMsg>([&am, handle]()
	{
		GF_STATUS status = am->Disconnect(handle);
		return static_cast<GF_UINT32>(status);
	}));
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
	GF_UINT32 status = executeCommand(make_shared<HubMsg>([&am, handle, mtuSize]()
	{
		GF_STATUS status = am->ConfigMtuSize(handle, mtuSize);
		return static_cast<GF_UINT32>(status);
	}));
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
	GF_UINT32 status = executeCommand(make_shared<HubMsg>([&am, handle,
		conn_interval_min, conn_interval_max, slave_latence, supervision_timeout]()
	{
		GF_STATUS status = am->ConnectionParameterUpdate(handle, conn_interval_min,
			conn_interval_max, slave_latence, supervision_timeout);
		return static_cast<GF_UINT32>(status);
	}));
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
	GF_UINT32 status = executeCommand(make_shared<HubMsg>([&am, handle,
		attribute_handle, data_length, data]()
	{
		GF_STATUS status = am->WriteCharacteristic(handle, static_cast<GF_UINT16>(attribute_handle),
			data_length, data);
		return static_cast<GF_UINT32>(status);
	}));
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
	GF_UINT32 status = executeCommand(make_shared<HubMsg>([&am, handle, attribute_handle]()
	{
		GF_STATUS status = am->ReadCharacteristic(handle, static_cast<GF_UINT16>(attribute_handle));
		return static_cast<GF_UINT32>(status);
	}));
	if (GF_OK == status)
		return GF_RET_CODE::GF_SUCCESS;
	else
		return GF_RET_CODE::GF_ERROR;
}

void BLEHub::notifyOrientationData(BLEDevice& dev, const Quaternion& rotation)
{
	for (auto& itor : mConnectedDevices)
	{
		if (itor.get() == &dev)
		{
			mNotifHelper.onOrientationData(itor, rotation);
			break;
		}
	}
}

void BLEHub::notifyGestureData(BLEDevice& dev, Gesture gest)
{
	for (auto& itor : mConnectedDevices)
	{
		if (itor.get() == &dev)
		{
			mNotifHelper.onGestureData(itor, gest);
			break;
		}
	}
}

void BLEHub::notifyReCenter(BLEDevice& dev)
{
	for (auto& itor : mConnectedDevices)
	{
		if (itor.get() == &dev)
		{
			mNotifHelper.onReCenter(itor);
			break;
		}
	}
}

const char* gForcePrefix = "gForceBLE";

gfsPtr<BLEDevice> BLEHub::createDeviceBeforeConnect(const GF_BLEDevice& bleDev)
{
	if (nullptr != strstr(bleDev.dev_name, gForcePrefix))
	{
		return static_pointer_cast<BLEDevice>(make_shared<GForceDevice>(*this, bleDev));
	}
	else
	{
#if ONLY_SCAN_GFORCE_DEVICE
		GF_LOGI("Device unmatch. %s", bleDev.dev_name);
		return nullptr;
#else
		return make_shared<BLEDevice>(*this, bleDev);
#endif
	}
}

void BLEHub::NotifyHelper::onScanfinished()
{
	if (WorkMode::Polling == mHub.mWorkMode)
	{
		for (auto& itor : mHub.mListeners)
		{
			mHub.mPollMsgQ.push(make_shared<PollingMsg>(
				[itor](){
				auto sp = itor.lock();
				if (nullptr != sp)
					sp->onScanfinished();
			}
			));
		}
	}
	else
	{
		for (auto& itor : mHub.mListeners)
		{
			auto sp = itor.lock();
			if (nullptr != sp)
				sp->onScanfinished();
		}
	}
}

void BLEHub::NotifyHelper::onStateChanged(HubState state)
{
	if (WorkMode::Polling == mHub.mWorkMode)
	{
		for (auto& itor : mHub.mListeners)
		{
			mHub.mPollMsgQ.push(make_shared<PollingMsg>(
				[itor, state](){
				auto sp = itor.lock();
				if (nullptr != sp)
					sp->onStateChanged(state);
			}
			));
		}
	}
	else
	{
		for (auto& itor : mHub.mListeners)
		{
			auto sp = itor.lock();
			if (nullptr != sp)
				sp->onStateChanged(state);
		}
	}
}

void BLEHub::NotifyHelper::onDeviceFound(WPDEVICE device)
{
	if (WorkMode::Polling == mHub.mWorkMode)
	{
		for (auto& itor : mHub.mListeners)
		{
			mHub.mPollMsgQ.push(make_shared<PollingMsg>(
				[itor, device](){
				auto sp = itor.lock();
				if (nullptr != sp)
					sp->onDeviceFound(device);
			}
			));
		}
	}
	else
	{
		for (auto& itor : mHub.mListeners)
		{
			auto sp = itor.lock();
			if (nullptr != sp)
				sp->onDeviceFound(device);
		}
	}
}

void BLEHub::NotifyHelper::onDeviceDiscard(WPDEVICE device)
{
	if (WorkMode::Polling == mHub.mWorkMode)
	{
		for (auto& itor : mHub.mListeners)
		{
			mHub.mPollMsgQ.push(make_shared<PollingMsg>(
				[itor, device](){
				auto sp = itor.lock();
				if (nullptr != sp)
					sp->onDeviceDiscard(device);
			}
			));
		}
	}
	else
	{
		for (auto& itor : mHub.mListeners)
		{
			auto sp = itor.lock();
			if (nullptr != sp)
				sp->onDeviceDiscard(device);
		}
	}
}

void BLEHub::NotifyHelper::onDeviceConnected(WPDEVICE device)
{
	if (WorkMode::Polling == mHub.mWorkMode)
	{
		for (auto& itor : mHub.mListeners)
		{
			mHub.mPollMsgQ.push(make_shared<PollingMsg>(
				[itor, device](){
				auto sp = itor.lock();
				if (nullptr != sp)
					sp->onDeviceConnected(device);
			}
			));
		}
	}
	else
	{
		for (auto& itor : mHub.mListeners)
		{
			auto sp = itor.lock();
			if (nullptr != sp)
				sp->onDeviceConnected(device);
		}
	}
}

void BLEHub::NotifyHelper::onDeviceDisconnected(WPDEVICE device, GF_UINT8 reason)
{
	if (WorkMode::Polling == mHub.mWorkMode)
	{
		for (auto& itor : mHub.mListeners)
		{
			mHub.mPollMsgQ.push(make_shared<PollingMsg>(
				[itor, device, reason](){
				auto sp = itor.lock();
				if (nullptr != sp)
					sp->onDeviceDisconnected(device, reason);
			}
			));
		}
	}
	else
	{
		for (auto& itor : mHub.mListeners)
		{
			auto sp = itor.lock();
			if (nullptr != sp)
				sp->onDeviceDisconnected(device, reason);
		}
	}
}

void BLEHub::NotifyHelper::onOrientationData(WPDEVICE device, const Quaternion& rotation)
{
	if (WorkMode::Polling == mHub.mWorkMode)
	{
		for (auto& itor : mHub.mListeners)
		{
			mHub.mPollMsgQ.push(make_shared<PollingMsg>(
				[itor, device, rotation](){
				auto sp = itor.lock();
				if (nullptr != sp)
					sp->onOrientationData(device, rotation);
			}
			));
		}
	}
	else
	{
		for (auto& itor : mHub.mListeners)
		{
			auto sp = itor.lock();
			if (nullptr != sp)
				sp->onOrientationData(device, rotation);
		}
	}
}

void BLEHub::NotifyHelper::onGestureData(WPDEVICE device, Gesture gest)
{
	if (WorkMode::Polling == mHub.mWorkMode)
	{
		for (auto& itor : mHub.mListeners)
		{
			mHub.mPollMsgQ.push(make_shared<PollingMsg>(
				[itor, device, gest](){
				auto sp = itor.lock();
				if (nullptr != sp)
					sp->onGestureData(device, gest);
			}
			));
		}
	}
	else
	{
		for (auto& itor : mHub.mListeners)
		{
			auto sp = itor.lock();
			if (nullptr != sp)
				sp->onGestureData(device, gest);
		}
	}
}

void BLEHub::NotifyHelper::onReCenter(WPDEVICE device)
{
	if (WorkMode::Polling == mHub.mWorkMode)
	{
		for (auto& itor : mHub.mListeners)
		{
			mHub.mPollMsgQ.push(make_shared<PollingMsg>(
				[itor, device](){
				auto sp = itor.lock();
				if (nullptr != sp)
					sp->onReCenter(device);
			}
			));
		}
	}
	else
	{
		for (auto& itor : mHub.mListeners)
		{
			auto sp = itor.lock();
			if (nullptr != sp)
				sp->onReCenter(device);
		}
	}
}

GF_RET_CODE BLEHub::run(GF_UINT32 ms, bool once)
{
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	unique_lock<mutex> lock(mPollMutex, try_to_lock);
	if (!lock.owns_lock())
		return ret;
	if (mWorkMode != WorkMode::Polling)
		return ret;
	if (mPolling)
		return ret;

	mPolling = true;
	ret = GF_RET_CODE::GF_SUCCESS;

	///
	lock.unlock();
	bool loop = !once;
	auto until = chrono::steady_clock::now() + chrono::duration<GF_UINT32, milli>(ms);
	GF_UINT32 time_last = ms;
	do {
		gfsPtr<PollingMsg> msg;
		if (!mPollMsgQ.pop_until(msg, until))
		{
			// timer out
			ret = GF_RET_CODE::GF_ERROR_TIMEOUT;
			break;
		}
		if (nullptr == msg)
		{
			GF_LOGE("Error: polling message is null.");
			ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
			break;
		}
		msg->fun();
	} while (loop);
	///

	lock.lock();
	mPolling = false;
	return ret;
}
