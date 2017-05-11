1. 安装的AndroidStudio版本是2.3

2. 需要修改每个project的local.properties文件，设置sdk和ndk的真实安装路径。
	sdk.dir=C\:\\Users\\yy220\\AppData\\Local\\Android\\Sdk
	ndk.dir=C\:\\Users\\yy220\\AppData\\Local\\Android\\android-ndk-r14b
	
3. \script\android\BLELibrary\这个project编译之后，会生成两个lib。
	\BLELibrary\app\build\intermediates\ndkBuild\debug\obj\local\xxx\libble-lib.so这个文件夹是jni(c++）生成的so文件，
	生成了各个平台下可调用的so, 其对应的source code 在\BLELibrary\app\src\main\cpp目录下。
	
	\script\android\BLELibrary\blelibrary\build\outputs\aar\blelibrary-debug.aar这是java文件的lib.
	其对应的source code在\BLELibrary\blelibrary\src\main\java\com\oym\blelibrary目录下，这些java文件会调用android标准的API。

4. 先编译BLELibrary project，再将生成的so文件放入另一个test project中。
	test project调用so和aar文件，生成一个apk，用来测试使用。
	将blelibrary-debug.aar放入\test\app\libs文件夹中。
	将对应平台的libble-lib.so放入\test\app\src\main\jniLibs\下对应平台的文件夹下。
	\test\app\build.gradle文件用来设置apk最终的运行平台。
		externalNativeBuild {
            ndkBuild {
                cppFlags ""
                cFlags  "-std=c++11"
                abiFilters "x86"
            }
        }
	如果是模拟器或者android x86,选择x86,
	如果是真实手机，选项armeabi平台
	\test\app\src\main\cpp\Android.mk文件要设置引用对应平台的so文件。
	LOCAL_PATH := $(call my-dir)

	include $(CLEAR_VARS)
	LOCAL_MODULE := ble-lib
	LOCAL_SRC_FILES := $(LOCAL_PATH)/../jniLibs/x86/libble-lib.so
	#LOCAL_SRC_FILES := $(LOCAL_PATH)/../jniLibs/armeabi/libble-lib.so
	include $(PREBUILT_SHARED_LIBRARY)

	include $(CLEAR_VARS)
	
5. 如果一切顺利的话，会在\test\app\build\outputs\apk\生成一个apk文件，能够在真实的android机器上运行。

6. 上层sdk需要调用的api在\BLELibrary\app\src\main\inc\AdapterManagerInterface.h
	接口形式与win7 + NPI dongle的接口是一样的。
	以下三个api暂时不支持。
	//因为android java层，没有接口设置具体的conn_interval, slave_latence, super_TO等参数。
	virtual GF_STATUS ConnectionParameterUpdate(GF_UINT16 conn_handle, GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max, GF_UINT16 slave_latence, GF_UINT16 supervision_timeout) = 0;

	//因为android java层，不会暴露attribute_handle出来，所以暂时不支持通过这种接口读写Characteristic。
	virtual GF_STATUS WriteCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data) = 0;
	virtual GF_STATUS ReadCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle) = 0;


	#ifndef __ADAPTERMANAGERINTERFACE_H__
	#define __ADAPTERMANAGERINTERFACE_H__
	#include "GFBLETypes.h"

	class GF_CClientCallback;
	class GF_CAdapterManager;

	class GF_CAdapterManagerInterface
	{
	public:
		static GF_CAdapterManagerInterface* GetInstance();
		virtual GF_STATUS Init(GF_UINT8 com_num, GF_UINT8 log_type) = 0;
		virtual GF_STATUS Deinit() = 0;
		virtual GF_STATUS StartScan(GF_UINT8 RSSI_Threshold) = 0;
		virtual GF_STATUS StopScan() = 0;
		virtual GF_STATUS Connect(GF_PUINT8 addr, GF_UINT8 addr_type, GF_BOOL is_direct_conn) = 0;
		virtual GF_STATUS CancelConnect(GF_PUINT8 addr, GF_UINT8 addr_type) = 0;
		virtual GF_STATUS Disconnect(GF_UINT16 handle) = 0;
		virtual GF_STATUS RegisterClientCallback(GF_CClientCallback* callback) = 0;
		virtual GF_STATUS UnregisterClientCallback() = 0;

		virtual GF_STATUS ConfigMtuSize(GF_UINT16 conn_handle, GF_UINT16 MTU_Size) = 0;
		virtual GF_STATUS ConnectionParameterUpdate(GF_UINT16 conn_handle, GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max, GF_UINT16 slave_latence, GF_UINT16 supervision_timeout) = 0;
		virtual GF_STATUS WriteCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data) = 0;
		virtual GF_STATUS ReadCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle) = 0;

		virtual GF_HubState GetHubState() = 0;
		virtual GF_UINT8 GetConnectedDeviceNum() = 0;
		/*connected_device is output result*/
		virtual GF_STATUS GetConnectedDeviceByIndex(GF_UINT8 index, GF_ConnectedDevice* connected_device) = 0;
	};
	#endif

	callback的接口定义在\BLELibrary\app\src\main\inc\ClientCallbackInterface.h文件中。
		这三个接口暂时不会调用。
		//原因同上，android不支持这种格式的接口，后续会统一起来。
		virtual void onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 handle, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency) = 0;
		virtual void onCharacteristicValueRead(GF_STATUS status, GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data) = 0;
		
		//android上需要这个callback。
		virtual void onComDestory() = 0;

	#ifndef ICLIENTCALLBADK
	#define ICLIENTCALLBADK
	#include "GFBLETypes.h"

	class GF_CClientCallback
	{
	public:
		virtual void onScanResult(GF_BLEDevice* device) = 0;
		virtual void onScanFinished() = 0;
		virtual void onDeviceConnected(GF_STATUS status, GF_ConnectedDevice *device) = 0;
		virtual void onDeviceDisconnected(GF_STATUS status, GF_ConnectedDevice *device, GF_UINT8 reason) = 0;

		virtual void onMTUSizeChanged(GF_STATUS status, GF_UINT16 handle, GF_UINT16 mtu_size) = 0;
		virtual void onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 handle, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency) = 0;
		virtual void onCharacteristicValueRead(GF_STATUS status, GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data) = 0;

		/*Notification format: data length(1 byte N) + data(N Bytes)*/
		virtual void onNotificationReceived(GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data) = 0;

		virtual void onComDestory() = 0;
	};
	#endif
	