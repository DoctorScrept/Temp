#include "core/USBDevice.h" 
#include "lib/mpusbapi.h"

//error codes
//lastError to all
//check all loaded functions
USBDevice::USBDevice(char* vidPid, char* inNAme, char* outName)
{
	lastError = 0;
	deviceVidPid = vidPid;
	outPipeName = inNAme;
	inPipeName = outName;
	logger = new Logger();
}

USBDevice::~USBDevice()
{
	LibraryLoader::FreeDll(hLib);
	delete logger;
}

int USBDevice::InitializeLibrary()
{
	hLib = LibraryLoader::LoadDll("mpusbapi.dll");
	logger->PushMessage(LibraryLoader::GetMoudlePath(hLib));

	if (!hLib) {
		logger->PushMessage("mpusbapi.dll not found");
		return 1;
	}

	Confirm(MPUSBGetDLLVersion = (DWORD(*)(void))GetProcAddress(hLib, "_MPUSBGetDLLVersion"));
	


	MPUSBGetDeviceCount = (DWORD(*)(PCHAR))GetProcAddress(hLib, "_MPUSBGetDeviceCount");
	MPUSBOpen = (HANDLE(*)(DWORD, PCHAR, PCHAR, DWORD, DWORD))GetProcAddress(hLib, "_MPUSBOpen");
	MPUSBWrite = (DWORD(*)(HANDLE, PVOID, DWORD, PDWORD, DWORD))GetProcAddress(hLib, "_MPUSBWrite");
	MPUSBRead = (DWORD(*)(HANDLE, PVOID, DWORD, PDWORD, DWORD))GetProcAddress(hLib, "_MPUSBRead");
	MPUSBReadInt = (DWORD(*)(HANDLE, PVOID, DWORD, PDWORD, DWORD))GetProcAddress(hLib, "_MPUSBReadInt");
	MPUSBClose = (BOOL(*)(HANDLE))GetProcAddress(hLib, "_MPUSBClose");

	return lastError;
}

int USBDevice::InitializeDevice()
{
	if (MPUSBGetDeviceCount == NULL) {
		return 3;
	}
	//��������� ���������� ��������� � ������� ���������������, ���� ����� ���� �� ������� ����������
	DWORD count = MPUSBGetDeviceCount(deviceVidPid);
	
	if (count <= 0) {
		return 2;
	}
	return 0;
}

int USBDevice::Connect()
{
	int initializeLibraryResult = InitializeLibrary();
	if (initializeLibraryResult == 0) {
		return InitializeDevice();
	}
	return initializeLibraryResult;
}

//�������� ������ � ��������� �������������
DWORD USBDevice::SendReceive(PBYTE SendData, DWORD SendLength, PBYTE ReceiveData, DWORD ExpectedReceiveLength, UINT SendDelay, UINT ReceiveDelay)
{
	if (lastError != 0) {
		return lastError;
	}
	//��������� ��� ��������� ����������� ����� (� ������) �������� �����
	PDWORD SentDataLength = new DWORD;
	PDWORD ReceiveLength = new DWORD;

	//�������� ������� ����������, ��������� ���� ��� ������
	if((outPipe != INVALID_HANDLE_VALUE) && (inPipe != INVALID_HANDLE_VALUE))
	{
		//�������� �����, ���� ������ - ������� ������
		if(MPUSBWrite(outPipe,SendData,SendLength,SentDataLength,SendDelay) != 0)
		{
			//������� ������ �����
			if(MPUSBRead(inPipe, ReceiveData, ExpectedReceiveLength, ReceiveLength, ReceiveDelay) != 0)
			{
				//���� ����������� ������ �������� ����� ����� ���������� ������� ��� ������ 0 
				if(*ReceiveLength == ExpectedReceiveLength) return 0;
				//���� ����������� ������ �������� ����� ������ ���������� ������ 1
				if(*ReceiveLength < ExpectedReceiveLength) return 1;
			}
			//��������� �����, ��������� ��� ������
			else CheckInvalidHandle();
		}
		//��������� ��������, ��������� ��� ������
		else CheckInvalidHandle();
	}
	//������ �����������
	return 2;
}

void USBDevice::CheckInvalidHandle()
{
	if(GetLastError() == ERROR_INVALID_HANDLE) {
		CloseSession();
	}
	//������� �������� �� ������
	else logger->PushError(GetLastError());
}

int USBDevice::OpenSession() {
	//��������� ���������� �� ����� � ��������
	outPipe = MPUSBOpen(0, deviceVidPid, outPipeName, MP_WRITE, 0);
	inPipe = MPUSBOpen(0, deviceVidPid, inPipeName, MP_READ, 0);
	
	if ((outPipe == INVALID_HANDLE_VALUE) || (inPipe == INVALID_HANDLE_VALUE)){
		return 1;
	}
	return 0;
}

void USBDevice::CloseSession() {
	//�������� �����������
	MPUSBClose(outPipe);
	MPUSBClose(inPipe);
	inPipe = INVALID_HANDLE_VALUE;
	outPipe = INVALID_HANDLE_VALUE;
}

int USBDevice::SendRequest(DeviceRequest* request)
{
	if (OpenSession() != 0) {
		logger->PushMessage("Status : USB Error()");
		return 1;
	}

	//�������� ������� � ����� ������
	int result = SendReceive(request->GetSendBuffer(), request->GetSendSize(), 
							request->GetReceiveBuffer(), request->GetExpectedSize(), 1000, 1000);

	CloseSession();
	return result;
}