﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Heep;

public class FakeDevice : MonoBehaviour {

	HeepDevice myDevice;

	void OnApplicationQuit()
	{
		Debug.Log("Application ending after " + Time.time + " seconds");
		myDevice.CloseDevice ();
	}

	void CreateHeepDevice()
	{
		List <byte> ID = new List<byte>();
		for(byte i = 0; i < 4; i++)
		{
			ID.Add(i);
		}

		DeviceID myID = new DeviceID(ID);
		myDevice = new HeepDevice (myID);

		Control theControl = Control.CreateControl (Control.CtrlInputOutput.input, Control.CtrlType.OnOff, "First");
		myDevice.AddControl (theControl);
		Control newControl = Control.CreateControl (Control.CtrlInputOutput.output, Control.CtrlType.OnOff, "Second");
		myDevice.AddControl (newControl);
		myDevice.SetDeviceName ("Unity");
		myDevice.StartListening ();
//		HeepCommunications.StartHeepServer (myDevice);
	}

	IEnumerator setControlsOnTimer()
	{
		int curValue = 0;

		while (true) {
			Debug.Log ("Setting to " + curValue);
			myDevice.SetControlByID (1, curValue);
			yield return new WaitForSeconds (2);

			if (curValue == 0)
				curValue = 1;
			else
				curValue = 0;
		}
	}

	// Use this for initialization
	void Start () {
		CreateHeepDevice ();
		StartCoroutine (setControlsOnTimer());
	}

	// Update is called once per frame
	void Update () {
		
	}
}
