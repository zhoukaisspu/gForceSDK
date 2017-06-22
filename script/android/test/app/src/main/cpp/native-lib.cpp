#include <string>
#include <android/log.h>
#include "jni.h"
#include "../inc/AdapterManagerInterface.h"
#include "../inc/ClientCallbackInterface.h"

#define TAG "Native-lib"
#define LOGE(...) __android_log_print( ANDROID_LOG_ERROR, TAG, __VA_ARGS__ )
#define LOGW(...) __android_log_print( ANDROID_LOG_WARN, TAG, __VA_ARGS__ )
#define LOGI(...) __android_log_print( ANDROID_LOG_INFO,  TAG, __VA_ARGS__ )
#define LOGD(...) __android_log_print( ANDROID_LOG_DEBUG, TAG, __VA_ARGS__ )

using namespace std;
static jclass GlobalClass;
static jobject GlobalObject;
JavaVM* globalJavaVM = NULL;

GF_CAdapterManagerInterface* amInterface;

static jmethodID method_onScanResult;
static jmethodID method_onScanFinished;
static jmethodID method_onDeviceConnected;
static jmethodID method_onDeviceDisconnected;
static jmethodID method_onMTUSizeChanged;
static jmethodID method_onNotificationReceived;

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

class Callback : public GF_CClientCallback
{
public:
	void onScanResult(GF_BLEDevice* device) 
	{
	    LOGD("%s:", __FUNCTION__);
		jbyteArray addr = NULL, devname = NULL;
		bool bAttached = false;

		if (method_onScanResult != NULL) {
            if (false == attachEnv(&env, &bAttached)) {
                return ;
            }

            addr = env->NewByteArray(BT_ADDRESS_SIZE);
    		if (addr == NULL) goto Fail;
    		env->SetByteArrayRegion(addr, 0, BT_ADDRESS_SIZE, (jbyte *)device->addr);

            int name_len = strlen(device->dev_name);
    		devname = env->NewByteArray(name_len);
    		if (devname == NULL) goto Fail;
			env->SetByteArrayRegion(devname, 0, name_len, (jbyte*)device->dev_name);

            if (env != NULL && GlobalObject != NULL) {
                env->CallVoidMethod(GlobalObject, method_onScanResult, addr, devname, device->rssi);
                //env->CallVoidMethod(GlobalObject, method_onScanFinished);
            }

			if(bAttached) {
				globalJavaVM->DetachCurrentThread();
		    }

		    env->DeleteLocalRef(addr);
            env->DeleteLocalRef(devname);

		    return;
		}
		
Fail:
	    if (addr) env->DeleteLocalRef(addr);
	    if (devname) env->DeleteLocalRef(devname);
	    if(bAttached) {
        	globalJavaVM->DetachCurrentThread();
        }
	}

	void onScanFinished()
	{
	    LOGD("%s:", __FUNCTION__);
		bool bAttached = false;
		
		if (method_onScanResult != NULL) {
	        if (false == attachEnv(&env, &bAttached)) {
	            return ;
	        }

	        if (env != NULL && GlobalObject != NULL) {
	            env->CallVoidMethod(GlobalObject, method_onScanFinished);
	        }

			if(bAttached) {
				globalJavaVM->DetachCurrentThread();
		    }
		}
	}

	void onDeviceDisconnected(GF_STATUS status, GF_ConnectedDevice *device, GF_UINT8 reason)
	{
	    LOGD("%s:", __FUNCTION__);
		jbyteArray addr = NULL;
		bool bAttached = false;

		if (method_onDeviceDisconnected != NULL) {
            if (false == attachEnv(&env, &bAttached)) {
                return ;
            }

            addr = env->NewByteArray(BT_ADDRESS_SIZE);
    		if (addr == NULL) goto Fail;
    		env->SetByteArrayRegion(addr, 0, BT_ADDRESS_SIZE, (jbyte *)device->address);
            
            if (env != NULL && GlobalObject != NULL) {
                env->CallVoidMethod(GlobalObject, method_onDeviceDisconnected, addr, device->handle, reason);
            }

			if(bAttached) {
				globalJavaVM->DetachCurrentThread();
		    }

		    env->DeleteLocalRef(addr);

		    return;
		}

Fail:
	    if (addr) env->DeleteLocalRef(addr);
	    if(bAttached) {
        	globalJavaVM->DetachCurrentThread();
        }
	}

