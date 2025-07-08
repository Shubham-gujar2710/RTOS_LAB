/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                              NXP LPC2148
*                                                on the
*                                     Keil MCB2140 Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : JJL
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_GLOBALS
#include <includes.h>
#include <stdio.h>
#include "func.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
                                                                /* ---------------------- GPIO0 Pins ---------------------- */
#define  GPIO0_TXD0             DEF_BIT_00
#define  GPIO0_RXD0             DEF_BIT_01
#define  GPIO0_TXD1             DEF_BIT_08
#define  GPIO0_RXD1             DEF_BIT_09
#define  GPIO0_INT1             DEF_BIT_14
#define  GPIO0_SCK1             DEF_BIT_17
#define  GPIO0_MISO1            DEF_BIT_18
#define  GPIO0_MOSI1            DEF_BIT_19
#define  GPIO0_SSEL1            DEF_BIT_20
#define  GPIO0_AVOUT            DEF_BIT_25
#define  GPIO0_DPP              DEF_BIT_26
#define  GPIO0_DPM              DEF_BIT_27
#define  GPIO0_AVIN             DEF_BIT_28


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

        CPU_INT32U  VIC_SpuriousInt;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


static  void  BSP_PLL_Init   (void);
//static  void  BSP_IO_Init    (void);
static  void  BSP_MAM_Init   (void);

static  void  Tmr_TickInit   (void);

static  void  VIC_Init       (void);
static  void  VIC_Dummy      (void);
static  void  VIC_DummyWDT   (void);
static  void  VIC_DummyTIMER0(void);
static  void  VIC_DummyTIMER1(void);
static  void  VIC_DummyUART0 (void);
static  void  VIC_DummyUART1 (void);
static  void  VIC_DummyPWM0  (void);
static  void  VIC_DummyI2C   (void);
static  void  VIC_DummySPI   (void);
static  void  VIC_DummyRTC   (void);
static  void  VIC_DummyEINT0 (void);
static  void  VIC_DummyEINT1 (void);
static  void  VIC_DummyEINT2 (void);

/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*********************************************************************************************************
*/

void  BSP_Init(void)
{
    MEMMAP = 2;                                                 /* Remap 64 bytes of int. RAM to 0x00                       */

    OS_CPU_InitExceptVect();

    BSP_PLL_Init();               /*Mangesh: Done by app startup code */   /* Initialize PLL0 and the VPB Divider Register             */
    BSP_MAM_Init();                                             /* Initialize the Memory Acceleration Module                */
    //BSP_IO_Init();                                              /* Initialize the board's I/Os                              */
	
    VIC_Init();                                            /* Initialize the Vectored Interrupt Controller             */

    Tmr_TickInit();                                             /* Initialize the uC/OS-II tick interrupt                   */

}


/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Get the CPU clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    CPU_INT32U  msel;
    CPU_INT32U  cpu_clk_freq;


    msel         = (CPU_INT32U)(PLLCFG & 0x1F);
    cpu_clk_freq = CPU_OSC_FREQ * (msel + 1);
    return (cpu_clk_freq);
}


/*
*********************************************************************************************************
*                                            BSP_CPU_PclkFreq()
*
* Description : Get the peripheral clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : The peripheral clock frequency, in Hz.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_PclkFreq (void)
{
    CPU_INT32U  msel;
    CPU_INT32U  vpbdiv;
    CPU_INT32U  clk_freq;
    CPU_INT32U  pclk_freq;


    msel         = (CPU_INT32U)(PLLCFG   & 0x1F);
    clk_freq     = CPU_OSC_FREQ * (msel + 1);

    vpbdiv       = (CPU_INT32U)(VPBDIV & 0x03);
    switch (vpbdiv) {
        case 0:
             pclk_freq = clk_freq / 4;
             break;

        case 1:
             pclk_freq = clk_freq;
             break;

        case 2:
             pclk_freq = clk_freq / 2;
             break;

        default:
             pclk_freq = clk_freq / 4;
             break;
    }
    return (pclk_freq);
}


/*
*********************************************************************************************************
*                                          OS_CPU_ExceptHndlr()
*
* Description : Handle any exceptions.
*
* Argument(s) : except_id     ARM exception type:
*
*                                  OS_CPU_ARM_EXCEPT_RESET             0x00
*                                  OS_CPU_ARM_EXCEPT_UNDEF_INSTR       0x01
*                                  OS_CPU_ARM_EXCEPT_SWI               0x02
*                                  OS_CPU_ARM_EXCEPT_PREFETCH_ABORT    0x03
*                                  OS_CPU_ARM_EXCEPT_DATA_ABORT        0x04
*                                  OS_CPU_ARM_EXCEPT_ADDR_ABORT        0x05
*                                  OS_CPU_ARM_EXCEPT_IRQ               0x06
*                                  OS_CPU_ARM_EXCEPT_FIQ               0x07
*
* Return(s)   : none.
*
* Caller(s)   : OS_CPU_ARM_EXCEPT_HANDLER(), which is declared in os_cpu_a.s.
*********************************************************************************************************
*/

