.extern start_kernel
    .section .text.init
    # .section .text.entry
    .globl _start
_start:
    # ----------------------
    # -  initialize stack  -
    # ----------------------

    # Set stack pointer to .bss.stack
    la sp, boot_stack_top  # Load the top of the stack into sp (stack pointer)


    # ----------------------
    # set stvec = _traps

    la t0, _traps            # Load _traps address into t0 register
    csrw stvec, t0           # Write t0 register value to stvec CSR register
    # ----------------------

    # Enable Supervisor Timer Interrupt (STIE)
    csrr t0, sie        # 读取sie寄存器的值
    ori t1, t0, 1<<5    # 将sie寄存器值的第五位置1，结果写入t1
    csrw sie, t1        # 将更改后的值保存回sie寄存器
    # ----------------------


    call clock_set_next_event

    # Enable global interrupts in Supervisor mode (SIE)
    csrr t0, sstatus        # 读取sie寄存器的值
    ori t1, t0, 1<<1    # 将sie寄存器值的第五位置1，结果写入t1
    csrw sstatus, t1        # 将更改后的值保存回sie寄存器s
    # ----------------------


    # Jump to the C function start_kernel
    j start_kernel           # Jump to start_kernel function

    # Stack definition
    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096              # Allocate 4096 bytes of space for the stack

    .globl boot_stack_top

boot_stack_top:
