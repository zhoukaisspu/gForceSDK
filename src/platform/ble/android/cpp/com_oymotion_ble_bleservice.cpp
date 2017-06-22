//
// Created by yy220 on 2017/4/24.
//
#include <string>
#include <mutex>
#include <android/log.h>
#include "jni.h"
#include "AdapterManagerInterface.h"
#include "ClientCallbackInterface.h"
#include "GFBLETypes.h"
#include "AdapterManager.h"

#define TAG "com_oymotion_ble_bleservice"
#define LOGE(...) __android_log_print( ANDROID_LOG_ERROR, TAG, __VA_ARGS__ )
#define LOGW(...) __android_log_print( ANDROID_LOG_WARN, TAG, __VA_ARGS__ )
#define LOGI(...) __android_log_print( ANDROID_LOG_INFO,  TAG, __VA_ARGS__ )
#define LOGD(...) __android_log_print( ANDROID_LOG_DEBUG, TAG, __VA_ARGS__ )

using namespace std;
static GF_CAdapterManager* SingleInstance = NULL;
std::mutex mtx;           // locks access to amInterface

static GF_CClientCallback* globalClientCallback;
static jmethodID method_StartScan;
static jmethodID method_StopScan;
static jmethodID method_Connect;
static jmethodID method_CancelConnect;
static jmethodID method_Disconnect;
static jmethodID method_ConfigMTUSize;
static jmethodID method_UpdateParameter;
static jmethodID method_GetConnectedDeviceNumber;
static jmethodID method_GetHubState;
static jmethodID method_GetHandleByIndex;
static jmethodID method_GetAddressByIndex;

JavaVM* globalJavaVM;


bool attachEnv(JNIEnv **env, bool* attach) {
    if (globalJavaVM == NULL) {
        LOGE("%s: globalJavaVM is NULL", __FUNCTION__);
        return false;
    }

    if((globalJavaVM != NULL) && (JNI_OK != globalJavaVM->GetEnv(reinterpret_cast<void**>(env),JNI_VERSION_1_6)))
    {
        JavaVMAttachArgs args = { JNI_VERSION_1_6, __FUNCTION__, __null };
        if(JNI_OK != globalJavaVM->AttachCurrentThread(env,&args))
        {
            LOGE("%s: AttachCurrentThread failed", __FUNCTION__);
            return false;
        }
        *attach = true;
    } else {
        LOGE("%s: GetEnv OK!", __FUNCTION__);
        *attach = false;
    }

    return true;
}

GF_CAdapterManager::GF_CAdapterManager()
{
    GlobalClass = NULL;
    GlobalObject = NULL;
	mIsScanning = GF_FALSE;
}

GF_CAdapterManager::~GF_CAdapterManager()
{
	mIsScanning = GF_FALSE;
}

GF_CAdapterManagerInterface* GF_CAdapterManagerInterface::GetInstance()
{
    LOGD("%s:", __FUNCTION__);
	if (SingleInstance == NULL)
	{
		mtx.lock();
		if (SingleInstance == NULL)
		{
			SingleInstance = new GF_CAdapterManager;
		}
		mtx.unlock();
	}

	return SingleInstance;
}

