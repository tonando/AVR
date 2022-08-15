#include <Macros.h>
uchar DATA_IN;
uchar DATA_OUT;


void init_usi (uchar _in,uchar _out) {  
  
  DATA_IN = _in;
  DATA_OUT = _out;
  sbi(DDRB,_out);
  cbi(DDRB,_in);
  
  USICR = 0;                      // Disable USI.
  GIFR = 1<<PCIF;                 // Clear pin change interrupt flag.
  GIMSK |= 1<<PCIE;               // Enable pin change interrupts
  PCMSK |= B(DATA_IN);             // Enable pin change on pin 0
}

ISR (PCINT0_vect) {
  if (!(PINB & 1<<PINB0)) {       // Ignore if DI is high
    GIMSK &= ~(1<<PCIE);          // Disable pin change interrupts
    TCCR0A = 2<<WGM00;            // CTC mode
    TCCR0B = 0<<WGM02 | 2<<CS00;  // Set prescaler to /8
    TCNT0 = 0;                    // Count up from 0
    OCR0A = 51;                   // Delay (51+1)*8 cycles
    TIFR |= 1<<OCF0A;             // Clear output compare flag
    TIMSK |= 1<<OCIE0A;           // Enable output compare interrupt
  }
}

ISR (TIMER0_COMPA_vect) {
  TIMSK &= ~(1<<OCIE0A);          // Disable COMPA interrupt
  TCNT0 = 0;                      // Count up from 0
  OCR0A = 103;                    // Shift every (103+1)*8 cycles
  // Enable USI OVF interrupt, and select Timer0 compare match as USI Clock source:
  USICR = 1<<USIOIE | 0<<USIWM0 | 1<<USICS0;
  USISR = 1<<USIOIF | 8;          // Clear USI OVF flag, and set counter
}

ISR (USI_OVF_vect) {
  USICR = 0;                      // Disable USI         
  int temp = USIDR;
  Display(ReverseByte(temp));
  GIFR = 1<<PCIF;                 // Clear pin change interrupt flag.
  GIMSK |= 1<<PCIE;               // Enable pin change interrupts again
}