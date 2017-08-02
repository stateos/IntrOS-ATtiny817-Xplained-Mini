/******************************************************************************

    @file    IntrOS: osport.h
    @author  Rajmund Szymanski
    @date    01.08.2017
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
#include <osdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#ifndef  OS_TICKLESS
#define  OS_TICKLESS          0 /* os not works in tick-less mode             */
#endif

#if      OS_TICKLESS
#error   osconfig.h: Incorrect OS_TICKLESS value! This port does not support tick-less mode.
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

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#endif//__INTROSPORT_H
