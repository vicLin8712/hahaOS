#include "sys/task.h"
#include "include/libc.h"
#include "sys/error.h"
#include "type.h"

#define STACK_SIZE 8192
struct tcb tcbs[PROCS_MAX];
kcb_t kernel_state = {.tcbs = (uint32_t *) &tcbs,
                      .cur_tcb = NULL,
                      .pid_assign = 0};
kcb_t *kcb = &kernel_state;

int32_t create_task(uint32_t entry)
{
    struct tcb*available = NULL;

    /* Search available task memory in the array */
    for (int index = 0; index < PROCS_MAX; index++) {
        if (tcbs[index].state == NO_TASK) {
            available = &tcbs[index];
            break;
        }
    }

    available->entry = entry;
    available->pid = kcb->pid_assign++;
    available->state = TASK_READY;

    /* Initialize jmp buffer of new task*/
    for (int i = 0; i < 12; i++) {
        available->context[i] = 0;
    }

    uintptr_t sp =
        (uintptr_t) (available->stack +
                     STACK_SIZE);  // Take highest address of stack address
    sp &= ~(uintptr_t) 0xF;        // 16-byte align (RISC-V ABI)

    available->context[12] = (uint32_t) sp;
    available->context[13] = (uint32_t) entry;
}

uint8_t sched_select_next_task(void)
{
    for (int i = 0; i < PROCS_MAX; i++) {
        if (tcbs[i].state == TASK_READY) {
            printf("FIND TASKS\n");
            for (int j = 0; j < 300000000; j++)
                ;
            kcb->cur_tcb = &tcbs[i];
            return 1;
        }
    }
    panic(ERR_NO_TASK);
}

void sched(void)
{
    longjmp(kcb->cur_tcb->context, 1);
}

void yield(void)
{
    kcb->cur_tcb->state = TASK_STOPPED;
    setjmp(kcb->cur_tcb->context);
    sched_select_next_task();
    sched();
}

/* Kernel panic */
void panic(int32_t code)
{
    const char *msg = "unknown error";
    for (size_t i = 0; perror[i].desc != ERR_UNKNOWN; ++i) {
        if (perror[i].code == code) {
            msg = perror[i].desc;
            break;
        }
    }
    printf("\n *** KERNEL PANIC (%d) – %s\n", (int) code, msg);
    hal_panic();
}