#include "shellMgr.h"


static void init();


static shellMgr_t shellMgrObj = {.init = init};

shellMgr_t *shellMgr = &shellMgrObj;

static void init()
{
    shellMgrImplInit();
    
}



