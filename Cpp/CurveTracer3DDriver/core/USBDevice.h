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

	DeviceRequest * currentRequest;
	CRITICAL_SECTION recvStateCS;
	HANDLE hThread;


	CRITICAL_SECTION baseOperarionsCS;

public:
	USBDevice(char* vidPid, char* inName, char* outName);
	virtual ~USBDevice();

private:
	FARPROC GetAndConfirmFuntion(HMODULE hModule, LPCSTR lpProcName);
	friend void ReqestThread(void* pParams);

	int IsSessionOpen();

protected:
	int OpenSession();
	void CloseSession();

	int InitializeLibrary();
	int InitializeDevice();

	void SetLastError(int error);

public:
	virtual int Connect();

	DWORD SendReceive(PBYTE SendData, DWORD SendLength, PBYTE ReceiveData, DWORD ExpectedReceiveLength, UINT SendDelay, UINT ReceiveDelay);

	int SendRequest(DeviceRequest* request);

	int GetLastError();

	int SendRequestAsync(DeviceRequest* request);
	int IsRequestEnd();

	bool SetRequestIfEmpty(DeviceRequest* request);
	void SetRequest(DeviceRequest* request);
	DeviceRequest* GetRequest();
};

#endif // USB_DEVICE_H