void  OS_CPU_ExceptHndlr (CPU_INT32U  except_id)
{
    CPU_FNCT_VOID  pfnct;


    if (except_id == OS_CPU_ARM_EXCEPT_IRQ) {

        pfnct = (CPU_FNCT_VOID)VICVectAddr;                     /* Read the interrupt vector from the VIC                   */
        while (pfnct != (CPU_FNCT_VOID)0) {                     /* Make sure we don't have a NULL pointer                   */
          (*pfnct)();                                           /* Execute the ISR for the interrupting device              */
            VICVectAddr = 1;                                    /* Acknowlege the VIC interrupt                             */
            pfnct = (CPU_FNCT_VOID)VICVectAddr;                 /* Read the interrupt vector from the VIC                   */
        }

    } else {

                                                                /* Infinite loop on other exceptions.                       */
                                                                /* Should be replaced by other behavior (reboot, etc.)      */
        while (DEF_TRUE) {
            ;
        }
    }
}


/*
*********************************************************************************************************
*                                           BSP_IntDisAll()
*
* Description : Disable ALL interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    VICIntEnClear = 0xFFFFFFFFL;                                /* Disable ALL interrupts                                   */
}


/*
*********************************************************************************************************
*********************************************************************************************************
**                                     uC/OS-II TIMER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            Tmr_TickInit()
*
* Description : Initialize uC/OS-II's tick source.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

static  void  Tmr_TickInit (void)
{
    CPU_INT32U  pclk_freq;
    CPU_INT32U  tmr_reload;

                                                                /* VIC TIMER #0 Initialization                              */
    VICIntSelect &= ~(1 << VIC_TIMER0);                         /* Make Timer0 as IRQ                                       */
    VICVectAddr2  = (CPU_INT32U)Tmr_TickISR_Handler;            /* Set the vector address                                   */
    VICVectCntl2  = 0x20 | VIC_TIMER0;                          /* Enable vectored interrupts slot for Timer0                              */
    VICIntEnable  = (1 << VIC_TIMER0);                          /* Enable Interrupts                                        */

    pclk_freq     = BSP_CPU_PclkFreq();
    tmr_reload    = pclk_freq / OS_TICKS_PER_SEC;
    T0TCR         = 0;                                          /* Disable timer 0.                                         */
    T0PC          = 0;                                          /* Prescaler is set to no division.                         */

    T0MR0         = tmr_reload;
    T0MCR         = 3;                                          /* Interrupt on MR0 (reset TC)                              */

    T0CCR         = 0;                                          /* Capture is disabled.                                     */
    T0EMR         = 0;                                          /* No external match output.                                */
    T0TCR         = 1;                                          /* Enable timer 0                                           */
}


/*
*********************************************************************************************************
*                                       Tmr_TickISR_Handler()
*
* Description : Handle the timer interrupt that is used to generate TICKs for uC/OS-II.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  Tmr_TickISR_Handler (void)
{
    T0IR = 0xFF;                                                /* Clear timer #0 interrupt                                 */
	//	  printf("Tick\r\n");
    OSTimeTick();                                               /* Call uC/OS-II's OSTimeTick()                             */
}


/*
*********************************************************************************************************
*********************************************************************************************************
**                                          LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        BSP_PLL_Init()
*
* Description : Set up and activate the PLL
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

static  void  BSP_PLL_Init (void)
{

#if OS_CRITICAL_METHOD == 3                                     /* Allocate storage for CPU status register                 */
    OS_CPU_SR   cpu_sr = 0;
