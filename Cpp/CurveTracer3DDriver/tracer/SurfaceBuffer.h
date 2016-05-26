#ifndef SURFACE_BUFFER_H
#define SURFACE_BUFFER_H

#define BUFFER_DATA_SIZE 16


struct SurfaceBuffer
{
public:
	float inData[BUFFER_DATA_SIZE];

	SurfaceBuffer();
	~SurfaceBuffer();
};

#endif // SURFACE_BUFFER_H