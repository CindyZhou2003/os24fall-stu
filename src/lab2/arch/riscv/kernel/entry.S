.section .text.entry
.align 2
.globl _traps
.globl __switch_to
.globl __dummy

_traps:
    # 1. 保存 31 个寄存器和 sepc 到栈

    addi sp, sp, -264           # 分配栈空间，32 * 8 + 8 = 264 字节

    sd x0, 0(sp)
    sd x1, 8(sp)

    sd x3, 24(sp)
    sd x4, 32(sp)
    sd x5, 40(sp)
    sd x6, 48(sp)
    sd x7, 56(sp)
    sd x8, 64(sp)
    sd x9, 72(sp)
    sd x10, 80(sp)
    sd x11, 88(sp)
    sd x12, 96(sp)
    sd x13, 104(sp)
    sd x14, 112(sp)
    sd x15, 120(sp)
    sd x16, 128(sp)
    sd x17, 136(sp)
    sd x18, 144(sp)
    sd x19, 152(sp)
    sd x20, 160(sp)
    sd x21, 168(sp)
    sd x22, 176(sp)
    sd x23, 184(sp)
    sd x24, 192(sp)
    sd x25, 200(sp)
    sd x26, 208(sp)
    sd x27, 216(sp)
    sd x28, 224(sp)
    sd x29, 232(sp)
    sd x30, 240(sp)
    sd x31, 248(sp)

    csrr t0, sepc               # 读取 sepc
    sd t0, 256(sp)              # 保存 sepc 到栈上

    sd x2, 16(sp)               # 保存 x2

    # 2. 调用 trap_handler
    csrr a0, scause             # scause -> a0
    csrr a1, sepc               # sepc -> a1
    call trap_handler           # 调用 C 语言中的 trap_handler

    # 3. 恢复 sepc 和 31 个寄存器
    ld t0, 256(sp)              # 恢复 sepc 的值
    csrw sepc, t0               # 将恢复的 sepc 写回 sepc

    ld x0, 0(sp)
    ld x1, 8(sp)
    ld x3, 24(sp)
    ld x4, 32(sp)
    ld x5, 40(sp)
    ld x6, 48(sp)
    ld x7, 56(sp)
    ld x8, 64(sp)
    ld x9, 72(sp)
    ld x10, 80(sp)
    ld x11, 88(sp)
    ld x12, 96(sp)
    ld x13, 104(sp)
    ld x14, 112(sp)
    ld x15, 120(sp)
    ld x16, 128(sp)
    ld x17, 136(sp)
    ld x18, 144(sp)
    ld x19, 152(sp)
    ld x20, 160(sp)
    ld x21, 168(sp)
    ld x22, 176(sp)
    ld x23, 184(sp)
    ld x24, 192(sp)
    ld x25, 200(sp)
    ld x26, 208(sp)
    ld x27, 216(sp)
    ld x28, 224(sp)
    ld x29, 232(sp)
    ld x30, 240(sp)
    ld x31, 248(sp)

    ld x2, 16(sp)               # 恢复原来的栈顶指针

    addi sp, sp, 264            # 调整栈指针恢复

    # 4. 从 trap 返回
    sret

    
__switch_to:
    sd ra, 32(a0)          # 保存 ra 到 prev->thread_struct.ra
    sd sp, 40(a0)          # 保存 sp 到 prev->thread_struct.sp

    # 保存 s0 ~ s11 到 prev->thread_struct.s[0] ~ s[11]
    sd s0, 48(a0)
    sd s1, 56(a0)
    sd s2, 64(a0)
    sd s3, 72(a0)
    sd s4, 80(a0)
    sd s5, 88(a0)
    sd s6, 96(a0)
    sd s7, 104(a0)
    sd s8, 112(a0)
    sd s9, 120(a0)
    sd s10, 128(a0)
    sd s11, 136(a0)

    # 加载下一个线程的上下文
    ld ra, 32(a1)          # 恢复 ra (从 next->thread_struct.ra 加载)
    ld sp, 40(a1)          # 恢复 sp (从 next->thread_struct.sp 加载)

    # 恢复 s0 ~ s11
    ld s0, 48(a1)
    ld s1, 56(a1)
    ld s2, 64(a1)
    ld s3, 72(a1)
    ld s4, 80(a1)
    ld s5, 88(a1)
    ld s6, 96(a1)
    ld s7, 104(a1)
    ld s8, 112(a1)
    ld s9, 120(a1)
    ld s10, 128(a1)
    ld s11, 136(a1)

    ret


__dummy:
    # 设置 sepc 寄存器为 dummy 函数的地址
    la      t0, dummy    // 将 dummy 地址加载到 t0 寄存器中
    csrw    sepc, t0     // 将 t0 中的地址写入到 sepc (Supervisor Exception Program Counter)
    # 使用 sret 返回到 S 模式，让新的线程从 sepc 中的地址（即 dummy 的地址）开始执行
    sret


