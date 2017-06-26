﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using gf;
using System.Runtime.InteropServices;
using System.Threading;

public class gForce : MonoBehaviour
{
    int s = 0;
    string info0 = "Init";
    string info1 = "__empty__";
    string info2 = "__empty__";
    string info3 = "Deinit";
    Hub mHub = Hub.Instance;

    // Use this for initialization
    void start()
    {
        info0 = "init";
        info1 = "startScan";
        info2 = "stopScan";
        mHub.setClientLogMethod(logfun);
        RetCode ret = mHub.registerListener(mLsn);
        Debug.LogFormat("registerListener = {0}", ret);
        s = (int)mHub.init(0);
        info0 = "Init: " + s;
        Debug.LogFormat("init = {0}", ret);
        Debug.LogFormat("Hub status is {0}", mHub.getStatus());
        mHub.setWorkMode(Hub.WorkMode.Polling);
        Debug.LogFormat("New work mode is {0}", mHub.getWorkMode());
        bRunThreadRun = true;
        runThread = new Thread(new ThreadStart(runThreadFn));
        runThread.Start();
    }

    // Is this the exit method?
    void stop()
    {
        Debug.Log("Stop");
        if (null != mDevice)
        {
            mDevice.disconnect();
            mDevice = null;
        }
        bRunThreadRun = false;
        if (runThread != null)
        {
            runThread.Join();
        }
        mHub.unregisterListener(mLsn);
        mHub.setClientLogMethod(null);
        mHub.deinit();
    }

    private Thread runThread;
    private void runThreadFn()
    {
        while (bRunThreadRun)
        {
            mHub.run(50);
        }
        Debug.Log("Leave thread");
    }

    // Update is called once per frame
    void Update()
    {

    }

    void OnGUI()
    {
        if (GUI.Button(new Rect(1, 1, 200, 100), info0))
        {
            start();
        }
        if (GUI.Button(new Rect(201, 1, 200, 100), info1))
        {
            s = (int)mHub.startScan();
            info1 = "startScan: " + s;
        }
        if (GUI.Button(new Rect(401, 1, 200, 100), info2))
        {
            s = (int)mHub.stopScan();
            info2 = "stopScan: " + s;
        }
        if (GUI.Button(new Rect(601, 1, 200, 100), info3))
        {
            stop();
        }
    }

    void OnApplicationQuit()
    {
        if (mHub != null)
        {
            stop();
            mHub.Dispose();
        }
    }

    private Device mDevice = null;

    private class Listener : HubListener
    {
        public override void onScanFinished()
        {
            Debug.Log("OnScanFinished");
            if (null == gfrce.mDevice)
            {
                // if no device found, we do scan again
                gfrce.mHub.startScan();
            }
            else
            {
                Debug.LogFormat("number of devices found: {0}", gfrce.mHub.getNumDevices(false));
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
            Debug.Log("onStateChanged");
        }
        public override void onDeviceFound(Device device)
        {
            Debug.Log("onDeviceFound");
            if (null == gfrce.mDevice)
            {
                gfrce.mDevice = device;
                string addr, name;
                device.getAddress(out addr);
                device.getName(out name);
                Debug.LogFormat("Device address type is {0}, address is {1}, name is {2}",
                    device.getAddrType(), addr, name);
                gfrce.mHub.stopScan();
            }
        }
        public override void onDeviceDiscard(Device device)
        {
            Debug.Log("onDeviceDiscard");
            if (device == gfrce.mDevice)
                gfrce.mDevice = null;
        }
        public override void onDeviceConnected(Device device)
        {
            Debug.Log("onDeviceConnected");
        }
        public override void onDeviceDisconnected(Device device, int reason)
        {
            Debug.Log("onDeviceDisconnected");
            if (device == gfrce.mDevice)
                device.connect();
        }
        public override void onOrientationData(Device device,
            float w, float x, float y, float z)
        {
            //Debug.LogFormat("onOrientationData, w({0}), x({1}), y({2}), z({3})",
            //    w, x, y, z);
        }
        public override void onGestureData(Device device, Device.Gesture gest)
        {
            Debug.LogFormat("onGestureData: {0}", gest);
        }
        public override void onReCenter(Device device)
        {
            Debug.Log("onReCenter");
        }

        public Listener(gForce theObj)
        {
            gfrce = theObj;
        }

        private gForce gfrce = null;
    };

    Listener mLsn = null;

    gForce()
    {
        mLsn = new Listener(this);
    }

    private static void DebugLog(Hub.LogLevel level, string value)
    {
        if (level >= Hub.LogLevel.GF_LOG_ERROR)
            Debug.LogError(value);
        else
            Debug.Log(value);
    }

    private Hub.logFn logfun = new Hub.logFn(gForce.DebugLog);
    private volatile bool bRunThreadRun = false;
}