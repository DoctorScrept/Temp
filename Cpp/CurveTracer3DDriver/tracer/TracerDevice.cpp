#include "TracerDevice.h"

TracerDevice::TracerDevice() : USBDevice("vid_04d8&pid_000c", "\\MCHP_EP1", "\\MCHP_EP1")
{
}

TracerDevice::~TracerDevice()
{
}
