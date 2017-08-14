#include "LogUtils.h"
#include "OADProfile.h"

using namespace gf;

GF_RET_CODE OADSetting::oadUpgrade(FILE* file, function<void(GF_UINT32 percentage)> progress)
{
	GF_LOGD("%s coming", __FUNCTION__);
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}

void OADSetting::dispatchResponse(GF_UINT8 command, GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, bool timeout)
{

}

void OADProfile::onData(GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD("%s", __FUNCTION__);
}

void OADProfile::onResponse(GF_UINT8 length, GF_PUINT8 data)
{
	if (nullptr != mDevSetting)
		mDevSetting->onResponse(length, data);
}

gfsPtr<DeviceSetting> OADProfile::getDeviceSetting()
{
	gfsPtr<BLEDevice> device = mDevice.lock();
	if (nullptr == device)
	{
		mDevSetting.reset();
		return gfsPtr<DeviceSetting>();
	}

	if (nullptr == mDevSetting)
	{
		mDevSetting = make_shared<OADSetting>(device);
	}

	return mDevSetting;
}