#include "remap.h"
#include "exports.h"


typedef BOOL(NTAPI* REMAP_ROUTINE)(
    _In_ PVOID pRemapRegion
    );



static ULONG g_CharacteristicsProtectionMap[2][2][2] =
{
    {
        { PAGE_NOACCESS, PAGE_WRITECOPY },
        { PAGE_READONLY, PAGE_READWRITE }

    },
    {
        { PAGE_EXECUTE, PAGE_EXECUTE_WRITECOPY },
        { PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE }
    },
};



_Check_return_
static
BOOL
RmppVerifyPeSectionAlignment(
    _In_ PIMAGE_NT_HEADERS pNtHeaders
);

static
VOID
RmppCopyPeSections(
    _In_ PIMAGE_NT_HEADERS pNtHeaders,
    _In_ ULONG_PTR DestinationBase
);

_Check_return_
static
BOOL
RmppRemapImageRoutine(
    _In_ PVOID pRemapRegion
);

_Check_return_
static
ULONG
RmppConvertSectionCharacteristicsToPageProtection(
    _In_ ULONG Characteristics
);

_Check_return_
static
BOOL
RmppMapProtectedView(
    _In_ HANDLE hSection,
    _In_ ULONG_PTR BaseAddress,
    _In_ SIZE_T cbSize,
    _In_ SIZE_T cbOffset,
    _In_ ULONG Protection
);

_Check_return_
static
BOOL
RmppValidateRemappedImageProtection(
    _In_ ULONG_PTR ImageBase
);

_Check_return_
static
BOOL
RmppValidateRemappedPeSectionProtection(
    _In_ PVOID pSectionBase
);



_Use_decl_annotations_
BOOL
RmpRemapImage(
    ULONG_PTR ImageBase
)
{
    PIMAGE_NT_HEADERS pNtHeaders = NULL;
    PVOID pRemapRegion = NULL;
    REMAP_ROUTINE fpRemapRoutine = NULL;
    BOOL status = TRUE;


    pNtHeaders = RtlImageNtHeader((PVOID)ImageBase);
    if (!pNtHeaders)
    {
        status = FALSE;
        goto exit;
    }

    status = RmppVerifyPeSectionAlignment(pNtHeaders);
    if (!status)
    {
        goto exit;
    }


    pRemapRegion = VirtualAlloc(
        NULL,
        pNtHeaders->OptionalHeader.SizeOfImage,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE);
    if (!pRemapRegion)
    {
        status = FALSE;
        goto exit;
    }



    RmppCopyPeSections(pNtHeaders, (ULONG_PTR)pRemapRegion);


    fpRemapRoutine = (REMAP_ROUTINE)(
        (ULONG_PTR)pRemapRegion +
        (ULONG_PTR)RmppRemapImageRoutine -
        ImageBase);



    status = fpRemapRoutine(pRemapRegion);
    if (!status)
    {
        goto exit;
    }

    status = RmppValidateRemappedImageProtection(ImageBase);
    if (!status)
    {
        goto exit;
    }

exit:
    if (pRemapRegion)
    {
        if (!VirtualFree(pRemapRegion, 0, MEM_RELEASE))
        {
            return EXIT_FAILURE;
        }
    }

    return status;
}



_Use_decl_annotations_ static BOOL RmppVerifyPeSectionAlignment(PIMAGE_NT_HEADERS pNtHeaders)
{
    SYSTEM_INFO SystemInfo = {};
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;
    ULONG_PTR SectionBase = 0;
    BOOL status = TRUE;

    GetSystemInfo(&SystemInfo);

    pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);

    for (WORD i = 0; i < pNtHeaders->FileHeader.NumberOfSections; ++i)
    {
        SectionBase = pNtHeaders->OptionalHeader.ImageBase + pSectionHeader[i].VirtualAddress;



        status = POINTER_IS_ALIGNED(SectionBase, SystemInfo.dwAllocationGranularity);
        if (!status)
        {
            goto exit;
        }
    }


    status = POINTER_IS_ALIGNED(
        pNtHeaders->OptionalHeader.ImageBase,
        SystemInfo.dwAllocationGranularity);
    if (!status)
    {

        goto exit;
    }

