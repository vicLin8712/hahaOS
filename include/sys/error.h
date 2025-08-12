#pragma once
#include "type.h"

/* This file include all error codes */
enum {
    /* Success and general fail code */
    ERR_OK = 0,   /* Operation successful */
    ERR_FAIL = 1, /* General fail */

    /* Task Management and Scheduler Error (auto-numbered from -16383) */
    ERR_NO_TASK = -16383, /* No task for scheduling */
    ERR_KCB_ALLOC,        /* Kernel control block allocation fail */
    ERR_TCB_ALLOC,        /* Thread control block allocation fail */
    ERR_STACK_ALLOC,      /* Task stack allocation fail */

    /* Memory Protection Error*/
    ERR_STACK_CHECK, /* Stack overflow or corruption detected */

    ERR_UNKNOWN /* Unknow error, the last error case */

};

struct err_code {
    int32_t code;     /* Error code */
    char *const desc; /* Decode table pointer, never should be modified */
};

extern const struct err_code *const perror;