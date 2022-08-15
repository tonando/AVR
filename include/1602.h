#ifndef LCD_1602_h
#define LCD_1602_h


#include<Macros.h>  
#include<string.h>   
#include<ctype.h>   
#include<stdlib.h>  

/*---------------------------------------------------------------------
-------------------CONTROL BITS OF LCD --------------------------------
This is basically just renaming everything to make it easy to work with
-----------------------------------------------------------------------*/
#define bit_enable		4
#define bit_select		5
// #define rw_pin			5

#define Set_Enable  			sbi(CONTROL_PORT,bit_enable)
#define Clear_Enable  			cbi(CONTROL_PORT,bit_enable)
// #define Write_Lcd	  			cbi(CONTROL_PORT,rw_pin)
// #define Read_Lcd	  			sbi(CONTROL_PORT,rw_pin)
#define Select_DataRegister 			sbi(CONTROL_PORT,bit_select)
#define Select_InstructionRegister  	cbi(CONTROL_PORT,bit_select)
#define updateLCD { \
	Set_Enable; \
	PAUSE; \
	Clear_Enable; \
	PAUSE; \
}

#define PAUSE						_delay_ms(1)	 

// #define SHOW_CURSOR 	0x0f
// #define HIDE_CURSOR 	0x0c
// #define HOME	 		Lcd_Cmd(0x02)
// #define CLR			 	Lcd_Clear();

// uchar cursor_pos;
char data[2][16];

void lcd_init();
void lcd_write( char a);
void lcd_cmd( char a);
void lcd_data( char a);
void lcd_clear();
void lcd_print(char[],uchar);
void lcd_print();
void lcd_print(const char[]);
void lcd_set(uchar,char);
// void Lcd_Print(const char* _s);
// void Data_Lcd(uchar d);
// void lcdPrint(unsigned char* _s);

// void lcd_init(uchar* _data, uchar* _control){
void lcd_init(){
	
	// DATA_PORT = &_data;
	// DATA_PORT = &_data;
	// CONTROL_PORT = &_control;
	
	char init[]={0x43,0x03,0x03,0x02,0x28,0x01,0x0C,0x06,0x02,0x02};

	//   Top Display Row |80|81|82|83|84|85|86|87|88|89|8A|8B|8C|8D|8E|8F|
	//Bottom Display Row |C0|C1|C2|C3|C4|C5|C6|C7|C8|C9|CA|CB|CC|CD|CE|CF|
	Select_InstructionRegister;  // Sets R/S pin on LCD low so the LCD knows we are giving it instructions, not characters
	 for (uchar count = 0; count <= 9; count++)
  {
 lcd_data(init[count]);
  PAUSE;
  }
  
	Select_DataRegister;  // Sets R/S pin on LCD low so the LCD knows we are giving it instructions, not characters
	_delay_ms(100);
} 
void lcd_data(char D){
	
 //data is in Temp Register
  char K1;
  K1=D;
  K1=K1 & 0xF0;
  K1=K1 >> 4;  //Send MSB
  
  DATA_PORT &= 0xF0; 
  DATA_PORT |= (K1 & 0x0F); 
updateLCD;

  K1=D;
  K1=K1 & 0x0F;  //Send LSB
  DATA_PORT &= 0xF0;  
  DATA_PORT |= K1;
updateLCD;
 	
	// DATA_PORT &= 0xf0;
	// DATA_PORT |= _data & 0x0f;


}
void lcd_print(const char* _txt,uchar _row){
	 for (uchar i = 0; i < 16; i++){
		 if(i>sizeof(_txt))
			data[_row][i] = ' ';
		else
			data[_row][i] = _txt[i];
	 }	
	 lcd_print(data[_row],_row);
}
void lcd_print(const char* _txt){
	uchar _row = 0;
	 for (uchar i = 0; i < 31; i++){
		 if(i==16)_row=1;
		 if(i>=sizeof(_txt))
		 data[_row][i] = ' ';
		else
		 data[_row][i] = _txt[i];
	 }
}

void lcd_print(){
	lcd_print(data[0],0);
	lcd_print(data[1],1);
}
void lcd_print(char txt[16],uchar row){
	lcd_cmd(0x02);
	if(row==0) lcd_cmd(0x80);
	else	   lcd_cmd(0xC0);
	
	for(uchar i=0; i<16; i++){
		lcd_write(txt[i]);
	}
}
void lcd_set(uchar i, char txt){
	
	uchar cmd;
	if(i<16) cmd = 0x80 + (i&15);
	else	 cmd = 0xC0 + (i&15);
	lcd_cmd(cmd);
	   
	lcd_write((uchar)txt);     
	
	// cursor_pos++;
	// cursor_pos &= 31;
	
	
	// if(cursor_pos == 16) Lcd_Cmd(0xC0);
	// if(cursor_pos == 0) Lcd_Cmd(0x02);

}
void lcd_cmd(char a){
	Select_InstructionRegister;  // Sets R/S pin on LCD low so the LCD knows we are giving it instructions, not characters
	
	lcd_data(a); 
}
void lcd_write(char a){
	
	Select_DataRegister;  // Sets R/S pin on LCD low so the LCD knows we are giving it instructions, not characters
	
	lcd_data(a); 
}
/* 
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

 */
#endif
