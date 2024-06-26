#include "thread.h"
#include "irqImpl.h"
#include "systickImpl.h"
#include "shellMgr.h"


uint32_t tickCount = 0;
void SysTick_Handler(void)
{


    mThreadYieldHigher();


}


void mocSystickInit(void)
{
    mocSystickInitImpl(1000);
}

