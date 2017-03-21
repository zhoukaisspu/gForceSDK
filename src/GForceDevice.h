// define gForce device

#pragma once

#include "BLEDevice.h"

namespace oym
{
	class BLEAdapter;

#define EVENT_MASK				0x7F
#define PCKID_FLAG_MASK			0x01
#define EVENT_RECENTER_MASK		0x01
#define INVALID_PACKAGE_ID		0xFFFF

	enum {
		EVENT_QUATERNION = 0x02,
		EVENT_GESTURE = 0x0F,
		EVENT_STATUS = 0x14
	};

	class GForceDevice : public BLEDevice
	{
	public:
		GForceDevice(IAdapter* adapter, const BLE_DEVICE& bleDev) : BLEDevice(adapter, bleDev) {}
		virtual ~GForceDevice() {}

	protected:
		virtual void onData(GF_UINT8 length, GF_PUINT8 data);

	private:
		void onQuaternion(GF_UINT8 length, GF_PUINT8 data);
		void onGesture(GF_UINT8 length, GF_PUINT8 data);
		void onStatus(GF_UINT8 length, GF_PUINT8 data);

	private:
		GF_UINT16 mPackageId = INVALID_PACKAGE_ID;
	};

} // namespace oym
