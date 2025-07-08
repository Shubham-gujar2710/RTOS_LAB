/****************************************************************************************
* 				uCos-II Semaphore Symcronisation
*****************************************************************************************
*
* Program Name : 
* 
* MicroEmbedded Technologies
* Processor : NXP LPC2148
* Board		: Micro-A748
*
****************************************************************************************/

#include <includes.h>
#include "func.h"

void InitBoard(void);

/*--------------- AAPLICATION STACKS ---------*/
static  OS_STK  AppTask0stk[256];		/* Create the required number of stacks need for every child task*/
static  OS_STK  AppTask1stk[256];

/*--------------- CHILDERN TRASKS --------------*/
static  void  AppTask0(void *p_arg);			 
static  void  AppTask1(void *p_arg);			 


OS_EVENT *Sem;                                /* Semaphore  */
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
    InitBoard();
	OSInit();                                 /* Initialize "uC/OS-II, The Real-Time Kernel"              */
	
   
	Sem = OSSemCreate(0);					/* Create a Semaphore */

	OSTaskCreate(AppTask0,0,&AppTask0stk[256 - 1],4 );									
	OSTaskCreate(AppTask1,0,&AppTask1stk[256 - 1],5 );
    
	OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)       */
}


/*******************************************************************************************
*                                    TASK-0 : AppTask0()
*
* Description : 
*
* Argument(s) : p_arg       Argument passed to 'AppTask0()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : (1) The first line of code is used to prevent a compiler warning 
*					because 'p_arg' is not used.  The compiler should not generate 
*					any code for this statement.
****************************************************************************************/
static  void  AppTask0 (void *p_arg)
{
   unsigned char err;
   	p_arg = p_arg;									   /*Just to avoid compiler Warning 			*/

    while(DEF_TRUE)
    {     
	 /* User Code Here */ 
		do									   /*	Wait till a key is pressed	*/
		{
			printf("TASK0: Press any key on Keypad to release Semaphore\n\r");
			OSTimeDlyHMSM(0,0,2,0);
		}while(!(GetKey()));
		OSSemPost(Sem);								   /*	Signal Semaphore	*/
		OSTimeDlyHMSM(0,0,0,200);
		do
		{
		 	printf("TASK0: Waiting for Semaphore signal\n\r");
			OSSemPend(Sem,200,&err); 
		}while(err != OS_NO_ERR);						   /*	Wait on Semaphore	*/
		printf("TASK0: Semaphore Acquired.... TASK0 will now perform operations....\n\r");
		OSTimeDlyHMSM(0,0,1,0);
    }
}


/*******************************************************************************************
*                                    TASK-1 : AppTask1()
*
* Description : 
*
* Argument(s) : p_arg       Argument passed to 'AppTask1()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : (1) The first line of code is used to prevent a compiler warning 
*					because 'p_arg' is not used.  The compiler should not generate 
*					any code for this statement.
****************************************************************************************/

static  void  AppTask1 (void *p_arg)
{
unsigned char err;
   	p_arg = p_arg;											 /* Just to avoid compiler Warning 			*/

   	while(DEF_TRUE)
   	{
	 /* User Code Here */
	 	do
		{
		 	printf("TASK1: Waiting for Semaphore signal\n\r");
			OSSemPend(Sem,200,&err);								 /*	Wait on Semaphore	*/
		}while(err != OS_NO_ERR);
		printf("TASK1: Semaphore Acquired.... TASK1 will now perform operations....\n\r");
		OSTimeDlyHMSM(0,0,1,0);								 
		do												 /*	Wait till a key pressed	*/
		{
			OSTimeDlyHMSM(0,0,2,0);
			printf("TASK1: Press any key on Keypad to release Semaphore\n\r");
		}while(!(GetKey()));
		OSSemPost(Sem);										 /*	Signal Semaphore	*/
		OSTimeDlyHMSM(0,0,0,200);

   	}
}


/*******************************************************************************************
*                                   Board Initialization 
*
* Description :  program to initilise the chip for peripherals
*
* Argument(s) : void'.
*
* Return(s)   : none.
*
* ****************************************************************************************/
void InitBoard(void)
{
	BSP_IntDisAll();                          /* Disable all interrupts until we are ready to accept them */
	BSP_Init();
	InitLCD();								/* Initialize LCD	*/
	kbdInit();								/* Initialize Keyboard	*/
	ADCInit();								/* Initialize ADC	*/
	LEDInit();	  							/* Initialize LED	*/
	UartInit(9600);							/* Initialise the UART*/
}




