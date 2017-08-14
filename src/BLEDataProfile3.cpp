/*
* Copyright 2017, OYMotion Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*/
#include "LogUtils.h"
#include "BLEDataProfile3.h"
#include "DeviceSettingDataProfile3.h"

using namespace gf;

BLEDataProfile3::BLEDataProfile3(gfwPtr<BLEDevice> device)
	: DeviceProfile(device)
{
}

BLEDataProfile3::~BLEDataProfile3()
{
}

void BLEDataProfile3::onData(GF_UINT8 length, GF_PUINT8 data)
{
	//GF_LOGD("%s", __FUNCTION__);
	gfsPtr<BLEDevice> device = mDevice.lock();
	if (nullptr == device)
		return;
	if (length <= 1)
		return;

	BLEDevice& ref = *device.get();
	GF_UINT32 offset = 0;
	GF_UINT8 dataHeader = data[offset++];
	GF_UINT8 dataType = dataHeader & EVENT_MASK;
	GF_UINT8 packageIdFlag = (dataHeader >> 7) & PCKID_FLAG_MASK;

	GF_UINT16 currPackageId = INVALID_PACKAGE_ID;
	if (packageIdFlag != 0)
	{
		currPackageId = data[offset++];

		lock_guard<mutex> lock(device->getLockable());
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
				GF_LOGE("%s:%s: package id error. id supposed is %u, but now is %u, gap is %u", __FUNCTION__,
					utils::tostring(ref.getName()).c_str(), mPackageId, currPackageId, (GF_UINT8)(currPackageId - mPackageId));
				mPackageId = currPackageId;
			}
		}
	}

	GF_PUINT8 payload = data + offset;
	GF_UINT32 lenPayload = length - offset;

	switch (dataType)
	{
	case DT_ACCELERATE:
		onAccelerateData(ref, lenPayload, payload); break;
	case DT_GYROSCOPE:
		onGyroscopeData(ref, lenPayload, payload); break;
	case DT_MAGNETOMETER:
		onMagnetometerData(ref, lenPayload, payload); break;
	case DT_EULERANGLE:
		onEulerAngleData(ref, lenPayload, payload); break;
	case DT_QUATERNION:
		onQuaternionData(ref, lenPayload, payload); break;
	case DT_ROTATIONMATRIX:
		onRotationMatrixData(ref, lenPayload, payload); break;
	case DT_GESTURE:
		onGestureData(ref, lenPayload, payload); break;
	case DT_EMGRAW:
		onEmgRawData(ref, lenPayload, payload); break;
	case DT_HIDMOUSE:
		onHIDMouseData(ref, lenPayload, payload); break;
	case DT_HIDJOYSTICK:
		onHIDJoystickData(ref, lenPayload, payload); break;
	case DT_DEVICESTATUS:
		onDeviceStatusData(ref, lenPayload, payload); break;
	default:
		;//GF_LOGE("Unknown data type: 0x%2.2X", dataType);
	}
}

void BLEDataProfile3::onResponse(GF_UINT8 length, GF_PUINT8 data)
{
	if (nullptr != mDevSetting)
		mDevSetting->onResponse(length, data);
}

gfsPtr<DeviceSetting> BLEDataProfile3::getDeviceSetting()
{
	GF_LOGD("%s", __FUNCTION__);
	gfsPtr<BLEDevice> device = mDevice.lock();
	if (nullptr == device)
	{
		mDevSetting.reset();
		return gfsPtr<DeviceSetting>();
	}

	if (nullptr == mDevSetting)
	{
		mDevSetting = make_shared<DeviceSettingDataProfile3>(device);
		mDevSetting->initialize();
	}

	return mDevSetting;
}


void BLEDataProfile3::onAccelerateData(BLEDevice& device, GF_UINT8 length, GF_PUINT8 data)
{

}

void BLEDataProfile3::onGyroscopeData(BLEDevice& device, GF_UINT8 length, GF_PUINT8 data)
{

}

void BLEDataProfile3::onMagnetometerData(BLEDevice& device, GF_UINT8 length, GF_PUINT8 data)
{

}

void BLEDataProfile3::onEulerAngleData(BLEDevice& device, GF_UINT8 length, GF_PUINT8 data)
{

}

void BLEDataProfile3::onQuaternionData(BLEDevice& device, GF_UINT8 length, GF_PUINT8 data)
{
	if (length < 16)
	{
		GF_LOGD("%s, length: %u, data insufficient.", __FUNCTION__, length);
		return;
	}
	GF_FLOAT f[4];
	// prevent align issue
	memcpy(f, data, sizeof(f));
	Quaternion q(f[0], f[1], f[2], f[3]);
	// GF_LOGD("Device: %s, Quaternion: %s", utils::tostring(getName()).c_str(), q.toString().c_str());

	device.getHub().notifyOrientationData(device, q);
}

void BLEDataProfile3::onRotationMatrixData(BLEDevice& device, GF_UINT8 length, GF_PUINT8 data)
{

}

void BLEDataProfile3::onGestureData(BLEDevice& device, GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD("%s, length: %u", __FUNCTION__, length);
	if (length < 1)
		return;

	Gesture gesture;
	switch (data[0])
	{
	case static_cast<GF_UINT8>(Gesture::Relax) :
		gesture = Gesture::Relax;
		break;
	case static_cast<GF_UINT8>(Gesture::Fist) :
		gesture = Gesture::Fist;
		break;
	case static_cast<GF_UINT8>(Gesture::SpreadFingers) :
		gesture = Gesture::SpreadFingers;
		break;
	case static_cast<GF_UINT8>(Gesture::WaveIn) :
		gesture = Gesture::WaveIn;
		break;
	case static_cast<GF_UINT8>(Gesture::WaveOut) :
		gesture = Gesture::WaveOut;
		break;
	case static_cast<GF_UINT8>(Gesture::Pinch) :
		gesture = Gesture::Pinch;
		break;
	case static_cast<GF_UINT8>(Gesture::Shoot) :
		gesture = Gesture::Shoot;
		break;
	case static_cast<GF_UINT8>(Gesture::Undefined) :
	default:
		gesture = Gesture::Undefined;
	}
	device.getHub().notifyGestureData(device, gesture);
}

void BLEDataProfile3::onEmgRawData(BLEDevice& device, GF_UINT8 length, GF_PUINT8 data)
{

}

void BLEDataProfile3::onHIDMouseData(BLEDevice& device, GF_UINT8 length, GF_PUINT8 data)
{

}

void BLEDataProfile3::onHIDJoystickData(BLEDevice& device, GF_UINT8 length, GF_PUINT8 data)
{

}

void BLEDataProfile3::onDeviceStatusData(BLEDevice& device, GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD("%s, length: %u", __FUNCTION__, length);
	if (length < 1)
		return;

	GF_UINT8 status = data[0];
	if (DSE_RECENTER == status)
	{
		device.getHub().notifyReCenter(device);
	}
}
