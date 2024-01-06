#include <Windows.h>
#include "asmm.h"

extern void write(DWORD dst, void* value, int len);
extern int read(DWORD src, int len);

void hooks();