#include <shellMgr/shellMgr.h>
#include <nrfx_uarte.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define SHELLMGR_ASSERT(condition, format) \
            do { \
                if(!(condition)){ \
                    int assertLen = sprintf(shell_buffer, "[Warning][%s](shellMgrImpl.c line:%d): %s\r\n", __func__, __LINE__, format); \
                    nrfx_uarte_tx(&instance, shell_buffer, assertLen); \
                } \
            } while(0)

/* FIXME: these defines are used temporarily, we need to move them to global space */
#define SHELLMGRUARTNUM 0
#define SHELLBUFFERLENGTH 1024

/* current static define is because nrfx_uarte buffer has to be in ram */
static uint8_t shell_buffer[SHELLBUFFERLENGTH];

static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(SHELLMGRUARTNUM);


static void event_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    if(p_event->type == NRFX_UARTE_EVT_RX_DONE) {
        if(*(char *)p_event->data.rxtx.p_data != '\r')
            shellMgr->outputString(p_event->data.rxtx.p_data);
        else {
            shellMgr->outputString("\r\n");
        }
    }

}

static void outputString(const char *string, ...)
{
    /* FIXME: this function needs mutex and nrfx_uarte_tx needs semaphore working with event_handler ￪ */
    va_list args;
    size_t length;
    va_start(args, string);
    length = vsprintf(shell_buffer, string, args);
    va_end(args);
    nrfx_err_t err = nrfx_uarte_tx(&instance, shell_buffer, length);
    
    SHELLMGR_ASSERT(length <= SHELLBUFFERLENGTH, "SHELLBUFFERLENGTH is too short, lost some string outputs!");

}


void shellMgrImplInit(void)
{
    memset(shell_buffer, 0, SHELLBUFFERLENGTH);
    nrfx_uarte_config_t uart_cfg = {
        .pseltxd   = 6,
        .pselrxd   = 8,
        .pselcts   = NRF_UARTE_PSEL_DISCONNECTED,
        .pselrts   = NRF_UARTE_PSEL_DISCONNECTED,
        .p_context = NULL,
        .baudrate  = NRF_UARTE_BAUDRATE_115200,
        .interrupt_priority = 7,
        .hwfc      = NRF_UARTE_HWFC_DISABLED,
        .parity    = NRF_UARTE_PARITY_EXCLUDED,
    };
    nrfx_uarte_init(&instance, &uart_cfg, event_handler);
    shellMgr->outputString = outputString;
    shellMgr->outputString("\r\nWelcome to MOCOS!\r\n");
}

