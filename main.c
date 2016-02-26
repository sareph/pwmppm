#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

// PA1 - LED
#define LED_ON() { PORTA |= _BV(1); }
#define LED_OFF() { PORTA &= ~_BV(1); }
#define LED_INIT() { DDRA |= _BV(1); }

// PD4 - PPM
#define PPM_HI() { PORTD |= _BV(4); }
#define PPM_LO() { PORTD &= ~_BV(4); }
#define PPM_INIT() { DDRD |= _BV(4); }

ISR(PCINT_vect) 
{
	PPM_HI();
	TCNT1 = 0;
	OCR1B = 38; // 304us
	
	LED_ON();
}

ISR(TIMER1_COMPB_vect) 
{
	PPM_LO();
	LED_OFF();
}

int main()
{
	GIMSK |= (1 << PCIE); // PC Int
	PCMSK = 0xFF; // whole port
	
	TCCR1A = 0;
	TCCR1B |= (1 << CS11) | (1 << CS10); // div 64
	TIMSK |= (1 << OCIE1B); // enable comp interrupt for timer 1

	PPM_INIT(); // setup as outputs
	LED_INIT();
	
	sei(); // enable all enabled interrupts
	
	for(;;)
	{
		sleep_enable();
		sleep_cpu();
		sleep_disable();
	}
	
	return 0;
}