	void onDeviceConnected(GF_STATUS status, GF_ConnectedDevice *device)
	{
	    LOGD("%s:", __FUNCTION__);
		jbyteArray addr = NULL;
		bool bAttached = false;

		if (method_onDeviceConnected != NULL) {
            if (false == attachEnv(&env, &bAttached)) {
                return ;
            }

            addr = env->NewByteArray(BT_ADDRESS_SIZE);
    		if (addr == NULL) goto Fail;
    		env->SetByteArrayRegion(addr, 0, BT_ADDRESS_SIZE, (jbyte *)device->address);
            
            if (env != NULL && GlobalObject != NULL) {
                env->CallVoidMethod(GlobalObject, method_onDeviceConnected, addr, device->handle);
            }

			if(bAttached) {
				globalJavaVM->DetachCurrentThread();
		    }

		    env->DeleteLocalRef(addr);

		    return;
		}

Fail:
	    if (addr) env->DeleteLocalRef(addr);
	    if(bAttached) {
        	globalJavaVM->DetachCurrentThread();
        }
	}

	void onMTUSizeChanged(GF_STATUS status, GF_UINT16 handle, GF_UINT16 mtu_size)
	{
	    LOGD("%s:", __FUNCTION__);
		bool bAttached = false;

		if (method_onMTUSizeChanged != NULL) {
            if (false == attachEnv(&env, &bAttached)) {
                return ;
            }

            if (env != NULL && GlobalObject != NULL) {
                env->CallVoidMethod(GlobalObject, method_onMTUSizeChanged, handle, mtu_size);
            }

			if(bAttached) {
				globalJavaVM->DetachCurrentThread();
		    }

		    return;
		}
	}

	/*Not support on Android platform, because connection parameter will not be accessable to upper level.*/
	void onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 handle, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency)
	{
		return;

	}

	/*Not support for now, because characteristic read with different format on window and android platform.*/
	void onCharacteristicValueRead(GF_STATUS status, GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data)
	{
		return;
	}

	void onNotificationReceived(GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data)
	{
	    LOGD("%s:", __FUNCTION__);
		jbyteArray notification = NULL;
		bool bAttached = false;

		if (method_onNotificationReceived != NULL) {
            if (false == attachEnv(&env, &bAttached)) {
                return ;
            }

            notification = env->NewByteArray(length);
    		if (notification == NULL) goto Fail;
    		env->SetByteArrayRegion(notification, 0, BT_ADDRESS_SIZE, (jbyte *)data);
            
            if (env != NULL && GlobalObject != NULL) {
                env->CallVoidMethod(GlobalObject, method_onNotificationReceived, handle, notification);
            }

			if(bAttached) {
				globalJavaVM->DetachCurrentThread();
		    }

		    env->DeleteLocalRef(notification);

		    return;
		}

Fail:
	    if (notification) env->DeleteLocalRef(notification);
	    if(bAttached) {
        	globalJavaVM->DetachCurrentThread();
        }
	}

	/*No need to be supported on Android platform.*/
	void onComDestory()
	{
		return;
	}

private:
    JNIEnv *env;
};

Callback* globalCallback;

static void classInitNative(JNIEnv* env, jclass clazz) {
    LOGD("%s:", __FUNCTION__);
	jclass jniCallbackClass = env->FindClass("com/oym/test/MainActivity");
	method_onScanResult = env->GetMethodID(jniCallbackClass, "onScanResult", "([B[BI)V");
	method_onScanFinished = env->GetMethodID(jniCallbackClass, "onScanFinished", "()V");
	method_onDeviceConnected = env->GetMethodID(jniCallbackClass, "onDeviceConnected", "([BI)V");
	method_onDeviceDisconnected = env->GetMethodID(jniCallbackClass, "onDeviceDisconnected", "([BIB)V");
	method_onMTUSizeChanged = env->GetMethodID(jniCallbackClass, "onMTUSizeChanged", "(II)V");
	method_onNotificationReceived = env->GetMethodID(jniCallbackClass, "onNotificationReceived", "(I[B)V");
}

static bool initNative(JNIEnv* env, jobject object) {
     LOGD("%s:", __FUNCTION__);
     amInterface = GF_CAdapterManagerInterface::GetInstance();
	 globalCallback = new Callback;
     if (amInterface != NULL) {
        if (GF_FAIL == amInterface->Init(0, 0)) {
			LOGD("%s:Init interface fail", __FUNCTION__);
			return JNI_FALSE;
		} else {
		    LOGD("%s: RegisterClientCallback ", __FUNCTION__);
			amInterface->RegisterClientCallback(globalCallback);
			GlobalObject = env->NewGlobalRef(object);
		}
     }

     return JNI_TRUE;
}

static bool cleanupNative(JNIEnv* env, jobject obj) {
    LOGD("%s:", __FUNCTION__);
    if (amInterface != NULL) {
        amInterface->UnregisterClientCallback();
        amInterface->Deinit();
    }
    env->DeleteGlobalRef(GlobalObject);
    GlobalObject = NULL;
    return JNI_TRUE;
}

