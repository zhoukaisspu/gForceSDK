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
#pragma once

#include <atomic>
#include <thread>
#include <set>

#include "Hub.h"
#include "IHub.h"
#include "BLEDevice.h"
#include "Utils.h"

#include "HubManager.h"
#include "ClientCallbackInterface.h"
#include "AdapterManagerInterface.h"


#define ONLY_SCAN_GFORCE_DEVICE (1)

namespace gf
{
	// define Characteristic read/write handle
	// will be deleted
#if 0
	enum class AttributeHandle : GF_UINT16 {
		GATTPrimServiceDeclaration1 = 0x0001,
		GATTCharacteristicDeclaration1,
		DeviceName,
		GATTCharacteristicDeclaration2,
		Appearance,
		GATTCharacteristicDeclaration3,
		PreferredConnectParamters,
		GATTPrimServiceDeclaration2,
		GATTPrimServiceDeclaration3,
		GATTCharacteristicDeclaration4,
		SystemID,
		GATTCharacteristicDeclaration5,
		ModelNumberStr,
		GATTCharacteristicDeclaration6,
		SerialNumberStr,
		GATTCharacteristicDeclaration7,
		FirmwareRevStr,
		GATTCharacteristicDeclaration8,
		HardwareRevStr,
		GATTCharacteristicDeclaration9,
		SoftwareRevStr,
		GATTCharacteristicDeclaration10,
		ManufactureNameStr,
		GATTCharacteristicDeclaration11,
		IEEE11073_20601,
		GATTCharacteristicDeclaration12,
		PnPID,
		GATTPrimServiceDeclaration4,
		GATTCharacteristicDeclaration13,
		Max
	};
	static const char charTypes[] =
		"BBSBBBBBBBBBSBSBSBSBSBSBBBBBBN";
#endif

	class BLEHub :
		public Hub, public GF_CClientCallback, public IHub
	{
	public:
		BLEHub(const tstring& sIdentifier);
		virtual ~BLEHub();

		//////////////////////////////////////////////////////////////
		// Derived from class Hub
		// module management
		virtual GF_RET_CODE init(GF_UINT8 comPort);
		virtual GF_RET_CODE deinit();
		virtual WorkMode getWorkMode() const {
			return mWorkMode;
		}
		virtual void setWorkMode(WorkMode newMode) {
			GF_LOGI("setWorkMode. %d", static_cast<int>(newMode));
			if (WorkMode::Polling == newMode)
				GF_LOGI("Please call method run(ms) to pull message.");
			mWorkMode = newMode;
		}
		// get status, version, etc.
		virtual HubState getState();
		virtual tstring getDescString() const;

		// setup listener
		virtual GF_RET_CODE registerListener(const gfwPtr<HubListener>& listener);
		virtual GF_RET_CODE unRegisterListener(const gfwPtr<HubListener>& listener);

		virtual GF_RET_CODE startScan(GF_UINT8 rssiThreshold);
		virtual GF_RET_CODE stopScan();

		virtual GF_SIZE getNumOfDevices(bool bConnectedOnly = true) const {
			lock_guard<mutex> lock(const_cast<mutex&>(mTaskMutex));
			if (bConnectedOnly)
				return mConnectedDevices.size();
			else
				return mDisconnDevices.size() + mConnectedDevices.size();
		}
		virtual void enumDevices(std::function<bool(WPDEVICE)>& funEnum, bool bConnectedOnly = true);
		virtual WPDEVICE findDevice(GF_UINT8 addrType, tstring address);
		// set up virtual device, client can combine two or more gdevices positioning in
		//   defferent location into one virtual device. client still can receive gesture
		//   data of these devices seperately, and can get one combined gesture
		//   data if it happens
		virtual GF_RET_CODE createVirtualDevice(vector<WPDEVICE> realDevices, WPDEVICE& newDevice);

	protected:
		//////////////////////////////////////////////////////////////
		// Derived from class IClientCallback to receive BLE event
		virtual void onScanResult(GF_BLEDevice* device);
		virtual void onScanFinished();
		virtual void onDeviceConnected(GF_STATUS status, GF_ConnectedDevice *device);
		virtual void onDeviceDisconnected(GF_STATUS status, GF_ConnectedDevice *device, GF_UINT8 reason);

		virtual void onMTUSizeChanged(GF_STATUS status, GF_UINT16 handle, GF_UINT16 mtu_size);
		virtual void onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 handle,
			GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency);
		virtual void onCharacteristicValueRead(GF_STATUS status, GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data);

