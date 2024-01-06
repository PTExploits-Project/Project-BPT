#include "Sistema.h"

std::string sDate, sHour;

std::tm GetTime(const std::string& date, const std::string& time) {
	std::tm tm = { 0 };
	std::sscanf(date.c_str(), "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
	std::sscanf(time.c_str(), "%d:%d:%d", &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
	tm.tm_year -= 1900; // Ajuste do ano
	tm.tm_mon -= 1;     // Ajuste do mês
	return tm;
}

int Tmain() {
	HINTERNET hSession = WinHttpOpen(L"Test", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (!hSession) {
		//std::cerr << "Erro ao abrir a sessão WinHTTP." << std::endl;
		return -1;
	}

	HINTERNET hConnect = WinHttpConnect(hSession, L"worldtimeapi.org", INTERNET_DEFAULT_HTTPS_PORT, 0);
	if (!hConnect) {
		//std::cerr << "Erro ao conectar ao servidor." << std::endl;
		WinHttpCloseHandle(hSession);
		return -2;
	}

	HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/api/timezone/America/Sao_Paulo", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	if (!hRequest) {
		//std::cerr << "Erro ao abrir o pedido HTTP." << std::endl;
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return -3;
	}

	if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
		//std::cerr << "Erro ao enviar a solicitação HTTP." << std::endl;
		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return -4;
	}

	if (!WinHttpReceiveResponse(hRequest, NULL)) {
		//std::cerr << "Erro ao receber a resposta HTTP." << std::endl;
		WinHttpCloseHandle(hRequest);
		WinHttpCloseHandle(hConnect);
		WinHttpCloseHandle(hSession);
		return -5;
	}

	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;

	std::string datetimeString;
	do {
		dwSize = 0;
		if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
			//std::cerr << "Erro ao consultar os dados disponíveis." << std::endl;
			WinHttpCloseHandle(hRequest);
			WinHttpCloseHandle(hConnect);
			WinHttpCloseHandle(hSession);
			return -6;
		}

		pszOutBuffer = new char[dwSize + 1];
		if (!pszOutBuffer) {
			//std::cerr << "Erro ao alocar memória." << std::endl;
			WinHttpCloseHandle(hRequest);
			WinHttpCloseHandle(hConnect);
			WinHttpCloseHandle(hSession);
			return -7;
		}

		ZeroMemory(pszOutBuffer, dwSize + 1);

		if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
			//std::cerr << "Erro ao ler os dados." << std::endl;
			delete[] pszOutBuffer;
			WinHttpCloseHandle(hRequest);
			WinHttpCloseHandle(hConnect);
			WinHttpCloseHandle(hSession);
			return -8;
		}

		std::string response(pszOutBuffer);

		std::regex datetimeRegex("\"datetime\":\"(.*?)\"");
		std::smatch match;

		if (std::regex_search(response, match, datetimeRegex) && match.size() > 1) {
			datetimeString = match.str(1);
		}

		delete[] pszOutBuffer;
	} while (dwSize > 0);

	// Extrair data (ano, mês, dia)
	sDate = datetimeString.substr(0, 10);

	// Extrair hora (hora, minuto, segundo)
	sHour = datetimeString.substr(11, 8);

	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);

	return 0;
}

void hHora() {
	while (true) {
		Sleep(800);

		if (!Tmain()) {
			// Convertendo as strings de data e hora para std::tm
			std::tm current = GetTime(sDate, sHour);
			std::tm expire = GetTime(expireDate, expireTime);

			// Convertendo std::tm para tempo em segundos
			std::time_t currentSeconds = std::mktime(&current);
			std::time_t expireSeconds = std::mktime(&expire);

			// Calculando a diferença de tempo em segundos
			std::time_t timeDifference = expireSeconds - currentSeconds;

			// Calculando dias, horas, minutos e segundos da diferença
			int days = timeDifference / (24 * 3600);
			int hours = (timeDifference % (24 * 3600)) / 3600;
			int minutes = (timeDifference % 3600) / 60;
			int seconds = timeDifference % 60;

			// Convertendo horas, minutos e segundos para strings no formato "HH:MM:SS"
			std::string strDays = std::to_string(days);
			std::string strHours = std::to_string(hours);
			std::string strMinutes = std::to_string(minutes);
			std::string strSeconds = std::to_string(seconds);

			// Adicionando zero à esquerda, se necessário
			if (strDays.size() == 1) strDays = "0" + strDays;
			if (strHours.size() == 1) strHours = "0" + strHours;
			if (strMinutes.size() == 1) strMinutes = "0" + strMinutes;
			if (strSeconds.size() == 1) strSeconds = "0" + strSeconds;

			//

			if (timeDifference <= 0) {
				Beep(500, 500);

				MessageBoxA(NULL, "Tempo de uso do cheat expirado! \n\nPor favor, contate o administrador: dwSize", "RIP", MB_ICONWARNING | MB_OK);
				ExitProcess(0);
			}

			/*std::string remainingTime = "Tempo restante: " + strDays + ":" + strHours + ":" + strMinutes + ":" + strSeconds;

			System::String^ str = msclr::interop::marshal_as<System::String^>(remainingTime);
			label2->Text = str;*/
		}
	}
}