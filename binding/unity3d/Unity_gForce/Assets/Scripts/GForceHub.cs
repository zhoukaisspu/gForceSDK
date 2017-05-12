using UnityEngine;
using gf;
using System.Threading;

#if UNITY_EDITOR
using UnityEditor;
#endif

using System.Collections.Generic;


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
            mHub.Dispose();
            mHub = null;

            foreach (GForceDevice dev in mDeviceComps)
            {
                dev.device = null;
            }
        }
#if UNITY_ANDROID && !UNITY_EDITOR
        AndroidJavaClass unityPlayerClass = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
        var unityActivity = unityPlayerClass.GetStatic<AndroidJavaObject>("currentActivity");

        // The Hub needs to be initialized on the Android UI thread.
        unityActivity.Call("runOnUiThread", new AndroidJavaRunnable(() => {
            mHub = Hub.Instance;
        }));
        return true; // Return true assuming the hub constructor will succeed. Debug.Log if it fails.
#else
        mHub = Hub.Instance;
        return true;
#endif
    }

    void Awake()
    {
        // Ensure that there is only one ThalmicHub.
        if (mInstance != null)
        {
#if UNITY_EDITOR
            EditorUtility.DisplayDialog("Can only have one ThalmicHub",
                                        "Your scene contains more than one ThalmicHub. Remove all but one ThalmicHub.",
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
            string errorMessage = "The HubComponent GameObject must have at least one child with a DeviceComonent.";
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

        // The Hub needs to be initialized on the Android UI thread.
        unityActivity.Call("runOnUiThread", new AndroidJavaRunnable(() => {
            mHub = Hub.Instance;
        }));
#else
        mHub = Hub.Instance;
#endif
        prepare();
    }

    void OnApplicationQuit()
    {
        if (mHub != null)
        {
            terminal();
            mHub.Dispose();
            mHub = null;
        }
    }

    void Update()
    {
    }


    private static GForceHub mInstance = null;
    private Hub mHub = null;
    private List<GForceDevice> mDeviceComps = new List<GForceDevice>();
    private Hub.logFn logfun = new Hub.logFn(GForceHub.DebugLog);

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
    private static void DebugLog(Hub.LogLevel level, string value)
    {
        if (level >= Hub.LogLevel.GF_LOG_ERROR)
            Debug.LogError(value);
        else
            Debug.Log(value);
    }
    private void prepare()
    {
        mLsn = new Listener(this);
        mHub.setClientLogMethod(logfun);
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
    }

    private void terminal()
    {
        mHub.deinit();
        mHub.unregisterListener(mLsn);
        mHub.setClientLogMethod(null);
        bRunThreadRun = false;
        if (runThread != null)
        {
            runThread.Join();
        }
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
}

