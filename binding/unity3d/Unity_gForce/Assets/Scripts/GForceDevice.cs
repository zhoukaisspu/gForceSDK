using UnityEngine;

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
        lock (mLock)
        {
            transform.rotation = quater;
            gesture = internalGest;
        }
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
        lock (mLock)
        {
            quater.w = w;
            quater.x = z;
            quater.y = -x;
            quater.z = -y;
        }
    }
    public void onGestureData(Device.Gesture gest)
    {
        Debug.LogFormat("onGestureData: {0} - {1}", mDevice, gest);

        lock (mLock)
        {
            internalGest = gest;
        }
    }
    public void onReCenter()
    {
        Debug.LogFormat("onReCenter: {0}", mDevice);
        lock (mLock)
        {
            recentered = true;
        }
    }

    private Object mLock = new Object();
    private Gesture internalGest = Gesture.Undefined;
}
