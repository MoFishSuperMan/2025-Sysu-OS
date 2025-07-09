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
    printf("request1: request 15900 pages----------\n");
    int addr1=memoryManager.allocatePages(AddressPoolType::KERNEL,15900);
    if(addr1){
        printf("allocate 15900 pages, start address is 0x%x\n",addr1);
    }
    printf("request2: request 128 pages----------\n");
    int addr2=memoryManager.allocatePages(AddressPoolType::KERNEL,128);
    if(addr2){
        printf("allocate 128 pages, start address is 0x%x\n",addr2);
    }
    printf("request3: request 64 pages----------\n");
    int addr3=memoryManager.allocatePages(AddressPoolType::KERNEL,64);
    if(addr3){
        printf("allocate 64 pages, start address is 0x%x\n",addr3);
    }
    printf("request4: request 64 pages----------\n");
    int addr4=memoryManager.allocatePages(AddressPoolType::KERNEL,64);
    if(addr4){
        printf("allocate 64 pages, start address is 0x%x\n",addr4);
    }
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