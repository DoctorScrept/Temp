using UnityEngine;
using System;//.Collections;
using System.Runtime.InteropServices;

public class TracerDevice : USBDevice {
	
//	void Start () {}
	public Plotter p;

//	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
//	protected delegate int SetBufferFunc([In, Out] IntPtr c);

	MUMBuffer.SetBufferFunc setBuffer;
	IntResultFunction getPercentComplete;

	MUMBuffer buffer;
	IntPtr pBuffer;

	bool isWaitingResults;

	void Awake()
	{
		TryLoadDll("CurveTracer3DDriver.dll");
		if (hLib == null) {
			return;
		}

		setBuffer = LoadFunction<MUMBuffer.SetBufferFunc>(hLib, "SetBuffer") as MUMBuffer.SetBufferFunc;
		getPercentComplete = LoadFunction<IntResultFunction>(hLib, "GetPercentComplete") as IntResultFunction;
		buffer = new MUMBuffer();

		if (hLib == null) {
			Debug.LogError("FUckk");
			// One or more functions not found
		}
	}

	void Update () {
		if (isWaitingResults) {
			int percentComplete = getPercentComplete();
			Debug.Log(percentComplete);
			if (percentComplete >= 100) {
				buffer.FromPtr(pBuffer);
				isWaitingResults = false;

				Surface s = new Surface(4, 4);
				s.SetData(buffer.data);
				p.DrawToSlot(0, s);

			}
		}
	}

	public void StatMeasure()
	{
		pBuffer = buffer.ToPtr();
		setBuffer(pBuffer);
		isWaitingResults = true;
	}

}


[StructLayout(LayoutKind.Sequential)]
public struct MUMBuffer
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
	public float[] data;


	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	public delegate int SetBufferFunc([In, Out] IntPtr buffer);

	public IntPtr ToPtr() {
		IntPtr _pStruct_buffer = Marshal.AllocCoTaskMem(Marshal.SizeOf(this));
		Marshal.StructureToPtr(this, _pStruct_buffer, false);
		return _pStruct_buffer;
	}

	public void FromPtr(IntPtr pBuffer) {
		this = (MUMBuffer)Marshal.PtrToStructure(pBuffer, typeof(MUMBuffer));
		Marshal.FreeCoTaskMem(pBuffer);
	}
}