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
#include "DeviceSettingDataProfile3.h"

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

struct Dispather {
	using OnRespFunc = void (DeviceSettingDataProfile3::*)(GF_UINT8, GF_UINT8, GF_PUINT8);
	GF_UINT32 cmd;
	OnRespFunc func;
};


DeviceSettingDataProfile3::DeviceSettingDataProfile3(gfwPtr<BLEDevice> device)
	: DeviceSettingHandle(device)
{
}


DeviceSettingDataProfile3::~DeviceSettingDataProfile3()
{
}

void DeviceSettingDataProfile3::initialize()
{
	// get device descriptions in async mode
	if (!mDeviceInfoReady)
	{
		internalGetProtocolVer();
	}
}

void DeviceSettingDataProfile3::dispatchResponse(GF_UINT8 command, GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, bool timeout)
{
	const Dispather DispatchTable[] =
	{
		{ CMD_GET_PROTOCOL_VERSION, &DeviceSettingDataProfile3::onProtocolVer },
		{ CMD_GET_FEATURE_MAP, &DeviceSettingDataProfile3::onFeatureMap },
		{ CMD_GET_DEVICE_NAME, &DeviceSettingDataProfile3::onDeviceName },
		{ CMD_GET_MODEL_NUMBER, &DeviceSettingDataProfile3::onModelNumber },
		{ CMD_GET_SERIAL_NUMBER, &DeviceSettingDataProfile3::onSerialNumber },
		{ CMD_GET_HW_REVISION, &DeviceSettingDataProfile3::onHWRevision },
		{ CMD_GET_FW_REVISION, &DeviceSettingDataProfile3::onFWRevision },
		{ CMD_GET_MANUFACTURER_NAME, &DeviceSettingDataProfile3::onManufacturerName },
		{ CMD_SEND_TRAINING_PACKAGE, &DeviceSettingDataProfile3::onSendTrainingModelData },
		{ CMD_SET_DATA_NOTIF_SWITCH, &DeviceSettingDataProfile3::onSetDataNotifSwitch },
		{ CMD_GET_BATTERY_LEVEL, &DeviceSettingDataProfile3::onGetBatteryLevel },
		{ CMD_GET_TEMPERATURE, &DeviceSettingDataProfile3::onGetTemperature },
		{ CMD_SET_SWO_LOG_LEVEL, &DeviceSettingDataProfile3::onSetLogLevel },
		{ CMD_SET_SWO_LOG_MODULE, &DeviceSettingDataProfile3::onSetLogModuleEnabled },
		{ CMD_PRINT_KERNEL_MSG, &DeviceSettingDataProfile3::onPrintKernelMsg },
		{ CMD_MOTOR_CONTROL, &DeviceSettingDataProfile3::onVibrateControl },
		{ CMD_LED_CONTROL_TEST, &DeviceSettingDataProfile3::onLedControl },
		{ CMD_PACKAGE_ID_CONTROL, &DeviceSettingDataProfile3::onPackageIdControl },
		{ CMD_GET_ACCELERATE_CAP, &DeviceSettingDataProfile3::onGetAccelerateCap },
		{ CMD_SET_ACCELERATE_CONFIG, &DeviceSettingDataProfile3::onSetAccelerateConfig },
		{ CMD_GET_GYROSCOPE_CAP, &DeviceSettingDataProfile3::onGetGyroscopeCap },
		{ CMD_SET_GYROSCOPE_CONFIG, &DeviceSettingDataProfile3::onGetGyroscopeConfig },
		{ CMD_GET_MAGNETOMETER_CAP, &DeviceSettingDataProfile3::onGetMagnetometerCap },
		{ CMD_SET_MAGNETOMETER_CONFIG, &DeviceSettingDataProfile3::onGetMagnetometerConfig },
		{ CMD_GET_EULER_ANGLE_CAP, &DeviceSettingDataProfile3::onGetEulerangleCap },
		{ CMD_SET_EULER_ANGLE_CONFIG, &DeviceSettingDataProfile3::onSetEulerangleConfig },
		{ CMD_GET_QUATERNION_CAP, &DeviceSettingDataProfile3::onGetQuaternionCap },
		{ CMD_SET_QUATERNION_CONFIG, &DeviceSettingDataProfile3::onSetQuaternionConfig },
		{ CMD_GET_ROTATION_MATRIX_CAP, &DeviceSettingDataProfile3::onGetRotationMatrixCap },
		{ CMD_SET_ROTATION_MATRIX_CONFIG, &DeviceSettingDataProfile3::onSetRotationMatrixConfig },
		{ CMD_GET_GESTURE_CAP, &DeviceSettingDataProfile3::onGetGestureCap },
		{ CMD_SET_GESTURE_CONFIG, &DeviceSettingDataProfile3::onSetGestureConfig },
		{ CMD_GET_EMG_RAWDATA_CAP, &DeviceSettingDataProfile3::onGetEMGRawDataCap },
		{ CMD_SET_EMG_RAWDATA_CONFIG, &DeviceSettingDataProfile3::onSetEMGRawDataConfig },
		{ CMD_GET_MOUSE_DATA_CAP, &DeviceSettingDataProfile3::onGetMouseDataCap },
		{ CMD_SET_MOUSE_DATA_CONFIG, &DeviceSettingDataProfile3::onSetMouseDataConfig },
		{ CMD_GET_JOYSTICK_DATA_CAP, &DeviceSettingDataProfile3::onGetJoystickDataCap },
		{ CMD_SET_JOYSTICK_DATA_CONFIG, &DeviceSettingDataProfile3::onSetJoystickDataConfig },
		{ CMD_GET_DEVICE_STATUS_CAP, &DeviceSettingDataProfile3::onGetDeviceStatusCap },
		{ CMD_SET_DEVICE_STATUS_CONFIG, &DeviceSettingDataProfile3::onSetDeviceStatusConfig },
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

void DeviceSettingDataProfile3::onTimeout(GF_UINT8 command)
{
	GF_LOGD("%s: command 0x%2.2X", __FUNCTION__, command);
}

void DeviceSettingDataProfile3::internalGetProtocolVer() {
	GF_UINT8 data = CMD_GET_PROTOCOL_VERSION;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetFeatureMap();
	}
}
void DeviceSettingDataProfile3::internalGetFeatureMap() {
	GF_UINT8 data = CMD_GET_FEATURE_MAP;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetDeviceName();
	}
}
void DeviceSettingDataProfile3::internalGetDeviceName() {
	GF_UINT8 data = CMD_GET_DEVICE_NAME;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetModelNumber();
	}
}
void DeviceSettingDataProfile3::internalGetModelNumber() {
	GF_UINT8 data = CMD_GET_MODEL_NUMBER;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetSerialNumber();
	}
}
void DeviceSettingDataProfile3::internalGetSerialNumber() {
	GF_UINT8 data = CMD_GET_SERIAL_NUMBER;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetHWRevision();
	}
}
void DeviceSettingDataProfile3::internalGetHWRevision() {
	GF_UINT8 data = CMD_GET_HW_REVISION;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetFWRevision();
	}
}
void DeviceSettingDataProfile3::internalGetFWRevision() {
	GF_UINT8 data = CMD_GET_FW_REVISION;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// try next
		internalGetManufacturerName();
	}
}
void DeviceSettingDataProfile3::internalGetManufacturerName() {
	GF_UINT8 data = CMD_GET_MANUFACTURER_NAME;
	GF_RET_CODE ret = sendCommand(1, &data);
	if (GF_RET_CODE::GF_SUCCESS != ret)
	{
		// turn on mDeviceInfoReady
		if (!mDeviceInfoReady)
			mDeviceInfoReady = true;
	}
}

