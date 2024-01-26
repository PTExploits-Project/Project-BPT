#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>

void UnlinkModuleFromPEB(HMODULE hModule);
void lol(void* pFunction, bool bLoop);

bool rsCompString(char* string);

void write(DWORD dst, void* value, int len);
int read(DWORD src, int len);
std::string readString(DWORD src, int len);