#include "pch.h"
#include <winternl.h>
#include "Thread.h"

enum THREADINFOCLASS {
	ThreadQuerySetWin32StartAddress = 0x09
};

typedef NTSTATUS(WINAPI* ntQueryInformationThread)(
	HANDLE,
	THREADINFOCLASS,
	PVOID,
	ULONG,
	PULONG
	);

ntQueryInformationThread NtQueryInformationThread;

// Suspende a execução das Threads de um Processo especificado
void Thread::SuspendThreads(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // dwFlagListarThreadsDoSistema
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL retornoThread = Thread32First(HandleSnapshot, &TE); // Retorna True enquanto houver processos para listar.
		while (retornoThread)
		{
			if (TE.th32OwnerProcessID == _ProcessID) {
				HANDLE ThreadAccess = OpenThread(THREAD_SUSPEND_RESUME, FALSE, TE.th32ThreadID); // dwDesiredAccess_THREAD_SUSPEND_RESUME
				SuspendThread(ThreadAccess);
				CloseHandle(ThreadAccess);
			}
			retornoThread = Thread32Next(HandleSnapshot, &TE);
		}
	}
	CloseHandle(HandleSnapshot);
}

// Retoma a execução das Threads de um Processo especificado
void Thread::ResumeThreads(DWORD _ProcessID) {
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // dwFlagListarThreadsDoSistema
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		THREADENTRY32 TE;
		TE.dwSize = sizeof(TE);
		BOOL retornoThread = Thread32First(HandleSnapshot, &TE); // Retorna True enquanto houver processos para listar.
		while (retornoThread)
		{
			if (TE.th32OwnerProcessID == _ProcessID) {
				HANDLE ThreadAccess = OpenThread(THREAD_SUSPEND_RESUME, FALSE, TE.th32ThreadID); // dwDesiredAccess_THREAD_SUSPEND_RESUME
				ResumeThread(ThreadAccess);
				CloseHandle(ThreadAccess);
			}
			retornoThread = Thread32Next(HandleSnapshot, &TE);
		}
	}
	CloseHandle(HandleSnapshot);
}

// Listagem das Start Address das Threads em execução de um Processo especificado
std::string Thread::GetThreadsStartName(DWORD _ProcessID, uintptr_t _StartAddress)
{
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _ProcessID);
	std::string retornoString;
	if (HandleSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 ME;
		ME.dwSize = sizeof(ME);
		BOOL retornoModule = Module32First(HandleSnapshot, &ME);
		while (retornoModule) {
			if ((DWORD)ME.modBaseAddr < _StartAddress && ((uintptr_t)ME.modBaseAddr + (uintptr_t)ME.modBaseSize) > _StartAddress) {
				char formatarString[FILENAME_MAX];
				sprintf(formatarString, "+0x%X", ((uintptr_t)ME.modBaseAddr + ME.modBaseSize) - _StartAddress);
				retornoString = retornoString + ME.szModule;
				retornoString = retornoString + formatarString;
				CloseHandle(HandleSnapshot);
				return retornoString;
			}
			retornoModule = Module32Next(HandleSnapshot, &ME);
		}
	}
	CloseHandle(HandleSnapshot);
	return retornoString;
}

void Thread::ListThreadsStartAddress(DWORD _ProcessID)
{
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if ((DWORD)NtQueryInformationThread == 0) {
		NtQueryInformationThread = (ntQueryInformationThread)GetProcAddress(GetModuleHandle("Ntdll.dll"), "NtQueryInformationThread");

		if (HandleSnapshot != INVALID_HANDLE_VALUE) {
			THREADENTRY32 TE;
			TE.dwSize = sizeof(TE);
			BOOL retornoThread = Thread32First(HandleSnapshot, &TE);
			while (retornoThread) {
				if (TE.th32OwnerProcessID == _ProcessID) {
					DWORD startAddress = 0;
					HANDLE handleThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, TE.th32ThreadID);
					NtQueryInformationThread(handleThread, ThreadQuerySetWin32StartAddress, &startAddress, sizeof(uintptr_t), nullptr);
					printf("%s \n", GetThreadsStartName(_ProcessID, startAddress).c_str());

					CloseHandle(handleThread);
				}
				retornoThread = Thread32Next(HandleSnapshot, &TE);
			}
		}
	}
	CloseHandle(HandleSnapshot);
}

void Thread::TerminateThreadsStartAddress(DWORD _ProcessID, const char _NameThread[])
{
	HANDLE HandleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if ((DWORD)NtQueryInformationThread == 0) {
		NtQueryInformationThread = (ntQueryInformationThread)GetProcAddress(GetModuleHandle("Ntdll.dll"), "NtQueryInformationThread");

		if (HandleSnapshot != INVALID_HANDLE_VALUE) {
			THREADENTRY32 TE;
			TE.dwSize = sizeof(TE);
			BOOL retornoThread = Thread32First(HandleSnapshot, &TE);
			while (retornoThread) {
				if (TE.th32OwnerProcessID == _ProcessID) {
					DWORD startAddress = 0;
					HANDLE handleThread = OpenThread(THREAD_ALL_ACCESS, FALSE, TE.th32ThreadID);
					NtQueryInformationThread(handleThread, ThreadQuerySetWin32StartAddress, &startAddress, sizeof(uintptr_t), nullptr);

					if (strstr(GetThreadsStartName(_ProcessID, startAddress).c_str(), _NameThread) != NULL) {
						TerminateThread(handleThread, 0);
					}
					CloseHandle(handleThread);
				}

				retornoThread = Thread32Next(HandleSnapshot, &TE);
			}
		}
	}
	CloseHandle(HandleSnapshot);
}