#include "WinApi.h"

void lol(void* pFunction, bool bLoop)
{
	void* pPosition;

	BYTE OPCodesLoop[17] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC, 0x31, 0xC0, 0xE8, 00, 00, 00, 00, 0xEB, 0xF9, 0x31, 0xC0, 0xC3 };
	BYTE OPCodesNoLoop[15] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC, 0x31, 0xC0, 0xE8, 00, 00, 00, 00, 0x31, 0xC0, 0xC3 };

	pPosition = VirtualAlloc(NULL, 50, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (bLoop)
		MoveMemory(pPosition, OPCodesLoop, 17);
	else
		MoveMemory(pPosition, OPCodesNoLoop, 15);

	*(DWORD*)((DWORD(pPosition) + 8)) = DWORD(pFunction) - DWORD(pPosition) - 0xC;

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pPosition, 0, 0, 0);
}

bool rsCompString(char* string) {
	BYTE dwChar[0xF] = { 0 };

	for (int i = 0; i < strlen(string); i++)
		dwChar[i] = read(0x037B2838 + i, 1);

	if (!strcmp(string, (char*)dwChar)) {
		write(0x037B2838, (BYTE*)"\x00", strlen(string));
		return TRUE;
	}

	return FALSE;
}

void write(DWORD dst, void* value, int len) {
	DWORD old;

	VirtualProtect((void*)dst, len, PAGE_EXECUTE_READWRITE, &old);
	MoveMemory((void*)dst, value, len);
	VirtualProtect((void*)dst, len, old, &old);
}

int read(DWORD src, int len) {
	DWORD old;
	int dst = NULL;

	VirtualProtect((void*)src, len, PAGE_EXECUTE_READWRITE, &old);
	MoveMemory(&dst, (void*)src, len);
	VirtualProtect((void*)src, len, old, &old);

	return (DWORD)dst;
}

std::string readString(DWORD src, int len) {
	DWORD oldProtection;
	char buffer[256];

	VirtualProtect((LPVOID)src, len, PAGE_EXECUTE_READWRITE, &oldProtection);
	MoveMemory(buffer, (LPVOID)src, len);
	VirtualProtect((LPVOID)src, len, oldProtection, &oldProtection);

	return std::string(buffer);
}