#endif
    CPU_INT16U  loop_ctr;

                                                                /* Configure PLL0, which determines the CPU clock           */
    PLLCFG   = 0x00000024;                                      /* Use PLL values of M = 4 and P = 2                        */
    PLLCON  |= 0x00000001;                                      /* Set the PLL Enable bit                                   */

    OS_ENTER_CRITICAL();
    PLLFEED  = 0xAA;                                            /* Write to the PLL Feed register                           */
    PLLFEED  = 0x55;
    OS_EXIT_CRITICAL();

    loop_ctr = 10000;                                           /* Wait for the PLL to lock into the new frequency          */
    while (((PLLSTAT & DEF_BIT_10) == 0) && (loop_ctr > 0)) {
        loop_ctr--;
    }

    PLLCON  |= 0x00000002;                                      /* Connect the PLL                                          */

    OS_ENTER_CRITICAL();
    PLLFEED  = 0xAA;                                            /* Write to the PLL Feed register                           */
    PLLFEED  = 0x55;
    OS_EXIT_CRITICAL();

   // VPBDIV   = 0x00000002;                                      /* Set the VPB frequency to one-half of the CPU clock       */
    VPBDIV   = 0x00000000;                                      /* Set the VPB frequency to one-half of the CPU clock       */
}


/*
*********************************************************************************************************
*                                          BSP_IO_Init()
*
* Description : Initilize the GPIO pins.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     :  (1) The pins P1.36-26 are setup as a Debug port and the pins P1.25-16 are setup as a
*                   trace port (by the last statements in this function).  If pins for one of these
*                   are to be used as GPIO, comment out the appropriate statement.
*
*               (2) Additional hardware may be necessary for using the Trace or Debug capability.
*                   According to section 23.5 of the LPC2148 user manual (UM10139, rev. 02):
*
*                       "To have these pins [P1.25-16] come as a Trace port, connect a
*                        weak bias resistor (4.7 kOhm) between the P1.20/TRACESYNC pin and Vss."
*
*                   Similarly, according to section 22.5 of the LPC2148 user manual,
*
*                       "To have them [pins P1.36-26] come up as a Debug port, connect a weak
*                        bias resistor (4.7-10kOhm depending on the external JTAG circuitry)
*                        between Vss and the P1.26/RTCK pin"
*********************************************************************************************************
*/
#if 0
static  void  BSP_IO_Init (void)
{
#if 0
//    SCS      = 0x00000001;                                      /* Fast GPIO registers will be used                         */

    PINSEL0  = 0x00000000;

    IO0DIR  &= ~GPIO0_INT1;                                     /* Push button INT1                                         */
    

                                                                /* The following statements setup the Trace/Debug           */
                                                                /* capabilitiy. If the pins indicated are to be used as     */
                                                                /* GPIO, remove or comment out these statements.            */

    PINSEL2  |= 0x00000004;                                     /* The pins P1.36-26 will be used as a Debug port.          */
//  PINSEL2  |= 0x00000008;                                     /* The pins P1.25-16 will be used as a Trace port.          */
#endif

    PINSEL0  = 0x00000000;
    PINSEL2  |= 0x00000004;                                     /* The pins P1.36-26 will be used as a Debug port.          */

}
#endif

/*
*********************************************************************************************************
*                                             BSP_MAM_Init()
*
* Description : Initialize the memory acceleration module.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

static  void  BSP_MAM_Init (void)
{
    MAMCR  = 0x00;                                              /* Disable the Memory Accelerator Module                    */
    MAMTIM = 0x03;                                              /* MAM fetch cycles are 3 CCLKs in duration                 */
    MAMCR  = 0x02;                                              /* Enable the Memory Accelerator Module                     */
}

/*
*********************************************************************************************************
*********************************************************************************************************
**                                            VIC FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        Vectored Interrupt Controller
*********************************************************************************************************
*/

static  void  VIC_Init (void)
{
    VICIntEnClear = 0xFFFFFFFF;                                 /* Disable ALL interrupts                                   */
    VICProtection = 0;                                          /* Setup interrupt controller                               */

    VICVectAddr1  = (CPU_INT32U)VIC_DummyWDT;                   /* Set the vector address                                   */
    VICVectAddr2  = (CPU_INT32U)VIC_DummyTIMER0;
    VICVectAddr3  = (CPU_INT32U)VIC_DummyTIMER1;
    VICVectAddr4  = (CPU_INT32U)VIC_DummyUART0;
    VICVectAddr5  = (CPU_INT32U)VIC_DummyUART1;
    VICVectAddr6  = (CPU_INT32U)VIC_DummyPWM0;
    VICVectAddr7  = (CPU_INT32U)VIC_DummyI2C;
    VICVectAddr8  = (CPU_INT32U)VIC_DummySPI;
    VICVectAddr9  = (CPU_INT32U)VIC_DummyRTC;
    VICVectAddr10 = (CPU_INT32U)VIC_DummyEINT0;
    VICVectAddr11 = (CPU_INT32U)VIC_DummyEINT1;
    VICVectAddr12 = (CPU_INT32U)VIC_DummyEINT2;
}

