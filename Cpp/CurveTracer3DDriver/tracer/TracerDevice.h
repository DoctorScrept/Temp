#ifndef TRACER_DEVICE_H
#define TRACER_DEVICE_H

#include "core/USBDevice.h"
#include "base/VersionedUSBDevice.h"
#include "tracer/SignalRequest.h"
#include "tracer/MeasureRequest.h"

#define MAJOR_VERSION 2
#define MINOR_VERSION 1

class TracerDevice : public VersionedUSBDevice
{
public:
	SignalRequest* signalRequest;
	MeasureRequest* measureRequest;

	TracerDevice();
	virtual ~TracerDevice();

	int Connect();

	virtual int IsVersionConfirmed();
};

#endif // TRACER_DEVICE_H