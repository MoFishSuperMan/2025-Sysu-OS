#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
// lab5
#include "program.h"
#include "thread.h"
// lab6
#include "sync.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
//lab5
// 程序管理器
ProgramManager programManager;


// lab7

void first_thread(void *arg)
{
    // 第1个线程不可以返回
    stdio.moveCursor(0);
    for (int i = 0; i < 25 * 80; ++i)
    {
        stdio.print(' ');
    }
    stdio.moveCursor(0);

    int memory = *((uint32 *)MEMORY_SIZE_ADDRESS);
    // ax寄存器保存的内容
    int low = memory & 0xffff;
    // bx寄存器保存的内容
    int high = (memory >> 16) & 0xffff;
    memory = low * 1024 + high * 64 * 1024;
    printf("total memory: %d bytes (%d MB)\n", memory, memory / 1024 / 1024);

    asm_halt();
}

//lab6
/*
Semaphore semaphore;

int cheese_burger;

void a_mother(void *arg)
{
    semaphore.P();

    int delay = 0;

    printf("Ltx23320104: start to make cheese burger, there are %d cheese burger now\n", cheese_burger);
    // make 10 cheese_burger
    cheese_burger += 10;

    printf("Ltx23320104: oh, I have to hang clothes out.\n");

    // hanging clothes out
    delay = 0xfffffff;
    while (delay)
        --delay;
    // done

    printf("Ltx23320104: Oh, Jesus! There are %d cheese burgers\n", cheese_burger);

    semaphore.V();
}

void a_naughty_boy(void *arg)
{
    semaphore.P();
    printf("boy   : Look what I found!\n");
    // eat all cheese_burgers out secretly
    cheese_burger -= 10;
    // run away as fast as possible
    semaphore.V();
}

void first_thread(void *arg)
{
    // 第1个线程不可以返回
    stdio.moveCursor(0);
    for (int i = 0; i < 25 * 80; ++i)
    {
        stdio.print(' ');
    }
    stdio.moveCursor(0);

    cheese_burger = 0;
    semaphore.initialize(1);
    
    programManager.executeThread(a_mother, nullptr, "second thread", 1);
    programManager.executeThread(a_naughty_boy, nullptr, "third thread", 1);

    asm_halt();
}
*/
//lab5
/*
void third_thread(void *arg) {
    printf("pid %d name \"%s\": Hello World!\n", programManager.running->pid, programManager.running->name);
    while(1) {

    }
}
void second_thread(void *arg) {
    printf("pid %d name \"%s\": Hello World!\n", programManager.running->pid, programManager.running->name);
    while(1) {

    }
}

void first_thread(void *arg)
{
    // 第1个线程不可以返回
    printf("pid %d name \"%s\": Hello World!\n", programManager.running->pid, programManager.running->name);
    if (!programManager.running->pid)
    {
        programManager.executeThread(second_thread, nullptr, "second thread", 1);
        programManager.executeThread(third_thread, nullptr, "third thread", 1);
    }
    asm_halt();
}
*/

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

    //lab5
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

    //asm_enable_interrupt();
    //lab5
    //printf("print percentage: %%\n"
    //    "print char \"N\": %c\n"
    //    "print string \"Hello World!\": %s\n"
    //    "print decimal: \"-1234\": %d\n"
    //    "print hexadecimal \"0x7abcdef0\": %x\n",
    //    'N', "Hello World!", -1234, 0x7abcdef0);
    //uint a = 1 / 0;
    
    asm_halt();
}