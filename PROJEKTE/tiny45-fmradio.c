#include <Macros.h>
#include <avr/eeprom.h>

#define SDA_release             DDRB &= 0xF7
#define SDA_low                 DDRB |= 0x08
#define SCL_release             DDRB &= 0xEF
#define SCL_low                 DDRB |= 0x10


#define FREQ_STEPS 10
#define RADIO_REG_CHIPID  0x00
#define RADIO_REG_CTRL    0x02
#define RADIO_REG_CTRL_OUTPUT 0x8000
#define RADIO_REG_CTRL_UNMUTE 0x4000
#define RADIO_REG_CTRL_MONO   0x2000
#define RADIO_REG_CTRL_BASS   0x1000
#define RADIO_REG_CTRL_SEEKUP 0x0200
#define RADIO_REG_CTRL_SEEK   0x0100
#define RADIO_REG_CTRL_RDS    0x0008
#define RADIO_REG_CTRL_NEW    0x0004
#define RADIO_REG_CTRL_RESET  0x0002
#define RADIO_REG_CTRL_ENABLE 0x0001

#define RADIO_REG_CHAN    0x03
#define RADIO_REG_CHAN_SPACE     0x0003
#define RADIO_REG_CHAN_SPACE_100 0x0000
#define RADIO_REG_CHAN_BAND      0x000C
#define RADIO_REG_CHAN_BAND_FM      0x0000
#define RADIO_REG_CHAN_BAND_FMWORLD 0x0008
#define RADIO_REG_CHAN_TUNE   0x0010
#define RADIO_REG_CHAN_NR     0x7FC0
#define RADIO_REG_R4    0x04
#define RADIO_REG_R4_EM50   0x0800
#define RADIO_REG_R4_SOFTMUTE   0x0200
#define RADIO_REG_R4_AFC   0x0100
#define RADIO_REG_VOL     0x05
#define RADIO_REG_VOL_VOL   0x000F

#define RADIO_REG_RA      0x0A
#define RADIO_REG_RA_RDS       0x8000
#define RADIO_REG_RA_RDSBLOCK  0x0800
#define RADIO_REG_RA_STEREO    0x0400
#define RADIO_REG_RA_NR        0x03FF

#define RADIO_REG_RB          0x0B
#define RADIO_REG_RB_FMTRUE   0x0100
#define RADIO_REG_RB_FMREADY  0x0080


#define RADIO_REG_RDSA   0x0C
#define RADIO_REG_RDSB   0x0D
#define RADIO_REG_RDSC   0x0E
#define RADIO_REG_RDSD   0x0F

// I2C-Address RDA Chip for sequential  Access
#define I2C_SEQ  0x20

// I2C-Address RDA Chip for Index  Access
#define I2C_INDX  0x22


#define   RADIO_BAND_NONE     0x0
#define   RADIO_BAND_FM       0x1
#define   RADIO_BAND_FMWORLD  0x2

uint16_t registers[12];
uint8_t  data[5];
//uint8_t fm, vol, BASS, MONO, ns;
bool BTNUP, BTNDN, BTNSK ;




void scan(void)
{
  data[4] = 0;
  data[0]=0;
  int ch_prev = 0;
  EEPROM_write(4, 1);
  setFrequency();

 for (int i = 0; i < 100; i++)
 {
   registers[RADIO_REG_CTRL] |= RADIO_REG_CTRL_SEEKUP | RADIO_REG_CTRL_SEEK;
  _saveRegister(RADIO_REG_CTRL);
  _delay_ms(400); 
  _readRegisters();
  uint16_t ch = registers[RADIO_REG_RA] & RADIO_REG_RA_NR;
  if (ch > ch_prev) {
  if (registers[RADIO_REG_RA] & RADIO_REG_RA_STEREO)
    {data[4]++; EEPROM_write(data[4]+4, ch);
     ch_prev = ch; }
 }
 EEPROM_write(4, data[4]);
 registers[RADIO_REG_CTRL] &= (~RADIO_REG_CTRL_SEEK);
 _saveRegister(RADIO_REG_CTRL);
}}

void setVol(void)
{  registers[RADIO_REG_VOL] = (0x9080 | data[1]); 
   _saveRegister(RADIO_REG_VOL);
   EEPROM_write(1, data[1]);
}

void setBassBoost()
{ registers[RADIO_REG_CTRL] &= (~RADIO_REG_CTRL_SEEK);
  if (data[3]) registers[RADIO_REG_CTRL] |= RADIO_REG_CTRL_MONO;
  if (data[2])  registers[RADIO_REG_CTRL] |= RADIO_REG_CTRL_BASS;
  _saveRegister(RADIO_REG_CTRL);
}

