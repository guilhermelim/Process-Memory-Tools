#pragma once
class Out
{
public:
	template<typename T>
	static T ReadProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress) {
		T retornoReadProcess = 0;
		SIZE_T NumberOfBytesToRead = sizeof(retornoReadProcess);
		SIZE_T NumberOfBytesActuallyRead;
		BOOL err = ReadProcessMemory(_ProcessHandle, reinterpret_cast<void*>(_BaseAddress), &retornoReadProcess, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
		if (err || NumberOfBytesActuallyRead != NumberOfBytesToRead)
			return retornoReadProcess;
	}

};
