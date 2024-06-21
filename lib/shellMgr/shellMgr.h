#pragma once

#include "shellMgrImpl.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    void (*init)(void);
    void (*outputString)(char *string);
} shellMgr_t;

extern shellMgr_t *shellMgr;





#ifdef __cplusplus
}
#endif
