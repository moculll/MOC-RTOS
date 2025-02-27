#include <core/mocStartEntry.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shellMgr/shellMgr.h>
#include <core/mocThread.h>
#include <osImpl/DebugTraceImpl.h>

extern int main();

static uint8_t stackStorge[1024];

void mocStartEntry(void)
{

    MOC::DebugTraceImpl::init();
    MOC::DebugTrace::addLogLevel("OS", 0);
    MOC::DebugTrace::addLogLevel("APP", 1);
    MOC::DebugTrace::printfEx(0, "DebugTrace inited.");
    /* mThreadCreate(stackStorge, sizeof(stackStorge), 1, main, NULL); */
    main();


}
