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
