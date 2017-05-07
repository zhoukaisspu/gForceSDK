using UnityEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


public class DeviceComponent : MonoBehaviour
{

    void Start()
    {
    }

    void Update()
    {
    }

    public gf.Device device
    {
        get { return mDevice; }
        set
        {
            mDevice = value;
        }
    }

    private gf.Device mDevice;
}
