#include <LPC21xx.H>

#define RDR (U0LSR&1)
#define THRE ((U0LSR>>5)&1)
void uart0_init(unsigned int baud)
{
	unsigned int pclk,result;
	int a[]={15,60,30,15,15};
	pclk=a[VPBDIV]*1000000;
	result=pclk/(16*baud);
	PINSEL0|=0X5;
	U0LCR=0X83;
	U0DLL=result&0xFF;
	U0DLM=(result>>8)&0XFF;
	U0LCR=0X03;
}

void uart0_tx(unsigned char ch)
{
	U0THR=ch;
	while(THRE==0);
}

unsigned char uart0_rx(void)
{
	while(RDR==0);
	return U0RBR;
}


void uart0_tx_string(unsigned char *ptr)
{
	while(*ptr)
	{
		uart0_tx(*ptr);
		ptr++;
	}
}

