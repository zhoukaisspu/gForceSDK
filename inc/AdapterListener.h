#pragma once

#include "gfTypes.h"

namespace oym
{
	class AdapterListener
	{
	public:
		virtual void onScanfinished() = 0;
		virtual void onDeviceFound(WPDEVICE device) = 0;
		virtual void onDeviceDiscard(WPDEVICE device) = 0;
		// when a device is specialized, it means that more functionalities
		// are recongnized, a subclass is created to take over the old one,
		// nothing more to be cared but discard the oldPtr.
		virtual void onDeviceSpecialized(WPDEVICE oldPtr, WPDEVICE newPtr) = 0;
		virtual void onStateChanged(AdapterState state) = 0;
	};

}
