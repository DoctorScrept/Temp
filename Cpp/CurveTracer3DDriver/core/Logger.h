#ifndef LOGGER_H
#define LOGGER_H

class Logger
{
	char * currentMessage;

public:
	Logger();
	~Logger();

	bool HasMessages();
	char* GetLastMessage();
	void PushMessage(char* message);
	void PushError(int err);
};

#endif // LOGGER_H