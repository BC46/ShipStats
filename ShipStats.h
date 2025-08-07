#include "Common.h"

struct ShipStat
{
    LPWSTR name;
    void (*printStat)(LPWSTR buffer, const Archetype::Ship& shipArch);
};
