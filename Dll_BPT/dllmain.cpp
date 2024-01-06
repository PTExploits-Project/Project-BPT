// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include <Windows.h>

extern void lol(void* pFunction, bool bLoop);

extern int menu();
extern void hooks();
extern void hHora();
extern void findMob();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        //Time do cheat
        lol(hHora, false);

        //Menu de opções para ativar
        lol(menu, false);

        //Hooks na sessão .text
        lol(hooks, false);

        //AutoBot..
        lol(findMob, false);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

