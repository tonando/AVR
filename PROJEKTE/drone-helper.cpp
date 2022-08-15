
#include <Macros.h>
// #include <EEPROM.h>

// #define OCR_VAL BAUD_115200
// #include <tiny_serial.h>

// #define MULT	32.25
#define MULT	11.8//( (3300.0/1024.0) * 0.1 )

#define BUZ 	PB1
#define VFC		PB3
#define ABAT 	1
#define BTN 	PB4
uchar LED = 0;

// #define EE_RUNS 0x00
#define BAT_GOOD  (8200)
#define BAT_WARN  (7600)
#define BAT_EMPTY (7250)		

#define TIME_WARN 600

uint adc(uchar);
bool use_eeprom = false;
bool fc_connected = false;
bool brownout = false;
bool beeping = false;
bool timeout = false;
// volatile bool paused = false;
uchar cells=0;

// char buf[30];
/* 
typedef struct {
	uchar secs;
	uchar v_start;
	uchar v_lowest;
	uchar s_lowest;
}run;
 */
// volatile uchar ovf=0;
// volatile uint c_secs=0;
uint c_secs=0;

// uint c_rom=0;
// uchar c_point=0;
// uchar points[4][2];

// bool  read_data();
// void  dump_data();
// void erase_data();
// void save_slot();

void setup_comparator();
// void sendTelemetry();
uint check_battery();
void tone(uchar f,uchar l,int);
void tone(uchar f,uchar l){tone(f,l,0);}
void beep(uchar f,uchar n);


	
uint vbat=0;
// uint ain=0;
// uint lain=0;


bool debug; 

int main (void)
{ 
	// OSCCAL = 200;
	/* 		
	if(debug){
		tiny_serial_init();	// 9600
		serial_print("Hi! \n\r");
		// if(!read_data())
			// serial_print("EEPROM error\n\r");	
		LED = BUZ;
	}
	 */
	OUTPUT(BUZ);
	INPUT(VFC);
	INPUT(3);
	BUTTON(BTN);
	OUTPUT(LED);
	
	ON(LED);
	
	// TCCR1 = 9;
	// TCNT1=0;
	// TIMSK |= B(TOIE1);
	
	uchar cnt=0;
	
	// tone(1,20,-3);
	// _delay_ms(200);
	
	vbat = check_battery();
	uint vold=vbat;


	
	if(vbat >= BAT_GOOD){ 
		tone(10,2,3);
		tone(6,2,-3);
	}

/* 
	for(uchar i=0; i<10;i++)
		EEPROM_write(i,0);


	vbat = check_battery();
	EEPROM_write(2,adc_read(1));
	EEPROM_write(0,adc_read(3));
	EEPROM_write(4,(vbat>>8));
	EEPROM_write(5,(vbat&255));
 */
 
	// setup_comparator();
	PCMSK |= B(BTN) | B(VFC);
	GIMSK |= B(PCIE);
	
	sei();

	while(1){

			if(beeping){
				beep(17,2);
				tone(30,20,-4);
				_delay_ms(400);
				beep(30,4);
				tone(28,15,-2);
				_delay_ms(400);
				
				continue;
			}
			 
			 
			if(timeout){
				
				tone(26,6,4);
				_delay_ms(1000);
				beep(25,4);
				_delay_ms(1000);
				tone(13,6,-4);
				_delay_ms(1000);
				beep(22,4);
				_delay_ms(1000);
			}
			
/* 			
			if(debug){
				sprintf(buf,"%d,%d V \t %d S",(vbat/1000),(vbat%1000), c_secs);
				const char* p = buf;
				serial_print(p);	
				
				if(c_secs > TIME_WARN)
					serial_print("\t TIME");	
				if(vbat < BAT_WARN)
					serial_print("\t LOW");	
				if(brownout){
					serial_print("\t BROWNOUT");	
					brownout = false;
				}
					
				serial_print("\n\r");	
			}
			
 */			
		
			
		if(!(cnt&63)){
			vold = vbat;
			uint v = check_battery();
			
			if(v < vold) vbat--;
			else vbat = v;
			
			
			if(fc_connected)
				setup_comparator();
			else{
				if(READ(VFC)){
					// tone(5,8,-1);
					fc_connected=true;
				}
			}

			
			if(!(cnt&31)){
				
				if(vbat < (BAT_WARN)  && vbat > 5000){
				
					// reads++;
			
					// if(reads >= 20){
					if(vbat < (BAT_EMPTY) && !(cnt&31)){
						tone(15,7,3);
						tone(30,7,-2);
						beep(8,2);
						tone(10,14,-4);
						tone(25,16,2);
					}
					
					if(!(cnt&127)){
						tone(20,1,1);
						tone(23,1,2);
						tone(20,1,1);
						_delay_ms(100);
						tone(26,2,2);
						beep(20,1);
						beep(30,1);
					}	
					
					_delay_ms(150);
				}
				// else{
					// if(reads<20) reads=0;
				
				// }
			}
			// else{
				// if(vbat > 6000) {
					// beep(1,5);
					// beep(1,4);
					// _delay_ms(5);
					// beep(1,3);
					// beep(1,2);
					// cells=2;
				// }
			// }
		}
		
		
		cnt++;
		cnt&=255;
		
		// if(!(cnt&15) && brownout)
			// tone(12,11,-2);
			
		if(!(cnt&31)){
			ON(LED);
			_delay_ms(80);
			OFF(LED);
			
			if(c_secs >= TIME_WARN) 
				timeout=true;
			else
				c_secs++;
				
		}
		else
			_delay_ms(40);
		
	}
	
	return 0;
}
void setup_comparator(){
/* 
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
	 */
}

