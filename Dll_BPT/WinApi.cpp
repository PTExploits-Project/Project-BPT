#include "WinApi.h"

typedef struct _PEB_LDR_DATA {
	UINT8 _PADDING_[12];
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB {
#ifdef _WIN64
	UINT8 _PADDING_[24];
#else
	UINT8 _PADDING_[12];
#endif
	PEB_LDR_DATA* Ldr;
} PEB, * PPEB;

typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	VOID* DllBase;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _UNLINKED_MODULE
{
	HMODULE hModule;
	PLIST_ENTRY RealInLoadOrderLinks;
	PLIST_ENTRY RealInMemoryOrderLinks;
	PLIST_ENTRY RealInInitializationOrderLinks;
	PLDR_DATA_TABLE_ENTRY Entry;
} UNLINKED_MODULE;

#define UNLINK(x)					\
	(x).Flink->Blink = (x).Blink;	\
	(x).Blink->Flink = (x).Flink;

#define RELINK(x, real)			\
	(x).Flink->Blink = (real);	\
	(x).Blink->Flink = (real);	\
	(real)->Blink = (x).Blink;	\
	(real)->Flink = (x).Flink;

std::vector<UNLINKED_MODULE> UnlinkedModules;

struct FindModuleHandle
{
	HMODULE m_hModule;
	FindModuleHandle(HMODULE hModule) : m_hModule(hModule)
	{
	}
	bool operator() (UNLINKED_MODULE const& Module) const
	{
		return (Module.hModule == m_hModule);
	}
};

void UnlinkModuleFromPEB(HMODULE hModule)
{
	std::vector<UNLINKED_MODULE>::iterator it = std::find_if(UnlinkedModules.begin(), UnlinkedModules.end(), FindModuleHandle(hModule));
	if (it != UnlinkedModules.end())
	{
		//DBGOUT(TEXT("Module Already Unlinked!"));
		return;
	}

#ifdef _WIN64
	PPEB pPEB = (PPEB)__readgsqword(0x60);
#else
	PPEB pPEB = (PPEB)__readfsdword(0x30);
#endif

	PLIST_ENTRY CurrentEntry = pPEB->Ldr->InLoadOrderModuleList.Flink;
	PLDR_DATA_TABLE_ENTRY Current = NULL;

	while (CurrentEntry != &pPEB->Ldr->InLoadOrderModuleList && CurrentEntry != NULL)
	{
		Current = CONTAINING_RECORD(CurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (Current->DllBase == hModule)
		{
			UNLINKED_MODULE CurrentModule = { 0 };
			CurrentModule.hModule = hModule;
			CurrentModule.RealInLoadOrderLinks = Current->InLoadOrderLinks.Blink->Flink;
			CurrentModule.RealInInitializationOrderLinks = Current->InInitializationOrderLinks.Blink->Flink;
			CurrentModule.RealInMemoryOrderLinks = Current->InMemoryOrderLinks.Blink->Flink;
			CurrentModule.Entry = Current;
			UnlinkedModules.push_back(CurrentModule);

			UNLINK(Current->InLoadOrderLinks);
			UNLINK(Current->InInitializationOrderLinks);
			UNLINK(Current->InMemoryOrderLinks);

			break;
		}

		CurrentEntry = CurrentEntry->Flink;
	}
}

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