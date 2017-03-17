#pragma once

#include "gfTypes.h"

namespace oym
{
	class DongleListener
	{
	public:
		virtual void onScanfinished() = 0;
		virtual void onDeviceFound(WPDEVICE) = 0;
		// when a device is specialized, it means that more functionalities
		// are recongnized, a subclass is created to take over the old one,
		// nothing more to be cared but discard the oldPtr.
		virtual void onDeviceSpecialized(WPDEVICE oldPtr, WPDEVICE newPtr) = 0;
		virtual void onStateChanged(DongleState) = 0;
	};

}
