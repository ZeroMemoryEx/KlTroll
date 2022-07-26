#include <Windows.h>
#include "pch.h"
unsigned char	patchOpcodes[] = { 0x48, 0xb8, // mov rax , fake func
									0, 0, 0, 0, 0, 0, 0, 0,
									0xff, 0xe0// jmp rax
};
unsigned char	msg[] = { 'T','R','O','L'
						,'L','E','D','-'
};
LPVOID	orgadd;
SIZE_T	nobytes;
static int i;

SHORT __stdcall FakeFunc(int vKey)
{
	if (vKey == msg[i])
	{
		msg[i++] = 'X';
		return -32767;
	}
	return 0;
}

int Entry()
{
	HMODULE nig = LoadLibraryA("user32.dll");
	orgadd = GetProcAddress(nig, "GetAsyncKeyState");
	if (!orgadd)
		return -1;
	void* vfunc = &FakeFunc;
	memcpy_s(patchOpcodes + 2, 8, &vfunc, 8);
	if (!WriteProcessMemory(GetCurrentProcess(), orgadd, patchOpcodes, sizeof(patchOpcodes), NULL))
		return -1;
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			Entry();
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

