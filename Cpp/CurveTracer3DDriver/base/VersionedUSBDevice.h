#ifndef VERSIONED_USB_DEVICE_H
#define VERSIONED_USB_DEVICE_H

#include "core/USBDevice.h"
#include "tracer/GetVersionRequest.h"

class VersionedUSBDevice : public USBDevice
{
public:
	GetVersionRequest * getVersionRequest;

	VersionedUSBDevice(char* vidPid, char* inName, char* outName);
	virtual ~VersionedUSBDevice();

	int GetMajorVersion();
	int GetMinorVersion();
	virtual int IsVersionConfirmed();
};

#endif // VERSIONED_USB_DEVICE_H