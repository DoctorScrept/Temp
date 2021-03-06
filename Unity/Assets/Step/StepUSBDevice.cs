﻿using UnityEngine;
using UnityEngine.UI;

public class StepUSBDevice : USBDevice
{
	public Text stateText;

	private IntResultFunction start;
	private IntResultFunction getMajorVersion;
	private IntResultFunction getMinorVersion;
	private IntResultFunction isVersionConfirmed;
	
	private VoidFuntion left;
	private VoidFuntion right;
	private VoidFuntion stop;

	void Awake()
	{
		TryLoadDll("CurveTracer3DDriver.dll");
		if (hLib == null) {
			return;
		}

		start = LoadFunction<IntResultFunction>(hLib, "Start") as IntResultFunction;
		getMajorVersion = LoadFunction<IntResultFunction>(hLib, "GetMajorVersion") as IntResultFunction;
		getMinorVersion = LoadFunction<IntResultFunction>(hLib, "GetMinorVersion") as IntResultFunction;
		isVersionConfirmed = LoadFunction<IntResultFunction>(hLib, "IsVersionConfirmed") as IntResultFunction;
		left = LoadFunction<VoidFuntion>(hLib, "TurnLeft") as VoidFuntion;
		right = LoadFunction<VoidFuntion>(hLib, "TurnRight") as VoidFuntion;
		stop = LoadFunction<VoidFuntion>(hLib, "Stop") as VoidFuntion;

		if (hLib == null) {
			stateText.text = NO_DLL;
		}
	}

	public void Connect()
	{
		System.Text.StringBuilder builder = new System.Text.StringBuilder(100);
		int result = start();
		if (result == 0) {
			builder.Append("Firmware v");
			builder.Append(getMajorVersion());
			builder.Append(".");
			builder.Append(getMinorVersion());

			if (isVersionConfirmed() != 0) {
				builder.Append("\nFirmware version confirmed!");
			} else {
				builder.Append("\nFirmware version error!");

			}

			stateText.text = builder.ToString();
		} else {
			builder.Append("Error code: ");
			builder.Append(result);
		}
	}

	public void TurnLeft() {
		left();
	}

	public void TurnRight() {
		right();
	}

	public void Stop() {
		stop();
	}
}
