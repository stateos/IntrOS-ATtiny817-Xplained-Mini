/******************************************************************************

    @file    IntrOS: osport.h
    @author  Rajmund Szymanski
    @date    01.01.2018
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
#ifndef   NOCONFIG
#include <osconfig.h>
#endif
#include <osdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#ifndef CPU_FREQUENCY
#define CPU_FREQUENCY         F_CPU
#endif

/* -------------------------------------------------------------------------- */

#ifndef OS_FREQUENCY
#define OS_FREQUENCY       1000 /* Hz */
#endif

/* -------------------------------------------------------------------------- */

#ifndef OS_TIMER_SIZE
#define OS_TIMER_SIZE        32 /* bit size of system timer counter */
#endif

/* -------------------------------------------------------------------------- */

#ifdef  HW_TIMER_SIZE
#error  HW_TIMER_SIZE is an internal os definition!
#elif   OS_FREQUENCY > 1000 
#define HW_TIMER_SIZE        16 /* bit size of hardware timer */
#else
#define HW_TIMER_SIZE         0 /* os does not work in tick-less mode */
#endif

/* -------------------------------------------------------------------------- */
// return current system time

#if HW_TIMER_SIZE >= OS_TIMER_SIZE

__STATIC_INLINE
uint16_t port_sys_time( void )
{
	return TCA0.SINGLE.CNT;
}

#endif

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#endif//__INTROSPORT_H
