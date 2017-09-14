# gForce SDK

## Brief
gForce SDK is the software development kit for developers to access gForce
armband.

**Note**:
> So far, only Windows is supported by gForce SDK; The Android version will
> come soon.

## Build and run the example code
Besides the documents located in the `doc` directory, gForce SDK also includes
example code to get you started quickly. The following steps instruct you to
building and running the example code:

1. Open `example/gForceExample.sln` in VisualStudio 2013. So far, the solution
    includes just one project named `FetchGForceData`, which is a console
    program and illustrates how to use the gForce SDK to fetch the gForce
    armband data such as gestures and orientations.

2. Build the solution, and the executable will be generated in the `bin`
    directory.

3. Put the gForce armband on your right forearm, and make sure that the USB port
    points to your medial middle finger. Please also make sure that armband is
    attached tightly onto your forearm.

4. Plug the gForceDongle into an USB port of your PC. Install the driver
    located in the `driver/usb/windows` directory if necessary. Open the
    `Device Manager`, make sure that the gForceDongle appears as a `USB Serial
    Port` under `Port (COM and LPT)`.

5. Turn on the gForce armband by pressing the button on it. The connection
    between the gForce armband and the gForceDongle will be established
    automatically. After connection is established successfully, the LED light
    will flash frequently.

6. Run the executable file of previously built `FetchGForceData`, you will be
    able to see the gesture and quaternion data printed in the console.

## Build and run Test Project
In gForceSDK,[googletest](https://github.com/google/googletest) is used as test
framework, we add it as a submodule of the repository that we're working on.
When you clone [gForceSDK](https://github.com/oymotion/gForceSDK),by default you
get the directories that contain submodule, but none of the files within them.
You must run two commands:```git submodule init``` and ```git submodule update```.

```
$ git submodule init
Submodule 'external/googletest' (https://github.com/google/googletest.git) registered for path 'external/googletest'
$ git submodule update
Cloning into 'E:/tempFile/gForceSDK/external/googletest'...
Submodule path 'external/googletest': checked out '61330388862cf011fa956f7f59082b9923e6be0e'
```
***Note:*** we should compile the `gtest` in```./external/googletest``` before compile the 
`gtestForgForceSDK` in VisualStudio 2013.  

## API documentation
Please see `doc/html/index.html`.

## Known issues
1. After the application exits abnormally, the connection between the gForce
    armband and the gForceDongle may remain. This will cause the next
    connection to fail when you restart the application. To work around this
    issue, you need to unplug-and-plug the gForceDongle or turn-off-and-on
    gForce armband again, and restart the application.
