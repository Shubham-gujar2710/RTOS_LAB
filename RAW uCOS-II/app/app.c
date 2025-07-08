/****************************************************************************************
* 					uCos-II Multitasking Application Code
*****************************************************************************************
*
* Program Name : 
* Description  : This program creates total four uCos-II tasks; out of which
*                main task acts as a parent task and create three child tasks
*
* 
* MicroEmbedded Technologies
* Processor : NXP LPC2148
* Board		: Micro-A748
*
****************************************************************************************/

#include <includes.h>
#include "func.h"

/*--------------- AAPLICATION STACKS ---------*/
/* Create the required number of stacks needed for every child task*/
static  OS_STK  AppTask0stk[256];		
static  OS_STK  AppTask1stk[256];
static  OS_STK  AppTask2stk[256];

/*-------------LOCAL FUNCTION PROTOTYPES--------------*/
void InitBoard(void);

/*--------------- CHILDERN TRASKS --------------*/
static  void  AppTask0   	 (void        *p_arg);			 
static  void  AppTask1     	 (void        *p_arg);			 
static  void  AppTask2  	 (void        *p_arg);			 
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
    InitBoard();										/*initialiase the board BSP*/
 	OSInit();                                 			/* Initialize "uC/OS-II, The Real-Time Kernel"              */
   
	OSTaskCreate(AppTask0,0,&AppTask0stk[256 - 1],4 );									
	OSTaskCreate(AppTask1,0,&AppTask1stk[256 - 1],5 );
	OSTaskCreate(AppTask2,0,&AppTask2stk[256 - 1],6 );
    
	OSStart();  /* Start multitasking (i.e. give control to uC/OS-II)       */
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
   p_arg = p_arg;									   /*Just to avoid compiler Warning 			*/

    while(DEF_TRUE)
    {     
	  /* User Code Here */
	
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
   p_arg = p_arg;											 /* Just to avoid compiler Warning 			*/

   while(DEF_TRUE)
   {
	  /* User Code Here */
	 

   }
}



/*******************************************************************************************
*                                    TASK-2 : AppTask2()
*
* Description : 
*
* Argument(s) : p_arg       Argument passed to 'AppTask2()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : (1) The first line of code is used to prevent a compiler warning 
*					because 'p_arg' is not used.  The compiler should not generate 
*					any code for this statement.
****************************************************************************************/

static  void  AppTask2 (void *p_arg)
{
   p_arg = p_arg;										/* Just to avoid compiler Warning 		*/
   printf("Stepper Motor Task started : ");
    while(DEF_TRUE)
    {
	 /* User Code Here */
	 

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
	StepperInit();							/* Initialize stepper Motor */
	UartInit(9600);							/* Initialise the UART*/
	
}




