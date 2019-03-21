//	Main.cpp : define o ponto de entrada para o aplicativo do console.
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
//		• Suporte Nativo a processos x86 e x64 (automatic)
//		• Listagem de Processos em execução no sistema					- ListProcess();
//		• Listagem de Modulos de um Processo especificado				- ListProcessModules(DWORD _ProcessID)
//		• Listagem de Threads de um Processo especificado				- ListThreads(DWORD _ProcessID)
//		• Get ID de um Processo especificado (Comumente chamado de PID)	- GetProcessID(const char _ProcessName[])
//		• Get Handle de um Processo especificado						- GetProcessHandle(DWORD _ProcessID)
//		• Get base address de um Modulo especificado					- GetModuleBaseAddress(DWORD _ProcessID, const char _ModuleName[])
//		• Get tamanho de um Modulo especificado							- GetModuleBaseSize(DWORD _ProcessID, const char _ModuleName[])
//		• Get base address de um Ponteiro especificado					- GetPointerBaseAddress(DWORD _ProcessID, HANDLE _ProcessHandle, const char _MooduleBase[], uintptr_t _Pointer, vector<uintptr_t> &_Offsets)
//																			Obs: Você pode escolher utilizar como parametro: _ProcessID ou _ProcessHandle do processo.
//		• Suspende a execução das Threads de um Processo especificado	- void SuspendThreads(DWORD _ProcessID)
//		• Retoma a execução das Threads de um Processo especificado		- ResumeThreads(DWORD _ProcessID)
//		• Read Process Memory v1.0										- ReadProcess(DWORD _ProcessID, uintptr_t _BaseAddress) OR ReadProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress) 
//																			Obs: Você pode escolher utilizar como parametro: _ProcessID ou _ProcessHandle do processo.
//		• Write Process Memory v1.0										- WriteProcess(DWORD _ProcessID, uintptr_t _BaseAddress, uintptr_t _buffer) OR WriteProcess(HANDLE _ProcessHandle, uintptr_t _BaseAddress, uintptr_t _buffer)
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
#include "UI.h"
#include "MemoryTools.h"

//
//      ██╗   ██╗ █████╗ ██████╗ ██╗ █████╗ ██████╗ ██╗     ███████╗███████╗
//      ██║   ██║██╔══██╗██╔══██╗██║██╔══██╗██╔══██╗██║     ██╔════╝██╔════╝
//      ██║   ██║███████║██████╔╝██║███████║██████╔╝██║     █████╗  ███████╗
//      ╚██╗ ██╔╝██╔══██║██╔══██╗██║██╔══██║██╔══██╗██║     ██╔══╝  ╚════██║
//       ╚████╔╝ ██║  ██║██║  ██║██║██║  ██║██████╔╝███████╗███████╗███████║
//        ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚══════╝╚══════╝                                                                                                                                                                                                                                      
//                              - Program Settings -
using namespace std;

DWORD PROCESS_ID;					// Está variável é responsável por armazenar o ID do Processo que você deseja acessar.
BOOL  dwFlagThreadsStatus = true;	// Flag de Status de Threads (TRUE -> ResumeThreads | FALSE -> SuspendThreads)

//
//      ██╗  ██╗ ██████╗ ████████╗██╗  ██╗███████╗██╗   ██╗███████╗
//      ██║  ██║██╔═══██╗╚══██╔══╝██║ ██╔╝██╔════╝╚██╗ ██╔╝██╔════╝
//      ███████║██║   ██║   ██║   █████╔╝ █████╗   ╚████╔╝ ███████╗
//      ██╔══██║██║   ██║   ██║   ██╔═██╗ ██╔══╝    ╚██╔╝  ╚════██║
//      ██║  ██║╚██████╔╝   ██║   ██║  ██╗███████╗   ██║   ███████║
//      ╚═╝  ╚═╝ ╚═════╝    ╚═╝   ╚═╝  ╚═╝╚══════╝   ╚═╝   ╚══════╝
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


// Endereços das coordenadas do player
uintptr_t coorX_adr;
uintptr_t coorY_adr;
uintptr_t coorZ_adr;

// Endereços das coordenadas da camera
uintptr_t cameX_adr;
uintptr_t cameY_adr;

// Função Assemble
uintptr_t assembleVoar_adr;

void GetEnderecos() {
	// Endereços das coordenadas do player
	coorX_adr = GetPointerBaseAddress(PROCESS_ID, "Gw2-64.exe", 0x02193218, { 0x48, 0x98, 0x38, 0x80, 0x120 });
	coorY_adr = GetPointerBaseAddress(PROCESS_ID, "Gw2-64.exe", 0x02193218, { 0x48, 0x98, 0x38, 0x80, 0x124 });
	coorZ_adr = GetPointerBaseAddress(PROCESS_ID, "Gw2-64.exe", 0x002193218, { 0x48, 0x98, 0x38, 0x80, 0x128 });

	// Endereços das coordenadas da camera
	cameX_adr = GetPointerBaseAddress(PROCESS_ID, "Gw2-64.exe", 0x020D4EC8, { 0x58 });
	cameY_adr = GetPointerBaseAddress(PROCESS_ID, "Gw2-64.exe", 0x020D4EC8, { 0x7C });

	// Função Assemble
	assembleVoar_adr = GetModuleBaseAddress(PROCESS_ID, "Gw2-64.exe") + 0x136FCE3;
}


int main()
{
	// Apresenta uma UI de Welcome na inicialização
	SetTitle_lithe(); // Você pode alterar o modelo de acordo com sua preferencia no header UI.h
	
	// Declarando o Process Memory Tools
	const char PROCESS_NAME[] = "Gw2-64.exe";		// Digite o nome do seu processo.
	PROCESS_ID = GetProcessID(PROCESS_NAME);	// Pega o PID do processo especificado.

	if (PROCESS_ID == 0) {						// Verifica se o processo existe.
		printf(" O processo '%s' não está em execução, abra e tente novamente!", PROCESS_NAME);
		getchar();
		return 0;								// Fecha o programa caso o processo não sejá encontrado.
	}

	printf("Processo encontrado!\nNome: %s\t\tPID: %d\n", PROCESS_NAME, PROCESS_ID);
	
	// Inicia Hotkeys Para Pausar e Retomar as Threads do processo.
	SuspendThread();
	ResumeThread();
	printf("Hotkey SuspendThread: ATIVADA\n");
	printf("Hotkey ResumeThread: ATIVADA\n\n");
	GetEnderecos();

	while (true) {
		Sleep(300);
		//			Codigo do Haker
		// Declaração de Variáveis do Hacker
		float speed = 6;
		// Calcula Inclinações e Velocidade

		if (GetAsyncKeyState(VK_SHIFT) < 0) {
			printf("Apertou SHIFT!\n");

			WriteProcess(PROCESS_ID, coorZ_adr, ReadProcess<float>(PROCESS_ID, coorZ_adr) - speed);
		}
	}

	
	getchar();									// Espera até que alguma tecla seja pressionada.
}