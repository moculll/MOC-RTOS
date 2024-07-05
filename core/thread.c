#include <stdalign.h>
#include <stdio.h>

#include <osImpl/irqImpl.h>
#include <core/thread.h>
#include <shellMgr/shellMgr.h>
#include <nrfx_systick.h>

#define CONFIG_THREAD_DEBUG_ASSERT 0
#if CONFIG_THREAD_DEBUG_ASSERT
#define ThreadDebugAssert() \
            do { \
                mEnableIrqImpl(); \
                while(1); \
           } while(0)
#else
#define ThreadDebugAssert()
#endif

#define CONFIG_THREAD_DEBUG_LOG 1
#if CONFIG_THREAD_DEBUG_LOG
/* FIXME: the debug function is not complete because we haven't impl the shellMgr done */
#define ThreadDebugLogout(format, ...) \
                do { \
                    nrfx_systick_delay_ms(10); \
                    shellMgr->outputString(format, ##__VA_ARGS__); \
                } while(0);
#else 

#define ThreadDebugLogout(format, ...)

#endif


volatile mKernel_t mKernel;

/* FIXME: current system supports only one cpu, current thread needs to be multiple */
volatile mThread_t *mThreadCurrent;

void mThreadDebugInfoOutput(mThread_t *thread)
{

    ThreadDebugLogout("Thread pid: %d, priority: %d, status: %d", thread->pid, thread->priority, thread->status);

}
void mThreadCurrentDebugInfoOutput(void)
{

    ThreadDebugLogout("Thread pid: %d, priority: %d, status: %d", mThreadCurrent->pid, mThreadCurrent->priority, mThreadCurrent->status);

}
/* FIXME: thread init function should support init status */
mThreadErrorType_t mThreadInit(void)
{

    mThreadErrorType_t err = mThreadErrorType_ERROR_NONE;
    
    for(int threadIndex = 0; threadIndex < MOC_MAX_THREAD_PRIO; threadIndex++)
        MOC_GET_THREAD_CPU0(threadIndex) = NULL;
    
    mThreadCurrent = NULL;
    
    return err;
}

/**
 * @brief we don't manage the mThreadCurrent there, this function is just for thread create & dlist management
 * 
 */
mThreadErrorType_t mThreadCreate(char *stackStart, int stackSize, int8_t priority, \
                                    mThreadCallback_t callback, void *context)
{
    
    MOC_THREAD_ASSERT(priority <= 32 && priority >= -32, mThreadErrorType_ERROR_THREADPRIOINVALID);
    /* ThreadDebugLogout("into thread create"); */

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
    /* FIXME: thread init status should be dummy, and there should be a function decide if the thread status change to prestart */
    newThread->status = mThreadStatus_PRESTART_BIT;
    newThread->priority = priority;
    newThread->sp = mThreadStackImplInit(callback, context, stackStart, stackSize);


    
    sys_dnode_init(&newThread->threadNode);
    if(MOC_GET_THREAD_BY_PRIO_CPU0(priority) == NULL){
        /* if current thread is the first of this priority, we set it as dlist */
        sys_dlist_init(&newThread->threadList);
        MOC_GET_THREAD_BY_PRIO_CPU0(priority) = newThread;
    }
    else{
        newThread->threadList = MOC_GET_THREAD_BY_PRIO_CPU0(priority)->threadList;
    }
    
    sys_dlist_append(&MOC_GET_THREAD_BY_PRIO_CPU0(priority)->threadList, &newThread->threadNode);



restore_irq:

    mRestoreIrq(state);

    mThreadYieldHigher();

    return err;
}




/**
 * @brief 
 *
 */
mThread_t *mThreadSchedule(void)
{
    mThread_t *threadToBeSchedule = NULL;

    /* FIXME: current supports only one thread and I think that there should be a activated thread list to reduce time usage */
    for(int8_t threadIndex = 0; threadIndex < MOC_MAX_THREAD_PRIO; threadIndex++){
        if(MOC_GET_THREAD_CPU0(threadIndex) == NULL)
            continue;
        mThread_t *threadForSwitch = NULL;

        
        sys_dnode_t *newNodeForSwitch = NULL;
        sys_dnode_t *tempNodeSafe = NULL;

        SYS_DLIST_FOR_EACH_NODE_SAFE(&MOC_GET_THREAD_CPU0(threadIndex)->threadList, newNodeForSwitch, tempNodeSafe) {
            
            threadForSwitch = CONTAINER_OF(newNodeForSwitch, mThread_t, threadNode);

            if(threadForSwitch->status & (mThreadStatus_ABORTING_BIT | mThreadStatus_PRESTART_BIT)){
                if(mThreadCurrent){
                    /* mThreadCurrentDebugInfoOutput(); */
                    if(mThreadCurrent->status & mThreadStatus_RUNNING_BIT)
                        mThreadCurrent->status = mThreadStatus_ABORTING_BIT;
                    else{
                        continue;
                    }
                }
                
                threadForSwitch->status = mThreadStatus_RUNNING_BIT;
                
                /* mThreadDebugInfoOutput(threadForSwitch); */

                return threadForSwitch;
            }

        }   


    }
    /* ThreadDebugLogout("schedule None"); */

    return NULL;

    

}