static bool startScanNative(JNIEnv* env, jobject object) {
    LOGD("%s:", __FUNCTION__);
    //env->CallBooleanMethod(GlobalObject, method_StartScan);
    if (amInterface != NULL) {
        amInterface->StartScan(0);
    }
    return JNI_TRUE;
}

static bool stopScanNative(JNIEnv* env, jobject object) {
    LOGD("%s:", __FUNCTION__);
    if (amInterface != NULL) {
        amInterface->StopScan();
    }

    return JNI_TRUE;
}

static bool connectNative(JNIEnv* env, jobject object, jbyteArray addr) {
    LOGD("%s:", __FUNCTION__);
    if (amInterface != NULL) {
        jbyte* addressArray = env->GetByteArrayElements(addr, 0);
        amInterface->Connect((GF_PUINT8)addressArray, 0, 0);
        env->ReleaseByteArrayElements(addr, addressArray, JNI_ABORT);
    } else {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool cancelConnectNative( JNIEnv* env, jobject obj, jbyteArray addr) {
    LOGD("%s:", __FUNCTION__);
    if (amInterface != NULL) {
        jbyte* addressArray = env->GetByteArrayElements(addr, 0);
        amInterface->CancelConnect((GF_PUINT8)addressArray, 0);
        env->ReleaseByteArrayElements(addr, addressArray, JNI_ABORT);
    } else {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool disconnectNative( JNIEnv* env, jobject obj, jint handle) {
    LOGD("%s:", __FUNCTION__);
    if (amInterface != NULL) {
        amInterface->Disconnect((GF_UINT16)(handle & 0xFFFF));
    } else {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static jbyte getHubStateNative( JNIEnv* env, jobject obj) {
    LOGD("%s:", __FUNCTION__);
    if (amInterface != NULL) {
        return amInterface->GetHubState();
    } else {
        return 0;
    }
}

static jbyte getConnectedDevNumNative( JNIEnv* env, jobject obj) {
    LOGD("%s:", __FUNCTION__);
    if (amInterface != NULL) {
        return amInterface->GetConnectedDeviceNum();
    } else {
        return 0;
    }
}

static jstring stringFromJNINative( JNIEnv* env, jobject obj) {
    LOGD("%s:", __FUNCTION__);
    std::string hello = "Hello from C++";
    return env->NewStringUTF("111 Hello from C++ 22...");
}

static bool writeCharecteristicNative(JNIEnv* env, jobject object, jbyte len, jbyteArray data) {
    LOGD("%s:", __FUNCTION__);
    if (amInterface != NULL) {
        jbyte* dataArray = env->GetByteArrayElements(data, 0);
        amInterface->WriteCharacteristic(0, 0, len, (GF_PUINT8)dataArray);
        env->ReleaseByteArrayElements(data, dataArray, JNI_ABORT);
    } else {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static JNINativeMethod sMethods[] = {
    /* name,            signature,         funcPtr */
    {"classInitNative", "()V", (void *) classInitNative},
    {"initNative", "()Z", (void *) initNative},
    {"startScanNative", "()Z", (void *) startScanNative},
    {"stopScanNative", "()Z", (void *) stopScanNative},
    {"cleanupNative", "()Z", (void*) cleanupNative},
    {"stringFromJNI", "()Ljava/lang/String;", (void*)stringFromJNINative},
    {"connectNative", "([B)Z", (void*) connectNative},
    {"cancelConnectNative", "([B)Z", (void*) cancelConnectNative},
    {"disconnectNative", "(I)Z", (void*) disconnectNative},
    {"getHubStateNative", "()B", (void*) getHubStateNative},
    {"getConnectedDevNumNative", "()B", (void*) getConnectedDevNumNative},
    {"writeCharecteristicNative", "(B[B)Z", (void*) writeCharecteristicNative},
};

/*
 * JNI Initialization
 */
jint JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    JNIEnv *env;
    int status;
    jclass cls;

    LOGD("Native-Lib : loading JNI\n");
    globalJavaVM = jvm;

    // Check JNI version
    if (jvm->GetEnv((void **)&env, JNI_VERSION_1_6)) {
        LOGE("JNI version mismatch error");
        return JNI_ERR;
    }

    cls = env->FindClass("com/oym/test/MainActivity");
    if (cls == NULL)
    {
        return JNI_ERR;
    }

    /*Register native function to java code.*/
    env->RegisterNatives(cls, sMethods, sizeof(sMethods) / sizeof(sMethods[0]));

    return JNI_VERSION_1_6;
}
