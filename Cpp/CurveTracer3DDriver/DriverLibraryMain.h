#ifndef DRIVER_LIBRARY_MAIN_H
#define DRIVER_LIBRARY_MAIN_H

extern "C" __declspec(dllexport) void Start();
extern "C" __declspec(dllexport) void TrurnLeft(); 
extern "C" __declspec(dllexport) void TrurnRight();
extern "C" __declspec(dllexport) void Stop();

#endif