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
#include "Utils.h"
#include "BLEDevice.h"
#include "DeviceSettingDataProfile4.h"

using namespace gf;

enum CommandType {
	CMD_GET_PROTOCOL_VERSION = 0x00,
	CMD_GET_FEATURE_MAP = 0x01,
	CMD_GET_DEVICE_NAME = 0x02,
	CMD_GET_MODEL_NUMBER = 0x03,
	CMD_GET_SERIAL_NUMBER = 0x04,
	CMD_GET_HW_REVISION = 0x05,
	CMD_GET_FW_REVISION = 0x06,
	CMD_GET_MANUFACTURER_NAME = 0x07,
	CMD_GET_BOOTLOADER_VERSION = 0x0A,

	CMD_GET_BATTERY_LEVEL = 0x08,
	CMD_GET_TEMPERATURE = 0x09,

	CMD_POWEROFF = 0x1D,
	CMD_SWITCH_TO_OAD = 0x1E,
	CMD_SYSTEM_RESET = 0x1F,
	CMD_SWITCH_SERVICE = 0x20,

	CMD_SET_SWO_LOG_LEVEL = 0x21,
	CMD_SET_SWO_LOG_MODULE = 0x22,
	CMD_PRINT_KERNEL_MSG = 0x23,
	CMD_MOTOR_CONTROL = 0x24,
	CMD_LED_CONTROL_TEST = 0x25,
	CMD_PACKAGE_ID_CONTROL = 0x26,
	CMD_SEND_TRAINING_PACKAGE = 0x27,

	CMD_GET_ACCELERATE_CAP = 0x30,
	CMD_SET_ACCELERATE_CONFIG = 0x31,

	CMD_GET_GYROSCOPE_CAP = 0x32,
	CMD_SET_GYROSCOPE_CONFIG = 0x33,

	CMD_GET_MAGNETOMETER_CAP = 0x34,
	CMD_SET_MAGNETOMETER_CONFIG = 0x35,

	CMD_GET_EULER_ANGLE_CAP = 0x36,
	CMD_SET_EULER_ANGLE_CONFIG = 0x37,

	CMD_GET_QUATERNION_CAP = 0x38,
	CMD_SET_QUATERNION_CONFIG = 0x39,

	CMD_GET_ROTATION_MATRIX_CAP = 0x3A,
	CMD_SET_ROTATION_MATRIX_CONFIG = 0x3B,

	CMD_GET_GESTURE_CAP = 0x3C,
	CMD_SET_GESTURE_CONFIG = 0x3D,

	CMD_GET_EMG_RAWDATA_CAP = 0x3E,
	CMD_SET_EMG_RAWDATA_CONFIG = 0x3F,

	CMD_GET_MOUSE_DATA_CAP = 0x40,
	CMD_SET_MOUSE_DATA_CONFIG = 0x41,

	CMD_GET_JOYSTICK_DATA_CAP = 0x42,
	CMD_SET_JOYSTICK_DATA_CONFIG = 0x43,

	CMD_GET_DEVICE_STATUS_CAP = 0x44,
	CMD_SET_DEVICE_STATUS_CONFIG = 0x45,

	CMD_SET_DATA_NOTIF_SWITCH = 0x4F,
};

enum ResponseCode {
	RC_SUCCESS = 0x00,
	RC_NOT_SUPPORT = 0x01,
	RC_BAD_PARAM = 0x02,
	RC_FAILED = 0x03,
	RC_TIMEOUT = 0xFE,
	RC_MAX = 0xFF,
};

inline ResponseResult responseConvert(ResponseCode code)
{
	switch (code)
	{
	case RC_SUCCESS:
		return ResponseResult::RREST_SUCCESS; break;
	case RC_NOT_SUPPORT:
		return ResponseResult::RREST_NOT_SUPPORT; break;
	case RC_BAD_PARAM:
		return ResponseResult::RREST_BAD_PARAM; break;
	case RC_TIMEOUT:
		return ResponseResult::RREST_TIMEOUT; break;
	case RC_FAILED:
	default:
		return ResponseResult::RREST_FAILED; break;
	}
}
inline ResponseResult responseConvert(GF_UINT32 code)
{
	return responseConvert(static_cast<ResponseCode>(code));
}

struct Dispather {
	using OnRespFunc = void (DeviceSettingDataProfile4::*)(GF_UINT8, GF_UINT8, GF_PUINT8, gfsPtr<void>);
	GF_UINT32 cmd;
	OnRespFunc func;
};

DeviceSettingDataProfile4::DeviceSettingDataProfile4(gfwPtr<BLEDevice> device)
	: DeviceSettingHandle(device)
{
}

DeviceSettingDataProfile4::~DeviceSettingDataProfile4()
{
}