exit:
    return status;
}



_Use_decl_annotations_ static VOID RmppCopyPeSections(PIMAGE_NT_HEADERS pNtHeaders, ULONG_PTR DestinationBase)
{
    ULONG_PTR SourceBase = 0;
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;

    SourceBase = pNtHeaders->OptionalHeader.ImageBase;

    pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);


    for (WORD i = 0; i < pNtHeaders->FileHeader.NumberOfSections; ++i)
    {


        RtlCopyMemory((PVOID)(DestinationBase + pSectionHeader[i].VirtualAddress), (PVOID)(SourceBase + pSectionHeader[i].VirtualAddress), pSectionHeader[i].Misc.VirtualSize);
    }


    RtlCopyMemory((PVOID)DestinationBase, (PVOID)SourceBase, PAGE_SIZE);
}

_Use_decl_annotations_ static BOOL
RmppRemapImageRoutine(PVOID pRemapRegion)
{
    PIMAGE_NT_HEADERS pNtHeaders = NULL;
    HANDLE hSection = NULL;
    LARGE_INTEGER cbSectionSize = {};
    PVOID pViewBase = NULL;
    ULONG_PTR ImageBase = 0;
    LARGE_INTEGER cbSectionOffset = {};
    SIZE_T cbViewSize = 0;
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;
    ULONG Protection = 0;
    NTSTATUS ntstatus = STATUS_SUCCESS;
    BOOL status = TRUE;

    pNtHeaders = RtlImageNtHeader(pRemapRegion);
    if (!pNtHeaders)
    {

        status = FALSE;
        goto exit;
    }

    cbSectionSize.QuadPart = pNtHeaders->OptionalHeader.SizeOfImage;


    ntstatus = NtCreateSection(&hSection, SECTION_ALL_ACCESS, NULL, &cbSectionSize, PAGE_EXECUTE_READWRITE, SEC_COMMIT | SEC_NO_CHANGE, NULL);
    if (!NT_SUCCESS(ntstatus))
    {
        status = FALSE;
        goto exit;
    }

    ntstatus = NtMapViewOfSection(hSection, NtCurrentProcess(), &pViewBase, 0, pNtHeaders->OptionalHeader.SizeOfImage, &cbSectionOffset, &cbViewSize, ViewUnmap, 0, PAGE_READWRITE);
    if (!NT_SUCCESS(ntstatus))
    {
        status = FALSE;
        goto exit;
    }


    RmppCopyPeSections(pNtHeaders, (ULONG_PTR)pViewBase);


    ntstatus = NtUnmapViewOfSection(NtCurrentProcess(), pViewBase);
    if (!NT_SUCCESS(ntstatus))
    {
        status = FALSE;
        goto exit;
    }

    ImageBase = pNtHeaders->OptionalHeader.ImageBase;

    ntstatus = NtUnmapViewOfSection(NtCurrentProcess(), (PVOID)ImageBase);
    if (!NT_SUCCESS(ntstatus))
    {
        status = FALSE;
        goto exit;
    }


    pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);

    for (WORD i = 0; i < pNtHeaders->FileHeader.NumberOfSections; ++i)
    {
        Protection = RmppConvertSectionCharacteristicsToPageProtection(
            pSectionHeader[i].Characteristics);

        status = RmppMapProtectedView(
            hSection,
            ImageBase + pSectionHeader[i].VirtualAddress,
            pSectionHeader[i].Misc.VirtualSize,
            pSectionHeader[i].VirtualAddress,
            Protection);
        if (!status)
        {
            goto exit;
        }
    }


    status = RmppMapProtectedView(
        hSection,
        ImageBase,
        PAGE_SIZE,
        0,
        PAGE_READONLY);
    if (!status)
    {
        goto exit;
    }

