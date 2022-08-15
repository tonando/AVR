
#include <Macros.h>
// #include <EEPROM.h>

// #define OCR_VAL BAUD_115200
// #include <tiny_serial.h>

#define MULT	32.25

#define BUZ 	PB2
#define VFC		PB1
#define ABAT 	3
#define BTN 	PB0
uchar LED = 4;

// #define EE_RUNS 0x00
#define BAT_WARN  7900
#define BAT_EMPTY 7600			
// #define TIME_WARN 300

uint adc(uchar);
// bool use_eeprom = false;
// bool fc_connected = false;
bool brownout = false;
// volatile bool paused = false;
// uchar cells=0;

// volatile uchar ovf=0;
// volatile uint c_secs=0;
// uint c_rom=0;		
// uchar c_point=0;
// uchar points[16][2];

void setup_comparator();
// void sendTelemetry();
uint check_battery();
void beep(uchar n,uchar m);
// void beep(){beep(1,0); }
	
uint vbat=0;
// uint ain=0;
// uint lain=0;


// bool debug=false; 

int main (void)
{ 

	OUTPUT(BUZ);
	INPUT(VFC);
	INPUT(ABAT);
	BUTTON(BTN);
	OUTPUT(LED);
	
	// TCCR1 = 9;
	// TCNT1=0;
	// TIMSK |= B(TOIE1);
	
	uchar cnt=0;
	
	// while(!cells){
		// _delay_ms(20);
		// vbat = check_battery();
	
		// if(vbat > 3000 && vbat < 4400) cells = 1;
		// if(vbat > 7100 && vbat < 8600) cells = 2;
		// if(vbat > 11000 && vbat < 12800) cells = 3;
		
		// if(vbat < 300)
			// break;		
	// }
	
	// ON(LED);
	// beep(3,9);
	// OFF(LED);
	// _delay_ms(100);
	
	// setup_comparator();
	// PCMSK |= B(BTN);
	// GIMSK |= B(PCIE);
	
	sei();

	while(1){

			// _delay_ms(10);
/* 	
			if(paused){
				while(paused){
					_delay_ms(100);
					PORTB ^= B(LED);
				}
			}
 */			
			
			
			// if(c_secs > TIME_WARN && !(cnt)){
				// beep(3,5);
				// beep(1,3);
				// beep(2,4);
			// }
		if(!(cnt&15)){	
			vbat = check_battery();
			
			if(vbat){
			
				if(vbat < (BAT_WARN) ){
					if(vbat < (BAT_EMPTY)){	
						beep(2,3);
						// beep(2,1);
					}
					// else if(!(cnt&127)){
						beep(2,4);
					// }
				}
			}
			// else{
				// if(vbat) cells=2;
			// }
			
			setup_comparator();
		}
		
		cnt++;
		// cnt&=255;
		
		// if(fc_connected)	
		// if(!(cnt)) OFF(LED);
			// fc_connected=true;
	
	
	}
	
	return 0;
}
void setup_comparator(){
	// cli();
 	ADMUX=(1<<ACME);//enable multiplexer
	ADCSRA&=~(1<<ADEN);//make sure ADC is OFF
	// ADCSRA|=B(ACME);
	// ADMUX&=~B(ADEN); //select ADC3 as negative AIN
	ADMUX|=(1<<ACME)|(1<<ADEN); //select ADC3 as negative AIN
	// ADMUX|=(1<<ACME)|(1<<MUX1)|(1<<MUX0); //select ADC3 as negative AIN
	ACSR|=
	(0<<ACD)|	//Comparator ON
	(1<<ACBG)|	//Connect 1.23V reference to AIN0
	(1<<ACIE)|	//Comparator Interrupt enable
	// (0<<ACIC)|	//input capture disabled
	(0<<ACIS1); //set interrupt on output toggle
	// (0<<ACIS0);
	sei();//enable global interrupts
}

uint check_battery(){
	// cli();
	float f=adc(ABAT);
	
	// lain = ain;
	// ain = adc(ABAT);
	
	f *= MULT;
	uint r = (uint)f;
	// sei();
	return r;
}
/*
ISR(PCINT0_vect){
	
	if(READ(BTN)) return;
	cli();
	
	
	// PORTB ^= B(LED);
	 
			uchar hold=0;
			
			while(!READ(BTN)){
				if(hold<20)
					hold++;
				// else
					// beep(1,4);
					
				_delay_ms(100);
			}
			// if(hold < 6){
				paused=!paused;
				// ON(LED);
				// _delay_ms(500);
				// save_slot();
				// OFF(LED);
				// if(debug) serial_print("saved\r\n");
				// }
			// else {
				// if(hold > 7) {
					// if(hold > 15){
						// erase_data();
						// if(debug) serial_print("erased\r\n");
						// }
					// else{
						// if(c_rom < 10) c_rom++;
							// if(debug) serial_print("slot changed\r\n");
						// }
					// }
			// }
			
	// PORTB ^= B(LED);
	sei();
	
	// if(debug)	serial_print("BTN \n\r");
}

		 */	

