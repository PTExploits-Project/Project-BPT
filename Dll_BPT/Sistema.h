#include <Windows.h>
#include <winhttp.h>
#include <regex>

#pragma comment(lib, "winhttp.lib")

//WMI
#include <Wbemidl.h>
#include <comdef.h>
#include <vector>

#pragma comment(lib, "wbemuuid.lib")

//Discord Webhook
#define CURL_STATICLIB

#include <nlohmann/json.hpp>
#include <curl/curl.h>

#pragma comment (lib, "libcurl_a.lib")
#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")
//

//_CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

extern void lol(void* pFunction, bool bLoop);
extern int read(DWORD src, int len);
extern std::string readString(DWORD src, int len);
extern int menu();
extern void hooks();
extern void findMob();
extern void disableProtectMouse();

unsigned long djb2Hash(const std::string& str);
bool bCheckRegister();
void protectMachine();

std::tm GetTime(const std::string& date, const std::string& time);
int Tmain();
void hHora();

HRESULT initializeCOM();
void uninitializeCOM();
HRESULT connectToWMI(IWbemLocator** pLoc, IWbemServices** pSvc);
HRESULT executeWMIQuery(IWbemServices* pSvc, const std::wstring& query, IEnumWbemClassObject** pEnumerator);

std::string getComputerNameWMI(IWbemServices* pSvc);
std::string getPrimaryMACAddressWMI(IWbemServices* pSvc);
std::string getProcessorId(IWbemServices* pSvc);
std::string getProcessorName(IWbemServices* pSvc);
std::string getBiosSerialNumber(IWbemServices* pSvc);
std::string getDiskDriveSerialNumber(IWbemServices* pSvc);

size_t callback(const char* in, size_t size, size_t num, std::string* out);
std::string getPublicIP();

void sendDiscordWebhook(std::string szMessage);

//Registro
std::string expireDate = "2024-01-13";
std::string expireTime = "23:59:00";

std::string expireDate_[] = {
    "2024-01-22",       //dwSize
    "2024-01-17",       //Gui
    "2024-01-17",       //Bless
    "2024-01-17",       //BarronT
    "2024-01-17",       //Lucas Gota
    "2024-01-17"        //blackout
};

// vetor de cadastro de serial
unsigned long dwSerialHD_Client[] = {
    47988480,           //dwSize
    2010330067,         //Gui
    1015480953,         //Bless
    1368213801,         //BarronT
    3418221424,         //Lucas Gota
    3172673466          //Blackout
};

std::string dwNome;

std::string dwNome_[] = {
    "dwSize - Leo",
    "Gui",
    "Blessthevi",
    "BarronT",
    "Lucas Gota",
    "Blackout"
};

//Fim registro

//Var
bool bDiscord = false, bLogin = false;
std::string dwLogin, dwPassword;

std::string biosSerialNumber;
std::string diskSerialNumber;

HRESULT hres = NULL;
IWbemLocator* pLoc = NULL;
IWbemServices* pSvc = NULL;

//