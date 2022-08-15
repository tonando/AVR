/**
 * Copyright (c) 2018, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 *
 * This is ATtiny13 "Print Temperature" example using attiny-ds18b20-library.
 * https://github.com/lpodkalicki/attiny-ds18b20-library
 */

#include <Macros.h>
#include <EEPROM.h>
#include "stdlib.h"
#include "stdio.h"
#define	UART_TX_ENABLED		(1) // Enable UART TX

#ifndef F_CPU
# define        F_CPU           (9600000UL) // 1.2 MHz
#endif  /* !F_CPU */

#if defined(UART_TX_ENABLED) && !defined(UART_TX)
# define        UART_TX         PB3 // Use PB3 as TX pin
#endif  /* !UART_TX */

#if defined(UART_RX_ENABLED) && !defined(UART_RX)
# define        UART_RX         PB4 // Use PB4 as RX pin
#endif  /* !UART_RX */
# define        UART_BAUDRATE   (57600)

#if (defined(UART_TX_ENABLED) || defined(UART_RX_ENABLED)) && !defined(UART_BAUDRATE)
# define        UART_BAUDRATE   (250000)
#endif  /* !UART_BAUDRATE */

#define	TXDELAY         	(int)(((F_CPU/UART_BAUDRATE)-7 +1.5)/3)
#define RXDELAY         	(int)(((F_CPU/UART_BAUDRATE)-5 +1.5)/3)
#define RXDELAY2        	(int)((RXDELAY*1.5)-2.5)
#define RXROUNDED       	(((F_CPU/UART_BAUDRATE)-5 +2)/3)
#if RXROUNDED > 127
# error Low baud rates are not supported - use higher, UART_BAUDRATE
#endif

char uart_getc(void);
void uart_putc(char c);
void uart_putu(uint16_t x);
void uart_puts(const char *s);
void uart_putt(uint16_t t);

char
uart_getc(void)
{
#ifdef	UART_RX_ENABLED
	char c;
	uint8_t sreg;

	sreg = SREG;
	cli();
	PORTB &= ~(1 << UART_RX);
	DDRB &= ~(1 << UART_RX);
	__asm volatile(
		" ldi r18, %[rxdelay2] \n\t" // 1.5 bit delay
		" ldi %0, 0x80 \n\t" // bit shift counter
		"WaitStart: \n\t"
		" sbic %[uart_port]-2, %[uart_pin] \n\t" // wait for start edge
		" rjmp WaitStart \n\t"
		"RxBit: \n\t"
		// 6 cycle loop + delay - total = 5 + 3*r22
		// delay (3 cycle * r18) -1 and clear carry with subi
		" subi r18, 1 \n\t"
		" brne RxBit \n\t"
		" ldi r18, %[rxdelay] \n\t"
		" sbic %[uart_port]-2, %[uart_pin] \n\t" // check UART PIN
		" sec \n\t"
		" ror %0 \n\t"
		" brcc RxBit \n\t"
		"StopBit: \n\t"
		" dec r18 \n\t"
		" brne StopBit \n\t"
		: "=r" (c)
		: [uart_port] "I" (_SFR_IO_ADDR(PORTB)),
		[uart_pin] "I" (UART_RX),
		[rxdelay] "I" (RXDELAY),
		[rxdelay2] "I" (RXDELAY2)
		: "r0","r18","r19"
	);
	SREG = sreg;
	return c;
#else
	return (-1);
#endif /* !UART_RX_ENABLED */
}

void
uart_putc(char c)
{
#ifdef	UART_TX_ENABLED
	uint8_t sreg;

	sreg = SREG;
	cli();
	PORTB |= 1 << UART_TX;
	DDRB |= 1 << UART_TX;
	__asm volatile(
		" cbi %[uart_port], %[uart_pin] \n\t" // start bit
		" in r0, %[uart_port] \n\t"
		" ldi r30, 3 \n\t" // stop bit + idle state
		" ldi r28, %[txdelay] \n\t"
		"TxLoop: \n\t"
		// 8 cycle loop + delay - total = 7 + 3*r22
		" mov r29, r28 \n\t"
		"TxDelay: \n\t"
		// delay (3 cycle * delayCount) - 1
		" dec r29 \n\t"
		" brne TxDelay \n\t"
		" bst %[ch], 0 \n\t"
		" bld r0, %[uart_pin] \n\t"
		" lsr r30 \n\t"
		" ror %[ch] \n\t"
		" out %[uart_port], r0 \n\t"
		" brne TxLoop \n\t"
		:
		: [uart_port] "I" (_SFR_IO_ADDR(PORTB)),
		[uart_pin] "I" (UART_TX),
		[txdelay] "I" (TXDELAY),
		[ch] "r" (c)
		: "r0","r28","r29","r30"
	);
	SREG = sreg;
#endif /* !UART_TX_ENABLED */
}

void uart_putu(uint16_t x)
{
        char buff[8] = {0};
        char *p = buff+6;
        do { *(p--) = (x % 10) + '0'; x /= 10; } while(x);
        uart_puts((const char *)(p+1));
}

void
uart_puts(const char *s)
{
     	while (*s) uart_putc(*(s++));
}

void uart_putt(uint16_t t)
{
	uint8_t n = t / 100;
	uart_putu(n);
	uart_putc('.');
	uart_putu(t - n);
	uart_puts(" st. C\n");
}

