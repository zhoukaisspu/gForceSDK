# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             gforcecsharp

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
			 ../../../src/gforce_export.cpp
			 ../../../../../../src/platform/ble/android/cpp/com_oymotion_ble_bleservice.cpp
			 ../../../../../../src/BLEDevice.cpp
			 ../../../../../../src/BLEHub.cpp
			 ../../../../../../src/OADProfile.cpp
			 ../../../../../../src/DeviceSettingHandle.cpp
			 ../../../../../../src/DeviceSettingDataProfile4.cpp
			 ../../../../../../src/BLEDataProfile4.cpp
			 ../../../../../../src/SimpleProfile.cpp
			 ../../../../../../src/HubManagerImpl.cpp
			 ../../../../../../src/common/src/LogUtil_Android.cpp)

include_directories(../../../src/)
include_directories(../../../../../../src/platform/ble/android/cpp/)
include_directories(../../../../../../src/ble/inc/)
include_directories(../../../../../../inc/)
include_directories(../../../../../../src/common/)
include_directories(../../../../../../src/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DGFORCE4CS_EXPORTS -fvisibility=hidden")
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       gforcecsharp

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )