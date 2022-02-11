#include <windows.h>


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch(ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            ::MessageBoxA(NULL, (char*)"DLL_PROCESS_ATTACH", (char*)"Tips", NULL);
            break;
        }
        case DLL_THREAD_ATTACH:
        {
            ::MessageBoxA(NULL, (char*)"DLL_THREAD_ATTACH", (char*)"Tips", NULL);
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            ::MessageBoxA(NULL, (char*)"DLL_PROCESS_DETACH", (char*)"Tips", NULL);
            break;
        }
        case DLL_THREAD_DETACH:
        {
            ::MessageBoxA(NULL, (char*)"DLL_THREAD_DETACH", (char*)"Tips", NULL);
            break;
        }

    }

    return TRUE;

}
