//
//       ██████╗ ██████╗ ██████╗ ███████╗    ██████╗ ██╗   ██╗██╗██╗     ██████╗ ██╗███╗   ██╗ ██████╗ 
//      ██╔════╝██╔═══██╗██╔══██╗██╔════╝    ██╔══██╗██║   ██║██║██║     ██╔══██╗██║████╗  ██║██╔════╝ 
//      ██║     ██║   ██║██║  ██║█████╗      ██████╔╝██║   ██║██║██║     ██║  ██║██║██╔██╗ ██║██║  ███╗
//      ██║     ██║   ██║██║  ██║██╔══╝      ██╔══██╗██║   ██║██║██║     ██║  ██║██║██║╚██╗██║██║   ██║
//      ╚██████╗╚██████╔╝██████╔╝███████╗    ██████╔╝╚██████╔╝██║███████╗██████╔╝██║██║ ╚████║╚██████╔╝
//       ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝    ╚═════╝  ╚═════╝ ╚═╝╚══════╝╚═════╝ ╚═╝╚═╝  ╚═══╝ ╚═════╝ 
//
//											Process Memory Tools C++
//		FEATURES
//		• Listagem de processos em execução no sistema					- ListProcess();
//		• Listagem de Modulos em execução de um Processo				- ListProcessModules(DWORD _ProcessID)
//		• Listagem de Threads em execução de um Processo especificado	- ListThreads(DWORD _ProcessID)
//		• Get ID do Processo especificado								- GetProcessID(const char _ProcessName[])
//		• Get Handle Process											- GetProcessHandle(DWORD _ProcessID)
//		• Get base address de um Modulo especificado					- GetModuleBaseAddress(DWORD _ProcessID, const char _ModuleName[])
//		• Get tamanho de um Modulo especificado							- GetModuleBaseSize(DWORD _ProcessID, const char _ModuleName[])
//		• Get base address de um Ponteiro especificado					- GetPointerBaseAddress(DWORD _ProcessID, HANDLE _ProcessHandle, const char _MooduleBase[], uintptr_t _Pointer, vector<uintptr_t> &_Offsets)
//																			Obs: Você pode escolher utilizar como parametro: _ProcessID ou _ProcessHandle do processo.
//		• Suspende a execução das Threads de um Processo especificado	- void SuspendThreads(DWORD _ProcessID)
//		• Retoma a execução das Threads de um Processo especificado		- ResumeThreads(DWORD _ProcessID)
//		• Read Process Memory											- ReadProcess(DWORD _ProcessID, uintptr_t _BaseAddress) OR ReadProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress) 
//																			Obs: Você pode escolher utilizar como parametro: _ProcessID ou _ProcessHandle do processo.
//		• Write Process Memory											- WriteProcess(DWORD _ProcessID, uintptr_t _BaseAddress, uintptr_t _buffer) OR WriteProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress, uintptr_t _buffer)
//																			Obs: Você pode escolher utilizar como parametro: _ProcessID ou _ProcessHandle do processo.
//
//		CRÉDITOS:
//		Programador						-> Guilherme Lima
//		Mentor							-> SmoLL_iCe
//		Solucionador de dúvidas e erros -> Define, Devecchi, Luan Devecchi, EuPoder, Bolard, Walison.
//

//      
//      ██╗███╗   ██╗ ██████╗██╗     ██╗   ██╗██████╗ ███████╗███████╗
//      ██║████╗  ██║██╔════╝██║     ██║   ██║██╔══██╗██╔════╝██╔════╝
//      ██║██╔██╗ ██║██║     ██║     ██║   ██║██║  ██║█████╗  ███████╗
//      ██║██║╚██╗██║██║     ██║     ██║   ██║██║  ██║██╔══╝  ╚════██║
//      ██║██║ ╚████║╚██████╗███████╗╚██████╔╝██████╔╝███████╗███████║
//      ╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝ ╚═════╝ ╚═════╝ ╚══════╝╚══════╝
#include "pch.h"
#include <iostream>
#include "MemoryTools.h"
#include "UI.h"
#include <Windows.h>
#include <thread>