#define	ONEWIRE_SEARCH_ROM	0xF0
#define	ONEWIRE_READ_ROM	0x33
#define	ONEWIRE_MATCH_ROM	0x55
#define	ONEWIRE_SKIP_ROM	0xCC
#define	ONEWIRE_ALARM_SEARCH	0xEC

void onewire_init(uint8_t pin);
uint8_t onewire_reset(void);
uint8_t onewire_write(uint8_t value);
uint8_t onewire_read(void);
#define	DS18B20_CONVERT_T	0x44
#define	DS18B20_READ		0xBE

#define ONEWIRE_PIN_INPUT()		(DDRB &= ~_onewire_pin)
#define ONEWIRE_PIN_OUTPUT()		(DDRB |= _onewire_pin)
#define ONEWIRE_PIN_LOW()		(PORTB &= ~_onewire_pin)
#define ONEWIRE_PIN_HIGH()		(PORTB |= _onewire_pin)
#define ONEWIRE_PIN_READ()		(PINB & _onewire_pin)
#define ONEWIRE_RESET_RETRIES_MAX	(128)


static uint8_t _onewire_pin = 0;

void onewire_init(uint8_t pin)
{
	_onewire_pin = _BV(pin);
}

uint8_t onewire_reset(void)
{
	uint8_t retval, retries;

	ONEWIRE_PIN_LOW();
	ONEWIRE_PIN_INPUT();

	retries = ONEWIRE_RESET_RETRIES_MAX;
	while (!ONEWIRE_PIN_READ()) {
		if (--retries == 0) {
			return (2);
		}
		_delay_us(1);
	}

	ONEWIRE_PIN_OUTPUT();
	_delay_us(480);
	ONEWIRE_PIN_INPUT();
	_delay_us(66);
	retval = ONEWIRE_PIN_READ();
	_delay_us(414);

	return (retval);
}

static uint8_t onewire_bit(uint8_t value)
{
	uint8_t sreg;

	sreg = SREG;
	cli();
  	ONEWIRE_PIN_OUTPUT();
  	_delay_us(1);
  	if (value) {
    		ONEWIRE_PIN_INPUT();
	}
	_delay_us(14);
	value = !(ONEWIRE_PIN_READ() == 0);
  	_delay_us(45);
  	ONEWIRE_PIN_INPUT();
  	SREG = sreg;

	return value;
}

uint8_t onewire_write(uint8_t value)
{
	uint8_t i, r;

	for (i = 0; i < 8; ++i) {
    		r = onewire_bit(value & 0x01);
		value >>= 1;
    		if (r) {
			value |= 0x80;
		}
	}

  	return value;
}

uint8_t onewire_read(void)
{

	return onewire_write(0xff);
}



void DS18B20_init(uint8_t pin)
{

	onewire_init(pin);
}

uint16_t DS18B20_read(void)
{
	uint16_t t;

	onewire_reset();
	onewire_write(ONEWIRE_SKIP_ROM);
	onewire_write(DS18B20_CONVERT_T);

	onewire_reset();
	onewire_write(ONEWIRE_SKIP_ROM);
	onewire_write(DS18B20_READ);

      	t = onewire_read();
      	t |= (uint16_t)onewire_read() << 8;

	return ((t >> 4) * 100 + ((t << 12) / 6553) * 10);
}


#define	DS18B20_PIN	PB0


char buf[64];

int main(void)
{
	// OSCCAL=100;
	
	OUTPUT(1);
	OUTPUT(2);
	BUTTON(4);
	
	bool serial = true;	
	bool autoread = true;//READ(1);
	bool minmax= false; //READ(2);
	
	uint del= autoread ? 100 : 20;
	
	uint saved=(uint)EEPROM_read(0);
	uint16_t t=0;
	uint16_t tlast=0;
	
	uint16_t tmin=0xffff;
	uint16_t tmax=0;

	DS18B20_init(DS18B20_PIN);
	
	while (1) {
		// if(!READ(1)){
			// spd++;
			// spd&=7;
		
			// del = 1000 * ((1+spd)<<1);
			// _delay_ms(500);
		// }
		if( !autoread){
			while(READ(1));
			delay_ms(1);
			while(!READ(1));
		}
		
		t = DS18B20_read();
		
		if(!READ(4)) {
		ON(1);ON(2);
			saved = t;
			EEPROM_write(0,t);
			delay_ms(500);
			}
		
		if(t != tlast && tlast){
			OFF(1);
			OFF(2);
			// OUTPUT(2);
			if(t > tlast+19) ON(2);
			if(t < tlast-19) ON(1);
			
			// delay_ms(5+MIN(50,(ABS(tlast-t)>>4)));
			delay_ms(60);
		}
		else{
			OFF(2);
			OFF(1);
		}
		
		if(minmax){
			tmin = MIN(tmin,t);
			tmax = MAX(tmax,t);

			if(serial){
			uart_putt(tmin);
			uart_putc('\t');
			uart_putt(tmax);
			uart_putc('\t');
			}
		}
		
		if(serial){
			// uart_putc('T');
			// uart_putc('=');
			uart_putt(t);
			uart_putc('\t');
			if(saved)
				uart_putt(saved);
			uart_putc('\r');
			uart_putc('\n');
		}	
		
		// if(t){
		// ON(1); 
		// delay_us(t);
		// OFF(1);
		// delay_us(1000-t);
		// }
		
		
		
		tlast=t;
		delay_ms(del);
		
		// if(OSCCAL) 
			// OSCCAL++;
		// else
			// OSCCAL=200;
		// OSCCAL&=250;	
		// delay_ms(100);
			
	}
	return 0;
}
