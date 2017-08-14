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
#pragma once

#include "DeviceSetting.h"
#include "Utils.h"
#include <map>

namespace gf
{
	class BLEDevice;

	const GF_UINT32 MAX_COMMAND_TIMEOUT = 5000; // ms

	class DeviceSettingHandle : public DeviceSetting
	{
	public:
		DeviceSettingHandle(gfwPtr<BLEDevice> device);
		virtual ~DeviceSettingHandle();
		void onResponse(GF_UINT8 length, GF_PUINT8 data);

	public:
		virtual GF_RET_CODE getProtocolVer(tstring& version) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getFeatureMap(GF_UINT32& featureMap) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getDeviceName(tstring& name) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getModelNumber(tstring& modelNumber) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getSerialNumber(tstring& serialNumber) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getHWRevision(tstring& version) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getFWRevision(tstring& version) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getManufacturerName(tstring& name) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE powerOff() override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE swithToOAD() override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE systemReset() override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE switchService(DeviceService service) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE oadUpgrade(FILE* file, function<void(GF_UINT32 percentage)> progress) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE sendTrainingModelData(GF_UINT32 length, GF_UINT8 data[]) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setDataNotifSwitch(DataNotifFlags flags) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getBatteryLevel(GF_UINT32& level) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getTemperature(GF_UINT32& temprature) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setLogLevel(SWOLogLevel level) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setLogModuleEnabled(GF_UINT32 modules) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE printKernelMsg(KernelMsgType type) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE vibrateControl(VibrateControlType type) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE ledControl(LedControlType type) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE packageIdControl(PackageControlType type) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getAccelerateCap(GF_UINT32& maxSampleRateHz,
			GF_UINT32& maxScaleRage_g, GF_UINT32& maxPackageDataLength) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setAccelerateConfig(GF_UINT32 sampleRateHz,
			GF_UINT32 fullScaleRage_g, GF_UINT32 packageDataLength) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getGyroscopeCap(GF_UINT32& maxSampleRateHz,
			GF_UINT32& maxScaleRage_dps, GF_UINT32& maxPackageDataLength) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getGyroscopeConfig(GF_UINT32 sampleRateHz,
			GF_UINT32 fullScaleRage_dps, GF_UINT32 packageDataLength) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getMagnetometerCap(GF_UINT32& maxSampleRateHz,
			GF_UINT32& maxScaleRage_uT, GF_UINT32& maxPackageDataLength) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getMagnetometerConfig(GF_UINT32 sampleRateHz,
			GF_UINT32 fullScaleRage_uT, GF_UINT32 packageDataLength) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getEulerangleCap(GF_UINT32& maxSampleRateHz) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setEulerangleConfig(GF_UINT32 sampleRateHz) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getQuaternionCap(GF_UINT32& maxSampleRateHz) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setQuaternionConfig(GF_UINT32 sampleRateHz) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getRotationMatrixCap(GF_UINT32& maxSampleRateHz) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setRotationMatrixConfig(GF_UINT32 sampleRateHz) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getGestureCap(GF_UINT32& number, Gesture* supportedGesture, GF_UINT32 dataSize) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setGestureConfig(GF_UINT32 number, Gesture interestingGesture[]) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getEMGRawDataCap(GF_UINT32& maxSampleRateHz,
			EMGRowDataChannels& supportedChannels, GF_UINT32& maxPackageDataLength) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setEMGRawDataConfig(GF_UINT32 sampleRateHz,
			EMGRowDataChannels interestingChannels, GF_UINT32 packageDataLength) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getMouseDataCap(GF_UINT32& maxSampleRateHz) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setMouseDataConfig(GF_UINT32 sampleRateHz) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getJoystickDataCap(GF_UINT32& maxSampleRateHz) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setJoystickDataConfig(GF_UINT32 sampleRateHz) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE getDeviceStatusCap(DeviceStatusFlags& flags) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }
		virtual GF_RET_CODE setDeviceStatusConfig(DeviceStatusFlags flags) override { return GF_RET_CODE::GF_ERROR_NOT_SUPPORT; }

	protected:
		gfwPtr<BLEDevice> mDevice;

	protected:
		//////////////////////////////////////////////////////////////
		// Message pool to process command and it's response
		mutex mMutex;
		SimpleTimer mTimer;
		map<GF_UINT8, SimpleTimer::TimePoint> mExecutingList;
		GF_RET_CODE sendCommand(GF_UINT8 dataLen, GF_PUINT8 commandData, bool hasResponse = true);

		void updateTimer();
		void onTimer();
		virtual void dispatchResponse(GF_UINT8 command, GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, bool timeout = false) = 0;
	};

}
