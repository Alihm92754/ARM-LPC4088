#include "LPC407x_8x_177x_8x.h"

void delay_ms(long ms);

int main(void)
{
	//1. Set the PCGPIO bit
  LPC_SC->PCONP |= (1<<15);
	
  //2. Configure pins P4.0 to P4.7 as GPIO pins
  LPC_IOCON->P4_0 = 0;
  LPC_IOCON->P4_1 = 0;
  LPC_IOCON->P4_2 = 0;
  LPC_IOCON->P4_3 = 0;
  LPC_IOCON->P4_4 = 0;
  LPC_IOCON->P4_5 = 0;
  LPC_IOCON->P4_6 = 0;
  LPC_IOCON->P4_7 = 0;
	
  //3. Configure the pins P4.0 to P4.7 as output
  LPC_GPIO4->DIR= 0xFF;
	
	while(1){
		//4. Send a High
    LPC_GPIO4->PIN= 0xFF;
    //5.Pause the system for few milliseconds
    delay_ms(500);
    //6.Send a low
    LPC_GPIO4->PIN= 0x00;
    //7.Pause the system for few milliseconds
    delay_ms(500);
	}
}


// delay 1 ms per count @ CCLK 120 MHz
void delay_ms(long ms) 
{
 long i,j;
 for (i = 0; i < ms; i++ )
 for (j = 0; j < 26659; j++ );
}