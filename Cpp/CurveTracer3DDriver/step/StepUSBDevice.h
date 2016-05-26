#include "core/USBDevice.h"
#include "tracer/GetVersionRequest.h"


class StepUSBDevice : public USBDevice
{
	PBYTE send_bufP;
	PBYTE receive_buf;
	char isReady;

	//������ ������ ����� ��� ������ (����� ������������=)
	int RecvLength;


public:
	GetVersionRequest * getVersionRequest;

	StepUSBDevice();
	~StepUSBDevice();

	void SetDir(BYTE direction);

	int Connect();

	void TurnLeft();
	void TurnRight();
	void Stop();

	int GetMajorVersion();
	int GetMinorVersion();

	int IsVersionConfirmed();
};