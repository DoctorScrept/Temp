#include "core/USBDevice.h"

class StepUSBDevice : public USBDevice
{
	PBYTE send_bufP;
	PBYTE receive_buf;

	//������ ������ ����� ��� ������ (����� ������������=)
	int RecvLength;

public:
	StepUSBDevice();
	~StepUSBDevice();

	void SetDir(BYTE direction);

	int Connect();

	void TurnLeft();
	void TurnRight();
	void Stop();

private:

};