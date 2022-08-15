#include "Macros.h"
#include <i2cmaster.h>
#include <fmradio.h>

int main(void)
{
    unsigned char ret;
    i2c_init();                             // initialize I2C library
    // write 0x75 to EEPROM address 5 (Byte Write) 
    // i2c_start_wait(I2C_INDX+I2C_WRITE);     // set device address and write mode
    i2c_start_wait(I2C_INDX);     // set device address and write mode
    i2c_write(RADIO_REG_CTRL_UNMUTE);                        // write address = 5
    i2c_write(1);                        // write value 0x75 to EEPROM
    i2c_stop();

    // read previously written value back from EEPROM address 5 
    
    i2c_start_wait(I2C_INDX);     // set device address and write mode
    i2c_write(0x05);                        // write address = 5
    // i2c_rep_start(I2C_INDX+I2C_READ);       // set device address and read mode
    i2c_rep_start(I2C_INDX);       // set device address and read mode
    ret = i2c_readNak();                    // read one byte from EEPROM
    i2c_stop();
    for(;;){
	    i2c_start_wait(Dev24C02+I2C_WRITE);     // set device address and write mode
		i2c_write(RADIO_REG_CTRL_SEEKUP);
		i2c_stop();  
	}
	}