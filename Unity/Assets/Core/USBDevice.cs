using System;
using UnityEngine;
using System.Runtime.InteropServices;

public class USBDevice : MonoBehaviour {
	
	protected IntPtr? hLib = null;

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	protected delegate int IntResultFunction();
	
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	protected delegate int VoidFuntion();

	protected void TryLoadDll(string path)
	{
		hLib = NativeMethods.LoadLibrary(path);
		if (hLib.Value == IntPtr.Zero) {
			hLib = null;
		}
	}
	
	protected void ClearDll()
	{
		if (hLib == null) {
			return;
		}
		if (!NativeMethods.FreeLibrary(hLib.Value)) {

		}
		hLib = null;
	}

	protected Delegate LoadFunction<T>(IntPtr? hModule, string name) where T: class
	{
		if (hModule == null) {
			return null;
		}
		IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(hModule.Value, name);
		if (pAddressOfFunctionToCall == IntPtr.Zero) {
			ClearDll();
			return null;
		}
		return Marshal.GetDelegateForFunctionPointer(pAddressOfFunctionToCall, typeof(T));
	}

	public bool IsReady() {
		return hLib != null;
	}
}