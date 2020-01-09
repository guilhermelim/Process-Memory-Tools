#include "pch.h"
#include "framework.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>
#include "MemoryTools.h"

// TODO: Este é um exemplo de uma função de biblioteca
void MemoryTools::EscreverA()
{
	std::cout << "Escrevendo da LIB Main.\n";
}

DWORD MemoryTools::GetProcessID(const char _ProcessName[])
{
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE;
		PE.dwSize = sizeof(PE);

		BOOL retornoProcess = Process32First(HandleSnapshot, &PE); // Retorna True enquanto houver processos para listar.
		while (retornoProcess)
		{
			if (_tcscmp(PE.szExeFile, _ProcessName) == 0) {
				CloseHandle(HandleSnapshot);
				return PE.th32ProcessID;
			}

			retornoProcess = Process32Next(HandleSnapshot, &PE);
		}
	}
	
	CloseHandle(HandleSnapshot);
	return 0;
}

HANDLE MemoryTools::GetProcessHandle(DWORD _ProcessID)
{
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, _ProcessID); // dwDesiredAccess
}
