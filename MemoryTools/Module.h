#pragma once
class Module
{
public:
	static uintptr_t GetBaseAddress(DWORD _ProcessID, const char _ModuleName[]);
	static uintptr_t GetBaseSize(DWORD _ProcessID, const char _ModuleName[]);
};