ISR(ANA_COMP_vect){
	if(brownout) return;
	cli();
	
	// points[c_point][0] = c_secs;
	// points[c_point][1] = 1;
	// c_point++;
	
	// ON(LED);
	// beep(7,0);
	// OFF(LED);
	
	brownout = true;
	sei();
}
/* 
ISR(TIMER1_OVF_vect){

	ovf++;
	
	if(ovf==80){
		ON(LED);
		ovf=0;
		if(c_secs < 10000)
			c_secs++;
	}
}
 */
uint adc(uchar c){
	// cl1111111111i();
	ADCSRA=0;//make sure ADC is OFF
	ADMUX = /* B(REFS0) | */ (c);       // use ADC2 for input (PB4), MUX bit 0
	ADCSRA = (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1);

	//Start conversion
	ADCSRA |= (1 << ADSC);         // start ADC measurement
	while (ADCSRA & (1 << ADSC) ); // wait till conversion complete 

	// sei();
	return (ADC);
	
	// setup_comparator();
}
void beep(uchar n,uchar m){
	// m = MIN(10,m);

	// uint e = 400 - (m<<3);
	uint d = 120 + (m<<4);
	
	for(uchar i=255-(m<<3); i;i--){
		ON(BUZ);
		delay_us(d);
		OFF(BUZ);
		delay_us(d);
		
		if(i<10)// && d<300) {
			d++;
			// d=MIN(d,450);
		// }
	}
	
	// OFF(LED);	
	
	_delay_ms(10);
	
	if(n>1)
		beep(n-1,m);
	// else
		// _delay_ms(10);
}

/*

uint8_t update_crc8(uint8_t crc, uint8_t crc_seed){
	uint8_t crc_u, i;
	crc_u = crc;
	crc_u ^= crc_seed;
	for ( i=0; i<8; i++) crc_u = ( crc_u & 0x80 ) ? 0x7 ^ ( crc_u << 1 ) : ( crc_u << 1 );
	return (crc_u);
}

uint8_t get_crc8(uint8_t *Buf, uint8_t BufLen){
uint8_t crc = 0, i;
for( i=0; i<BufLen; i++) crc = update_crc8(Buf[i], crc);
return (crc);
}

void send_telemetry(){
	
}
One transmission will have 10 8-bit bytes sent with 115200 baud and 3.6V.
Byte 0: Temperature 
Byte 1: Voltage high byte
Byte 2: Voltage low byte
Byte 3: Current high byte
Byte 4: Current low byte
Byte 5: Consumption high byte
Byte 6: Consumption low byte
Byte 7: Rpm high byte
Byte 8: Rpm low byte
Byte 9: 8-bit CRCThe CRC8
To validate the integrity of the received bytes you can check the CRC8.
This functions can be used to do it. 

uint8_t update_crc8(uint8_t crc, uint8_t crc_seed){
uint8_t crc_u, i;
crc_u = crc;
crc_u ^= crc_seed;
for ( i=0; i<8; i++) crc_u = ( crc_u & 0x80 ) ? 0x7 ^ ( crc_u << 1 ) : ( crc_u << 1 );
return (crc_u);
}

uint8_t get_crc8(uint8_t *Buf, uint8_t BufLen){
uint8_t crc = 0, i;
for( i=0; i<BufLen; i++) crc = update_crc8(Buf[i], crc);
return (crc);
}


it dont needs much recources.
Converting the received values to standard units
int8_t Temperature = Temperature in 1°C
uint16_t Voltage = Volt *100 so 1000 are 10.00V
uint16_t Current = Ampere * 100 so 1000 are 10.00A
uint16_t Consumption = Consumption in 1mAh
uint16_t ERpm = Electrical Rpm /100 so 100 are 10000 Erpm
note: to get the real Rpm of the motor you will need to divide the Erpm result by the magnetpole count divided by two.
So with a 14magnetpole motor:
Rpm = Erpm/7

*/
