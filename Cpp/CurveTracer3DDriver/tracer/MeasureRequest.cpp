#include "MeasureRequest.h"


MeasureRequest::MeasureRequest(void) : DeviceRequest(10, 10)
{
	sendBuffer[0] = MEASURE;
}

MeasureRequest::~MeasureRequest(void)
{
}

int MeasureRequest::GetExpectedSize()
{
	return 8;
}

int MeasureRequest::GetSendSize()
{
	return 1;
}

bool MeasureRequest::OnAsyncResponse() {

	//return false;
	float* f = ((float*)receiveBuffer);
	s(f[0]);
	s(f[1]);

	//tempCounter += 2;
	return tempCounter < 16;
}

int MeasureRequest::GetPercentComplete() {
	if (tempCounter < 16) {
		float f = tempCounter;
		f = f * 100 / 16;
		return f;
	}
	return 100;
}

void MeasureRequest::s(float value) {
	buffer->inData[tempCounter] = value;
	tempCounter++;
}

void MeasureRequest::SetBuffer(SurfaceBuffer* buffer)
{
	this->buffer = buffer;
	tempCounter = 0;


	//s(1); s(1); s(0); s(0);
	//s(0); s(1); s(1); s(2);
	//s(0); s(1); s(0); s(2);
	//s(0); s(1); s(0); s(0);
}