#ifndef LIBRARY_LOADER_H
#define LIBRARY_LOADER_H

#include <windows.h>

class LibraryLoader {
public:
	static HMODULE LoadDll(char* name);
	static void FreeDll(HMODULE hlib);
	static char* GetMoudlePath(HMODULE hlib);
};

#endif // LIBRARY_LOADER_H

