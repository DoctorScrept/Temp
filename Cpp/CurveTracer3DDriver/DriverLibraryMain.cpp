#include "DriverLibraryMain.h"
#include "step/StepUSBDevice.h"
#include "tracer/TracerDevice.h"
#include <conio.h>
#include <stdio.h>
#include <windows.h>

#define USE_TRACER

StepUSBDevice * stepUsbDevice = NULL;
TracerDevice * tracerDevice = NULL;
USBDevice * device = NULL;
GetVersionRequest * getVersionRequest = NULL;

//=========================================================
// Common
//=========================================================
extern "C" __declspec(dllexport) int GetMajorVersion() {
	return getVersionRequest->GetMajorVersion();
}

extern "C" __declspec(dllexport) int GetMinorVersion() {
	return getVersionRequest->GetMinorVersion();
}

extern "C" __declspec(dllexport) int IsVersionConfirmed()
{
#ifdef USE_TRACER
	return tracerDevice->IsVersionConfirmed();
#else
	return stepUsbDevice->IsVersionConfirmed();
#endif
}

extern "C" __declspec(dllexport) int GetLastDeviceError() {
	return device->GetLastError();
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

//int tempCounter;
//void s(float val) {
//	tracerDevice->measureRequest->buffer->inData[tempCounter] = val;
//	tempCounter++;
//}

extern "C" __declspec(dllexport) int SetBuffer(SurfaceBuffer * buffer)
{
	tracerDevice->measureRequest->SetBuffer(buffer);
	tracerDevice->SendRequestAsync(tracerDevice->measureRequest);

	//tracerDevice->measureRequest->buffer = buffer;
	//tempCounter = 0;
	//s(1); s(1); s(0); s(0);
	//s(0); s(1); s(1); s(2);
	//s(0); s(1); s(0); s(2);
	//s(0); s(1); s(0); s(0);

	return 19;
}





BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason){
	case DLL_PROCESS_ATTACH:

#ifdef USE_TRACER
		tracerDevice = new TracerDevice();
		printf("%d", tracerDevice);
		device = tracerDevice;
		getVersionRequest = tracerDevice->getVersionRequest;
		tracerDevice->Connect();
#else
		stepUsbDevice = new StepUSBDevice();
		device = stepUsbDevice;
		getVersionRequest = stepUsbDevice->getVersionRequest;
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