#include "sbi.h"
#include "printk.h"
// QEMU中时钟的频率是10MHz, 也就是1秒钟相当于10000000个时钟周期。
unsigned long TIMECLOCK = 10000000;

unsigned long get_cycles() {
    unsigned long time;
    // 使用 rdtime 汇编指令读取当前的 time (mtime) 寄存器的值
    __asm__ volatile("rdtime %0" : "=r"(time));
    return time;
}

void clock_set_next_event() {
    // 下一次 时钟中断 的时间点
    unsigned long next = get_cycles() + TIMECLOCK;
    printk("Setting next timer event at cycle: %lu\n", next);
    // 使用 sbi_ecall 来完成对下一次时钟中断的设置
    struct sbiret ret = sbi_ecall(0x00, 0x00, next, 0, 0, 0, 0, 0);

    // 检查返回的 error 字段
    // if (ret.error) {
    //     printk("SBI call error: %ld\n", ret.error);  // 打印SBI调用错误码
    // } else {
    //     printk("SBI call succeeded, value: %ld\n", ret.value);  // 打印成功返回的值
    // }
} 
