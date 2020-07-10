/*
    Copyright © 2019 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <stdarg.h>
#include <stdint.h>

#include "config.h"
#include "support.h"
#include "tlsf.h"
#include "mmu.h"
#include "devicetree.h"
#include "m68k.h"
#include "hunkloader.h"
#include "duffcopy.h"
#include "emulogo.h"
#include "emufeatures.h"
#include "registerallocator.h"

extern uint64_t mmu_user_L1[512];
extern uint64_t mmu_user_L2[4*512];

void _start();

asm("   .section .startup           \n"
"       .globl _start               \n"
"       .type _start,%function      \n"
"_start:                            \n"
"       mrs     r4, cpsr_all        \n" /* Check if in hypervisor mode */
"       and     r4, r4, #0x1f       \n"
"       mov     r8, #0x1a           \n"
"       cmp     r4, r8              \n"
"       beq     leave_hyper         \n"
"continue_boot:                     \n"
"       cps     #0x13               \n" /* Should be in SVC (supervisor) mode already, but just incase.. */
#if EMU68_HOST_BIG_ENDIAN
"       setend  be                  \n" /* Switch to big endian mode */
#endif
"       ldr     sp, tmp_stack_ptr   \n"
"       mrc     p15,0,r4,c1,c0,2    \n" /* Enable signle and double VFP coprocessors */
"       orr     r4, r4, #0x00f00000 \n" /* This is necessary since gcc might want to use vfp registers  */
"       mcr     p15,0,r4,c1,c0,2    \n" /* Either as cache for general purpose regs or e.g. for division. This is the case with gcc9 */
"       isb                         \n" /* Synchronize the pipeline */
"       isb                         \n" /* Synchronize the pipeline */
"       isb                         \n" /* Synchronize the pipeline */
"       isb                         \n" /* Synchronize the pipeline */
"       isb                         \n" /* Synchronize the pipeline */
"       vmrs    r4,fpexc            \n" /* Fetch fpexc */
"       orr     r4,r4,#0x40000000   \n" /* Set enable bit */
"       vmsr    fpexc,r4            \n" /* Enable VFP now */
"       mov     r0,#0               \n"
"       dsb                         \n"
"       mcr     p15,0,r0,c7,c5,0    \n" /* invalidate icache */
"       isb                         \n"
"       ldr     r4, mmu_table_ptr   \n" /* Load MMU table pointer */
"       mcr     p15,0,r4,c2,c0,0    \n" /* Write page_dir address to ttbr0 */
"       mov     r8, #0              \n"
"       mcr     p15,0,r8,c2,c0,2    \n" /* Write ttbr control N = 0 (use only ttbr0) */
"       mov     r4, #1              \n"
"       mcr     p15,0,r4,c3,c0,0    \n" /* Set domains - Dom0 is usable, rest is disabled */
"       mrc     p15,0,r4,c1,c0,0    \n" /* Load control register */
"       orr     r4,r4,#8388608      \n" /* v6 page tables, subpages disabled */
"       orr     r4,r4,#1            \n" /* Enable MMU */
#if EMU68_HOST_BIG_ENDIAN
"       orr     r4,r4,#1<<25        \n" /* MMU tables in big endian */
#endif
"       dsb                         \n" /* DSB */
"       mcr     p15,0,r4,c1,c0,0    \n" /* Set control register and thus really enable mmu */
"       isb                         \n"
"       ldr r4, =__bss_start        \n" /* Clear .bss */
"       ldr r9, =__bss_end          \n"
"       mov r5, #0                  \n"
"       mov r6, #0                  \n"
"       mov r7, #0                  \n"
"       mov r8, #0                  \n"
"       b       2f                  \n"
"1:                                 \n"
"       stmia r4!, {r5-r8}          \n"
"2:                                 \n"
"       cmp r4, r9                  \n"
"       blo 1b                      \n"
"       ldr     r4, boot_address    \n"
"       isb                         \n" /* ISB */
"       bx      r4                  \n"
"leave_hyper:                       \n"
#if EMU68_HOST_BIG_ENDIAN
"       setend  be                  \n"
#endif
"       adr     r4, continue_boot   \n"
"       .byte   0x04,0xf3,0x2e,0xe1 \n" /* msr     ELR_hyp, r4  */
"       mrs     r4, cpsr_all        \n"
"       and     r4, r4, #0x1f       \n"
"       orr     r4, r4, #0x13       \n"
"       .byte   0x04,0xf3,0x6e,0xe1 \n" /* msr     SPSR_hyp, r4 */
"       .byte   0x6e,0x00,0x60,0xe1 \n" /* eret                 */
"       .section .text              \n"
".byte 0                            \n"
".string \"$VER: Emu68.img " VERSION_STRING_DATE "\"\n"
".byte 0                            \n"
"\n\t\n\t"
);

void platform_init()
{
}

void platform_post_init()
{
}
