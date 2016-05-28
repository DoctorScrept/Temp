#include "VersionedUSBDevice.h"

VersionedUSBDevice::VersionedUSBDevice(char* vidPid, char* inName, char* outName) : USBDevice(vidPid, inName, outName)
{
	getVersionRequest = new GetVersionRequest();
}

VersionedUSBDevice::~VersionedUSBDevice()
{
	delete getVersionRequest;
}

int VersionedUSBDevice::IsVersionConfirmed() {
	return 0;
}

int VersionedUSBDevice::GetMajorVersion() {
	return getVersionRequest->GetMajorVersion();
}

int VersionedUSBDevice::GetMinorVersion() {
	return getVersionRequest->GetMinorVersion();
}