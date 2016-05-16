using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System;

public class StepControler : MonoBehaviour {

//	[DllImport("dllForUnity", EntryPoint = "Func1")]
//	public static extern int Test();

	IntPtr? hLib = null;

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate int GetInt();

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate int SetDirection();

	GetInt start;
	GetInt getMajorVersion;

	SetDirection left;
	SetDirection stop;

	void Start ()
	{
//		LoadFuntions(@"E:\SndBox\dllForUnity.dll");
		LoadFuntions(@"E:\Diplom\TempDocs\Cpp\CurveTracer3DDriver\Debug\CurveTracer3DDriver.dll");
//		LoadFuntions(@"E:\SndBox\CurveTracer3DDriver.dll");
//		Debug.Log(test());
		int s = start ();
		Debug.Log (s);
		Debug.Log (getMajorVersion ());
	}

	void OnApplicationQuit() {
		ClearDll();
	}
	
	private void LoadFuntions(string dllPath)
	{
		hLib = NativeMethods.LoadLibrary(dllPath);
		if (hLib.Value == IntPtr.Zero) {
			Debug.LogError("DLL not found");
			//ClearDll();
			return;
		}

		IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(hLib.Value, "Start");
		if (pAddressOfFunctionToCall == IntPtr.Zero) {
			Debug.Log("Function not found");
			ClearDll();
			return;
		}
		start = (GetInt)Marshal.GetDelegateForFunctionPointer(pAddressOfFunctionToCall, typeof(GetInt));


		pAddressOfFunctionToCall = NativeMethods.GetProcAddress(hLib.Value, "GetMajorVersion");
		if (pAddressOfFunctionToCall == IntPtr.Zero) {
			Debug.Log("Function not found");
			ClearDll();
			return;
		}
		getMajorVersion = (GetInt)Marshal.GetDelegateForFunctionPointer(pAddressOfFunctionToCall, typeof(GetInt));
	



		pAddressOfFunctionToCall = NativeMethods.GetProcAddress(hLib.Value, "TurnLeft");
		if (pAddressOfFunctionToCall == IntPtr.Zero) {
			Debug.Log("Function not found");
			ClearDll();
			return;
		}
		left = (SetDirection)Marshal.GetDelegateForFunctionPointer(pAddressOfFunctionToCall, typeof(SetDirection));

		pAddressOfFunctionToCall = NativeMethods.GetProcAddress(hLib.Value, "Stop");
		if (pAddressOfFunctionToCall == IntPtr.Zero) {
			Debug.Log("Function not found");
			ClearDll();
			return;
		}
		stop = (SetDirection)Marshal.GetDelegateForFunctionPointer(pAddressOfFunctionToCall, typeof(SetDirection));

	}

	private void ClearDll()
	{
		if (hLib == null) {
			return;
		}
		
		if (!NativeMethods.FreeLibrary (hLib.Value)) {
			Debug.LogError("FreeLibrary error");
		}
		
		hLib = null;
	}

	public void Left() {
		left ();
	}
	public void Stop() {
		stop ();
	}
}

static class NativeMethods
{
	[DllImport("kernel32.dll")]
	public static extern IntPtr LoadLibrary(string dllToLoad);
	
	[DllImport("kernel32.dll")]
	public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);
	
	[DllImport("kernel32.dll")]
	public static extern bool FreeLibrary(IntPtr hModule);
}
