#include "sys/task.h"
#include "include/lib/list.h"
#include "include/libc.h"
#include "sys/error.h"
#include "type.h"


/* Initialization */
sched_t scheduler_state = {

    .bitmap = 0,
    .ctxsw_total = 0,
    .total_exe_ticks = 0,
    .idle = NULL,
};
sched_t *sched = &scheduler_state;

kcb_t kernel_state = {
    .hart_id = 0,
    .cur_tcb = NULL,
    .pid_assign = 0,
    .preemptive = true,
    .ticks = 0,

    .task_list = NULL,
    .ready = NULL,
    .wait = NULL,
    .suspend = NULL,
};
kcb_t *kcb = &kernel_state;

/* Initialize queue of tasks */
uint32_t init_task_q(void)
{
    kcb->task_list = list_init();
    for (int i = 0; i < PRIOR_LEVEL; i++)
        kcb->ready[i] = list_init();
    kcb->suspend = list_init();
    kcb->wait = list_init();
}



uint32_t create_task(uint32_t entry, uint8_t prio)
{
    list_node_t *new_task_node = malloc(sizeof(list_node_t));
    tcb_t *new_task = malloc(sizeof(tcb_t));
    new_task_node->data = new_task;

    /* Basic properties */
    new_task->id = kcb->pid_assign++;
    new_task->state = TASK_READY;
    new_task->prio = prio;
    new_task->entry = entry;

    /* Initialize jmp buffer of new task */
    for (int i = 0; i < 15; i++) {
        new_task->context[i] = 0;
    }

    uintptr_t sp =
        (uintptr_t) (new_task->stack +
                     STACK_SIZE);  // Take highest address of stack address
    sp &= ~(uintptr_t) 0xF;        // 16-byte align (RISC-V ABI)

    new_task->context[12] = (uint32_t) sp;
    new_task->context[13] = (uint32_t) entry;
    new_task->context[14] = (uint32_t) 1u << 3 | 3u << 11;


    /* Hang up first task node into kcb */
    if (!kcb->cur_tcb) {
        kcb->cur_tcb = new_task;
        kcb->cur_tcb->state = TASK_RUNNING;
        return true;
    }
    /* Push tcb to corresponding lists */
    list_push(kcb->task_list, new_task);
    list_push(kcb->ready[prio], new_task);
}

uint8_t sched_select_next_task(void)
{
    /* Reschedule new tasks*/
    tcb_t *next = (tcb_t *) list_pop(kcb->ready[1]);

    /* Stop current task */
    tcb_t *curr = kcb->cur_tcb;

    if (curr && curr->state == TASK_RUNNING)
        curr->state = TASK_READY;

    kcb->cur_tcb = next;

    list_push(kcb->ready[1], curr);

    if (!next) {
        panic(ERR_NO_TASK);
        return 0;
    }
    return next->id;
}

void scheduler(void)
{
    if (hal_context_save((tcb_t *) (kcb->cur_tcb)->context) != 0) {
        return;
    }

    sched_select_next_task();
    hal_context_restore(((tcb_t *) (kcb->cur_tcb)->context), 1);
}

void yield(void)
{
    setjmp(kcb->cur_tcb->context);
    sched_select_next_task();
    scheduler();
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
