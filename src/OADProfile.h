#pragma once
#include "DeviceProfile.h"
#include "DeviceSettingHandle.h"

namespace gf
{
	class OADSetting :
		public DeviceSettingHandle
	{
	public:
		OADSetting(gfwPtr<BLEDevice> device)
			: DeviceSettingHandle(device) {}
	public:
		virtual GF_RET_CODE oadUpgrade(FILE* file, function<void(GF_UINT32 percentage)> progress) override;
		virtual void dispatchResponse(GF_UINT8 command, GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, bool timeout = false) override;
	};

	class OADProfile :
		public DeviceProfile
	{
	public:
		OADProfile(gfwPtr<BLEDevice> device)
			: DeviceProfile(device) {}

	public:
		virtual void onData(GF_UINT8 length, GF_PUINT8 data) override;
		virtual void onResponse(GF_UINT8 length, GF_PUINT8 data) override;
		virtual gfsPtr<DeviceSetting> getDeviceSetting() override;

	private:
		gfsPtr<OADSetting> mDevSetting;
	};

}
