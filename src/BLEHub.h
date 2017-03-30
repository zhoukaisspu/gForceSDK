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

	class BLEHub :
		public Hub, public GF_CClientCallback, public IHub
	{
	public:
		BLEHub(const tstring& sIdentifier);
		virtual ~BLEHub();


		// module management
		virtual GF_RET_CODE init(GF_UINT8 comPort);
		virtual GF_RET_CODE deinit();
		virtual WorkMode getWorkMode() const {
			lock_guard<mutex> lock(const_cast<mutex&>(mTaskMutex));
			return mWorkMode;
		}
		virtual WorkMode setWorkMode(WorkMode newMode);
		// get status, version, etc.
		virtual HubState getStatus();
		virtual tstring getDescString() const;

		// setup listener
		virtual GF_RET_CODE registerListener(const gfwPtr<HubListener>& listener);
		virtual GF_RET_CODE unRegisterListener(const gfwPtr<HubListener>& listener);

		virtual GF_RET_CODE startScan(GF_UINT8 rssiThreshold);
		virtual GF_RET_CODE stopScan();

		virtual GF_SIZE getNumOfDevices() const {
			lock_guard<mutex> lock(const_cast<mutex&>(mTaskMutex));
			return mDisconnDevices.size() + mConnectedDevices.size();
		}
		virtual GF_SIZE getNumOfConnectedDevices() const {
			lock_guard<mutex> lock(const_cast<mutex&>(mTaskMutex));
			return mConnectedDevices.size();
		}
		virtual void enumDevices(FunEnumDevice funEnum, bool bConnectedOnly = true);
		virtual WPDEVICE findDevice(GF_UINT8 addrType, tstring address);
		// set up virtual device, client can combine two or more gdevices positioning in
		//   defferent location into one virtual device. client still can receive gesture
		//   data of these devices seperately, and can get one combined gesture
		//   data if it happens
		virtual GF_RET_CODE createVirtualDevice(int numDevices, vector<WPDEVICE> realDevices, WPDEVICE& newDevice);

	protected:

		virtual void onScanResult(GF_BLEDevice* device);
		virtual void onScanFinished();
		virtual void onDeviceConnected(GF_STATUS status, GF_ConnectedDevice *device);
		virtual void onDeviceDisonnected(GF_STATUS status, GF_ConnectedDevice *device, GF_UINT8 reason);

		virtual void onMTUSizeChanged(GF_STATUS status, GF_UINT16 handle, GF_UINT16 mtu_size);
		virtual void onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 handle,
			GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency);
		virtual void onCharacteristicValueRead(GF_STATUS status, GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data);

		/*Notification format: data length(1 byte N) + data(N Bytes)*/
		virtual void onNotificationReceived(GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data);

		virtual void onComDestory();

	protected:
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

	protected:
		// device factory methods
		gfsPtr<BLEDevice> createDeviceBeforeConnect(const GF_BLEDevice& bleDev);
	protected:
		GF_CAdapterManagerInterface* mAM = nullptr;
		set<gfsPtr<BLEDevice>, DevComp<gfsPtr<BLEDevice>>> mDisconnDevices;
		set<gfsPtr<BLEDevice>, ConnectedDevComp<gfsPtr<BLEDevice>>> mConnectedDevices;
		set<gfwPtr<HubListener>, WeakPtrComp<HubListener>> mListeners;

	protected:
		struct HubMsg
		{
			function<GF_UINT32()> fun;
			GF_UINT32 ret = 0;
			atomic<bool> executed = false;
			condition_variable syncCallCond;
			HubMsg(decltype(fun) foo) : fun(foo) {}
		};
		GF_UINT32 executeCommand(gfsPtr<HubMsg> msg);
		void commandTask();
		mutex mTaskMutex;
		thread mCommandThread;
		BQueue<gfsPtr<HubMsg>> mMsgQ;

	public:
		virtual GF_RET_CODE poll(Event& event, GF_UINT32 ms = 0)
		{
			event;
			if (mWorkMode == WorkMode::Messaging)
				return GF_RET_CODE::GF_ERROR_BAD_STATE;

			//unique_lock<mutex> lock(mPollMutex);
			//mPollCond.wait_for(lock, duration, pred);
			return GF_RET_CODE::GF_SUCCESS;
		}
	protected:
		WorkMode mWorkMode = WorkMode::Messaging;
		mutex mPollMutex;
		condition_variable mPollCond;
	};


} // namespace gf
