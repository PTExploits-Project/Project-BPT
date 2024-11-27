#include "asmm.h"
#include <Windows.h>
#include <iostream>

__declspec(naked) void travaHp(short iVar) {
	__asm {
		cmp byte ptr ds:[bNoHp], 0
		jne ctravaHp
		mov eax, 0x4E1C0C
		jmp eax

		ctravaHp:
		ret
	}
}

int jmpSetMana = 0x004E1C69;

__declspec(naked) void travaMp() {
	__asm {
		cmp byte ptr ds : [bNoMp] , 0
		jne ctravaMp
		jmp[jmpSetMana]

		ctravaMp :
		mov eax, dword ptr ds:[0x033DED30]
		mov cx, [esp + 04]
		cmp cx, [eax + 0x128]
		jl cJmp
		jmp[jmpSetMana]

		cJmp:
		retn
	}
}

int jmpSetStm = 0x004E1CAE;

__declspec(naked) void travaStm() {
	__asm {
		cmp byte ptr ds:[bNoStm], 0
		jne ctravaStm
		jmp[jmpSetStm]

		ctravaStm :
		mov eax, dword ptr ds:[0x033DED30]
		mov cx, [esp + 04]
		cmp cx, [eax + 0x12c]
		jl cJmp
		jmp[jmpSetStm]

		cJmp :
		retn
	}
}

int iParamSend = 0x006288D9;
int iReturn = 0x00407423;

__declspec(naked) void fdm_SendTransDamage() {
	__asm {
		push ebp
		mov ebp, esp
		sub esp, 0x64

		mov eax, dword ptr ds: [0x00880FB4]
		xor eax, dword ptr ds: [ebp + 0x04]
		push ebx
		mov dword ptr ds: [ebp - 0x04], eax
		mov eax, dword ptr ds: [0x00AFE540]
		mov dword ptr ds: [0x008BEF60], eax //move mouse
		mov dword ptr ds: [0x008BEF64], eax //move char
		push esi
		push edi
		//push eax
		//call [iParamSend]
		//pop ecx

		jmp [iReturn]
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

			GetFullPathName((LPCSTR)log, MAX_PATH, (LPSTR)szPath, 0);

			f = fopen(szPath, "a");
			fprintf(f, "%d:%d:%d : > **** Starting Service **** - ( %.2d/%.2d )\n\n", sLocalTime.wHour, sLocalTime.wMinute, sLocalTime.wSecond, sLocalTime.wMonth, sLocalTime.wDay);
		}
	}
}