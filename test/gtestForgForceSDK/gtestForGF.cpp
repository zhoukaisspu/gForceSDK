// gtestForGF.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#define  TAG "gTestForGF"

#include "HubManager.h"
#include "Device.h"
#include "HubListener.h"
#include "DeviceSetting.h"
#include "LogUtils.h"
#include "Utils.h"

#include "gtest/gtest.h"

#include <windows.h>
#include <tchar.h>
#include <list>
#include <chrono>
#include <atomic>
#include <functional>

using namespace std;
using namespace gf;
using namespace std::chrono;

gfsPtr<Hub> pHub = nullptr;  // shart ptr of hub
gfsPtr<HubListener> listener;
list<gfsPtr<Device>> listDev;



atomic<char> c_scanFinished = 0x00;
atomic<char> c_DeviceConnected = 0x00;

#define DEFAULT_RSSI 0xA0

#define GTEST_GETCOMMAND(fun,callback,response) \
	do{ \
		for (auto& itor : listDev) { \
			if (DeviceConnectionStatus::Connected != itor->getConnectionStatus()){ \
				continue;\
						} \
			auto ds = itor->getDeviceSetting(); \
			EXPECT_FALSE(ds == nullptr) << "get null device setting"; \
			if (nullptr == ds){ \
				continue; \
						} \
			response = ResponseResult::RREST_TIMEOUT;\
			ds->fun(callback); \
			std::chrono::duration<int> one_second(1); \
			system_clock::time_point now_point = system_clock::now(); \
			system_clock::time_point end_point = now_point + one_second; \
			while ((system_clock::now() <= end_point) && (response == ResponseResult::RREST_TIMEOUT)); \
			EXPECT_TRUE(response == ResponseResult::RREST_SUCCESS); \
		 } \
  } while (false)

bool enumDevice(SPDEVICE device)
{
	ASSERT_VALID_PTR(device);
	if (nullptr == device)
	{
		cout << "empty device.\n";
		//GF_LOGW("%s: empty device???", __FUNCTION__);
	}
	else
	{
		/*GF_LOGI("Dev: addrtype: %u, address: %s, name: %s, connstatus: %u, alias:%s",
			device->getAddrType(), utils::tostring(device->getAddress()).c_str(), utils::tostring(device->getName()).c_str(),
			static_cast<GF_UINT>(device->getConnectionStatus()), utils::tostring(device->getAlias()).c_str());*/
		listDev.push_back(device);
	}

	// don't want to break enumerate, so always return true
	return true;
}
class HubListenerImp : public HubListener
{
	virtual void onScanFinished() override
	{
		c_scanFinished = 0x01;
	}
	virtual void onStateChanged(HubState state) override
	{
		//TODO
	}
	virtual void onDeviceFound(SPDEVICE device) override
	{
		//TODO
	}
	virtual void onDeviceDiscard(SPDEVICE device) override
	{
		//TODO
	}
	virtual void onDeviceConnected(SPDEVICE device) override
	{
	}
	virtual void onDeviceDisconnected(SPDEVICE device, GF_UINT8 reason) override
	{
		//TODO
	}
	virtual void onOrientationData(SPDEVICE device, const Quaternion& rotation) override
	{
		//TODO
	}
	virtual void onGestureData(SPDEVICE device, Gesture gest) override
	{
		//TODO
	}
	virtual void onDeviceStatusChanged(SPDEVICE device, DeviceStatus status) override
	{
		//TODO
	}
	virtual void onExtendedDeviceData(SPDEVICE device, DeviceDataType dataType, gfsPtr<const vector<GF_UINT8>> data) override
	{
		//TODO
	}

private:
	chrono::system_clock::time_point mLastPrinted = chrono::system_clock::now();
};



TEST(GetHubInstance, getInstance){
	ASSERT_FALSE(pHub == nullptr) <<"Can not get valid instance";
}

TEST(RegistListener, hublistener){
	if (pHub != nullptr) {
		ASSERT_TRUE(GF_RET_CODE::GF_ERROR_BAD_PARAM == pHub->registerListener(listener));
		listener = make_shared<HubListenerImp>();
		GF_RET_CODE ret = pHub->registerListener(listener);
		ASSERT_TRUE(GF_RET_CODE::GF_SUCCESS == ret);
		ret = pHub->registerListener(listener);
		ASSERT_TRUE(GF_RET_CODE::GF_SUCCESS == ret);
	}
}

