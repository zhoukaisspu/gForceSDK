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
	using OnRespFunc = void (DeviceSettingDataProfile4::*)(GF_UINT8, GF_UINT8, GF_PUINT8);
	GF_UINT32 cmd;
	OnRespFunc func;
};

struct CommandToResponse {
	ResponseType res;
	CommandType cmd;
};

static const CommandToResponse ResponseMapping[] =
{
	{ ResponseType::RESP_GET_BATTERY_LEVEL, CMD_SEND_TRAINING_PACKAGE },
	{ ResponseType::RESP_GET_TEMPERATURE, CMD_SET_DATA_NOTIF_SWITCH },
	{ ResponseType::RESP_SET_SWO_LOG_LEVEL, CMD_GET_BATTERY_LEVEL },
	{ ResponseType::RESP_SET_SWO_LOG_MODULE, CMD_GET_TEMPERATURE },
	{ ResponseType::RESP_PRINT_KERNEL_MSG, CMD_SET_SWO_LOG_LEVEL },
	{ ResponseType::RESP_MOTOR_CONTROL, CMD_SET_SWO_LOG_MODULE },
	{ ResponseType::RESP_LED_CONTROL_TEST, CMD_PRINT_KERNEL_MSG },
	{ ResponseType::RESP_PACKAGE_ID_CONTROL, CMD_MOTOR_CONTROL },
	{ ResponseType::RESP_SEND_TRAINING_PACKAGE, CMD_LED_CONTROL_TEST },
	{ ResponseType::RESP_GET_ACCELERATE_CAP, CMD_PACKAGE_ID_CONTROL },
	{ ResponseType::RESP_SET_ACCELERATE_CONFIG, CMD_GET_ACCELERATE_CAP },
	{ ResponseType::RESP_GET_GYROSCOPE_CAP, CMD_SET_ACCELERATE_CONFIG },
	{ ResponseType::RESP_SET_GYROSCOPE_CONFIG, CMD_GET_GYROSCOPE_CAP },
	{ ResponseType::RESP_GET_MAGNETOMETER_CAP, CMD_SET_GYROSCOPE_CONFIG },
	{ ResponseType::RESP_SET_MAGNETOMETER_CONFIG, CMD_GET_MAGNETOMETER_CAP },
	{ ResponseType::RESP_GET_EULER_ANGLE_CAP, CMD_SET_MAGNETOMETER_CONFIG },
	{ ResponseType::RESP_SET_EULER_ANGLE_CONFIG, CMD_GET_EULER_ANGLE_CAP },
	{ ResponseType::RESP_GET_QUATERNION_CAP, CMD_SET_EULER_ANGLE_CONFIG },
	{ ResponseType::RESP_SET_QUATERNION_CONFIG, CMD_GET_QUATERNION_CAP },
	{ ResponseType::RESP_GET_ROTATION_MATRIX_CAP, CMD_SET_QUATERNION_CONFIG },
	{ ResponseType::RESP_SET_ROTATION_MATRIX_CONFIG, CMD_GET_ROTATION_MATRIX_CAP },
	{ ResponseType::RESP_GET_GESTURE_CAP, CMD_SET_ROTATION_MATRIX_CONFIG },
	{ ResponseType::RESP_SET_GESTURE_CONFIG, CMD_GET_GESTURE_CAP },
	{ ResponseType::RESP_GET_EMG_RAWDATA_CAP, CMD_SET_GESTURE_CONFIG },
	{ ResponseType::RESP_SET_EMG_RAWDATA_CONFIG, CMD_GET_EMG_RAWDATA_CAP },
	{ ResponseType::RESP_GET_MOUSE_DATA_CAP, CMD_SET_EMG_RAWDATA_CONFIG },
	{ ResponseType::RESP_SET_MOUSE_DATA_CONFIG, CMD_GET_MOUSE_DATA_CAP },
	{ ResponseType::RESP_GET_JOYSTICK_DATA_CAP, CMD_SET_MOUSE_DATA_CONFIG },
	{ ResponseType::RESP_SET_JOYSTICK_DATA_CONFIG, CMD_GET_JOYSTICK_DATA_CAP },
	{ ResponseType::RESP_GET_DEVICE_STATUS_CAP, CMD_SET_JOYSTICK_DATA_CONFIG },
	{ ResponseType::RESP_SET_DEVICE_STATUS_CONFIG, CMD_GET_DEVICE_STATUS_CAP },
	{ ResponseType::RESP_SET_DATA_NOTIF_SWITCH, CMD_SET_DEVICE_STATUS_CONFIG },
};


