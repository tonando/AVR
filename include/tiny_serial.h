
#include <Macros.h>
#include "stdlib.h"
#include "stdio.h"
// #define USE_PRINTF 

#ifndef TX_PIN
#define TX_PIN 4
#endif

#define BAUD_4800 		207	
#define BAUD_9600 		103	
#define BAUD_19200 		51	
#define BAUD_31250 		31	
#define BAUD_57600		17	
#define BAUD_115200		8	

#ifndef OCR_VAL
#define OCR_VAL BAUD_9600
#endif
/* prototypes */
static void tiny_serial_init();
static void tiny_serial_init(uchar);
static void serial_write(uint8_t tx_byte);
static uint64_t millis(void);
static void serial_write(uint8_t tx_byte);
static void serial_print(const char *str);
static void serial_print_bits(uchar v);

/* some vars */
static volatile uint64_t _millis    = 0;
static volatile uint16_t _1000us    = 0;
uint64_t old_millis = 0;

static volatile uint8_t tx_buzy = 0;
static volatile uint8_t bit_index;
static volatile uint8_t _tx_buffer; 

static volatile uint besetzt = 0;

/*** ISR ***/

// compare match interrupt service for OCR0A
ISR(TIM0_COMPA_vect) { 

  // send data
  if (tx_buzy) {
									sbi(DDRB,  TX_PIN); // TX_PIN as output

    if (bit_index == 0) {
      // start bit (= 0)
      cbi(PORTB, TX_PIN);
    } else if (bit_index <=8) {
      // LSB to MSB
      if (_tx_buffer & 1) {
        sbi(PORTB, TX_PIN);
      } else {
        cbi(PORTB, TX_PIN);
      }
      _tx_buffer >>= 1;        
    } else if (bit_index >= 9) {
      // stop bit (= 1)
      sbi(PORTB, TX_PIN);
									cbi(DDRB,  TX_PIN); // TX_PIN as output
			
      tx_buzy = 0;
    }
    bit_index++;
  }
									  else{
										if(READ(TX_PIN) == 0)
											besetzt = 100;
										
										if(besetzt) besetzt--;	
									  }
  
  
  // millis update
  _1000us += OCR_VAL;
  while (_1000us > 1000) {
    _millis++;
    _1000us -= 1000;
  }
  
}

/*** UART routines ***/
// send serial data to software UART, block until UART buzy
static void serial_write(uint8_t tx_byte) {
									while(besetzt);
  while(tx_buzy);
  bit_index  = 0;
  _tx_buffer = tx_byte;
  tx_buzy = 1;
}

static void serial_print(const char *str) {
  uint8_t i;
  for (i = 0; str[i] != 0; i++) {
    serial_write(str[i]);
  }
}


static void serial_print_int(long v){

		char buf[32];
		sprintf(buf,"%ld",(v));
		const char* p = buf;
		serial_print(p);	

}
static void serial_print_bits(uchar v){

	for(uchar i=0; i<8; i++){
		if(rbi(v,i))
			serial_print("1");
		else
			serial_print("0");
	}	
}
// #endif

/*** misc routines ***/

// safe access to millis counter
static uint64_t millis() {
  uint64_t m;
  cli();
  m = _millis;
  sei();
  return m;
}

/*** main routines ***/

// void tiny_serial_init(uchar _tx) {
static void tiny_serial_init(uchar ocr_val) {
  // Software UART IO
										cbi(DDRB,  TX_PIN); // TX_PIN as output
  // sbi(DDRB,  TX_PIN); // TX_PIN as output
  sbi(PORTB, TX_PIN); // serial idle level is '1'
  /* interrup setup */
  // call ISR(TIM0_COMPA_vect) every 103us (for 9600 bauds)
  // set CTC mode : clear timer on comapre match
  // -> reset TCNTO (timer 0) when TCNTO == OCR0A
  sbi(TCCR0A, WGM01);
  // prescaler : clk/8 (1 tic = 1us for 8MHz clock)
  sbi(TCCR0B, CS01);
  // compare register A at 103 us
  OCR0A = ocr_val;
  // interrupt on compare match OCROA == TCNT0
  sbi(TIMSK, OCIE0A);
  // Enable global interrupts
  sei();
}

static void tiny_serial_init() { tiny_serial_init(OCR_VAL); }
