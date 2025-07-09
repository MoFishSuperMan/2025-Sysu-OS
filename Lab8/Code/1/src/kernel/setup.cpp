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
// lab8
#include "syscall.h"

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

// lab8
// 系统调用
SystemService systemService;
int syscall_0(int first, int second, int third, int forth, int fifth)
{
    printf("systerm call 0: %d, %d, %d, %d, %d\n",
           first, second, third, forth, fifth);
    return first + second + third + forth + fifth;
}

// lab7

void first_thread(void *arg)
{
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

    // lab8
    // 初始化系统调用
    systemService.initialize();
    // 设置0号系统调用
    systemService.setSystemCall(0, (int)syscall_0);

    int ret;
    
    ret = asm_system_call(0);
    printf("return value: %d\n", ret);

    ret = asm_system_call(0, 123);
    printf("return value: %d\n", ret);

    ret = asm_system_call(0, 123, 324);
    printf("return value: %d\n", ret);

    ret = asm_system_call(0, 123, 324, 9248);
    printf("return value: %d\n", ret);

    ret = asm_system_call(0, 123, 324, 9248, 7);
    printf("return value: %d\n", ret);

    ret = asm_system_call(0, 123, 324, 9248, 7, 123);
    printf("return value: %d\n", ret);
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
