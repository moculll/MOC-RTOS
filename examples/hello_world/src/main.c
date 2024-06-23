#include <stdio.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <string.h>
#include "shellMgr.h"
#include "thread.h"
#include <nrfx_systick.h>
char stackStorge[512];

static void testThreadCallback(void *arg)
{
    
    while(1){
        
        shellMgr->outputString("current is in thread!\r\n");
        nrfx_systick_delay_ms(1000);
    }
        

}

/* FIXME: we haven't put the main function into thread, so it can't run with other threads at the same time */
void main()
{

    static char buffer;
    nrfx_uarte_t shitinstance = NRFX_UARTE_INSTANCE(0);

    memset((void *)&stackStorge, 0, sizeof(stackStorge));


    mThreadCreate(stackStorge, sizeof(stackStorge), 1, testThreadCallback, NULL);

    while(1){
        nrfx_uarte_rx(&shitinstance, &buffer, 1);

    }

    
}
