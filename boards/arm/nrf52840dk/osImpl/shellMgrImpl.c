#include "shellMgr.h"
#include <nrfx_uarte.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FIXME: these defines are used temporarily, we need to move them to global space */
#define SHELLMGRUARTNUM 0
#define SHELLBUFFERLENGTH 1024
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

static void outputString(char *string)
{
    size_t length = strlen(string);
    if(length < SHELLBUFFERLENGTH) {
        memcpy(shell_buffer, string, length);
        nrfx_err_t err = nrfx_uarte_tx(&instance, shell_buffer, length);
        return;
    }
    size_t left = length % SHELLBUFFERLENGTH + 1;
    for(int i = 0; i < length / SHELLBUFFERLENGTH; i++) {
        memcpy(shell_buffer, string, SHELLBUFFERLENGTH);
        nrfx_err_t err = nrfx_uarte_tx(&instance, shell_buffer, SHELLBUFFERLENGTH);
        string += SHELLBUFFERLENGTH;
    }
    memcpy(shell_buffer, string, left);
    nrfx_err_t err = nrfx_uarte_tx(&instance, shell_buffer, left);
    return;
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
    shellMgr->outputString("Welcome to MOCOS!\r\n");
}

