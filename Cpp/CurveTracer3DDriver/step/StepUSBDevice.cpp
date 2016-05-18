#include "StepUSBDevice.h"

//(DS) Commands
#define READ_VERSION 0x00
#define MOTOR_ON 0x01

//(DS) Direction
#define	STOP 0x00
#define TURN_LEFT 0x01
#define TURN_RIGHT 0x02

StepUSBDevice::StepUSBDevice() : USBDevice("vid_04d8&pid_000c", "\\MCHP_EP1", "\\MCHP_EP1")
{
	send_bufP = new BYTE[10];
	receive_buf = new BYTE[10];

	majorVersion = 0;
	minorVersion = 0;
	isReady = 0;
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
		return 1;
	}

	//заполнение полей даных для запроса
	send_bufP[0] = READ_VERSION;    // command
	send_bufP[1] = 2;               // Expected length of the result
	RecvLength = 4;

	//отправка комнады и прием ответа
	if (SendReceive(send_bufP, 2, receive_buf, RecvLength, 1000, 1000) == 0)
	{
		//это сообщение с номером версии?
		if (receive_buf[0] == READ_VERSION)
		{
			majorVersion = receive_buf[1];
			minorVersion = receive_buf[2];

			//printf("firmware v%d.%d", receive_buf[1], receive_buf[2]);

			//если версия = 2.1
			if ((receive_buf[1] == 2) && (receive_buf[2] == 1))
			{
				isReady = 1;
				//printf("\nfirmware version confirmed\n");
			}
			//else printf("\nfirmware version error\n");
		}
	}

	CloseSession();
	return STATE_OK;
}

void StepUSBDevice::SetDir(BYTE direction)
{
	if (!isReady) {
		return;
	}

	if (OpenSession() != 0) {
		return;
	}

	// Сообщение для отправки на контроллер : <MOTOR_ON><0x00><DIRECTION> = 3 байта
	// Ответ контроллера : <MOTOR_ON> =  1 байт (подтверждение получения даных)
	send_bufP[0] = MOTOR_ON;        // Command
	send_bufP[1] = 0;               // Expected length of the result
	send_bufP[2] = direction;		// Motor direction
	RecvLength = 1;                 // Length of the Packet

	//отправка комнады и прием ответа
	if (SendReceive(send_bufP, 3, receive_buf, RecvLength, 1000, 1000) == 0)
	{
		//"отчет о доставке"
		if (receive_buf[0] != MOTOR_ON) {
			lastError = COMMUNICATION_ERROR;
		}
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

int StepUSBDevice::IsVersionConfirmed() {
	return isReady;
}

int StepUSBDevice::GetMajorVersion() {
	return majorVersion;
}

int StepUSBDevice::GetMinorVersion() {
	return minorVersion;
}