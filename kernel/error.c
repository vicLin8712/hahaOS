#include "sys/error.h"

static const struct err_code desc[] = {
    /* Success and general fail code */
    {ERR_OK, "no error"},          /* Operation successful */
    {ERR_FAIL, "generic failure"}, /* General fail */

    /* Task Management and Scheduler Error (auto-numbered from -16383) */
    {ERR_NO_TASK, "no task for scheduling"}, /* No task for scheduling */
    {ERR_KCB_ALLOC,
     "KCB malloc fail"}, /* Kernel control block allocation fail */
    {ERR_TCB_ALLOC,
     "TCB malloc fail"}, /* Thread control block allocation fail */
    {ERR_STACK_ALLOC,
     "Task stack malloc fail"}, /* Task stack allocation fail */

    /* Memory Protection Error*/
    {ERR_STACK_CHECK,
     "Stack malloc overflow or corruption"}, /* Stack overflow or corruption
                                                detected */
    {ERR_MALLOC, "Malloc out of heap bottom! "},
    {ERR_UNKNOWN, "Unknow error happened! "},
};

const struct err_code *const perror = desc;