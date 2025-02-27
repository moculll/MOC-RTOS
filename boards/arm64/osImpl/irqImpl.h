#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline __attribute__((always_inline))
unsigned int mDisableIrqImpl(void)
{
    uint64_t daif;
    __asm__ volatile("mrs %0, DAIF" : "=r"(daif)); // 读取 DAIF 寄存器
    __asm__ volatile("msr DAIFSet, #0x2" : : : "memory"); // 禁用 IRQ (设置 DAIF.I)
    return (unsigned int)(daif & 0x2); // 返回之前的 IRQ 状态
}

static inline __attribute__((always_inline)) __attribute__((used))
unsigned int mEnableIrqImpl(void)
{
    uint64_t daif;
    __asm__ volatile("mrs %0, DAIF" : "=r"(daif)); // 读取 DAIF 寄存器
    __asm__ volatile("msr DAIFClr, #0x2" : : : "memory"); // 启用 IRQ (清除 DAIF.I)
    return (unsigned int)(daif & 0x2); // 返回之前的 IRQ 状态
}

static inline __attribute__((always_inline))
void mRestoreIrqImpl(unsigned int state)
{
    if (state) {
        __asm__ volatile("msr DAIFSet, #0x2" : : : "memory"); // 恢复 IRQ 状态
    } else {
        __asm__ volatile("msr DAIFClr, #0x2" : : : "memory"); // 恢复 IRQ 状态
    }
}

static inline __attribute__((always_inline))
uint8_t mIsIrqIn(void)
{
    uint64_t daif;
    __asm__ volatile("mrs %0, DAIF" : "=r"(daif)); // 读取 DAIF 寄存器
    return (uint8_t)((daif & 0x2) == 0); // 检查 IRQ 是否启用
}

static inline __attribute__((always_inline))
void mThreadYieldHigher(void)
{
    // Cortex-A53 没有 PENDSV，可以使用 SVC 或 WFE/WFI 实现线程切换
    __asm__ volatile("svc #0" : : : "memory"); // 触发 SVC 异常
    __asm__ volatile("isb" : : : "memory"); // 确保指令流水线刷新
}

#ifdef __cplusplus
}
#endif