GF_RET_CODE DeviceSettingDataProfile3::getProtocolVer(tstring& version)
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
GF_RET_CODE DeviceSettingDataProfile3::getFeatureMap(GF_UINT32& featureMap)
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
GF_RET_CODE DeviceSettingDataProfile3::getDeviceName(tstring& name)
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
GF_RET_CODE DeviceSettingDataProfile3::getModelNumber(tstring& modelNumber)
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
GF_RET_CODE DeviceSettingDataProfile3::getSerialNumber(tstring& serialNumber)
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
GF_RET_CODE DeviceSettingDataProfile3::getHWRevision(tstring& version)
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
GF_RET_CODE DeviceSettingDataProfile3::getFWRevision(tstring& version)
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
GF_RET_CODE DeviceSettingDataProfile3::getManufacturerName(tstring& name)
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

GF_RET_CODE DeviceSettingDataProfile3::powerOff()
{
	GF_LOGD(__FUNCTION__);
	GF_UINT8 data = CMD_POWEROFF;
	return sendCommand(1, &data, false);
}
GF_RET_CODE DeviceSettingDataProfile3::swithToOAD()
{
	GF_LOGD(__FUNCTION__);
	GF_UINT8 data = CMD_SWITCH_TO_OAD;
	return sendCommand(1, &data, false);
}
GF_RET_CODE DeviceSettingDataProfile3::systemReset()
{
	GF_LOGD(__FUNCTION__);
	GF_UINT8 data = CMD_SYSTEM_RESET;
	return sendCommand(1, &data, false);
}

