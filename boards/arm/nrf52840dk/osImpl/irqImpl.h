#pragma once

#include <stdint.h>
#include "nrf52840.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline __attribute__((always_inline))
unsigned int mDisableIrqImpl(void)
{
    uint32_t mask = __get_PRIMASK();
    __ASM volatile ("cpsid i" : : : "memory");
    return mask;
}

static inline __attribute__((always_inline)) __attribute__((used))
unsigned int mEnableIrqImpl(void)
{
    unsigned result = __get_PRIMASK();
    __ASM volatile ("cpsie i" : : : "memory");
    return result;
}

static inline __attribute__((always_inline))
void mRestoreIrq(unsigned int state)
{
    __set_PRIMASK(state);
}

static inline __attribute__((always_inline))
uint8_t mIsIrqIn(void)
{
    return (__get_IPSR() & 0xFF);
}


static inline __attribute__((always_inline))
void mThreadYieldHigher(void)
{
    /* trigger the PENDSV interrupt to run scheduler and schedule new thread if
     * applicable */
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
    /* flush the pipeline. Otherwise we risk that subsequent instructions are
     * executed before the IRQ has actually triggered */
    __ISB();
}

#ifdef __cplusplus
}
#endif