#include <core/thread.h>
#include <osImpl/irqImpl.h>
#include <osImpl/systickImpl.h>
#include <shellMgr/shellMgr.h>

void SysTick_Handler(void)
{


    mThreadYieldHigher();


}


void mocSystickInit(void)
{
    mocSystickInitImpl(1000);
}

