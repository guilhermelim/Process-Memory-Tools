#include <iostream>
#include <thread>  // Inclua este cabeçalho para std::this_thread
#include <chrono>  // Inclua este cabeçalho para std::chrono
#include "memory.h"

int main() {
	std::vector<ProcessInfo> processes = listProcesses();

	std::cout << "Listing all processes:" << std::endl;
	for (const auto& process : processes) {
		std::cout << "Process ID: " << process.id
			<< ", Name: " << process.name << std::endl;
	}

	std::string processName = "Notepad.exe";
	HANDLE hProcess = getProcessHandleByName(processName);
	DWORD idProcess = getProcessId(processName);

	if (hProcess != nullptr) {
		std::cout << "Handle for process " << processName << ": " << hProcess << std::endl;
		CloseHandle(hProcess);
	}
	else {
		std::cout << "Failed to obtain handle for process " << processName << std::endl;
	}

	if (idProcess != 0) {
		std::cout << "Id for process " << processName << ": " << idProcess << std::endl;

		// Listar e exibir módulos do processo
		std::vector<ModuleInfo> modules = listModules(idProcess);
		std::cout << "Listing modules for process " << processName << ":" << std::endl;
		for (const auto& module : modules) {
			std::cout << "Module Name: " << module.name
				<< ", Base Address: " << std::hex << module.baseAddress
				<< ", Base Size: " << std::dec << module.baseSize << " bytes" << std::endl;
		}

		// Buscar um módulo específico
		std::string moduleName = "ntdll.dll"; // Nome do módulo que você quer obter informações
		ModuleInfo moduleInfo = getProcessModuleByName(idProcess, moduleName);
		if (!moduleInfo.name.empty()) {
			std::cout << "Module Name: " << moduleInfo.name
				<< ", Base Address: " << std::hex << moduleInfo.baseAddress
				<< ", Base Size: " << std::dec << moduleInfo.baseSize << " bytes" << std::endl;
		}
		else {
			std::cout << "Module " << moduleName << " not found in process " << processName << std::endl;
		}

		// Exemplo de leitura de um valor inteiro de 4 bytes da memória do processo		
		uintptr_t address = 0xF0AF3E9880; // Endereço real deve ser especificado		
		int value = readProcess<int>(idProcess, address);
		std::cout << "Value read from process memory at address " << std::hex << address
			<< ": " << std::dec << value << std::endl;

		// Exemplo de leitura de um valor de ponto flutuante de 4 bytes da memória do processo
		float floatValue = readProcess<float>(idProcess, 0x20394352F10);
		std::cout << "Float value read from process memory at address " << std::hex << address << ": " << floatValue << std::endl;

		// Exemplo de escrita de um valor inteiro de 4 bytes na memória do processo
		int newValue = 10;
		bool writeSuccess = writeProcess(idProcess, address, newValue);
		if (writeSuccess) {
			std::cout << "Successfully wrote value " << newValue << " to address " << std::hex << address << std::endl;
		}
		else {
			std::cout << "Failed to write value to address " << std::hex << address << std::endl;
		}

		// Exemplo de escrita de um array de bytes (hex) na memória do processo
		BYTE nop[] = { 0x90, 0x90, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x8B, 0x6E };
		writeSuccess = writeProcess(idProcess, address, nop);
		if (writeSuccess) {
			std::cout << "Successfully wrote nop array to address " << std::hex << address << std::endl;
		}
		else {
			std::cout << "Failed to write nop array to address " << std::hex << address << std::endl;
		}

		// Esperar 5 segundos antes de restaurar o valor original
		std::this_thread::sleep_for(std::chrono::seconds(5));

		BYTE restauraNop[] = { 0x0A, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x8B, 0x6E };
		writeSuccess = writeProcess(idProcess, address, restauraNop);
		if (writeSuccess) {
			std::cout << "Successfully restored nop array at address " << std::hex << address << std::endl;
		}
		else {
			std::cout << "Failed to restore nop array at address " << std::hex << address << std::endl;
		}

		// Exemplo de uso do getPointerAddress
		std::vector<uintptr_t> offsets = { 0xA8, 0x20, 0x38, 0x260, 0x218, 0xC80, 0x7B0 };
		uintptr_t pointerAddress = getPointerAddress(idProcess, "Microsoft.UI.Xaml.dll", 0x00583410, offsets);
		std::cout << "Pointer address calculated: " << std::hex << std::uppercase << pointerAddress << std::endl;

		// Exemplo de uso de getPointerAddress com endereço direto
		ModuleInfo moduleTest = getProcessModuleByName(idProcess, "Microsoft.UI.Xaml.dll");
		if (moduleTest.name.empty()) {
			std::cerr << "Module Microsoft.UI.Xaml.dll not found in process with ID " << idProcess << std::endl;
			return 0;
		}
		uintptr_t baseAddressTest = moduleTest.baseAddress + 0x00583410;
		uintptr_t pointerAddressWithAddress = getPointerAddress(idProcess, baseAddressTest, { 0xA8, 0x20, 0x38, 0x260, 0x218, 0xC80, 0x7B0 });
		std::cout << "Pointer address calculated with address: " << std::hex << std::uppercase << pointerAddressWithAddress << std::endl;
	}
	else {
		std::cout << "Failed to obtain id for process " << processName << std::endl;
	}

	return 0;
}
