// define the device manager as hub
#pragma once

#include "gfTypes.h"
#include <vector>

namespace oym
{
	class DongleListener;

	class Dongle
	{
	public:

		// module management
		virtual GF_RET_CODE init() = 0;
		virtual GF_RET_CODE deinit() = 0;
		// get status, version, etc.
		virtual DongleState getStatus() const = 0;
		virtual tstring getDescString() const = 0;

		// setup listener
		virtual GF_RET_CODE registerListener(const gfwPtr<DongleListener>& listener) = 0;

		virtual GF_RET_CODE startScan(GF_UINT8 rssiThreshold = 0) = 0;
		virtual GF_RET_CODE stopScan() = 0;

		virtual int getNumOfDevices() const = 0;
		virtual int getNumOfConnectedDevices() const = 0;
		virtual void enumDevices(FunEnumDevice funEnum, bool bConnectedOnly = true) = 0;
		virtual WPDEVICE findDevice(GF_UINT8 addrType, tstring address) = 0;
		// set up virtual device, client can combine two or more gdevices positioning in
		//   defferent location into one virtual device. client still can receive gesture
		//   data of these devices seperately, and can get one combined gesture
		//   data if it happens
		virtual GF_RET_CODE createVirtualDevice(int numDevices, std::vector<WPDEVICE> realDevices, WPDEVICE& newDevice) = 0;
		
		// removing method releaseDevice, virtual device can be destoryed by calling disconnect
		//virtual int releaseDevice(SPTR_DEV& device) = 0;

	protected:
		Dongle() {}
		virtual ~Dongle() {}
		friend class DongleManager;
	};

} // namespace oym
