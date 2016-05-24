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
	int result = InitializeLibrary();
	if (result == STATE_OK) {
		return InitializeDevice();
	}
	return result;
}

//отправка данных и иожидание подтверждения
DWORD USBDevice::SendReceive(PBYTE SendData, DWORD SendLength, PBYTE ReceiveData, DWORD ExpectedReceiveLength, UINT SendDelay, UINT ReceiveDelay)
{
	if (lastError != STATE_OK) {
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
					return STATE_OK;
				}
				//если фактическмй размер принятых даных меньше ожидаемого веруть 1
				if(*ReceiveLength < ExpectedReceiveLength) {
					return lastError = NOT_FULL_DATA;
				}
			}
			//неудачный приеи, провереть код ошибки
			else CheckInvalidHandle();
		}
		//неудачная отправка, провереть код ошибки
		else CheckInvalidHandle();
	}
	//ошибка подключения
	return lastError;// = INVALID_PIPE_HANDLE;
}

void USBDevice::CheckInvalidHandle()
{
	if(GetLastError() == ERROR_INVALID_HANDLE) {
		CloseSession();
		lastError = INVALID_PIPE_HANDLE;
	}
	//вывести сообщене об ошибке
	else {
		lastError = CALL_GET_LAST_ERROR;
	}
}

int USBDevice::OpenSession()
{
	if (IsSessionOpen()) {
		return lastError = SESSION_ALREADY_OPEN;
	}
	//(DS) Opens connstions for send and receive
	outPipe = MPUSBOpen(0, deviceVidPid, outPipeName, MP_WRITE, 0);
	inPipe = MPUSBOpen(0, deviceVidPid, inPipeName, MP_READ, 0);
	
	if ((outPipe == INVALID_HANDLE_VALUE) || (inPipe == INVALID_HANDLE_VALUE)) {
		return lastError = INVALID_PIPE_HANDLE;
	}
	isSessionOpen = true;
	return STATE_OK;
}

void USBDevice::CloseSession()
{
	if (IsSessionOpen())
	{
		//Close all connections
		MPUSBClose(outPipe);
		MPUSBClose(inPipe);
		inPipe = INVALID_HANDLE_VALUE;
		outPipe = INVALID_HANDLE_VALUE;
		isSessionOpen = false;
	}
}

int USBDevice::SendRequest(DeviceRequest* request)
{
	if (currentRequest != NULL) {
		return lastError = ASYNC_SESSION_RUNNING;
	}

	int result = OpenSession();
	if (result != STATE_OK) {
		return result;
	}

	currentRequest = request;
	result = SendReceive(request->GetSendBuffer(), request->GetSendSize(), 
						request->GetReceiveBuffer(), request->GetExpectedSize(), 1000, 1000);

	CloseSession();
	currentRequest = NULL;
	return result;
}

int USBDevice::IsSessionOpen() {
	return isSessionOpen;
}

int USBDevice::GetLastError() {
	return lastError;
}

void ReqestThread(void* pParams)
{
	USBDevice * device = (USBDevice*)pParams;
	int result = device->OpenSession();
	if (result != STATE_OK) {
		return;
	}

	EnterCriticalSection(&device->recvStateCS);
	DeviceRequest* request = device->currentRequest;
	LeaveCriticalSection(&device->recvStateCS);

	do {
		if (request != NULL) {
			result = device->SendReceive(request->GetSendBuffer(), request->GetSendSize(),
				request->GetReceiveBuffer(), request->GetExpectedSize(), 1000, 1000);
		} else {
			break;
		}
	} while (request->OnResponse());

	device->CloseSession();

	device->FreeRequest();
	
	return;
}

int USBDevice::SendRequestAsync(DeviceRequest* request)
{
	if (currentRequest != NULL) {
		return ASYNC_SESSION_RUNNING;
	}
	currentRequest = request;
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReqestThread, this, 0, NULL);// &uThrID);
	//currentRequest = NULL;
	return STATE_OK;
}

int USBDevice::IsRequestEnd()
{
	EnterCriticalSection(&recvStateCS);
	bool result = currentRequest == NULL;
	LeaveCriticalSection(&recvStateCS);
	return result;
}

void USBDevice::FreeRequest()
{
	EnterCriticalSection(&recvStateCS);
	currentRequest = NULL;
	LeaveCriticalSection(&recvStateCS);
}