#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class RenderDisplayList
{};

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
        BYTE x104[0x3C];
        int nanobotLimit; // 0x140
        int shieldBatteryLimit; // 0x144
    };
}
