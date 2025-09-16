#pragma once
#include "hal.h"
#include "lib/list.h"
#include "type.h"

/* Stack size and begining */
extern uint32_t __stack_top;
#define STACK_SIZE 8192

/* Define priority level of task */
#define PRIOR_LEVEL 8

/* Task state definition */
#define TASK_RUNNING 0   /* The task executing on the CPU */
#define TASK_READY 1     /* The tasks ready for scheduler selecting */
#define TASK_BLOCKED 2   /* The task blocked and wait for event happen */
#define TASK_SUSPENDED 3 /* The tasks stop itself */
#define TASK_DELETED                                         \
    4 /* Deleted tasks, wait for idle task to recycle memory \
       */

/* Task control block */
typedef struct tcb {
    /* Task basic properties */
    int32_t id;
    uint8_t state;
    uint8_t prio;
    list_node_t cur_node; /* Task node at queue*/
    uint32_t stack[STACK_SIZE];
    uintptr_t entry;

    /* Time relative */
    uint64_t total_exe_ticks;
    uint64_t time_slice_left; /* RR remaining ticks */
    uint64_t wakeup_tick;     /* Absolute tick to wakeup */

    /* Context switch relative */
    jmp_buf context;
    uint64_t ctxsw_irq;   /* Interrupt times*/
    uint64_t ctxsw_total; /* Total context switch times */
    uint8_t block_reason; /* Interrupt reasons */

} tcb_t;

/* Sched_t is the data structure that major recording properties of scheduler */
typedef struct {
    /* Tasks list properties */
    uint8_t bitmap;                    /* Bit map for O(1) scheduler */
    uint32_t tasks_count[PRIOR_LEVEL]; /* Number of tasks in each queue */
    uint64_t ctxsw_total;              /* Total context switch times */
    uint64_t total_exe_ticks;

    /* Special tasks */
    tcb_t *idle; /* Idle task */

} sched_t;

/* kcb also maintain task lists */
typedef struct {
    /* Basic properties about kernel state */
    int hart_id;             /* CPU hart ID */
    struct tcb *cur_tcb;     /* Task control block of current task */
    struct sched *sched;     /* Scheduler control block*/
    uint8_t pid_assign;      /* Check pid and assign to new tcb */
    jmp_buf context;         /* Kernel state context */
    bool preemptive;         /* Preemptive option */
    volatile uint32_t ticks; /* Global system tick, incremented by timer */

    /* All tasks lists */
    list_t *task_list;          /* List for all tasks, including DELETED */
    list_t *ready[PRIOR_LEVEL]; /* Ready for picked up tasks */
    list_t *wait;               /* Wait event tasks */
    list_t *suspend;            /* Suspended by itself tasks */

} kcb_t;


extern kcb_t *kcb;
extern sched_t *sched;
uint32_t init_task_q(void);
/* Scheduler */
uint32_t create_task(uint32_t, uint8_t);
/* Find next available tcb*/
uint8_t sched_select_next_task(void);
/* Schedule process */
void scheduler(void);
/* Yield to sched*/
void yield(void);

/* Handle error and print message */
void panic(int32_t code);