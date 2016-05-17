#include "DriverLibraryMain.h"
#include "step/StepUSBDevice.h"
#include <conio.h>
#include <stdio.h>
#include <windows.h>

StepUSBDevice * stepUsbDevice;

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

extern "C" __declspec(dllexport) int GetMajorVersion() {
	return stepUsbDevice->GetMajorVersion();
}

extern "C" __declspec(dllexport) int GetMinorVersion() {
	return stepUsbDevice->GetMinorVersion();
}

extern "C" __declspec(dllexport) int IsVersionConfirmed() {
	return stepUsbDevice->IsVersionConfirmed();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason){
	case DLL_PROCESS_ATTACH:
		printf("DLL_PROCESS_ATTACH\n");
		stepUsbDevice = new StepUSBDevice();
		break;
	case DLL_THREAD_ATTACH:
		printf("DLL_THREAD_ATTACH)))))))))))))))))))))))\n");
		break;
	case DLL_THREAD_DETACH:
		printf("-----------DLL_THREAD_DETACH\n");
		break;
	case DLL_PROCESS_DETACH:
		printf("DLL_********************************PROCESS****************_DETACH\n");
		delete stepUsbDevice;
		break;
	}
	return TRUE;
}