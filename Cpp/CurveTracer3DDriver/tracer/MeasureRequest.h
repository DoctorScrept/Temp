#ifndef MEASURE_REQUEST_H
#define MEASURE_REQUEST_H

#include "core/DeviceRequest.h"
#include "tracer/SurfaceBuffer.h"

#define MEASURE 0x02

class MeasureRequest : public DeviceRequest
{
	int tempCounter;
public:
	SurfaceBuffer* buffer;

	MeasureRequest();
	~MeasureRequest();

	virtual int GetExpectedSize();
	virtual int GetSendSize();
	virtual bool MeasureRequest::OnAsyncResponse();

	int GetPercentComplete();
	void SetBuffer(SurfaceBuffer* buffer);
private:
	void s(float value);
};

#endif // MEASURE_REQUEST_H