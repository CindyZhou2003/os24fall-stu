.section .text.entry
.align 2
.globl _traps 
_traps:
    # 1. 保存 31 个寄存器和 sepc 到栈
    csrw sscratch, sp # 将原先的栈顶指针保存到sscratch

    addi sp, sp, -264         # 分配栈空间，33 * 8 = 248 字节，sepc 需要额外 8 字节

    sd x0, 0*8(sp)
    sd x1, 1*8(sp)
    sd x3, 3*8(sp)
    sd x4, 4*8(sp)
    sd x5, 5*8(sp)
    sd x6, 6*8(sp)
    sd x7, 7*8(sp)
    sd x8, 8*8(sp)
    sd x9, 9*8(sp)
    sd x10, 10*8(sp)
    sd x11, 11*8(sp)
    sd x12, 12*8(sp)
    sd x13, 13*8(sp)
    sd x14, 14*8(sp)
    sd x15, 15*8(sp)
    sd x16, 16*8(sp)
    sd x17, 17*8(sp)
    sd x18, 18*8(sp)
    sd x19, 19*8(sp)
    sd x20, 20*8(sp)
    sd x21, 21*8(sp)
    sd x22, 22*8(sp)
    sd x23, 23*8(sp)
    sd x24, 24*8(sp)
    sd x25, 25*8(sp)
    sd x26, 26*8(sp)
    sd x27, 27*8(sp)
    sd x28, 28*8(sp)
    sd x29, 29*8(sp)
    sd x30, 30*8(sp)
    sd x31, 31*8(sp)

    csrr t0, sepc              # 读取 sepc
    sd t0, 32*8(sp)             # 保存 sepc 到栈上

    csrr t0, sscratch
    sd t0, 2*8(sp)
   

    # 2. 调用 trap_handler
    csrr a0, scause   # scause -> a0
    csrr a1, sepc     # sepc -> a1
    call trap_handler           # 调用C语言中的trap_handler

    # 3. 恢复 sepc 和 31 个寄存器
    ld t0, 32*8(sp)             # 恢复 sepc 的值
    csrw sepc, t0              # 将恢复的 sepc 写回 sepc

    ld x0, 0*8(sp)
    ld ra, 1*8(sp)
    ld x3, 3*8(sp)
    ld x4, 4*8(sp)
    ld x5, 5*8(sp)
    ld x6, 6*8(sp)
    ld x7, 7*8(sp)
    ld x8, 8*8(sp)
    ld x9, 9*8(sp)
    ld x10, 10*8(sp)
    ld x11, 11*8(sp)
    ld x12, 12*8(sp)
    ld x13, 13*8(sp)
    ld x14, 14*8(sp)
    ld x15, 15*8(sp)
    ld x16, 16*8(sp)
    ld x17, 17*8(sp)
    ld x18, 18*8(sp)
    ld x19, 19*8(sp)
    ld x20, 20*8(sp)
    ld x21, 21*8(sp)
    ld x22, 22*8(sp)
    ld x23, 23*8(sp)
    ld x24, 24*8(sp)
    ld x25, 25*8(sp)
    ld x26, 26*8(sp)
    ld x27, 27*8(sp)
    ld x28, 28*8(sp)
    ld x29, 29*8(sp)
    ld x30, 30*8(sp)
    ld x31, 31*8(sp)

    ld x2, 2*8(sp)

    addi sp, sp, 264            # 调整栈指针恢复

    # 4. 从 trap 返回
    sret

