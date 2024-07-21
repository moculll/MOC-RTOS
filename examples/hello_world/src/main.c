#include <stdio.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <string.h>
#include <shellMgr/shellMgr.h>
#include <core/thread.h>
#include <nrfx_systick.h>
#include <mMemoryMgr/mMemoryMgr.h>
static uint8_t stackStorge[1024];

static void testThreadCallback(void *arg)
{
    mMemoryMgrInit(40, 1, 0);

    
    while(1){
        /* char *space = mMemoryMalloc(40);
        strcpy(space, "fuckyou\r\n");

        shellMgr->outputString(space);
        mMemoryFree(space); */
        printf("[thread1]clock:: %d\r\n", nrf_systick_val_get());
        /* shellMgr->outputString("[thread1]clock: %ld\r\n", nrf_systick_val_get()); */

        nrfx_systick_delay_ms(500);
    }
        

}

void main()
{
    
    static char buffer;
    nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

    printf("created main\r\n");

    memset((void *)&stackStorge, 0, sizeof(stackStorge));



    mThreadCreate(stackStorge, sizeof(stackStorge), 2, testThreadCallback, NULL);

    while(1){

        nrfx_uarte_rx(&instance, &buffer, 1);
        nrfx_systick_delay_ms(50);

    }

    
}
