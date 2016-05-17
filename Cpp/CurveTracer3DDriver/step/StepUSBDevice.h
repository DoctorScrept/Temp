#include "core/USBDevice.h"

class StepUSBDevice : public USBDevice
{
	PBYTE send_bufP;
	PBYTE receive_buf;

	int majorVersion;
	int minorVersion;
	char isReady;

	//–азмер масива даных жд€ приема (часто используетс€=)
	int RecvLength;

public:
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

private:

};