#ifndef VERSIONED_USB_DEVICE_H
#define VERSIONED_USB_DEVICE_H

#include "core/USBDevice.h"
#include "tracer/GetVersionRequest.h"

class VersionedUSBDevice : public USBDevice
{
public:
	VersionedUSBDevice(char* vidPid, char* inName, char* outName);
	virtual ~VersionedUSBDevice();
};

#endif // VERSIONED_USB_DEVICE_H