#include "LPC407x_8x_177x_8x.h"
#include <stdio.h>

#define DHT11_Port_Conf (LPC_IOCON -> P4_0)
#define DHT11_Dir (LPC_GPIO4 -> DIR)
#define DHT11_Set (LPC_GPIO4 -> SET)
#define DHT11_Clr (LPC_GPIO4 -> CLR)
#define DHT11_Pin (LPC_GPIO4 -> PIN)

#define port_pin_number 0
#define high (1 << port_pin_number)
#define low  (0 << port_pin_number)

#define PRESCALE_1 (30)
#define PRESCALE_2 (30000)

static unsigned char dht11_data[8], data_string_1[8], data_string_2[8], display[30] = "HUMIDITY & TEMPERATURE --";

void Config_DHT11(void);
void Init_DHT11(void);
unsigned char receive_bytes(void);
void Delay_ms(long ms);
void Delay_us(long us);
void Init_GPIO(void);
void integer_to_ascii(unsigned int Val, unsigned char *str);
void Init_Uart0(void);
void UART0_Print(unsigned char *);
void UART0_Txt(unsigned char);
void Init_Timer_1(void);
void Init_Timer_2(void);

void Init_Timer_2(void)
{
	LPC_SC -> PCONP |= (1 << 22);
	//LPC_IOCON -> P0_6  =  0x3;
	LPC_TIM2 -> CTCR = 0x0;
	LPC_TIM2 -> PR = PRESCALE_2 - 1;
	LPC_TIM2 -> TCR = 0x2;
}	

void Init_Timer_1(void)
{
	LPC_SC -> PCONP |= (1 << 2);
	//LPC_IOCON -> P0_6  =  0x3;
	LPC_TIM1 -> CTCR = 0x0;
	LPC_TIM1 -> PR = PRESCALE_1 - 1;
	LPC_TIM1 -> TCR = 0x2;
}	

void Init_Uart0(void)
{
	//1. set bit PCUART0
	LPC_SC -> PCONP |= (1 << 3);
	
	//2. Configure pins P0.2 and p0.3 as UART0 TX and RX
	LPC_IOCON -> P0_2 |= 1;  //pin p0.2 used as TXD0
	LPC_IOCON -> P0_3 |= 1;  //pin p0.3 used as RXD0
	
	//3. Select clock source and frequency = PCLK i.e 30MHZ
	// 8 bits, no parity, 1 stop bit
	// set DLAB = 1 to access baud rate register
  LPC_UART0 -> LCR = 0x83;

  //4. Derive baud rate from the UART clock source,
	//DLM:DLL=PCLK/(16*baud) = 30MHZ/(16*115200) = 16
  LPC_UART0 -> DLL = 16;  //115200 BAUD RATE @ 30.0 MHZ PCLK
	LPC_UART0 -> DLM = 0;   //MSB = 0
  LPC_UART0 -> LCR = 0x03; 
}


void UART0_Txt(unsigned char character)
{
	while((LPC_UART0 -> LSR & 0x20) == 0);
	LPC_UART0 -> THR = character;
}

void UART0_Print(unsigned char *print_string)
{
	while(*print_string)
		UART0_Txt(*print_string++);
}

void integer_to_ascii(unsigned int Val, unsigned char *str)
{
	str[0] = Val/1000 + 0x30;
	Val = Val%1000;
	str[1] = Val/100 + 0x30;
	Val = Val%100;
	str[2] = Val/10 + 0x30;
	str[3] = Val%10 + 0x30;
	str[4] = '\0';
}

void Init_GPIO(void)
{
	LPC_SC -> PCONP |= (1 << 15);
}

void Delay_ms(long ms)
{
	LPC_TIM2 -> TCR = 0x2;
	LPC_TIM2 -> TCR = 0x1;
	while(LPC_TIM2 -> TC < ms);
	LPC_TIM2 -> TCR = 0x0;
}

void Delay_us(long us)
{
	LPC_TIM1 -> TCR = 0x2;
	LPC_TIM1 -> TCR = 0x1;
	while(LPC_TIM1 -> TC < us);
	LPC_TIM1 -> TCR = 0x0;
}

void Config_DHT11(void)
{
	DHT11_Port_Conf = 0;
	DHT11_Dir = high;
	DHT11_Set = high;
	Delay_ms(100);
}

unsigned char receive_bytes(void)
{
	unsigned char a = 0;
	unsigned char out;
	for (a=0;a<=7;a++){
		while((DHT11_Pin & high) == 0);
		Delay_us(35);
		if((DHT11_Pin & high) == 1){
			out |= 1;
			out = out << 1;
		}
		else{
			out |= 0;
			out = out << 1;
		}
		while((DHT11_Pin & high) == 1);
	}
	return (out);
}

void Init_DHT11(void)
{
	unsigned char a = 0;
	DHT11_Dir = high;
	DHT11_Set = high;
	DHT11_Clr = high;
	Delay_ms(25);
	DHT11_Set = high;
	DHT11_Dir = low;
	
	while((DHT11_Pin & high) == 1);
	while((DHT11_Pin & high) == 0);
	while((DHT11_Pin & high) == 1);
	
	for(a=0;a<=4;a++){
		dht11_data[a] = receive_bytes();
	}
}

int main(void)
{
	Init_GPIO();
	Init_Timer_1();
	Init_Timer_2();
	Init_Uart0():
	Config_DHT11();
	Delay_ms(500);
	
	while(1)
	{
		Init_DHT11();
		dht11_data[0] = dht11_data[0]/1.60;
		integer_to_ascii(dht11_data[0]),data_string_1);
		dht11_data[2] = dht11_data[2]/2.05;
		integer_to_ascii(dht11_data[2],data_string_2);
		UART0_Print(display);
		UART0_Print(data_string_1);
		UART0_Txt('\t');
		UART0_Print(data_string_2);
		UART0_Txt('\r');
		UART0_Txt('\n');
		Delay_ms(10000);
		Delay_ms(1000);
	}
}
