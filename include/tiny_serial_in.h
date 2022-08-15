/* 
  sample UART software
  transmit serial data at 9600,N,8,1
  code for avr-gcc
           ATTiny85 at 8 MHz
  code in public domain
*/

// #define F_CPU 8000000UL
// #define USE_PRINTF 
#include <Macros.h>

#define RX_PIN 2	// INT0
// #define BAUD_57600 17	
#define BAUD_MIDI 31	
#define BAUD_4800 207	
#define BAUD_9600 102	
#define BAUD_19200 51	
#define BAUD_38400 207 // x1 pre	

#define OCR_VAL BAUD_9600

/* prototypes */
uchar serial_getc();
uint64_t millis(void);

/* some vars */
volatile bool rx_available    = 0;
volatile uint64_t _millis    = 0;
volatile uint16_t _1000us    = 0;
uint64_t old_millis = 0;

// must be volatile (change and test in main and ISR)
// volatile uint8_t rx_buzy = 0;
volatile uint8_t bit_index;
volatile uint8_t _rx_buffer; 

void timer_start(){
	cbi(GIMSK,INT0);
	TCNT0=bit_index=_rx_buffer=_millis=_1000us=0;
	// rx_busy=true;
	sbi(TCCR0B, CS01);
	sbi(TIMSK, OCIE0A);
}
void timer_stop(){
	rx_available=true;
	sbi(GIMSK,INT0);
}
/*** ISR ***/

ISR(INT0_vect) { 
	timer_start();
}

// compare match interrupt service for OCR0A
// call every 103us
ISR(TIM0_COMPA_vect) { 
	
  // software UART
  // start(0),data,stop(1)
  
  // if (rx_buzy) {
    if (bit_index){
      
	// LSB to MSB
      if (rbi(PINB, RX_PIN)) 
        sbi(_rx_buffer,7);

      _rx_buffer >>= 1;        
    } 
    bit_index++;
	
	if(bit_index >= 10)
		timer_stop();
  // }
  // millis update
  _1000us += OCR_VAL;
  while (_1000us > 1000) {
    _millis++;
    _1000us -= 1000;
  }
}

/*** UART routines ***/
// send serial data to software UART, block until UART buzy
uchar serial_getc() {
	uchar r = _rx_buffer;
	_rx_buffer = 0;
	rx_available=false;
	
	return r;
}

/*** misc routines ***/

// safe access to millis counter
uint64_t millis() {
  uint64_t m;
  cli();
  m = _millis;
  sei();
  return m;
}

/*** main routines ***/

// void tiny_serial_init(uchar _tx) {
void tiny_serial_init() {
  // Software UART IO
  BUTTON(RX_PIN); // TX_PIN as output
  /* interrup setup */
  // call ISR(TIM0_COMPA_vect) every 103us (for 9600 bauds)
  // set CTC mode : clear timer on comapre match
  // -> reset TCNTO (timer 0) when TCNTO == OCR0A
  sbi(TCCR0A, WGM01);
  // prescaler : clk/8 (1 tic = 1us for 8MHz clock)
  TCCR0B=0;
  // compare register A at 103 us
  OCR0A = OCR_VAL;
  // interrupt on compare match OCROA == TCNT0
  
  sbi(MCUCR,1);
  sbi(GIMSK,INT0);
  // Enable global interrupts
  sei();
}