TEST(HubInit, init){
	if (pHub != nullptr){
		GF_RET_CODE ret = pHub->init();
		ASSERT_TRUE(GF_RET_CODE::GF_SUCCESS == ret);
	}
}

TEST(gForceHub, ScanAndConnect){
	listDev.clear();
	GF_RET_CODE ret = GF_RET_CODE::GF_ERROR;
	while (true){
		ret = GF_RET_CODE::GF_ERROR;
		while (ret != GF_RET_CODE::GF_SUCCESS){
			ret = pHub->startScan(DEFAULT_RSSI);
			EXPECT_TRUE(GF_RET_CODE::GF_SUCCESS == ret);
		}//need 12s to wait for scanfinished
		using std::chrono::system_clock;
		std::chrono::duration<int> tweleve_second(12);
		system_clock::time_point now_point = system_clock::now();
		system_clock::time_point end_point = now_point + tweleve_second;
		while ((system_clock::now() <= end_point) && (c_scanFinished == 0x00));  // wait for 12 seconds
		if (c_scanFinished){
			cout << "ScanFinished" << endl;
			std::function<bool(SPDEVICE)> enumFn(enumDevice);
			pHub->enumDevices(enumFn, false);
			if (listDev.size() == 0){
				c_scanFinished = 0x00;
				continue;
			}
			for (auto& itor : listDev){
				if (DeviceConnectionStatus::Disconnected == itor->getConnectionStatus()){
					ret = itor->connect();
					ASSERT_TRUE(GF_RET_CODE::GF_SUCCESS == ret);
					break;
				}
			}
			now_point = system_clock::now();
			std::chrono::duration<int> five_second(5);
			end_point = now_point + five_second;
			while ((system_clock::now() <= end_point) && (c_DeviceConnected == 0x00));
			if (c_DeviceConnected){
				break; // quit from while(true)
			}
			else {
				ASSERT_TRUE(false) << "Connect failed";
			}

		}
		else {
			ASSERT_TRUE(false) << "scan time out";
		}
	}
}


TEST(DeviceSettingForgForce, getProtocolVer){

	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, tstring str){
		response = res;
	};
	GTEST_GETCOMMAND(getProtocolVer, cb, response);
}

TEST(DeviceSettingForgForce, getFeatureMap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT32 featuremap){
		response = res;
	};
	GTEST_GETCOMMAND(getFeatureMap, cb, response);
}

TEST(DeviceSettingForgForce, getDeviceName){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, tstring str){
		response = res;
	};
	GTEST_GETCOMMAND(getDeviceName, cb, response);
}

TEST(DeviceSettingForgForce, getModelNumber){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, tstring str){
		response = res;
	};
	GTEST_GETCOMMAND(getModelNumber, cb, response);
}

TEST(DeviceSettingForgForce, getSerialNumber){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, tstring str){
		response = res;
	};
	GTEST_GETCOMMAND(getSerialNumber, cb, response);
}

TEST(DeviceSettingForgForce, getHWRevision){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, tstring str){
		response = res;
	};
	GTEST_GETCOMMAND(getHWRevision, cb, response);
}

TEST(DeviceSettingForgForce, getFWRevision){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, tstring str){
		response = res;
	};
	GTEST_GETCOMMAND(getFWRevision, cb, response);
}

TEST(DeviceSettingForgForce, getManufacturerName){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, tstring str){
		response = res;
	};
	GTEST_GETCOMMAND(getManufacturerName, cb, response);
}

TEST(DeviceSettingForgForce, getBootloaderVer){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, tstring str){
		response = res;
	};
	GTEST_GETCOMMAND(getBootloaderVer, cb, response);
}

TEST(DeviceSettingForgForce, getBatteryLevel){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT32 batlevel){
		response = res;
	};
	GTEST_GETCOMMAND(getBatteryLevel, cb, response);
}

TEST(DeviceSettingForgForce, getTemperature){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT32 temperature){
		response = res;
	};
	GTEST_GETCOMMAND(getTemperature, cb, response);

}

TEST(DeviceSettingForgForce, getAccelerateCap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT16 maxSampleRateHz,
		GF_UINT8 maxScaleRange_g, GF_UINT8 maxPackageDataLength) {
		response = res;
	};
	GTEST_GETCOMMAND(getAccelerateCap, cb, response);

}