void DeviceSettingDataProfile4::dispatchResponse(GF_UINT8 command, GF_UINT8 retval, GF_UINT8 length,
	GF_PUINT8 data, gfsPtr<void> cb, bool timeout)
{
	const Dispather DispatchTable[] =
	{
		{ CMD_GET_PROTOCOL_VERSION, &DeviceSettingDataProfile4::onProtocolVer },
		{ CMD_GET_FEATURE_MAP, &DeviceSettingDataProfile4::onFeatureMap },
		{ CMD_GET_DEVICE_NAME, &DeviceSettingDataProfile4::onDeviceName },
		{ CMD_GET_MODEL_NUMBER, &DeviceSettingDataProfile4::onModelNumber },
		{ CMD_GET_SERIAL_NUMBER, &DeviceSettingDataProfile4::onSerialNumber },
		{ CMD_GET_HW_REVISION, &DeviceSettingDataProfile4::onHWRevision },
		{ CMD_GET_FW_REVISION, &DeviceSettingDataProfile4::onFWRevision },
		{ CMD_GET_MANUFACTURER_NAME, &DeviceSettingDataProfile4::onManufacturerName },
		{ CMD_GET_BOOTLOADER_VERSION, &DeviceSettingDataProfile4::onBootloaderVer },
		{ CMD_SEND_TRAINING_PACKAGE, &DeviceSettingDataProfile4::onSendTrainingModelData },
		{ CMD_SET_DATA_NOTIF_SWITCH, &DeviceSettingDataProfile4::onSetDataNotifSwitch },
		{ CMD_GET_BATTERY_LEVEL, &DeviceSettingDataProfile4::onGetBatteryLevel },
		{ CMD_GET_TEMPERATURE, &DeviceSettingDataProfile4::onGetTemperature },
		{ CMD_SET_SWO_LOG_LEVEL, &DeviceSettingDataProfile4::onSetLogLevel },
		{ CMD_SET_SWO_LOG_MODULE, &DeviceSettingDataProfile4::onSetLogModuleEnabled },
		{ CMD_PRINT_KERNEL_MSG, &DeviceSettingDataProfile4::onPrintKernelMsg },
		{ CMD_MOTOR_CONTROL, &DeviceSettingDataProfile4::onVibrateControl },
		{ CMD_LED_CONTROL_TEST, &DeviceSettingDataProfile4::onLedControlTest },
		{ CMD_PACKAGE_ID_CONTROL, &DeviceSettingDataProfile4::onPackageIdControl },
		{ CMD_GET_ACCELERATE_CAP, &DeviceSettingDataProfile4::onGetAccelerateCap },
		{ CMD_SET_ACCELERATE_CONFIG, &DeviceSettingDataProfile4::onSetAccelerateConfig },
		{ CMD_GET_GYROSCOPE_CAP, &DeviceSettingDataProfile4::onGetGyroscopeCap },
		{ CMD_SET_GYROSCOPE_CONFIG, &DeviceSettingDataProfile4::onSetGyroscopeConfig },
		{ CMD_GET_MAGNETOMETER_CAP, &DeviceSettingDataProfile4::onGetMagnetometerCap },
		{ CMD_SET_MAGNETOMETER_CONFIG, &DeviceSettingDataProfile4::onSetMagnetometerConfig },
		{ CMD_GET_EULER_ANGLE_CAP, &DeviceSettingDataProfile4::onGetEulerangleCap },
		{ CMD_SET_EULER_ANGLE_CONFIG, &DeviceSettingDataProfile4::onSetEulerangleConfig },
		{ CMD_GET_QUATERNION_CAP, &DeviceSettingDataProfile4::onGetQuaternionCap },
		{ CMD_SET_QUATERNION_CONFIG, &DeviceSettingDataProfile4::onSetQuaternionConfig },
		{ CMD_GET_ROTATION_MATRIX_CAP, &DeviceSettingDataProfile4::onGetRotationMatrixCap },
		{ CMD_SET_ROTATION_MATRIX_CONFIG, &DeviceSettingDataProfile4::onSetRotationMatrixConfig },
		{ CMD_GET_GESTURE_CAP, &DeviceSettingDataProfile4::onGetGestureCap },
		{ CMD_SET_GESTURE_CONFIG, &DeviceSettingDataProfile4::onSetGestureConfig },
		{ CMD_GET_EMG_RAWDATA_CAP, &DeviceSettingDataProfile4::onGetEMGRawDataCap },
		{ CMD_SET_EMG_RAWDATA_CONFIG, &DeviceSettingDataProfile4::onSetEMGRawDataConfig },
		{ CMD_GET_MOUSE_DATA_CAP, &DeviceSettingDataProfile4::onGetMouseDataCap },
		{ CMD_SET_MOUSE_DATA_CONFIG, &DeviceSettingDataProfile4::onSetMouseDataConfig },
		{ CMD_GET_JOYSTICK_DATA_CAP, &DeviceSettingDataProfile4::onGetJoystickDataCap },
		{ CMD_SET_JOYSTICK_DATA_CONFIG, &DeviceSettingDataProfile4::onSetJoystickDataConfig },
		{ CMD_GET_DEVICE_STATUS_CAP, &DeviceSettingDataProfile4::onGetDeviceStatusCap },
		{ CMD_SET_DEVICE_STATUS_CONFIG, &DeviceSettingDataProfile4::onSetDeviceStatusConfig },
	};

	if (timeout)
	{
		retval = RC_TIMEOUT;
	}
	for (auto& i : DispatchTable)
	{
		if (i.cmd == command)
		{
			(this->*(i.func))(retval, length, data, cb);
			break;
		}
	}
}

