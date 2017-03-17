// Device listener

#pragma once

#include "gfTypes.h"

namespace oym
{
	class Device;

	class DeviceListener
	{
		// regarding the device data/status updates
		virtual void onDeviceConnected(WPDEVICE device, GF_STATUS status) = 0;
		virtual void onDeviceDisonnected(WPDEVICE device, GF_STATUS status, GF_UINT8 reason) = 0;

		virtual void onMTUSizeChanged(WPDEVICE device, GF_STATUS status, GF_UINT16 mtu_size) = 0;
		virtual void onConnectionParmeterUpdated(WPDEVICE device, GF_STATUS status, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency) = 0;
		virtual void onChracteristicValueRead(WPDEVICE device, GF_STATUS status, GF_UINT8 length, GF_PUINT8 data) = 0;

		/*Notification format: data length(1 byte N) + data(N Bytes)*/
		virtual void onNotificationReceived(WPDEVICE device, GF_UINT8 length, GF_PUINT8 data) = 0;

		virtual void onEvent(WPDEVICE device, GF_EVENT EVENT, GF_CPCHAR data) = 0;
	};

} // namespace oym
