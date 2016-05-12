#include "LibraryLoader.h"

HMODULE LibraryLoader::LoadDll(char* name) {
	//strcpy
	//hLibrary = LoadLibrary("E:\\Diplom\\driverProj\\core\\m1pusbapi.dll");
	return LoadLibrary(name);
}

void LibraryLoader::FreeDll(HMODULE hlib) {
	FreeLibrary(hlib);
}

char* LibraryLoader::GetMoudlePath(HMODULE hlib) {
	char * path = new char[256];
	GetModuleFileName(hlib, path, 255);
	return path;
}