GF_RET_CODE DeviceSettingDataProfile4::getProtocolVer(function<void(ResponseResult res, tstring version)> cb)
{
	GF_LOGD(__FUNCTION__);
	if (mProtocolVer.size() != 0)
	{
		if (nullptr != cb)
			cb(responseConvert(RC_SUCCESS), mProtocolVer);
		return GF_RET_CODE::GF_SUCCESS;
	}
	GF_UINT8 data = CMD_GET_PROTOCOL_VERSION;
	gfsPtr<function<void(ResponseResult, tstring)>> pcb = make_shared<function<void(ResponseResult, tstring)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(1, &data, true, pvoid);
}
GF_RET_CODE DeviceSettingDataProfile4::getFeatureMap(function<void(ResponseResult res, GF_UINT32 featureMap)> cb)
{
	GF_LOGD(__FUNCTION__);
	if (mFeatureMap != 0)
	{
		if (nullptr != cb)
			cb(responseConvert(RC_SUCCESS), mFeatureMap);
		return GF_RET_CODE::GF_SUCCESS;
	}
	GF_UINT8 data = CMD_GET_FEATURE_MAP;
	gfsPtr<function<void(ResponseResult, GF_UINT32)>> pcb = make_shared<function<void(ResponseResult, GF_UINT32)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(1, &data, true, pvoid);
}
GF_RET_CODE DeviceSettingDataProfile4::getDeviceName(function<void(ResponseResult res, tstring name)> cb)
{
	GF_LOGD(__FUNCTION__);
	if (mDeviceName.size() != 0)
	{
		if (nullptr != cb)
			cb(responseConvert(RC_SUCCESS), mDeviceName);
		return GF_RET_CODE::GF_SUCCESS;
	}
	GF_UINT8 data = CMD_GET_DEVICE_NAME;
	gfsPtr<function<void(ResponseResult, tstring)>> pcb = make_shared<function<void(ResponseResult, tstring)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(1, &data, true, pvoid);
}
GF_RET_CODE DeviceSettingDataProfile4::getModelNumber(function<void(ResponseResult res, tstring modelNumber)> cb)
{
	GF_LOGD(__FUNCTION__);
	if (mModelNumber.size() != 0)
	{
		if (nullptr != cb)
			cb(responseConvert(RC_SUCCESS), mModelNumber);
		return GF_RET_CODE::GF_SUCCESS;
	}
	GF_UINT8 data = CMD_GET_MODEL_NUMBER;
	gfsPtr<function<void(ResponseResult, tstring)>> pcb = make_shared<function<void(ResponseResult, tstring)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(1, &data, true, pvoid);
}
GF_RET_CODE DeviceSettingDataProfile4::getSerialNumber(function<void(ResponseResult res, tstring serialNumber)> cb)
{
	GF_LOGD(__FUNCTION__);
	if (mSerialNumber.size() != 0)
	{
		if (nullptr != cb)
			cb(responseConvert(RC_SUCCESS), mSerialNumber);
		return GF_RET_CODE::GF_SUCCESS;
	}
	GF_UINT8 data = CMD_GET_SERIAL_NUMBER;
	gfsPtr<function<void(ResponseResult, tstring)>> pcb = make_shared<function<void(ResponseResult, tstring)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(1, &data, true, pvoid);
}
GF_RET_CODE DeviceSettingDataProfile4::getHWRevision(function<void(ResponseResult res, tstring version)> cb)
{
	GF_LOGD(__FUNCTION__);
	if (mHWRev.size() != 0)
	{
		if (nullptr != cb)
			cb(responseConvert(RC_SUCCESS), mHWRev);
		return GF_RET_CODE::GF_SUCCESS;
	}
	GF_UINT8 data = CMD_GET_HW_REVISION;
	gfsPtr<function<void(ResponseResult, tstring)>> pcb = make_shared<function<void(ResponseResult, tstring)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(1, &data, true, pvoid);
}
GF_RET_CODE DeviceSettingDataProfile4::getFWRevision(function<void(ResponseResult res, tstring version)> cb)
{
	GF_LOGD(__FUNCTION__);
	if (mFWRev.size() != 0)
	{
		if (nullptr != cb)
			cb(responseConvert(RC_SUCCESS), mFWRev);
		return GF_RET_CODE::GF_SUCCESS;
	}
	GF_UINT8 data = CMD_GET_FW_REVISION;
	gfsPtr<function<void(ResponseResult, tstring)>> pcb = make_shared<function<void(ResponseResult, tstring)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(1, &data, true, pvoid);
}
GF_RET_CODE DeviceSettingDataProfile4::getManufacturerName(function<void(ResponseResult res, tstring name)> cb)
{
	GF_LOGD(__FUNCTION__);
	if (mManuName.size() != 0)
	{
		if (nullptr != cb)
			cb(responseConvert(RC_SUCCESS), mManuName);
		return GF_RET_CODE::GF_SUCCESS;
	}
	GF_UINT8 data = CMD_GET_MANUFACTURER_NAME;
	gfsPtr<function<void(ResponseResult, tstring)>> pcb = make_shared<function<void(ResponseResult, tstring)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(1, &data, true, pvoid);
}
GF_RET_CODE DeviceSettingDataProfile4::getBootloaderVer(function<void(ResponseResult res, tstring version)> cb)
{
	GF_LOGD(__FUNCTION__);
	if (mBootloaderVer.size() != 0)
	{
		if (nullptr != cb)
			cb(responseConvert(RC_SUCCESS), mBootloaderVer);
		return GF_RET_CODE::GF_SUCCESS;
	}
	GF_UINT8 data = CMD_GET_BOOTLOADER_VERSION;
	gfsPtr<function<void(ResponseResult, tstring)>> pcb = make_shared<function<void(ResponseResult, tstring)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(1, &data, true, pvoid);
}

GF_RET_CODE DeviceSettingDataProfile4::powerOff()
{
	GF_LOGD(__FUNCTION__);
	GF_UINT8 data = CMD_POWEROFF;
	return sendCommand(1, &data);
}
GF_RET_CODE DeviceSettingDataProfile4::swithToOAD()
{
	GF_LOGD(__FUNCTION__);
	GF_UINT8 data = CMD_SWITCH_TO_OAD;
	return sendCommand(1, &data);
}
GF_RET_CODE DeviceSettingDataProfile4::systemReset()
{
	GF_LOGD(__FUNCTION__);
	GF_UINT8 data = CMD_SYSTEM_RESET;
	return sendCommand(1, &data);
}

GF_RET_CODE DeviceSettingDataProfile4::switchService(DeviceService service)
{
	GF_LOGD("%s: service = %u", __FUNCTION__, service);
	GF_UINT8 data[2];
	data[0] = CMD_SWITCH_SERVICE;
	data[1] = static_cast<GF_UINT8>(service);
	return sendCommand(2, data);
}

GF_RET_CODE DeviceSettingDataProfile4::setDataNotifSwitch(DataNotifFlags flags, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: flags = 0x%8.8X", __FUNCTION__, flags);
	mCfgApplying.dataFlags = flags;
	const GF_UINT8 length = 5;
	GF_UINT8 data[length];
	data[0] = CMD_SET_DATA_NOTIF_SWITCH;
	data[1] = (GF_UINT8)(flags);
	data[2] = (GF_UINT8)(flags >> 8);
	data[3] = (GF_UINT8)(flags >> 16);
	data[4] = (GF_UINT8)(flags >> 24);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}

