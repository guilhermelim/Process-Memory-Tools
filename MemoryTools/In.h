#pragma once
class In
{
public:
	// Write Process Memory Para Valores Primitivos
	template<typename T>
	static bool WriteProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress, T _buffer) {
		SIZE_T numberOfBytesToRead = sizeof(_buffer);
		return WriteProcessMemory(_ProcessHandle, reinterpret_cast<void*>(_BaseAddress), &_buffer, numberOfBytesToRead, NULL);
	}

	// Write Process Memory Para Array of Bytes
	static bool WriteProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress, const std::vector<BYTE>& _buffer) {
		auto bufferToArray = _buffer.data(); // Converte vector para array[]
		const SIZE_T numberOfBytesToRead = _buffer.size();
		return WriteProcessMemory(_ProcessHandle, reinterpret_cast<void*>(_BaseAddress), bufferToArray, numberOfBytesToRead, NULL);
	}
};