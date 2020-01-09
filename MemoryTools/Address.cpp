#include "pch.h"
#include "Address.h"
#include "MemoryTools.h"

void Address::EscreverB()
{
	std::cout << "Escrevendo do objeto da lib Address.\n";
	MemoryTools::EscreverA();
}

//	Get base address de um Ponteiro especificado passando PID & Handle do Processo
uintptr_t Address::GetPointerBaseAddress(DWORD _ProcessID, HANDLE _ProcessHandle, const char _MooduleBase[], uintptr_t _Pointer, const std::vector<uintptr_t>& _Offsets) {
	uintptr_t retornoPointerBaseAddress = 0;
	int tamanho = _Offsets.size();

	uintptr_t MooduleBase = Module::GetBaseAddress(_ProcessID, _MooduleBase);
	retornoPointerBaseAddress = MemoryTools::out.ReadProcess<uintptr_t>(_ProcessHandle, MooduleBase + _Pointer);
	tamanho--;

	for (int i = 0; i <= tamanho; i++)
	{
		if (i < tamanho) {
			retornoPointerBaseAddress = MemoryTools::out.ReadProcess<uintptr_t>(_ProcessHandle, retornoPointerBaseAddress + _Offsets[i]);
		}
		else
		{
			retornoPointerBaseAddress = retornoPointerBaseAddress + _Offsets[i];
		}
	}
	return retornoPointerBaseAddress;
}

//	Get base address de um Ponteiro especificado passando apenas PID & Handle do Processo
uintptr_t Address::GetPointerBaseAddress(DWORD _ProcessID, const char _MooduleBase[], uintptr_t _Pointer, const std::vector<uintptr_t>& _Offsets) {
	uintptr_t retornoPointerBaseAddress = 0;
	int tamanho = _Offsets.size();
	HANDLE processHandle = MemoryTools::GetProcessHandle(_ProcessID);

	uintptr_t MooduleBase = Module::GetBaseAddress(_ProcessID, _MooduleBase);
	//printf("Module %I64X\n", MooduleBase);
	retornoPointerBaseAddress = MemoryTools::out.ReadProcess<uintptr_t>(processHandle, MooduleBase + _Pointer);
	tamanho--;

	//printf("module + pointer %I64X  \n", retornoPointerBaseAddress);
	for (int i = 0; i <= tamanho; i++)
	{
		if (i < tamanho) {
			retornoPointerBaseAddress = MemoryTools::out.ReadProcess<uintptr_t>(processHandle, retornoPointerBaseAddress + _Offsets[i]);
		}
		else
		{
			retornoPointerBaseAddress = retornoPointerBaseAddress + _Offsets[i];
		}
	}
	CloseHandle(processHandle); // Fecha o Handle aberto
	return retornoPointerBaseAddress;
}
