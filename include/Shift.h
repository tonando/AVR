#ifndef Shift_h
#define Shift_h

#include <Macros.h>

#define MOSI PB0
#define CLK PB2
//static uint8_t CLK,MOSI;



// typedef struct shift_register_struct{
	// byte* data;
	// byte chain_length;
//	char out:3;
//	char  clock:3;
//	char  latch:3;
//	char  reset:3;
	// uint8_t last_ddr:3;
	// bool restore_pins;
// }ShiftRegister;
/*
void initShift(uint8_t _out, uint8_t _clock){
	MOSI = _out;
	CLK = _clock;
	DDRB |= CLK | MOSI;
	PORTB &= ~(CLK | MOSI);
}
*/
/*
void initShift(ShiftRegister* sh, uint8_t _outpin, uint8_t _clockpin, uint8_t _latchpin){
	sh->data = 0xff;
	sh->out = _outpin;
	sh->clock = _clockpin;
	sh->latch = _latchpin;
	//sh->reset = -1;
	
	// sh->restore_pins = true;
	// sh->last_ddr = IS_OUTPUT(_outpin) | IS_OUTPUT(_clockpin) | IS_OUTPUT(_latchpin);
	
	// SET_OUTPUT(sh->out);
	// SET_OUTPUT(sh->clock);
	// SET_OUTPUT(sh->latch);
}

static void beginShift(ShiftRegister* sh){
	SET_OUTPUT(sh->out);
	SET_OUTPUT(sh->clock);
	SET_OUTPUT(sh->latch);
	
	OFF(sh->clock);
	OFF(sh->latch);
	OFF(sh->out);
}
static void endShift(ShiftRegister* sh){
	ON(sh->latch);
	OFF(sh->out);
	OFF(sh->clock);
	OFF(sh->latch);


	if(sh->restore_pins){
		if(!rbi(sh->last_ddr,0))
			SET_INPUT(sh->out);
		if(!rbi(sh->last_ddr,1))
			SET_INPUT(sh->clock);
		if(!rbi(sh->last_ddr,2))
			SET_INPUT(sh->latch);			
	}
}
*/
// void shiftOutBit(ShiftRegister* sh, uint8_t bit){
void shiftOutBit(uint8_t bit){
	
	PORTB &= ~(CLK | MOSI);
	if(bit&1)
		PORTB |= MOSI;
		
	PORTB |= CLK;
/* 	cli();
	if(bit){
		ON(sh->out);
	}
	else{
		OFF(sh->out);
	}
		
	ON(sh->clock);
#ifdef SHIFT_DELAY
	_delay_us(SHIFT_DELAY);	
#endif	
	OFF(sh->clock);
	sei(); */
}	
//static void shiftOutData(ShiftRegister* sh,uint8_t* _data, byte data_length){
//void shiftOut(ShiftRegister* sh,uint8_t _data){
// void shiftOut(ShiftRegister* sh ){
void shiftOut(uint8_t _data){
	// beginShift(sh);
	
	// do{data_length--;
		// char i=sizeof(uint8_t);
		uint8_t i=8;
		do{i--;
			PORTB &= ~(CLK | MOSI);
			if((_data >> i) & 1)
				PORTB |= MOSI;
			PORTB |= CLK;
		}while(i);
		PORTB &= ~(CLK);
		// _data++;
	// }while(data_length);
	
	// endShift(sh);
}
/*

void shiftOutInt(ShiftRegister* sh,uint16_t _data){
	beginShift(sh);
	
	// do{data_length--;
		char i=sizeof(uint16_t);
		do{i--;
			shiftOutBit(sh,rbi(_data,i));
		}while(i);
		// _data++;
	// }while(data_length);
	
	endShift(sh);
}
void shiftOutLong(ShiftRegister* sh,uint32_t _data){
	beginShift(sh);
	
	// do{data_length--;
		char i=sizeof(uint32_t);
		do{i--;
			shiftOutBit(sh,rbi(_data,i));
		}while(i);
		// _data++;
	// }while(data_length);
	
	endShift(sh);
}
#ifndef QUICK_MODE
	void shiftOut(uint8_t data,uint8_t outpin, uint8_t clockpin, uint8_t latchpin);
	void shiftOut(uint8_t data,uint8_t outpin, uint8_t clockpin);
#else	
	#define SHDDR 	DDRB
	#define SHPORT 	PORTB
	#define SHPIN 	PINB

	#define DEFAULT_CLOCK_PIN 		5
	#define DEFAULT_SHIFT_IN_PIN 	4
	#define DEFAULT_SHIFT_OUT_PIN 	3
	#define DEFAULT_LATCH_PIN 		2
	#define DEFAULT_PS_PIN 			1
	static uint8_t inited;
	static uint8_t out_pin,clock_pin,latch_pin;
	static uint8_t in_pin,ps_pin;	// _BV() masks
	
	void shiftOutInit(uint8_t outpin, uint8_t clockpin, uint8_t latchpin);
	void shiftOut(uint8_t data);
	
	void shiftBitOut(uint8_t bit){
		// cli();
		if(bit)
			SHPORT |= out_pin;
		else
			SHPORT &= ~out_pin;
			
		SHPORT |= clock_pin;
		//	if(SHIFT_DELAY)	_delay_us(SHIFT_DELAY);	
		SHPORT &= ~clock_pin;
		// sei();
	}
				
	void shiftOutInit(byte _outpin, byte _clockpin, byte _latchpin){
		out_pin=_BV(_outpin);
		clock_pin=_BV(_clockpin);
		
		if(latch_pin == 99)
			latch_pin=0;	// nothing happens (PORT |= 0)
		else
			latch_pin=_BV(_latchpin);

		SHDDR |= (out_pin|latch_pin|clock_pin);
		SHPORT &= ~(clock_pin);
		SHPORT |= latch_pin; 
		
		inited = true;
	}

	void shiftOut(uint8_t data){
		if(!inited)
			shiftOutInit(DEFAULT_CLOCK_PIN,DEFAULT_SHIFT_OUT_PIN,DEFAULT_LATCH_PIN);
				
		SHPORT &= ~latch_pin;
		
		uint8_t i=8;
		
		do{--i;
			shiftBitOut( ((data>>i) & 1) ); 
		//  shiftBitOut( ((data >> MSB_FIRST ? i : 7-i) & 1) );
		}while(i);
		
		SHPORT |= latch_pin;

	}
#endif


#endif


uint8_t shiftIn(){
	uint8_t result=0;
	
	return result;
}


struct StepButtons{
	public:
	StepButtons(uint8_t mi,uint8_t mo,uint8_t cl, uint8_t l){
		this->_miso = mi;	
		this->_mosi = mo;
		this->_clock = cl;
		this->_latch = l;		

		SET_OUTPUT(this->_miso);
		SET_OUTPUT(this->_clock);
		SET_OUTPUT(this->_latch);
		SET_INPUT(this->_mosi);

		ON(this->_latch);

		
	};
	StepButtons(){ StepButtons(11,12,13,10); };
	
	uint8_t shift(uint8_t out){
		uint8_t result=0;
		
		OFF(this->_latch);
		OFF(this->_clock);
		
		uint8_t i=8;
		do{i--;
			if(rbi(out,i))
				ON(this->_miso);
			else
				OFF(this->_miso);

			ON(this->_clock);	
			result = (result << 1) | READ(this->_mosi);
			OFF(this->_clock);

		}while(i);

		ON(this->_latch);

		this->_out_uint8_t = out;
		this->_in_uint8_t = result;
		
		return result;
	}

private:

	uint8_t _miso;	
	uint8_t _mosi;
	uint8_t _clock;
	uint8_t _latch;

	uint8_t _out_uint8_t;
	uint8_t _in_uint8_t;

//	uint32_t _last_micros;
};
*/

//void shiftInit();
//void shiftOut(uint8_t data);
//void shiftOut(uint8_t c, uint8_t o, uint8_t l, uint8_t data);
/*
struct ShiftRegister{
	uint8_t _type : 1;
	uint8_t _clk : 3;
	uint8_t _data : 3;
	uint8_t _latch : 3;
};
typedef struct ShiftRegister ShiftRegister;

void shiftInit(ShiftRegister *sr){
	sr->_type = 0;
	sr->_clk = _BV(5);
	sr->_data = _BV(3);
	sr->_latch = _BV(2);
	
	SHDDR |= (sr->_clk|sr->_latch);
	
	if(sr->_type){
		SHDDR &= ~sr->_data;
		}
	else{
		SHDDR |= sr->_data;
		SHPORT |=  sr->_latch;
	}
		
	SHPORT &= ~sr->_clk;
}
//void shiftOut(uint8_t data){ shiftOut(DEFAULT_CLOCK_PIN,DEFAULT_SHIFT_OUT_PIN,DEFAULT_LATCH_PIN,data); }

*/
#endif