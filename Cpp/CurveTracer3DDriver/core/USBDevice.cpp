#include "USBDevice.h" 
#include "lib/mpusbapi.h"


USBDevice::USBDevice(char* vidPid, char* inName, char* outName)
{
	lastError = STATE_OK;
	deviceVidPid = vidPid;
	outPipeName = inName;
	inPipeName = outName;
	logger = new Logger();
	isSessionOpen = false;

	InitializeCriticalSection(&recvStateCS);
	InitializeCriticalSection(&baseOperarionsCS);
	currentRequest = NULL;
}

USBDevice::~USBDevice()
{
	CloseSession();
	LibraryLoader::FreeDll(hLib);
	delete logger;
}

FARPROC USBDevice::GetAndConfirmFuntion(HMODULE hModule, LPCSTR lpProcName)
{
	FARPROC result = GetProcAddress(hModule, lpProcName);
	if (result == NULL) {
		lastError = NO_FUNCTION;
	}
	return result;
}

int USBDevice::InitializeLibrary()
{
	if (lastError != STATE_OK) {
		return lastError;
	}

	hLib = LibraryLoader::LoadDll("mpusbapi.dll");
	logger->PushMessage(LibraryLoader::GetMoudlePath(hLib));

	if (!hLib) {
		return lastError = NO_DLL;
	}

	MPUSBGetDLLVersion = (DWORD(*)(void))GetAndConfirmFuntion(hLib, "_MPUSBGetDLLVersion");
	MPUSBGetDeviceCount = (DWORD(*)(PCHAR))GetAndConfirmFuntion(hLib, "_MPUSBGetDeviceCount");
	MPUSBOpen = (HANDLE(*)(DWORD, PCHAR, PCHAR, DWORD, DWORD))GetAndConfirmFuntion(hLib, "_MPUSBOpen");
	MPUSBWrite = (DWORD(*)(HANDLE, PVOID, DWORD, PDWORD, DWORD))GetAndConfirmFuntion(hLib, "_MPUSBWrite");
	MPUSBRead = (DWORD(*)(HANDLE, PVOID, DWORD, PDWORD, DWORD))GetAndConfirmFuntion(hLib, "_MPUSBRead");
	MPUSBReadInt = (DWORD(*)(HANDLE, PVOID, DWORD, PDWORD, DWORD))GetAndConfirmFuntion(hLib, "_MPUSBReadInt");
	MPUSBClose = (BOOL(*)(HANDLE))GetAndConfirmFuntion(hLib, "_MPUSBClose");

	return lastError;
}

int USBDevice::InitializeDevice()
{
	if (lastError != STATE_OK) {
		return lastError;
	}

	//(DS) Get count of devices with specified vid and pid.
	DWORD count = MPUSBGetDeviceCount(deviceVidPid);
	
	if (count <= 0) {
		return lastError = NO_DEVICE_CONNECTED;
	}
	return STATE_OK;
}

int USBDevice::Connect()
{
	EnterCriticalSection(&baseOperarionsCS);

	int result = InitializeLibrary();
	if (result == STATE_OK) {
		result = InitializeDevice();
	}

	LeaveCriticalSection(&baseOperarionsCS);
	return result;
}

//отправка данных и иожидание подтверждения
DWORD USBDevice::SendReceive(PBYTE SendData, DWORD SendLength, PBYTE ReceiveData, DWORD ExpectedReceiveLength, UINT SendDelay, UINT ReceiveDelay)
{
	EnterCriticalSection(&baseOperarionsCS);

	if (!IsSessionOpen()) {
		lastError = NO_SESSION;
		LeaveCriticalSection(&baseOperarionsCS);
		return NO_SESSION;
	}
		
	if (lastError != STATE_OK) {
		LeaveCriticalSection(&baseOperarionsCS);
		return lastError;
	}

	//указатели для получения фактической длины (в байтах) преданых даных
	PDWORD SentDataLength = new DWORD;
	PDWORD ReceiveLength = new DWORD;

	//Проверка статуса соединений, отправить если нет ошибок
	if((outPipe != INVALID_HANDLE_VALUE) && (inPipe != INVALID_HANDLE_VALUE))
	{
		//отправка даных, если удачно - попытка приема
		if(MPUSBWrite(outPipe,SendData,SendLength,SentDataLength,SendDelay) != 0)
		{
			//попытка приема даных
			if(MPUSBRead(inPipe, ReceiveData, ExpectedReceiveLength, ReceiveLength, ReceiveDelay) != 0)
			{
				//если фактическмй размер принятых даных равен ожидаемому вернуть код ошибки 0 
				if(*ReceiveLength == ExpectedReceiveLength) {
					LeaveCriticalSection(&baseOperarionsCS);
					return STATE_OK;
				}
				//если фактическмй размер принятых даных меньше ожидаемого веруть 1
				if(*ReceiveLength < ExpectedReceiveLength) {
					lastError = NOT_FULL_DATA;
					LeaveCriticalSection(&baseOperarionsCS);
					return NOT_FULL_DATA;
				}
			}
		}

		// Error in MPUSBWrite or MPUSBRead
		if (GetLastError() == ERROR_INVALID_HANDLE) {
			CloseSession();
			lastError = INVALID_PIPE_HANDLE;
		}
		else {
			lastError = CALL_GET_LAST_ERROR;
		}
	}
	
	LeaveCriticalSection(&baseOperarionsCS);

	return lastError;
}

