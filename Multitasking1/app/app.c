/****************************************************************************************
*						 uCos-II MailBox
*****************************************************************************************
*
* MicroEmbedded Technologies
* Processor : NXP LPC2148
* Board		: Micro-A748 SI
*
****************************************************************************************/

#include <includes.h>		 	/* includes all the ucos ii functionality */
#include "func.h"				/* includes all the microembedded board functionality */


/*--------------- AAPLICATION STACKS ---------*/
OS_STK  AppTask0stk[256];		/* Create the required number of stacks need for every child task*/
OS_STK  AppTask1stk[256];
OS_STK  AppTask2stk[256];
OS_STK  AppTask3stk[256];

/*--------------- CHILDERN TRASKS --------------*/
void  AppTask0(void *);			 
void  AppTask1(void *);		 
void  AppTask2(void *);		 
void  AppTask3(void *);





/**********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none
*
* Return(s)   : none
**********************************************************************************************************/

int  main (void)
{
 //   BSP_Init();								/*	Initialize Board specific programs	*/
	BSP_IntDisAll();                          /* Disable all interrupts until we are ready to accept them */
	BSP_Init();
	InitLCD();								/* Initialize LCD	*/
	kbdInit();								/* Initialize Keyboard	*/
	ADCInit();								/* Initialize ADC	*/
	LEDInit();	  							/* Initialize LED	*/
	UartInit(9600);							/* Initialise the UART*/
	StepperInit();
	OSInit();                               /* Initialize "uC/OS-II, The Real-Time Kernel"              */
	
/*----------write your application here------------------*/

	OSTaskCreate(AppTask0, 0, &AppTask0stk[255], 4);
	OSTaskCreate(AppTask1, 0, &AppTask1stk[255], 5);
	OSTaskCreate(AppTask2, 0, &AppTask2stk[255], 6);
	OSTaskCreate(AppTask3, 0, &AppTask3stk[255], 7);

/*---------------------------------------------------------*/
	OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)       */
}



void  AppTask0(void *p_arg)
{
   	p_arg = p_arg;									   /*Just to avoid compiler Warning 			*/
	while(1)
    {  
		LEDset(0);
		OSTimeDlyHMSM(0,0,0,100);
		LEDclr(0);
		OSTimeDlyHMSM(0,0,0,100);
    }
}


void AppTask1(void *p_arg)
{
   	p_arg = p_arg;											 /* Just to avoid compiler Warning 			*/
   	while(1)
   	{
	 	LEDset(1);
		OSTimeDlyHMSM(0,0,0,200);
		LEDclr(1);
		OSTimeDlyHMSM(0,0,0,200);
   	}
}


void AppTask2(void *p_arg)
{
   	p_arg = p_arg;											 /* Just to avoid compiler Warning 			*/
   	while(1)
   	{
	 	LEDset(2);
		OSTimeDlyHMSM(0,0,0,300);
		LEDclr(2);
		OSTimeDlyHMSM(0,0,0,300);
   	}
}


void AppTask3(void *p_arg)
{
   	p_arg = p_arg;											 /* Just to avoid compiler Warning 			*/
   	while(1)
   	{
	 	LEDset(3);
		OSTimeDlyHMSM(0,0,0,400);
		LEDclr(3);
		OSTimeDlyHMSM(0,0,0,400);
   	}
}