GF_RET_CODE DeviceSettingDataProfile3::switchService(DeviceService service)
{
	GF_LOGD("%s: service = %u", __FUNCTION__, service);
	GF_UINT8 data[2];
	data[0] = CMD_SWITCH_SERVICE;
	data[1] = static_cast<GF_UINT8>(service);
	return sendCommand(2, data, false);
}

GF_RET_CODE DeviceSettingDataProfile3::setDataNotifSwitch(DataNotifFlags flags)
{
	GF_LOGD("%s: flags = 0x%8.8X", __FUNCTION__, flags);
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
GF_RET_CODE DeviceSettingDataProfile3::sendTrainingModelData(GF_UINT32 length, GF_UINT8 data[]) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
GF_RET_CODE DeviceSettingDataProfile3::getBatteryLevel(GF_UINT32& level) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.1
GF_RET_CODE DeviceSettingDataProfile3::getTemperature(GF_UINT32& temprature) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.3
GF_RET_CODE DeviceSettingDataProfile3::setLogLevel(SWOLogLevel level) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.3
GF_RET_CODE DeviceSettingDataProfile3::setLogModuleEnabled(GF_UINT32 modules) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.3
GF_RET_CODE DeviceSettingDataProfile3::printKernelMsg(KernelMsgType type) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.4
GF_RET_CODE DeviceSettingDataProfile3::vibrateControl(VibrateControlType type) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.5
GF_RET_CODE DeviceSettingDataProfile3::ledControl(LedControlType type) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
GF_RET_CODE DeviceSettingDataProfile3::packageIdControl(PackageControlType type) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }

