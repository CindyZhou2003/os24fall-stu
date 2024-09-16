#include "printk.h"
#include "sbi.h"
// #include "../arch/riscv/kernel/clock.c"

// extern unsigned long get_cycles();
extern void clock_set_next_event();


extern void test();

#include <defs.h>

void check_sstatus() {
    // 读取 sstatus 寄存器的值
    unsigned long sstatus_value = csr_read(sstatus);

    // Print the sstatus value
    printk("sstatus value: 0x%lx\n", sstatus_value);
}

void check_sscratch() {
    // 向 sscratch 写入一个值 (例如 0x12345678)
    unsigned long write_value = 0x12345678;
    csr_write(sscratch, write_value);

    // 读取 sscratch 寄存器的值以验证
    unsigned long sscratch_value = csr_read(sscratch);

    // 打印写入值和读取值，检查是否匹配
    printk("Written value: 0x%lx\n", write_value);
    printk("Read back sscratch value: 0x%lx\n", sscratch_value);

    // 验证是否写入成功
    if (sscratch_value == write_value) {
        printk("sscratch write success!\n");
    } else {
        printk("sscratch write failed!\n");
    }
}

int start_kernel() {
    printk("2024");
    printk(" Hello RISC-V\n");

    // check_sstatus();
    // check_sscratch();

    // init_trap();  // 设置trap处理函数
    // enable_interrupts();  // 启用全局中断和时钟中断
    //clock_set_next_event();  // 设置时钟中断

    test(); // DO NOT DELETE !!!

	return 0;
}