///////////////////////////////////////////////
GF_RET_CODE DeviceSettingDataProfile4::sendTrainingModelData(GF_UINT32 length, GF_UINT8 data[], function<void(ResponseResult res, GF_UINT32 percentage)> progress) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
GF_RET_CODE DeviceSettingDataProfile4::getBatteryLevel(function<void(ResponseResult res, GF_UINT32 percentage)> cb)
{
	GF_LOGD(__FUNCTION__);
	GF_UINT8 data = CMD_GET_BATTERY_LEVEL;
	gfsPtr<function<void(ResponseResult, GF_UINT32)>> pcb = make_shared<function<void(ResponseResult, GF_UINT32)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(1, &data, true, pvoid);
}
// C.1
GF_RET_CODE DeviceSettingDataProfile4::getTemperature(function<void(ResponseResult res, GF_UINT32 temperature)> cb)
{
	GF_LOGD(__FUNCTION__);
	GF_UINT8 data = CMD_GET_TEMPERATURE;
	gfsPtr<function<void(ResponseResult, GF_UINT32)>> pcb = make_shared<function<void(ResponseResult, GF_UINT32)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(1, &data, true, pvoid);
}
// C.3
GF_RET_CODE DeviceSettingDataProfile4::setLogLevel(SWOLogLevel level, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: level = %u", __FUNCTION__, level);
	const GF_UINT8 length = 2;
	GF_UINT8 data[length];
	data[0] = CMD_SET_SWO_LOG_LEVEL;
	data[1] = static_cast<GF_UINT8>(level);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.3
GF_RET_CODE DeviceSettingDataProfile4::setLogModuleEnabled(SWOModule modules, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: modules = %u", __FUNCTION__, modules);
	const GF_UINT8 length = 5;
	GF_UINT8 data[length];
	data[0] = CMD_SET_SWO_LOG_MODULE;
	data[1] = (GF_UINT8)(modules);
	data[2] = (GF_UINT8)(modules >> 8);
	data[3] = (GF_UINT8)(modules >> 16);
	data[4] = (GF_UINT8)(modules >> 24);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.3
GF_RET_CODE DeviceSettingDataProfile4::printKernelMsg(KernelMsgType type, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: type = %u", __FUNCTION__, type);
	const GF_UINT8 length = 2;
	GF_UINT8 data[length];
	data[0] = CMD_PRINT_KERNEL_MSG;
	data[1] = static_cast<GF_UINT8>(type);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.4
GF_RET_CODE DeviceSettingDataProfile4::vibrateControl(VibrateControlType type, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: type = %u", __FUNCTION__, type);
	const GF_UINT8 length = 2;
	GF_UINT8 data[length];
	data[0] = CMD_MOTOR_CONTROL;
	data[1] = static_cast<GF_UINT8>(type);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.5
GF_RET_CODE DeviceSettingDataProfile4::ledControlTest(LedControlTestType type, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: type = %u", __FUNCTION__, type);
	const GF_UINT8 length = 2;
	GF_UINT8 data[length];
	data[0] = CMD_LED_CONTROL_TEST;
	data[1] = static_cast<GF_UINT8>(type);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
GF_RET_CODE DeviceSettingDataProfile4::packageIdControl(PackageControlType type, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: %s", __FUNCTION__, type == PackageControlType::Enable ? "Enabled" : "Disabled");
	const GF_UINT8 length = 2;
	GF_UINT8 data[length];
	data[0] = CMD_PACKAGE_ID_CONTROL;
	data[1] = (type == PackageControlType::Enable ? 0x01 : 0x00);
	mCfgApplying.pkgIdCtrl = type;
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}

// C.7
GF_RET_CODE DeviceSettingDataProfile4::getAccelerateCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz,
	GF_UINT8 maxScaleRange_g, GF_UINT8 maxPackageDataLength)> cb)
{
	GF_LOGD("%s", __FUNCTION__);
	const GF_UINT8 length = 1;
	GF_UINT8 data[length];
	data[0] = CMD_GET_ACCELERATE_CAP;
	gfsPtr<function<void(ResponseResult, GF_UINT16, GF_UINT8, GF_UINT8)>> pcb =
		make_shared<function<void(ResponseResult, GF_UINT16, GF_UINT8, GF_UINT8)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.7
GF_RET_CODE DeviceSettingDataProfile4::setAccelerateConfig(GF_UINT16 sampleRateHz,
	GF_UINT8 fullScaleRange_g, GF_UINT8 packageDataLength, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: sample rate = %u(Hz), full scale range = %u(g), package data length = %u(bytes)",
		__FUNCTION__, sampleRateHz, fullScaleRange_g, packageDataLength);
	const GF_UINT8 length = 5;
	GF_UINT8 data[length];
	data[0] = CMD_SET_ACCELERATE_CONFIG;
	data[1] = (GF_UINT8)(sampleRateHz);
	data[2] = (GF_UINT8)(sampleRateHz >> 8);
	data[3] = fullScaleRange_g;
	data[4] = packageDataLength;
	GF_UINT32 value;
	memcpy(&value, &(data[1]), 4);
	mCfgApplying.accelerateCfg = value;
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.8
GF_RET_CODE DeviceSettingDataProfile4::getGyroscopeCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz,
	GF_UINT16 maxScaleRange_dps, GF_UINT8 maxPackageDataLength)> cb)
{
	GF_LOGD("%s", __FUNCTION__);
	const GF_UINT8 length = 1;
	GF_UINT8 data[length];
	data[0] = CMD_GET_GYROSCOPE_CAP;
	gfsPtr<function<void(ResponseResult, GF_UINT16, GF_UINT16, GF_UINT8)>> pcb =
		make_shared<function<void(ResponseResult, GF_UINT16, GF_UINT16, GF_UINT8)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.8
GF_RET_CODE DeviceSettingDataProfile4::setGyroscopeConfig(GF_UINT16 sampleRateHz,
	GF_UINT16 fullScaleRange_dps, GF_UINT8 packageDataLength, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: sample rate = %u(Hz), full scale range = %u(g), package data length = %u(bytes)",
		__FUNCTION__, sampleRateHz, fullScaleRange_dps, packageDataLength);
	const GF_UINT8 length = 6;
	GF_UINT8 data[length];
	data[0] = CMD_SET_GYROSCOPE_CONFIG;
	data[1] = (GF_UINT8)(sampleRateHz);
	data[2] = (GF_UINT8)(sampleRateHz >> 8);
	data[3] = (GF_UINT8)(fullScaleRange_dps);
	data[4] = (GF_UINT8)(fullScaleRange_dps >> 8);
	data[5] = packageDataLength;
	GF_UINT32 hvalue;
	memcpy(&hvalue, &(data[1]), 4);
	GF_UINT64 value = (((GF_UINT64)hvalue) << 32) | packageDataLength;
	mCfgApplying.gyroscopeCfg = value;
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.9
GF_RET_CODE DeviceSettingDataProfile4::getMagnetometerCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz,
	GF_UINT16 maxScaleRange_uT, GF_UINT8 maxPackageDataLength)> cb)
{
	GF_LOGD("%s", __FUNCTION__);
	const GF_UINT8 length = 1;
	GF_UINT8 data[length];
	data[0] = CMD_GET_MAGNETOMETER_CAP;
	gfsPtr<function<void(ResponseResult, GF_UINT16, GF_UINT16, GF_UINT8)>> pcb =
		make_shared<function<void(ResponseResult, GF_UINT16, GF_UINT16, GF_UINT8)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.9
GF_RET_CODE DeviceSettingDataProfile4::setMagnetometerConfig(GF_UINT16 sampleRateHz,
	GF_UINT16 fullScaleRange_uT, GF_UINT8 packageDataLength, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: sample rate = %u(Hz), full scale range = %u(g), package data length = %u(bytes)",
		__FUNCTION__, sampleRateHz, fullScaleRange_uT, packageDataLength);
	const GF_UINT8 length = 6;
	GF_UINT8 data[length];
	data[0] = CMD_SET_MAGNETOMETER_CONFIG;
	data[1] = (GF_UINT8)(sampleRateHz);
	data[2] = (GF_UINT8)(sampleRateHz >> 8);
	data[3] = (GF_UINT8)(fullScaleRange_uT);
	data[4] = (GF_UINT8)(fullScaleRange_uT >> 8);
	data[5] = packageDataLength;
	GF_UINT32 hvalue;
	memcpy(&hvalue, &(data[1]), 4);
	GF_UINT64 value = (((GF_UINT64)hvalue) << 32) | packageDataLength;
	mCfgApplying.magnetometerCfg = value;
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.10
GF_RET_CODE DeviceSettingDataProfile4::getEulerangleCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz)> cb)
{
	GF_LOGD("%s", __FUNCTION__);
	const GF_UINT8 length = 1;
	GF_UINT8 data[length];
	data[0] = CMD_GET_EULER_ANGLE_CAP;
	gfsPtr<function<void(ResponseResult, GF_UINT16)>> pcb =
		make_shared<function<void(ResponseResult, GF_UINT16)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.10
GF_RET_CODE DeviceSettingDataProfile4::setEulerangleConfig(GF_UINT16 sampleRateHz, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: sample rate = %u(Hz)", __FUNCTION__, sampleRateHz);
	const GF_UINT8 length = 3;
	GF_UINT8 data[length];
	data[0] = CMD_SET_EULER_ANGLE_CONFIG;
	data[1] = (GF_UINT8)(sampleRateHz);
	data[2] = (GF_UINT8)(sampleRateHz >> 8);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.11
GF_RET_CODE DeviceSettingDataProfile4::getQuaternionCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz)> cb)
{
	GF_LOGD("%s", __FUNCTION__);
	const GF_UINT8 length = 1;
	GF_UINT8 data[length];
	data[0] = CMD_GET_QUATERNION_CAP;
	gfsPtr<function<void(ResponseResult, GF_UINT16)>> pcb =
		make_shared<function<void(ResponseResult, GF_UINT16)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.11
GF_RET_CODE DeviceSettingDataProfile4::setQuaternionConfig(GF_UINT16 sampleRateHz, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: sample rate = %u(Hz)", __FUNCTION__, sampleRateHz);
	const GF_UINT8 length = 3;
	GF_UINT8 data[length];
	data[0] = CMD_SET_QUATERNION_CONFIG;
	data[1] = (GF_UINT8)(sampleRateHz);
	data[2] = (GF_UINT8)(sampleRateHz >> 8);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.12
GF_RET_CODE DeviceSettingDataProfile4::getRotationMatrixCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz)> cb)
{
	GF_LOGD("%s", __FUNCTION__);
	const GF_UINT8 length = 1;
	GF_UINT8 data[length];
	data[0] = CMD_GET_ROTATION_MATRIX_CAP;
	gfsPtr<function<void(ResponseResult, GF_UINT16)>> pcb =
		make_shared<function<void(ResponseResult, GF_UINT16)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.12
GF_RET_CODE DeviceSettingDataProfile4::setRotationMatrixConfig(GF_UINT16 sampleRateHz, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: sample rate = %u(Hz)", __FUNCTION__, sampleRateHz);
	const GF_UINT8 length = 3;
	GF_UINT8 data[length];
	data[0] = CMD_SET_ROTATION_MATRIX_CONFIG;
	data[1] = (GF_UINT8)(sampleRateHz);
	data[2] = (GF_UINT8)(sampleRateHz >> 8);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.13
GF_RET_CODE DeviceSettingDataProfile4::getGestureCap(function<void(ResponseResult res, GF_UINT32 number, Gesture supportedGestures[])> cb)
{
	GF_LOGD("%s", __FUNCTION__);
	const GF_UINT8 length = 1;
	GF_UINT8 data[length];
	data[0] = CMD_GET_GESTURE_CAP;
	gfsPtr<function<void(ResponseResult, GF_UINT32, Gesture[])>> pcb =
		make_shared<function<void(ResponseResult, GF_UINT32, Gesture[])>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.13
enum GestureMask : GF_UINT32
{
	GM_NONE = 0x00000000,
	GM_RELAX = 0x00000001,
	GM_FIST = 0x00000002,
	GM_SPREAD = 0x00000004,
	GM_WAVEIN = 0x00000008,
	GM_WAVEOUT = 0x00000010,
	GM_PINCH = 0x00000020,
	GM_SHOOT = 0x00000040,
};
GF_RET_CODE DeviceSettingDataProfile4::setGestureConfig(GF_UINT32 number, Gesture interestingGesture[], function<void(ResponseResult res)> cb)
{
	GF_UINT32 mask = GM_NONE;
	for (GF_UINT32 i = 0; i < number; i++)
	{
		switch (interestingGesture[i])
		{
		case Gesture::Relax:
			mask |= GM_RELAX; break;
		case Gesture::Fist:
			mask |= GM_FIST; break;
		case Gesture::SpreadFingers:
			mask |= GM_SPREAD; break;
		case Gesture::WaveIn:
			mask |= GM_WAVEIN; break;
		case Gesture::WaveOut:
			mask |= GM_WAVEOUT; break;
		case Gesture::Pinch:
			mask |= GM_PINCH; break;
		case Gesture::Shoot:
			mask |= GM_SHOOT; break;
		default:
			GF_LOGD("%s: unknown gesture type: %u", __FUNCTION__, static_cast<GF_UINT32>(interestingGesture[i]));
		}
	}
	GF_LOGD("%s: mask = %8.8X", __FUNCTION__, mask);
	const GF_UINT8 length = 5;
	GF_UINT8 data[length];
	data[0] = CMD_SET_GESTURE_CONFIG;
	data[1] = (GF_UINT8)(mask);
	data[2] = (GF_UINT8)(mask >> 8);
	data[3] = (GF_UINT8)(mask >> 16);
	data[4] = (GF_UINT8)(mask >> 24);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.14
GF_RET_CODE DeviceSettingDataProfile4::getEMGRawDataCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz,
	EMGRowDataChannels supportedChannels, GF_UINT8 maxPackageDataLength)> cb)
{
	GF_LOGD("%s", __FUNCTION__);
	const GF_UINT8 length = 1;
	GF_UINT8 data[length];
	data[0] = CMD_GET_EMG_RAWDATA_CAP;
	gfsPtr<function<void(ResponseResult, GF_UINT16, EMGRowDataChannels, GF_UINT8)>> pcb =
		make_shared<function<void(ResponseResult, GF_UINT16, EMGRowDataChannels, GF_UINT8)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.14
GF_RET_CODE DeviceSettingDataProfile4::setEMGRawDataConfig(GF_UINT16 sampleRateHz,
	EMGRowDataChannels interestingChannels, GF_UINT8 packageDataLength, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: sample rate = %u(Hz), channels = %u, package data length = %u(bytes)",
		__FUNCTION__, sampleRateHz, interestingChannels, packageDataLength);
	const GF_UINT8 length = 6;
	GF_UINT8 data[length];
	data[0] = CMD_SET_EMG_RAWDATA_CONFIG;
	data[1] = (GF_UINT8)(sampleRateHz);
	data[2] = (GF_UINT8)(sampleRateHz >> 8);
	data[3] = (GF_UINT8)(interestingChannels);
	data[4] = (GF_UINT8)(interestingChannels >> 8);
	data[5] = packageDataLength;
	GF_UINT32 hvalue;
	memcpy(&hvalue, &(data[1]), 4);
	GF_UINT64 value = (((GF_UINT64)hvalue) << 32) | packageDataLength;
	mCfgApplying.magnetometerCfg = value;
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.15
GF_RET_CODE DeviceSettingDataProfile4::getMouseDataCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz)> cb)
{
	GF_LOGD("%s", __FUNCTION__);
	const GF_UINT8 length = 1;
	GF_UINT8 data[length];
	data[0] = CMD_GET_MOUSE_DATA_CAP;
	gfsPtr<function<void(ResponseResult, GF_UINT16)>> pcb = make_shared<function<void(ResponseResult, GF_UINT16)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.15
GF_RET_CODE DeviceSettingDataProfile4::setMouseDataConfig(GF_UINT16 sampleRateHz, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: sample rate = %u(Hz)", __FUNCTION__, sampleRateHz);
	const GF_UINT8 length = 3;
	GF_UINT8 data[length];
	data[0] = CMD_SET_MOUSE_DATA_CONFIG;
	data[1] = (GF_UINT8)(sampleRateHz);
	data[2] = (GF_UINT8)(sampleRateHz >> 8);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.16
GF_RET_CODE DeviceSettingDataProfile4::getJoystickDataCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz)> cb)
{
	GF_LOGD("%s", __FUNCTION__);
	const GF_UINT8 length = 1;
	GF_UINT8 data[length];
	data[0] = CMD_GET_JOYSTICK_DATA_CAP;
	gfsPtr<function<void(ResponseResult, GF_UINT16)>> pcb = make_shared<function<void(ResponseResult, GF_UINT16)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.16
GF_RET_CODE DeviceSettingDataProfile4::setJoystickDataConfig(GF_UINT16 sampleRateHz, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: sample rate = %u(Hz)", __FUNCTION__, sampleRateHz);
	const GF_UINT8 length = 3;
	GF_UINT8 data[length];
	data[0] = CMD_SET_JOYSTICK_DATA_CONFIG;
	data[1] = (GF_UINT8)(sampleRateHz);
	data[2] = (GF_UINT8)(sampleRateHz >> 8);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.17
GF_RET_CODE DeviceSettingDataProfile4::getDeviceStatusCap(function<void(ResponseResult res, DeviceStatusFlags flags)> cb)
{
	GF_LOGD("%s", __FUNCTION__);
	const GF_UINT8 length = 1;
	GF_UINT8 data[length];
	data[0] = CMD_GET_DEVICE_STATUS_CAP;
	gfsPtr<function<void(ResponseResult, DeviceStatusFlags)>> pcb = make_shared<function<void(ResponseResult, DeviceStatusFlags)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}
// C.17
GF_RET_CODE DeviceSettingDataProfile4::setDeviceStatusConfig(DeviceStatusFlags flags, function<void(ResponseResult res)> cb)
{
	GF_LOGD("%s: flags = 0x%4.4X", __FUNCTION__, flags);
	const GF_UINT8 length = 3;
	GF_UINT8 data[length];
	data[0] = CMD_SET_DEVICE_STATUS_CONFIG;
	data[1] = (GF_UINT8)(flags);
	data[2] = (GF_UINT8)(flags >> 8);
	gfsPtr<function<void(ResponseResult)>> pcb = make_shared<function<void(ResponseResult)>>(cb);
	gfsPtr<void> pvoid = static_pointer_cast<void>(pcb);
	return sendCommand(length, data, true, pvoid);
}



///////////////////////////////////////////////////////////////////////////////////////////////////

tstring DeviceSettingDataProfile4::getStringCommon(GF_UINT8 length, GF_PUINT8 data)
{
	unique_ptr<char[]> up(new char[length + 1]);
	char* str = up.get();
	memcpy(str, data, length);
	str[length] = '\0';
	return utils::totstring(str);
}

void DeviceSettingDataProfile4::onProtocolVer(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	if (retval == RC_SUCCESS)
	{
		if (length >= 2)
		{
			std::ostringstream stm;
			stm << 'V' << (GF_UINT32)data[0] << '.' << (GF_UINT32)data[1];
			mProtocolVer = utils::totstring(stm.str());
			GF_LOGD("%s: %s", __FUNCTION__, stm.str().c_str());
		}
		else
		{
			retval = RC_FAILED;
		}
	}
	gfsPtr<function<void(ResponseResult, tstring)>> callback = static_pointer_cast<function<void(ResponseResult, tstring)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval), mProtocolVer);
}
void DeviceSettingDataProfile4::onFeatureMap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	if (retval == RC_SUCCESS)
	{
		if (length >= 4)
		{
			mFeatureMap = (GF_UINT32)(data[0])
				| ((GF_UINT32)(data[1]) << 8)
				| ((GF_UINT32)(data[2]) << 16)
				| ((GF_UINT32)(data[3]) << 24);
			GF_LOGD("%s: 0x%8.8X", __FUNCTION__, mFeatureMap);
		}
		else
		{
			retval = RC_FAILED;
		}
	}
	gfsPtr<function<void(ResponseResult, GF_UINT32)>> callback = static_pointer_cast<function<void(ResponseResult, GF_UINT32)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval), mFeatureMap);
}
void DeviceSettingDataProfile4::onDeviceName(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	if (retval == RC_SUCCESS)
	{
		if (length > 0)
		{
			mDeviceName = getStringCommon(length, data);
			GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mDeviceName).c_str());
		}
		else
		{
			retval = RC_FAILED;
		}
	}
	gfsPtr<function<void(ResponseResult, tstring)>> callback = static_pointer_cast<function<void(ResponseResult, tstring)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval), mDeviceName);
}
void DeviceSettingDataProfile4::onModelNumber(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	if (retval == RC_SUCCESS)
	{
		if (length > 0)
		{
			mModelNumber = getStringCommon(length, data);
			GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mModelNumber).c_str());
		}
		else
		{
			retval = RC_FAILED;
		}
	}
	gfsPtr<function<void(ResponseResult, tstring)>> callback = static_pointer_cast<function<void(ResponseResult, tstring)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval), mModelNumber);
}
void DeviceSettingDataProfile4::onSerialNumber(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	if (retval == RC_SUCCESS)
	{
		if (length > 0)
		{
			mSerialNumber = getStringCommon(length, data);
			GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mSerialNumber).c_str());
		}
		else
		{
			retval = RC_FAILED;
		}
	}
	gfsPtr<function<void(ResponseResult, tstring)>> callback = static_pointer_cast<function<void(ResponseResult, tstring)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval), mSerialNumber);
}
void DeviceSettingDataProfile4::onHWRevision(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	if (retval == RC_SUCCESS)
	{
		if (length >= 1)
		{
			std::ostringstream stm;
			stm << 'R' << (GF_UINT32)data[0];
			mHWRev = utils::totstring(stm.str());
			GF_LOGD("%s: %s", __FUNCTION__, stm.str().c_str());
		}
		else
		{
			retval = RC_FAILED;
		}
	}
	gfsPtr<function<void(ResponseResult, tstring)>> callback = static_pointer_cast<function<void(ResponseResult, tstring)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval), mHWRev);
}
void DeviceSettingDataProfile4::onFWRevision(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	if (retval == RC_SUCCESS)
	{
		if (length >= 3)
		{
			std::ostringstream stm;
			stm << 'V' << (GF_UINT32)data[0] << '.' << (GF_UINT32)data[1] << '-' << (GF_UINT32)data[2];
			mFWRev = utils::totstring(stm.str());
			GF_LOGD("%s: %s", __FUNCTION__, stm.str().c_str());
		}
		else
		{
			retval = RC_FAILED;
		}
	}
	gfsPtr<function<void(ResponseResult, tstring)>> callback = static_pointer_cast<function<void(ResponseResult, tstring)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval), mFWRev);
}
void DeviceSettingDataProfile4::onManufacturerName(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	if (retval == RC_SUCCESS)
	{
		if (length > 0)
		{
			mManuName = getStringCommon(length, data);
			GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mManuName).c_str());
		}
		else
		{
			retval = RC_FAILED;
		}
	}
	gfsPtr<function<void(ResponseResult, tstring)>> callback = static_pointer_cast<function<void(ResponseResult, tstring)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval), mManuName);
}
void DeviceSettingDataProfile4::onBootloaderVer(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	if (retval == RC_SUCCESS)
	{
		if (length >= 3)
		{
			std::ostringstream stm;
			stm << 'V' << (GF_UINT32)data[0] << '.' << (GF_UINT32)data[1] << '-' << (GF_UINT32)data[2];
			mBootloaderVer = utils::totstring(stm.str());
			GF_LOGD("%s: %s", __FUNCTION__, stm.str().c_str());
		}
		else
		{
			retval = RC_FAILED;
		}
	}
	gfsPtr<function<void(ResponseResult, tstring)>> callback = static_pointer_cast<function<void(ResponseResult, tstring)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval), mBootloaderVer);
}
void DeviceSettingDataProfile4::onSendTrainingModelData(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}

