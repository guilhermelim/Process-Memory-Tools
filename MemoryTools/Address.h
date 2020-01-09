#pragma once

class Address
{
public:
	static void EscreverB();

	static uintptr_t GetPointerBaseAddress(DWORD _ProcessID, HANDLE _ProcessHandle, const char _MooduleBase[], uintptr_t _Pointer, const std::vector<uintptr_t>& _Offsets);

	static uintptr_t GetPointerBaseAddress(DWORD _ProcessID, const char _MooduleBase[], uintptr_t _Pointer, const std::vector<uintptr_t>& _Offsets);

};

