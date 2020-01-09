#pragma once
#include "List.h"
#include "Module.h"
#include "Address.h"
#include "Out.h"
#include "In.h"
#include "Thread.h"

class MemoryTools
{
public:
	static void EscreverA();
	//	Get ID do Processo especificado
	static DWORD GetProcessID(const char _ProcessName[]);
	//	Get Handle do Processo especificado
	static HANDLE GetProcessHandle(DWORD _ProcessID);

	static List list;
	static Module module;
	static Address address;
	static In in;
	static Out out;
	static Thread thread;
};
