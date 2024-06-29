
#include <core/thread.h>
mThread_t *mGetCurrentThread(void);

void __attribute__((naked)) __attribute__((used)) PendSV_Handler(void) {
    __asm__ volatile (
    /* PendSV handler entry point */
    /* save context by pushing unsaved registers to the stack */
    /* {r0-r3,r12,LR,PC,xPSR,s0-s15,FPSCR} are saved automatically on exception entry */
    ".thumb_func                      \n"
    ".syntax unified                  \n"
    /* skip context saving if mGetCurrentThread == NULL */
    "ldr    r1, =mThreadCurrent  \n" /* r1 = &mGetCurrentThread  */
    "push   {r4, lr}                  \n" /* push r4 and exception return code */
    "ldr    r4, [r1]                  \n" /* r4 = mGetCurrentThread   */

    "cpsid  i                         \n" /* Disable IRQs during sched_run */
    "bl     mThreadSchedule                 \n" /* perform scheduling */
    "cpsie  i                         \n" /* Re-enable interrupts */
    "cmp    r0, r4                    \n" /* if r0 == r1: (new thread == old
                                               thread, no switch required) */
    "it     eq                        \n"
    "popeq  {r4, pc}                  \n" /* Pop exception to pc to return */

    "mov    r1, r4                    \n" /* save mGetCurrentThread in r1 */
    "pop    {r4, lr}                  \n" /* Pop exception from the exception stack */


    "cmp    r1, #0                    \n"  /* Compare r1 with 0 */
    "beq    update_and_restore_context\n" /* If r1 == 0, branch to update_and_restore_context */
    

    "mrs    r2, psp                   \n" /* get stack pointer from user mode */
    
    
    "tst    lr, #0x10                 \n"
    "it     eq                        \n"
    "vstmdbeq r2!, {s16-s31}          \n" /* save FPU registers if FPU is used */

    "stmdb  r2!,{r4-r11,lr}           \n" /* save regs, including lr */

    "str    r2, [r1]                  \n" /* write r0 to thread->sp */
    "ldr    r1, =mThreadCurrent       \n"  /* Load address of mThreadCurrent into r1 */
    "str    r0, [r1]                  \n"  /* Store r0 into mThreadCurrent */


    "b      restore_context           \n" /* Branch to restore_context */

    /* Update mThreadCurrent if r1 == 0, then branch to restore_context */
    "update_and_restore_context:      \n"
    
    "ldr    r1, =mThreadCurrent       \n"   /* Load address of mThreadCurrent into r1 */
    "str    r0, [r1]                  \n"    /* Store r0 into mThreadCurrent */

    /* current thread context is now saved */
    "restore_context:                 \n" /* Label to skip thread state saving */

    "ldr    r0, [r0]                  \n" /* load tcb->sp to register 0 */

    "ldmia  r0!, {r4-r11,lr}          \n" /* restore other registers, including lr */

    "tst    lr, #0x10                 \n"
    "it     eq                        \n"
    "vldmiaeq r0!, {s16-s31}          \n" /* load FPU registers if saved */

    "msr    psp, r0                   \n" /* restore user mode SP to PSP reg */
    "bx     lr                        \n" /* load exception return value to PC,
                                           * causes end of exception*/

    /* return from exception mode to application mode */
    /* {r0-r3,r12,LR,PC,xPSR,s0-s15,FPSCR} are restored automatically on exception return */
     ".ltorg                           \n" /* literal pool needed to access
                                            * mGetCurrentThread */
     :
     :
     :
    );
}
