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
#include "DeviceSettingHandle.h"

namespace gf
{
	class BLEDevice;

	class DeviceSettingDataProfile4 : public DeviceSettingHandle
	{
	public:
		DeviceSettingDataProfile4(gfwPtr<BLEDevice> device);
		virtual ~DeviceSettingDataProfile4();

		void initialize();

	public:
		/////////////////////////////////////////////////////
		// description
		// note that the below properties are cached for multiple access
		virtual GF_RET_CODE getProtocolVer(tstring& version) override;
		virtual GF_RET_CODE getFeatureMap(GF_UINT32& featureMap) override;
		virtual GF_RET_CODE getDeviceName(tstring& name) override;
		virtual GF_RET_CODE getModelNumber(tstring& modelNumber) override;
		virtual GF_RET_CODE getSerialNumber(tstring& serialNumber) override;
		virtual GF_RET_CODE getHWRevision(tstring& version) override;
		virtual GF_RET_CODE getFWRevision(tstring& version) override;
		virtual GF_RET_CODE getManufacturerName(tstring& name) override;

	public:
		/////////////////////////////////////////////////////
		// control
		virtual GF_RET_CODE powerOff() override;
		virtual GF_RET_CODE swithToOAD() override;
		virtual GF_RET_CODE systemReset() override;
		// C.2
		virtual GF_RET_CODE switchService(DeviceService service) override;

	public:
		/////////////////////////////////////////////////////
		// config
		// C.6
		virtual GF_RET_CODE sendTrainingModelData(GF_UINT32 length, GF_UINT8 data[]) override;
		virtual GF_RET_CODE setDataNotifSwitch(DataNotifFlags flags) override;

		///////////////////////////////////////////////
		virtual GF_RET_CODE getBatteryLevel(GF_UINT32& level) override;
		// C.1
		virtual GF_RET_CODE getTemperature(GF_UINT32& temprature) override;
		// C.3
		virtual GF_RET_CODE setLogLevel(SWOLogLevel level) override;
		// C.3
		virtual GF_RET_CODE setLogModuleEnabled(GF_UINT32 modules) override;
		// C.3
		virtual GF_RET_CODE printKernelMsg(KernelMsgType type) override;
		// C.4
		virtual GF_RET_CODE vibrateControl(VibrateControlType type) override;
		// C.5
		virtual GF_RET_CODE ledControl(LedControlType type) override;
		virtual GF_RET_CODE packageIdControl(PackageControlType type) override;