TEST(DeviceSettingForgForce, getGyroscopeCap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT16 maxSampleRateHz,
		GF_UINT16 maxScaleRange_dps, GF_UINT8 maxPackageDataLength) {
		response = res;
	};
	GTEST_GETCOMMAND(getGyroscopeCap, cb, response);
}

TEST(DeviceSettingForgForce, getMagnetometerCap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT16 maxSampleRateHz,
		GF_UINT16 maxScaleRange_uT, GF_UINT8 maxPackageDataLength) {
		response = res;
	};
	GTEST_GETCOMMAND(getMagnetometerCap, cb, response);
}

TEST(DeviceSettingForgForce, getEulerangleCap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT16 maxSampleRateHz) {
		response = res;
	};
	GTEST_GETCOMMAND(getEulerangleCap, cb, response);
}

TEST(DeviceSettingForgForce, getQuaternionCap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT16 maxSampleRateHz) {
		response = res;
	};
	GTEST_GETCOMMAND(getQuaternionCap, cb, response);
}

TEST(DeviceSettingForgForce, getRotationMatrixCap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT16 maxSampleRateHz) {
		response = res;
	};
	GTEST_GETCOMMAND(getRotationMatrixCap, cb, response);
}

TEST(DeviceSettingForgForce, getGestureCap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_SIZE number, const Gesture supportedGestures[]) {
		response = res;
	};
	GTEST_GETCOMMAND(getGestureCap, cb, response);
}

TEST(DeviceSettingForgForce, getEMGRawDataCap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT16 maxSampleRateHz,
		DeviceSetting::EMGRowDataChannels supportedChannels, GF_UINT8 maxPackageDataLength) {
		response = res;
	};
	GTEST_GETCOMMAND(getEMGRawDataCap, cb, response);
}

TEST(DeviceSettingForgForce, getMouseDataCap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT16 maxSampleRateHz) {
		response = res;
	};
	GTEST_GETCOMMAND(getMouseDataCap, cb, response);
}

TEST(DeviceSettingForgForce, getJoystickDataCap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, GF_UINT16 maxSampleRateHz) {
		response = res;
	};
	GTEST_GETCOMMAND(getJoystickDataCap, cb, response);
}

TEST(DeviceSettingForgForce, getDeviceStatusCap){
	ResponseResult response = ResponseResult::RREST_TIMEOUT;
	auto cb = [&response](ResponseResult res, DeviceSetting::DeviceStatusFlags flags) {
		response = res;
	};
	GTEST_GETCOMMAND(getDeviceStatusCap, cb, response);
}

//TEST(DeviceSettingForgForce, sendTrainingModelData){
//	ResponseResult response = ResponseResult::RREST_TIMEOUT;
//	auto cb = [&response](ResponseResult res, DeviceSetting::DeviceStatusFlags flags) {
//		response = res;
//	};
//	GTEST_GETCOMMAND(getDeviceStatusCap, cb, response);
//}

//TEST(DeviceSettingForgForce, getDeviceStatusCap){
//	ResponseResult response = ResponseResult::RREST_TIMEOUT;
//	auto cb = [&response](ResponseResult res, DeviceSetting::DeviceStatusFlags flags) {
//		response = res;
//	};
//	GTEST_GETCOMMAND(getDeviceStatusCap, cb, response);
//}

TEST(HubUnRegisterListener, listener){
	if (pHub != nullptr){
		gfsPtr<HubListener> tempListener;
		GF_RET_CODE ret = pHub->unRegisterListener(tempListener);
		ASSERT_TRUE(GF_RET_CODE::GF_ERROR_BAD_PARAM == ret);
		ret = pHub->unRegisterListener(listener);
		ASSERT_TRUE(GF_RET_CODE::GF_SUCCESS == ret);
		ret = pHub->unRegisterListener(listener);
		ASSERT_TRUE(GF_RET_CODE::GF_SUCCESS == ret);
	}
}


TEST(HubDeint, deint){
	if (pHub != nullptr){
		GF_RET_CODE ret = GF_RET_CODE::GF_ERROR;
		ret = pHub->deinit();
		ASSERT_TRUE(GF_RET_CODE::GF_SUCCESS == ret);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	pHub = HubManager::getHubInstance(_T("TestApp"));
	pHub->setWorkMode(WorkMode::Freerun);
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("PAUSE");
	return 0;
}

