#ifndef TRACER_DEVICE_H
#define TRACER_DEVICE_H

#include "core/USBDevice.h"

class TracerDevice : public USBDevice
{
public:
	TracerDevice();
	virtual ~TracerDevice();
};

#endif // TRACER_DEVICE_H