DeviceSettingDataProfile4::DeviceSettingDataProfile4(gfwPtr<BLEDevice> device)
	: DeviceSettingHandle(device)
{
}


DeviceSettingDataProfile4::~DeviceSettingDataProfile4()
{
}

void DeviceSettingDataProfile4::initialize()
{
	// get device descriptions in async mode
	if (!mDeviceInfoReady)
	{
		internalGetProtocolVer();
	}
}

void DeviceSettingDataProfile4::dispatchResponse(GF_UINT8 command, GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, bool timeout)
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
		{ CMD_SEND_TRAINING_PACKAGE, &DeviceSettingDataProfile4::onSendTrainingModelData },
		{ CMD_SET_DATA_NOTIF_SWITCH, &DeviceSettingDataProfile4::onSetDataNotifSwitch },
		{ CMD_GET_BATTERY_LEVEL, &DeviceSettingDataProfile4::onGetBatteryLevel },
		{ CMD_GET_TEMPERATURE, &DeviceSettingDataProfile4::onGetTemperature },
		{ CMD_SET_SWO_LOG_LEVEL, &DeviceSettingDataProfile4::onSetLogLevel },
		{ CMD_SET_SWO_LOG_MODULE, &DeviceSettingDataProfile4::onSetLogModuleEnabled },
		{ CMD_PRINT_KERNEL_MSG, &DeviceSettingDataProfile4::onPrintKernelMsg },
		{ CMD_MOTOR_CONTROL, &DeviceSettingDataProfile4::onVibrateControl },
		{ CMD_LED_CONTROL_TEST, &DeviceSettingDataProfile4::onLedControl },
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
		onTimeout(command);
		return;
	}

	for (auto& i : DispatchTable)
	{
		if (i.cmd == command)
		{
			(this->*(i.func))(retval, length, data);
			break;
		}
	}
}

void DeviceSettingDataProfile4::onTimeout(GF_UINT8 command)
{
	GF_LOGD("%s: command 0x%2.2X", __FUNCTION__, command);
	switch (command)
	{
	case CMD_GET_PROTOCOL_VERSION:
	case CMD_GET_FEATURE_MAP:
	case CMD_GET_DEVICE_NAME:
	case CMD_GET_MODEL_NUMBER:
	case CMD_GET_SERIAL_NUMBER:
	case CMD_GET_HW_REVISION:
	case CMD_GET_FW_REVISION:
	case CMD_GET_MANUFACTURER_NAME:
		// restart the cycle again
		internalGetProtocolVer();
		break;
	default:
		ResponseType resp = ResponseType::RESP_MAX;
		for (auto i : ResponseMapping)
		{
			if (i.cmd == command)
			{
				resp = i.res;
				break;
			}
		}
		if (resp == ResponseType::RESP_MAX)
		{
			GF_LOGD("Unknown command 0x%2.2X", command);
			return;
		}
		mRespHandle(mDevice.lock(), resp, ResponseResult::RREST_TIMEOUT, 0, 0, 0, 0);
	}
}

