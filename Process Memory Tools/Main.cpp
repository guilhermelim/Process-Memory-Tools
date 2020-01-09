//	Main.cpp : define o ponto de entrada para o aplicativo do console.
#include "pch.h"
#include "../MemoryTools/MemoryTools.cpp"

using namespace std;

int main()
{
	//MemoryTools::EscreverA();
	//MemoryTools::address.EscreverB();
	DWORD ProcessID = MemoryTools::GetProcessID("Gw2-64.exe");
	HANDLE myHandle = MemoryTools::GetProcessHandle(ProcessID);
	
	printf("Process PID: %d\n", ProcessID);
	printf("Module Base Anddress: %I64X\n", MemoryTools::module.GetBaseAddress(ProcessID, "Gw2-64.exe"));
	printf("Module Base Size: %d\n", (int)MemoryTools::module.GetBaseSize(ProcessID, "Gw2-64.exe"));
	
	uintptr_t flyMode_adr = MemoryTools::module.GetBaseAddress(ProcessID, "Gw2-64.exe") + 0x1376953;
	printf("Module + Point: %I64X\n", flyMode_adr);

	uintptr_t coordenadasDoPlayerX_adr = MemoryTools::address.GetPointerBaseAddress(ProcessID, "Gw2-64.exe", 0x0219BD68, { 0xB0, 0x8, 0x40, 0xE0, 0x120 });
	printf("Pointer Base Address: %I64X\n", coordenadasDoPlayerX_adr);
	
	MemoryTools::thread.ListThreadsStartAddress(ProcessID);
	
	system("pause");
	return 0;
}