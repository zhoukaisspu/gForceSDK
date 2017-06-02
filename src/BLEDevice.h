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
// define generic device
#pragma once

#include "Device.h"
#include "IHub.h"
#include "Utils.h"
#include "HubListener.h"
#include "ClientCallbackInterface.h"

#include <set>
#include <mutex>
using namespace std;

namespace gf
{

	const static GF_UINT16 INVALID_HANDLE = 0xFFFF;

	class BLEDevice : public Device
	{
	public:
		BLEDevice(IHub& hub, const GF_BLEDevice& bleDev);
		virtual ~BLEDevice();

		virtual GF_UINT8 getAddrType() const override { return mAddrType; }
		virtual GF_RET_CODE getAddress(GF_UINT8* addr, GF_SIZE bufLen) const;
		virtual tstring getAddress() const override;
		virtual tstring getName() const override { return mName; }
		virtual GF_UINT8 getRssi() const { return mRssi; }
		virtual GF_UINT16 getHandle() const { return mHandle; }
		virtual GF_UINT16 getConnInt() const { return mConnInt; }
		virtual GF_UINT16 getSuperTO() const { return mSuperTO; }
		virtual GF_UINT16 getSlavelatency() const { return mSlavelatency; }
		virtual GF_UINT16 getMTUsize() const { return mMTUsize; }

		virtual GF_RET_CODE identify(int msec = 1000) override;

		virtual DeviceConnectionStatus getConnectionStatus() const override { return mCnntStatus; }
		virtual GF_RET_CODE setPostion(DevicePosition pos) override;
		virtual DevicePosition getPosition() const override { return mPosition; }

		virtual GF_RET_CODE connect(bool directConn = true) override;
		virtual GF_RET_CODE disconnect() override;
		virtual GF_RET_CODE cancelConnect() override;

		virtual GF_RET_CODE configMtuSize(GF_UINT16 mtuSize);
		virtual GF_RET_CODE connectionParameterUpdate(GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max,
			GF_UINT16 slave_latence, GF_UINT16 supervision_timeout);
		virtual GF_RET_CODE writeCharacteristic(AttributeHandle attribute_handle, GF_UINT8 dataLen, GF_PUINT8 data);
		virtual GF_RET_CODE readCharacteristic(AttributeHandle attribute_handle);

	public:
		virtual void updateData(const GF_BLEDevice& bleDev);
		virtual void onConnected(GF_STATUS status, const GF_ConnectedDevice& connedDevice);
		virtual void onDisconnected(GF_STATUS status, GF_UINT8 reason);
		virtual void onMTUSizeChanged(GF_STATUS status, GF_UINT16 mtu_size);
		virtual void onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency);
		virtual void onCharacteristicValueRead(GF_STATUS status, GF_UINT8 length, GF_PUINT8 data);
		virtual void onData(GF_UINT8 length, GF_PUINT8 data);

	public:
		virtual bool operator < (const BLEDevice& devRight) const {
			if (mAddrType == devRight.mAddrType) {
				return ((0 > memcmp(mAddress, devRight.mAddress, sizeof(mAddress))) ?
					true : false);
			}
			else {
				return (mAddrType < devRight.mAddrType ? true : false);
			}
		}
		virtual bool isMyself(GF_UINT8 addrType, GF_UINT8 addr[]) const;
		virtual bool isMyself(GF_UINT8 addrType, tstring addr) const;
		virtual bool takeover(BLEDevice& from);

	protected:
		IHub& mHub;

	protected:
		DeviceConnectionStatus mCnntStatus = DeviceConnectionStatus::Disconnected;
		DevicePosition mPosition = DevicePosition::NotAssigned;
		GF_UINT8	mAddrType = 0;
		GF_UINT8	mAddress[BT_ADDRESS_SIZE];
		tstring		mName;
		GF_UINT8	mRssi = 0;

		GF_UINT16	mHandle = INVALID_HANDLE;
		GF_UINT16	mConnInt = 0;
		GF_UINT16	mSuperTO = 0;
		GF_UINT16	mSlavelatency = 0;
		GF_UINT16	mMTUsize = 0;

	protected:
		// TODO: thread-safe
		mutex mMutex;
	};

} // namespace gf
