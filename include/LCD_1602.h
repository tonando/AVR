#ifndef LCD_1602_h
#define LCD_1602_h


#include<Macros.h>  
#include<string.h>   
#include<ctype.h>   
#include<util/delay.h>
#include<stdlib.h>  
// #include<bmp085.h>  

// #define USE_SHIFT_REGISTER

#ifdef USE_SHIFT_REGISTER

	#include "shiftout.h"
	#define CLOCK_PIN	5
	#define MOSI_PIN	3
	#define LATCH_PIN	2
#else
	#define DATA_DDR	 DDRB			// this is where you will change the port if you are using a different AVR
	#define DATA_PORT 	 PORTB
#endif
	#define CONTROL_DDR	 	 DDRC       // this is where you will change the port if you are using a different AVR
	#define CONTROL_PORT 	 PORTC
	#define Enable_Pin		  3
	#define RegSelect_Pin     1
	#define ReadWrite_Pin     2

/*---------------------------------------------------------------------
-------------------CONTROL BITS OF LCD --------------------------------
This is basically just renaming everything to make it easy to work with
-----------------------------------------------------------------------*/

#define Set_Enable  				CONTROL_PORT|=_BV(Enable_Pin)
#define Clear_Enable 				CONTROL_PORT&=~_BV(Enable_Pin)
#define Write_Lcd			    	CONTROL_PORT&=~_BV(ReadWrite_Pin)
#define Read_Lcd			   		CONTROL_PORT|=_BV(ReadWrite_Pin)
#define Select_InstructionRegister 	CONTROL_PORT&=~_BV(RegSelect_Pin)
#define Select_DataRegister	    	CONTROL_PORT|=_BV(RegSelect_Pin)
// #define Data_Lcd(a)					DATA_PORT=a

#define PAUSE						_delay_us(512)	 

#define SHOW_CURSOR 	0x0f
#define HIDE_CURSOR 	0x0c
#define HOME	 	Lcd_Cmd(0x02)
#define CLR			 	Lcd_Clear();
// #define RETURN 			LCD_HOME
	
volatile uchar cursor_pos;


void Init_Ports(void);  
void Init_Lcd(void);  // function sets LCD into instruction mode and reads in all the preferences you have for the display
void Lcd_Send(unsigned char a);    // Function that actually pushes each letter into the LCD driver
void Lcd_Cmd(unsigned char a);
void updateLCD(void);
void Lcd_Print(const char* _s);
void Lcd_Clear(void);
void Data_Lcd(uchar d);
// void itoa();   // initialize itoa, no really necessary, but it removed the warning I got upon compiling
// void lcdPrint(unsigned char* _s);

void Init_Lcd(void){
	char init[7];
	int i;

	// init[0] = 0x38;  // 
	init[0] = 0x38;  // 2z 4p
	init[1] = 0x06;  //  entry mode set - increment cursor position to write left to right with no character shifting
	init[2] = 0x80;  //  home ddram position 0x80 - 0x8F   
	init[3] = 0x0e;  
	//   Top Display Row |80|81|82|83|84|85|86|87|88|89|8A|8B|8C|8D|8E|8F|
	//Bottom Display Row |C0|C1|C2|C3|C4|C5|C6|C7|C8|C9|CA|CB|CC|CD|CE|CF|
	Select_InstructionRegister;
	Write_Lcd;
	for(i=0;i<4;i++)	// same story, set LCD in instruction mode and clock in command data
	{
		Data_Lcd(init[i]);
		updateLCD();
	}  // end for

} 
void Init_Ports(void){
	#ifdef USE_SHIFT_REGISTER
	initShift(CLOCK_PIN,MOSI_PIN,LATCH_PIN);
	#else
	DATA_DDR=0XFF; //Setting data port for output
	#endif
	CONTROL_DDR|=(_BV(Enable_Pin)|_BV(RegSelect_Pin )|_BV(ReadWrite_Pin));
	CONTROL_PORT&=~(_BV(Enable_Pin)|_BV(RegSelect_Pin )|_BV(ReadWrite_Pin)); //setting values to 0 at starting
	// CONTROL_PORT=0x00;
}
void Data_Lcd(uchar d){
	
	#ifdef USE_SHIFT_REGISTER
	shiftOut(d);
	#else
	// DATA_PORT&=0x0f;
	DATA_PORT=d;
	// DATA_PORT|=d&0xf0;
	#endif
}
void Lcd_Send(unsigned char a){
	// if(!cursor_pos) Lcd_Clear();
		// PAUSE;
		
	// cli(); //disable interrupts
	Select_DataRegister;   // Sets R/S pin on LCD HI so the LCD knows we are giving it characters, not instructions
	Write_Lcd;		 // R/W pin of LCD is set low so LCD knows we are writing to it, not reading from
	Data_Lcd(a);     // put ASCII hex code for the given letter/character out on PORTB for the LCD data pins. The ASCII hex codes are handled in the compiler, nice!
	updateLCD();
	// sei();			 // re-enable interrupts
	
	cursor_pos++;
	cursor_pos &= 31;
	
	
	if(cursor_pos == 16) Lcd_Cmd(0xC0);
	if(cursor_pos == 0) Lcd_Cmd(0x02);

	PAUSE;
}
void Lcd_Cmd(unsigned char a){
	Select_InstructionRegister;  // Sets R/S pin on LCD low so the LCD knows we are giving it instructions, not characters
	Write_Lcd;       // R/W pin of LCD is set low so LCD knows we are writing to it, not reading from
	Data_Lcd(a); 
	updateLCD();
}
void Lcd_Goto(unsigned char a){
	unsigned char c;
	if(a > 15)
		c=0xC0;
	else
		c=0x80;
	
	c += a & 15;
	cursor_pos = c;
	Lcd_Cmd(c);
}

void Lcd_Clear(void){
	Lcd_Cmd(0x01);
	PAUSE;
	PAUSE;
	Lcd_Goto(0);
	PAUSE;
}
 void Lcd_Print(const char* _s){
	// static char* tmp;
	uchar sl=strlen(_s),i=0;
	do{
		Lcd_Send(_s[i]);
	}while(++i < sl);

 }
 void Lcd_Print_Float(float _s){ }
 void Lcd_Print_Int(uint32_t _s){
	// i = max(16,i);
	char vuf[10] = {'0'};

	if(_s < 0){
		Lcd_Send('-');
		// _s *= -1;
	}
	itoa(abs(_s),vuf,10);
	for(uchar i=0; isdigit(vuf[i]); i++)
		Lcd_Send(vuf[i]);
 }


void updateLCD(void){
	Set_Enable; 
	PAUSE;
	Clear_Enable; 
	PAUSE;
}

#endif