using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

using gf;

namespace MgTest
{


    class Program
    {
        private class Listener : HubListener
        {
            public override void onScanFinished()
            {
                System.Console.WriteLine("OnScanFinished");
                if (null == gfrce.mDevice)
                {
                    // if no device found, we do scan again
                    gfrce.mHub.startScan();
                }
                else
                {
                    System.Console.WriteLine("number of devices found: {0}", gfrce.mHub.getNumDevices(false));
                    // enum devices
                    gfrce.mHub.enumDevices(new Hub.deviceEnumFn(gfrce.deviceEnum), false);
                    // or if there already is a device found and it's not
                    //     in connecting or connected state, try to connect it.
                    Device.ConnectionStatus status = gfrce.mDevice.getConnectionStatus();
                    if (Device.ConnectionStatus.Connected != status &&
                        Device.ConnectionStatus.Connecting != status)
                    {
                        gfrce.mDevice.connect();
                    }
                }
            }
            public override void onStateChanged(Hub.HubState state)
            {
                System.Console.WriteLine("onStateChanged");
                if (Hub.HubState.Disconnected == state)
                {
                    gfrce.mLoop = false;
                }
            }
            public override void onDeviceFound(Device device)
            {
                System.Console.WriteLine("onDeviceFound");
                if (null == gfrce.mDevice)
                {
                    gfrce.mDevice = device;
                    string addr, name;
                    device.getAddress(out addr);
                    device.getName(out name);
                    System.Console.WriteLine("Device address type is {0}, address is {1}, name is {2}",
                        device.getAddrType(), addr, name);
                    gfrce.mHub.stopScan();
                }
            }
            public override void onDeviceDiscard(Device device)
            {
                System.Console.WriteLine("onDeviceDiscard");
                if (device == gfrce.mDevice)
                    gfrce.mDevice = null;
            }
            public override void onDeviceConnected(Device device)
            {
                System.Console.WriteLine("onDeviceConnected");
            }
            public override void onDeviceDisconnected(Device device, int reason)
            {
                System.Console.WriteLine("onDeviceDisconnected");
                if (device == gfrce.mDevice)
                    device.connect();
            }
            public override void onOrientationData(Device device,
                float w, float x, float y, float z)
            {
                //System.Console.WriteLine("onOrientationData, w({0}), x({1}), y({2}), z({3})",
                //    w, x, y, z);
            }
            public override void onGestureData(Device device, Device.Gesture gest)
            {
                System.Console.WriteLine("onGestureData: {0}", gest);
            }
            public override void onReCenter(Device device)
            {
                System.Console.WriteLine("onReCenter");
            }

            public Listener(Program theObj)
            {
                gfrce = theObj;
            }

            private Program gfrce = null;
        };

        static void Main(string[] args)
        {
            progm = new Program();
            progm.test();
        }

        private bool deviceEnum(Device dev)
        {
            // TODO: ERROR! cannot operate device due to mutex issue
            //string addr, name;
            //dev.getAddress(out addr);
            //dev.getName(out name);
            System.Console.WriteLine("deviceEnum: {0}", dev);
            return true;
        }

        void test()
        {
            RetCode retCode = RetCode.GF_SUCCESS;

            mLsn = new Listener(this);
            RetCode ret = mHub.registerListener(mLsn);
            mHub.init(0);
            System.Console.WriteLine("Hub status is {0}", mHub.getStatus());
            mHub.setWorkMode(Hub.WorkMode.Polling);
            System.Console.WriteLine("New work mode is {0}", mHub.getWorkMode());

            mHub.startScan();

            // set console handler to receive Ctrl+C command so that we can exit the app by Ctrl+C.
            if (SetConsoleCtrlHandler(ctrlhandler, true))
            {
                do
                {
                    // set up 50ms timeout so we can handle console commands
                    uint period = 50; // ms

                    // Hub::run could be failed in the below cases:
                    //   1. other threads have already been launching it.
                    //   2. WorkMode is not set to WorkMode::Polling.
                    // A return of GF_RET_CODE::GF_ERROR_TIMEOUT means no error but period expired.
                    retCode = mHub.run(period);
                    if (RetCode.GF_SUCCESS != retCode &&
                        RetCode.GF_ERROR_TIMEOUT != retCode)
                    {
                        System.Console.WriteLine("Method run() failed: {0}", retCode);
                        break;
                    }
                }
                // Loop ends while the hub is unplugged or Ctrl+C is pressed.
                while (getLoop() && !bExiting);
            }
            else
            {
                System.Console.WriteLine("Error: could not set control handler.");
            }

        exit:
            mHub.unregisterListener(mLsn);
            mHub.deinit();
            SetConsoleCtrlHandler(ctrlhandler, false);
        }
        // Indicates if we want to exit app
        bool getLoop()
        {
            return mLoop;
        }

        // Indicates if we will keep message polling
        volatile bool mLoop = true;
        volatile bool bExiting = false;
        private Device mDevice = null;
        Listener mLsn = null;
        Hub mHub = Hub.Instance;
        static Program progm;

        public delegate bool ControlCtrlDelegate(int CtrlType);
        [DllImport("kernel32.dll")]
        private static extern bool SetConsoleCtrlHandler(ControlCtrlDelegate HandlerRoutine, bool Add);
        private static ControlCtrlDelegate ctrlhandler = new ControlCtrlDelegate(HandlerRoutine);

        public static bool HandlerRoutine(int CtrlType)
        {
            switch (CtrlType)
            {
                case 0:
                    progm.bExiting = true;
                    break;
                case 2:
                    progm.bExiting = true;
                    break;
            }
            return progm.bExiting;
        }

    }
}
