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

	public:
		/////////////////////////////////////////////////////
		// description
		// note that the below properties are cached for multiple access
		virtual GF_RET_CODE getProtocolVer(function<void(ResponseResult res, tstring version)> cb) override;
		virtual GF_RET_CODE getFeatureMap(function<void(ResponseResult res, GF_UINT32 featureMap)> cb) override;
		virtual GF_RET_CODE getDeviceName(function<void(ResponseResult res, tstring name)> cb) override;
		virtual GF_RET_CODE getModelNumber(function<void(ResponseResult res, tstring modelNumber)> cb) override;
		virtual GF_RET_CODE getSerialNumber(function<void(ResponseResult res, tstring serialNumber)> cb) override;
		virtual GF_RET_CODE getHWRevision(function<void(ResponseResult res, tstring version)> cb) override;
		virtual GF_RET_CODE getFWRevision(function<void(ResponseResult res, tstring version)> cb) override;
		virtual GF_RET_CODE getManufacturerName(function<void(ResponseResult res, tstring name)> cb) override;
		virtual GF_RET_CODE getBootloaderVer(function<void(ResponseResult res, tstring version)> cb) override;

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
		virtual GF_RET_CODE sendTrainingModelData(GF_UINT32 length, GF_UINT8 data[], function<void(ResponseResult res, GF_UINT32 percentage)> progress) override;
		virtual GF_RET_CODE setDataNotifSwitch(DataNotifFlags flags, function<void(ResponseResult res)> cb) override;

		///////////////////////////////////////////////
		virtual GF_RET_CODE getBatteryLevel(function<void(ResponseResult res, GF_UINT32 percentage)> cb) override;
		// C.1
		virtual GF_RET_CODE getTemperature(function<void(ResponseResult res, GF_UINT32 temperature)> cb) override;
		// C.3
		virtual GF_RET_CODE setLogLevel(SWOLogLevel level, function<void(ResponseResult res)> cb) override;
		// C.3
		virtual GF_RET_CODE setLogModuleEnabled(SWOModule modules, function<void(ResponseResult res)> cb) override;
		// C.3
		virtual GF_RET_CODE printKernelMsg(KernelMsgType type, function<void(ResponseResult res)> cb) override;
		// C.4
		virtual GF_RET_CODE vibrateControl(VibrateControlType type, function<void(ResponseResult res)> cb) override;
		// C.5
		virtual GF_RET_CODE ledControlTest(LedControlTestType type, function<void(ResponseResult res)> cb) override;
		virtual GF_RET_CODE packageIdControl(PackageControlType type, function<void(ResponseResult res)> cb) override;

		// C.7
		virtual GF_RET_CODE getAccelerateCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz,
			GF_UINT8 maxScaleRange_g, GF_UINT8 maxPackageDataLength)> cb) override;
		// C.7
		virtual GF_RET_CODE setAccelerateConfig(GF_UINT16 sampleRateHz,
			GF_UINT8 fullScaleRange_g, GF_UINT8 packageDataLength, function<void(ResponseResult res)> cb) override;
		// C.8
		virtual GF_RET_CODE getGyroscopeCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz,
			GF_UINT16 maxScaleRange_dps, GF_UINT8 maxPackageDataLength)> cb) override;
		// C.8
		virtual GF_RET_CODE setGyroscopeConfig(GF_UINT16 sampleRateHz,
			GF_UINT16 fullScaleRange_dps, GF_UINT8 packageDataLength, function<void(ResponseResult res)> cb) override;
		// C.9
		virtual GF_RET_CODE getMagnetometerCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz,
			GF_UINT16 maxScaleRange_uT, GF_UINT8 maxPackageDataLength)> cb) override;
		// C.9
		virtual GF_RET_CODE setMagnetometerConfig(GF_UINT16 sampleRateHz,
			GF_UINT16 fullScaleRange_uT, GF_UINT8 packageDataLength, function<void(ResponseResult res)> cb) override;
		// C.10
		virtual GF_RET_CODE getEulerangleCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz)> cb) override;
		// C.10
		virtual GF_RET_CODE setEulerangleConfig(GF_UINT16 sampleRateHz, function<void(ResponseResult res)> cb) override;
		// C.11
		virtual GF_RET_CODE getQuaternionCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz)> cb) override;
		// C.11
		virtual GF_RET_CODE setQuaternionConfig(GF_UINT16 sampleRateHz, function<void(ResponseResult res)> cb) override;
		// C.12
		virtual GF_RET_CODE getRotationMatrixCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz)> cb) override;
		// C.12
		virtual GF_RET_CODE setRotationMatrixConfig(GF_UINT16 sampleRateHz, function<void(ResponseResult res)> cb) override;
		// C.13
		virtual GF_RET_CODE getGestureCap(function<void(ResponseResult res, GF_SIZE number, const Gesture supportedGestures[])> cb) override;
		// C.13
		virtual GF_RET_CODE setGestureConfig(GF_UINT32 number, Gesture interestingGesture[], function<void(ResponseResult res)> cb) override;
		// C.14
		virtual GF_RET_CODE getEMGRawDataCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz,
			EMGRowDataChannels supportedChannels, GF_UINT8 maxPackageDataLength)> cb) override;
		// C.14
		virtual GF_RET_CODE setEMGRawDataConfig(GF_UINT16 sampleRateHz,
			EMGRowDataChannels interestingChannels, GF_UINT8 packageDataLength, function<void(ResponseResult res)> cb) override;
		// C.15
		virtual GF_RET_CODE getMouseDataCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz)> cb) override;
		// C.15
		virtual GF_RET_CODE setMouseDataConfig(GF_UINT16 sampleRateHz, function<void(ResponseResult res)> cb) override;
		// C.16
		virtual GF_RET_CODE getJoystickDataCap(function<void(ResponseResult res, GF_UINT16 maxSampleRateHz)> cb) override;
		// C.16
		virtual GF_RET_CODE setJoystickDataConfig(GF_UINT16 sampleRateHz, function<void(ResponseResult res)> cb) override;
		// C.17
		virtual GF_RET_CODE getDeviceStatusCap(function<void(ResponseResult res, DeviceStatusFlags flags)> cb) override;
		// C.17
		virtual GF_RET_CODE setDeviceStatusConfig(DeviceStatusFlags flags, function<void(ResponseResult res)> cb) override;

	protected:
		virtual void dispatchResponse(GF_UINT8 command, GF_UINT8 retval, GF_UINT8 length,
			GF_PUINT8 data, gfsPtr<void> cb, bool timeout = false) override;
		static tstring getStringCommon(GF_UINT8 length, GF_PUINT8 data);


	protected:
		void onProtocolVer(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onFeatureMap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onDeviceName(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onModelNumber(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSerialNumber(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onHWRevision(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onFWRevision(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onManufacturerName(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onBootloaderVer(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSendTrainingModelData(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetDataNotifSwitch(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetBatteryLevel(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetTemperature(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetLogLevel(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetLogModuleEnabled(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onPrintKernelMsg(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onVibrateControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onLedControlTest(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onPackageIdControl(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetAccelerateCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetAccelerateConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetGyroscopeCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetGyroscopeConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetMagnetometerCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetMagnetometerConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetEulerangleCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetEulerangleConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetQuaternionCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetQuaternionConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetRotationMatrixCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetRotationMatrixConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetGestureCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetGestureConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetEMGRawDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetEMGRawDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetMouseDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetMouseDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetJoystickDataCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetJoystickDataConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onGetDeviceStatusCap(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);
		void onSetDeviceStatusConfig(GF_UINT8 retval, GF_UINT8 length, GF_PUINT8 data, gfsPtr<void> cb);

	protected:
		// TODO: add thread-safe protection for below members
		tstring mProtocolVer;
		GF_UINT32 mFeatureMap = 0;
		tstring mDeviceName;
		tstring mModelNumber;
		tstring mSerialNumber;
		tstring mHWRev;
		tstring mFWRev;
		tstring mManuName;
		tstring mBootloaderVer;

		struct Config
		{
			Config()
				: pkgIdCtrl(PackageControlType::Disable)
				, dataFlags(DataNotifFlags::DNF_OFF)
				, accelerateCfg(0)
				, gyroscopeCfg(0)
				, magnetometerCfg(0)
				, emgrawCfg(0)
			{}
			atomic<PackageControlType> pkgIdCtrl;
			atomic<DataNotifFlags> dataFlags;
			atomic<GF_UINT32> accelerateCfg;
			atomic<GF_UINT64> gyroscopeCfg;
			atomic<GF_UINT64> magnetometerCfg;
			atomic<GF_UINT64> emgrawCfg;
		} mCfg, mCfgApplying;

	public:
		void resetConfig();

	private:
		void configMtuSize();

	private:
		// sendTrainingModelData
		unique_ptr<GF_UINT8[]> mTraingModelBuffer;
		GF_UINT32 mTraingModelBufferLen = 0;
		GF_UINT32 mLastSentPercentage = (GF_UINT32)-1;
		function<void(ResponseResult res, GF_UINT32 percentage)> mTrainingModelCallback;
		void trainingModelOnResponse(ResponseResult respval, GF_UINT16 nextPkgNo);
		GF_RET_CODE trainingModelSendNextPackage(GF_UINT16 nextPkgId);
		void resetTrainingModelData();
	};

}
