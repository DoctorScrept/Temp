#ifndef USB_DEVICE_H
#define USB_DEVICE_H

#include "core/Logger.h"
#include "core/LibraryLoader.h"
#include "core/DeviceRequest.h"
#include "core/USBDeviceError.h"

#include <WinDef.h>

class USBDevice
{
	char * deviceVidPid;
	char * outPipeName;
	char * inPipeName;

	HANDLE outPipe;
	HANDLE inPipe;

	HMODULE hLib;
	int lastError;
	bool isSessionOpen;
protected:
	Logger * logger;

public:
	USBDevice(char* vidPid, char* inNAme, char* outName);
	~USBDevice();

private:
	void CheckInvalidHandle();
	void Confirm(void* pFunction) {
		if (pFunction == NULL) {
			lastError = NO_FUNCTION;
		}
	}

	FARPROC GetAndConfirmFuntion(HMODULE hModule, LPCSTR lpProcName);

protected:
	int OpenSession();
	void CloseSession();

	int InitializeLibrary();
	int InitializeDevice();

	int IsSessionOpen();

public:
	virtual int Connect();

	DWORD SendReceive(PBYTE SendData, DWORD SendLength, PBYTE ReceiveData, DWORD ExpectedReceiveLength, UINT SendDelay, UINT ReceiveDelay);

	int SendRequest(DeviceRequest* request);
};

#endif // USB_DEVICE_H