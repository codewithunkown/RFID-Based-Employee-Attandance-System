#include <LPC21xx.H>

#include"header.h"


#define RS (1<<17)

#define RW (1<<18)

#define EN (1<<19)


void lcd_data(unsigned char data)

{

 int temp;

 IOCLR1=0X00FE0000;

 temp=(data&0xf0)<<16;

 IOSET1=temp;

 IOSET1=RS;

 IOCLR1=RW;

 IOSET1=EN;

 delay_ms(2);

 IOCLR1=EN;

 IOCLR1=0X00FE0000;

 temp=(data&0x0f)<<20;

 IOSET1=temp;

 IOSET1=RS;

 IOCLR1=RW;

 IOSET1=EN;

 delay_ms(2);

 IOCLR1=EN;

}

void lcd_cmd(unsigned char cmd)

{

 int temp;

 IOCLR1=(0X00FE)<<16;

 temp=(cmd&0xf0)<<16;

 IOSET1=temp;

 IOCLR1=RS;

 IOCLR1=RW;

 IOSET1=EN;

 delay_ms(2);

 IOCLR1=EN;

 IOCLR1=(0X00FE)<<16;

 temp=(cmd&0x0f)<<20;

 IOSET1=temp;

 IOCLR1=RS;

 IOCLR1=RW;

 IOSET1=EN;

 delay_ms(2);

 IOCLR1=EN;

}


void lcd_init(void)

{

PINSEL2=0;

IODIR1|=0X00FE0000;

lcd_cmd(0X2);

lcd_cmd(0X3);

lcd_cmd(0x28);

lcd_cmd(0xE);

lcd_cmd(0x1);

}


void lcd_string(char *ptr)

{

	while(*ptr)

	{

		lcd_data(*ptr);

		ptr++;

	}

}


////////  to print an integer///////


void lcd_integer(long long int n)

{

char a[10];

int i=0;

if(n==0)

{

lcd_data('0');

return;

}

if(n<0)

{

lcd_data('-');

n=-n;

}

while(n)

{

	   a[i++]=n%10+48;

	   n=n/10;

}

for(i--;i>=0;i--)

{

lcd_data(a[i]);

}

}




////////////////// to print an float//////


void lcd_float(float f)

{

	char a[20]={0};

	int i=0,c,c1;

	long long  int n;

	if(f==0)

	{

		lcd_data('0');

		return;

	}

	if(f<0)

	{

	lcd_data('-');

	f=-f;

	}

	n=f*1000000;

	for(c=0;(n%10)==0 ; c++,n=n/10);

	c=6-c;

	c1=c;

	while(n || c1>=0)

	{					  

	if(c==i)

	a[i++]='.';


	a[i++]=n%10+48;

	n=n/10;

	c1--;

	}

	for(i--;i>=0;i--)

	lcd_data(a[i]);			

}	

	///////////







//////////cgram//////////


char a[]={0x17,0x14,0x14,0x1f,0x05,0x05,0x10};

void lcd_cgram(void)

{

int i;

lcd_cmd(0X40);

for(i=0;i<7;i++)

lcd_data(a[i]);

}

