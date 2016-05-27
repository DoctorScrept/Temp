#include "VersionedUSBDevice.h"

VersionedUSBDevice::VersionedUSBDevice(char* vidPid, char* inName, char* outName) : USBDevice(vidPid, inName, outName)
{
}

VersionedUSBDevice::~VersionedUSBDevice()
{
}