// C.7
GF_RET_CODE DeviceSettingDataProfile3::getAccelerateCap(GF_UINT32& maxSampleRateHz,
	GF_UINT32& maxScaleRage_g, GF_UINT32& maxPackageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.7
GF_RET_CODE DeviceSettingDataProfile3::setAccelerateConfig(GF_UINT32 sampleRateHz,
	GF_UINT32 fullScaleRage_g, GF_UINT32 packageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.8
GF_RET_CODE DeviceSettingDataProfile3::getGyroscopeCap(GF_UINT32& maxSampleRateHz,
	GF_UINT32& maxScaleRage_dps, GF_UINT32& maxPackageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.8
GF_RET_CODE DeviceSettingDataProfile3::getGyroscopeConfig(GF_UINT32 sampleRateHz,
	GF_UINT32 fullScaleRage_dps, GF_UINT32 packageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.9
GF_RET_CODE DeviceSettingDataProfile3::getMagnetometerCap(GF_UINT32& maxSampleRateHz,
	GF_UINT32& maxScaleRage_uT, GF_UINT32& maxPackageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.9
GF_RET_CODE DeviceSettingDataProfile3::getMagnetometerConfig(GF_UINT32 sampleRateHz,
	GF_UINT32 fullScaleRage_uT, GF_UINT32 packageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.10
GF_RET_CODE DeviceSettingDataProfile3::getEulerangleCap(GF_UINT32& maxSampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.10
GF_RET_CODE DeviceSettingDataProfile3::setEulerangleConfig(GF_UINT32 sampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.11
GF_RET_CODE DeviceSettingDataProfile3::getQuaternionCap(GF_UINT32& maxSampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.11
GF_RET_CODE DeviceSettingDataProfile3::setQuaternionConfig(GF_UINT32 sampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.12
GF_RET_CODE DeviceSettingDataProfile3::getRotationMatrixCap(GF_UINT32& maxSampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.12
GF_RET_CODE DeviceSettingDataProfile3::setRotationMatrixConfig(GF_UINT32 sampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.13
GF_RET_CODE DeviceSettingDataProfile3::getGestureCap(GF_UINT32& number, Gesture* supportedGesture, GF_UINT32 dataSize) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.13
GF_RET_CODE DeviceSettingDataProfile3::setGestureConfig(GF_UINT32 number, Gesture interestingGesture[]) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.14
GF_RET_CODE DeviceSettingDataProfile3::getEMGRawDataCap(GF_UINT32& maxSampleRateHz,
	EMGRowDataChannels& supportedChannels, GF_UINT32& maxPackageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.14
GF_RET_CODE DeviceSettingDataProfile3::setEMGRawDataConfig(GF_UINT32 sampleRateHz,
	EMGRowDataChannels interestingChannels, GF_UINT32 packageDataLength) {
	return GF_RET_CODE::GF_ERROR_NOT_SUPPORT;
}
// C.15
GF_RET_CODE DeviceSettingDataProfile3::getMouseDataCap(GF_UINT32& maxSampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.15
GF_RET_CODE DeviceSettingDataProfile3::setMouseDataConfig(GF_UINT32 sampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.16
GF_RET_CODE DeviceSettingDataProfile3::getJoystickDataCap(GF_UINT32& maxSampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.16
GF_RET_CODE DeviceSettingDataProfile3::setJoystickDataConfig(GF_UINT32 sampleRateHz) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.17
GF_RET_CODE DeviceSettingDataProfile3::getDeviceStatusCap(DeviceStatusFlags& flags) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
// C.17
GF_RET_CODE DeviceSettingDataProfile3::setDeviceStatusConfig(DeviceStatusFlags flags) { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }



///////////////////////////////////////////////////////////////////////////////////////////////////

tstring DeviceSettingDataProfile3::getStringCommon(GF_UINT8 length, GF_PUINT8 data)
{
	unique_ptr<char[]> up(new char[length + 1]);
	char* str = up.get();
	memcpy(str, data, length);
	str[length] = '\0';
	return utils::totstring(str);
}

void DeviceSettingDataProfile3::onProtocolVer(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length >= 2)
	{
		std::ostringstream stm;
		stm << (GF_UINT32)data[0] << '.' << (GF_UINT32)data[1];
		mProtocolVer = utils::totstring(stm.str());
		GF_LOGD("%s: %s", __FUNCTION__, stm.str().c_str());
	}
	// come to next
	//internalGetFeatureMap();
}
void DeviceSettingDataProfile3::onFeatureMap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
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
	//internalGetDeviceName();
}
void DeviceSettingDataProfile3::onDeviceName(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length > 0)
	{
		mDeviceName = getStringCommon(length, data);
		GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mDeviceName).c_str());
	}
	// come to next
	//internalGetModelNumber();
}
void DeviceSettingDataProfile3::onModelNumber(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length > 0)
	{
		mModelNumber = getStringCommon(length, data);
		GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mModelNumber).c_str());
	}
	// come to next
	//internalGetSerialNumber();
}
void DeviceSettingDataProfile3::onSerialNumber(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length > 0)
	{
		mSerialNumber = getStringCommon(length, data);
		GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mSerialNumber).c_str());
	}
	// come to next
	//internalGetHWRevision();
}
void DeviceSettingDataProfile3::onHWRevision(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length >= 1)
	{
		std::ostringstream stm;
		stm << (GF_UINT32)data[0];
		mHWRev = utils::totstring(stm.str());
		GF_LOGD("%s: %s", __FUNCTION__, stm.str().c_str());
	}
	// come to next
	//internalGetFWRevision();
}
void DeviceSettingDataProfile3::onFWRevision(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length >= 3)
	{
		std::ostringstream stm;
		stm << (GF_UINT32)data[0] << '.' << (GF_UINT32)data[1] << '-' << (GF_UINT32)data[2];
		mFWRev = utils::totstring(stm.str());
		GF_LOGD("%s: %s", __FUNCTION__, stm.str().c_str());
	}
	// come to next
	//internalGetManufacturerName();
}
void DeviceSettingDataProfile3::onManufacturerName(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	if (retval == RC_SUCCESS && length > 0)
	{
		mManuName = getStringCommon(length, data);
		GF_LOGD("%s: %s", __FUNCTION__, utils::tostring(mManuName).c_str());
	}
	//mDeviceInfoReady = true;
	// done, notify client
	gfsPtr<BLEDevice> sp = mDevice.lock();
	if (nullptr != sp.get())
	{
		//sp->getHub().xxx();
	}
}

void DeviceSettingDataProfile3::onSendTrainingModelData(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetDataNotifSwitch(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data)
{
	GF_LOGD("%s, retval = %u", __FUNCTION__, retval);
}
void DeviceSettingDataProfile3::onGetBatteryLevel(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetTemperature(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetLogLevel(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetLogModuleEnabled(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onPrintKernelMsg(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onVibrateControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onLedControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onPackageIdControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetAccelerateCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetAccelerateConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetGyroscopeCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetGyroscopeConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetMagnetometerCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetMagnetometerConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetEulerangleCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetEulerangleConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetQuaternionCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetQuaternionConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetRotationMatrixCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetRotationMatrixConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetGestureCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetGestureConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetEMGRawDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetEMGRawDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetMouseDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetMouseDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetJoystickDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetJoystickDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onGetDeviceStatusCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
void DeviceSettingDataProfile3::onSetDeviceStatusConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data) {}
