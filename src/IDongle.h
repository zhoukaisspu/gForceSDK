#pragma once

#include "gfTypes.h"

namespace oym
{
	class BLEDevice;

	class IDongle
	{
	public:
		virtual GF_RET_CODE connect(BLEDevice& dev, bool directConn = true) = 0;
		virtual GF_RET_CODE cancelConnect(BLEDevice& dev) = 0;
		virtual GF_RET_CODE disconnect(BLEDevice& dev) = 0;
		virtual GF_RET_CODE configMtuSize(BLEDevice& dev, GF_UINT16 mtuSize) = 0;
		virtual GF_RET_CODE connectionParameterUpdate(BLEDevice& dev,
			GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max,
			GF_UINT16 slave_latence, GF_UINT16 supervision_timeout) = 0;
		virtual GF_RET_CODE writeCharacteristic(BLEDevice& dev,
			AttributeHandle attribute_handle, GF_UINT8 data_length, GF_PUINT8 data) = 0;
		virtual GF_RET_CODE readCharacteristic(BLEDevice& dev, AttributeHandle attribute_handle) = 0;
	};
} // namespace oym

