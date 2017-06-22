using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SampleGUI : MonoBehaviour
{

    public GameObject gforce = null;
	string log = "Demo text";

    // Draw some basic instructions.
    void OnGUI()
    {
        GUI.skin.label.fontSize = 20;

        GForceHub hub = GForceHub.instance;

        // Access the attached object.
        GForceDevice device = gforce.GetComponent<GForceDevice>();

        if (null != device)
        {
			if (!string.IsNullOrEmpty(hub.lastlog))
				log = hub.lastlog;
            GUI.Label(new Rect(12, 8, Screen.width, Screen.height), log);
        }
        else
        {
            GUI.Label(new Rect(12, 8, Screen.width, Screen.height),
                "Loading failed."
            );
        }
    }

    // Use this for initialization
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
    }
}
