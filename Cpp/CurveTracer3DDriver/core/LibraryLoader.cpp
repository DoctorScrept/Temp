#include "LibraryLoader.h"

HMODULE LibraryLoader::LoadDll(TCHAR* name) {
	//strcpy
	//hLibrary = LoadLibrary("E:\\Diplom\\driverProj\\core\\m1pusbapi.dll");
	return LoadLibrary(name);
}

void LibraryLoader::FreeDll(HMODULE hlib) {
	FreeLibrary(hlib);
}

TCHAR* LibraryLoader::GetMoudlePath(HMODULE hlib) {
	TCHAR * path = new TCHAR[256];
	GetModuleFileName(hlib, path, 255);
	return path;
}