uint check_battery(){
	// cli();
	float f=adc(ABAT);
	
	// lain = ain;
	// ain = adc(ABAT);
	
	f *= MULT;
	// uint r = (uint)f;
	// sei();
	// return adc(ABAT) * 14;
	return f;
}

ISR(PCINT0_vect){
	
	if(READ(BTN)){ 
		beeping=false;
	}
	else{
		tone(2,30,-3);
		beeping=true;
	}
		// _delay_ms(100); 
	if(!READ(VFC) && fc_connected){
		tone(12,30,3);
		brownout=true;
	}
	// if(fc_connected){
		// tone(3,20,-1);
		// _delay_ms(100); 
	// }
	// else{
	// }
	
	// cli();
	
	
	// PORTB ^= B(LED);
	/* 
			uchar hold=0;
			
			while(!READ(BTN)){
				if(hold<20)
					hold++;
				else
					beep(1,4);
					
				_delay_ms(100);
			}
			
			if(hold < 6){
				paused=!paused;
				ON(LED);
				_delay_ms(500);
				save_slot();
				OFF(LED);
				// if(debug) serial_print("saved\r\n");
				}
			else {
				if(hold > 7) {
					if(hold > 15){
						erase_data();
						// if(debug) serial_print("erased\r\n");
						}
					else{
						// if(c_rom < 10) c_rom++;
							// if(debug) serial_print("slot changed\r\n");
						}
					}
			}
	 */		
	// PORTB ^= B(LED);
	// sei();
	
	// if(debug)	serial_print("BTN \n\r");
}
/* 
void sendTelemetry(){
	// serial_write(27); //Byte 0: Temperature 
	// serial_write((vbat/10)>>8); //Byte 1: Voltage high byte
	// serial_write((vbat/10)&255); //Byte 2: Voltage low byte
	// serial_write(0); //Byte 3: Current high byte
	// serial_write(5); //Byte 4: Current low byte
	// serial_write(0); //Byte 5: Consumption high byte
	// serial_write(0); //Byte 6: Consumption low byte
	// serial_write(0); //Byte 7: Rpm high byte
	// serial_write(0); //Byte 8: Rpm low byte
	// serial_write(230); //Byte 9: 8-bit CRCThe CRC8
}

void save_slot(){
	if(!use_eeprom) return;
	
	// if(debug) serial_print("saving...");	
	
	// uint i=0;
	// while(points[i>>1][1]){
		// EEPROM_write(c_rom+i,points[i>>1][0]);
		// EEPROM_write(c_rom+i+1,points[i>>1][1]);
		// i+=2;
	// }
}
void dump_data(){
	if(!use_eeprom) return;


	serial_print("dumping...");	
	// uint ei = 32*c_rom;
	// for(uchar i=1; i < ei+1; i++){
	for(unsigned int i=0; i < 512; i++){
		if(i&1) continue;
		uchar m = EEPROM_read(i);
		uchar c = EEPROM_read(i+1);
		
		if(!c && !m){
			c_rom = i;
		}
		
		if(c == 255){
			sprintf(buf,"Slot %d\n\r=======\n\r",(m));
			const char* p = buf;
			serial_print(p);	
		}
		else{
			uchar s = m % 60;
			m /= 60;
			
			sprintf(buf,"%d:%d \t %d \t",m,s,c);
			const char* p = buf;
			serial_print(p);	

			if(c == 1)
				serial_print("brownout");
				
			serial_print("\n\r");
		}
	}

void erase_data(){
if(!use_eeprom) return;
if(debug) serial_print("erasing...");	
	for(int i=0; i<512; i++){
		EEPROM_write(i,0);
	}
}
bool read_data(){
if(!use_eeprom) return true;
	uint i=0;
	while(EEPROM_read(i)) 
		i++;
		
	c_rom=i;
		
	if(!i) return false;
		
	return true;
}
 */
 /* 
ISR(ANA_COMP_vect){
	if(brownout) return;
	
	brownout = true;
	
	cli();
	
	// tone(20,30,5);
	beep(20,15);

	
	// points[c_point][0] = c_secs;
	// points[c_point][1] = 1;
	// c_point++;
	
	// ON(LED);
	// beep(5,0);
	// OFF(LED);
	
	sei();
}
 */
/* 
ISR(TIMER1_OVF_vect){

	ovf++;
	
	if(ovf==80){
		beep(22,1);
		ovf=0;
		if(c_secs < TIME_WARN)
			c_secs++;
		else{
			timeout=true;
			TCCR1 = 0;
		}
	}
}
 */
uint adc(uchar c){
	// cli();
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

void tone(uchar f,uchar l, int bend){
	l = constrain(l,1,20);
	f = constrain(f,1,30);
	bend = constrain(bend,-10,10);
	
	uint d = MAX(50,300-(f<<2));
	uint e = 200 + l * (200-(f<<2));// + (l*f*10)/d;
	// if(m) d += MAX(2,(((m+3)*12) / 3));
	
	for(uint i=MIN(3200,e); i;i--){
		ON(BUZ);
		delay_us(d);
		OFF(BUZ);
		delay_us(d);
		
		if(bend && i<400 && !(i&15)){
			if(d+bend>30 && d+bend<260) 
				d+=bend;
		}
		/*
		*/
			// d=MIN(d,450);
		// }
	}


}

void beep(uchar f,uchar n){
	// m = MIN(10,m);
	ON(LED);
	
	for(uint i=n; i;i--){
		tone(f,2);
		
		delay_ms(190 + (f<<2));
		// if(i<50 && d<140) {
			// d+=2;
			
		// }
	}
	
		
	if(!brownout)
		OFF(LED);	
			
	_delay_ms(300);
	// _delay_ms(16);
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
