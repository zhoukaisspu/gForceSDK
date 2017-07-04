/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */
﻿using UnityEngine;
using System.Threading;
using System.Collections.Generic;
using gf;

#if UNITY_EDITOR
using UnityEditor;
#endif



public class GForceHub : MonoBehaviour
{
    public static GForceHub instance
    {
        get { return mInstance; }
    }

    public bool Reset()
    {
        if (mHub != null)
        {
            foreach (GForceDevice dev in mDeviceComps)
            {
                dev.device.disconnect();
                dev.device = null;
            }

            mHub.Dispose();
            mHub = null;
        }
#if UNITY_ANDROID && !UNITY_EDITOR
        AndroidJavaClass unityPlayerClass = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
        var unityActivity = unityPlayerClass.GetStatic<AndroidJavaObject>("currentActivity");

        unityActivity.Call("runOnUiThread", new AndroidJavaRunnable(() => {
            mHub = Hub.Instance;
            prepare();
        }));
        return true;
#else
        mHub = Hub.Instance;
        prepare();
        return true;
#endif
    }

    void Awake()
    {
        // Ensure that there is only one Hub.
        if (mInstance != null)
        {
#if UNITY_EDITOR
            EditorUtility.DisplayDialog("Can only have one Hub",
                                        "Your scene contains more than one Hub. Remove all but one Hub.",
                                        "OK");
#endif
            Destroy(this.gameObject);
            return;
        }
        else
        {
            mInstance = this;
        }

        // Do not destroy this game object. This will ensure that it remains active even when
        // switching scenes.
        DontDestroyOnLoad(this);

        for (int i = 0; i < transform.childCount; ++i)
        {
            Transform child = transform.GetChild(i);

            var dev = child.gameObject.GetComponent<GForceDevice>();
            if (dev != null)
            {
                mDeviceComps.Add(dev);
            }
        }

        if (mDeviceComps.Count < 1)
        {
            string errorMessage = "The GForceHub GameObject must have at least one child with a DeviceComonent.";
#if UNITY_EDITOR
            EditorUtility.DisplayDialog("No DeviceComponent child.", errorMessage, "OK");
#else
            throw new UnityException (errorMessage);
#endif
        }

#if UNITY_ANDROID && !UNITY_EDITOR
        AndroidJavaClass unityPlayerClass = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
        var unityActivity = unityPlayerClass.GetStatic<AndroidJavaObject>("currentActivity");

        var applicationContext = unityActivity.Call<AndroidJavaObject>("getApplicationContext");

        // Need to pass the Android Application Context to the jni plugin before initializing the Hub.
        AndroidJavaClass nativeEventsClass = new AndroidJavaClass("com.oymotion.ble.GlobalContext");
        nativeEventsClass.CallStatic("setApplicationContext", applicationContext);

        unityActivity.Call("runOnUiThread", new AndroidJavaRunnable(() => {
            mHub = Hub.Instance;
            prepare();
        }));
#else
        mHub = Hub.Instance;
        prepare();
#endif
    }

    void OnApplicationQuit()
    {
        if (mHub != null)
        {
            terminal();
            mHub.Dispose();
            mHub = null;
        }
        Debug.Log("GForceHub.OnApplicationQuit");
    }

    void Update()
    {
    }


    private static GForceHub mInstance = null;
    private Hub mHub = null;
	private List<GForceDevice> mDeviceComps = new List<GForceDevice>();
#if !UNITY_ANDROID
    private Hub.logFn logfun = new Hub.logFn(GForceHub.DebugLog);
#endif

