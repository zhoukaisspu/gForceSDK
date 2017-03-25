// define the device manager as hub
#pragma once

#include "gfTypes.h"
#include <vector>

namespace gf
{
	class HubListener;

	class Hub
	{
	public:

		// module management
		virtual GF_RET_CODE init(GF_UINT8 comPort = 0) = 0;
		virtual GF_RET_CODE deinit() = 0;
		virtual WorkMode getWorkMode() const = 0;
		virtual WorkMode setWorkMode(WorkMode newMode) = 0;
		// get status, version, etc.
		virtual HubState getStatus() = 0;
		virtual tstring getDescString() const = 0;

		// setup listener
		virtual GF_RET_CODE registerListener(const gfwPtr<HubListener>& listener) = 0;
		virtual GF_RET_CODE unRegisterListener(const gfwPtr<HubListener>& listener) = 0;

		virtual GF_RET_CODE startScan(GF_UINT8 rssiThreshold = 0) = 0;
		virtual GF_RET_CODE stopScan() = 0;

		virtual GF_SIZE getNumOfDevices() const = 0;
		virtual GF_SIZE getNumOfConnectedDevices() const = 0;
		virtual void enumDevices(FunEnumDevice funEnum, bool bConnectedOnly = true) = 0;
		virtual WPDEVICE findDevice(GF_UINT8 addrType, tstring address) = 0;
		// set up virtual device, client can combine two or more gdevices positioning in
		//   defferent location into one virtual device. client still can receive gesture
		//   data of these devices seperately, and can get one combined gesture
		//   data if it happens
		virtual GF_RET_CODE createVirtualDevice(int numDevices, std::vector<WPDEVICE> realDevices, WPDEVICE& newDevice) = 0;

		// removing method releaseDevice, virtual device can be destoryed by calling disconnect
		//virtual int releaseDevice(SPTR_DEV& device) = 0;

		virtual GF_RET_CODE poll(Event& event) = 0;

	protected:
		Hub() {}
		virtual ~Hub() {}
		friend class HubManager;
	};

} // namespace gf
