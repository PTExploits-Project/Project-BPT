// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include <Windows.h>

extern void lol(void* pFunction, bool bLoop);

extern int menu();
extern void hooks();

extern void hHora();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        lol(hHora, false);

        lol(menu, false);
        lol(hooks, false);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

