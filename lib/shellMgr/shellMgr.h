#pragma once

#include <osImpl/shellMgrImpl.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    void (*init)(void);
    /* FIXME: current sshellMgr doesn't support format function */
    void (*outputString)(char *string);
} shellMgr_t;

extern shellMgr_t *shellMgr;





#ifdef __cplusplus
}
#endif
