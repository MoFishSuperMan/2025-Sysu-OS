#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
// lab5
#include "program.h"
#include "thread.h"
// lab6
#include "sync.h"
// lab7
#include "memory.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;

// lab5
// 程序管理器
ProgramManager programManager;

// lab7
// 内存管理器
MemoryManager memoryManager;

// lab7

void first_thread(void *arg)
{
    int address=memoryManager.allocatePhysicalPages(AddressPoolType::USER,1);
    char* chs = (char*)address;
    int p = (int)chs+0x104;
    printf("the virtualAddress is 0x%x\n",chs+0x0104);
    int direct = *(PAGE_DIRECTORY + (p >> 22));
    int page = *(direct + (p >> 22)) + ((p & 0x003) >> 12);
    int physicaladdress= direct | page | (p & 0x00000111);
    printf("the physicalAddress is 0x%x\n",physicaladdress);
    asm_halt();
}

extern "C" void setup_kernel()
{
    // 中断处理部件
    interruptManager.initialize();
    interruptManager.enableTimeInterrupt();
    interruptManager.setTimeInterrupt((void *)asm_time_interrupt_handler);

    // 屏幕IO处理部件
    stdio.initialize();

    //lab5
    // 进程/线程管理器
    programManager.initialize();

    // lab7
    // 内存管理器
    memoryManager.openPageMechanism();
    memoryManager.initialize();

    // 创建第一个线程
    int pid = programManager.executeThread(first_thread, nullptr, "first thread", 1);
    if (pid == -1)
    {
        printf("can not execute thread\n");
        asm_halt();
    }

    ListItem *item = programManager.readyPrograms.front();
    PCB *firstThread = ListItem2PCB(item, tagInGeneralList);
    firstThread->status = RUNNING;
    programManager.readyPrograms.pop_front();
    programManager.running = firstThread;
    asm_switch_thread(0, firstThread);
    asm_halt();
}