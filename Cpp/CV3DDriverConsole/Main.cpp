#include<conio.h>
#include<stdio.h>
//Prject properties -> Configurarion (...) -> C/C++ -> General -> Iclude path

#include "step/StepUSBDevice.h" 
#include "tracer/signal/SignalRequest.h"

void main()
{
	StepUSBDevice d;
	if (d.Connect() != 0) {
		printf("error in Connect");
		_getch();
		return;
	}
	printf("firmware v%d.%d", d.GetMajorVersion(), d.GetMinorVersion());

	char command;
	//*
	command = _getch();
	do
	{
		switch (command) {
		case '1':
			d.TurnLeft(); 
			break;
		case '2':
			d.TurnRight();
			break;
		case '3':
			d.Stop();
			break;
		
		case '4':
		{
			SignalRequest r;
			d.SendRequest(&r);
			unsigned char * b = r.GetReceiveBuffer();
			printf("firmware v%d.%d", b[1], b[2]);
		}
			break;
		case '5':
			SignalRequest r;
			d.SendRequestAsync(&r);
			while (!d.IsRequestEnd()) {
				printf("-");
			}
			unsigned char * b = r.GetReceiveBuffer();
			printf("\nfirmware v%d.%d ASYNC", b[1], b[2]);
			break;
		}
		command = _getch();
	} while (command != '0');
}