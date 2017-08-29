/******************************************************************************

    @file    IntrOS: oscore.h
    @author  Rajmund Szymanski
    @date    29.08.2017
    @brief   IntrOS port file for AVR8 uC.

 ******************************************************************************

    IntrOS - Copyright (C) 2013 Rajmund Szymanski.

    This file is part of IntrOS distribution.

    IntrOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 3 of the License,
    or (at your option) any later version.

    IntrOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#ifndef __INTROSCORE_H
#define __INTROSCORE_H

#include <osbase.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_STACK_SIZE
#define  OS_STACK_SIZE       64 /* default task stack size in bytes           */
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_FUNCTIONAL

#define  OS_FUNCTIONAL        0 /* c++ functional library header not included */

#elif    OS_FUNCTIONAL

#error   c++ functional library not allowed for this compiler.

#endif //OS_FUNCTIONAL

/* -------------------------------------------------------------------------- */

typedef  uint8_t              lck_t;
typedef  uint8_t              stk_t;

/* -------------------------------------------------------------------------- */

extern   stk_t              __stack[];
#define  MAIN_TOP           __stack+1

/* -------------------------------------------------------------------------- */

// task context

typedef struct __ctx ctx_t;

struct __ctx
{
#if defined(__AVR_TINY__)
	char     regs[2];  // call-saved registers (r18-r19)
#else
	char     regs[16]; // call-saved registers (r2-r17)
#endif
	unsigned fp;       // frame pointer        (r29:r28)
	void   * sp;       // stack pointer        (SPH:SPL)
	char     sreg;     // status register      (SREG)
	fun_t  * pc;       // return address       (PC)
#if defined(__AVR_3_BYTE_PC__) && __AVR_3_BYTE_PC__
	char     pc_;      // high part of return address
#endif
};

#if defined(__AVR_3_BYTE_PC__) && __AVR_3_BYTE_PC__
#define _CTX_INIT() { { 0 }, 0, 0, 0, 0, 0 }
#else
#define _CTX_INIT() { { 0 }, 0, 0, 0, 0 }
#endif

/* -------------------------------------------------------------------------- */

__STATIC_INLINE
void port_ctx_init( ctx_t *ctx, stk_t *sp, fun_t *pc )
{
	ctx->sp = sp - 1;
	ctx->pc = pc;
}

/* -------------------------------------------------------------------------- */

// get current stack pointer
__STATIC_INLINE
void * port_get_sp( void )
{
	return (void *) SP;
}

/* -------------------------------------------------------------------------- */

#define  port_get_lock()      SREG
#define  port_put_lock(lck)   SREG = lck

#define  port_set_lock()      cli()
#define  port_clr_lock()      sei()

#define  port_sys_lock()      do { lck_t __LOCK = port_get_lock(); port_set_lock()
#define  port_sys_unlock()         port_put_lock(__LOCK); } while(0)

#define  port_isr_lock()      do { port_set_lock()
#define  port_isr_unlock()         port_clr_lock(); } while(0)

#define  port_cnt_lock()      do { lck_t __LOCK = port_get_lock(); port_set_lock()
#define  port_cnt_unlock()         port_put_lock(__LOCK); } while(0)

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif//__INTROSCORE_H
