/******************************************************************************

    @file    IntrOS: osport.c
    @author  Rajmund Szymanski
    @date    25.05.2017
    @brief   IntrOS port file for ATtiny817 uC.

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

#include <oskernel.h>

/* -------------------------------------------------------------------------- */

void port_sys_init( void )
{
/******************************************************************************
 Put here configuration of system timer
*******************************************************************************/

	TCA0.SINGLE.PER     = CPU_FREQUENCY / OS_FREQUENCY / 16 - 1;
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA0.SINGLE.CTRLB   = TCA_SINGLE_WGMODE_NORMAL_gc;
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_CLKSEL_DIV16_gc
	                    | TCA_SINGLE_ENABLE_bm;
	sei();

/******************************************************************************
 End of configuration
*******************************************************************************/
}

/* -------------------------------------------------------------------------- */

#if OS_TIMER == 0

/******************************************************************************
 Put here the procedure of interrupt handler of system timer
*******************************************************************************/

ISR( TCA0_OVF_vect )
{
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
	System.cnt++;
}

/******************************************************************************
 End of the procedure of interrupt handler
*******************************************************************************/

#endif//OS_TIMER

/* -------------------------------------------------------------------------- */