void DeviceSettingDataProfile4::internalGetProtocolVer() {
	GF_UINT8 data = CMD_GET_PROTOCOL_VERSION;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetFeatureMap();
	}
}
void DeviceSettingDataProfile4::internalGetFeatureMap() {
	GF_UINT8 data = CMD_GET_FEATURE_MAP;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetDeviceName();
	}
}
void DeviceSettingDataProfile4::internalGetDeviceName() {
	GF_UINT8 data = CMD_GET_DEVICE_NAME;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetModelNumber();
	}
}
void DeviceSettingDataProfile4::internalGetModelNumber() {
	GF_UINT8 data = CMD_GET_MODEL_NUMBER;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetSerialNumber();
	}
}
void DeviceSettingDataProfile4::internalGetSerialNumber() {
	GF_UINT8 data = CMD_GET_SERIAL_NUMBER;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetHWRevision();
	}
}
void DeviceSettingDataProfile4::internalGetHWRevision() {
	GF_UINT8 data = CMD_GET_HW_REVISION;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetFWRevision();
	}
}
void DeviceSettingDataProfile4::internalGetFWRevision() {
	GF_UINT8 data = CMD_GET_FW_REVISION;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetManufacturerName();
	}
}
void DeviceSettingDataProfile4::internalGetManufacturerName() {
	GF_UINT8 data = CMD_GET_MANUFACTURER_NAME;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// turn on mDeviceInfoReady
		if (!mDeviceInfoReady)
			mDeviceInfoReady = true;
	}
}

GF_RET_CODE DeviceSettingDataProfile4::getProtocolVer(tstring& version)
{
	GF_LOGD(__FUNCTION__);
	if (!mDeviceInfoReady)
	{
		//internalGetProtocolVer();
		return GF_RET_CODE::GF_ERROR_NOT_READY;
	}
	version = mProtocolVer;
	return GF_RET_CODE::GF_SUCCESS;
}
GF_RET_CODE DeviceSettingDataProfile4::getFeatureMap(GF_UINT32& featureMap)
{
	GF_LOGD(__FUNCTION__);
	if (!mDeviceInfoReady)
	{
		//internalGetFeatureMap();
		return GF_RET_CODE::GF_ERROR_NOT_READY;
	}
	featureMap = mFeatureMap;
	return GF_RET_CODE::GF_SUCCESS;
}
GF_RET_CODE DeviceSettingDataProfile4::getDeviceName(tstring& name)
{
	GF_LOGD(__FUNCTION__);
	if (!mDeviceInfoReady)
	{
		//internalGetDeviceName();
		return GF_RET_CODE::GF_ERROR_NOT_READY;
	}
	name = mDeviceName;
	return GF_RET_CODE::GF_SUCCESS;
}
GF_RET_CODE DeviceSettingDataProfile4::getModelNumber(tstring& modelNumber)
{
	GF_LOGD(__FUNCTION__);
	if (!mDeviceInfoReady)
	{
		//internalGetModelNumber();
		return GF_RET_CODE::GF_ERROR_NOT_READY;
	}
	modelNumber = mModelNumber;
	return GF_RET_CODE::GF_SUCCESS;
}
GF_RET_CODE DeviceSettingDataProfile4::getSerialNumber(tstring& serialNumber)
{
	GF_LOGD(__FUNCTION__);
	if (!mDeviceInfoReady)
	{
		//internalGetSerialNumber();
		return GF_RET_CODE::GF_ERROR_NOT_READY;
	}
	serialNumber = mHWRev;
	return GF_RET_CODE::GF_SUCCESS;
}
GF_RET_CODE DeviceSettingDataProfile4::getHWRevision(tstring& version)
{
	GF_LOGD(__FUNCTION__);
	if (!mDeviceInfoReady)
	{
		//internalGetHWRevision();
		return GF_RET_CODE::GF_ERROR_NOT_READY;
	}
	version = mSerialNumber;
	return GF_RET_CODE::GF_SUCCESS;
}
GF_RET_CODE DeviceSettingDataProfile4::getFWRevision(tstring& version)
{
	GF_LOGD(__FUNCTION__);
	if (!mDeviceInfoReady)
	{
		//internalGetFWRevision();
		return GF_RET_CODE::GF_ERROR_NOT_READY;
	}
	version = mFWRev;
	return GF_RET_CODE::GF_SUCCESS;
}
GF_RET_CODE DeviceSettingDataProfile4::getManufacturerName(tstring& name)
{
	GF_LOGD(__FUNCTION__);
	if (!mDeviceInfoReady)
	{
		//internalGetManufacturerName();
		return GF_RET_CODE::GF_ERROR_NOT_READY;
	}
	name = mManuName;
	return GF_RET_CODE::GF_SUCCESS;
}

