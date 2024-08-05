#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <string>
#include <windows.h>

// Estrutura para armazenar informa��es sobre um processo
struct ProcessInfo {
    DWORD id;
    std::string name;

    // Construtor que inicializa os membros
    ProcessInfo(DWORD id = 0, const std::string& name = "") : id(id), name(name) {}
};

// Estrutura para armazenar informa��es sobre um m�dulo
struct ModuleInfo {
    std::string name;
    uintptr_t baseAddress;
    SIZE_T baseSize;

    // Construtor que inicializa os membros
    ModuleInfo(const std::string& name = "", uintptr_t baseAddress = 0, SIZE_T baseSize = 0)
        : name(name), baseAddress(baseAddress), baseSize(baseSize) {}
};

// Fun��o para listar todos os processos em execu��o e retornar como um vetor
std::vector<ProcessInfo> listProcesses();

// Fun��o para obter o handle de um processo pelo ID
HANDLE getProcessHandleById(DWORD processId);

// Fun��o para obter o handle de um processo pelo nome
HANDLE getProcessHandleByName(const std::string& processName);

// Fun��o para obter o ID de um processo pelo nome
DWORD getProcessId(const std::string& processName);

// Fun��o para listar todos os m�dulos de um processo e retornar como um vetor
std::vector<ModuleInfo> listModules(DWORD processId);

// Fun��o para obter informa��es sobre um m�dulo espec�fico de um processo pelo nome do m�dulo
ModuleInfo getProcessModuleByName(DWORD processId, const std::string& moduleName);

// Fun��o template para ler um bloco de mem�ria de um processo
template <typename T>
T readProcess(DWORD processId, uintptr_t address) {
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, processId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process for reading. Process ID: " << processId << std::endl;
        return T(); // Retorna um valor padr�o para o tipo T
    }

    T result = T();
    SIZE_T bytesRead;
    BOOL success = ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(address), &result, sizeof(T), &bytesRead);

    if (!success || bytesRead != sizeof(T)) {
        std::cerr << "Failed to read memory. Process ID: " << processId << ", Address: " << std::hex << address << std::dec << std::endl;
        result = T(); // Retorna um valor padr�o para o tipo T
    }

    CloseHandle(hProcess);
    return result;
}

template <typename T>
T readProcess(HANDLE hProcess, uintptr_t address) {
    if (hProcess == NULL) {
        std::cerr << "Invalid process handle." << std::endl;
        return T(); // Retorna um valor padr�o para o tipo T
    }

    T result = T();
    SIZE_T bytesRead;
    BOOL success = ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(address), &result, sizeof(T), &bytesRead);

    if (!success || bytesRead != sizeof(T)) {
        std::cerr << "Failed to read memory. Address: " << std::hex << address << std::dec << std::endl;
        result = T();
    }

    return result;
}

// Fun��o template para escrever em um bloco de mem�ria de um processo
template <typename T>
bool writeProcess(DWORD processId, uintptr_t baseAddress, const T& data);

template <typename T>
bool writeProcess(DWORD processId, uintptr_t baseAddress, const T& data) {
    HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process ID: " << processId << std::endl;
        return false;
    }

    SIZE_T dataSize = sizeof(T);
    SIZE_T bytesWritten;
    BOOL success = WriteProcessMemory(hProcess, (LPVOID)baseAddress, &data, dataSize, &bytesWritten);

    if (!success || bytesWritten != dataSize) {
        std::cerr << "Failed to write memory to process ID: " << processId << std::endl;
        CloseHandle(hProcess);
        return false;
    }

    CloseHandle(hProcess);
    return true;
}

template <typename T>
bool writeProcess(HANDLE hProcess, uintptr_t baseAddress, const T& data) {
    if (hProcess == NULL) {
        std::cerr << "Invalid process handle." << std::endl;
        return false;
    }

    SIZE_T dataSize = sizeof(T);
    SIZE_T bytesWritten;
    BOOL success = WriteProcessMemory(hProcess, (LPVOID)baseAddress, &data, dataSize, &bytesWritten);

    if (!success || bytesWritten != dataSize) {
        std::cerr << "Failed to write memory. Address: " << std::hex << baseAddress << std::dec << std::endl;
        return false;
    }

    return true;
}

// Fun��o para calcular o endere�o do ponteiro com base em offsets
uintptr_t getPointerAddress(DWORD processId, const std::string& moduleName, uintptr_t baseOffset, const std::vector<uintptr_t>& offsets);
uintptr_t getPointerAddress(DWORD processId, uintptr_t address, const std::vector<uintptr_t>& offsets);

#endif // PROCESS_H
