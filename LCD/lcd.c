 #include "LPC407x_8x_177x_8x.h"
 
#define RS_PORT 0
#define RS_PIN 4
#define RS_VAL (1 << RS_PIN)
#define EN_PORT 0
#define EN_PIN 5
#define EN_VAL (1 << EN_PIN)
#define DATA LPC_GPIO4 -> PIN

unsigned char Lcd_line1[] = {"CORTEX DEV BOARD"};
unsigned char Lcd_line2[] = {"LCD DEMO PROGRAM"};
unsigned char i = 0;

void delay_ms(long ms);
void lcd_command(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_init(void);

void delay_ms(long ms) //delay for 1 ms @ cclk 120 mhz
{
  long i,j;
	for(i=0;i<ms;i++)
	for(j=0;j<26659;j++);
}

void lcd_init(void)
{
	delay_ms(100);
	lcd_command(0x33); // wakeup
	lcd_command(0x32); // wakeup
	lcd_command(0x28); // 4 bit mode
	lcd_command(0x0C); // display on and cursor off
	lcd_command(0x06); // Entry mode and shift
	lcd_command(0x01); // Clear LCD
	delay_ms(200);     // Give more time to settle
}

void lcd_command(unsigned char data)
{
	LPC_GPIO0 -> PIN |= RS_VAL; //clear the RS pin
	DATA = data << 24; // MSB 4 bits
	LPC_GPIO0 -> PIN |= EN_VAL;
	LPC_GPIO0 -> CLR |= EN_VAL;
	
	DATA = data << 28; //LSB 4 bits
	LPC_GPIO0 -> PIN |= EN_VAL;
  LPC_GPIO0 -> CLR |= EN_VAL;
	
	delay_ms(5);
}

int main(void)
{
	LPC_SC -> PCONP1 |= (1 << 15); // Power Control
	LPC_IOCON -> P0_4 = 0;
	LPC_IOCON -> P0_5 = 0;
	LPC_IOCON -> P0_28 = 0;
	LPC_IOCON -> P0_29 = 0;
	LPC_IOCON -> P0_30 = 0;
	LPC_IOCON -> P0_31 = 0;
	
	LPC_GPIO0 -> DIR |= 0x30;
	LPC_GPIO4 -> DIR |= (0xF << 28);
	lcd_init();
	while(1)
	{
		lcd_command(0x80);
		for(i=0;Lcd_line1[i]!='\0';i++){
			lcd_data(Lcd_line1[i]);
			delay_ms(50);
		}
		
		lcd_command(0xC0);
		for(i=0;Lcd_line2[i]!='\0';i++){
			lcd_data(Lcd_line2[i]);
			delay_ms(50);
		}
		
		delay_ms(1000);
		lcd_command(0x01);
	}
	
}	