#include "asmm.h"

__declspec(naked) void travaHp(short iVar) {
	__asm {
		cmp byte ptr ds:[bNoHp], 0
		jne trava
		mov eax, 0x4E1C0C
		jmp eax

		trava:
		ret
	}
}