// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include <Windows.h>

extern void UnlinkModuleFromPEB(HMODULE hModule);
extern void lol(void* pFunction, bool bLoop);

extern void protectMachine();
extern void logx();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        //PEB
        UnlinkModuleFromPEB(hModule);

        //Protect
        lol(protectMachine, false);
        //lol(logx, false);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

