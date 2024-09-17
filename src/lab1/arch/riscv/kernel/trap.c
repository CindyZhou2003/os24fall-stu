// trap.c

extern unsigned long TIMECLOCK;
extern unsigned long get_cycles();
extern void clock_set_next_event();

#include "printk.h"


// Trap处理函数
void trap_handler(unsigned long scause, unsigned long sepc) {
    // 打印详细信息
    printk("scause: %lx, sepc: %lx\n", scause, sepc);

    if (scause >> 63) {  // 中断

        if( (scause & 0xFF) == 5) { // 左移去掉63号位，右移恢复，得到 Exception Code
            // Exception Code == 5，判断为 timer interrupt
            printk("%s", "[Trap Handler] S-Mode Timer Interrupt!\n");
            clock_set_next_event();
        } else {
            printk("Other interrupt");
        }
    } else {  // 异常
        printk("Exception occurred, type: %lx at SEPC: %lx\n", scause, sepc);
    }
}