#define MTU_DEFAULT (23)
#define MTU_HEAD_LENGTH (4)
#define MTU_QUATERNION (16)
#define MTU_ROTATIONMATRIX (32)
#define DNF_MTU_ENLARGE_CFG		(DeviceSetting::DNF_ACCELERATE |	\
								DeviceSetting::DNF_GYROSCOPE |		\
								DeviceSetting::DNF_MAGNETOMETER |	\
								DeviceSetting::DNF_ROTATIONMATRIX |	\
								DeviceSetting::DNF_EMG_RAW)

void DeviceSettingDataProfile4::onSetDataNotifSwitch(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	GF_LOGD("%s, retval = %u", __FUNCTION__, retval);
	if (retval == RC_SUCCESS)
	{
		auto applied = mCfgApplying.dataFlags.load();
		auto previous = mCfg.dataFlags.load();
		mCfg.dataFlags = applied;
		// check if need to re-config MTU size
		if ((applied & DNF_MTU_ENLARGE_CFG) !=
			(previous & DNF_MTU_ENLARGE_CFG))
		{
			configMtuSize();
		}
	}
	gfsPtr<function<void(ResponseResult)>> callback = static_pointer_cast<function<void(ResponseResult)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval));
}
void DeviceSettingDataProfile4::onGetBatteryLevel(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	GF_UINT8 val = 0;
	if (retval == RC_SUCCESS)
	{
		if (length >= 1)
		{
			val = data[0];
			GF_LOGD("%s: 0x%8.8X", __FUNCTION__, val);
		}
		else
		{
			retval = RC_FAILED;
		}
	}
	gfsPtr<function<void(ResponseResult, GF_UINT32)>> callback = static_pointer_cast<function<void(ResponseResult, GF_UINT32)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval), val);
}
void DeviceSettingDataProfile4::onGetTemperature(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	GF_UINT8 val = 0;
	if (retval == RC_SUCCESS)
	{
		if (length >= 1)
		{
			// according to the spec, the value should subtract 40
			val = data[0] - 40;
			GF_LOGD("%s: 0x%8.8X", __FUNCTION__, val);
		}
		else
		{
			retval = RC_FAILED;
		}
	}
	gfsPtr<function<void(ResponseResult, GF_UINT32)>> callback = static_pointer_cast<function<void(ResponseResult, GF_UINT32)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval), val);
}
void DeviceSettingDataProfile4::onSetLogLevel(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetLogModuleEnabled(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onPrintKernelMsg(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onVibrateControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	GF_LOGD("%s, retval = %u", __FUNCTION__, retval);
	gfsPtr<function<void(ResponseResult)>> callback = static_pointer_cast<function<void(ResponseResult)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval));
}
void DeviceSettingDataProfile4::onLedControlTest(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	GF_LOGD("%s, retval = %u", __FUNCTION__, retval);
	gfsPtr<function<void(ResponseResult)>> callback = static_pointer_cast<function<void(ResponseResult)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval));
}
void DeviceSettingDataProfile4::onPackageIdControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	GF_LOGD("%s, retval = %u", __FUNCTION__, retval);
	if (retval == RC_SUCCESS)
	{
		auto applied = mCfgApplying.pkgIdCtrl.load();
		auto previous = mCfg.pkgIdCtrl.load();
		mCfg.pkgIdCtrl = applied;
		// check if need to re-config MTU size
		if (applied != previous)
		{
			configMtuSize();
		}
	}
	gfsPtr<function<void(ResponseResult)>> callback = static_pointer_cast<function<void(ResponseResult)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval));
}
void DeviceSettingDataProfile4::onGetAccelerateCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetAccelerateConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	GF_LOGD("%s, retval = %u", __FUNCTION__, retval);
	if (retval == RC_SUCCESS)
	{
		auto applied = mCfgApplying.accelerateCfg.load();
		mCfg.accelerateCfg = applied;
		// check if need to re-config MTU size
		if (mCfg.dataFlags & DataNotifFlags::DNF_ACCELERATE)
		{
			configMtuSize();
		}
	}
	gfsPtr<function<void(ResponseResult)>> callback = static_pointer_cast<function<void(ResponseResult)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval));
}
void DeviceSettingDataProfile4::onGetGyroscopeCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetGyroscopeConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	GF_LOGD("%s, retval = %u", __FUNCTION__, retval);
	if (retval == RC_SUCCESS)
	{
		auto applied = mCfgApplying.gyroscopeCfg.load();
		mCfg.gyroscopeCfg = applied;
		// check if need to re-config MTU size
		if (mCfg.dataFlags & DataNotifFlags::DNF_GYROSCOPE)
		{
			configMtuSize();
		}
	}
	gfsPtr<function<void(ResponseResult)>> callback = static_pointer_cast<function<void(ResponseResult)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval));
}
void DeviceSettingDataProfile4::onGetMagnetometerCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetMagnetometerConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	GF_LOGD("%s, retval = %u", __FUNCTION__, retval);
	if (retval == RC_SUCCESS)
	{
		auto applied = mCfgApplying.magnetometerCfg.load();
		mCfg.magnetometerCfg = applied;
		// check if need to re-config MTU size
		if (mCfg.dataFlags & DataNotifFlags::DNF_MAGNETOMETER)
		{
			configMtuSize();
		}
	}
	gfsPtr<function<void(ResponseResult)>> callback = static_pointer_cast<function<void(ResponseResult)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval));
}
void DeviceSettingDataProfile4::onGetEulerangleCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetEulerangleConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onGetQuaternionCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetQuaternionConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onGetRotationMatrixCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetRotationMatrixConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onGetGestureCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetGestureConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onGetEMGRawDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetEMGRawDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb)
{
	GF_LOGD("%s, retval = %u", __FUNCTION__, retval);
	if (retval == RC_SUCCESS)
	{
		auto applied = mCfgApplying.emgrawCfg.load();
		mCfg.emgrawCfg = applied;
		// check if need to re-config MTU size
		if (mCfg.dataFlags & DataNotifFlags::DNF_EMG_RAW)
		{
			configMtuSize();
		}
	}
	gfsPtr<function<void(ResponseResult)>> callback = static_pointer_cast<function<void(ResponseResult)>>(cb);
	if (callback && *callback.get())
		(*callback.get())(responseConvert(retval));
}
void DeviceSettingDataProfile4::onGetMouseDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetMouseDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onGetJoystickDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetJoystickDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onGetDeviceStatusCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}
void DeviceSettingDataProfile4::onSetDeviceStatusConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb) {}

