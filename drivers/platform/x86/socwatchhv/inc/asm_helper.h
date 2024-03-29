/* SPDX-License-Identifier: GPL-2.0 AND BSD-3-Clause
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2014 - 2019 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * Contact Information:
 * SoC Watch Developer Team <socwatchdevelopers@intel.com>
 * Intel Corporation,
 * 1300 S Mopac Expwy,
 * Austin, TX 78746
 *
 * BSD LICENSE
 *
 * Copyright(c) 2014 - 2019 Intel Corporation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ASM_HELPER_H_
#define _ASM_HELPER_H_

#include <linux/version.h>

#if KERNEL_VERSION(4, 1, 0) > LINUX_VERSION_CODE

#include <asm/dwarf2.h>
#include <asm/calling.h>

#else

#ifdef CONFIG_AS_CFI

#define CFI_STARTPROC         (.cfi_startproc)
#define CFI_ENDPROC           (.cfi_endproc)
#define CFI_ADJUST_CFA_OFFSET (.cfi_adjust_cfa_offset)
#define CFI_REL_OFFSET        (.cfi_rel_offset)
#define CFI_RESTORE           (.cfi_restore)

#else

.macro cfi_ignore a = 0, b = 0, c = 0, d = 0
.endm

#define CFI_STARTPROC           cfi_ignore
#define CFI_ENDPROC             cfi_ignore
#define CFI_ADJUST_CFA_OFFSET   cfi_ignore
#define CFI_REL_OFFSET          cfi_ignore
#define CFI_RESTORE             cfi_ignore
#endif

#ifdef CONFIG_X86_64
	.macro SAVE_C_REGS_HELPER offset = 0 rax = 1 rcx = 1 r8910 = 1 r11 = 1
	.if \r11
	movq % r11, 6*8+\offset(%rsp)
	CFI_REL_OFFSET r11, \offset
	.endif
	.if \r8910
	movq % r10, 7*8+\offset(%rsp)
	CFI_REL_OFFSET r10, \offset

	movq % r9,  8*8+\offset(%rsp)
	CFI_REL_OFFSET r9, \offset

	movq % r8,  9*8+\offset(%rsp)
	CFI_REL_OFFSET r8, \offset
	.endif
	.if \rax
	movq % rax, 10*8+\offset(%rsp)
	CFI_REL_OFFSET rax, \offset
	.endif
	.if \rcx
	movq % rcx, 11*8+\offset(%rsp)
	CFI_REL_OFFSET rcx, \offset
	.endif
	movq % rdx, 12*8+\offset(%rsp)
	CFI_REL_OFFSET rdx, \offset

	movq % rsi, 13*8+\offset(%rsp)
	CFI_REL_OFFSET rsi, \offset

	movq % rdi, 14*8+\offset(%rsp)
	CFI_REL_OFFSET rdi, \offset
	.endm
	.macro SAVE_C_REGS offset = 0
	SAVE_C_REGS_HELPER \offset, 1, 1, 1, 1
	.endm
	.macro SAVE_EXTRA_REGS offset = 0
	movq % r15, 0*8+\offset(%rsp)
	CFI_REL_OFFSET r15, \offset

	movq % r14, 1*8+\offset(%rsp)
	CFI_REL_OFFSET r14, \offset

	movq % r13, 2*8+\offset(%rsp)
	CFI_REL_OFFSET r13, \offset

	movq % r12, 3*8+\offset(%rsp)
	CFI_REL_OFFSET r12, \offset

	movq % rbp, 4*8+\offset(%rsp)
	CFI_REL_OFFSET rbp, \offset

	movq % rbx, 5*8+\offset(%rsp)
	CFI_REL_OFFSET rbx, \offset
	.endm

	.macro RESTORE_EXTRA_REGS offset = 0
	movq 0*8+\offset(%rsp), % r15
	CFI_RESTORE r15
	movq 1*8+\offset(%rsp), % r14
	CFI_RESTORE r14
	movq 2*8+\offset(%rsp), % r13
	CFI_RESTORE r13
	movq 3*8+\offset(%rsp), % r12
	CFI_RESTORE r12
	movq 4*8+\offset(%rsp), % rbp
	CFI_RESTORE rbp
	movq 5*8+\offset(%rsp), % rbx
	CFI_RESTORE rbx
	.endm
	.macro RESTORE_C_REGS_HELPER rstor_rax = 1, rstor_rcx = 1, rstor_r11 = 1, rstor_r8910 = 1, rstor_rdx = 1
	.if \rstor_r11
	movq 6*8(%rsp), % r11
	CFI_RESTORE r11
	.endif
	.if \rstor_r8910
	movq 7*8(%rsp), % r10
	CFI_RESTORE r10
	movq 8*8(%rsp), % r9
	CFI_RESTORE r9
	movq 9*8(%rsp), % r8
	CFI_RESTORE r8
	.endif
	.if \rstor_rax
	movq 10*8(%rsp), % rax
	CFI_RESTORE rax
	.endif
	.if \rstor_rcx
	movq 11*8(%rsp), % rcx
	CFI_RESTORE rcx
	.endif
	.if \rstor_rdx
	movq 12*8(%rsp), % rdx
	CFI_RESTORE rdx
	.endif
	movq 13*8(%rsp), % rsi
	CFI_RESTORE rsi
	movq 14*8(%rsp), % rdi
	CFI_RESTORE rdi
	.endm
	.macro RESTORE_C_REGS
	RESTORE_C_REGS_HELPER 1, 1, 1, 1, 1
	.endm

	.macro ALLOC_PT_GPREGS_ON_STACK addskip = 0
	subq	$15*8+\addskip, % rsp
	CFI_ADJUST_CFA_OFFSET 15*8+\addskip
	.endm

	.macro REMOVE_PT_GPREGS_FROM_STACK addskip = 0
	addq $15*8+\addskip, % rsp
	CFI_ADJUST_CFA_OFFSET - (15*8+\addskip)
	.endm

	.macro SAVE_ALL
	ALLOC_PT_GPREGS_ON_STACK
	SAVE_C_REGS
	SAVE_EXTRA_REGS
	.endm

	.macro RESTORE_ALL
	RESTORE_EXTRA_REGS
	RESTORE_C_REGS
	REMOVE_PT_GPREGS_FROM_STACK
	.endm
#endif /* CONFIG_X86_64 */
#endif

#endif
