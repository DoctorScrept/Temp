using System;
using UnityEngine;
using System.Runtime.InteropServices;

public class USBDevice : MonoBehaviour {
	
	protected IntPtr? hLib = null;

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	protected delegate int IntResultFunction();
	
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	protected delegate int VoidFuntion();

	protected int lastError = 0;
	protected string lastErrorText = null;

	protected const string NO_DLL = "Library or functions not found";

	protected string[] additionalPaths = {
		@"..\Cpp\CurveTracer3DDriver\Debug\",
		@"..\Cpp\CurveTracer3DDriver\Release\"
	};

	protected void TryLoadDll(string name)
	{
		hLib = NativeLibraries.LoadLibrary(name);
		if (hLib.Value == IntPtr.Zero) {
			foreach (string additionalPath in additionalPaths) {
				string fullPath = additionalPath + name;
				hLib = NativeLibraries.LoadLibrary(fullPath);
				if (hLib.Value != IntPtr.Zero) {
					return;
				}
			}
			hLib = null;
		}
	}

	protected virtual void OnApplicationQuit() {
		ClearDll();
	}

	protected void ClearDll()
	{
		if (hLib == null) {
			return;
		}
		if (!NativeLibraries.FreeLibrary(hLib.Value)) {

		}
		hLib = null;
	}

	protected Delegate LoadFunction<T>(IntPtr? hModule, string name) where T: class
	{
		if (hModule == null) {
			return null;
		}
		IntPtr pAddressOfFunctionToCall = NativeLibraries.GetProcAddress(hModule.Value, name);
		if (pAddressOfFunctionToCall == IntPtr.Zero) {
			ClearDll();
			return null;
		}
		return Marshal.GetDelegateForFunctionPointer(pAddressOfFunctionToCall, typeof(T));
	}

	public bool IsInitialized() {
		return hLib != null;
	}

	public virtual string GetErrorText()
	{
		string result = null;
		if (lastError != 0) {
			result = lastError.ToString();
			lastError = 0;
		}
		if (lastErrorText != null) {
			result = lastErrorText;
			lastErrorText = null;
		}
		return result;
	}
}