using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct SurfaceBuffer
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
		this = (SurfaceBuffer)Marshal.PtrToStructure(pBuffer, typeof(SurfaceBuffer));
		Marshal.FreeCoTaskMem(pBuffer);
	}
}