GF_STATUS GF_CAdapterManager::Init(GF_UINT8 com_num, GF_UINT8 log_type)
{
	GF_STATUS result = GF_OK;
    LOGD("%s:", __FUNCTION__);
    JNIEnv *env = NULL;
    bool bAttached = false;

    if (false == attachEnv(&env, &bAttached)) {
        return GF_FAIL;
    }

    jclass cls = env->FindClass("com/oymotion/ble/BLEService");
    if (cls != NULL) {
        LOGD("%s: global class is available!", __FUNCTION__);
    }
    else{
        LOGD("%s: global class is not available!", __FUNCTION__);
		if(bAttached && globalJavaVM != NULL) {
			globalJavaVM->DetachCurrentThread();
    	}
        return GF_FAIL;
    }

    GlobalClass = (jclass) env->NewGlobalRef(cls);
    jmethodID GetInstance = env->GetStaticMethodID(cls,"GetInstance","()Lcom/oymotion/ble/BLEService;");
    jobject obj = env->CallStaticObjectMethod(cls, GetInstance);
    GlobalObject = env->NewGlobalRef(obj);

    method_StartScan = env->GetMethodID(cls,"startScan","()Z");
    method_StopScan = env->GetMethodID(cls,"stopScan","()Z");
    method_Connect = env->GetMethodID(cls,"connectDevice","([BZ)Z");
    method_CancelConnect = env->GetMethodID(cls,"cancelConnect","([B)Z");
    method_Disconnect = env->GetMethodID(cls,"disconnect","(I)Z");
    method_ConfigMTUSize = env->GetMethodID(cls,"configMTUSize","(II)Z");
	method_GetConnectedDeviceNumber = env->GetMethodID(cls,"getConnectedDeviceNumber","()B");
	method_GetHubState = env->GetMethodID(cls,"getHubState","()B");
	method_GetHandleByIndex = env->GetMethodID(cls,"getHandleByIndex","(B)I");
	method_GetAddressByIndex = env->GetMethodID(cls,"getAddressByIndex","(B)Ljava/lang/String;");

    if(bAttached && globalJavaVM != NULL) {
		globalJavaVM->DetachCurrentThread();
    }
	return result;
}

GF_STATUS GF_CAdapterManager::Deinit()
{
	GF_STATUS result = GF_OK;
    LOGD("%s:", __FUNCTION__);
    JNIEnv *env = NULL;
    bool bAttached = false;

    if (false == attachEnv(&env, &bAttached)) {
        return GF_FAIL;
    }

    if (GlobalClass != NULL) {
	    env->DeleteGlobalRef(GlobalClass);
    }

    if (GlobalObject != NULL) {
        env->DeleteGlobalRef(GlobalObject);
    }

    if(bAttached && globalJavaVM != NULL) {
		globalJavaVM->DetachCurrentThread();
    }
	return result;
}


GF_STATUS GF_CAdapterManager::RegisterClientCallback(GF_CClientCallback* callback)
{
    LOGD("%s:", __FUNCTION__);
	globalClientCallback = callback;
	return GF_OK;
}

GF_STATUS GF_CAdapterManager::UnregisterClientCallback()
{
    LOGD("%s:", __FUNCTION__);
	globalClientCallback = NULL;
	return GF_OK;
}

/*RSSI_Threshold is used to filter device by RSSI
* if RSSI_Threshold = 0, all device will be sent to client,
* if RSSI_Threshold = x, only device with rssi > x will be sent to client.
*/
GF_STATUS GF_CAdapterManager::StartScan(GF_UINT8 RSSI_Threshold)
{
    GF_STATUS result = GF_OK;
    JNIEnv *env = NULL;
    bool bAttached = false;
    if (false == attachEnv(&env, &bAttached)) {
        return GF_FAIL;
    }

    if (env != NULL) {
        if (!env->CallBooleanMethod(GlobalObject, method_StartScan)) {
			result = GF_FAIL;
		} else {
			result = GF_OK;
			mIsScanning = GF_TRUE;
		}
    }

	if(bAttached && globalJavaVM != NULL) {
    	globalJavaVM->DetachCurrentThread();
    }

    return result;
}

GF_STATUS GF_CAdapterManager::StopScan()
{
	JNIEnv *env = NULL;
	GF_STATUS result = GF_OK;
    bool bAttached = false;
    if (false == attachEnv(&env, &bAttached)) {
        return GF_FAIL;
    }

    /*cannot start to scaning while scanning or connecting is on the way.*/
    if (mIsScanning != GF_TRUE)
    {
    	return GF_ERR_CODE_SCAN_BUSY;
    }

    if (env != NULL) {
        if (!env->CallBooleanMethod(GlobalObject, method_StopScan)) {
        	mIsScanning = GF_FALSE;
			result = GF_OK;
		} else {
			result = GF_FAIL;
		}
    }

    if(bAttached && globalJavaVM != NULL) {
        globalJavaVM->DetachCurrentThread();
    }

    return result;
}

