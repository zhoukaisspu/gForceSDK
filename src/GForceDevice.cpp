#include "GForceDevice.h"
#include "utils.h"
#include "LogUtils.h"

using namespace gf;


void GForceDevice::onData(GF_UINT8 length, GF_PUINT8 data)
{
	if (length <= 1)
		return;

	GF_UINT8 evtType = data[0] & EVENT_MASK;
	GF_UINT8 packageIdFlag = (data[0] >> 7) & PCKID_FLAG_MASK;
	GF_UINT8 payloadLength = data[1];
	GF_UINT16 currPackageId = INVALID_PACKAGE_ID;
	GF_PUINT8 payload = nullptr;
	
	if (packageIdFlag != 0)
	{
		currPackageId = data[2];
		payload = &data[3];
		--payloadLength;
		if (mPackageId == INVALID_PACKAGE_ID)
		{
			mPackageId = currPackageId;
		}
		else
		{
			++mPackageId;
			if (mPackageId > 0xFF)
				mPackageId = 0;
			if (mPackageId != currPackageId)
			{
				GF_LOGE("%s: package id error.", __FUNCTION__);
				mPackageId = currPackageId;
			}
		}
	}
	else
	{
		payload = &data[2];
	}

	switch (evtType)
	{
	case EVENT_QUATERNION:
		onQuaternion(payloadLength, payload);
		break;
	case EVENT_GESTURE:
		onGesture(payloadLength, payload);
		break;
	case EVENT_STATUS:
		onStatus(payloadLength, payload);
	default:
		//GF_LOGE("%s: unknown event ID: %2.2X", __FUNCTION__, evtType);
		;
	}
}

void GForceDevice::onQuaternion(GF_UINT8 length, GF_PUINT8 data)
{
	length; data;
}

void GForceDevice::onGesture(GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD("%s, length: %u", __FUNCTION__, length);
	if (length > 0)
	{
		for (auto& itor : mListeners)
		{
			auto ptr = itor.lock();
			if (nullptr != ptr)
				ptr->onEvent(this, GF_EVT_DATA_GESTURE, 1, data);
		}
	}
}

void GForceDevice::onStatus(GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD("%s, length: %u", __FUNCTION__, length);
	if (length > 0)
	{
		GF_UINT8 status = data[0] & EVENT_RECENTER_MASK;
		if (1 == status)
		{
			for (auto& itor : mListeners)
			{
				auto ptr = itor.lock();
				if (nullptr != ptr)
					ptr->onEvent(this, GF_EVT_DEVICE_RECENTER, 0, nullptr);
			}
		}
	}
}
