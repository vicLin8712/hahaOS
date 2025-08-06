#pragma once
#include "include/type.h"

/* Max number of process */
#define PROCS_MAX 8

/* Unused process control structure */
#define PROC_UNUSED 0
/* Runnable process */
#define PROC_RUNNABLE 1

struct process{
    int pid;
    int state;
    vaddr_t sp;
    uint8_t stack[8192];
};

extern struct process procs[PROCS_MAX];
void switch_context (vaddr_t *prev_sp, vaddr_t* next_sp);
void init_context (vaddr_t *init_sp);
struct process *create_process(uint32_t pc);

void task_A();
void task_B();
struct process *scheduler();
void schedule();