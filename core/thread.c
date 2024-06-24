#include <stdalign.h>
#include <stdio.h>

#include "irqImpl.h"
#include "thread.h"
#include "shellMgr.h"
#include <nrfx_systick.h>

char logbuffer[100];

/* FIXME: the debug function is not complete because we haven't impl the shellMgr done */
#define ThreadDebugLogout(format, ...) \
                do { \
                    snprintf(logbuffer, 100, format"\r\n", ##__VA_ARGS__); \
                    shellMgr->outputString(logbuffer); \
                    nrfx_systick_delay_ms(50); \
                } while(0);

/* FIXME: thread init function should support init status */
mThreadErrorType_t mThreadInit(void)
{
    ThreadDebugLogout("into thread init");
    mThreadErrorType_t err = mThreadErrorType_ERROR_NONE;
    
    for(int threadIndex = 0; threadIndex < MOC_MAX_THREAD_PRIO; threadIndex++)
        MOC_GET_THREAD_CPU0(threadIndex) = NULL;
    
    mThreadCurrent = NULL;
    
    return err;
}


mThreadErrorType_t mThreadCreate(char *stackStart, int stackSize, int8_t priority, \
                                    mThreadCallback_t callback, void *context)
{
    
    MOC_THREAD_ASSERT(priority <= 32 && priority >= -32, mThreadErrorType_ERROR_THREADPRIOINVALID);
    ThreadDebugLogout("into thread create");

    mThreadErrorType_t err = mThreadErrorType_ERROR_NONE;
    /* align the stack on a 16/32bit boundary */
    uintptr_t misalignment = (uintptr_t)stackStart % alignof(void *);
    if (misalignment) {
        misalignment = alignof(void *) - misalignment;
        stackStart += misalignment;
        stackSize -= misalignment;
    }

    /* make room for the thread control block */
    stackSize -= sizeof(mThread_t);

    /* round down the stackSize to a multiple of mThread_t alignments (usually 16/32bit) */
    stackSize -= stackSize % alignof(mThread_t);

    MOC_THREAD_ASSERT(stackSize > 0, mThreadErrorType_ERROR_THREADPRIOINVALID);

    /* allocate our thread control block at the top of our stackspace. Cast to
     * (uintptr_t) intermediately to silence -Wcast-align. (We manually made
     * sure alignment is correct above.) */
    mThread_t *newThread = (mThread_t *)(uintptr_t)(stackStart + stackSize);

    unsigned state = mDisableIrqImpl();
    
    newThread->stackStart = stackStart;
    newThread->context = context;
    newThread->pid++;
    newThread->status = mThreadStatus_PRESTART;
    newThread->priority = priority;
    newThread->sp = mThreadStackImplInit(callback, context, stackStart, stackSize);


    mThread_t *threadList = MOC_GET_THREAD_BY_PRIO_CPU0(priority);

    if(threadList == NULL){
        sys_dlist_init(&newThread->threadNode);
        MOC_GET_THREAD_BY_PRIO_CPU0(priority) = newThread;
        goto restore_irq;
    }
        

    sys_dnode_init(&newThread->threadNode);
    sys_dlist_append(&threadList->threadNode, &newThread->threadNode);
    


restore_irq:

    mRestoreIrq(state);

    ThreadDebugLogout("ready to call pendsv");
    mThreadYieldHigher();

    return err;
}

void mThread_debug(void)
{
    ThreadDebugLogout("mThread_debug");

}

mThread_t *mThreadSchedule(void)
{
    mThread_t *threadToBeSchedule = NULL;

    /* FIXME: current supports only one thread and I think that there should be a activated thread list to reduce time usage */
    for(int8_t threadIndex = 0; threadIndex < MOC_MAX_THREAD_PRIO; threadIndex++){
        mThread_t *threadTmp = MOC_GET_THREAD_CPU0(threadIndex);
        
        if(threadTmp == NULL)
            continue;
        
        return threadTmp;
    }
    return NULL;

    

}