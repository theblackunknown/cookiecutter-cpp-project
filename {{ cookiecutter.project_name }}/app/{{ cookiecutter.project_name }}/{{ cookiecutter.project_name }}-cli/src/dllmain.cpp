#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#undef ERROR

#pragma clang diagnostic ignored "-Wmissing-prototypes"

BOOL WINAPI DllMain( HMODULE /*hModule*/, DWORD   /*ul_reason_for_call*/, LPVOID  /*lpReserved*/ )
{
    // switch (ul_reason_for_call)
    // {
    // case DLL_PROCESS_ATTACH:
    // case DLL_THREAD_ATTACH:
    // case DLL_THREAD_DETACH:
    // case DLL_PROCESS_DETACH:
    //     break;
    // }
    return TRUE;
}