GF_STATUS GF_CAdapterManager::Connect(GF_PUINT8 addr, GF_UINT8 addr_type, GF_BOOL is_direct_conn)
{
	JNIEnv *env = NULL;
	GF_STATUS result = GF_OK;
	jbyteArray address = NULL;
    bool bAttached = false;
    if (false == attachEnv(&env, &bAttached)) {
        return GF_FAIL;
    }

    if (env != NULL) {
        address = env->NewByteArray(BT_ADDRESS_SIZE);
        if (address) {
            env->SetByteArrayRegion(address, 0, BT_ADDRESS_SIZE, (jbyte *)addr);
            if (env->CallBooleanMethod(GlobalObject, method_Connect, address, false)) {
				result = GF_OK;
			} else {
				result = GF_FAIL;
			}
            env->DeleteLocalRef(address);
        }
    }

    if(bAttached && globalJavaVM != NULL) {
        globalJavaVM->DetachCurrentThread();
    }

    return result;
}

GF_STATUS GF_CAdapterManager::CancelConnect(GF_PUINT8 addr, GF_UINT8 addr_type)
{
	JNIEnv *env = NULL;
	GF_STATUS result = GF_OK;
	jbyteArray address = NULL;
    bool bAttached = false;
    if (false == attachEnv(&env, &bAttached)) {
        return GF_FAIL;
    }

    if (env != NULL) {
        address = env->NewByteArray(BT_ADDRESS_SIZE);
        if (address) {
            env->SetByteArrayRegion(address, 0, BT_ADDRESS_SIZE, (jbyte *)addr);
			if (env->CallBooleanMethod(GlobalObject, method_CancelConnect, address)) {
				result = GF_OK;
			} else {
				result = GF_FAIL;
			}
            env->DeleteLocalRef(address);
        }

    }

    if(bAttached && globalJavaVM != NULL) {
        globalJavaVM->DetachCurrentThread();
    }

    return GF_OK;
}

GF_STATUS GF_CAdapterManager::Disconnect(GF_UINT16 handle)
{
	JNIEnv *env = NULL;
	jbyteArray address = NULL;
	GF_STATUS result = GF_OK;
    bool bAttached = false;
    if (false == attachEnv(&env, &bAttached)) {
        return GF_FAIL;
    }

    if (env != NULL) {
		if (env->CallBooleanMethod(GlobalObject, method_Disconnect, (handle & 0x0000FFFF))) {
			result = GF_OK;
		} else {
			result = GF_FAIL;
		}
    }

    if(bAttached && globalJavaVM != NULL) {
        globalJavaVM->DetachCurrentThread();
    }

    return GF_OK;
}

GF_STATUS GF_CAdapterManager::ConfigMtuSize(GF_UINT16 conn_handle, GF_UINT16 MTU_Size)
{
	JNIEnv *env = NULL;
	GF_STATUS result = GF_OK;
    bool bAttached = false;
    if (false == attachEnv(&env, &bAttached)) {
        return GF_FAIL;
    }

    if (env != NULL) {
        if (env->CallBooleanMethod(GlobalObject, method_ConfigMTUSize, (conn_handle & 0x0000FFFF), (MTU_Size & 0x0000FFFF))) {
			result = GF_OK;
		} else {
			result = GF_FAIL;
		}
    }

    if(bAttached && globalJavaVM != NULL) {
        globalJavaVM->DetachCurrentThread();
    }

    return GF_OK;
}

GF_STATUS GF_CAdapterManager::ConnectionParameterUpdate(GF_UINT16 conn_handle, GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max, GF_UINT16 slave_latence, GF_UINT16 supervision_timeout)
{
    /*not supported on Android platform, can not specify parameter.*/
    return GF_FAIL;
}

GF_STATUS GF_CAdapterManager::WriteCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data)
{
    /*not supported on Android platform for now*/
    return GF_FAIL;
}

