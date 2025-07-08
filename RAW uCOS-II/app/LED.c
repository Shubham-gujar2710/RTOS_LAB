/********************************************************/
/* This Program is to demonstrate the LED Functionality */
/*******************************************************
Author 		: Ashish
Board 		: SBC_ARM7
Processor 	: LPC2148

LED Mapping 	: LED0 thru LED7 : P1.24 thru P1.31

Jumper Settings :

********************************************************/

#include<lpc214x.h>

/*----LED Mapping for the board----*/
#define LED_MASK 	0xFF000000


/*---- LED initialize function ----*/
void LEDInit(void)
{
 PINSEL1 &= 0x0000FFFF;

 IODIR1 |= LED_MASK;			// set the direction of the pins as  output : 1
}


void LEDset(unsigned char temp)
{
IOSET1 = (1<<(temp+24));
}

void LEDclr(unsigned char temp)
{
IOCLR1 = (1<<(temp+24));
}
