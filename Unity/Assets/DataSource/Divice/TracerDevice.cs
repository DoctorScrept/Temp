using System;
using UnityEngine;
using System.Runtime.InteropServices;

public class TracerDevice : USBDevice
{
	public const int MIN_PERSENT = 0;
	public const int MAX_PERSENT = 100;
	public const int NO_DATA = -1;

	private int percentComplete = NO_DATA;

	private SurfaceBuffer.SetBufferFunc setBuffer;
	private IntResultFunction getPercentComplete;

	private SurfaceBuffer buffer;
	private IntPtr pBuffer;

	void Awake()
	{
		TryLoadDll("CurveTracer3DDriver.dll");
		if (hLib == null) {
			return;
		}

		setBuffer = LoadFunction<SurfaceBuffer.SetBufferFunc>(hLib, "SetBuffer") as SurfaceBuffer.SetBufferFunc;
		getPercentComplete = LoadFunction<IntResultFunction>(hLib, "GetPercentComplete") as IntResultFunction;
		buffer = new SurfaceBuffer();

		if (hLib == null) {
			lastErrorText = NO_DLL;
		}
	}

	void Update ()
	{
		if (percentComplete >= MIN_PERSENT && percentComplete < MAX_PERSENT)
		{
			percentComplete = getPercentComplete();
			Debug.Log(percentComplete);
			if (percentComplete >= MAX_PERSENT) {
				buffer.FromPtr(pBuffer);
			}
		}
	}

	public bool StatMeasure()
	{
		if (!IsInitialized()) {
			lastErrorText = NO_DLL;
			return false;
		}

		pBuffer = buffer.ToPtr();
		lastError = setBuffer(pBuffer);
		if (lastError == 0) {
			percentComplete = MIN_PERSENT;
			return true;
		}
		return false;
	}

	public void StopMeasure() {
		percentComplete = NO_DATA;
	}

	public int GetPercentComplete() {
		return percentComplete;
	}

	public float[] GetData()
	{
		if (percentComplete >= MAX_PERSENT) {
			percentComplete = NO_DATA;
			return buffer.data;
		} 
		return null;
	}

	public override string GetErrorText()
	{
		if (lastError == 3) {
			return "Прилад не підключено";
		}
		return base.GetErrorText();
	}
}