void setFrequency(void) {
 uint8_t newChannel;
  uint16_t regChannel = 0; //registers[RADIO_REG_CHAN] & (RADIO_REG_CHAN_SPACE | RADIO_REG_CHAN_BAND);
  newChannel = EEPROM_read(data[0]+4);
  regChannel += RADIO_REG_CHAN_TUNE; // enable tuning
  regChannel |= newChannel << 6;
  // enable output and unmute
  registers[RADIO_REG_CTRL] |= RADIO_REG_CTRL_OUTPUT | RADIO_REG_CTRL_UNMUTE | RADIO_REG_CTRL_ENABLE; //  | RADIO_REG_CTRL_NEW
  _saveRegister(RADIO_REG_CTRL);
  registers[RADIO_REG_CHAN] = regChannel;
  _saveRegister(RADIO_REG_CHAN);
   
  registers[RADIO_REG_R4] = RADIO_REG_R4_EM50;
  _saveRegister(RADIO_REG_R4);
} // setFrequency()

void _readRegisters()
{ 
    i2c_start(I2C_SEQ+1);
    for (byte i = 0; i < 2; i++) 
  {registers[0xA+i]= _read16();}
    i2c_stop();
}

uint16_t _read16(void)
{ uint8_t hiByte = i2c_read();
  uint8_t loByte = i2c_read();
  return(256*hiByte + loByte);
} // _read16

// Save one register back to the chip
void _saveRegister(byte regNr)
{
  i2c_start(I2C_INDX);
  i2c_write(regNr);
  _write16(registers[regNr]);
   i2c_stop();
} // _saveRegister

void _write16(uint16_t val)
{
  i2c_write(val >> 8); 
  i2c_write(val & 0xFF);
} // _write16

void i2c_clk(void)
{
  SCL_release;    //Release Clock
  _delay_us(5);
  SCL_low;      //Pull Clock Low
}
char i2c_read()
{
         unsigned char inData;
         inData=0x00;
         for(int inBits = 0; inBits < 8; inBits++)
         {
                SDA_release;    
                SCL_release;                    //Clock High
               _delay_us(5);
                inData <<= 1;
                inData |=(!(!(PINB & 0x08)));
                SCL_low;        //Clock Low
        }
         SDA_low;              //Release Data to check for ACK
         i2c_clk();
         SDA_release;
    return inData;
}

/**************************************************************/
void i2c_write(char data)
{
byte  i;

  for (i=0;i<8;i++)            //Clocks out byte MSB first
  {
    if (data & 0x80)  {SDA_release;}  //Release Data
    else        {SDA_low;};   //Pull Data Low
    data = data * 2;
    i2c_clk();
  }
         SDA_low;              //Release Data to check for ACK
         i2c_clk();
         SDA_release;
}
/**************************************************************/
void i2c_start(char data)
{ 

  SDA_low;    //Pull Data Low
  SCL_low;    //Pull Clock Low
  i2c_write(data);
}
/**************************************************************/
void i2c_stop(void)
{
  SCL_release;  //Release Clock
  SDA_release ; //Release Data
}
void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
  while(EECR & (1<<EEWE)); /*Ждать завершения предыдущей записи*/
  EEAR = uiAddress; /*Проинициализировать регистры*/
  EEDR = ucData;
  EECR |= (1<<EEMWE); /*Установить флаг EEMWE*/
  EECR |= (1<<EEWE); /*Начать запись в EEPROM*/
}
unsigned char EEPROM_read(unsigned int uiAddress)
{
  while(EECR & (1<<EEWE)); /*Ждать завершения предыдущей записи*/
  EEAR = uiAddress; /* Проинициализировать регистр адреса*/
  EECR |= (1<<EERE); /*Выполнить чтение*/
  return EEDR;
}


int main( void ){
  DDRB = 0x00;    //PORTB = Inputs
  PORTB = 0x87;   //Set Outputs to Low (when DDRB is set to low the pin associated goes Hi-Z)
   for (byte i = 0; i <5; i++) data[i]=EEPROM_read(i);
  _delay_ms(1000); 
  setVol();
  setFrequency();
  if (!(PINB & (1<<PINB0))) EEPROM_write(2, !(data[2]));
  if (!(PINB & (1<<PINB1))) EEPROM_write(3, !(data[3]));
  if (!(PINB & (1<<PINB2))) scan();
  setBassBoost();
while(1){
     
 if (!(PINB & (1<<PINB2)))
   {data[0] ++;
   if (data[0]>data[4]) data[0]=1;  
   setFrequency();
   EEPROM_write(0, data[0]);
   while((!(PINB & (1<<PINB2))));}
 if (!(PINB & (1<<PINB1)))
   {data[1] --;
   if (data[1]<1) data[1]=1;  
   setVol();
   while((!(PINB & (1<<PINB1))));}
  if (!(PINB & (1<<PINB0)))
   {data[1] ++;
   if (data[1]>15) data[1]=15; 
   setVol();
   while((!(PINB & (1<<PINB0))));}
   _delay_ms(50);
  } 
}