		// C.7
		virtual GF_RET_CODE getAccelerateCap(GF_UINT32& maxSampleRateHz,
			GF_UINT32& maxScaleRage_g, GF_UINT32& maxPackageDataLength) override;
		// C.7
		virtual GF_RET_CODE setAccelerateConfig(GF_UINT32 sampleRateHz,
			GF_UINT32 fullScaleRage_g, GF_UINT32 packageDataLength) override;
		// C.8
		virtual GF_RET_CODE getGyroscopeCap(GF_UINT32& maxSampleRateHz,
			GF_UINT32& maxScaleRage_dps, GF_UINT32& maxPackageDataLength) override;
		// C.8
		virtual GF_RET_CODE getGyroscopeConfig(GF_UINT32 sampleRateHz,
			GF_UINT32 fullScaleRage_dps, GF_UINT32 packageDataLength) override;
		// C.9
		virtual GF_RET_CODE getMagnetometerCap(GF_UINT32& maxSampleRateHz,
			GF_UINT32& maxScaleRage_uT, GF_UINT32& maxPackageDataLength) override;
		// C.9
		virtual GF_RET_CODE getMagnetometerConfig(GF_UINT32 sampleRateHz,
			GF_UINT32 fullScaleRage_uT, GF_UINT32 packageDataLength) override;
		// C.10
		virtual GF_RET_CODE getEulerangleCap(GF_UINT32& maxSampleRateHz) override;
		// C.10
		virtual GF_RET_CODE setEulerangleConfig(GF_UINT32 sampleRateHz) override;
		// C.11
		virtual GF_RET_CODE getQuaternionCap(GF_UINT32& maxSampleRateHz) override;
		// C.11
		virtual GF_RET_CODE setQuaternionConfig(GF_UINT32 sampleRateHz) override;
		// C.12
		virtual GF_RET_CODE getRotationMatrixCap(GF_UINT32& maxSampleRateHz) override;
		// C.12
		virtual GF_RET_CODE setRotationMatrixConfig(GF_UINT32 sampleRateHz) override;
		// C.13
		virtual GF_RET_CODE getGestureCap(GF_UINT32& number, Gesture* supportedGesture, GF_UINT32 dataSize) override;
		// C.13
		virtual GF_RET_CODE setGestureConfig(GF_UINT32 number, Gesture interestingGesture[]) override;
		// C.14
		virtual GF_RET_CODE getEMGRawDataCap(GF_UINT32& maxSampleRateHz,
			EMGRowDataChannels& supportedChannels, GF_UINT32& maxPackageDataLength) override;
		// C.14
		virtual GF_RET_CODE setEMGRawDataConfig(GF_UINT32 sampleRateHz,
			EMGRowDataChannels interestingChannels, GF_UINT32 packageDataLength) override;
		// C.15
		virtual GF_RET_CODE getMouseDataCap(GF_UINT32& maxSampleRateHz) override;
		// C.15
		virtual GF_RET_CODE setMouseDataConfig(GF_UINT32 sampleRateHz) override;
		// C.16
		virtual GF_RET_CODE getJoystickDataCap(GF_UINT32& maxSampleRateHz) override;
		// C.16
		virtual GF_RET_CODE setJoystickDataConfig(GF_UINT32 sampleRateHz) override;
		// C.17
		virtual GF_RET_CODE getDeviceStatusCap(DeviceStatusFlags& flags) override;
		// C.17
		virtual GF_RET_CODE setDeviceStatusConfig(DeviceStatusFlags flags) override;

	protected:
		virtual void dispatchResponse(GF_UINT8 command, GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, bool timeout = false) override;
		virtual void onTimeout(GF_UINT8 command);
		tstring getStringCommon(GF_UINT8 length, GF_PUINT8 data);

	protected:
		virtual void internalGetProtocolVer();
		virtual void internalGetFeatureMap();
		virtual void internalGetDeviceName();
		virtual void internalGetModelNumber();
		virtual void internalGetSerialNumber();
		virtual void internalGetHWRevision();
		virtual void internalGetFWRevision();
		virtual void internalGetManufacturerName();

	protected:
		virtual void onProtocolVer(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onFeatureMap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onDeviceName(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onModelNumber(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSerialNumber(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onHWRevision(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onFWRevision(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onManufacturerName(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSendTrainingModelData(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetDataNotifSwitch(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetBatteryLevel(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetTemperature(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetLogLevel(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetLogModuleEnabled(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onPrintKernelMsg(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onVibrateControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onLedControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onPackageIdControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetAccelerateCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetAccelerateConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetGyroscopeCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetGyroscopeConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetMagnetometerCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetMagnetometerConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetEulerangleCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetEulerangleConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetQuaternionCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetQuaternionConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetRotationMatrixCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetRotationMatrixConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetGestureCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetGestureConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetEMGRawDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetEMGRawDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetMouseDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetMouseDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetJoystickDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetJoystickDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onGetDeviceStatusCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);
		virtual void onSetDeviceStatusConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data);

	protected:
		bool mDeviceInfoReady = false;
		tstring mProtocolVer;
		GF_UINT32 mFeatureMap = 0;
		tstring mDeviceName;
		tstring mModelNumber;
		tstring mSerialNumber;
		tstring mHWRev;
		tstring mFWRev;
		tstring mManuName;
	};

}