GF_RET_CODE DeviceSettingDataProfile4::powerOff()
{
	GF_LOGD(__FUNCTION__);
	GF_UINT8 data = CMD_POWEROFF;
	return sendCommand(1, &data, false);
}
GF_RET_CODE DeviceSettingDataProfile4::swithToOAD()
{
	GF_LOGD(__FUNCTION__);
	GF_UINT8 data = CMD_SWITCH_TO_OAD;
	return sendCommand(1, &data, false);
}
GF_RET_CODE DeviceSettingDataProfile4::systemReset()
{
	GF_LOGD(__FUNCTION__);
	GF_UINT8 data = CMD_SYSTEM_RESET;
	return sendCommand(1, &data, false);
}

GF_RET_CODE DeviceSettingDataProfile4::switchService(DeviceService service)
{
	GF_LOGD("%s: service = %u", __FUNCTION__, service);
	GF_UINT8 data[2];
	data[0] = CMD_SWITCH_SERVICE;
	data[1] = static_cast<GF_UINT8>(service);
	return sendCommand(2, data, false);
}

GF_RET_CODE DeviceSettingDataProfile4::setDataNotifSwitch(DataNotifFlags flags)
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
	return sendCommand(length, data);
}

///////////////////////////////////////////////
GF_RET_CODE DeviceSettingDataProfile4::sendTrainingModelData(GF_UINT32 length, GF_UINT8 data[]) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
GF_RET_CODE DeviceSettingDataProfile4::getBatteryLevel(GF_UINT32& level) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.1
GF_RET_CODE DeviceSettingDataProfile4::getTemperature(GF_UINT32& temprature) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.3
GF_RET_CODE DeviceSettingDataProfile4::setLogLevel(SWOLogLevel level) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.3
GF_RET_CODE DeviceSettingDataProfile4::setLogModuleEnabled(GF_UINT32 modules) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.3
GF_RET_CODE DeviceSettingDataProfile4::printKernelMsg(KernelMsgType type) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.4
GF_RET_CODE DeviceSettingDataProfile4::vibrateControl(VibrateControlType type) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.5
GF_RET_CODE DeviceSettingDataProfile4::ledControl(LedControlType type) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
GF_RET_CODE DeviceSettingDataProfile4::packageIdControl(PackageControlType type)
{
	GF_LOGD("%s: %s", __FUNCTION__, type == PackageControlType::Enable ? "Enabled" : "Disabled");
	const GF_UINT8 length = 2;
	GF_UINT8 data[length];
	data[0] = CMD_PACKAGE_ID_CONTROL;
	data[1] = (type == PackageControlType::Enable ? 0x01 : 0x00);
	mCfgApplying.pkgIdCtrl = type;
	return sendCommand(length, data);
}

