#ifndef LOGGER_H
#define LOGGER_H

#include <DsProjectDefines.h>

class Logger
{
	char * currentMessage;

public:
	Logger();
	~Logger();

	bool HasMessages();
	TCHAR* GetLastMessage();
	void PushMessage(TCHAR* message);
	void PushError(int err);
};

#endif // LOGGER_H