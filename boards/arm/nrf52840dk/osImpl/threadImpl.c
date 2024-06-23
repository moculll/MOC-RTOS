#include "thread.h"

/* the unique val to mark the start of a stack */
#define STACK_MARKER (0x15292061)

/**
 * @brief   Initial program status register value for a newly created thread
 *
 * In the initial state, only the Thumb mode-bit is set
 */
#define INITIAL_XPSR                (0x01000000)

/**
 * @brief   ARM Cortex-M specific exception return value, that triggers the
 *          return to the task mode stack pointer
 */
#define EXCEPT_RET_TASK_MODE        (0xfffffffd)

char *mThreadStackImplInit(mThreadCallback_t callback, void *context, void *stackStart, int stackSize)
{
    uint32_t *stk;
    stk = (uint32_t *)((uintptr_t)stackStart + stackSize);

    /* adjust to 32 bit boundary by clearing the last two bits in the address */
    stk = (uint32_t *)(((uint32_t)stk) & ~((uint32_t)0x3));

    /* stack start marker */
    stk--;
    *stk = STACK_MARKER;

    /* make sure the stack is double word aligned (8 bytes) */
    /* This is required in order to conform with Procedure Call Standard for the
     * ARM® Architecture (AAPCS) */
    /* http://infocenter.arm.com/help/topic/com.arm.doc.ihi0042e/IHI0042E_aapcs.pdf */
    if (((uint32_t) stk & 0x7) != 0) {
        /* add a single word padding */
        --stk;
        *stk = ~((uint32_t)STACK_MARKER);
    }

    /* ****************************** */
    /* Automatically popped registers */
    /* ****************************** */

    /* The following eight stacked registers are popped by the hardware upon
     * return from exception. (bx instruction in select_and_restore_context) */

    /* xPSR - initial status register */
    stk--;
    *stk = (uint32_t)INITIAL_XPSR;
    /* pc - initial program counter value := thread entry function */
    stk--;
    *stk = (uint32_t)callback;
    /* lr - contains the return address when the thread exits */
    stk--;

    /* FIXME: haven't done the schedue function, this needs to be the exit entry of a thread */
    *stk = (uint32_t)callback;

    /* r12 */
    stk--;
    *stk = 0;
    /* r3 - r1 */
    for (int i = 3; i >= 1; i--) {
        stk--;
        *stk = i;
    }
    /* r0 - contains the thread function parameter */
    stk--;
    *stk = (uint32_t)context;

    /* ************************* */
    /* Manually popped registers */
    /* ************************* */

    /* The following registers are not handled by hardware in return from
     * exception, but manually by select_and_restore_context.
     * For the Cortex-M0, Cortex-M0+ and Cortex-M23 we write registers R11-R4
     * in two groups to allow for more efficient context save/restore code.
     * For the Cortex-M3, Cortex-M33 and Cortex-M4 we write them continuously
     * onto the stack as they can be read/written continuously by stack
     * instructions. */

    /* exception return code  - return to task-mode process stack pointer */
    stk--;
    *stk = (uint32_t)EXCEPT_RET_TASK_MODE;

    /* r11 - r4 */
    for (int i = 11; i >= 4; i--) {
        stk--;
        *stk = i;
    }
    /* The returned stack pointer will be aligned on a 32 bit boundary not on a
     * 64 bit boundary because of the odd number of registers above (8+9).
     * This is not a problem since the initial stack pointer upon process entry
     * _will_ be 64 bit aligned (because of the cleared bit 9 in the stacked
     * xPSR and aligned stacking of the hardware-handled registers). */

    return (char *)stk;

}


