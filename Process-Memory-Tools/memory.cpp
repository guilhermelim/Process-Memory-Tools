#include <iostream>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <string>

#include "memory.h"
#include "Converter.h"

std::vector<ProcessInfo> listProcesses() {
    std::vector<ProcessInfo> processes;
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32{};

    // Criar um "snapshot" de todos os processos
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to take snapshot of processes." << std::endl;
        return processes;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Recuperar informações do primeiro processo
    if (!Process32First(hProcessSnap, &pe32)) {
        std::cerr << "Failed to retrieve process information." << std::endl;
        CloseHandle(hProcessSnap);
        return processes;
    }

    // Adicionar processos ao vetor
    do {
        ProcessInfo pInfo;
        pInfo.id = pe32.th32ProcessID;
        pInfo.name = Converter::toString(pe32.szExeFile);
       
        processes.push_back(pInfo);
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return processes;
}

HANDLE getProcessHandleById(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == nullptr) {
        std::cerr << "Failed to open process ID: " << processId << std::endl;
    }
    return hProcess;
}

HANDLE getProcessHandleByName(const std::string& processName) {
    std::vector<ProcessInfo> processes = listProcesses();
    for (const auto& process : processes) {
        if (process.name == processName) {
            return getProcessHandleById(process.id);
        }
    }
    std::cerr << "Process not found: " << processName << std::endl;
    return nullptr;
}

DWORD getProcessId(const std::string& processName) {
    std::vector<ProcessInfo> processes = listProcesses();
    for (const auto& process : processes) {
        if (process.name == processName) {
            return process.id;
        }
    }
    std::cerr << "Process not found: " << processName << std::endl;
    return 0;
}

std::vector<ModuleInfo> listModules(DWORD processId) {
    std::vector<ModuleInfo> modules;
    HANDLE hModuleSnap;
    MODULEENTRY32 me32;

    // Criar um "snapshot" de todos os módulos do processo
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (hModuleSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to take snapshot of modules." << std::endl;
        return modules;
    }

    me32.dwSize = sizeof(MODULEENTRY32);

    // Recuperar informações do primeiro módulo
    if (!Module32First(hModuleSnap, &me32)) {
        std::cerr << "Failed to retrieve module information." << std::endl;
        CloseHandle(hModuleSnap);
        return modules;
    }

    // Adicionar módulos ao vetor
    do {
        ModuleInfo mInfo;
        mInfo.name = Converter::toString(me32.szModule);
        mInfo.baseAddress = reinterpret_cast<uintptr_t>(me32.modBaseAddr);
        mInfo.baseSize = me32.modBaseSize;

        modules.push_back(mInfo);
    } while (Module32Next(hModuleSnap, &me32));

    CloseHandle(hModuleSnap);
    return modules;
}

ModuleInfo getProcessModuleByName(DWORD processId, const std::string& moduleName) {
    std::vector<ModuleInfo> modules = listModules(processId);
    for (const auto& module : modules) {
        if (module.name == moduleName) {
            return module;
        }
    }
    // Se o módulo não for encontrado, retornamos um ModuleInfo com valores padrão
    return ModuleInfo();
}

uintptr_t getPointerAddress(DWORD processId, const std::string& moduleName, uintptr_t baseOffset, const std::vector<uintptr_t>& offsets) {
    // Obter o módulo base
    ModuleInfo module = getProcessModuleByName(processId, moduleName);
    if (module.name.empty()) {
        std::cerr << "Module " << moduleName << " not found in process with ID " << processId << std::endl;
        return 0;
    }

    // Calcular o endereço base
    uintptr_t address = module.baseAddress + baseOffset;

    // Iterar sobre os offsets para calcular o endereço final
    for (const auto& offset : offsets) {
        address = readProcess<uintptr_t>(processId, address);
        if (address == 0) {
            std::cerr << "Failed to read memory at address " << std::hex << address << std::endl;
            return 0;
        }
        address += offset;
    }

    return address;
}

uintptr_t getPointerAddress(DWORD processId, uintptr_t address, const std::vector<uintptr_t>& offsets) {
    // Iterar sobre os offsets para calcular o endereço final
    for (const auto& offset : offsets) {
        address = readProcess<uintptr_t>(processId, address);
        if (address == 0) {
            std::cerr << "Failed to read memory at address " << std::hex << address << std::endl;
            return 0;
        }
        address += offset;
    }

    return address;
}

