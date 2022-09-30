#include <Windows.h>
#include "remap.h"

#pragma comment(linker, "/ALIGN:0x10000")

bool selfcode_protect() {
    ULONG_PTR ImageBase = 0;
    UNREFERENCED_PARAMETER(NULL);
    UNREFERENCED_PARAMETER(NULL);

    ImageBase = (ULONG_PTR)GetModuleHandleA(NULL);
    if (!ImageBase)
    {
        return EXIT_FAILURE;
    }

    if (!RmpRemapImage(ImageBase))
    {
        return EXIT_FAILURE;
    }
    return true;
}

