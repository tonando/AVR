/**
 * Copyright (c) 2018, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 *
 * This is ATtiny13 "Print Temperature" example using attiny-ds18b20-library.
 * https://github.com/lpodkalicki/attiny-ds18b20-library
 */

#include <Macros.h>
// #include <EEPROM.h>
#include "stdlib.h"
#include "stdio.h"

#define CLOCK	2
#define DATA	1
#define LATCH	0

#define	ONEWIRE_SEARCH_ROM	0xF0
#define	ONEWIRE_READ_ROM	0x33
#define	ONEWIRE_MATCH_ROM	0x55
#define	ONEWIRE_SKIP_ROM	0xCC
#define	ONEWIRE_ALARM_SEARCH	0xEC
// uint8_t PINS[8] = {0,1,2,3,4,5,6,7};
uint8_t S_DIGITS[10] = {
   0b11111100, // 0
   0b01100000, // 1
   0b11011010, // 2
   0b11110010, // 3
   0b01100110, // 4
   0b10110110, // 5
   0b10111110, // 6
   0b11100000, // 7
   0b11111110, // 8
   0b11110110  // 9
};

volatile uchar cval = 0;

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
void shiftOut(uint t);
static uint8_t _onewire_pin = 3;

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


#define	DS18B20_PIN	PB4


// char buf[16];

int main(void)
{
	// OSCCAL=100;
	
	OUTPUT(4);
	OUTPUT(3);
	OUTPUT(CLOCK);
	OUTPUT(DATA);
	OUTPUT(LATCH);
	ON(LATCH);
	// INPUT(4);
	DS18B20_init(DS18B20_PIN);
	
	TCCR0B = 4;
	TIMSK |= B(TOIE0);
	sei();
	
	// if(READ(4))
	// else while(1){
	// uchar i=50;    
		// cval=lfsr()&255;
		// _delay_ms(500);
	// }
	
	
	/* 
	while(i){
		i--;
		cval=i;
		_delay_ms(200);
	}
	 */

	uint16_t tm=0;
	// uint16_t tlast=0;

	while (1) {

		tm = DS18B20_read();
		cval = (tm/100);

		_delay_ms(200);
		// tlast=tm;
			
	}
	return 0;
}
ISR(TIMER0_OVF_vect){
	shiftOut(cval);
}

void shiftOut(uint t){
	t = constrain(t,0,99);
	// t /= 100;
	uchar vl = t % 10;
	uchar vh = (t-vl) / 10;
	
	for (uchar n=0; n<2; n++){
		uchar v = 0;

		if(n){
			// ON(3);
			ON(4);
			v=S_DIGITS[vl];
		}
		else{
			OFF(4);
			// OFF(3);
			v=S_DIGITS[vh];
			sbi(v,7);
		}
		
		uchar i=8; 
		do{i--;
 			OFF(CLOCK);

			 if(!rbi(v,i))  
			   ON(DATA);
			 else
			   OFF(DATA);
		 
			 ON(CLOCK);  
		}while(i);	
		
		OFF(LATCH);
		ON(LATCH);
		
		//if(!n)
		_delay_ms(2);
		// else
		// _delay_us(300);
	}
}