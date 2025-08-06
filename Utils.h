#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void Patch(DWORD vOffset, LPVOID mem, UINT len);

void Nop(DWORD vOffset, UINT len);

void Hook(DWORD location, DWORD hookFunc, UINT instrLen, bool jmp);
