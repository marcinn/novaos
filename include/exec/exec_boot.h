#ifndef EXEC_BOOT_H
#define EXEC_BOOT_H

#include <exec/types.h>

int _start();
void exec_boot();
ULONG exec_CountMem();
void exec_ZeroMem(ULONG start, ULONG size);

#endif

