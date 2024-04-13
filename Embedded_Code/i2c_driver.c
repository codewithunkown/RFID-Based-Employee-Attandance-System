#include <LPC21xx.H>
#include "header.h"

/*typedef unsigned char u8;


typedef signed char s8;


typedef unsigned int u32;


typedef signed int s32;		  */


#define SI ((I2CONSET>>3)&1)

void i2c_init(void)

{

	PINSEL0|=0X50;

	I2SCLH=I2SCLL=75;

	I2CONSET=(1<<6);

}

void i2c_byte_write_frame(u8 sa,u8 mr,u8 data)

{

	// generate start condition

	I2CONSET=(1<<5);

	I2CONCLR=(1<<3);

	while(SI==0);

	I2CONCLR=(1<<5);

	if(I2STAT!=0X08)

	{

		uart0_tx_string("Err: start condi\r\n");

		goto exit;

	}

	

	// send sa+w

	I2DAT=sa;

	I2CONCLR=(1<<3);

	while(SI==0);

	if(I2STAT==0X20)

	{

		uart0_tx_string("Err: sa+w\r\n");

		goto exit;

	}


	//send mr

	I2DAT=mr;

	I2CONCLR=(1<<3);

	while(SI==0);

	if(I2STAT==0X30)

	{

		uart0_tx_string("err: m/r addr");

		goto exit;

	}

	

	//send data

	I2DAT=data;

	I2CONCLR=(1<<3);

	while(SI==0);

	if(I2STAT==0X30)

	{

		uart0_tx_string("err: m/r data");

		goto exit;

	}

	

	//genarate stop con

	exit:

		I2CONSET=1<<4;

		I2CONCLR=1<<3;

}


u8 i2c_byte_read_frame(u8 sa,u8 mr)

{

	u8 temp;

	// generate start condition

	I2CONSET=(1<<5);

	I2CONCLR=(1<<3);

	while(SI==0);

	I2CONCLR=(1<<5);

	if(I2STAT!=0X08)

	{

		uart0_tx_string("Err: start condi\r\n");

		goto exit;

	}


	// send sa+w

	I2DAT=sa;

	I2CONCLR=(1<<3);

	while(SI==0);

	if(I2STAT==0X20)

	{

		uart0_tx_string("Err: sa+w\r\n");

		goto exit;

	}

	

	//send mr

	I2DAT=mr;

	I2CONCLR=(1<<3);

	while(SI==0);

	if(I2STAT==0X30)

	{

		uart0_tx_string("err: m/r addr\r\n");

		goto exit;

	}

	

	// generate re-start condition

	I2CONSET=(1<<5);

	I2CONCLR=(1<<3);

	while(SI==0);

	I2CONCLR=(1<<5);

	if(I2STAT!=0X10)

	{

		uart0_tx_string("Err: re-start condi\r\n");

		goto exit;

	}

	

	//sa+r;

	I2DAT=sa|1;

	I2CONCLR=(1<<3);

	while(SI==0);

	if(I2STAT==0X48)

	{

		uart0_tx_string("Err: sa+r\r\n");

		goto exit;

	}


	

	//raed data from slave

	I2CONCLR=1<<3;

	while(SI==0);

	temp=I2DAT;

	//genarate stop con

	exit:

		I2CONSET=1<<4;

		I2CONCLR=1<<3;

		return temp;

}




