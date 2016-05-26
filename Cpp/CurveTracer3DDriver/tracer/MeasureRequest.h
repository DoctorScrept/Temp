#ifndef MEASURE_REQUEST_H
#define MEASURE_REQUEST_H

#include "core/DeviceRequest.h"

#define MEASURE 0x02

class MeasureRequest : public DeviceRequest
{
public:
	MeasureRequest();
	~MeasureRequest();

	virtual int GetExpectedSize();
	virtual int GetSendSize();
};

#endif // MEASURE_REQUEST_H