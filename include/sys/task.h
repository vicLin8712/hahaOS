#pragma once
#include "type.h"
#include "hal.h"

/* Max number of task */
#define PROCS_MAX 8

/* End task (can put new task)*/
#define NO_TASK 0
/* Ready to run task*/
#define TASK_READY 1
/* Runnable task */
#define TASK_RUNNING 2
/* Stop task (can be resumed)*/
#define TASK_STOPPED 3

struct task {
    uint32_t entry; /* new function entry address */
    int32_t pid;
    int32_t state;
    jmp_buf context;
    uint8_t stack[8192];
};

typedef struct{
    struct task *tasks; /* Data structure store all tasks */
    struct task *cur_task; /* Current running task */
    uint8_t pid_assign; /* Check pid and assign to new task */


}kcb_t;

extern kcb_t *kcb;

extern struct task tasks[PROCS_MAX];

/* Scheduler */
int32_t create_task(uint32_t pc);

/* Find next available task*/
uint8_t sched_select_next_task(void);

/* Schedule process */
void sched(void);

/* Yield to sched*/
void yield(void);
