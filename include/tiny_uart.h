#ifndef TINY_UART_h
#define TINY_UART_h

#include <Macros.h>


#define SOFT_TX_PIN	(1<<PB1)
#define SOFT_TX_PORT PORTB
#define SOFT_TX_DDR DDRB

#define SOFT_RX_PIN	(1<<PB0)
#define SOFT_RX_DDR DDRB
#define SOFT_RX_PORT PINB

//global variables
volatile unsigned char uart_rx;
volatile unsigned char uart_rx_flag;
volatile unsigned char c_r_p, c_w_p;	//circular buffer read-write index
volatile uint8_t ser_available;
uint8_t c_buff[16];

//bitbanged UART transmit byte
void
uart_send_byte (unsigned char data)
{
  unsigned char i;
  TCCR0B = 0;
  TCNT0 = 0;
  TIFR0 |= 1 << OCF0A;
  TCCR0B |= (1 << CS00);
  TIFR0 |= 1 << OCF0A;
  SOFT_TX_PORT &= ~SOFT_TX_PIN;
  while (!(TIFR0 & (1 << OCF0A)));
  TIFR0 |= 1 << OCF0A;
  for (i = 0; i < 8; i++)
    {
      if (data & 1)
	SOFT_TX_PORT |= SOFT_TX_PIN;
      else
	SOFT_TX_PORT &= ~SOFT_TX_PIN;
      data >>= 1;
      while (!(TIFR0 & (1 << OCF0A)));
      TIFR0 |= 1 << OCF0A;
    }
  SOFT_TX_PORT |= SOFT_TX_PIN;
  while (!(TIFR0 & (1 << OCF0A)));
  TIFR0 |= 1 << OCF0A;
}

void
uart_print (char *p)
{
  while (*p)
    uart_send_byte (*p++);
}
// uart receive data
ISR (PCINT0_vect)
{
  int cn;
  uart_rx = 0;
  _delay_us (2);
  TCCR0B = 0;
  TCNT0 = 0;
  TIFR0 |= 1 << OCF0A;
  TCCR0B |= (1 << CS00);
  for (cn = 0; cn < 8; cn++)
    {
      while (!(TIFR0 & (1 << OCF0A)));
      TIFR0 |= 1 << OCF0A;
      uart_rx >>= 1;
      if (PINB & (1 << PB0))
	uart_rx |= (1 << 7);
    }

  ser_available = 1;
  c_buff[c_w_p] = uart_rx;
  c_w_p++;
  c_w_p &= 15;

  while (!(TIFR0 & (1 << OCF0A)));
  TIFR0 |= 1 << OCF0A;
  GIFR |= 1 << PCIF;
}

unsigned char
serial_read ()
{
  unsigned char rett = 0;
  ser_available = 0;
  if (c_r_p != c_w_p)
    {
      rett = c_buff[c_r_p];
      c_r_p++;
      c_r_p &= 15;
    }

  if (c_r_p != c_w_p)
    ser_available = 1;

  return rett;
}


void
uart_rx_int_init ()
{
  SOFT_RX_DDR &= ~(SOFT_RX_PIN);
  SOFT_RX_PORT |= (SOFT_RX_PIN);
  GIMSK |= 1 << PCIE;
  PCMSK |= 1 << PCINT0;
}

void
uart_tx_init ()
{
  TCCR0A = 1 << WGM01;		// compare mode
  TCCR0B = 1 << CS00;		// prescaler 1
  ADMUX |= 1 << REFS0;
  SOFT_TX_PORT |= SOFT_TX_PIN;
  SOFT_TX_DDR |= SOFT_TX_PIN;
  OCR0A = 75;			//115200 baudrate at prescaler 1
}
#endif