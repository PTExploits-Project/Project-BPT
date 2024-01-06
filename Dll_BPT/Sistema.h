#include <Windows.h>
#include <winhttp.h>
#include <regex>

//_CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#pragma comment(lib, "winhttp.lib")

std::string expireDate = "2024-01-08";
std::string expireTime = "23:59:00";

std::tm GetTime(const std::string& date, const std::string& time);
int Tmain();
void hHora();