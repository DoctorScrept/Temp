#include<conio.h>
#include<stdio.h>
//Prject properties -> Configurarion (...) -> C/C++ -> General -> Iclude path
#include "core/USBDevice.h" 
#include "step/StepUSBDevice.h" 
#include "core/DeviceRequest.h" 

void main()
{
	StepUSBDevice d;
	if (d.Connect() != 0) {
		printf("error in Connect");
		_getch();
		return;
	}

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
			DeviceRequest r;
			d.SendRequest(&r);
			unsigned char * b = r.GetReceiveBuffer();
			printf("firmware v%d.%d", b[1], b[2]);
		}
			break;
		}
		command = _getch();
	} while (command != '0');
}