static  void  VIC_Dummy (void)
{
    while (1) {
        (void)VIC_SpuriousInt;
    }
}

static  void  VIC_DummyWDT (void)
{
    VIC_SpuriousInt = VIC_WDT;
    VIC_Dummy();
}

static  void  VIC_DummyTIMER0 (void)
{
    VIC_SpuriousInt = VIC_TIMER0;
    VIC_Dummy();
}

static  void  VIC_DummyTIMER1 (void)
{
    VIC_SpuriousInt = VIC_TIMER1;
    VIC_Dummy();
}

static  void  VIC_DummyUART0 (void)
{
    VIC_SpuriousInt = VIC_UART0;
    VIC_Dummy();
}

static  void  VIC_DummyUART1 (void)
{
    VIC_SpuriousInt = VIC_UART1;
    VIC_Dummy();
}

static  void  VIC_DummyPWM0 (void)
{
    VIC_SpuriousInt = VIC_UART1;
    VIC_Dummy();
}

static  void  VIC_DummyI2C (void)
{
    VIC_SpuriousInt = VIC_I2C;
    VIC_Dummy();
}

static  void  VIC_DummySPI (void)
{
    VIC_SpuriousInt = VIC_SPI;
    VIC_Dummy();
}

static  void  VIC_DummyRTC (void)
{
    VIC_SpuriousInt = VIC_RTC;
    VIC_Dummy();
}

static  void  VIC_DummyEINT0 (void)
{
    VIC_SpuriousInt = VIC_EINT0;
    VIC_Dummy();
}

static  void  VIC_DummyEINT1 (void)
{
    VIC_SpuriousInt = VIC_EINT1;
    VIC_Dummy();
}

static  void  VIC_DummyEINT2 (void)
{
    VIC_SpuriousInt = VIC_EINT2;
    VIC_Dummy();
}


/*
*********************************************************************************************************
*                                         PB_GetStatus()
*
* Description : Get the status of a push button on the board.
*
* Argument(s) : pb      The ID of the push button to probe
*
*                       1    probe the push button INT1
*
* Return(s)   : DEF_FALSE   if the push button is pressed
*               DEF_TRUE    if the push button is not pressed
*********************************************************************************************************
*/

CPU_BOOLEAN  PB_GetStatus (CPU_INT08U pb)
{
    CPU_BOOLEAN  status;


    status = DEF_FALSE;

    switch (pb) {
        case 1:
             if ((IO0PIN & GPIO0_INT1) == 0) {
                 return (DEF_TRUE);
             }
             break;

        default:
             break;
    }

    return (status);
}






/*
*********************************************************************************************************
*********************************************************************************************************
**                                          uC/OS-View FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              OSView_TmrInit()
*
* Description : Select & initialize a timer for use with uC/OS-View.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

#if (OS_VIEW_MODULE > 0)
void  OSView_TmrInit (void)
{
    T1PR  = 0;
    T1TCR = 0x00000001;                                         /* Enable the timer                                         */

}
#endif


/*
*********************************************************************************************************
*                                         OSView_TmrRd()
*
* Description : Read the current counts of a 32-bit free running timer.
*
* Argument(s) : none.
*
* Return(s)   : The 32 bit counts of the timer.
*********************************************************************************************************
*/

#if (OS_VIEW_MODULE > 0)
CPU_INT32U  OSView_TmrRd (void)
{
    return ((CPU_INT32U)T1TC);
}
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                             uC/Probe PLUG-IN FOR uC/OS-II FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       OSProbe_TmrInit()
*
* Description : Select & initialize a timer for use with the uC/Probe Plug-In for uC/OS-II.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN == 1)
void  OSProbe_TmrInit (void)
{
    T1PR  = 0;
    T1TCR = 0x00000001;                                         /* Enable the timer                                         */

}
#endif


/*
*********************************************************************************************************
*                                        OSProbe_TmrRd()
*
* Description : Read the current counts of a 32-bit free running timer.
*
* Argument(s) : none.
*
* Return(s)   : The 32bit counts of the timer.
*********************************************************************************************************
*/

#if (uC_PROBE_OS_PLUGIN > 0) && (OS_PROBE_HOOKS_EN == 1)
CPU_INT32U  OSProbe_TmrRd (void)
{
    return ((CPU_INT32U)T1TC);
}
#endif


