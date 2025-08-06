#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Utils.h"
#include "Common.h"

#define APPEND_XML_IDS_TO_RDL_ADDR 0x57DA40
#define APPEND_WSTR_TO_RDL_ADDR 0x57E2C0

#define MOV_SHIP_INFO_INVENTORY_ADDR 0x47BD41

#define APPEND_SHIP_INFO_INVENTORY_ADDR 0x47C1E2

typedef bool AppendXmlIdsToRdlFunc(UINT ids, RenderDisplayList& rdl);
typedef void AppendWstrToRdlFunc(LPCWSTR wstr, UINT strLen, RenderDisplayList& rdl, DWORD flags);

inline bool AppendXmlIdsToRdl(UINT ids, RenderDisplayList& rdl)
{
    return ((AppendXmlIdsToRdlFunc*) APPEND_XML_IDS_TO_RDL_ADDR)(ids, rdl);
}

inline void AppendWstrToRdl(LPCWSTR wstr, UINT strLen, RenderDisplayList& rdl)
{
    ((AppendWstrToRdlFunc*) APPEND_WSTR_TO_RDL_ADDR)(wstr, strLen, rdl, 0);
}

void AppendShipInfo_Inventory_Hook(const Archetype::Ship& shipArch, RenderDisplayList& rdl)
{
    AppendXmlIdsToRdl(1337, rdl);
}

void Init()
{
    BYTE movEcxEbp[3] = { 0x89, 0xE9, 0x90 };
    Patch(MOV_SHIP_INFO_INVENTORY_ADDR, movEcxEbp, sizeof(movEcxEbp));

    Hook(APPEND_SHIP_INFO_INVENTORY_ADDR, (DWORD) AppendShipInfo_Inventory_Hook, 5, false);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(hinstDLL);
    UNREFERENCED_PARAMETER(lpReserved);

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
        Init();
    }

    return TRUE;
}
