#ifndef DRIVER_LIBRARY_MAIN_H
#define DRIVER_LIBRARY_MAIN_H

struct MUMBuffer;

extern "C" __declspec(dllexport) int Start();
extern "C" __declspec(dllexport) void TurnLeft(); 
extern "C" __declspec(dllexport) void TurnRight();
extern "C" __declspec(dllexport) void Stop();

extern "C" __declspec(dllexport) int GetMajorVersion();
extern "C" __declspec(dllexport) int GetMinorVersion();

extern "C" __declspec(dllexport) int IsVersionConfirmed();


extern "C" __declspec(dllexport) bool IsReseived();
extern "C" __declspec(dllexport) int SetBuffer(MUMBuffer * buffer);

#endif