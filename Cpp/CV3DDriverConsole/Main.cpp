#include<conio.h>
#include<stdio.h>
//Prject properties -> Configurarion (...) -> C/C++ -> General -> Iclude path

#include "step/StepUSBDevice.h" 
#include "tracer/signal/SignalRequest.h"
#include "tracer/measurer/StartMeasure.h"

void pr(unsigned char * b, int id) {
	float f = ((float*)b)[id];
	printf("\n%f", f);
}

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
			StartMeasure r;
			d.SendRequest(&r);
			unsigned char * b = r.GetReceiveBuffer();
			pr(b, 0);
			pr(b, 1);
			/*float f = ((float*)b)[0];
			printf("\n%f", f);*/
			//printf("firmware v%d.%d", b[1], b[2]);
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