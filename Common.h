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
        BYTE x1C[0xD0];
        UINT idsInfo1; // 0xEC, name and description
        UINT idsInfo2; // 0xF0, stat names
        UINT idsInfo3; // 0xF4, stat values
    };
}
