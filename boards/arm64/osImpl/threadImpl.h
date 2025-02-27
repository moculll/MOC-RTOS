#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*mThreadCallback_t)(void *);

char *mThreadStackImplInit(mThreadCallback_t callback, void *context, void *stackStart, int stackSize);


#ifdef __cplusplus
}
#endif