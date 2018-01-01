/******************************************************************************

    @file    IntrOS: osport.c
    @author  Rajmund Szymanski
    @date    01.01.2018
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
#if HW_TIMER_SIZE == 0

/******************************************************************************
 Non-tick-less mode: configuration of system timer
 It must generate interrupts with frequency OS_FREQUENCY
*******************************************************************************/

	TCA0.SINGLE.PER     = (CPU_FREQUENCY)/(OS_FREQUENCY)/16-1;
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA0.SINGLE.CTRLB   = TCA_SINGLE_WGMODE_NORMAL_gc;
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV16_gc;

/******************************************************************************
 End of configuration
*******************************************************************************/

#else //HW_TIMER_SIZE

/******************************************************************************
 Tick-less mode: configuration of system timer
 It must be rescaled to frequency OS_FREQUENCY
*******************************************************************************/

	TCA0.SINGLE.PER     = UINT16_MAX;
	#if HW_TIMER_SIZE < OS_TIMER_SIZE
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	#endif
	TCA0.SINGLE.CTRLB   = TCA_SINGLE_WGMODE_NORMAL_gc;
	#if   CPU_FREQUENCY == (OS_FREQUENCY) * 1
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV1_gc;
	#elif CPU_FREQUENCY == (OS_FREQUENCY) * 2
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV2_gc;
	#elif CPU_FREQUENCY == (OS_FREQUENCY) * 4
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV4_gc;
	#elif CPU_FREQUENCY == (OS_FREQUENCY) * 8
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV8_gc;
	#elif CPU_FREQUENCY == (OS_FREQUENCY) * 16
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV16_gc;
	#elif CPU_FREQUENCY == (OS_FREQUENCY) * 64
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV64_gc;
	#elif CPU_FREQUENCY == (OS_FREQUENCY) * 256
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV256_gc;
	#elif CPU_FREQUENCY == (OS_FREQUENCY) * 1024
	TCA0.SINGLE.CTRLA   = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV1024_gc;
	#else
	#error osconfig.h: Invalid OS_FREQUENCY value!
	#endif

/******************************************************************************
 End of configuration
*******************************************************************************/

#endif//HW_TIMER_SIZE

	port_clr_lock();
}

/* -------------------------------------------------------------------------- */

#if HW_TIMER_SIZE == 0

/******************************************************************************
 Non-tick-less mode: interrupt handler of system timer
*******************************************************************************/

ISR( TCA0_OVF_vect )
{
//	if (TCA0.SINGLE.INTFLAGS & TCA_SINGLE_OVF_bm)
	{
		TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
		core_sys_tick();
	}
}

/******************************************************************************
 End of the handler
*******************************************************************************/

#else //HW_TIMER_SIZE

/******************************************************************************
 Tick-less mode: interrupt handler of system timer
*******************************************************************************/

#if HW_TIMER_SIZE < OS_TIMER_SIZE

ISR( TCA0_OVF_vect )
{
//	if (TCA0.SINGLE.INTFLAGS & TCA_SINGLE_OVF_bm)
	{
		TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
		core_sys_tick();
	}
}

#endif

/******************************************************************************
 End of the handler
*******************************************************************************/

/******************************************************************************
 Tick-less mode: return current system time
*******************************************************************************/

#if HW_TIMER_SIZE < OS_TIMER_SIZE

cnt_t port_sys_time( void )
{
	cnt_t    cnt;
	uint16_t tck;

	cnt = System.cnt;
	tck = TCA0.SINGLE.CNT;

	if (TCA0.SINGLE.INTFLAGS & TCA_SINGLE_OVF_bm)
	{
		tck = TCA0.SINGLE.CNT;
		cnt += (cnt_t)(1) << (HW_TIMER_SIZE);
	}

	return cnt + tck;
}

#endif

/******************************************************************************
 End of the function
*******************************************************************************/

#endif//HW_TIMER_SIZE

/* -------------------------------------------------------------------------- */
