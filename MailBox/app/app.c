/****************************************************************************************
*						 uCos-II MailBox
*****************************************************************************************
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

OS_EVENT *TxMbox;

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
	
/*----------write your application here------------------*/
	TxMbox  = OSMboxCreate((void *)0);		/* Create Mailbox	*/

	OSTaskCreate(AppTask0,0,&AppTask0stk[256-1],4);
	OSTaskCreate(AppTask1,0,&AppTask1stk[256-1],5);

/*---------------------------------------------------------*/
	OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)       */
}


static  void  AppTask0 (void *p_arg)
{
   unsigned char msg;
	unsigned int i;
   	p_arg = p_arg;									   /*Just to avoid compiler Warning 			*/

   
	while(1)
    {  
		printf("TASK0: press any key on PC Keyboard\n\r");
		OSTimeDlyHMSM(0,0,0,100);   	   	
		msg = UART_GetChar();
		OSMboxPost(TxMbox,&msg);
		OSTimeDlyHMSM(0,0,1,0);
    }
}

static void AppTask1(void *p_arg)
{
	unsigned char err, *msg;
   	p_arg = p_arg;											 /* Just to avoid compiler Warning 			*/

	printf("TASK1: Waiting for message from Mailbox\n\r");
   	while(1)
   	{
	 	/* User Code Here */
	 	msg = (unsigned char*)OSMboxPend(TxMbox,0,&err);
		printf("TASK1: Message received....\n\r\n\r");
		LCD_cmd(0x01);
		LCD_data(*msg);
		OSTimeDlyHMSM(0,0,1,0);

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

