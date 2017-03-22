// Device listener

#pragma once

#include "gfTypes.h"

namespace gf
{
	class Device;

	class DeviceListener
	{
	public:
		// regarding the device data/status updates
		virtual void onDeviceConnected(Device* device, GF_STATUS status) = 0;
		virtual void onDeviceDisonnected(Device* device, GF_STATUS status, GF_UINT8 reason) = 0;

		virtual void onMTUSizeChanged(Device* device, GF_STATUS status, GF_UINT16 mtu_size) = 0;
		virtual void onConnectionParmeterUpdated(Device* device, GF_STATUS status, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency) = 0;
		virtual void onChracteristicUpdated(Device* device, GF_STATUS status, AttributeHandle attribute_handle, GF_UINT8 length, GF_PUINT8 data) = 0;

		/*Notification format: data length(1 byte N) + data(N Bytes)*/
		//virtual void onNotificationReceived(WPDEVICE device, GF_UINT8 length, GF_PUINT8 data) = 0;

		virtual void onEvent(Device* device, GF_EVENT event, GF_UINT8 length, GF_PUINT8 data) = 0;
	};

} // namespace gf
