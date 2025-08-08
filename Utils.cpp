#include "utils.h"

// Patches the memory
void Patch(DWORD vOffset, LPVOID mem, UINT len)
{
    DWORD _;
    VirtualProtect((PVOID) vOffset, len, PAGE_EXECUTE_READWRITE, &_);
    memcpy((PVOID) vOffset, mem, len);
}

// Replaces instructions with nop (0x90)
void Nop(DWORD vOffset, UINT len)
{
    DWORD _;
    VirtualProtect((PVOID) vOffset, len, PAGE_EXECUTE_READWRITE, &_);
    memset((PVOID) vOffset, 0x90, len);
}

// Adds a jmp or call instruction at a specified address which detours the instruction pointer to arbitrary code
void Hook(DWORD location, PVOID hookFunc, UINT instrLen, bool jmp)
{
    BYTE callOpcode = 0xE8, jmpOpcode = 0xE9;
    DWORD relOffset = (DWORD) hookFunc - location - 5;

    // Set the opcode for the call or jmp instruction
    Patch(location, &(jmp ? jmpOpcode : callOpcode), sizeof(BYTE));

    // Set and calculate the relative offset for the hook function
    Patch(location + 1, &relOffset, sizeof(DWORD));

    // Nop out excess bytes
    if (instrLen > 5)
        Nop(location + 5, instrLen - 5);
}
