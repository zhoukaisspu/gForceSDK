#pragma once

#include "gfTypes.h"

namespace oym
{
	class DeviceListener;

	class Device
	{
	public:
		Device() {}

		virtual GF_UINT8 getAddrType() const = 0;
		virtual GF_RET_CODE getAddress(GF_UINT8* addr, GF_SIZE bufLen) const = 0;
		virtual tstring getAddress() const = 0;
		virtual tstring getName() const = 0;
		virtual GF_UINT8 getRssi() const = 0;
		virtual GF_UINT16 getConnInt() const = 0;
		virtual GF_UINT16 getSuperTO() const = 0;
		virtual GF_UINT16 getSlavelatency() const = 0;
		virtual GF_UINT16 getMTUsize() const = 0;

		virtual GF_RET_CODE registerListener(const gfwPtr<DeviceListener>& listener) = 0;

		// configuration

		// control, like light, vibration, etc
		// identify a device by vibration or light, if the device support
		virtual GF_RET_CODE identifyDevice(int msec = 1000) = 0;

		// read info, like status, power, version, capacity, etc
		virtual DeviceConnectionStatus getConnectionStatus() const = 0;
		virtual GF_RET_CODE setPostion(DevicePosition pos) = 0;
		virtual DevicePosition getPosition() const = 0;

		virtual GF_RET_CODE connect(bool directConn = true) = 0;
		virtual GF_RET_CODE disconnect() = 0;
		virtual GF_RET_CODE cancelConnect() = 0;
		virtual GF_RET_CODE configMtuSize(GF_UINT16 mtuSize) = 0;
		virtual GF_RET_CODE connectionParameterUpdate(GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max,
			GF_UINT16 slave_latence, GF_UINT16 supervision_timeout) = 0;

		/*
		GF_STATUS WriteCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data);
		GF_STATUS ReadCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle);
		*/
	protected:
		// all devices have to be created by hub
		virtual ~Device() {}
	};

} // namespace oym
