#include "header.h"
unsigned char header[50];
unsigned int f=0;
unsigned int index1=0;
int  main()
{

u8 h,s,m;
	
//////LCD////////////////

 lcd_init(); //LCD INIT 
 i2c_init();	//I2C INIT
 uart0_init(9600);	//UART0 INIT
 uart1_init(9600);	//UART1 INIT
 conf();
 uart1_en(); 
 ////////////////////////////////////////////

 lcd_cmd(0xC0);		//LCD	2nd LINE EN
 lcd_string("SCAN THE CARD XD");
 delay_ms(100);	
// uart0_tx_string("DS1307 RTC \r\n");

/*SET RTC TIME 11:59:55 */

	i2c_byte_write_frame(0xD0,0x0,0x55);
	i2c_byte_write_frame(0xD0,0x1,0x59);
	i2c_byte_write_frame(0xD0,0x2,0x23);


/*READ RTC TIME & PRINT ON SERIAL TERMINAL*/
/*READ RTC TIME & PRINT ON LCD*/
while(1)
{
	while(index1<11){
	lcd_cmd(0x80);
	h=i2c_byte_read_frame(0xD0,0x2);   // read hr
	m=i2c_byte_read_frame(0xD0,0x1);  //	read min
	s=i2c_byte_read_frame(0xD0,0x0); //	read sec

	lcd_data((h/0x10)+48);	//lcd_H
	lcd_data((h%0x10)+48);	//lcd_H
	lcd_data(':');			//lcd_H

	lcd_data((m/0x10)+48);	//lcd_m
	lcd_data((m%0x10)+48);	//lcd_m
	lcd_data(':');			//lcd_m

	lcd_data((s/0x10)+48);	//lcd_s
	lcd_data((s%0x10)+48);	//lcd_s

//		lcd_cmd(0x8c);
//		if((h>>6)&1)
//		{
//			if((h>>5&)1)
//			lcd_data("PM");
//			else
//			lcd_data("AM");

			lcd_cmd(0xc0);
	        lcd_string("SCAN YOUR ID XD \r\n");
		}
	   header[index1++]='\r'	;
	   header[index1++]='\n';	
	   header[index1++]='\0';
	   uart0_tx_string(header);
	   index1=0;
}
}
