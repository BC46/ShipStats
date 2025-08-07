#include "Utils.h"
#include "Common.h"
#include <stdio.h>

#define APPEND_XML_IDS_TO_RDL_ADDR 0x57DA40
#define APPEND_WSTR_TO_RDL_ADDR 0x57E2C0

#define MOV_SHIP_INFO_INVENTORY_ADDR 0x47BD41

#define APPEND_SHIP_INFO_INVENTORY_ADDR 0x47C1E2

#define FL_BUFFER ((LPWSTR) 0x66FC60)

LPCWSTR statsHeader = L"<RDL><PUSH/><PARA/><JUST loc=\"c\"/><TRA bold=\"true\"/><TEXT>Stats</TEXT><TRA bold=\"false\"/><PARA/><JUST loc=\"l\"/><PARA/><TEXT>Gun/Turret Mounts: 6/1</TEXT><PARA/><TEXT>Armor: 7900</TEXT><PARA/><TEXT>Cargo Space: 70</TEXT><PARA/><TEXT>Max. Batteries/Nanobots: 49/49</TEXT><PARA/><TEXT>Optimal Weapon Class: 7</TEXT><PARA/><TEXT>Maximum Weapon Class: 9</TEXT><PARA/><TEXT>Additional Equipment: M, CM, CD/T</TEXT><PARA/><POP/></RDL>",
        armorStatStr = L"Armor: ",
        cargoSpaceStatStr = L"Cargo Space: ",
        maxBatBotsStatStr = L"Max. Batteries/Nanobots: ",
        statStartStr = L"<TEXT>",
        statEndStr = L"</TEXT><PARA/>";

inline bool AppendXmlIdsToRdl(UINT ids, RenderDisplayList& rdl)
{
    typedef bool AppendXmlIdsToRdlFunc(UINT, RenderDisplayList&);
    return ((AppendXmlIdsToRdlFunc*) APPEND_XML_IDS_TO_RDL_ADDR)(ids, rdl);
}

inline void AppendXmlWstrToRdl(LPCWSTR wstr, RenderDisplayList& rdl)
{
    typedef void AppendXmlWstrToRdlFunc(LPCWSTR, UINT strLen, RenderDisplayList&, DWORD flags);
    ((AppendXmlWstrToRdlFunc*) APPEND_WSTR_TO_RDL_ADDR)(wstr, wcslen(wstr), rdl, NULL);
}

void AppendShipInfo_Inventory_Hook(const Archetype::Ship& shipArch, RenderDisplayList& rdl)
{
    // Some ships in vanilla FL (e.g. capships) only have a description as infocard, so make sure this is printed.
    if (!shipArch.idsInfo1)
        AppendXmlIdsToRdl(shipArch.idsInfo, rdl);

    // TODO: Make overriding existing stats optional.

    wcscpy(FL_BUFFER, L"<RDL><PUSH/><PARA/><JUST loc=\"c\"/><TRA bold=\"true\"/><TEXT>Stats</TEXT><TRA bold=\"false\"/><PARA/><JUST loc=\"l\"/>");

    wcscat(FL_BUFFER, L"<PARA/><TEXT>Armor: ");

    WCHAR armor[64];
    _snwprintf(armor, sizeof(armor) / sizeof(WCHAR), L"%.0f", shipArch.hitPts);
    wcscat(FL_BUFFER, armor);
    wcscat(FL_BUFFER, L"</TEXT>");

    wcscat(FL_BUFFER, L"<POP/></RDL>");

    AppendXmlWstrToRdl(FL_BUFFER, rdl);
}

void Init()
{
    BYTE movEcxEbp[3] = { 0x89, 0xE9, 0x90 };
    Patch(MOV_SHIP_INFO_INVENTORY_ADDR, movEcxEbp, sizeof(movEcxEbp));

    Hook(APPEND_SHIP_INFO_INVENTORY_ADDR, AppendShipInfo_Inventory_Hook, 5, false);
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
