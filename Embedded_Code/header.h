																	 #include<LPC21XX.h>

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;
/////////Delay_MS//////////////////////
void delay_ms(unsigned int ms);
void delay_ms(unsigned int ms);
/////////UART1///////////////////////
void uart1_init(unsigned int baud);
void uart1_tx(unsigned char ch);
unsigned char uart1_rx(void);
void uart1_tx_string(unsigned char *ptr);
////////////////UART1 INT////////////////////////////
void conf(void);
void uart1_en(void);
/////////////UART0//////////////////////////////////
void uart0_init(unsigned int baud);
void uart0_tx(unsigned char ch);
unsigned char uart0_rx(void);
void uart0_tx_string(unsigned char *ptr);
/////////lcd///////////////////////
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_init(void);
void lcd_string(char *ptr);
//////////////I2C///////////////////////////
extern void i2c_init(void);
extern void i2c_byte_write_frame(u8 sa,u8 mr,u8 data);
u8 i2c_byte_read_frame(u8 sa,u8 mr);

void i2c_24hr(u8 mr);
void i2c_12hr(u8 mr);
