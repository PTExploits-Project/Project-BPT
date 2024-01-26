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
		iHookMp = read(0x004E1C64 + 0x1, 4);

		//Salto
		iHookMp += 0x004E1C64 + 5;

		//Ler o valor do segundo endereço - Nesse cara que eu vou fazer o hook para minha função
		iHook2Mp = read(iHookMp + 0x12, 4);

		int jmpMp = (DWORD)travaMp;
		write(iHook2Mp, &jmpMp, 4);

		//-------------- Trava Stm -------------------//

		int iHookStm = 0, iHook2Stm = 0;

		//Ler o valor do endereço
		iHookStm = read(0x004E1CA9 + 0x1, 4);

		//Salto
		iHookStm += 0x004E1CA9 + 5;

		//Ler o valor do segundo endereço - Nesse cara que eu vou fazer o hook para minha função
		iHook2Stm = read(iHookStm + 0x12, 4);

		int jmpStm = (DWORD)travaStm;
		write(iHook2Stm, &jmpStm, 4);

		ExitThread(0);
	}
}