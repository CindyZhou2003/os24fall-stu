#ifndef _DEFS_H
#define _DEFS_H
#define PHY_START 0x0000000080000000
#define PHY_SIZE 128 * 1024 * 1024 // 128 MiB，QEMU 默认内存大小
#define PHY_END (PHY_START + PHY_SIZE)

#define PGSIZE 0x1000 // 4 KiB
#define PGROUNDUP(addr) ((addr + PGSIZE - 1) & (~(PGSIZE - 1)))
#define PGROUNDDOWN(addr) (addr & (~(PGSIZE - 1)))


#include "types.h"

#define csr_read(csr)                       \
({                                          \
    register uint64 __v;                    \
    asm volatile ("csrr %0, " #csr          \
                  : "=r" (__v)              \
                  :                         \
                  : "memory");              \
    __v;                                    \
})

#define csr_write(csr, val)                         \
({                                                  \
    uint64 __v = (uint64)(val);                     \
    asm volatile ("csrw " #csr ", %0"               \
                    : : "r" (__v)                   \
                    : "memory");                    \
})

#endif
/*
csr_read(csr)：

使用 csrr 指令读取 CSR 寄存器的值。该值通过 csrr %0, csr 读取并存储在 __v 中。
__v 是一个临时变量，用于保存从 CSR 读取的值。
csr_write(csr, val)：

csr_write 宏的实现已经是正确的。它将值 val 写入到指定的 CSR 寄存器中，使用 csrw 指令来完成写入操作。
*/