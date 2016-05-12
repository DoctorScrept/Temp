#include "step/StepUSBDevice.h"
#include <stdio.h> 

#define ShowMessage(m) printf(m)

#define MOTOR_ON 0x01

// онстанты команд
#define READ_VERSION 0x00
#define	STOP 0x00
#define TURN_LEFT 0x01
#define TURN_RIGHT 0x02

StepUSBDevice::StepUSBDevice() : USBDevice("vid_04d8&pid_000c", "\\MCHP_EP1", "\\MCHP_EP1")
{
	send_bufP = new BYTE[10];
	receive_buf = new BYTE[10];
}

StepUSBDevice::~StepUSBDevice()
{
	delete[] send_bufP;
	delete[] receive_buf;
}

int StepUSBDevice::Connect()
{
	int result = USBDevice::Connect();
	if (result != 0) {
		return result;
	}

	if (OpenSession() != 0) {
		logger->PushMessage("Status : USB Error()");
		return 1;
	}

	//заполнение полей даных дл€ запроса
	send_bufP[0] = READ_VERSION;    // command
	send_bufP[1] = 2;               // Expected length of the result
	RecvLength = 4;

	//отправка комнады и прием ответа
	if (SendReceive(send_bufP, 2, receive_buf, RecvLength, 1000, 1000) == 0)
	{
		//это сообщение с номером версии?
		if (receive_buf[0] == READ_VERSION)
		{
			//вывести версию
			printf("firmware v%d.%d", receive_buf[1], receive_buf[2]);

			//если верси€ = 2.1
			if ((receive_buf[1] == 2) && (receive_buf[2] == 1))
			{
				ShowMessage("\nfirmware version confirmed\n");
			}
			else ShowMessage("\nfirmware version error\n");
		}
	}

	CloseSession();

	return 0;
}

void StepUSBDevice::SetDir(BYTE direction)
{
	if (OpenSession() != 0) {
		logger->PushMessage("Status : USB Error()");
		return;
	}

	// —ообщение дл€ отправки на контроллер : <MOTOR_ON><0x00><DIRECTION> = 3 байта
	// ќтвет контроллера : <MOTOR_ON> =  1 байт (подтверждение получени€ даных)
	send_bufP[0] = MOTOR_ON;        // Command
	send_bufP[1] = 0;               // Expected length of the result
	send_bufP[2] = direction;		// Motor direction
	RecvLength = 1;                 // Length of the Packet

	//отправка комнады и прием ответа
	if (SendReceive(send_bufP, 3, receive_buf, RecvLength, 1000, 1000) == 0)
	{
		//"отчет о доставке"
		if (receive_buf[0] != MOTOR_ON)
			logger->PushMessage("Status : Communication Error!");
	}

	CloseSession();
}

void StepUSBDevice::TurnLeft() {
	SetDir(TURN_LEFT);
}

void StepUSBDevice::TurnRight() {
	SetDir(TURN_RIGHT);
}

void StepUSBDevice::Stop() {
	SetDir(STOP);
}