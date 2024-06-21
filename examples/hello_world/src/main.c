#include <stdio.h>
#include <nrfx_uarte.h>
#include "shellMgr.h"

void main()
{

    static char buffer;
    nrfx_uarte_t shitinstance = NRFX_UARTE_INSTANCE(0);

    while(1){
        nrfx_uarte_rx(&shitinstance, &buffer, 1);

    }

    
}