//
//      ██╗   ██╗ █████╗ ██████╗ ██╗ █████╗ ██████╗ ██╗     ███████╗███████╗
//      ██║   ██║██╔══██╗██╔══██╗██║██╔══██╗██╔══██╗██║     ██╔════╝██╔════╝
//      ██║   ██║███████║██████╔╝██║███████║██████╔╝██║     █████╗  ███████╗
//      ╚██╗ ██╔╝██╔══██║██╔══██╗██║██╔══██║██╔══██╗██║     ██╔══╝  ╚════██║
//       ╚████╔╝ ██║  ██║██║  ██║██║██║  ██║██████╔╝███████╗███████╗███████║
//        ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚══════╝╚══════╝                                                                                                                                                                                                                                      
//                              - Program Settings -
using namespace std;

DWORD PROCESS_ID;
BOOL  dwFlagThreadsStatus = true;	// Flag de Status de Threads (TRUE -> ResumeThreads | FALSE -> SuspendThreads)

#pragma region Hotkeys
void HotkeySuspend() {
	bool CheckPress;
	while (true) {

		if (GetAsyncKeyState(VK_SHIFT) < 0 && GetAsyncKeyState(VK_F1) < 0) {
			if (CheckPress == true) {
				CheckPress = false;
				if (dwFlagThreadsStatus == true) {
					SuspendThreads(PROCESS_ID);
					dwFlagThreadsStatus = false;
				}
			}
		}
		else
		{
			CheckPress = true;
		}
	}
}
void HotkeyResume() {
	bool CheckPress;
	while (true) {

		if (GetAsyncKeyState(VK_SHIFT) < 0 && GetAsyncKeyState(VK_F2) < 0) {
			if (CheckPress == true) {
				CheckPress = false;
				if (dwFlagThreadsStatus == false) {
					ResumeThreads(PROCESS_ID);
					dwFlagThreadsStatus = true;
				}
			}
		}
		else
		{
			CheckPress = true;
		}
	}
}
void HotkeyPress() {
	bool CheckPress;
	while (true) {

		if (GetAsyncKeyState(VK_SHIFT) < 0) {
			if (CheckPress == true) {
				CheckPress = false;
				// Codigo aqui ...
			}
		}
		else
		{
			CheckPress = true;
		}
	}
}

// Comandos de Hotkeys
void SuspendThread() {
	std::thread t1(HotkeySuspend); // Executa Thread
	t1.detach(); // desanexar t1 do thread principal
}
void ResumeThread() {
	std::thread t1(HotkeyResume); // Executa Thread
	t1.detach(); // desanexar t1 do thread principal
}
void HotkeyPressThread() {
	std::thread t1(HotkeyPress); // Executa Thread
	t1.detach(); // desanexar t1 do thread principal
}
#pragma endregion

int main()
{
	SetTitle_Dois();
	


	/*
	// Inicia o MemoryTools
	const char PROCESS_NAME[] = "Gw2-64.exe";
	PROCESS_ID = GetProcessID(PROCESS_NAME);
	printf("Proces Name: %s\t\tPID: %d\n", PROCESS_NAME, PROCESS_ID);

	printf("• Listagem de processos em execução no sistema\n");
	ListProcess();

	// Inicia Hotkeys
	SuspendThread();
	ResumeThread();

	// Leitura
	uintptr_t endereco = GetModuleBaseAddress(PROCESS_ID, "Gw2-64.exe") + 0x1E47E54;
	printf("Endereco: %I64X\n", endereco);
	printf("Valor:    %d\n", ReadProcess<uintptr_t>(PROCESS_ID, endereco));

	// Escrita
	WriteProcess(PROCESS_ID, endereco, 77777); // Escreve
	printf("Valor agora: %d\n", ReadProcess<uintptr_t>(PROCESS_ID, endereco));

	// Ler Ponteiro
	vector<uintptr_t> ponteiro_offsets = { 0x1A0, 0xD0 };
	uintptr_t ponteiro = GetPointerBaseAddress(PROCESS_ID, "Gw2-64.exe", 0x01D9B9D0, ponteiro_offsets);
	printf("Ponteiro Endereço: %I64X\n", ponteiro);
	*/

	getchar();
}