// device combination

#pragma once

#include "Device.h"

namespace gf
{

	class VirtualDevice : public Device
	{
	public:
		VirtualDevice() {}
		virtual GF_RET_CODE setPostion(DevicePosition pos) { return GF_ERROR_NOT_SUPPORT; }
		virtual DevicePosition getPosition() const {
			return DevicePosition::Virtual;
		}

		// consider some real devices of this virtual device has been specialized

	protected:
		virtual ~VirtualDevice() {}
	};

} // namespace gf
