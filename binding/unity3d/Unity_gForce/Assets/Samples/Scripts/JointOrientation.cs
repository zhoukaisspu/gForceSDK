using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using Gesture = gf.Device.Gesture;

public class JointOrientation : MonoBehaviour
{

    // Use this for initialization
    void Start()
    {

    }

    public GameObject gforce = null;
    public Quaternion quater; // for display in editor only
    public Gesture lastGesture = Gesture.Undefined;

    // Update is called once per frame.
    void Update()
    {
        GForceDevice gforceDevice = gforce.GetComponent<GForceDevice>();

        lastGesture = gforceDevice.gesture;
        quater = gforceDevice.quater;

        transform.rotation = quater;
    }

}
