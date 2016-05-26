#include<conio.h>
#include<stdio.h>
//Prject properties -> Configurarion (...) -> C/C++ -> General -> Iclude path

#include "step/StepUSBDevice.h" 
#include "tracer/TracerDevice.h"


bool ConnectDevice(USBDevice * device) {
	int result = device->Connect();
		if (result != 0) {
		printf("error in Connect (%d)", result);
		_getch();
		return false;
	}
	return true;
}

void TestStep()
{
	StepUSBDevice d;
	if (!ConnectDevice(&d)) {
		return;
	}
	printf("firmware v%d.%d", d.GetMajorVersion(), d.GetMinorVersion());

	char command = _getch();
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
				d.SendRequest(d.getVersionRequest);
				unsigned char * b = d.getVersionRequest->GetReceiveBuffer();
				printf("\nfirmware v%d.%d", b[1], b[2]);
			}
			break;
		case '5':
			{
				d.SendRequestAsync(d.getVersionRequest);
				while (!d.IsRequestEnd()) {
					printf("-");
				}
				unsigned char * b = d.getVersionRequest->GetReceiveBuffer();
				printf("\nfirmware v%d.%d ASYNC", b[1], b[2]);
			}
			break;
		}
		command = _getch();
	} while (command != '0');
}



void pr(unsigned char * b, int id) {
	float f = ((float*)b)[id];
	printf("\n%f", f);
}


void TestTracer()
{
	TracerDevice d;
	if (!ConnectDevice(&d)) {
		return;
	}
	printf("firmware v%d.%d", d.getVersionRequest->GetMajorVersion(), d.getVersionRequest->GetMinorVersion());

	char command = _getch();
	do
	{
		switch (command) {
		case '1':
			break;
		case '2':
			break;
		case '3':
			break;

		case '4':
		{
			d.SendRequest(d.measureRequest);
			unsigned char * b = d.measureRequest->GetReceiveBuffer();
			pr(b, 0);
			pr(b, 1);
		}
		break;

		case '5':
			break;
		}
		command = _getch();
	} while (command != '0');
}


void main()
{
	TestTracer();
	//TestStep();
}