using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

using System.Collections.Generic;


public class HubComponent : MonoBehaviour
{
    public static HubComponent instance
    {
        get { return mInstance; }
    }

    public bool Reset()
    {
        if (mHub != null)
        {
            mHub.Dispose();
            mHub = null;

            foreach (DeviceComponent dev in mDevices)
            {
                //myo.internalMyo = null;
            }
        }
#if UNITY_ANDROID && !UNITY_EDITOR
        AndroidJavaClass unityPlayerClass = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
        var unityActivity = unityPlayerClass.GetStatic<AndroidJavaObject>("currentActivity");

        // The Hub needs to be initialized on the Android UI thread.
        unityActivity.Call("runOnUiThread", new AndroidJavaRunnable(() => {
            mHub = gf.Hub.Instance;
        }));
        return true; // Return true assuming the hub constructor will succeed. Debug.Log if it fails.
#else
        mHub = gf.Hub.Instance;
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

            var dev = child.gameObject.GetComponent<DeviceComponent>();
            if (dev != null)
            {
                mDevices.Add(dev);
            }
        }

        if (mDevices.Count < 1)
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
            mHub = gf.Hub.Instance;
        }));
#else
        mHub = gf.Hub.Instance;
#endif
    }

    void OnApplicationQuit()
    {
        if (mHub != null)
        {
            mHub.Dispose();
            mHub = null;
        }
    }

    void Update()
    {
    }


    private static HubComponent mInstance = null;

    private gf.Hub mHub = null;

    private List<DeviceComponent> mDevices = new List<DeviceComponent>();
}

