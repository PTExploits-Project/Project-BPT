#include "Game.h"

int menu() {
	while (true) {
		Sleep(10);

		if (rsCompString((char*)"/nohpon"))
			bNoHp = true;

		if (rsCompString((char*)"/nohpoff"))
			bNoHp = false;
	}

	return 0;
}