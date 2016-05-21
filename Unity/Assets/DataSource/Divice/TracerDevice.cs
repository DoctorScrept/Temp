using UnityEngine;
using System;//.Collections;
using System.Runtime.InteropServices;

public class TracerDevice : USBDevice {
	
//	void Start () {}
//	void Update () {}

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	protected delegate int SetBufferFunc([In, Out] IntPtr c);

	SetBufferFunc setBuffer;
	MUMBuffer buffer;
	IntPtr pBuffer;

	void Awake()
	{
		TryLoadDll("CurveTracer3DDriver.dll");
		if (hLib == null) {
			return;
		}

		setBuffer = LoadFunction<SetBufferFunc>(hLib, "SetBuffer") as SetBufferFunc;

		buffer = new MUMBuffer();
		IntPtr _pStruct_buffer = Marshal.AllocCoTaskMem(Marshal.SizeOf(buffer));
		Marshal.StructureToPtr(buffer, _pStruct_buffer, false);
		pBuffer = _pStruct_buffer;


		IntPtr ptr1 = Marshal.AllocCoTaskMem(Marshal.SizeOf(buffer));
		Marshal.StructureToPtr(buffer, ptr1, false);
		int retInt = setBuffer(ptr1); // вызов ветода
		buffer = (MUMBuffer)Marshal.PtrToStructure(ptr1, typeof(MUMBuffer)); /// получаем наше значение 

		Marshal.FreeCoTaskMem(_pStruct_buffer);
		Marshal.FreeCoTaskMem(ptr1);
		pBuffer = IntPtr.Zero;

		Debug.Log(buffer.data[0]);
		Debug.Log(retInt);

//		start = LoadFunction<IntResultFunction>(hLib, "Start") as IntResultFunction;
//		getMajorVersion = LoadFunction<IntResultFunction>(hLib, "GetMajorVersion") as IntResultFunction;
//		getMinorVersion = LoadFunction<IntResultFunction>(hLib, "GetMinorVersion") as IntResultFunction;
//		isVersionConfirmed = LoadFunction<IntResultFunction>(hLib, "IsVersionConfirmed") as IntResultFunction;
//		left = LoadFunction<VoidFuntion>(hLib, "TurnLeft") as VoidFuntion;
//		right = LoadFunction<VoidFuntion>(hLib, "TurnRight") as VoidFuntion;
//		stop = LoadFunction<VoidFuntion>(hLib, "Stop") as VoidFuntion;
		
		if (hLib == null) {
			// One or more functions not found
		}
	}
}


[StructLayout(LayoutKind.Sequential)]
public struct MUMBuffer
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
	public float[] data;
}