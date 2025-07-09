#ifndef OS_MODULES_H
#define OS_MODULES_H

#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "memory.h"
#include "syscall.h"

extern InterruptManager interruptManager;
extern STDIO stdio;
extern ProgramManager programManager;
// lab8
extern SystemService systemService;

#endif
