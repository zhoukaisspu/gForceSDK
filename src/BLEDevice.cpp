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
#include "BLEDevice.h"
#include "Utils.h"
#include <cstdio>


using namespace gf;


BLEDevice::BLEDevice(IHub& hub, const GF_BLEDevice& bleDev)
	: mHub(hub)
	, mAddrType(bleDev.addr_type)
	, mRssi(bleDev.rssi)
{
	memcpy(mAddress, bleDev.addr, sizeof(mAddress));

	GF_CHAR name[BLE_DEVICE_NAME_LENGTH + 1] = "";
	memcpy(name, bleDev.dev_name, sizeof(bleDev.dev_name));
	name[BLE_DEVICE_NAME_LENGTH] = '\0';
	mName = utils::totstring(string(name));

	GF_LOGD("BLEDevice++: address type: %u, address: %s, name: %s", (GF_UINT)mAddrType,
		utils::tostring(getAddress()).c_str(), utils::tostring(mName).c_str());
}

BLEDevice::~BLEDevice()
{
	GF_LOGD("BLEDevice--");
}

GF_RET_CODE BLEDevice::identify(int msec)
{
	msec;
	GF_LOGI("%s: Opps, not implemented yet.", __FUNCTION__);
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}

GF_RET_CODE BLEDevice::getAddress(GF_UINT8* addr, GF_SIZE bufLen) const
{
	if (nullptr == addr || bufLen < sizeof(mAddress))
		return GF_RET_CODE::GF_ERROR_BAD_PARAM;
	memcpy(addr, mAddress, sizeof(mAddress));
	return GF_RET_CODE::GF_SUCCESS;
}

tstring BLEDevice::getAddress() const
{
	return utils::deviceAddressToStringT(mAddress, sizeof(mAddress));
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
	return true;
}

GF_RET_CODE BLEDevice::setPostion(DevicePosition pos)
{
	GF_LOGD(__FUNCTION__);
	mPosition = pos;
	return GF_RET_CODE::GF_SUCCESS;
}

GF_RET_CODE BLEDevice::connect(bool directConn)
{
	GF_LOGD(__FUNCTION__);
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR;
	switch (mCnntStatus)
	{
	case DeviceConnectionStatus::Connected:
		return GF_RET_CODE::GF_SUCCESS;
	case DeviceConnectionStatus::Disconnecting:
		break;
	case DeviceConnectionStatus::Disconnected:
	case DeviceConnectionStatus::Connecting:
		ret = mHub.connect(*this, directConn);
		break;
	default:;
	}
	if (ret == GF_RET_CODE::GF_SUCCESS)
		mCnntStatus = DeviceConnectionStatus::Connecting;
	return ret;
}

GF_RET_CODE BLEDevice::disconnect()
{
	GF_LOGD(__FUNCTION__);
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR;
	switch (mCnntStatus)
	{
	case DeviceConnectionStatus::Connected:
		ret = mHub.disconnect(*this);
		break;
	case DeviceConnectionStatus::Disconnected:
	case DeviceConnectionStatus::Disconnecting:
		return GF_RET_CODE::GF_SUCCESS;
	case DeviceConnectionStatus::Connecting:
		break;
	default:;
	}
	if (ret == GF_RET_CODE::GF_SUCCESS)
		mCnntStatus = DeviceConnectionStatus::Disconnecting;
	return ret;
}

GF_RET_CODE BLEDevice::cancelConnect()
{
	GF_LOGD(__FUNCTION__);
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR;
	switch (mCnntStatus)
	{
	case DeviceConnectionStatus::Connected:
	case DeviceConnectionStatus::Disconnected:
	case DeviceConnectionStatus::Disconnecting:
		break;
	case DeviceConnectionStatus::Connecting:
		ret = mHub.cancelConnect(*this);
		break;
	default:;
	}
	if (ret == GF_RET_CODE::GF_SUCCESS)
	{
		mCnntStatus = DeviceConnectionStatus::Disconnecting;
		GF_LOGD("Connection cancelling...");
	}
	return ret;
}

