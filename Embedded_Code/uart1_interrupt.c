#include"header.h"
extern unsigned int f;
extern unsigned int index1;
extern char header[40];
void uart1_irq(void) __irq
{
	f=U1IIR;
	if(f&4)
	{
		header[index1]=U1RBR;
		index1++;
	}
//	uart1_tx(index1+48);
	VICVectAddr=0;
}
void conf()
{
	VICIntSelect=0;
	VICVectCntl0=(1<<5)|7;
	VICVectAddr0=(int)uart1_irq;
	VICIntEnable=1<<7;	
}

void uart1_en()
{
	U1IER=0X3;
}
