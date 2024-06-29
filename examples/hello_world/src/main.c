#include <stdio.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <string.h>
#include <shellMgr/shellMgr.h>
#include <core/thread.h>
#include <nrfx_systick.h>
static uint8_t stackStorge[1024];
char logbuffer1[1000];
#define MainDebugLogout(format, ...) \
                do { \
                    snprintf(logbuffer1, 1000, "[%s]"format"\r\n", __func__, ##__VA_ARGS__); \
                    shellMgr->outputString(logbuffer1); \
                } while(0);
static void testThreadCallback(void *arg)
{
    int a = 0;
    while(1){
        
        MainDebugLogout("in thread1 %ld", nrf_systick_val_get());

        nrfx_systick_delay_ms(1000);
    }
        

}

void main()
{
    
    static char buffer;
    nrfx_uarte_t shitinstance = NRFX_UARTE_INSTANCE(0);

    shellMgr->outputString("created main\r\n");
    nrfx_systick_delay_ms(50);

    memset((void *)&stackStorge, 0, sizeof(stackStorge));


    mThreadCreate(stackStorge, sizeof(stackStorge), 2, testThreadCallback, NULL);

    while(1){

        if(NRFX_SUCCESS == nrfx_uarte_rx(&shitinstance, &buffer, 1))
            shellMgr->outputString(&buffer);

    }

    
}
