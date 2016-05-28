#include "DriverLibraryMain.h"
#include "step/StepUSBDevice.h"
#include "tracer/TracerDevice.h"
#include <conio.h>
#include <stdio.h>
#include <windows.h>

#define USE_TRACER

StepUSBDevice * stepUsbDevice = NULL;
TracerDevice * tracerDevice = NULL;
VersionedUSBDevice * versionedUSBDevice = NULL;

//=========================================================
// Common
//=========================================================
extern "C" __declspec(dllexport) int GetMajorVersion() {
	return versionedUSBDevice->GetMajorVersion();
}

extern "C" __declspec(dllexport) int GetMinorVersion() {
	return versionedUSBDevice->GetMinorVersion();
}

extern "C" __declspec(dllexport) int IsVersionConfirmed() {
	return versionedUSBDevice->IsVersionConfirmed();
}

extern "C" __declspec(dllexport) int GetLastDeviceError() {
	return versionedUSBDevice->GetLastError();
}

//=========================================================
// Step
//=========================================================
extern "C" __declspec(dllexport) int Start() {
	return stepUsbDevice->Connect();
}

extern "C" __declspec(dllexport) void TurnLeft() {
	stepUsbDevice->TurnLeft();
} 

extern "C" __declspec(dllexport) void TurnRight() {
	stepUsbDevice->TurnRight();
}

extern "C" __declspec(dllexport) void Stop() {
	stepUsbDevice->Stop();
}

//=========================================================
// Tracer
//=========================================================
extern "C" __declspec(dllexport) int GetPercentComplete() {
	return tracerDevice->measureRequest->GetPercentComplete();
}

extern "C" __declspec(dllexport) int SetBuffer(SurfaceBuffer * buffer)
{
	tracerDevice->measureRequest->SetBuffer(buffer);
	int result = tracerDevice->SendRequestAsync(tracerDevice->measureRequest);

	return result;
}





BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason){
	case DLL_PROCESS_ATTACH:

#ifdef USE_TRACER
		tracerDevice = new TracerDevice();
		versionedUSBDevice = tracerDevice;
		tracerDevice->Connect();
#else
		stepUsbDevice = new StepUSBDevice();
		versionedUSBDevice = stepUsbDevice;
#endif
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		delete stepUsbDevice;
		delete tracerDevice;
		break;
	}
	return TRUE;
}