#include "pch.h"
#include "List.h"

//		• Listagem de processos em execução no sistema
void List::Processes() {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // dwFlagListarProcessos
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE;
		PE.dwSize = sizeof(PE);
		BOOL retornoProcess = Process32First(HandleSnapshot, &PE); // Retorna True enquanto houver processos para listar.
		while (retornoProcess)
		{
			printf("ID: %d  \tNome: %s \n", PE.th32ProcessID, PE.szExeFile);
			retornoProcess = Process32Next(HandleSnapshot, &PE);
		}
	}
	CloseHandle(HandleSnapshot);
}
//		• Listagem de Modulos em execução de um Processo
void List::ProcessModules(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _ProcessID); // #dwFlagListarModulosDoProcesso | dwFlagListarModulosDoProcesso32bits
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL retornoModule = Module32First(HandleSnapshot, &ME); // Retorna True enquanto houver processos para listar.
		while (retornoModule)
		{
			printf("Nome: %s \t\tBase: %I64X \t\tSize: %d\n", ME.szModule, ME.modBaseAddr, ME.modBaseSize);
			retornoModule = Module32Next(HandleSnapshot, &ME);
		}
	}
	CloseHandle(HandleSnapshot);
}
//		• Listagem de Threads em execução de um Processo especificado
void List::ProcessThreads(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // dwFlagListarThreadsDoSistema
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL retornoThread = Thread32First(HandleSnapshot, &TE); // Retorna True enquanto houver processos para listar.
		while (retornoThread)
		{
			if (TE.th32OwnerProcessID == _ProcessID) {
				printf("Thread ID: %d\n", TE.th32ThreadID);
			}
			retornoThread = Thread32Next(HandleSnapshot, &TE);
		}
	}
	CloseHandle(HandleSnapshot);
}