void DeviceSettingDataProfile4::resetConfig()
{
	mCfg.dataFlags = mCfgApplying.dataFlags = DataNotifFlags::DNF_OFF;
	mCfg.accelerateCfg = mCfgApplying.accelerateCfg = 0;
	mCfg.gyroscopeCfg = mCfgApplying.gyroscopeCfg = 0;
	mCfg.magnetometerCfg = mCfgApplying.magnetometerCfg = 0;
	mCfg.emgrawCfg = mCfgApplying.emgrawCfg = 0;
}

void DeviceSettingDataProfile4::configMtuSize()
{
	auto flags = mCfg.dataFlags.load();
	// MTU_QUATERNION is the most large one in numbers that smaller than (MTU_DEFAULT-MTU_HEAD_LENGTH-PACKAGEID_BYTE)
	GF_UINT16 payloadLength = MTU_QUATERNION;
	// calculate new MTU size
	if (flags & DeviceSetting::DNF_ACCELERATE)
	{
		if (((GF_UINT8)mCfg.accelerateCfg) > payloadLength)
			payloadLength = (GF_UINT8)mCfg.accelerateCfg;
	}
	if (flags & DeviceSetting::DNF_GYROSCOPE)
	{
		if (((GF_UINT8)mCfg.gyroscopeCfg) > payloadLength)
			payloadLength = (GF_UINT8)mCfg.gyroscopeCfg;
	}
	if (flags & DeviceSetting::DNF_MAGNETOMETER)
	{
		if (((GF_UINT8)mCfg.magnetometerCfg) > payloadLength)
			payloadLength = (GF_UINT8)mCfg.magnetometerCfg;
	}
	if (flags & DeviceSetting::DNF_ROTATIONMATRIX)
	{
		if (payloadLength < MTU_ROTATIONMATRIX)
			payloadLength = MTU_ROTATIONMATRIX;
	}
	if (flags & DeviceSetting::DNF_EMG_RAW)
	{
		if (((GF_UINT8)mCfg.emgrawCfg) > payloadLength)
			payloadLength = (GF_UINT8)mCfg.emgrawCfg;
	}
	if (PackageControlType::Enable == mCfg.pkgIdCtrl)
		payloadLength += 1;

	payloadLength += MTU_HEAD_LENGTH;

	GF_UINT16 mtusize = MTU_DEFAULT;
	if (payloadLength > mtusize)
		mtusize = payloadLength;
	auto dev = mDevice.lock();
	if (nullptr != dev)
		dev->configMtuSize(mtusize);
}
