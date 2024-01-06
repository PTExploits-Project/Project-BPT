#include "Hook.h"

void hooks() {
	while (true) {
		int iHook = 0, iHook2 = 0;

		//Ler o valor do endereço
		iHook = read(0x0041FB62, 4);

		//Salto
		iHook += 0x0041FB61 + 5;

		//Ler o valor do segundo endereço - Nesse cara que eu vou fazer o hook para minha função
		iHook2 = read(iHook + 0xAC, 4);

		int a = (DWORD)travaHp;
		write(iHook2, &a, 4);

		ExitThread(0);
	}
}