#include "core/Logger.h"
#include <stdio.h>

//TODO remove printf
Logger::Logger(USBDevice * device)
{
	this->device = device;
	currentMessage = NULL;
}

Logger::Logger()
{
	device = NULL;
	currentMessage = NULL;
}

Logger::~Logger()
{
}

TCHAR* Logger::GetLastMessage() {
	TCHAR * mes = currentMessage;
	currentMessage = NULL;
	return mes;
}

void Logger::PushMessage(TCHAR* message) {
	printf(message);
	printf("\n");
}

TCHAR* Logger::GetLastErrorText()
{
	switch (device->GetLastError()) {
	case STATE_OK:
		return "";
		break;
	case NO_DLL:
		return "mpusbapi.dll not found";
		break;
	case NO_FUNCTION:
		return "";
		break;
	case NO_DEVICE_CONNECTED:
		return "";
		break;
	case NOT_FULL_DATA:
		return "";
		break;
	case INVALID_PIPE_HANDLE:
		return "Status : USB Error()";
		break;
	case CALL_GET_LAST_ERROR:
		return "";
		break;
	case SESSION_ALREADY_OPEN:
		return "Status : USB Error()";
		break;
	}
	return NULL;
}