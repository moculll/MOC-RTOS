#include <core/mocStartEntry.h>
#include <stdio.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <string.h>
#include <shellMgr/shellMgr.h>
#include <core/thread.h>
#include <nrfx_systick.h>

extern void main();

static uint8_t stackStorge[1024];




void mocStartEntry(void)
{

    mThreadCreate(stackStorge, sizeof(stackStorge), 1, main, NULL);


}