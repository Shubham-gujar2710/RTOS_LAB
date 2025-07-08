/****************************************************************************************
* uCos-II Multitasking Application Code
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

/********** Define Task Priorities ***********/
#define  APP_TASK_START_PRIO                   4
#define  APP_TASK0_PRIO                        5
#define  APP_TASK1_PRIO                        6
#define  APP_TASK2_PRIO                        7



/*--------------- AAPLICATION STACKS ---------*/
static  OS_STK       AppTaskStartStk[APP_TASK_STK_SIZE];
static  OS_STK       AppTask0stk[APP_TASK_STK_SIZE];		/* Create the required number of stacks need for every child task*/
static  OS_STK       AppTask1stk[APP_TASK_STK_SIZE];
static  OS_STK       AppTask2stk[APP_TASK_STK_SIZE];



/*-------------LOCAL FUNCTION PROTOTYPES--------------*/

/*--------------- A PARENT TASK (MAIN TASK) ---------*/
static  void  AppTaskStart (void        *p_arg); 			 /* Main(Parent) Task Function */
static  void  AppTaskCreate(void);				  			 /* Separate Function To Create Child Task(s) */

/*--------------- CHILDERN TRASKS --------------*/
static  void  AppTask0   	 (void        *p_arg);			 
static  void  AppTask1     	 (void        *p_arg);			 
static  void  AppTask2  	 (void        *p_arg);	


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
    BSP_IntDisAll();                          /* Disable all interrupts until we are ready to accept them */
    OSInit();                                 /* Initialize "uC/OS-II, The Real-Time Kernel"              */
	
    OSTaskCreate(AppTaskStart,                               /* Create the starting task i.e. Main Task        */
                    (void *)0,
                    (OS_STK *)&AppTaskStartStk[APP_TASK_STK_SIZE - 1],
                    APP_TASK_START_PRIO);

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)       */
}


/***************************************************************************************
*                                          AppTaskStart()
*
* Description : The startup task. The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument(s) : p_arg       Argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this 
*					statement.
*
*               (2) Interrupts are enabled by uCoss-II once the task starts because 
*					main() has disbled it.
****************************************************************************************/

static  void  AppTaskStart (void *p_arg)
{
    p_arg = p_arg;							/*Just to avoid compiler Warning 			*/

        BSP_Init();                             /* Initialize BSP functions 	*/ 
	InitLCD();								/* Initialize LCD	*/
	kbdInit();								/* Initialize Keyboard	*/
	ADCInit();								/* Initialize ADC	*/
	LEDInit();	  							/* Initialize LED	*/
	UartInit(9600);							/* Initialise the UART*/			

	TxMbox  = OSMboxCreate((void *)0);		/* Create Mailbox	*/

	AppTaskCreate();                        /* Create application tasks  (child tasks)        */

    while(DEF_TRUE)
	{
	 printf(" \r\nMAIN TASK: Created 2 Tasks. Now going to deep sleep...");
	 printf(" \r\n======================================================");
	 OSTimeDlyHMSM(1, 0, 0, 0);
	}
}


/*
*********************************************************************************************************
*                                      AppTaskCreate()
*
* Description :  Create the application tasks.
*
* Argument(s) :  none.
*
* Return(s)   :  none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
	/* Create User Tasks */
	OSTaskCreate(AppTask0,											 // Name of Task
        		(void *)0,											 // Pointer to arguments for task execution
        		(OS_STK *)&AppTask0stk[APP_TASK_STK_SIZE - 1],		 // Pointer to top-of-stack of the assigned stack
        		APP_TASK0_PRIO );

	OSTaskCreate(AppTask1,											 // Name of Task
        		(void *)0,											 // Pointer to arguments for task execution
        		(OS_STK *)&AppTask1stk[APP_TASK_STK_SIZE - 1],		 // Pointer to top-of-stack of the assigned stack
        		APP_TASK1_PRIO );

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
	unsigned char ch;
   	p_arg = p_arg;									   /*Just to avoid compiler Warning 			*/

    while(DEF_TRUE)
    {     
	 /* User Code Here */ 
		if(ch=GetKey())							 /*	if key is pressed	*/
		{
			OSMboxPost(TxMbox, (void*)&ch);		 /* Post the key to the Mailbox */
		}
		OSTimeDlyHMSM(0,0,0,100);
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
	unsigned char *msg,err;
   	p_arg = p_arg;											 /* Just to avoid compiler Warning 			*/

   while(DEF_TRUE)
   {
	 /* User Code Here */
	 	msg = (unsigned char*)OSMboxPend(TxMbox,0,&err);
		LCD_cmd(0x80);				 /*	cursor 1st line 1st position	*/
		LCD_data(*msg);				 /*	display the key pressed	*/
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

    while(DEF_TRUE)
    {
	 /* User Code Here */


   }
}





