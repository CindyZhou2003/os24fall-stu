#include "types.h"
#include "sbi.h"
/*
我们首先定义结构 struct sbiret 来保存函数返回的错误码和值。
函数 sbi_ecall 接受 SBI 调用的所有参数，包括扩展 ID 和函数 ID，以及最多六个参数 (arg0 到 arg5)。
我们使用 GCC 的内嵌汇编语法将这些参数放入 RISC-V 的寄存器 a0 到 a7 中。这些寄存器在 SBI 规范中定义用于参数传递和返回值。
ecall 指令触发环境调用，根据 a7 和 a6 的值跳转到相应的 SBI 功能。
在 ecall 完成后，结果将存放在 a0 和 a1 中，分别为错误码和实际的返回值，这些值被保存到 struct sbiret 结构体实例 result 中并返回。
 */

struct sbiret sbi_ecall(uint64_t ext, uint64_t fid, 
                        uint64_t arg0, uint64_t arg1, uint64_t arg2,
                        uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    struct sbiret ret;

    __asm__ volatile (
        // Load arguments into registers
        "mv a7, %[ext]\n"         // Move ext (Extension ID) to a7
        "mv a6, %[fid]\n"         // Move fid (Function ID) to a6
        "mv a0, %[arg0]\n"        // Move arg0 to a0
        "mv a1, %[arg1]\n"        // Move arg1 to a1
        "mv a2, %[arg2]\n"        // Move arg2 to a2
        "mv a3, %[arg3]\n"        // Move arg3 to a3
        "mv a4, %[arg4]\n"        // Move arg4 to a4
        "mv a5, %[arg5]\n"        // Move arg5 to a5
        "ecall\n"                 // Make the SBI call
        // Store the return values
        "mv %[error], a0\n"       // Move the error code (from a0) to ret.error
        "mv %[value], a1\n"       // Move the return value (from a1) to ret.value
        : [error] "=r" (ret.error), [value] "=r" (ret.value)  // Outputs
        : [ext] "r" (ext), [fid] "r" (fid),                   // Inputs
          [arg0] "r" (arg0), [arg1] "r" (arg1), 
          [arg2] "r" (arg2), [arg3] "r" (arg3), 
          [arg4] "r" (arg4), [arg5] "r" (arg5)
        : "memory", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7");  // Clobbered registers

    return ret;  // Return the structure containing error and value
}

struct sbiret sbi_debug_console_write_byte(uint8_t byte) {
    // Use Extension ID 0x4442434e and Function ID 2
    return sbi_ecall(0x4442434e, 2, byte, 0, 0, 0, 0, 0);
}

struct sbiret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason) {
    // Use Extension ID 0x53525354 and Function ID 0
    return sbi_ecall(0x53525354, 0, reset_type, reset_reason, 0, 0, 0, 0);
}

