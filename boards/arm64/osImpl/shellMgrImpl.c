#include <shellMgr/shellMgr.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define SHELLMGR_ASSERT(condition, format) \
            do { \
                if(!(condition)){ \
                    int assertLen = sprintf(shell_buffer, "[Warning][%s](shellMgrImpl.c line:%d): %s\r\n", __func__, __LINE__, format); \
                } \
            } while(0)

/* FIXME: these defines are used temporarily, we need to move them to global space */
#define SHELLMGRUARTNUM 0
#define SHELLBUFFERLENGTH 1024

/* current static define is because nrfx_uarte buffer has to be in ram */
static uint8_t shell_buffer[SHELLBUFFERLENGTH];



static void outputString(const char *string, ...)
{
    /* FIXME: this function needs mutex and nrfx_uarte_tx needs semaphore working with event_handler ï¿? */
    va_list args;
    size_t length;
    va_start(args, string);
    length = vsprintf(shell_buffer, string, args);
    va_end(args);
    
    
    SHELLMGR_ASSERT(length <= SHELLBUFFERLENGTH, "SHELLBUFFERLENGTH is too short, lost some string outputs!");

}

static size_t outputStringSize(const char *string, size_t size)
{
    memcpy(shell_buffer, string, size);
    
        return size;
    return 0;
}


static size_t getString(char *string, size_t size)
{
    
        return size;
    return 0;

}



void shellMgrImplInit(void)
{
    memset(shell_buffer, 0, SHELLBUFFERLENGTH);
 
    shellMgr->outputString = outputString;
    shellMgr->getString = getString;
    shellMgr->outputStringSize = outputStringSize;
    shellMgr->outputString("\r\nWelcome to MOCOS!\r\n");
}