// C.7
GF_RET_CODE DeviceSettingDataProfile4::getAccelerateCap(GF_UINT16& maxSampleRateHz,
	GF_UINT8& maxScaleRange_g, GF_UINT8& maxPackageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.7
GF_RET_CODE DeviceSettingDataProfile4::setAccelerateConfig(GF_UINT16 sampleRateHz,
	GF_UINT8 fullScaleRange_g, GF_UINT8 packageDataLength) {
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
	return sendCommand(length, data);
}
// C.8
GF_RET_CODE DeviceSettingDataProfile4::getGyroscopeCap(GF_UINT16& maxSampleRateHz,
	GF_UINT16& maxScaleRange_dps, GF_UINT8& maxPackageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.8
GF_RET_CODE DeviceSettingDataProfile4::setGyroscopeConfig(GF_UINT16 sampleRateHz,
	GF_UINT16 fullScaleRange_dps, GF_UINT8 packageDataLength) {
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
	return sendCommand(length, data);
}
// C.9
GF_RET_CODE DeviceSettingDataProfile4::getMagnetometerCap(GF_UINT16& maxSampleRateHz,
	GF_UINT16& maxScaleRange_uT, GF_UINT8& maxPackageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.9
GF_RET_CODE DeviceSettingDataProfile4::setMagnetometerConfig(GF_UINT16 sampleRateHz,
	GF_UINT16 fullScaleRange_uT, GF_UINT8 packageDataLength) {
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
	return sendCommand(length, data);
}
// C.10
GF_RET_CODE DeviceSettingDataProfile4::getEulerangleCap(GF_UINT16& maxSampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.10
GF_RET_CODE DeviceSettingDataProfile4::setEulerangleConfig(GF_UINT16 sampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.11
GF_RET_CODE DeviceSettingDataProfile4::getQuaternionCap(GF_UINT16& maxSampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.11
GF_RET_CODE DeviceSettingDataProfile4::setQuaternionConfig(GF_UINT16 sampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.12
GF_RET_CODE DeviceSettingDataProfile4::getRotationMatrixCap(GF_UINT16& maxSampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.12
GF_RET_CODE DeviceSettingDataProfile4::setRotationMatrixConfig(GF_UINT16 sampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.13
GF_RET_CODE DeviceSettingDataProfile4::getGestureCap(GF_UINT32& number, Gesture* supportedGesture, GF_UINT32 dataSize) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.13
GF_RET_CODE DeviceSettingDataProfile4::setGestureConfig(GF_UINT32 number, Gesture interestingGesture[]) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.14
GF_RET_CODE DeviceSettingDataProfile4::getEMGRawDataCap(GF_UINT16& maxSampleRateHz,
	EMGRowDataChannels& supportedChannels, GF_UINT8& maxPackageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.14
GF_RET_CODE DeviceSettingDataProfile4::setEMGRawDataConfig(GF_UINT16 sampleRateHz,
	EMGRowDataChannels interestingChannels, GF_UINT8 packageDataLength)
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
	return sendCommand(length, data);
}
// C.15
GF_RET_CODE DeviceSettingDataProfile4::getMouseDataCap(GF_UINT16& maxSampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.15
GF_RET_CODE DeviceSettingDataProfile4::setMouseDataConfig(GF_UINT16 sampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.16
GF_RET_CODE DeviceSettingDataProfile4::getJoystickDataCap(GF_UINT16& maxSampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.16
GF_RET_CODE DeviceSettingDataProfile4::setJoystickDataConfig(GF_UINT16 sampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.17
GF_RET_CODE DeviceSettingDataProfile4::getDeviceStatusCap(DeviceStatusFlags& flags) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.17
GF_RET_CODE DeviceSettingDataProfile4::setDeviceStatusConfig(DeviceStatusFlags flags) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }



///////////////////////////////////////////////////////////////////////////////////////////////////

tstring DeviceSettingDataProfile4::getStringCommon(GF_UINT8 length, GF_PUINT8 data)
{
	unique_ptr<char[]> up = make_unique<char[]>(length + 1);
	char* str = up.get();
	memcpy(str, data, length);
	str[length] = '\0';
	return utils::totstring(str);
}

void DeviceSettingDataProfile4::onProtocolVer(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length >= 2)
	{
		std::ostringstream stm;
		stm << 'V' << (GF_UINT32)data[0] << '.' << (GF_UINT32)data[1];
		mProtocolVer = utils::totstring(stm.str());
		GF_LOGD("%s: %s", __FUNCTION__, stm.str().c_str());
	}
	// come to next
	internalGetFeatureMap();
}
void DeviceSettingDataProfile4::onFeatureMap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length >= 4)
	{
		mFeatureMap = (GF_UINT32)(data[0])
			| ((GF_UINT32)(data[1]) << 8)
			| ((GF_UINT32)(data[2]) << 16)
			| ((GF_UINT32)(data[3]) << 24);
		GF_LOGD("%s: 0x%8.8X", __FUNCTION__, mFeatureMap);
	}
	// come to next
	internalGetDeviceName();
}
void DeviceSettingDataProfile4::onDeviceName(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length > 0)
	{
		mDeviceName = getStringCommon(length, data);
		GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mDeviceName).c_str());
	}
	// come to next
	internalGetModelNumber();
}
void DeviceSettingDataProfile4::onModelNumber(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length > 0)
	{
		mModelNumber = getStringCommon(length, data);
		GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mModelNumber).c_str());
	}
	// come to next
	internalGetSerialNumber();
}
void DeviceSettingDataProfile4::onSerialNumber(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length > 0)
	{
		mSerialNumber = getStringCommon(length, data);
		GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mSerialNumber).c_str());
	}
	// come to next
	internalGetHWRevision();
}
void DeviceSettingDataProfile4::onHWRevision(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length >= 1)
	{
		std::ostringstream stm;
		stm << 'R' << (GF_UINT32)data[0];
		mHWRev = utils::totstring(stm.str());
		GF_LOGD("%s: %s", __FUNCTION__, stm.str().c_str());
	}
	// come to next
	internalGetFWRevision();
}
void DeviceSettingDataProfile4::onFWRevision(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length >= 3)
	{
		std::ostringstream stm;
		stm << 'V' << (GF_UINT32)data[0] << '.' << (GF_UINT32)data[1] << '-' << (GF_UINT32)data[2];
		mFWRev = utils::totstring(stm.str());
		GF_LOGD("%s: %s", __FUNCTION__, stm.str().c_str());
	}
	// come to next
	internalGetManufacturerName();
}
void DeviceSettingDataProfile4::onManufacturerName(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length > 0)
	{
		mManuName = getStringCommon(length, data);
		GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mManuName).c_str());
	}
	mDeviceInfoReady = true;
	// done, notify client
	mRespHandle(mDevice.lock(), ResponseType::RESP_GET_DEVICE_INFO, ResponseResult::RREST_SUCCESS, 0, 0, 0, 0);
}
void DeviceSettingDataProfile4::onSendTrainingModelData(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}

