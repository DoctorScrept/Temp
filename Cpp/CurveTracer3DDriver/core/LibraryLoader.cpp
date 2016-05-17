#include "LibraryLoader.h"


HMODULE LibraryLoader::LoadDll(TCHAR* name) {
	HMODULE result = LoadLibrary(name);
	if (result == NULL) {
		TCHAR path[] = "..\\..\\CurveTracer3DDriver\\lib";
		TCHAR fullPath[100];
		strcpy(fullPath, path);
		strcpy(fullPath, name);
		result = LoadLibrary(fullPath);
	}
	return result;
}

void LibraryLoader::FreeDll(HMODULE hlib) {
	FreeLibrary(hlib);
}

TCHAR* LibraryLoader::GetMoudlePath(HMODULE hlib) {
	TCHAR * path = new TCHAR[256];
	GetModuleFileName(hlib, path, 255);
	return path;
}