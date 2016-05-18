#ifndef LOGGER_H
#define LOGGER_H

#include "core/USBDevice.h"
#include <windows.h>

class USBDevice;

class Logger
{
	char * currentMessage;
	USBDevice * device;

public:
	Logger(USBDevice * device);
	Logger();
	~Logger();

	TCHAR* GetLastMessage();
	void PushMessage(TCHAR* message);
	TCHAR* GetLastErrorText();
};

#endif // LOGGER_H