#define MTU_DEFAULT (23)
#define MTU_HEAD_LENGTH (4)
#define MTU_QUATERNION (16)
#define MTU_ROTATIONMATRIX (32)
#define DNF_MTU_ENLARGE_CFG		(DeviceSetting::DNF_ACCELERATE |	\
								DeviceSetting::DNF_GYROSCOPE |		\
								DeviceSetting::DNF_MAGNETOMETER |	\
								DeviceSetting::DNF_ROTATIONMATRIX |	\
								DeviceSetting::DNF_EMG_RAW)

void DeviceSettingDataProfile4::onSetDataNotifSwitch(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
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
	mRespHandle(mDevice.lock(), ResponseType::RESP_SET_DATA_NOTIF_SWITCH, responseConvert(retval), 0, 0, 0, 0);
}
void DeviceSettingDataProfile4::onGetBatteryLevel(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onGetTemperature(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetLogLevel(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetLogModuleEnabled(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onPrintKernelMsg(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onVibrateControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onLedControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onPackageIdControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
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
	mRespHandle(mDevice.lock(), ResponseType::RESP_PACKAGE_ID_CONTROL, responseConvert(retval), 0, 0, 0, 0);
}
void DeviceSettingDataProfile4::onGetAccelerateCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetAccelerateConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
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
	mRespHandle(mDevice.lock(), ResponseType::RESP_SET_ACCELERATE_CONFIG, responseConvert(retval), 0, 0, 0, 0);
}
void DeviceSettingDataProfile4::onGetGyroscopeCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetGyroscopeConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
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
	mRespHandle(mDevice.lock(), ResponseType::RESP_SET_GYROSCOPE_CONFIG, responseConvert(retval), 0, 0, 0, 0);
}
void DeviceSettingDataProfile4::onGetMagnetometerCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetMagnetometerConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
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
	mRespHandle(mDevice.lock(), ResponseType::RESP_SET_MAGNETOMETER_CONFIG	, responseConvert(retval), 0, 0, 0, 0);
}
void DeviceSettingDataProfile4::onGetEulerangleCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetEulerangleConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onGetQuaternionCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetQuaternionConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onGetRotationMatrixCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetRotationMatrixConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onGetGestureCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetGestureConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onGetEMGRawDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetEMGRawDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
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
	mRespHandle(mDevice.lock(), ResponseType::RESP_SET_EMG_RAWDATA_CONFIG, responseConvert(retval), 0, 0, 0, 0);
}
void DeviceSettingDataProfile4::onGetMouseDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetMouseDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onGetJoystickDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetJoystickDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onGetDeviceStatusCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile4::onSetDeviceStatusConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}

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
