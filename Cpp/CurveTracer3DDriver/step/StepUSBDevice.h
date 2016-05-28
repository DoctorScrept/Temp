#include "core/USBDevice.h"
#include "base/VersionedUSBDevice.h"


class StepUSBDevice : public VersionedUSBDevice
{
	PBYTE send_bufP;
	PBYTE receive_buf;
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

	virtual int IsVersionConfirmed();
};