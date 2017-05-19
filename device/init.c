#include <avr/io.h>
#include <init.h>

__INIT(3)
void CLKCTRL_init( void )
{
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = 0;
}
