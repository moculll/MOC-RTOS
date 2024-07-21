#pragma once

#include <osImpl/shellMgrImpl.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    void (*init)(void);
    /* FIXME: current sshellMgr doesn't support format function */
    void (*outputString)(const char *string, ...);
    size_t (*outputStringSize)(const char *string, size_t size);
    size_t (*getString)(char *string, size_t size);
} shellMgr_t;

extern shellMgr_t *shellMgr;





#ifdef __cplusplus
}
#endif
