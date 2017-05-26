/******************************************************************************

    @file    IntrOS: osport.h
    @author  Rajmund Szymanski
    @date    25.05.2017
    @brief   IntrOS port definitions for ATtiny817 uC.

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

#ifndef __INTROSPORT_H
#define __INTROSPORT_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <osconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_TIMER
#define  OS_TIMER             0
#endif

#if      OS_TIMER
#error   osconfig.h: Incorrect OS_TIMER value! This port does not support tick-less mode.
#endif

/* -------------------------------------------------------------------------- */

#ifndef CPU_FREQUENCY
#define CPU_FREQUENCY         F_CPU
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_FREQUENCY
#define  OS_FREQUENCY      1000 /* Hz */
#endif

#if     (OS_FREQUENCY > 1000)
#error   osconfig.h: Incorrect OS_FREQUENCY value!
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_STACK_SIZE
#define  OS_STACK_SIZE       64 /* default task stack size in bytes           */
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_ASSERT
#define  OS_ASSERT            0 /* do not include standard assertions         */
#endif

#if     (OS_ASSERT == 0)
#ifndef  NDEBUG
#define  NDEBUG
#endif
#endif

#include <assert.h>

/* -------------------------------------------------------------------------- */

#define  __INIT(N)          __attribute__((used, naked, optimize("s"), section(".init"#N)))
#define  __FINI(N)          __attribute__((used, naked, optimize("s"), section(".fini"#N)))

/* -------------------------------------------------------------------------- */

#ifndef  __CONSTRUCTOR
#define  __CONSTRUCTOR      __INIT(5)
#endif
#ifndef  __NO_RETURN
#define  __NO_RETURN        __attribute__(( noreturn ))
#endif
#ifndef  __STATIC_INLINE
#define  __STATIC_INLINE      static inline
#endif
#ifndef  __WFI
#define  __WFI
#endif

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#endif//__INTROSPORT_H
