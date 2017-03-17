#pragma once

#include <set>

#include "Dongle.h"
#include "BLEDevice.h"
#include "Utils.h"

#include "AdapterManager.h"
#include "IClientCallback.h"

namespace oym
{

	class BLEDongle :
		public Dongle, public IClientCallback
	{
	public:
		BLEDongle(const tstring& sIdentifier);
		virtual ~BLEDongle();


		// module management
		virtual GF_RET_CODE init();
		virtual GF_RET_CODE deinit();
		// get status, version, etc.
		virtual DongleState getStatus() const;
		virtual tstring getDescString() const;

		// setup listener
		virtual GF_RET_CODE registerListener(const gfwPtr<DongleListener>& listener);

		virtual GF_RET_CODE startScan(OYM_UINT8 rssiThreshold = 0);
		virtual GF_RET_CODE stopScan();

		virtual int getNumOfDevices() const { return mDevices.size(); }
		virtual int getNumOfConnectedDevices() const;
		virtual void enumDevices(FunEnumDevice funEnum, bool bConnectedOnly = true);
		virtual WPDEVICE findDevice(OYM_UINT8 addrType, tstring address);
		// set up virtual device, client can combine two or more gdevices positioning in
		//   defferent location into one virtual device. client still can receive gesture
		//   data of these devices seperately, and can get one combined gesture
		//   data if it happens
		virtual GF_RET_CODE createVirtualDevice(int numDevices, vector<WPDEVICE> realDevices, WPDEVICE& newDevice);

	protected:

		virtual void onScanResult(BLE_DEVICE* device);
		virtual void onScanFinished();
		virtual void onDeviceConnected(OYM_STATUS status, GF_ConnectedDevice *device);
		virtual void onDeviceDisonnected(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT8 reason);

		virtual void onMTUSizeChanged(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 mtu_size);
		virtual void onConnectionParmeterUpdated(OYM_STATUS status, OYM_UINT16 handle,
			OYM_UINT16 conn_int, OYM_UINT16 superTO, OYM_UINT16 slavelatency);
		virtual void onChracteristicValueRead(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT8 length, OYM_PUINT8 data);

		/*Notification format: data length(1 byte N) + data(N Bytes)*/
		virtual void onNotificationReceived(OYM_UINT16 handle, OYM_UINT8 length, OYM_PUINT8 data);

		virtual void onComDestory();

	private:
		gfsPtr<OYM_AdapterManager> mAM;
		typedef gfsPtr<BLEDevice> PTR_DEVICE;
		typedef set<gfsPtr<BLEDevice>, DevComp<gfsPtr<BLEDevice>>> SET_DEVICE;
		SET_DEVICE mDevices;
		set<gfwPtr<DongleListener>, WeakPtrComp<DongleListener>> mListeners;
	};


} // namespace oym
