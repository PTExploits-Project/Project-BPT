#include "Hook.h"

void hooks() {
	while (true) {
		//-------------- Trava Hp -------------------//
		int iHookHp = 0, iHook2Hp = 0;

		//Ler o valor do endereço
		iHookHp = read(0x0041FB62, 4);

		//Salto
		iHookHp += 0x0041FB61 + 5;

		//Ler o valor do segundo endereço - Nesse cara que eu vou fazer o hook para minha função
		iHook2Hp = read(iHookHp + 0xAC, 4);

		int jmpHp = (DWORD)travaHp;
		write(iHook2Hp, &jmpHp, 4);

		//-------------- Trava Mp -------------------//

		int iHookMp = 0, iHook2Mp = 0;

		//Ler o valor do endereço
		iHookMp = read(0x004E1C65, 4);

		//Salto
		iHookMp += 0x004E1C64 + 5;

		//Ler o valor do segundo endereço - Nesse cara que eu vou fazer o hook para minha função
		iHook2Mp = read(iHookMp + 0x12, 4);

		int jmpMp = (DWORD)travaMp;
		write(iHook2Mp, &jmpMp, 4);

		ExitThread(0);
	}
}