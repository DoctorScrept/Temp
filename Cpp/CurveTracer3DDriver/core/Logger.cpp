#include "core/Logger.h"
#include <stdio.h>

Logger::Logger()
{
	currentMessage = NULL;
}

Logger::~Logger()
{
}

bool Logger::HasMessages() {
	return currentMessage != NULL;
}

char* Logger::GetLastMessage() {
	char * mes = currentMessage;
	currentMessage = NULL;
	return mes;
}

void Logger::PushMessage(char* message) {
	//currentMessage = message;
	printf(message);
	printf("\n");
}

void Logger::PushError(int err) {
	printf("Error Code: %d\n", err);
}