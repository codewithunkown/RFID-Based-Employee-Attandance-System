#include <LPC21xx.H>
#define RDR (U1LSR&1)
#define THRE ((U1LSR>>5)&1)
void uart1_init(unsigned int baud)
{
	unsigned int pclk,result;
	int a[]={15,60,30,15,15};
	pclk=a[VPBDIV]*1000000;
	result=pclk/(16*baud);
	PINSEL0|=0X50000;
	U1LCR=0X83;
	U1DLL=result&0xFF;
	U1DLM=(result>>8)&0XFF;
	U1LCR=0X03;
}

void uart1_tx(unsigned char ch)
{
	U1THR=ch;
	while(THRE==0);
}

unsigned char uart1_rx(void)
{
	while(RDR==0);
	return U1RBR;
}


void uart1_tx_string(unsigned char *ptr)
{
	while(*ptr)
	{
		uart1_tx(*ptr);
		ptr++;
	}
}

void uart1_rx_string(unsigned char *ptr,int size)
{
	int i=0;
	while(i<size)
	{
		ptr[i]=uart1_rx();
		if(ptr[i]=='\r')
			break;
		i++;
	}
	ptr[i]='\0';
}