int USBDevice::OpenSession()
{
	int result = STATE_OK;
	EnterCriticalSection(&baseOperarionsCS);

	if (!IsSessionOpen())
	{
		outPipe = MPUSBOpen(0, deviceVidPid, outPipeName, MP_WRITE, 0);
		inPipe = MPUSBOpen(0, deviceVidPid, inPipeName, MP_READ, 0);

		if ((outPipe != INVALID_HANDLE_VALUE) && (inPipe != INVALID_HANDLE_VALUE)) {
			isSessionOpen = true;
		} else {
			result = lastError = INVALID_PIPE_HANDLE;
		}
	}
	else {
		result = lastError = SESSION_ALREADY_OPEN;
	}

	LeaveCriticalSection(&baseOperarionsCS);
	return result;
}

void USBDevice::CloseSession()
{
	EnterCriticalSection(&baseOperarionsCS);
	if (IsSessionOpen())
	{
		//Close all connections
		MPUSBClose(outPipe);
		MPUSBClose(inPipe);
		inPipe = INVALID_HANDLE_VALUE;
		outPipe = INVALID_HANDLE_VALUE;
		isSessionOpen = false;
	}
	LeaveCriticalSection(&baseOperarionsCS);
}

int USBDevice::IsSessionOpen() {
	return isSessionOpen;
}

int USBDevice::GetLastError()
{
	EnterCriticalSection(&baseOperarionsCS);
	int result = lastError;
	LeaveCriticalSection(&baseOperarionsCS);
	return result;
}

void USBDevice::SetLastError(int error)
{
	EnterCriticalSection(&baseOperarionsCS);
	lastError = error;
	LeaveCriticalSection(&baseOperarionsCS);
}

int USBDevice::SendRequest(DeviceRequest* request)
{
	if (!SetRequestIfEmpty(request)) {
		SetLastError(ASYNC_SESSION_RUNNING);
		return ASYNC_SESSION_RUNNING;
	}

	int result = OpenSession();
	if (result != STATE_OK) {
		return result;
	}

	result = SendReceive(request->GetSendBuffer(), request->GetSendSize(), 
						request->GetReceiveBuffer(), request->GetExpectedSize(), 1000, 1000);

	CloseSession();
	SetRequest(NULL);
	return result;
}



void ReqestThread(void* pParams)
{
	USBDevice * device = (USBDevice*)pParams;
	DeviceRequest* request = device->GetRequest();
	int result;

	do {
		if (request != NULL) {
			result = device->SendReceive(request->GetSendBuffer(), request->GetSendSize(),
				request->GetReceiveBuffer(), request->GetExpectedSize(), 1000, 1000);
			if (result != STATE_OK) {
				break;
			}
		} else {
			break;
		}
	} while (request->OnResponse());

	device->CloseSession();
	device->SetRequest(NULL);
}

int USBDevice::SendRequestAsync(DeviceRequest* request)
{
	if (!SetRequestIfEmpty(request)) {
		SetLastError(ASYNC_SESSION_RUNNING);
		return ASYNC_SESSION_RUNNING;
	}

	int result = OpenSession();
	if (result != STATE_OK) {
		return result;
	}

	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReqestThread, this, 0, NULL);// &uThrID);

	return STATE_OK;
}

int USBDevice::IsRequestEnd() {
	return GetRequest() == NULL;
}

void USBDevice::SetRequest(DeviceRequest* request)
{
	EnterCriticalSection(&recvStateCS);
	currentRequest = request;
	LeaveCriticalSection(&recvStateCS);
}

DeviceRequest* USBDevice::GetRequest()
{
	EnterCriticalSection(&recvStateCS);
	DeviceRequest* result = currentRequest;
	LeaveCriticalSection(&recvStateCS);
	return result;
}

bool USBDevice::SetRequestIfEmpty(DeviceRequest* request)
{
	bool result;
	EnterCriticalSection(&recvStateCS);
	if (currentRequest == NULL) {
		currentRequest = request;
		result = true;
	}
	else {
		result = false;
	}
	LeaveCriticalSection(&recvStateCS);
	return result;
}