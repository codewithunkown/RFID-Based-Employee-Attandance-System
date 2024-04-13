#include <LPC21xx.H>
#include"header.h"
unsigned int f=0;
unsigned int index1=0;
unsigned char header[40];
int main()
{
	int h,m,s,date,year,mon;
	uart1_init(9600);
	uart0_init(9600);
	lcd_init();
	i2c_init();
	conf();
	uart1_en();
//	uart0_tx_string("started\r\n");
//	lcd_string("data was came");
//	delay_ms(500);
	while(1)
	{
		while(index1<11)
		{
		  lcd_cmd(0X80);
			h=i2c_byte_read_frame(0XD0,0X02);
			m=i2c_byte_read_frame(0XD0,0X01);
			s=i2c_byte_read_frame(0XD0,0X00);
			date=i2c_byte_read_frame(0XD0,0X04);
			year=i2c_byte_read_frame(0XD0,0X06);
			mon=i2c_byte_read_frame(0XD0,0X05);
			if((h>>6)&1)
			{
				lcd_cmd(0X8C);
				if((h>>5)&1)
					lcd_string("PM");
				else
					lcd_string("AM");
			 	h=h&0X1F;
				lcd_cmd(0X80);
			lcd_data((h/0X10)+48);
			lcd_data((h%0X10)+48);
			}
			else
			{
				lcd_cmd(0X80);
				lcd_data((h/0X10)+48);
				lcd_data((h%0X10)+48);
			}
			lcd_data(':');
			lcd_data((m/0X10)+48);
			lcd_data((m%0X10)+48);
			lcd_data(':');
			lcd_data((s/0X10)+48);
			lcd_data((s%0X10)+48);
			
			lcd_cmd(0XC0);
			lcd_string("Scan the card..");
		} 
//		uart1_rx_string(header,12); 
		header[index1++]=',';
		header[index1++]=h/0X10+48;
		header[index1++]=h%0X10+48;
		header[index1++]=',';
		header[index1++]=m/0X10+48;
		header[index1++]=m%0X10+48;
		header[index1++]=',';
		header[index1++]=date/0X10+48;
		header[index1++]=date%0X10+48;
		header[index1++]=',';
		header[index1++]=mon/0X10+48;
		header[index1++]=mon%0X10+48;
		header[index1++]=',';
		header[index1++]=year/0X10+48;
		header[index1++]=year%0X10+48;
		header[index1++]='\r';
		header[index1++]='\n';
		header[index1]='\0';		  
		uart0_tx_string(header);
//		uart1_tx_string(header);
//		lcd_string(header);
		index1=0;
	}
}