		/*Notification format: data length(1 byte N) + data(N Bytes)*/
		virtual void onNotificationReceived(GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data);

		virtual void onComDestory();

	protected:
		//////////////////////////////////////////////////////////////
		// IHub for device usage
		virtual GF_RET_CODE connect(BLEDevice& dev, bool directConn = true);
		virtual GF_RET_CODE cancelConnect(BLEDevice& dev);
		virtual GF_RET_CODE disconnect(BLEDevice& dev);
		virtual GF_RET_CODE configMtuSize(BLEDevice& dev, GF_UINT16 mtuSize);
		virtual GF_RET_CODE connectionParameterUpdate(BLEDevice& dev,
			GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max,
			GF_UINT16 slave_latence, GF_UINT16 supervision_timeout);
		virtual GF_RET_CODE writeCharacteristic(BLEDevice& dev,
			AttributeHandle attribute_handle, GF_UINT8 data_length, GF_PUINT8 data);
		virtual GF_RET_CODE readCharacteristic(BLEDevice& dev, AttributeHandle attribute_handle);
		virtual void notifyOrientationData(BLEDevice& dev, const Quaternion& rotation);
		virtual void notifyGestureData(BLEDevice& dev, Gesture gest);
		virtual void notifyReCenter(BLEDevice& dev);

	protected:
		//////////////////////////////////////////////////////////////
		// device factory methods
		gfsPtr<BLEDevice> createDeviceBeforeConnect(const GF_BLEDevice& bleDev);
	protected:
		//////////////////////////////////////////////////////////////
		// generic members
		GF_CAdapterManagerInterface* mAM = nullptr;
		set<gfsPtr<BLEDevice>, DevComp<gfsPtr<BLEDevice>>> mDisconnDevices;
		set<gfsPtr<BLEDevice>, ConnectedDevComp<gfsPtr<BLEDevice>>> mConnectedDevices;
		set<gfwPtr<HubListener>, WeakPtrComp<HubListener>> mListeners;

	protected:
		//////////////////////////////////////////////////////////////
		// Inner working thread to process call to AdapterManager
		struct HubMsg
		{
			function<GF_UINT32()> fun;
			GF_UINT32 ret = 0;
			atomic<bool> executed = false;
			condition_variable syncCallCond;
			HubMsg(decltype(fun) foo) : fun(foo) {}
		};
		GF_UINT32 executeCommand(gfsPtr<HubMsg>& msg);
		void commandTask();
		mutex mTaskMutex;
		thread mCommandThread;
		BQueue<gfsPtr<HubMsg>> mMsgQ;

	public:
		//////////////////////////////////////////////////////////////
		// Inner working thread to process Polling mode
		virtual GF_RET_CODE run(GF_UINT32 ms, bool once = false);
	protected:
		class NotifyHelper
		{
		public:
			NotifyHelper(BLEHub& theHub) : mHub(theHub) {}
			virtual void onScanfinished();
			virtual void onStateChanged(HubState state);
			virtual void onDeviceFound(WPDEVICE device);
			virtual void onDeviceDiscard(WPDEVICE device);
			// regarding the device data/status updates
			virtual void onDeviceConnected(WPDEVICE device);
			virtual void onDeviceDisconnected(WPDEVICE device, GF_UINT8 reason);

			virtual void onOrientationData(WPDEVICE device, const Quaternion& rotation);
			virtual void onGestureData(WPDEVICE device, Gesture gest);
			virtual void onReCenter(WPDEVICE device);

			// remove warning
			NotifyHelper& operator = (const NotifyHelper& right) = delete;
		private:
			BLEHub& mHub;
		} mNotifHelper;
		struct PollingMsg
		{
			function<void()> fun;
			PollingMsg(decltype(fun) foo) : fun(foo) {}
		};
		BQueue<gfsPtr<PollingMsg>> mPollMsgQ;
		atomic<WorkMode> mWorkMode = WorkMode::Freerun;
		mutex mPollMutex;
		bool mPolling = false;
		condition_variable mPollCond;
	};


} // namespace gf
