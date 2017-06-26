# How to use Unity3d sample app.

The supported version of Unity is 5.6.1f1+

## Android

### Prerequisites
* Android Studio

### Build Steps

1. Build the following Android Studio projects:
	+ `script/android/gForceSDK`
	+ `binding/unity3d/libgforce/android/gforce`
2. Copy the following files to folder `./binding/unity3d/gforceu3d/Asserts/Plugins/android`:   
	+ `script/android/gForceSDK/ble/build/outputs/aar/ble-{debug|release}.aar`
	+ `binding/unity3d/libgforce/android/gforce/libgforce/build/outputs/aar/libgforce-{debug|release}.aar`
	+ `binding/unity3d/libgforce/android/gforce/libgforce/src/main/AndroidManifest.xml`
3. Open Unity project `binding/uinty3d/gforceu3d`.
4. In the Project view, drag `Asserts -> Samples -> Stick_with_Ball.unity`
   into Hierarchy view.
5. In the Inspector view, configure the gforce object for `Stick_with_Ball`,
   `Sphere` and `SampleGUI`.
6. in the Inspector view, configure materials for `Stick_with_Ball` and `Sphere`.
7. In Hierarchy view, remove the default scene.
8. In `File -> Build Settings`, select `Android`.
4. Select from `File -> Build & Run`.

## Windows

### Prerequisites
* Visual Studio 2013
* .NET Framework 3.5

### Build Steps
1. Open solution `script/vs2013/gForceSDK.sln`.
2. Build project `gForce4CS`.
	+ The built DLL file will be copied to
	  `binding/unity3d/gforceu3d/Asserts/Plugins/win` after build completed.
3. Open Unity project `binding/uinty3d/gforceu3d`.
4. In the Project view, drag `Asserts -> Samples -> Stick_with_Ball.unity`
   into the Hierarchy view.
5. In the Inspector view, configure gforce object for `Stick_with_Ball`,
   `Sphere` and `SampleGUI`.
6. In the Inspector view, configure materials for `Stick_with_Ball` and `Sphere`.
7. In the Hierarchy view, remove the default scene.
8. Click the `PLAY` button to demo and test in Unity.
8. In `File -> Build Settings`, select `PC, MAC & Linux Standalone`.
4. Select `File -> Build & Run`