GF_STATUS GF_CAdapterManager::ReadCharacteristic(GF_UINT16 conn_handle, GF_UINT16 attribute_handle)
{
    /*not supported on Android platform for now*/
    return GF_FAIL;
}

GF_HubState GF_CAdapterManager::GetHubState()
{
    JNIEnv *env = NULL;
    bool bAttached = false;
	jbyte result = 0;
    if (false == attachEnv(&env, &bAttached)) {
        return (GF_HubState)result;
    }

    if (env != NULL) {
        result = env->CallByteMethod(GlobalObject, method_GetHubState);
    }

    if(bAttached && globalJavaVM != NULL) {
        globalJavaVM->DetachCurrentThread();
    }

    return (GF_HubState)result;
}

GF_UINT8 GF_CAdapterManager::GetConnectedDeviceNum()
{
    JNIEnv *env = NULL;
    bool bAttached = false;
	jbyte result = 0;
    if (false == attachEnv(&env, &bAttached)) {
        return result;
    }

    if (env != NULL) {
        result = env->CallByteMethod(GlobalObject, method_GetConnectedDeviceNumber);
    }

    if(bAttached && globalJavaVM != NULL) {
        globalJavaVM->DetachCurrentThread();
    }

    return result;
}

/*connected_device is output result*/
GF_STATUS GF_CAdapterManager::GetConnectedDeviceByIndex(GF_UINT8 index, GF_ConnectedDevice* connected_device)
{
	JNIEnv *env = NULL;
	bool bAttached = false;
	jint handle = 0;
	jstring address;
	memset(connected_device, 0, sizeof(*connected_device));
	if (false == attachEnv(&env, &bAttached)) {
		return GF_FAIL;
	}

	if (env != NULL) {
		if (method_GetHandleByIndex != NULL) {
			handle = env->CallBooleanMethod(GlobalObject, method_GetHandleByIndex, index);
			connected_device->handle = (handle & 0x0000FFFF);
		}
		
		if (method_GetAddressByIndex != NULL) {
			address = (jstring)env->CallObjectMethod(GlobalObject, method_GetAddressByIndex, index);
			char* addr = (char*) env->GetStringUTFChars(address, NULL);
			memcpy(connected_device->address, addr, BT_ADDRESS_SIZE);
		} 
	}

	if(bAttached && globalJavaVM != NULL) {
		globalJavaVM->DetachCurrentThread();
	}

    return GF_OK;
}

static void onDeviceFoundNative(JNIEnv* env, jobject object, jbyteArray val) {
    LOGD("%s:", __FUNCTION__);
    jbyte* array = env->GetByteArrayElements(val, 0);
    int val_len = env->GetArrayLength(val);
	GF_BLEDevice device((GF_UINT8*)array, 0);
	int name_length = min((val_len - BT_ADDRESS_SIZE - 1), BLE_DEVICE_NAME_LENGTH);
	memset(device.dev_name, '\0', BLE_DEVICE_NAME_LENGTH);
	memcpy(device.dev_name, array + BT_ADDRESS_SIZE, name_length);
	device.rssi = array[val_len-1];

	if (globalClientCallback != NULL) {
		globalClientCallback->onScanResult(&device);
	}
    env->ReleaseByteArrayElements(val, array, JNI_ABORT);
}

static void onScanFinishedNative(JNIEnv* env, jobject object) {
    LOGD("%s:", __FUNCTION__);
	if (globalClientCallback != NULL) {
		globalClientCallback->onScanFinished();
	}
}

