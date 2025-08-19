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
    // TODO: Non-hardcoded optimal weapon class?
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
        m   |= (shipArch.hpTypes[i].id == HPTYPEID_MINE_DROPPER);
        cm  |= (shipArch.hpTypes[i].id == HPTYPEID_COUNTERMEASURE_DROPPER);
        cd  |= (shipArch.hpTypes[i].id == HPTYPEID_TORPEDO_SPECIAL_2);
        t   |= (shipArch.hpTypes[i].id == HPTYPEID_TORPEDO_SPECIAL_1);
    }

    LPCWSTR mStr = m ? ((cm | cd | t) ? L"M, " : L"M") : L"";
    LPCWSTR cmStr = cm ? ((cd | t) ? L"CM, " : L"CM") : L"";
    LPCWSTR cdtStr = (cd & t) ? L"CD/T" : (cd ? L"CD" : (t ? L"T" : L""));
    swprintf(buffer, L"%s%s%s", mStr, cmStr, cdtStr);
}

void AppendShipInfo(const Archetype::Ship& shipArch, RenderDisplayList& rdl, bool printStatNames)
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
    if (!shipArch.idsInfo1 && printStatNames)
        AppendXmlIdsToRdl(shipArch.idsInfo, rdl);

    // TODO: Make overriding existing hard-coded stat infocards optional.
    LPCWSTR statsHeader =  L"<RDL><PUSH/>";
    wcscpy(FL_BUFFER, statsHeader);

    LPCWSTR statsHeaderText = printStatNames
        ? L"<PARA/><JUST loc=\"c\"/><TRA bold=\"true\"/><TEXT>Stats</TEXT><TRA bold=\"false\"/><PARA/><JUST loc=\"l\"/><PARA/>"
        : L"<PARA/><PARA/>";
    wcscat(FL_BUFFER, statsHeaderText);

    WCHAR lineBuffer[128], statBuffer[64];
    for (int i = 0; i < sizeof(shipStats) / sizeof(shipStats[0]); ++i)
    {
        shipStats[i].printStat(statBuffer, shipArch);
        if (printStatNames)
            swprintf(lineBuffer, L"<TEXT>%s: %s</TEXT><PARA/>", shipStats[i].name, statBuffer);
        else
            swprintf(lineBuffer, L"<TEXT>%s</TEXT><PARA/>", statBuffer);
        wcscat(FL_BUFFER, lineBuffer);
    }

    LPCWSTR statsFooter = L"<POP/></RDL>";
    wcscat(FL_BUFFER, statsFooter);

    AppendXmlWstrToRdl(FL_BUFFER, rdl);
}

void AppendShipInfoWithStatNames(const Archetype::Ship& shipArch, RenderDisplayList& rdl)
{
    AppendShipInfo(shipArch, rdl, true);
}

void AppendShipInfoWithoutStatNames(const Archetype::Ship& shipArch, RenderDisplayList& rdl)
{
    AppendShipInfo(shipArch, rdl, false);
}

void Init()
{
    #define MOV_SHIP_INFO_INVENTORY_ADDR 0x47BD41
    #define MOV_SHIP_INFO_QUESTION_MARK_DEALER_ADDR 0x47BF9B
    #define MOV_PLAYER_SHIP_INFO_SHIP_DEALER_ADDR 0x4B8FF1
    #define MOV_OFFERED_SHIP_INFO_SHIP_DEALER_ADDR 0x4B8C06

    // Replaces the ship arch's infocard IDS with a pointer to the ship arch such that we can check it in the hooks.
    BYTE movEcxEbp[] = { 0x89, 0xE9, 0x90 };
    BYTE movEdxEbp[] = { 0x89, 0xEA, 0x0F, 0x1F, 0x40, 0x00 };
    BYTE movEdxEdi[] = { 0x89, 0xFA, 0x0F, 0x1F, 0x40, 0x00 };
    Patch(MOV_SHIP_INFO_INVENTORY_ADDR, movEcxEbp, sizeof(movEcxEbp)); // mov ecx, [ebp+0x18] -> mov ecx, ebp
    Patch(MOV_SHIP_INFO_QUESTION_MARK_DEALER_ADDR, movEdxEbp, sizeof(movEdxEbp)); // mov edx, [ebp+0xF4] -> mov edx, ebp
    Patch(MOV_PLAYER_SHIP_INFO_SHIP_DEALER_ADDR, movEdxEdi, sizeof(movEdxEdi)); // mov edx, [edi+0xF4] -> mov edx, edi
    Patch(MOV_OFFERED_SHIP_INFO_SHIP_DEALER_ADDR, movEdxEdi, sizeof(movEdxEdi)); // mov edx, [edi+0xF4] -> mov edx, edi

    #define APPEND_SHIP_INFO_INVENTORY_ADDR 0x47C1E2
    #define APPEND_SHIP_INFO_SHIP_DEALER_ADDR 0x4B8EB9
    #define APPEND_SHIP_INFO_QUESTION_MARK_DEALER_ADDR 0x47BFA7

    Hook(APPEND_SHIP_INFO_INVENTORY_ADDR, AppendShipInfoWithStatNames, 5);
    Hook(APPEND_SHIP_INFO_SHIP_DEALER_ADDR, AppendShipInfoWithoutStatNames, 5);
    Hook(APPEND_SHIP_INFO_QUESTION_MARK_DEALER_ADDR, AppendShipInfoWithoutStatNames, 5);
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
