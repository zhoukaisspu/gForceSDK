# gForce SDK

## Brief
gForce SDK is the software development kit for developers to access gForce
armband.

**Note**:
> So far, only Windows is supported by gForce SDK; The Android version will
> come soon.

## Build and run the example code
Besides the documents located in the `doc` directory, gForce SDK also includes
example code to help you quickly understand the API. Please follow the
following steps to build and run the example code:

1. Open `example/gForceExample.sln` in VisualStudio 2013. So far, the solution
    includes just one project named `FetchGForceData`, which is a console
    program and illustrates how to use the gForce SDK to fetch the gForce
    armband data such as gestures and orientations.

2. Build the solution, and the executable will be generated in the `bin`
    directory.

3. Put the gForce armband on your right forearm, and make sure that it is
    attached tightly on your forearm and the USB port points to the inner of
    middle finger.

4. Plug the gForceDongle into an USB port of your PC. Install the driver
    located in the `driver/usb/windows` directory if necessary. Open the
    `Device Manager`, make sure that the gForceDongle appears as a `USB Serial
    Port` under `Port (COM and LPT)`.

5. Turn on the gForce armband by pressing the button on it. The connection
    between the gForce armband and the gForceDongle will be established
    automatically.

6. Run the executable file of previously built `FetchGForceData`, you will see
    the gesture and quaternion data in the console.