    private class Listener : HubListener
    {
        public override void onScanFinished()
        {
            Debug.Log("OnScanFinished");

            int found = 0;
            foreach (GForceDevice dev in hubcomp.mDeviceComps)
            {
                if (null == dev.device)
                {
                    // if not all preset devices found, do scan again
                    Debug.LogFormat("{0} devices found, not enough, re-scan again.", found);
                    hubcomp.mHub.startScan();
                    return;
                }
                else
                {
                    found++;
                }
            }

            Debug.Log("Now try to connect to gForce one by one.");
            foreach (GForceDevice dev in hubcomp.mDeviceComps)
            {
                Device.ConnectionStatus status = dev.device.getConnectionStatus();
                if (Device.ConnectionStatus.Connected != status &&
                    Device.ConnectionStatus.Connecting != status)
                {
                    dev.device.connect();
                    // one time connect to one device only
                    return;
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
            int i = 0;
            foreach (GForceDevice dev in hubcomp.mDeviceComps)
            {
                ++i;
                if (null == dev.device)
                {
                    // assign the new device to an empty DeviceComponent
                    dev.device = device;
                    break;
                }
            }
            if (i >= hubcomp.mDeviceComps.Count)
            {
                // all DeviceComponent objects got a Device object
                hubcomp.mHub.stopScan();
            }
        }
        public override void onDeviceDiscard(Device device)
        {
            Debug.Log("onDeviceDiscard");
            foreach (GForceDevice dev in hubcomp.mDeviceComps)
            {
                if (device == dev.device)
                {
                    dev.device = null;
                    break;
                }
            }
        }
        public override void onDeviceConnected(Device device)
        {
            Debug.Log("onDeviceConnected");
            foreach (GForceDevice dev in hubcomp.mDeviceComps)
            {
                // since we connect to device one by one,
                // we need to connect the next while the previous one ready
                Device.ConnectionStatus status = dev.device.getConnectionStatus();
                if (Device.ConnectionStatus.Connected != status &&
                    Device.ConnectionStatus.Connecting != status)
                {
                    dev.device.connect();
                    // one time connect to one device only
                    return;
                }
            }
        }
        public override void onDeviceDisconnected(Device device, int reason)
        {
            Debug.LogFormat("onDeviceDisconnected, reason is {0}", reason);
            foreach (GForceDevice dev in hubcomp.mDeviceComps)
            {
                Device.ConnectionStatus status = dev.device.getConnectionStatus();
                if (Device.ConnectionStatus.Connecting == status)
                {
                    // a device is in connecting state, we will do something after it connected
                    return;
                }
            }
            foreach (GForceDevice dev in hubcomp.mDeviceComps)
            {
                if (device == dev.device)
                {
                    // no other devices are connecting, try it
                    device.connect();
                }
            }
        }
        public override void onOrientationData(Device device,
            float w, float x, float y, float z)
        {
            foreach (GForceDevice dev in hubcomp.mDeviceComps)
            {
                if (device == dev.device)
                    dev.onOrientationData(w, x, y, z);
            }
        }
        public override void onGestureData(Device device, Device.Gesture gest)
        {
            foreach (GForceDevice dev in hubcomp.mDeviceComps)
            {
                if (device == dev.device)
                    dev.onGestureData(gest);
            }
        }
        public override void onReCenter(Device device)
        {
            foreach (GForceDevice dev in hubcomp.mDeviceComps)
            {
                if (device == dev.device)
                    dev.onReCenter();
            }
        }

        public Listener(GForceHub theObj)
        {
            hubcomp = theObj;
        }

        private GForceHub hubcomp = null;
    };

    Listener mLsn = null;
    private volatile bool bRunThreadRun = false;

	public string lastlog;
    private static void DebugLog(Hub.LogLevel level, string value)
    {
		mInstance.lastlog = value;
        if (level >= Hub.LogLevel.GF_LOG_ERROR)
            Debug.LogError(value);
        else
            Debug.Log(value);
    }
    private void prepare()
    {
        mLsn = new Listener(this);
#if !UNITY_ANDROID
        mHub.setClientLogMethod(logfun);
#endif
        RetCode ret = mHub.registerListener(mLsn);
        Debug.LogFormat("registerListener = {0}", ret);
        ret = mHub.init(0);
        Debug.LogFormat("init = {0}", ret);
        Debug.LogFormat("Hub status is {0}", mHub.getStatus());
        mHub.setWorkMode(Hub.WorkMode.Polling);
        Debug.LogFormat("New work mode is {0}", mHub.getWorkMode());
        bRunThreadRun = true;
        runThread = new Thread(new ThreadStart(runThreadFn));
        runThread.Start();
        ret = mHub.startScan();
        Debug.LogFormat("startScan = {0}", ret);
		if (RetCode.GF_SUCCESS == ret) {
			lastlog = "BLE scan starting succeeded.";
		} else {
			lastlog = "BLE scan starting failed.";
		}
    }

    private void terminal()
    {
        bRunThreadRun = false;
        if (runThread != null)
        {
            runThread.Join();
        }
		mHub.unregisterListener(mLsn);
#if !UNITY_ANDROID
        mHub.setClientLogMethod(null);
#endif
        mHub.deinit();
    }
    private Thread runThread;
    private void runThreadFn()
    {
		int loop = 0;
        while (bRunThreadRun)
        {
			loop++;
            mHub.run(50);
#if DEBUG
			if (loop % 200 == 0)
				Debug.LogFormat("runThreadFn: {0} seconds elapsed.", loop/20);
#endif
        }
        Debug.Log("Leave thread");
    }
}

