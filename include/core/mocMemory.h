#pragma once


#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*init)(void);
    /* FIXME: current sshellMgr doesn't support format function */
    void (*malloc)(size_t len);
} mMemory_t;

extern mMemory_t *mMemoryMgr;

#ifdef __cplusplus
}
#endif