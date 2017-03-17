// device GForce

#pragma once

#include "BLEDevice.h"

namespace oym
{

	class GForceDevice : public BLEDevice
	{
	public:
		GForceDevice(gfsPtr<OYM_AdapterManager>& am, const BLE_DEVICE& bleDev) : BLEDevice(am, bleDev) {}

	protected:
		virtual ~GForceDevice() {}
	};

} // namespace oym
