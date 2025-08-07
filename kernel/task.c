#include "include/sys/task.h"
#include "type.h"
#include "include/libc.h"
struct task tasks[PROCS_MAX];
kcb_t kernel_state = {
    .tasks = (uint32_t*)&tasks,
    .cur_task = NULL,
    .pid_assign = 0
};
kcb_t *kcb = &kernel_state;

int32_t create_task(uint32_t entry)
{
    struct task *available = NULL;
    
    /* Search available task memory in the array */
    for (int index = 0;index < PROCS_MAX; index++)
    {   
        if (tasks[index].state == NO_TASK)
        {
            available = &tasks[index];
            break;
        }
    }

    available->entry = entry;
    available->pid = kcb->pid_assign++;

    /* Initialize jmp buffer of new task*/
    for (int i = 0; i<12; i++){
        available->context[i] = 0;
    }
    available->context[12] = (uint32_t)&available->context[0]; /* Stack pointer */
    available->context[13] = entry;

    available->state = TASK_READY;
}

uint8_t sched_select_next_task(void)
{
    for (int i = 0;i < PROCS_MAX;i++)
    {
        if (tasks[i].state == TASK_READY)
        {
            kcb->cur_task = &tasks[i];
            return 1;
        }
        
    }
    printf("NO TASK FOUND");
    while (1);
}

void sched(void) {
    longjmp(kcb->cur_task->context, 1);
}

void yield(void) {
    setjmp(kcb->cur_task->context);
    kcb->cur_task->state = TASK_STOPPED;
    sched_select_next_task();
    sched();
}