static void onDeviceConnectedNative(JNIEnv* env, jobject object, jbyteArray val, jint handle) {
    LOGD("%s:", __FUNCTION__);
    jbyte* array = env->GetByteArrayElements(val, 0);
    int val_len = env->GetArrayLength(val);
    LOGD("%s:len = %d", __FUNCTION__, val_len);
    LOGD("%s:address = %02x:%02x:%02x:%02x:%02x:%02x", __FUNCTION__, array[0], array[1], array[2], array[3], array[4], array[5]);
	GF_ConnectedDevice device;
	device.address_type = 0;
	memcpy(device.address, array, BT_ADDRESS_SIZE);
	device.handle = (handle & 0xFFFF);
	device.conn_int = 0;
	device.superTO = 0;
	device.slavelatency = 0;
	device.MTUsize = 0;

	if (globalClientCallback != NULL) {
		globalClientCallback->onDeviceConnected(0, &device);
	}

    env->ReleaseByteArrayElements(val, array, JNI_ABORT);
}

static void onDeviceDisconnectedNative(JNIEnv* env, jobject object, jbyteArray val, jint handle, jbyte reason) {
    LOGD("%s:", __FUNCTION__);
    jbyte* array = env->GetByteArrayElements(val, 0);
    int val_len = env->GetArrayLength(val);
    LOGD("%s:len = %d", __FUNCTION__, val_len);
    LOGD("%s:address = %02x:%02x:%02x:%02x:%02x:%02x", __FUNCTION__, array[0], array[1], array[2], array[3], array[4], array[5]);
	GF_ConnectedDevice device;
	device.address_type = 0;
	memcpy(device.address, array, BT_ADDRESS_SIZE);
	device.handle = (handle & 0xFFFF);
	device.conn_int = 0;
	device.superTO = 0;
	device.slavelatency = 0;
	device.MTUsize = 0;

	if (globalClientCallback != NULL) {
		globalClientCallback->onDeviceDisconnected(0, &device, reason);
	}

    env->ReleaseByteArrayElements(val, array, JNI_ABORT);
}

static void onMTUSizeChangedNative(JNIEnv* env, jobject object, jbyte status, jint handle, jint mtu_size) {
    LOGD("%s:", __FUNCTION__);
	
	if (globalClientCallback != NULL) {
		globalClientCallback->onMTUSizeChanged((GF_STATUS)status,(GF_UINT16)(handle & 0xFFFF), (GF_UINT16)(mtu_size & 0xFFFF));
	}
}

static void onNotificationReceivedNative(JNIEnv* env, jobject object, jbyteArray val, jint handle) {
    LOGD("%s:", __FUNCTION__);
    jbyte* array = env->GetByteArrayElements(val, 0);
    int val_len = env->GetArrayLength(val);
	
	if (globalClientCallback != NULL) {
		globalClientCallback->onNotificationReceived((handle & 0xFFFF), (val_len & 0xFF), (GF_PUINT8)array);
	}

    env->ReleaseByteArrayElements(val, array, JNI_ABORT);
}


static JNINativeMethod sMethods[] = {
    /* name,            signature,         funcPtr */
    {"onDeviceFoundNative", "([B)V", (void *) onDeviceFoundNative},
    {"onScanFinishedNative", "()V", (void *) onScanFinishedNative},
    {"onDeviceConnectedNative", "([BI)V", (void *) onDeviceConnectedNative},
    {"onDeviceDisconnectedNative", "([BIB)V", (void *) onDeviceDisconnectedNative},
    {"onMTUSizeChangedNative", "(BII)V", (void *) onMTUSizeChangedNative},
    {"onNotificationReceivedNative", "([BI)V", (void *) onNotificationReceivedNative},
};


/*
 * JNI Initialization
 */
jint JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    JNIEnv *env;
    globalJavaVM = jvm;
	jclass cls;
	LOGD("%s:", __FUNCTION__);
	
	// Check JNI version
    if (jvm->GetEnv((void **)&env, JNI_VERSION_1_6)) {
        LOGE("JNI version mismatch error");
        return JNI_ERR;
    }

    cls = env->FindClass("com/oymotion/ble/BLEService");
    if (cls == NULL)
    {
        return JNI_ERR;
    }

    /*Register native function to java code.*/
    env->RegisterNatives(cls, sMethods, sizeof(sMethods) / sizeof(sMethods[0]));

    return JNI_VERSION_1_6;
}
