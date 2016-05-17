#ifndef START_MEASURE_H
#define START_MEASURE_H

#include "core/DeviceRequest.h"

class StartMeasure : public DeviceRequest
{
public:
	StartMeasure(void);
	~StartMeasure(void);

	virtual int GetExpectedSize();
	virtual int GetSendSize();
};

#endif // START_MEASURE_H