void BLEDevice::updateData(const GF_BLEDevice& bleDev)
{
	lock_guard<mutex> lock(mMutex);
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

void BLEDevice::onConnected(GF_STATUS status, const GF_ConnectedDevice& connedDevice)
{
	lock_guard<mutex> lock(mMutex);
	GF_LOGD("Device connected: previous state is: %u", static_cast<GF_UINT>(mCnntStatus));
	mHandle = connedDevice.handle;
	mConnInt = connedDevice.conn_int;
	mSuperTO = connedDevice.superTO;
	mSlavelatency = connedDevice.slavelatency;
	mMTUsize = connedDevice.MTUsize;

#if 0 // don't change status in onXxxx methods
	if (DeviceConnectionStatus::Disconnecting == mCnntStatus)
	{
		if (GF_RET_CODE::GF_SUCCESS == mHub.disconnect(*this))
		{
			// if user selected to cancel connection, make it and no notifiction
			mCnntStatus = DeviceConnectionStatus::Disconnecting;
			GF_LOGD("Trying to disconnect as client sent a cancel order.");
			return;
		}
	}
#endif
	mCnntStatus = DeviceConnectionStatus::Connected;
}

void BLEDevice::onDisconnected(GF_STATUS status, GF_UINT8 reason)
{
	lock_guard<mutex> lock(mMutex);
	GF_LOGD("Device disconnected: previous state is: %u, reason is: %u",
		static_cast<GF_UINT>(mCnntStatus), (GF_UINT)reason);
	mCnntStatus = DeviceConnectionStatus::Disconnected;
	mHandle = INVALID_HANDLE;
	mConnInt = 0;
	mSuperTO = 0;
	mSlavelatency = 0;
	mMTUsize = 0;
}

GF_RET_CODE BLEDevice::configMtuSize(GF_UINT16 mtuSize)
{
	GF_LOGD(__FUNCTION__);
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (mHandle != INVALID_HANDLE)
	{
		ret = mHub.configMtuSize(*this, mtuSize);
	}
	return ret;
}

GF_RET_CODE BLEDevice::connectionParameterUpdate(GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max,
	GF_UINT16 slave_latence, GF_UINT16 supervision_timeout)
{
	GF_LOGD(__FUNCTION__);
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (mHandle != INVALID_HANDLE)
	{
		ret = mHub.connectionParameterUpdate(*this, conn_interval_min, conn_interval_max,
			slave_latence, supervision_timeout);
	}
	return ret;
}

GF_RET_CODE BLEDevice::writeCharacteristic(AttributeHandle attribute_handle, GF_UINT8 dataLen, GF_PUINT8 data)
{
	GF_LOGD(__FUNCTION__);
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (mHandle != INVALID_HANDLE)
	{
		ret = mHub.writeCharacteristic(*this, attribute_handle, dataLen, data);
	}
	return ret;
}

GF_RET_CODE BLEDevice::readCharacteristic(AttributeHandle attribute_handle)
{
	GF_LOGD(__FUNCTION__);
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR_BAD_STATE;
	if (mHandle != INVALID_HANDLE)
	{
		ret = mHub.readCharacteristic(*this, attribute_handle);
	}
	return ret;
}

void BLEDevice::onMTUSizeChanged(GF_STATUS status, GF_UINT16 mtu_size)
{
	GF_LOGD("%s: status: %u, new MTU size: %u", __FUNCTION__, (GF_UINT)status, (GF_UINT)mtu_size);
	lock_guard<mutex> lock(mMutex);
	mMTUsize = mtu_size;

	//mHub.notifyDeviceEvent(*this, [status, mtu_size](HubListener& listener, WPDEVICE& self){
	//	listener; self;
	//	//listener.onMTUSizeChanged(self, status, mtu_size);
	//});
}

void BLEDevice::onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency)
{
	GF_LOGD(__FUNCTION__);
	GF_LOGD("status: %u", (GF_UINT)status);
	GF_LOGD("conn_int: %u", (GF_UINT)conn_int);
	GF_LOGD("superTO: %u", (GF_UINT)superTO);
	GF_LOGD("slavelatency: %u", (GF_UINT)slavelatency);
	lock_guard<mutex> lock(mMutex);
	mConnInt = conn_int;
	mSuperTO = superTO;
	mSlavelatency = slavelatency;

	// notify client.
	//mHub.notifyDeviceEvent(*this, [status, conn_int, superTO, slavelatency](HubListener& listener, WPDEVICE& self){
	//	listener; self;
	//	//listener.onConnectionParmeterUpdated(this, status, conn_int, superTO, slavelatency);
	//});
}

void BLEDevice::onCharacteristicValueRead(GF_STATUS status, GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD(__FUNCTION__);
	GF_LOGD("status: %u", (GF_UINT)status);
	GF_LOGD("length: %u", (GF_UINT)length);
	GF_LOGD("&data: %p", data);
	// TODO: notify client
}

void BLEDevice::onData(GF_UINT8, GF_PUINT8)
{
	// nothing to do
}
