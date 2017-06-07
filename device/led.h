#ifndef __LED_H__
#define __LED_H__

// btn: PORTC.0

#include <avr/io.h>
#include <stdint.h>

struct  __LEDs { uint8_t: PIN0_bp; volatile uint8_t f: 1; uint8_t: 0; };

#define LED   (((struct __LEDs *)&(PORTC.OUT))->f)

static inline void led_init  ( void ) { PORTC.DIRSET = PIN0_bm; }
static inline void led_set   ( void ) { PORTC.OUTSET = PIN0_bm; }
static inline void led_clear ( void ) { PORTC.OUTCLR = PIN0_bm; }
static inline void led_toggle( void ) { PORTC.OUTTGL = PIN0_bm; }

#endif//__LED_H__
