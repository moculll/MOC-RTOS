#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <shellMgr/shellMgr.h>
#include <core/mocThread.h>
#include <drivers/uart/pl011.h>
#include <mMemoryMgr/mMemoryMgr.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <DebugTrace/DebugTrace.h>

void test_entry()
{

    MOC::DebugTrace::printfEx(1, "hello main thread.");
}

int main()
{
   test_entry();
   return 0;
}
