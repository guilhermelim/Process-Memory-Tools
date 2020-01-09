#include "pch.h"
#include "Module.h"

//	Get base address de um Modulo especificado
uintptr_t Module::GetBaseAddress(DWORD _ProcessID, const char _ModuleName[]) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _ProcessID); // dwFlagListarModulosDoProcesso | dwFlagListarModulosDoProcesso32bits
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL retornoModule = Module32First(HandleSnapshot, &ME); // Retorna True enquanto houver processos para listar.
		while (retornoModule)
		{
			if (_tcscmp(ME.szModule, _ModuleName) == 0) {
				CloseHandle(HandleSnapshot);
				return (uintptr_t)ME.modBaseAddr;
			}
			retornoModule = Module32Next(HandleSnapshot, &ME);
		}
	}
	CloseHandle(HandleSnapshot);
	return 0;
}
//	Get tamanho de um Modulo especificado
uintptr_t Module::GetBaseSize(DWORD _ProcessID, const char _ModuleName[]) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _ProcessID); // dwFlagListarModulosDoProcesso | dwFlagListarModulosDoProcesso32bits
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL retornoModule = Module32First(HandleSnapshot, &ME); // Retorna True enquanto houver processos para listar.
		while (retornoModule)
		{
			if (_tcscmp(ME.szModule, _ModuleName) == 0) {
				CloseHandle(HandleSnapshot);
				return (uintptr_t)ME.modBaseSize;
			}
			retornoModule = Module32Next(HandleSnapshot, &ME);
		}
	}
	CloseHandle(HandleSnapshot);
	return 0;
}
