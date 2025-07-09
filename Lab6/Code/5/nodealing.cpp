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

//lab6
Semaphore chopstick[5];
Semaphore mutex;

void Philosopher1(void *arg)
{
    
    {
        mutex.P();
        printf("Philosopher 1 left chopstick is picked.\n");
        chopstick[0].P();
        printf("Philosopher 1 right chopstick is picked.\n");
        chopstick[1].P();
        mutex.V();
        printf("Philosopher 1 is eating meal.\n");
        chopstick[0].V();
        chopstick[1].V();
        printf("Philosopher 1 is thinking.\n");
    } 
    
}

void Philosopher2(void *arg)
{
    
    {
        mutex.P();
        printf("Philosopher 2 left chopstick is picked.\n");
        chopstick[1].P();
        printf("Philosopher 2 right chopstick is picked.\n");
        chopstick[2].P();
        mutex.V();
        printf("Philosopher 2 is eating meal.\n");
        chopstick[1].V();
        chopstick[2].V();
        printf("Philosopher 2 is thinking.\n");
    } 
}

void Philosopher3(void *arg)
{
    
    {
        mutex.P();
        printf("Philosopher Ltx0104 left chopstick is picked.\n");
        chopstick[2].P();
        printf("Philosopher Ltx0104 right chopstick is picked.\n");
        chopstick[3].P();
        mutex.V();
        printf("Philosopher Ltx0104 is eating meal.\n");
        chopstick[2].V();
        chopstick[3].V();
        printf("Philosopher Ltx0104 is thinking.\n");
    } 
}

void Philosopher4(void *arg)
{
    
    {
        mutex.P();
        printf("Philosopher 4 left chopstick is picked.\n");
        chopstick[3].P();
        printf("Philosopher 4 right chopstick is picked.\n");
        chopstick[4].P();
        mutex.V();
        printf("Philosopher 4 is eating meal.\n");
        chopstick[3].V();
        chopstick[4].V();
        printf("Philosopher 4 is thinking.\n");
    } 
}

void Philosopher5(void *arg)
{
    
    {
        mutex.P();
        printf("Philosopher 5 left chopstick is picked.\n");
        chopstick[4].P();
        printf("Philosopher 5 right chopstick is picked.\n");
        chopstick[0].P();
        mutex.V();
        printf("Philosopher 5 is eating meal.\n");
        chopstick[4].V();
        chopstick[0].V();
        printf("Philosopher 5 is thinking.\n");
    } 
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

    for(int i=0;i<5;i++){
        chopstick[i].initialize(1);
    }
    mutex.initialize(1);

    programManager.executeThread(Philosopher1, nullptr, "p1 thread", 1);
    programManager.executeThread(Philosopher2, nullptr, "p2 thread", 1);
    programManager.executeThread(Philosopher3, nullptr, "p3 thread", 1);
    programManager.executeThread(Philosopher4, nullptr, "p4 thread", 1);
    programManager.executeThread(Philosopher5, nullptr, "p5 thread", 1);
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
    asm_halt();
}