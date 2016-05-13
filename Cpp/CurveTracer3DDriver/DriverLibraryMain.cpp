#include "DriverLibraryMain.h"
#include <conio.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void Start()
{
	printf("asdas");
}

extern "C" __declspec(dllexport) void TrurnLeft()
{
} 

extern "C" __declspec(dllexport) void TrurnRight()
{
}

extern "C" __declspec(dllexport) void Stop()
{
}