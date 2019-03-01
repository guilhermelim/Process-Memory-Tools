#pragma once
#include <Windows.h>
#include <iostream>

/*
	Você pode criar modelos personalizados de UI através do site http://patorjk.com/software/taag.
	Recomendo utilizar as seguintes configurações:
	Font: ANSI Shadow
	Character Width: Full
	Character Height: Full
	Link de Test: http://patorjk.com/software/taag/#p=display&h=0&v=0&f=ANSI%20Shadow&t=TEST

*/

void SetTitle_lithe() {
	SetConsoleTitle("Process Memory Tools C++ - Family { CODE BUILDING }");
	system("color 0A");

	printf("\n");
	printf("       ██████╗ ██████╗ ██████╗ ███████╗    ██████╗ ██╗   ██╗██╗██╗     ██████╗ ██╗███╗   ██╗ ██████╗ \n");
	printf("      ██╔════╝██╔═══██╗██╔══██╗██╔════╝    ██╔══██╗██║   ██║██║██║     ██╔══██╗██║████╗  ██║██╔════╝ \n");
	printf("      ██║     ██║   ██║██║  ██║█████╗      ██████╔╝██║   ██║██║██║     ██║  ██║██║██╔██╗ ██║██║  ███╗\n");
	printf("      ██║     ██║   ██║██║  ██║██╔══╝      ██╔══██╗██║   ██║██║██║     ██║  ██║██║██║╚██╗██║██║   ██║\n");
	printf("      ╚██████╗╚██████╔╝██████╔╝███████╗    ██████╔╝╚██████╔╝██║███████╗██████╔╝██║██║ ╚████║╚██████╔╝\n");
	printf("       ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝    ╚═════╝  ╚═════╝ ╚═╝╚══════╝╚═════╝ ╚═╝╚═╝  ╚═══╝ ╚═════╝ \n");
	printf("                            Process Memory Tools C++ - Family { CODE BUILDING }\n");
	printf("\n");
}

void SetTitle() {
	SetConsoleTitle("Process Memory Tools C++ - Family { CODE BUILDING }");
	system("color 0A");
	printf("\n");
	printf("       ██████╗ ██████╗ ██████╗ ███████╗    ██████╗ ██╗   ██╗██╗██╗     ██████╗ ██╗███╗   ██╗ ██████╗ \n");
	printf("      ██╔════╝██╔═══██╗██╔══██╗██╔════╝    ██╔══██╗██║   ██║██║██║     ██╔══██╗██║████╗  ██║██╔════╝ \n");
	printf("      ██║     ██║   ██║██║  ██║█████╗      ██████╔╝██║   ██║██║██║     ██║  ██║██║██╔██╗ ██║██║  ███╗\n");
	printf("      ██║     ██║   ██║██║  ██║██╔══╝      ██╔══██╗██║   ██║██║██║     ██║  ██║██║██║╚██╗██║██║   ██║\n");
	printf("      ╚██████╗╚██████╔╝██████╔╝███████╗    ██████╔╝╚██████╔╝██║███████╗██████╔╝██║██║ ╚████║╚██████╔╝\n");
	printf("       ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝    ╚═════╝  ╚═════╝ ╚═╝╚══════╝╚═════╝ ╚═╝╚═╝  ╚═══╝ ╚═════╝ \n");
	printf("                            Process Memory Tools C++ - Family { CODE BUILDING }\n");
	printf("\n");
	printf("CRÉDITOS:\n");
	printf("Programador			-> Guilherme Lima\n");
	printf("Mentor				-> SmoLL_iCe\n");
	printf("Solucionador de dúvidas e erros -> Define, Devecchi, Luan Devecchi, EuPoder, Bolard, Walison.\n");
	printf("\n");
	printf("FEATURES\n");
	printf(" • Listagem de processos em execução no sistema\n");
	printf(" • Listagem de Modulos em execução de um Processo\n");
	printf(" • Listagem de Threads em execução de um Processo especificado\n");
	printf(" • Get ID do Processo especificado\n");
	printf(" • Get Handle Process\n");
	printf(" • Get base address de um Modulo especificado\n");
	printf(" • Get tamanho de um Modulo especificado\n");
	printf(" • Get base address de um Ponteiro especificado\n");
	printf(" • Suspende a execução das Threads de um Processo especificado\n");
	printf(" • Retoma a execução das Threads de um Processo especificado\n");
	printf(" • Read Process Memory\n");
	printf(" • Write Process Memory\n");
}
