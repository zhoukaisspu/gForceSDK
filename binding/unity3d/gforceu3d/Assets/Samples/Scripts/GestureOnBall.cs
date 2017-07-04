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
﻿using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using Gesture = gf.Device.Gesture;

public class GestureOnBall : MonoBehaviour
{

    public GameObject gforce = null;
    public Quaternion quater; // for display in editor only
    public Gesture lastGesture = Gesture.Undefined;

    public Material relaxMaterial;
    public Material shootMaterial;
    public Material otherMaterial;
    public Material undefinedMaterial;

    private DateTime relaxStart = DateTime.Now;
    private const int RelaxDelay = 300;

    // Use this for initialization
    void Start()
    {
        GetComponent<Renderer>().material = relaxMaterial;
    }

    // Update is called once per frame
    void Update()
    {
        GForceDevice gforceDevice = gforce.GetComponent<GForceDevice>();
        quater = gforceDevice.quater;

        // Check if the gesture has changed since last update.
        if (gforceDevice.gesture != lastGesture)
        {
            lastGesture = gforceDevice.gesture;

            switch (lastGesture)
            {
                case Gesture.Relax:
                    // seems Relax is always coming fast, so setup a delay timer,
                    // only when Relax keeps for a while, then render relaxMaterial.
                    //GetComponent<Renderer>().material = relaxMaterial;
                    relaxStart = DateTime.Now;
                    break;
                case Gesture.Shoot:
                    GetComponent<Renderer>().material = shootMaterial;
                    break;
                case Gesture.Undefined:
                    GetComponent<Renderer>().material = undefinedMaterial;
                    break;
                default:
                    GetComponent<Renderer>().material = otherMaterial;
                    break;
            }
        }
        else
        {
            if (Gesture.Relax == lastGesture &&
                GetComponent<Renderer>().material != relaxMaterial)
            {
                TimeSpan ts = DateTime.Now - relaxStart;
                if (ts.Milliseconds > RelaxDelay)
                    GetComponent<Renderer>().material = relaxMaterial;
            }
        }
    }
}