exit:
    if (hSection)
    {
        ntstatus = NtClose(hSection);
        if (!NT_SUCCESS(ntstatus))
        {
            return EXIT_FAILURE;
        }
    }

    return status;
}


_Use_decl_annotations_ static ULONG RmppConvertSectionCharacteristicsToPageProtection(ULONG Characteristics)
{
    BOOL fExecutable = FALSE;
    BOOL fReadable = FALSE;
    BOOL fWritable = FALSE;
    ULONG Protection = 0;

    if (0 != (IMAGE_SCN_MEM_EXECUTE & Characteristics))
    {
        fExecutable = TRUE;
    }

    if (0 != (IMAGE_SCN_MEM_READ & Characteristics))
    {
        fReadable = TRUE;
    }

    if (0 != (IMAGE_SCN_MEM_WRITE & Characteristics))
    {
        fWritable = TRUE;
    }

    Protection = g_CharacteristicsProtectionMap[fExecutable][fReadable][fWritable];

    if (0 != (IMAGE_SCN_MEM_NOT_CACHED & Characteristics))
    {
        Protection |= PAGE_NOCACHE;
    }

    return Protection;
}

_Use_decl_annotations_
static
BOOL
RmppMapProtectedView(
    HANDLE hSection,
    ULONG_PTR BaseAddress,
    SIZE_T cbSize,
    SIZE_T cbOffset,
    ULONG Protection
)
{
    LARGE_INTEGER cbSectionOffset = {};
    PVOID pViewBase = NULL;
    SIZE_T cbViewSize = 0;
    NTSTATUS ntstatus = STATUS_SUCCESS;
    BOOL status = TRUE;

    pViewBase = (PVOID)BaseAddress;
    cbViewSize = cbSize;
    cbSectionOffset.QuadPart = cbOffset;

    ntstatus = NtMapViewOfSection(
        hSection,
        NtCurrentProcess(),
        &pViewBase,
        0,
        0,
        &cbSectionOffset,
        &cbViewSize,
        ViewUnmap,
        SEC_NO_CHANGE,
        Protection);
    if (!NT_SUCCESS(ntstatus))
    {

        status = FALSE;
        goto exit;
    }

exit:
    return status;
}

_Use_decl_annotations_ static BOOL RmppValidateRemappedImageProtection(ULONG_PTR ImageBase)
{
    PIMAGE_NT_HEADERS pNtHeaders = NULL;
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;
    BOOL status = TRUE;


    pNtHeaders = RtlImageNtHeader((PVOID)ImageBase);
    if (!pNtHeaders)
    {
        status = FALSE;
        goto exit;
    }

    pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);

    for (WORD i = 0; i < pNtHeaders->FileHeader.NumberOfSections; ++i)
    {
        status = RmppValidateRemappedPeSectionProtection(
            (PVOID)(ImageBase + pSectionHeader[i].VirtualAddress));
        if (!status)
        {
            goto exit;
        }
    }


    status = RmppValidateRemappedPeSectionProtection((PVOID)ImageBase);
    if (!status)
    {
        goto exit;
    }

exit:
    return status;
}



_Use_decl_annotations_
static
BOOL
RmppValidateRemappedPeSectionProtection(
    PVOID pSectionBase
)
{
    MEMORY_BASIC_INFORMATION MemoryBasicInfo = {};
    ULONG TestProtect = 0;
    ULONG PreviousProtect = 0;
    BOOL status = TRUE;

    if (!VirtualQuery(pSectionBase, &MemoryBasicInfo, sizeof(MemoryBasicInfo)))
    {

        status = FALSE;
        goto exit;
    }


    if (PAGE_EXECUTE_READWRITE != MemoryBasicInfo.Protect)
    {
        TestProtect = PAGE_EXECUTE_READWRITE;
    }
    else
    {
        TestProtect = PAGE_NOACCESS;
    }


    status = VirtualProtect(
        pSectionBase,
        MemoryBasicInfo.RegionSize,
        TestProtect,
        &PreviousProtect);
    if (status)
    {

        goto exit;
    }

    status = TRUE;

exit:
    return status;
}
