/***************************************************************/
/* This Program is to demonstrate the Keypad Functionality */
/****************************************************************
Author 		: Ashish
Board 		: SBC_ARM7
Processor 	: LPC2148

Keypad Mapping :
Row 0 : P0.16					Col 0 : P0.12
Row 1 :	P0.17					Col 1 : P0.13
Row 2 :	P0.18					Col 2 :	P0.14
Row 3 :	P0.19					Col 3 :	P0.15


*****************************************************************/
#include<lpc214x.h>
 
#define	  Col0	1<<12
#define	  Col1	1<<13
#define	  Col2	1<<14
#define	  Col3	1<<15
#define	  Row0	1<<16
#define	  Row1	1<<17
#define	  Row2	1<<18
#define	  Row3	1<<19

unsigned int ROW[]={Row0,Row1,Row2,Row3};
unsigned int COL[]={Col0,Col1,Col2,Col3};
unsigned int KeyCodes[]={	'1','4','7','*',
							'2','5','8','0',
							'3','6','9','#',
							'A','B','C','D'	};


void kbddelay(unsigned int time)
{
int i,j;
for(i=0;i<time;i++)
	for(j=0;j<1000;j++);
}

void kbdInit(void)
{
	PINSEL0 &= 0x00FFFFFF;		// set the pins function as GPIO.
	PINSEL1 &= 0xFFFFFF00;
	IODIR0 &= ~0x000FF000;
	IODIR0 |= 0x0000F000;		// set the direction of pins COL as OUT and ROWS as IN.
	IOSET0 = 0x0000F000;
}


unsigned char GetKey(void)
{
unsigned int col,rowdata,count=0;
	unsigned char key;
	
	
	for(col = 0 ; col < 4 ; col++)
	{
		IOCLR0 = COL[col] ;			// set the column line low
		rowdata = IOPIN0;
		kbddelay(100);
		rowdata = IOPIN0;
		rowdata >>= 16;
	
		if((rowdata & 0x0F)!=0x0F)
		{
			for(count = 0; count < 4 ; count++)
			{
				if((rowdata & 0x01)== 0)
				{
					key = KeyCodes[(col*4)+ count];
					IOSET0 = COL[col] ;			// set the column line HIGH
					return key;
				}
				rowdata >>= 1;
			}
		}
		IOSET0 = COL[col] ;			// set the column line HIGH
	}
	
	return 0;
}

