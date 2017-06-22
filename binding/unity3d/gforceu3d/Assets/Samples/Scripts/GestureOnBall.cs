using System;
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
