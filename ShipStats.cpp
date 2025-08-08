#include "ShipStats.h"
#include "Freelancer.h"
#include "Utils.h"
#include <stdio.h>
#include <set>

void AddHardpointIdsToSet(std::set<UINT> &set, const st6::vector<LPCSTR>& hardpoints)
{
    for (UINT i = 0; i < hardpoints.size(); ++i)
        set.insert(CreateID(hardpoints[i]));
}

void PrintGunTurretMountsStat(LPWSTR buffer, const Archetype::Ship& shipArch)
{
    std::set<UINT> gunMountIds, turretMountIds;

    for (UINT i = 0; i < shipArch.hpTypes.size(); ++i)
    {
        if (shipArch.hpTypes[i].id >= HPTYPEID_GUN_SPECIAL_1 && shipArch.hpTypes[i].id <= HPTYPEID_GUN_SPECIAL_10)
            AddHardpointIdsToSet(gunMountIds, shipArch.hpTypes[i].hardpoints);
        else if (shipArch.hpTypes[i].id >= HPTYPEID_TURRET_SPECIAL_1 && shipArch.hpTypes[i].id <= HPTYPEID_TURRET_SPECIAL_10)
            AddHardpointIdsToSet(turretMountIds, shipArch.hpTypes[i].hardpoints);
    }

    swprintf(buffer, L"%u/%u", gunMountIds.size(), turretMountIds.size());
}

void PrintArmorStat(LPWSTR buffer, const Archetype::Ship& shipArch)
{
    swprintf(buffer, L"%.0f", shipArch.hitPts);
}

void PrintCargoSpaceStat(LPWSTR buffer, const Archetype::Ship& shipArch)
{
    swprintf(buffer, L"%.0f", shipArch.holdSize);
}

void PrintBatsBotsStat(LPWSTR buffer, const Archetype::Ship& shipArch)
{
    swprintf(buffer, L"%d/%d", shipArch.shieldBatteryLimit, shipArch.nanobotLimit);
}

int GetMaximumWeaponClass(const Archetype::Ship& shipArch)
{
    int result = 1;

    for (UINT i = 0; i < shipArch.hpTypes.size(); ++i)
    {
        if (shipArch.hpTypes[i].id >= HPTYPEID_GUN_SPECIAL_1 && shipArch.hpTypes[i].id <= HPTYPEID_GUN_SPECIAL_10)
            result = max(result, shipArch.hpTypes[i].id - HPTYPEID_GUN_SPECIAL_1 + 1);
        else if (shipArch.hpTypes[i].id >= HPTYPEID_TURRET_SPECIAL_1 && shipArch.hpTypes[i].id <= HPTYPEID_TURRET_SPECIAL_10)
            result = max(result, shipArch.hpTypes[i].id - HPTYPEID_TURRET_SPECIAL_1 + 1);
    }

    return result;
}

void PrintOptimalWeaponClassStat(LPWSTR buffer, const Archetype::Ship& shipArch)
{
    swprintf(buffer, L"%d", max(GetMaximumWeaponClass(shipArch) - 2, 1));
}

void PrintMaximumWeaponClassStat(LPWSTR buffer, const Archetype::Ship& shipArch)
{
    swprintf(buffer, L"%d", GetMaximumWeaponClass(shipArch));
}

void PrintAdditionalEquipmentStat(LPWSTR buffer, const Archetype::Ship& shipArch)
{
    bool m = false, cm = false, cd = false, t = false;

    for (UINT i = 0; i < shipArch.hpTypes.size(); ++i)
    {
        m   |= shipArch.hpTypes[i].id == HPTYPEID_MINE_DROPPER;
        cm  |= shipArch.hpTypes[i].id == HPTYPEID_COUNTERMEASURE_DROPPER;
        cd  |= shipArch.hpTypes[i].id == HPTYPEID_TORPEDO_SPECIAL_2;
        t   |= shipArch.hpTypes[i].id == HPTYPEID_TORPEDO_SPECIAL_1;
    }

    LPCWSTR mStr = m ? ((cm | cd | t) ? L"M, " : L"M") : L"";
    LPCWSTR cmStr = cm ? ((cd | t) ? L"CM, " : L"CM") : L"";
    LPCWSTR cdtStr = (cd & t) ? L"CD/T" : (cd ? L"CD" : (t ? L"T" : L""));
    swprintf(buffer, L"%s%s%s", mStr, cmStr, cdtStr);
}

void AppendShipInfo_Inventory_Hook(const Archetype::Ship& shipArch, RenderDisplayList& rdl)
{
    // TODO: Make name strings configurable (localizations).
    static const ShipStat shipStats[] =
    {
        { L"Gun/Turret Mounts",         PrintGunTurretMountsStat },
        { L"Armor",                     PrintArmorStat },
        { L"Cargo Space",               PrintCargoSpaceStat },
        { L"Max. Batteries/Nanobots",   PrintBatsBotsStat },
        { L"Optimal Weapon Class",      PrintOptimalWeaponClassStat },
        { L"Maximum Weapon Class",      PrintMaximumWeaponClassStat },
        { L"Additional Equipment",      PrintAdditionalEquipmentStat }
    };

    // Some ships in vanilla FL (e.g. capships) only have a description as infocard, so make sure this is printed.
    if (!shipArch.idsInfo1)
        AppendXmlIdsToRdl(shipArch.idsInfo, rdl);

    // TODO: Make overriding existing hard-coded stat infocards optional.

    LPCWSTR statsHeader =  L"<RDL><PUSH/><PARA/><JUST loc=\"c\"/><TRA bold=\"true\"/><TEXT>Stats</TEXT><TRA bold=\"false\"/><PARA/><JUST loc=\"l\"/><PARA/>";
    wcscpy(FL_BUFFER, statsHeader);

    WCHAR lineBuffer[128], statBuffer[64];
    for (int i = 0; i < sizeof(shipStats) / sizeof(shipStats[0]); ++i)
    {
        shipStats[i].printStat(statBuffer, shipArch);
        swprintf(lineBuffer, L"<TEXT>%s: %s</TEXT><PARA/>", shipStats[i].name, statBuffer);
        wcscat(FL_BUFFER, lineBuffer);
    }

    LPCWSTR statsFooter = L"<POP/></RDL>";
    wcscat(FL_BUFFER, statsFooter);

    AppendXmlWstrToRdl(FL_BUFFER, rdl);
}

void Init()
{
    #define MOV_SHIP_INFO_INVENTORY_ADDR 0x47BD41

    #define APPEND_SHIP_INFO_INVENTORY_ADDR 0x47C1E2

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
