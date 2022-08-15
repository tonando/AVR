#define BOT_H

#define M_LEFT 0x45
#define M_RIGHT 0x21

volatile unsigned char buffer[4];
volatile unsigned char b_cnt;

unsigned char c_motor = M_LEFT;


static void bot_tx(unsigned char _m, int _s){
	if(b_cnt)
		return;
		
	c_motor = _m;
	
	buffer[0] = _m;
	buffer[1] = ~_m;
	buffer[2] = _s;
	buffer[3] = ~_s;
}

ISR(TIM0_OVF_vect){
	if(b_cnt >= sizeof(buffer) * 8){
		b_cnt = 0;
		buffer[0]=0;
		buffer[1]=0;
		buffer[2]=0;
		buffer[3]=0;
	}
	else{
		
	uchar bit,byte;
	bit = b_cnt & 7;
	byte = b_cnt >> 7;
	
#ifdef TX
	if(buffer[0]){
		if(rbi(buffer[byte],bit)) 
			sbi(PORTB,bit);
		else
			cbi(PORTB,bit);
		b_cnt++;
	}
#else
	sbi(buffer[byte],bit);
	b_cnt++;
#endif
	}
	
	
}


