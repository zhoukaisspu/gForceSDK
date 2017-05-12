using UnityEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using gf;

using Gesture = gf.Device.Gesture;

public class GForceDevice : MonoBehaviour
{
    public Gesture gesture = Gesture.Undefined;
    public Quaternion quater = new Quaternion();
    public bool recentered = false; // handled and reset outside by client

    void Start()
    {
    }

    void Update()
    {
    }

    public Device device
    {
        get { return mDevice; }
        set
        {
            mDevice = value;
        }
    }

    private Device mDevice;

    public void onOrientationData(float w, float x, float y, float z)
    {
        quater.w = w;
        quater.x = x;
        quater.y = y;
        quater.z = z;
    }
    public void onGestureData(Device.Gesture gest)
    {
        Debug.LogFormat("onGestureData: {0} - {1}", mDevice, gest);
        gesture = gest;
    }
    public void onReCenter()
    {
        Debug.LogFormat("onReCenter: {0}", mDevice);
        recentered = true;
    }
}
