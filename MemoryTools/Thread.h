#pragma once
class Thread
{
private:

public:
	static void SuspendThreads(DWORD _ProcessID);
	static void ResumeThreads(DWORD _ProcessID);
	static std::string GetThreadsStartName(DWORD _ProcessID, uintptr_t _StartAddress);
	static void ListThreadsStartAddress(DWORD _ProcessID);
	static void TerminateThreadsStartAddress(DWORD _ProcessID, const char _NameThread[]);
};
