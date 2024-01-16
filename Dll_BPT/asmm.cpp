#include "asmm.h"
#include <Windows.h>
#include <iostream>

__declspec(naked) void travaHp(short iVar) {
	__asm {
		cmp byte ptr ds:[bNoHp], 0
		jne trava
		mov eax, 0x4E1C0C
		jmp eax

		trava:
		ret
	}
}

int iParam = 0, iParam2 = 0, iParam3 = 0, iParam4 = 0;

__declspec(naked) void sendtwo() {
	__asm {
		mov eax, dword ptr ds: [ebp-4]

		cmp dword ptr ds: [eax], 0x30
		jg teste
		mov eax, dword ptr ds : [eax]
		mov dword ptr ds: [iParam], eax

		teste:
		mov eax, 0x45f591
		call eax
	}
}

//_CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

FILE* f;

void logx() {
	while (true) {
		Sleep(10);

		SYSTEMTIME sLocalTime;
		GetLocalTime(&sLocalTime);

		if (f != NULL) {

			if (iParam > 0) {
				if (fprintf(f, "%.2d:%.2d:%.2d : size: 0x%08X ", sLocalTime.wHour, sLocalTime.wMinute, sLocalTime.wSecond, iParam)) {

					fprintf(f, "\n");

					iParam = 0;
				}
			}
		}
		else {
			char szPath[MAX_PATH];
			char* log = (char*)"logs.txt";

			GetFullPathName((LPCWSTR)log, MAX_PATH, (LPWSTR)szPath, 0);

			f = fopen(szPath, "a");
			fprintf(f, "%d:%d:%d : > **** Starting Service **** - ( %.2d/%.2d )\n\n", sLocalTime.wHour, sLocalTime.wMinute, sLocalTime.wSecond, sLocalTime.wMonth, sLocalTime.wDay);
		}
	}
}