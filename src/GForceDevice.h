// device GForce

#pragma once

#include "BLEDevice.h"

namespace oym
{

	class GForceDevice : public BLEDevice
	{
	public:
		GForceDevice(IDongle* dongle, const BLE_DEVICE& bleDev) : BLEDevice(dongle, bleDev) {}

		//void parseData();

	protected:
		virtual ~GForceDevice() {}
	};

} // namespace oym
