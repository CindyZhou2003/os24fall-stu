// arch/riscv/kernel/proc.c
#include "mm.h"
#include "defs.h"
#include "proc.h"
#include "stdlib.h"
#include "printk.h"

extern void __dummy();

struct task_struct *idle;           // idle process
struct task_struct *current;        // 指向当前运行线程的 task_struct
struct task_struct *task[NR_TASKS]; // 线程数组，所有的线程都保存在此

void task_init() {
    srand(2024);  // 设置随机种子

    // 1. 为 idle 线程分配物理页
    idle = (struct task_struct *)kalloc();
    if (!idle) {
        printk("Failed to allocate memory for idle task.\n");
        return;
    }

    // 2. 初始化 idle 线程的状态
    idle->state = TASK_RUNNING;
    idle->counter = 0;
    idle->priority = 12345;
    idle->pid = 0; // 将 idle 线程的 pid 设为 0

    // 3. 设置当前线程和任务数组
    current = idle;
    task[0] = idle;

    // 为 task[1] ~ task[NR_TASKS - 1] 进行初始化
    for (int i = 1; i < NR_TASKS; ++i) {
        // 1. 为每个线程分配物理页
        task[i] = (struct task_struct *)kalloc();
        if (!task[i]) {
            printk("Failed to allocate memory for task %d.\n", i);
            return;
        }

        // 2. 初始化线程状态
        task[i]->state = TASK_RUNNING;
        task[i]->counter = 0;
        task[i]->priority =  rand() % 10 + 1;  // 设置随机优先级
        task[i]->pid = i;

        // 3. 设置线程的 ra 和 sp
        task[i]->thread.ra = (uint64_t)__dummy;
        task[i]->thread.sp = (uint64_t)task[i] + 4096;  // 物理页的高地址
    }

    printk("...task_init done!\n");
}

#if TEST_SCHED
#define MAX_OUTPUT ((NR_TASKS - 1) * 10)
char tasks_output[MAX_OUTPUT];
int tasks_output_index = 0;
char expected_output[] = "2222222222111111133334222222222211111113";
#include "sbi.h"
#endif

void dummy() {
    uint64_t MOD = 1000000007;
    uint64_t auto_inc_local_var = 0;
    int last_counter = -1;
    while (1) {
        if ((last_counter == -1 || current->counter != last_counter) && current->counter > 0) {
            if (current->counter == 1) {
                --(current->counter);   // forced the counter to be zero if this thread is going to be scheduled
            }                           // in case that the new counter is also 1, leading the information not printed.
            last_counter = current->counter;
            auto_inc_local_var = (auto_inc_local_var + 1) % MOD;
            printk("[PID = %d] is running. auto_inc_local_var = %d\n", current->pid, auto_inc_local_var);
            #if TEST_SCHED
            tasks_output[tasks_output_index++] = current->pid + '0';
            if (tasks_output_index == MAX_OUTPUT) {
                for (int i = 0; i < MAX_OUTPUT; ++i) {
                    if (tasks_output[i] != expected_output[i]) {
                        printk("\033[31mTest failed!\033[0m\n");
                        printk("\033[31m    Expected: %s\033[0m\n", expected_output);
                        printk("\033[31m    Got:      %s\033[0m\n", tasks_output);
                        sbi_system_reset(SBI_SRST_RESET_TYPE_SHUTDOWN, SBI_SRST_RESET_REASON_NONE);
                    }
                }
                printk("\033[32mTest passed!\033[0m\n");
                printk("\033[32m    Output: %s\033[0m\n", expected_output);
                sbi_system_reset(SBI_SRST_RESET_TYPE_SHUTDOWN, SBI_SRST_RESET_REASON_NONE);
            }
            #endif
        }
    }
}


extern void __switch_to(struct task_struct *prev, struct task_struct *next);

void switch_to(struct task_struct *next) {
    if (current == next) {
        printk(RED"Already running task %d, no switch needed.\n", next->pid); 
        return; 
    } 

    struct task_struct *prev = current;
    printk(RED"Switching from task %d to task %d :[PID = %d, COUNTER = %d, PRIORITY = %d]\n", 
        prev->pid, next->pid, next->counter, next->priority);
    current = next;  // 更新 current 指向

    // 传递 prev 和 next
    __switch_to(prev, next);  // 切换上下文
}

void do_timer(void) {   
   /* 如果当前线程是 idle 线程 直接进行调度 */
    if (current == idle || current->pid == 0) {
        printk(YELLOW"idle process is running!\n");
        schedule();
        return;
    }
    /* 如果当前线程不是 idle 对当前线程的运行剩余时间减 1 
       若剩余时间仍然大于0 则直接返回 否则进行调度 */
    

    if(current->counter > 0) {
        current->counter -- ;
        printk(YELLOW"do_timer: Task %d has counter = %d\n", current->pid, current->counter);
        return;
    }

    if (current->counter == 0) {
        printk(YELLOW"Task %d has counter 0, forcing switch...\n", current->pid);
        schedule();  // Trigger rescheduling
        return;
    }

}


void schedule() {
    uint64_t max_counter = 0;
    struct task_struct *next_task = NULL;

    printk("Starting schedule...\n");

    // 检查所有任务状态
    for (int i = 1; i < NR_TASKS; ++i) {
        printk("Task %d: [PID = %d, COUNTER = %d, PRIORITY = %d]\n",
               i, task[i]->pid, task[i]->counter, task[i]->priority);

        if (task[i]->state == TASK_RUNNING && task[i]->counter >= max_counter && task[i]->counter > 0) {
            max_counter = task[i]->counter;
            next_task = task[i];
        }
    }
    printk("Checking counters, max_counter = %d\n", max_counter);

    // 如果所有任务的 counter 都为 0，则重新分配 counter = priority
    if (max_counter == 0 || next_task == NULL) {
        printk("All task counters are 0, resetting counters based on priority...\n");
        for (int i = 1; i < NR_TASKS; ++i) {
            task[i]->counter = task[i]->priority;
            // task[i]->state = TASK_RUNNING;  // 强制将所有任务状态设置为 TASK_RUNNING
            printk(GREEN"SET [PID = %d PRIORITY = %d COUNTER = %d, STATE = %d]\n",
                   task[i]->pid, task[i]->priority, task[i]->counter, task[i]->state);
        }

        // 重新选择调度的线程
        for (int i = 1; i < NR_TASKS; ++i) {
            if (task[i]->state == TASK_RUNNING && task[i]->counter >= max_counter && task[i]->counter > 0) {
                max_counter = task[i]->counter;
                next_task = task[i];
            }
        }
        printk("After resetting counters, max_counter = %d\n", max_counter);
    }
    

    if (next_task) {
        printk(PURPLE"Selected task %d with counter %d for next run.\n", next_task->pid, next_task->counter);
    }

    // 调用 switch_to 切换到下一个线程
    if (next_task && next_task != current) {
        printk(PURPLE"Switching to task %d with counter %d\n", next_task->pid, next_task->counter);
        switch_to(next_task);
    } else {
        printk(PURPLE"No task switch needed. Current task: %d, Counter: %d\n", current->pid, current->counter);
    }

}
