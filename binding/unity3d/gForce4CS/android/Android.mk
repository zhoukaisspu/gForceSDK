# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
GFORCE4CS_PATH := C:\Enjoy\cross_work\oymotion\SDK_BLE-dev\binding\unity3d\gForce4CS
LIBGFORCE_PATH := C:\Enjoy\cross_work\oymotion\SDK_BLE-dev\src
#LIBBLE_PATH := $(LOCAL_PATH)/../../../../script/android/BLELibrary/app/src/main/cpp

include $(CLEAR_VARS)

LOCAL_MODULE    := gforce
LOCAL_C_INCLUDES := $(GFORCE4CS_PATH)/lib \
					$(GFORCE4CS_PATH)/android \
					$(LIBGFORCE_PATH)/../inc \
					$(LIBGFORCE_PATH)/common \
					$(LIBGFORCE_PATH)/ble/inc \
					$(LIBGFORCE_PATH)
					
LOCAL_SRC_FILES := \
    $(GFORCE4CS_PATH)/lib/gforce_export.cpp
	
LOCAL_SRC_FILES += \
    $(GFORCE4CS_PATH)/android/com_oymotion_libble_ble.cpp
#    $(LIBBLE_PATH)/com_oym_blelibrary_bleservice.cpp
	
LOCAL_SRC_FILES += \
    $(LIBGFORCE_PATH)/BLEDevice.cpp \
    $(LIBGFORCE_PATH)/BLEHub.cpp \
    $(LIBGFORCE_PATH)/GForceDevice.cpp \
    $(LIBGFORCE_PATH)/HubManagerImpl.cpp \
    $(LIBGFORCE_PATH)/common/src/LogUtil_Android.cpp
	
LOCAL_LDLIBS += -llog
LOCAL_CPP_FEATURES := exceptions
LOCAL_CFLAGS += -DGFORCE4CS_EXPORTS -fvisibility=hidden

include $(BUILD_SHARED_LIBRARY)