#include "Game.h"

int menu() {
	while (true) {
		Sleep(10);

		if (rsCompString((char*)"/nohpon"))
			bNoHp = true;

		if (rsCompString((char*)"/nohpoff"))
			bNoHp = false;

		if (rsCompString((char*)"/autoboton"))
			bAutoBot = true;

		if (rsCompString((char*)"/autobotoff"))
			bAutoBot = false;
	}

	return 0;
}