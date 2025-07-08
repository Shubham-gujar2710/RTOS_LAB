/********************************************************/
/* This Program is to demonstrate the stepper motor Functionality */
/*******************************************************
Author 		: Ashish
Board 		: SBC_ARM7
Processor 	: LPC2148

stepper Mapping 	: P0.4 thru P0.7

Jumper Settings :

********************************************************/

#include<lpc214x.h>

/*----stepper Mapping for the board----*/
#define Stepper_MASK 	0x000000F0


/*---- stepper initialize function ----*/
void StepperInit(void)
{
 PINSEL0 &= 0xFFFF00FF;	  		//Set stepper pins as GPIO
 IODIR0 |= Stepper_MASK;			// set the direction of the pins as  output : 1
}


void StepperSet(unsigned char temp)
{
IOSET0 = 0x000000F0;
IOCLR0 = (temp << 4);
}

void StepperClr(unsigned char temp)
{
IOSET0 = 0x000000F0;
}
