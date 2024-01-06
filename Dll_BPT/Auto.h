#include <Windows.h>

extern void write(DWORD dst, void* value, int len);
extern int read(DWORD src, int len);

extern bool bAutoBot;

void findMob();