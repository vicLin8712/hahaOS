#pragma once
#include "hal.h"
#include "type.h"

extern uint32_t __stack_top;

/* Max number of tcb */
#define PROCS_MAX 8

/* End tcb (can put new tcb)*/
#define NO_TASK 0
/* Ready to run tcb*/
#define TASK_READY 1
/* Runnable tcb */
#define TASK_RUNNING 2
/* Stop tcb (can be resumed)*/
#define TASK_STOPPED 3
/* Postpone task */
#define TASK_POSTPONE 4

typedef struct tcb {
    uint32_t entry; /* new function entry address */
    int32_t pid;
    int32_t state;
    jmp_buf context;
    uint32_t stack[8192];
    uint32_t task_index;
} tcb_t;

typedef struct {
    struct tcb *tcbs;    /* Data structure store all tcbs */
    struct tcb *cur_tcb; /* Current running tcb */
    uint8_t pid_assign;  /* Check pid and assign to new tcb */
    jmp_buf context;

} kcb_t;

extern kcb_t *kcb;

extern struct tcb tcbs[PROCS_MAX];
/* Scheduler */
int32_t create_task(uint32_t pc);
/* Find next available tcb*/
uint8_t sched_select_next_task(void);
/* Schedule process */
void scheduler(void);
/* Yield to sched*/
void yield(void);

/* Handle error and print message */
void panic(int32_t code);