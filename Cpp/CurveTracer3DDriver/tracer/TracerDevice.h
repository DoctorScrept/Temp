#ifndef TRACER_DEVICE_H
#define TRACER_DEVICE_H

#include "core/USBDevice.h"
#include "tracer/GetVersionRequest.h"
#include "tracer/SignalRequest.h"
#include "tracer/MeasureRequest.h"

#define MAJOR_VERSION 2
#define MINOR_VERSION 1

class TracerDevice : public USBDevice
{
public:
	GetVersionRequest * getVersionRequest;
	SignalRequest* signalRequest;
	MeasureRequest* measureRequest;

	TracerDevice();
	virtual ~TracerDevice();

	int Connect();

	int IsVersionConfirmed();
};

#endif // TRACER_DEVICE_H