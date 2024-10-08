// trap.c
extern unsigned long TIMECLOCK;
extern unsigned long get_cycles();
extern void clock_set_next_event();
extern void do_timer();  // 声明 do_timer 函数

#include "printk.h"

// Trap处理函数
void trap_handler(unsigned long scause, unsigned long sepc) {
    // 打印详细信息
    // printk("scause: %lx, sepc: %lx\n", scause, sepc);
    unsigned long stval, sstatus;

    // 获取 stval 和 sstatus 的值，用于进一步分析异常信息
    __asm__ volatile("csrr %0, stval" : "=r"(stval));
    __asm__ volatile("csrr %0, sstatus" : "=r"(sstatus));
    
    if (scause >> 63) {  // 中断
        if ((scause & 0xFF) == 5) { // Exception Code == 5, 判断为 timer interrupt
            // printk("%s", "[Trap Handler] S-Mode Timer Interrupt!\n");
            
            // 处理定时器中断
            clock_set_next_event();  // 设置下一个时钟中断事件
            do_timer();              // 调用 do_timer 处理调度逻辑
        } else {
            printk("Other interrupt");
        }
    } else {  // 异常
        printk("Exception occurred, type: %lx at SEPC: %lx, STVAL: %lx, SSTATUS: %lx\n", scause, sepc, stval, sstatus);
    }
}
