#ifndef USB_DEVICE_H
#define USB_DEVICE_H

#include "core/Logger.h"
#include "core/LibraryLoader.h"
#include "core/DeviceRequest.h"
#include "core/USBDeviceError.h"

#include <WinDef.h>

class Logger;

class USBDevice
{
	char * deviceVidPid;
	char * outPipeName;
	char * inPipeName;

	HANDLE outPipe;
	HANDLE inPipe;

	HMODULE hLib;
	
	bool isSessionOpen;
protected:
	Logger * logger;
	int lastError;

public:
	USBDevice(char* vidPid, char* inName, char* outName);
	~USBDevice();

private:
	void CheckInvalidHandle();
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

	int GetLastError();
};

#endif // USB_DEVICE_H