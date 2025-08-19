#ifndef _COMMON_H_
#define _COMMON_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef USE_ST6
#include "st6.h"
#else
#include <vector>
namespace st6 = std;
#endif

class RenderDisplayList
{};

typedef enum HPTYPEID {
    HPTYPEID_MINE_DROPPER = 11,
    HPTYPEID_COUNTERMEASURE_DROPPER = 12,
    HPTYPEID_GUN_SPECIAL_1 = 15,
    HPTYPEID_GUN_SPECIAL_2 = 16,
    HPTYPEID_GUN_SPECIAL_3 = 17,
    HPTYPEID_GUN_SPECIAL_4 = 18,
    HPTYPEID_GUN_SPECIAL_5 = 19,
    HPTYPEID_GUN_SPECIAL_6 = 20,
    HPTYPEID_GUN_SPECIAL_7 = 21,
    HPTYPEID_GUN_SPECIAL_8 = 22,
    HPTYPEID_GUN_SPECIAL_9 = 23,
    HPTYPEID_GUN_SPECIAL_10 = 24,
    HPTYPEID_TURRET_SPECIAL_1 = 25,
    HPTYPEID_TURRET_SPECIAL_2 = 26,
    HPTYPEID_TURRET_SPECIAL_3 = 27,
    HPTYPEID_TURRET_SPECIAL_4 = 28,
    HPTYPEID_TURRET_SPECIAL_5 = 29,
    HPTYPEID_TURRET_SPECIAL_6 = 30,
    HPTYPEID_TURRET_SPECIAL_7 = 31,
    HPTYPEID_TURRET_SPECIAL_8 = 32,
    HPTYPEID_TURRET_SPECIAL_9 = 33,
    HPTYPEID_TURRET_SPECIAL_10 = 34,
    HPTYPEID_TORPEDO_SPECIAL_2 = 35,
    HPTYPEID_TORPEDO_SPECIAL_1 = 36,
    HPTYPEID_FORCE_DWORD = 0x7FFFFFFF
} HPTYPEID;

struct HpType
{
    HPTYPEID id;
    st6::vector<LPCSTR> hardpoints;
};

namespace Archetype
{
    struct Ship
    {
        BYTE x00[0x18];
        UINT idsInfo; // 0x18, full stats
        float hitPts; // 0x1C
        BYTE x20[0xCC];
        UINT idsInfo1; // 0xEC, name and description
        UINT idsInfo2; // 0xF0, stat names
        UINT idsInfo3; // 0xF4, stat values
        BYTE xF8[0x8];
        float holdSize; // 0x100
        BYTE x104[0x2C];
        st6::vector<HpType> hpTypes; // 0x130
        int nanobotLimit; // 0x140
        int shieldBatteryLimit; // 0x144
    };
}

__declspec( dllimport ) UINT CreateID(LPCSTR str);

#endif
