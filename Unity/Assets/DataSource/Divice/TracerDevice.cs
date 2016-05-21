using UnityEngine;
using System;//.Collections;
using System.Runtime.InteropServices;

public class TracerDevice : USBDevice {
	
//	void Start () {}
	public Plotter p;

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	protected delegate int SetBufferFunc([In, Out] IntPtr c);

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	protected delegate bool IsReseivedFunc();

	SetBufferFunc setBuffer;
	IsReseivedFunc isReseived;
	MUMBuffer buffer;
	IntPtr pBuffer;

	bool isWaitingResults;

	void Awake()
	{
		TryLoadDll("CurveTracer3DDriver.dll");
		if (hLib == null) {
			return;
		}

		setBuffer = LoadFunction<SetBufferFunc>(hLib, "SetBuffer") as SetBufferFunc;
		isReseived = LoadFunction<IsReseivedFunc>(hLib, "IsReseived") as IsReseivedFunc;
		buffer = new MUMBuffer();

		if (hLib == null) {
			// One or more functions not found
		}
	}

	void Update () {
		if (isWaitingResults) {
//			f ();
			if (isReseived ()) {
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
		int r = setBuffer(pBuffer);
		isWaitingResults = true;
	}

}


[StructLayout(LayoutKind.Sequential)]
public struct MUMBuffer
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
	public float[] data;

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