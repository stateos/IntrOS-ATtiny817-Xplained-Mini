/******************************************************************************

    @file    IntrOS: osport.c
    @author  Rajmund Szymanski
    @date    20.12.2017
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
#if OS_TICKLESS == 0

/******************************************************************************
 Non-tick-less mode: configuration of system timer
 It must generate interrupts with frequency OS_FREQUENCY
*******************************************************************************/

	TCA0.SINGLE.PER     = (CPU_FREQUENCY)/(OS_FREQUENCY)/16-1;
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA0.SINGLE.CTRLB   = TCA_SINGLE_WGMODE_NORMAL_gc;
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_CLKSEL_DIV16_gc
	                    | TCA_SINGLE_ENABLE_bm;

/******************************************************************************
 End of configuration
*******************************************************************************/

#else //OS_TICKLESS

/******************************************************************************
 Tick-less mode: configuration of system timer
 It must be rescaled to frequency OS_FREQUENCY
*******************************************************************************/

	TCA0.SINGLE.PER     = UINT16_MAX;
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA0.SINGLE.CTRLB   = TCA_SINGLE_WGMODE_NORMAL_gc;
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_CLKSEL_DIV16_gc
	                    | TCA_SINGLE_ENABLE_bm;

/******************************************************************************
 End of configuration
*******************************************************************************/

#endif//OS_TICKLESS

	port_clr_lock();
}

/* -------------------------------------------------------------------------- */

#if OS_TICKLESS == 0

/******************************************************************************
 Non-tick-less mode: interrupt handler of system timer
*******************************************************************************/

ISR( TCA0_OVF_vect )
{
	TCA0.SINGLE.INTFLAGS = 0xFF;
	core_sys_tick();
}

/******************************************************************************
 End of the handler
*******************************************************************************/

#else //OS_TICKLESS

/******************************************************************************
 Tick-less mode: interrupt handler of system timer
*******************************************************************************/

ISR( TCA0_OVF_vect )
{
	TCA0.SINGLE.INTFLAGS = 0xFF;
	System.cnt += 1UL<<16; // TCA0.SINGLE.CNT is 16-bit
}

/******************************************************************************
 End of the handler
*******************************************************************************/

/******************************************************************************
 Tick-less mode: return current system time
*******************************************************************************/

uint32_t port_sys_time( void )
{
	uint32_t cnt;
	uint16_t tck;

	cnt = System.cnt;
	tck = TCA0.SINGLE.CNT;

	if (TCA0.SINGLE.INTFLAGS)
	{
		cnt += 1UL << 16; // TCA0.SINGLE.CNT is 16-bit
		tck = TCA0.SINGLE.CNT;
	}

	return cnt + tck;
}

/******************************************************************************
 End of the function
*******************************************************************************/

#endif//OS_TICKLESS

/* -------------------------------------------------------------------------- */
