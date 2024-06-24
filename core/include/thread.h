#pragma once

#include <stdint.h>
#include "dlist.h"
#include "threadImpl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MOC_THREAD_ASSERT(condition, returnValue) \
            do { \
                if(!(condition)) \
                    return returnValue; \
            } while(0)

/* FIXME: this is not the correct position to define MOC_MAX_THREAD_PRIO */
/* -32 - 0 - 32 */
#define MOC_MAX_THREAD_PRIO 2 * 32 + 1
typedef int16_t mThreadPID_t;


typedef enum {
    mThreadErrorType_ERROR_NONE,

    /* thread priority is not valid */
    mThreadErrorType_ERROR_THREADPRIOINVALID,

    /* thread priority is not valid */
    mThreadErrorType_ERROR_THREADSTACKOVERLAP,
} mThreadErrorType_t;

typedef enum {
    /* not a real thread */
    mThreadStatus_DUMMY,

    /* thread is stopped */
    mThreadStatus_STOPED,

    /* thread not yet started  */
    mThreadStatus_PRESTART,

    /* thread is running */
    mThreadStatus_RUNNING,

    /* thread is suspended */
    mThreadStatus_SUSPENDED,

    /* thread is being aborted */
    mThreadStatus_ABORTING,

    /* thread is waiting on other objects */
    mThreadStatus_PENDING,

    /* thread is sleeping */
    mThreadStatus_SLEEPING,

    /* thread status total number */
    mThreadStatus_NUM,

} mThreadStatus_t;

typedef struct {
    /* NOTICE: the position of sp shouldn't be changed because we use the address of mThread_t to locate sp */
    char *sp;                   /* stack pointer of the thread */

    /* FIXME: current system supports only doubled-list sheduling */
    /* the first array member contains type dlist, the rest are dnode */
    union {
		sys_dnode_t threadNode; /* zephyr dlist lib ( $(MOC_ROOT)/lib/dlist/dlist.h ) */
	};

    char *stackStart;           /* start address of the thread stack */
    mThreadPID_t pid;                /* pid of the thread */

    int8_t priority;            /* priority of the thread */
    mThreadStatus_t status;     /* status of the thread */
    
    void *context;              /* user context passed to thread */

} mThread_t;


typedef struct {
    mThread_t *thread[MOC_MAX_THREAD_PRIO];

} mCPU_t;



typedef struct {
    /* FIXME: current system supports only one cpu */
    mCPU_t cpu[1];

} mKernel_t;

static mKernel_t mKernel;

/* FIXME: current system supports only one cpu, current thread needs to be multiple */
static mThread_t *mThreadCurrent;

/* mThread_t *mGetCurrentThread(void)
{
    return (mThread_t *)mThreadCurrent;
} */

#define MOC_PRIO_MAP_TO_ARRAY(priority) ((priority) + (MOC_MAX_THREAD_PRIO - 1) / 2)

#define MOC_GET_KERNEL() mKernel

#define MOC_GET_CPU_BY_NUM(num) mKernel.cpu[num]

#define MOC_GET_THREAD_CPU0(num) mKernel.cpu[0].thread[num]

#define MOC_GET_THREAD_NODE_CPU0(num) mKernel.cpu[0].thread[num]->threadNode

#define MOC_GET_THREAD_BY_PRIO_CPU0(priority) mKernel.cpu[0].thread[MOC_PRIO_MAP_TO_ARRAY(priority)]



mThreadErrorType_t mThreadInit(void);
mThreadErrorType_t mThreadCreate(char *stackStart, int stackSize, int8_t priority, \
                                    mThreadCallback_t callback, void *context);

mThread_t *mThreadSchedule(void);
void mThread_debug(void);
#ifdef __cplusplus
}
#endif