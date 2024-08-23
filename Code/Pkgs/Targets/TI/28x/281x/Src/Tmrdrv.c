/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    tmrdrv.c

PURPOSE:
    device driver for TI F28x EV timer

FUNCTION(S):
    TMRDRV_fnCtor               - create timer instances
    TMRDRV_fnDtor               - destroy timer instances
    TMRDRV_fnStart              - Start the timer
    TMRDRV_fnStop               - Stop the timer
    TMRDRV_fnHold               - Hold the timer
    TMRDRV_fnResume             - resume the timer from hold

    TMRDRV_fnSet                - Set timer control registers 
    TMRDRV_fnSetPrd             - Set a timer period value
    TMRDRV_fnSetPrdFast         - set timer specific period registers
    TMRDRV_fnSetCmp             - Update a timer compare value
    TMRDRV_fnSetDelay           - set timer compare value with delay time
    TMRDRV_fnGetStatus          - Returns the status of the timer.
    TMRDRV_fnGetPrd             - return timer period
    TMRDRV_fnEnInt              - enable a interrupt
    TMRDRV_fnDisInt             - disable a interrupt
    TMRDRV_fnDisIntAll          - disable all interrupts
    TMRDRV_fnClrIntFlag         - clear a interrupt flag
    TMRDRV_fnClrIntFlagAll      - clear all timer interrupt flag
    TMRDRV_fnReset              - reset timer registers

    local:
    tmrdrv_fnCalcTimerParams    - calculation of timer prescaler and period register
    timer_fnInitTimerReg        - init timer registers 
    timer_fnGetId               - assign id from function

NOTES:

CHANGE HISTORY:
$Log: Tmrdrv.c $

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/7
    User: HorstM     Date: 02/22/05  Time: 11:31PM
    removed local isr definitions

    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/6
    User: HorstM     Date: 02/22/05  Time: 06:31PM
    removed IER bit setting/clearing; IER control done in asm isr
    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/5
    User: HorstM     Date: 02/21/05  Time: 12:51AM
    code review changes - changes made to timer driver interface (function names),
    and timer struct member names, among other things
    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/4
    User: HorstM     Date: 02/17/05  Time: 07:27PM
    moved protected register alterations to macro,  added EALLOW/EDIS; slight change
    to isr's
    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/3
    User: HorstM     Date: 02/15/05  Time: 12:25AM
    added isr's; added device register inits in ctor; corrected clock init in ctor;
    corrected timer flag reference syntax and moved declarations to top of isr's
    (coding error)
    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/2
    User: HorstM     Date: 02/14/05  Time: 06:55PM
    corrected clock init in ctor; corrected timer flag reference syntax and moved
    declarations to top of isr's (coding error)
    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/1
    User: HorstM     Date: 02/12/05  Time: 01:17AM
    added isr's; added device register inits in ctor
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/2
    User: JohnB     Date: 01/24/05  Time: 09:14PM
    Updated for use with SYSCLK package. Removed INIT_fnGetSysClk()
    functionality.
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/5
    User: HorstM     Date: 01/19/05  Time: 11:42PM
    merged
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/4
    User: HorstM     Date: 01/19/05  Time: 11:34PM
    moved interrupt en/disabling into this driver
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/1
    User: JohnB     Date: 01/19/05  Time: 06:03PM
    Updated Timer function allocations for FAN and CAPDRV.
    
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/3
    User: HorstM     Date: 12/15/04  Time: 08:26PM
    modified to use renamed constants from F28x.h
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/2
    User: HorstM     Date: 12/08/04  Time: 08:36PM
    fixed headers/comments;
    moved GetClkSpeed function to init package & changed function call accordingly
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/1
    User: HorstM     Date: 12/07/04  Time: 09:34PM
    fixed header tags/comments;
    moved fnGetClkSpeed to init pkg.;
    changed function call accordingly
    
    
Date:  DEC-3-2004 - Horst Maurer
    -   Created
    
==============================================================================*/
/*==============================================================================
                              Includes
==============================================================================*/
#include "Tmrdrv.h"         // this package
#include "SysClk.h"         // for system clock functions
#include "config.h"         // for device-specific definitions
#include "Device.h"         // device-specific constants
#include "DeviceMacro.h"    // for interrupt enable/disable macros in isr's
#include "xassert.h"        // for assert
#include <errno.h>

/*==============================================================================
                             Defines
==============================================================================*/
//------------------------- FEATURE OPTION ENABLES -----------------------------
#define TMRDRV_COMPARE_EN     ( TRUE )  //no compare feature
#define TMRDRV_COMPARE_OUT_EN ( TRUE )  //no compare output
#define TMRDRV_EMU_MODE_EN    ( TRUE )  //emu set to immediate stop
#define TMRDRV_SYN_EN         ( FALSE )  //no synchronize feature
#define TMRDRV_CLOCK_MODE_EN  ( TRUE )  //clock source always internal
#define TMRDRV_ADC_EN         ( TRUE )  //no ADC mode feature
                                
/*------------------------ Function Enables ----------------------------------*/
                                    //---------------- Public ------------------
#define TMR_FN_CTOR_EN          ( TRUE  )//Include code for constructor
#define TMR_FN_SET_EN           ( TRUE  )//Include code for 

#define TMR_FN_SET_PRD_EN       ( TRUE  )//Include code for set period
#define TMR_FN_SET_PRD_FAST_EN  ( TRUE  )//Include code for set period fast
#define TMR_FN_GET_PRD_EN       ( TRUE  )//Include code for get period

#define TMR_FN_SET_COMP_EN      ( TRUE )//Exclude code for set compare
#define TMRDRV_COMPARE_DUTY     ( TRUE )//compare register define by delay
#define TMRDRV_COMPARE_PER      ( TRUE )//compare reg define with duty

#define TMR_FN_START_EN         ( TRUE  )//Include code for start

#if( defined( GLBL_DTOR_EN ) || defined( FBT ) )
#define TMR_FN_STOP_EN          ( TRUE )//Exclude code for stop
#else
#define TMR_FN_STOP_EN          ( FALSE )//Exclude code for stop
#endif

#define TMR_FN_DIS_INT_EN       ( TRUE  )//Include code for interrupt disable
#define TMR_FN_EN_INT_EN        ( TRUE  )//Include code for interrupt enable
#define TMR_FN_CLR_INT_FLAG_EN  ( TRUE  )//Include code for interrupt cleraing

#define TMR_FN_GET_STATUS_EN    ( TRUE  )//Include code for getting the status

#if( defined( GLBL_DTOR_EN ) || defined( FBT ) )
#define TMR_FN_PACK_HOLD_RESUME ( TRUE)//Exclude code for HOLD and RESUME fn's
#else
#define TMR_FN_PACK_HOLD_RESUME ( FALSE )//Exclude code for HOLD and RESUME fn's
#endif
#define TMR_FN_HOLD_EN          ( TMR_FN_PACK_HOLD_RESUME )//Code for Hold
#define TMR_FN_RESUME_EN        ( TMR_FN_PACK_HOLD_RESUME )//Code for Resume


#define TMRDRV_FN_PACK_INIT_EN      ( FALSE )//Exclude code for INT fn's.
#define TMR_FN_CLR_INT_FLAG_ALL_EN ( TMRDRV_FN_PACK_INIT_EN )//Code for clr int
#define TMR_FN_DIS_INT_ALL_EN   ( TMRDRV_FN_PACK_INIT_EN )//Code for dis all ints
#define TMR_FN_INIT_PKG_EN         ( TMRDRV_FN_PACK_INIT_EN )//Code for pkg init

                                    //--------------- Private ------------------
#define TMR_FN_INIT_TMR_REG_EN  ( TRUE  )//Include code for Init Tmr regs
#define TMR_FN_CALC_PARAM_EN    ( TRUE  )//Include code for timer param calc

//------------------------------------------------------------------------------
//default definition
#define TMRDRV_PERIOD_DFLT     ( 0 )
#define TMRDRV_COMPARE_DFLT    ( 0 )
#define TMRDRV_INT_DISABLE_ALL    ( 0x0000 )     //0 disable
#define TMRDRV_INT_FLAG_CLEAR_ALL ( 0xFFFF )     //write 1 to clear int flag
#define TMRDRV_NOT_USED           ( 0 )
#define TMRDRV_USED               ( 1 )
#define TMRDRV_PRESCALER_DFLT     ( 0 )

#define TMRDRV_INT_TOTAL         ( 5 )   //total valid interrupt cases
#define TMRDRV_CNT_MODE_TOTAL    ( 4 )   //total valid count mode
#define TMRDRV_ADC_MODE_TOTAL    ( 4 )   //total valid adc mode
#define TMRDRV_CLK_MODE_TOTAL    ( 3 )   //total valid clock mode    
#define TMRDRV_OUT_MODE_TOTAL    ( 4 )   //total valid compare output mode
#define TMRDRV_COMP_MODE_TOTAL   ( 4 )   //total valid compare mode
#define TMRDRV_EMU_MODE_TOTAL    ( 3 )   //total valid emulation mode
#define TMRDRV_SYN_MODE_TOTAL    ( 4 )   //total valid synchronization mode

//define for general operation
#define TMRDRV_DOUBLE_FREQ   ( 1 )           //shift left by 1 bit
#define TMRDRV_INTEGER_GAIN  ( 10000 )       //100%

//
#define TMRDRV_REG_MAX_PRD   ( 0xFFFF )      //prd register maxmum value
#define TMRDRV_REG_MIN_PRD   ( 5 )           //prd register minimum value

//macros
#define mTMRDRV_CLR_INT_FLAG_ALL()                          \
            *(EvaRegs.EVAIFRA) = TMRDRV_INT_FLAG_CLEAR_ALL; \
            *(EvaRegs.EVAIFRB) = TMRDRV_INT_FLAG_CLEAR_ALL; \
            *(EvaRegs.EVAIFRC) = TMRDRV_INT_FLAG_CLEAR_ALL; \
            *(EvbRegs.EVBIFRA) = TMRDRV_INT_FLAG_CLEAR_ALL; \
            *(EvbRegs.EVBIFRB) = TMRDRV_INT_FLAG_CLEAR_ALL; \
            *(EvbRegs.EVBIFRC) = TMRDRV_INT_FLAG_CLEAR_ALL
        
#define mTMRDRV_CLR_INT_EN_ALL()                            \
            *(EvaRegs.EVAIMRA) = TMRDRV_INT_DISABLE_ALL;    \
            *(EvaRegs.EVAIMRB) = TMRDRV_INT_DISABLE_ALL;    \
            *(EvaRegs.EVAIMRC) = TMRDRV_INT_DISABLE_ALL;    \
            *(EvbRegs.EVBIMRA) = TMRDRV_INT_DISABLE_ALL;    \
            *(EvbRegs.EVBIMRB) = TMRDRV_INT_DISABLE_ALL;    \
            *(EvbRegs.EVBIMRC) = TMRDRV_INT_DISABLE_ALL
                                    
#define mTMRDRV_CLR_CMPR_ALL()                              \
            *(EvaRegs.T1CMPR) = TMRDRV_COMPARE_DFLT;        \
            *(EvaRegs.T2CMPR) = TMRDRV_COMPARE_DFLT;        \
            *(EvbRegs.T3CMPR) = TMRDRV_COMPARE_DFLT;        \
            *(EvbRegs.T4CMPR) = TMRDRV_COMPARE_DFLT

#define mTMRDRV_CLR_PRD_ALL()                               \
            *(EvaRegs.T1PR) = TMRDRV_PERIOD_DFLT;           \
            *(EvaRegs.T2PR) = TMRDRV_PERIOD_DFLT;           \
            *(EvbRegs.T3PR) = TMRDRV_PERIOD_DFLT;           \
            *(EvbRegs.T4PR) = TMRDRV_PERIOD_DFLT

#define mTMRDRV_EVA_CLK_EN() \
                         EALLOW; \
                         SysCtrlRegs.PCLKCR.bit.EVAENCLK = F28X_PCLKCR_CLK_ON; \
                         EDIS

#define mTMRDRV_EVB_CLK_EN() \
                         EALLOW; \
                         SysCtrlRegs.PCLKCR.bit.EVBENCLK   = F28X_PCLKCR_CLK_ON; \
                         EDIS
/*==============================================================================
                            File Variables
==============================================================================*/
//The Timers available to the device.
typedef enum eTIMER_ID
{
    eTIMER1 = 0,
    eTIMER2,
    eTIMER3,
    eTIMER4,
    eTIMER_TOTAL
    
}teTIMER_ID;

//functions that assigned to each timer
typedef enum eTIMER_FUNCTION_ASIGNED
{
    eTIMER1_FUNCTION = TMRDRV_ePWM,
    eTIMER2_FUNCTION = TMRDRV_eSW,
    eTIMER3_FUNCTION = TMRDRV_eFAN,
    eTIMER4_FUNCTION = TMRDRV_eCAP
}teTIMER_FUNCTION_ASIGNED;


typedef struct zTIMER_REGS
{
    volatile uint16 *puiGptcon; //point to timer register GPTCONx 
    volatile uint16 *puiTcon;   //point to timer control register
    volatile uint16 *puiTcnt;   //point to timer counter register
    volatile uint16 *puiTprd;   //point to timer period register
    volatile uint16 *puiTcmpr;  //point to timer compare register
}tzTIMER_REGS;

typedef struct zTIMER_INT_REG
{
    volatile uint16 *puiFlag;   //point to timer interrupt flag register
    volatile uint16 *puiMask;   //point to timer interrupt mask register
    volatile uint16 *puiPIEMask;//point to timer interrupt mask register
             uint16  uiShift;
}tzTIMER_INT_REG;

//timer status structure
typedef struct zTIMER_STATUS
{
             uint16  uiTimerUsed;       //1-this timer is used, 0-available
             uint16  uiTimerPrescaler;  //mirror the timer prescaler value
    volatile uint16 *puiTmrIo;          //points to timer associate io register
    volatile uint16 *puiTmrMcr;         //points to timer io mcr register
    tzTIMER_REGS zTimerRegs;
    tzTIMER_INT_REG zTimerIntRegs;
}tzTIMER_STATUS;

//timer variables to store usage of each timer
// re-defined for F28X - 6/18/04 - HM
static tzTIMER_STATUS zTimers[ eTIMER_TOTAL ] = 
{
    {
        ////////////////////////////
        // timer 1
        TMRDRV_NOT_USED,            // uiTimerUsed
        TMRDRV_PRESCALER_DFLT,      // uiTimerPrescaler
        &(GpioMuxRegs.GPADIR.all),  // puiTmrIo
        &(GpioMuxRegs.GPAMUX.all),  // puiTmrMcr

        // tzTIMER_REGS
        &(EvaRegs.GPTCONA.all),     // puiGptcon
        &(EvaRegs.T1CON.all),       // puiTcon
        &(EvaRegs.T1CNT),           // puiTcnt
        &(EvaRegs.T1PR),            // puiTprd
        &(EvaRegs.T1CMPR),          // puiTcmpr

        // tzTIMER_INT_REG
        &(EvaRegs.EVAIFRA.all),     // puiFlag
        &(EvaRegs.EVAIMRA.all),     // puiMask
        &(PieCtrlRegs.PIEIER2.all), // puiPIEMask
        XT_BITPOS7                  // uiShift
    },

    {
        ////////////////////////////
        // timer 2
        TMRDRV_NOT_USED,
        TMRDRV_PRESCALER_DFLT,
        &(GpioMuxRegs.GPADIR.all),
        &(GpioMuxRegs.GPAMUX.all),

        // tzTIMER_REGS
        &(EvaRegs.GPTCONA.all),
        &(EvaRegs.T2CON.all),
        &(EvaRegs.T2CNT),
        &(EvaRegs.T2PR),
        &(EvaRegs.T2CMPR),

        // tzTIMER_INT_REG
        &(EvaRegs.EVAIFRB.all),
        &(EvaRegs.EVAIMRB.all),
        &(PieCtrlRegs.PIEIER3.all),
        XT_BITPOS0
    },

    {
        ////////////////////////////
        // timer 3
        TMRDRV_NOT_USED,
        TMRDRV_PRESCALER_DFLT,
        &(GpioMuxRegs.GPBDIR.all),
        &(GpioMuxRegs.GPBMUX.all),

        // tzTIMER_REGS
        &(EvbRegs.GPTCONB.all),
        &(EvbRegs.T3CON.all),
        &(EvbRegs.T3CNT),
        &(EvbRegs.T3PR),
        &(EvbRegs.T3CMPR),

        // tzTIMER_INT_REG
        &(EvbRegs.EVBIFRA.all),
        &(EvbRegs.EVBIMRA.all),
        &(PieCtrlRegs.PIEIER4.all),
        XT_BITPOS7
    },

    {
        ////////////////////////////
        // timer 4
        TMRDRV_NOT_USED,
        TMRDRV_PRESCALER_DFLT,
        &(GpioMuxRegs.GPBDIR.all),
        &(GpioMuxRegs.GPBMUX.all),

        // tzTIMER_REGS
        &(EvbRegs.GPTCONB.all),
        &(EvbRegs.T4CON.all),
        &(EvbRegs.T4CNT),
        &(EvbRegs.T4PR),
        &(EvbRegs.T4CMPR),

        // tzTIMER_INT_REG
        &(EvbRegs.EVBIFRB.all),
        &(EvbRegs.EVBIMRB.all),
        &(PieCtrlRegs.PIEIER5.all),
        XT_BITPOS0
    }
};

#if( TMRDRV_SYN_EN == TRUE )
// synchronize timer
// timer 1 always syncs with timer 2, and vice-versa,
// timer 3 always syncs with timer 4, and vice-versa
const uint16 TimerSync[ eTIMER_TOTAL ] =
{ 
    eTIMER2,    //timer1
    eTIMER1,    //timer2
    eTIMER4,    //timer3
    eTIMER3     //timer4
};
#endif

//timer enable bit 
// re-defined for F28X - 6/18/04 - HM
const uint16 TimerEnable[ eTIMER_TOTAL ] =
{ 
    F28X_T1CON_TENABLE_EN,
    F28X_T2CON_TENABLE_EN,
    F28X_T3CON_TENABLE_EN,
    F28X_T4CON_TENABLE_EN
};

// CPU-level interrupt enable (IER) bit for each timer
// re-defined for F28X - 6/18/04 - HM
const uint16 TimerImrBit[ eTIMER_TOTAL ] =
{
    F28X_IER_INT2_ENA,   // EVA timer1: CPU INT2
    F28X_IER_INT3_ENA,   // EVA timer2: CPU INT3
    F28X_IER_INT4_ENA,   // EVB timer3: CPU INT4
    F28X_IER_INT5_ENA    // EVB timer4: CPU INT5
};   

// PIE controller interrupt enable (PIEIER) mask for each timer
// masks all four interrupt types for a particular timer
const uint16 TimerPieIerBits[ eTIMER_TOTAL ] =
{
    F28X_PIEIERX_INT_EN & F28X_PIE_T1_ALL_MASK,   // PIEIER2
    F28X_PIEIERX_INT_EN & F28X_PIE_T2_ALL_MASK,   // PIEIER3
    F28X_PIEIERX_INT_EN & F28X_PIE_T3_ALL_MASK,   // PIEIER4
    F28X_PIEIERX_INT_EN & F28X_PIE_T4_ALL_MASK    // PIEIER5
};   

//timer prescaler msk bit 
// re-defined for F28X - 6/18/04 - HM
const uint16 TimerPrescalerMsk[ eTIMER_TOTAL ] =
{
    F28X_T1CON_TPS_MSK,      // EVA timer1: prescaler bits
    F28X_T2CON_TPS_MSK,      // EVA timer2: prescaler bits
    F28X_T3CON_TPS_MSK,      // EVA timer3: prescaler bits
    F28X_T4CON_TPS_MSK       // EVA timer4: prescaler bits
};   

//timer prescaler shift bits 
// re-defined for F28X - 6/18/04 - HM
const uint16 TimerPrescalerSft[ eTIMER_TOTAL ] =
{
    F28X_T1CON_TPS0,     //timer1
    F28X_T2CON_TPS0,     //timer2
    F28X_T3CON_TPS0,     //timer3
    F28X_T4CON_TPS0      //timer4
}; 

#if( TMRDRV_COMPARE_OUT_EN == TRUE )
//timer compare output io pin function bits 
// re-defined for F28X - 6/18/04 - HM
const uint16 TimerMcrBits[ eTIMER_TOTAL ] =
{
    F28X_GPAMUX_MSK_T1PWM_T1CMP,   //timer1
    F28X_GPAMUX_MSK_T2PWM_T2CMP,   //timer2
    F28X_GPBMUX_MSK_T3PWM_T3CMP,   //timer3
    F28X_GPBMUX_MSK_T4PWM_T4CMP    //timer4
}; 
#endif

#if( TMRDRV_COMPARE_OUT_EN == TRUE )
//timer compare output io pin direction bits 
// re-defined for F28X - 6/18/04 - HM
const uint16 TimerIoBits[ eTIMER_TOTAL ] =
{
    F28X_GPADIR_T1DIR_OUTPUT,  //timer1
    F28X_GPADIR_T2DIR_OUTPUT,  //timer2
    F28X_GPBDIR_T3DIR_OUTPUT,  //timer3
    F28X_GPBDIR_T4DIR_OUTPUT   //timer4
}; 
#endif

//timer interrupt bit, same sequence with TMRDRV_teADC_MODE
// used to access int mask reg with no offset (shift), 
// and int flag reg with offset (shift)
const uint16 TimerIntBit[ TMRDRV_INT_TOTAL ] =
{
    F28X_EVXIFR_NO_INT,      //no interrupt
    F28X_EVXIFR_TXPINT,      //period interrupt
    F28X_EVXIFR_TXCINT,      //compare interrupt
    F28X_EVXIFR_TXUFINT,     //underflow interrupt
    F28X_EVXIFR_TXOFINT      //overflow interrupt
};  

//timer interrupt status bit, same sequence with TMRDRV_teADC_MODE
const uint16 TimerIntStBit[ TMRDRV_INT_TOTAL ] =
{
    F28X_EVXIFR_NO_INT,      //no interrupt
    F28X_EVXIFR_TXPINT,      //period interrupt
    F28X_EVXIFR_TXCINT,      //compare interrupt
    F28X_EVXIFR_TXUFINT,     //underflow interrupt
    F28X_EVXIFR_TXOFINT      //overflow interrupt
}; 

//timer counter mode setting bit with same sequence with TMRDRV_teCOUNT_MODE
// re-defined for F28X - 6/18/04 - HM
const uint16 TimerCntModeBit[ TMRDRV_CNT_MODE_TOTAL ] =
{
    F28X_TXCON_TMODE_STOPHOLD,   //stophold mode
    F28X_TXCON_TMODE_CONTUPDOWN, //continuous up down mode
    F28X_TXCON_TMODE_CONTUP,     //continuous up mode 
    F28X_TXCON_TMODE_DIRUPDOWN   //direction up down mode
};

#if( TMRDRV_ADC_EN == TRUE )
//timer adc mode mask bit 
// re-defined for F28X - 6/18/04 - HM
const uint16 TimerAdcModeMsk[ eTIMER_TOTAL ]=
{
    F28X_GPTCONA_T1TOADC_MSK,    //timer1
    F28X_GPTCONA_T2TOADC_MSK,    //timer2
    F28X_GPTCONB_T3TOADC_MSK,    //timer3
    F28X_GPTCONB_T4TOADC_MSK     //timer4
};
#endif

#if( TMRDRV_ADC_EN == TRUE )
// bit-shift count for adc mode bits
const uint16 TimerAdcModeSft[ eTIMER_TOTAL ] =
{
    7,  //timer1
    9,  //timer2
    7,  //timer3
    9   //timer4
}; 
#endif

#if( TMRDRV_ADC_EN == TRUE )
//timer ADC mode setting bit with same sequence with TMRDRV_teADC_MODE
const uint16 TimerAdcModeBits[ TMRDRV_ADC_MODE_TOTAL ] =
{
    F28X_GPTCON_TXTOADC_DISAB,   //separate
    F28X_GPTCON_TXTOADC_COMP,    //compare match
    F28X_GPTCON_TXTOADC_PRD,     //period match
    F28X_GPTCON_TXTOADC_UNDER    //under flow match
};
#endif

#if( TMRDRV_COMPARE_OUT_EN == TRUE )
//timer output mode mask bit 
// re-defined for F28X - 6/18/04 - HM
const uint16 TimerOutModeMsk[ eTIMER_TOTAL ]=
{
    F28X_GPTCONA_T1PIN_MSK,  //timer1
    F28X_GPTCONA_T2PIN_MSK,  //timer2
    F28X_GPTCONB_T3PIN_MSK,  //timer3
    F28X_GPTCONB_T4PIN_MSK   //timer4
};
#endif

#if( TMRDRV_COMPARE_OUT_EN == TRUE )
// bit-shift count for timer output mode bits
const uint16 TimerOutModeSft[ eTIMER_TOTAL ] =
{
    0,  //timer1
    2,  //timer2
    0,  //timer3
    2   //timer4
}; 
#endif

#if( TMRDRV_COMPARE_OUT_EN == TRUE )
//timer Out mode setting bit with same sequence with TMRDRV_teCOMP_OUTPUT
const uint16 TimerOutModeBits[ TMRDRV_OUT_MODE_TOTAL ] =
{
    F28X_GPTCON_TXPIN_FRC_LOW,   //force low
    F28X_GPTCON_TXPIN_ACT_LOW,   //active low
    F28X_GPTCON_TXPIN_ACT_HI,    //active high
    F28X_GPTCON_TXPIN_FRC_HI     //force high
};
#endif

#if( TMRDRV_CLOCK_MODE_EN == TRUE )
//timer clock mode setting bits, same sequence with TMRDRV_teCLOCK
// modified for F28X - 6/18/04 - HM
const uint16 TimerClkModeBits[ TMRDRV_CLK_MODE_TOTAL ] =
{
    F28X_TXCON_TCLK_INT,     // internal
    F28X_TXCON_TCLK_EXT,     // external
    F28X_TXCON_TCLK_QEP      // QEP cct - changed 6/14/04 - HM
};
#endif
    
#if( TMRDRV_EMU_MODE_EN == TRUE )
//timer emulation mode setting bits, same sequence with TMRDRV_teEMU
// modified for F28X - 6/18/04 - HM
const uint16 TimerEmuModeBits[ TMRDRV_EMU_MODE_TOTAL ] =
{
    F28X_TXCON_EMMCTRL_STOP_IMMED,          // stop immediatly
    F28X_TXCON_EMMCTRL_NO_EFFECT2,          // free run
    F28X_TXCON_EMMCTRL_STOP_PER_COMP        // stop after period complete
};
#endif

#if( TMRDRV_SYN_EN == TRUE )
//timer sync mode setting bits, same sequence with TMRDRV_teSYN
const uint16 TimerSynModeBits[ TMRDRV_SYN_MODE_TOTAL ] =
{
    F28X_TXCON_SYN_SEP,      // no sync
    F28X_TXCON_SYN_PRD,      // synchronized period
    F28X_TXCON_SYN_START,    // synchronized start point
    F28X_TXCON_SYN_PRDST     // synchronized period and startpoint
};
#endif

#if( TMRDRV_COMPARE_EN == TRUE )
//timer compare mode setting bits, same sequence with TMRDRV_teCOMP_MODE
// NOTE: these also affect the setting of timer compare enable bit (TECMPR)
const uint16 TimerCompModeBits[ TMRDRV_COMP_MODE_TOTAL ] =
{
    TMRDRV_eNO_COMP,        // no compare
    F28X_TXCON_ZERO_RELOAD,      // reload at zero
    F28X_TXCON_EQUAL_RELOAD,     // reload at period
    F28X_TXCON_IMM_RELOAD        // reload immediatly
};
#endif
/*==============================================================================
                       Local Function Protoypes
==============================================================================*/
//Init timer registers
static tuiSTATUS tmrdrv_fnInitTimerReg( TMRDRV_tzTIMER *ptzTimer );

//calculate timer prescaler and period
static tuiSTATUS tmrdrv_fnCalcTimerParams( TMRDRV_tzPRD *ptzTimer );

//assign id from function
static tuiSTATUS tmrdrv_fnGetId( TMRDRV_tzTIMER *ptzTimer );
/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnCtor
  
PURPOSE: 
    create timer instances, initialize timer variable ucTimerID, according to 
    function defined

INPUTS: 
    a pointer to a tzTIMER structure that needs to be created

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Oct-31-2002  By: Yingran Duan
    - create

Version: 1.00  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.10  Date: May, 23, 2003  By: Yingran Duan
    - use assert to replace errno
    - use function getid to assign id according to function define

Version: 1.20  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.30  Date: JAN-19-2005  By: Horst Maurer
    - moved IER enabling from TMRDRV_fnEnInt
    - added PIEIER enabling

Version: 1.31  Date: FEB-17-2005  By: Horst Maurer
    - added assert of input parameter

*******************************************************************************/
#if( TMR_FN_CTOR_EN == TRUE )                   //Function compile enable
tuiSTATUS TMRDRV_fnCtor( TMRDRV_tzTIMER *ptzTimer)
{
    tzTIMER_INT_REG *ptzTimerIntReg;

    assert( ptzTimer );

    //get id
    ( void )tmrdrv_fnGetId( ptzTimer);

    //check timer availibility, and init timerID if avaliable
    assert( zTimers[ ptzTimer->ucTimerID ].uiTimerUsed == TMRDRV_NOT_USED );

    zTimers[ ptzTimer->ucTimerID ].uiTimerUsed = TMRDRV_USED;

    // make a local copy of the timer's interrupt register pointers
    ptzTimerIntReg = &( zTimers[ ptzTimer->ucTimerID ].zTimerIntRegs );

    //enable interrupt in IER
    IER |= TimerImrBit[ ptzTimer->ucTimerID ];

    // enable PIE controller interrupts
    *( ptzTimerIntReg->puiPIEMask ) |= TimerPieIerBits[ ptzTimer->ucTimerID ];

    // Enables PIE to drive a pulse into the CPU
    PieCtrlRegs.PIEACK.all |= TimerImrBit[ ptzTimer->ucTimerID ];

    // Enable the PIE block
    PieCtrlRegs.PIECRTL.bit.ENPIE = 1;

    // enable the ev clocks
    if( ( ptzTimer->ucTimerID == eTIMER1 ) || 
        ( ptzTimer->ucTimerID == eTIMER2 ) )
    {
        mTMRDRV_EVA_CLK_EN();
    }
    else if( ( ptzTimer->ucTimerID == eTIMER3 ) || 
             ( ptzTimer->ucTimerID == eTIMER4 ) )
    {
        mTMRDRV_EVB_CLK_EN();
    }

    return( eSTATUS_OK );

}//end of TMRDRV_fnCtor
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnDtor
  
PURPOSE: 
    destroy timer instances, stop a timer and clear the timer assignment

INPUTS: 
    a pointer to a tzTIMER structure that needs to be destroyed

OUTPUTS:
    tuiSTATUS

NOTES:
    None

VERSION HISTORY:

Version: 0.10  Date: Nov-1-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure
    
Version: 1.20  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.30  Date: JAN-19-2005  By: Horst Maurer
    - added IER/PIEIER disabling

Version: 1.31  Date: FEB-17-2005  By: Horst Maurer
    - added assert of input parameter

*******************************************************************************/
#if( defined( GLBL_DTOR_EN ) || defined( FBT ) )
tuiSTATUS TMRDRV_fnDtor( TMRDRV_tzTIMER *ptzTimer )
{
    tzTIMER_INT_REG *ptzTimerIntReg;

    assert( ptzTimer );

    //stop timer
    ( void )TMRDRV_fnStop( ptzTimer );

    // make a local copy of the timer's interrupt register pointers
    ptzTimerIntReg = &( zTimers[ ptzTimer->ucTimerID ].zTimerIntRegs );

    // disable interrupt in IER
    IER &= ~(TimerImrBit[ ptzTimer->ucTimerID ]);

    // disable PIE controller interrupts
    *( ptzTimerIntReg->puiPIEMask ) &= ~(TimerPieIerBits[ ptzTimer->ucTimerID ]);

    // disable EV interrupts for this timer
    ( void )TMRDRV_fnDisInt( ptzTimer ); 
    
    //tmrdrv_ID checked already
    zTimers[ ptzTimer->ucTimerID ].uiTimerUsed   = TMRDRV_NOT_USED; 
   
    return( eSTATUS_OK );
   
}//end of TMRDRV_fnDtor
#endif   //TMRDRV_DTOR_EN

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnStart
  
PURPOSE: 
    start a timer specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that needs to be started

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-1-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure
        
Version: 1.20  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.21  Date: FEB-17-2005  By: Horst Maurer
    - added assert of input parameter

*******************************************************************************/
#if( TMR_FN_START_EN == TRUE )                      //Function compile enable
tuiSTATUS TMRDRV_fnStart( TMRDRV_tzTIMER *ptzTimer)
{
    assert( ptzTimer );

    //update timer control registers
    //set timer period return if error happens
    ( void )TMRDRV_fnSet( ptzTimer );
    ( void )TMRDRV_fnSetPrd( ptzTimer );

#if( TMR_FN_SET_COMP_EN == TRUE  )
#if( TMRDRV_COMPARE_DUTY == FALSE )
    //set compare registers return if error happens
    ( void )TMRDRV_fnSetCmp( ptzTimer );
#endif   //TMRDRV_COMPARE_DUTY
#endif  //TMRDRV_COMPARE_EN

    //set timer enable bit in TCON
    *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTcon ) |= 
                                      TimerEnable[ ptzTimer->ucTimerID ];

    return( eSTATUS_OK );
    
}//end of TMRDRV_fnStart
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnStop
  
PURPOSE: 
    stop a timer specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that needs to be stopped

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-1-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure
    
Version: 1.20  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.21  Date: FEB-17-2005  By: Horst Maurer
    - added assert of input parameter

*******************************************************************************/
#if( TMR_FN_STOP_EN == TRUE )                   //Function compile enable
tuiSTATUS TMRDRV_fnStop( TMRDRV_tzTIMER *ptzTimer )
{
    assert( ptzTimer );

    //
    ( void )TMRDRV_fnHold( ptzTimer );

    //clear counter
    *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTcnt ) = 0;

    return( eSTATUS_OK );

}//end of TMRDRV_fnStop
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnHold
  
PURPOSE: 
    Hold a timer specified. Hold a timer means stop it but not clear counter
    register
    same as stop

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that needs to be suspended

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-4-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.20  Date: May, 26, 2003  By: Yingran Duan
    - check input
    
Version: 1.30  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

*******************************************************************************/
#if( TMR_FN_HOLD_EN == TRUE )                       //Function compile enable
tuiSTATUS TMRDRV_fnHold( TMRDRV_tzTIMER *ptzTimer)
{
    
    //check input
    assert( ptzTimer );
    
    //check timer_ID
    assert( ( ptzTimer->ucTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->ucTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
    
    //clear timer enable bit (TENABLE in TxCON)
    *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTcon ) &=
                ~( TimerEnable[ ptzTimer->ucTimerID ] );

    return( eSTATUS_OK );
}//end of TMRDRV_fnHold
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnResume
  
PURPOSE: 
    restart timer from hold, don't clear counter, update register and period
    compare.

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that needs to be restarted

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-13-2002  By: Yingran Duan
    - create
    
Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.20  Date: May, 26, 2003  By: Yingran Duan
    - check input

Version: 1.30  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

*******************************************************************************/
#if( TMR_FN_RESUME_EN == TRUE )                   //Function compile enable
tuiSTATUS TMRDRV_fnResume( TMRDRV_tzTIMER *ptzTimer )
{
    
    //CHECK INPUT
    assert( ptzTimer );
    
    //check timer_ID
    assert( ( ptzTimer->ucTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->ucTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
    
    //enable enable bit
    *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTcon ) |=
                ( TimerEnable[ ptzTimer->ucTimerID ] );

    return( eSTATUS_OK );

}//end of TMRDRV_fnResume
#endif   //TMRDRV_HOLD_RESUME_EN

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnSet
  
PURPOSE: 
    initial control register of a timer specified

INPUTS: 
    a pointer to a tzTIMER structure that needs to be initialized

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10    Date: Nov-1-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.20  Date: May, 23, 2003  By: Yingran Duan
    - add assert on input
    - assert to replace tuiStatus
    
Version: 1.30  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

*******************************************************************************/
#if( TMR_FN_SET_EN == TRUE )
tuiSTATUS TMRDRV_fnSet( TMRDRV_tzTIMER *ptzTimer )
{

    //check input
    assert( ptzTimer );
    
    //check timer_ID
    assert( ( ptzTimer->ucTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->ucTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
    
    //clear counter
    *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTcnt ) = 0;
    
    //set control registers
    return( tmrdrv_fnInitTimerReg( ptzTimer) );
    
}//end of TMRDRV_fnSet
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnSetPrd
  
PURPOSE: 
    set period register of a timer specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that needs it's period set

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10    Date: Nov-4-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.20  Date: May, 23, 2003  By: Yingran Duan
    - add assert on input
    
Version: 1.30  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.40  Date: FEB-17-2005  By: Horst Maurer
    - changed function name
    - optimized code
    - code review changes

*******************************************************************************/
#if( TMR_FN_SET_PRD_EN == TRUE )                    //Function compile enable
tuiSTATUS TMRDRV_fnSetPrd( TMRDRV_tzTIMER *ptzTimer)
{
    TMRDRV_tzPRD tzTimerPrd;

    //check input
    assert( ptzTimer );
    
    //get period setting
    tzTimerPrd.ulFrequencyHz = ptzTimer->ulTimerFreq;
    
    //frequency has to be doubled if in continous up and down mode
    if( ptzTimer->teTimerCountMode == TMRDRV_eCONTUPDOWN )
    {
        tzTimerPrd.ulFrequencyHz <<= TMRDRV_DOUBLE_FREQ;
    }   
    
    //call fast period update function
    if( tmrdrv_fnCalcTimerParams( &tzTimerPrd ) == eSTATUS_OK )
    {
        return( TMRDRV_fnSetPrdFast( ptzTimer, &tzTimerPrd ));
    }
    else
    {
        return( eSTATUS_ERR );
    }

}//end of TMRDRV_fnSetPrd
#endif


/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnSetPrdFast
  
PURPOSE: 
    set period register of a timer specified with input period values

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that needs it's period to be set
    a pointer to a TMRDRV_tzPRD structure that takes period values

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10    Date: Dec-16-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.20  Date: May, 23, 2003  By: Yingran Duan
    - add assert on input
    
Version: 1.30  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

*******************************************************************************/
#if( TMR_FN_SET_PRD_FAST_EN == TRUE )           //Function compile enable
tuiSTATUS TMRDRV_fnSetPrdFast( TMRDRV_tzTIMER *ptzTimer, 
                               TMRDRV_tzPRD *ptzTimerPrd )
{
    //check input
    assert( ptzTimer );
    assert( ptzTimerPrd );
    
    //check timer_ID
    assert( ( ptzTimer->ucTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->ucTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
 
    //set period
    *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTprd ) = 
                        ptzTimerPrd->uiPeriod;   
    
    //set prescaler
    *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTcon ) = 
                        *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTcon ) &
                        ~( TimerPrescalerMsk[ ptzTimer->ucTimerID ] ) |
                        ( ptzTimerPrd->uiPrescaler << 
                        TimerPrescalerSft[ ptzTimer->ucTimerID ] );                  
    zTimers[ ptzTimer->ucTimerID ].uiTimerPrescaler = ptzTimerPrd->uiPrescaler;
    return( eSTATUS_OK );
    
}//end of TMRDRV_fnSetPrdFast
#endif

/*******************************************************************************
FUNCTION NAME: 
    TMRDRV_fnSetCmp
  
PURPOSE: 
    set compare register of a timer specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that needs to set compare register

OUTPUTS:
    tuiSTATUS

NOTES:
    if further reduction on operation time the this function is needed
        *&>> may need to replace / operation.

VERSION HISTORY:

Version: 0.10   Date: Nov-4-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.20  Date: May, 23, 2003  By: Yingran Duan
    - add assert on input
    
Version: 1.30  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.40  Date: FEB-17-2005  By: Horst Maurer
    - changed function name
    - code review changes

*******************************************************************************/
#if( TMR_FN_SET_COMP_EN == TRUE  )           //Function compile enable
#if( TMRDRV_COMPARE_PER == TRUE ) 
tuiSTATUS TMRDRV_fnSetCmp( TMRDRV_tzTIMER *ptzTimer)
{
    //check input
    assert( ptzTimer );
    
    //check timer_ID
    assert( ( ptzTimer->ucTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->ucTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
    
    
    //update compare register
    //Multiply first to save resolution.  Divide last(shift).
    *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTcmpr ) = 
        ( ( uint32 ) *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTprd ) ) *
        ( ptzTimer->uiTimerDutyPct ) / TMRDRV_INTEGER_GAIN; 
            

    return( eSTATUS_OK );
    
}//end of TMRDRV_fnSetCmp
#endif //TMRDRV_COMPARE_DUTY
#endif //Function compile enable


/*******************************************************************************
FUNCTION NAME: 
    TMRDRV_fnSetDelay
  
PURPOSE: 
    set compare register of a timer specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that needs to set compare register
    uiTimerDelay set compare register value

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10   Date: Dec-2-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.20  Date: May, 26, 2003  By: Yingran Duan
    - add assert on input
    
Version: 1.30  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

*******************************************************************************/
#if( TMR_FN_SET_COMP_EN == TRUE )              //Function compile enable
#if( TMRDRV_COMPARE_DUTY  == TRUE )
tuiSTATUS TMRDRV_fnSetDelay( TMRDRV_tzTIMER *ptzTimer, 
                             uint16 uiTimeDelay)
{
    
    //check input
    assert( ptzTimer );

    //check timer_ID
    assert( ( ptzTimer->ucTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->ucTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
    
    //update compare register
    *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTcmpr ) = uiTimeDelay;
                        
    return( eSTATUS_OK );
    
}//end of TMRDRV_fnSetDelay
#endif //TMRDRV_COMPARE_DUTY
#endif  //TMRDRV_COMPARE_EN

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnGetStatus
  
PURPOSE: 
    get status of a specific timer

INPUTS: 
    pointer to structure if the timer

OUTPUTS:
    tuiSTATUS    

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-4-2002  By: Yingran Duan
    - new
       
Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure
    
Version: 1.20  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.21  Date: FEB-17-2005  By: Horst Maurer
    - added assert of input parameter

*******************************************************************************/
#if( TMR_FN_GET_STATUS_EN == TRUE )                 //Function compile enable
tuiSTATUS TMRDRV_fnGetStatus( TMRDRV_tzTIMER *ptzTimer )
{
    tuiSTATUS tuiStatus = eSTATUS_OK;

    assert( ptzTimer );

    ( void )tmrdrv_fnGetId( ptzTimer ); 
    
    //if timer is not used, this timer instance is not created yet
    //return with error
    if( zTimers[ ptzTimer->ucTimerID ].uiTimerUsed == TMRDRV_NOT_USED )
    {
        mWRITE_STATUS_AND_ERRNO( TMRDRV_eERRNO_TIMER_NOT_ASSIGNED );
    }
            
    return( tuiStatus );

}//end of TMRDRV_fnGetStatus
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnGetPrd
  
PURPOSE: 
    set period register of a timer specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that need to get period
    a pointer to a TMRDRV_tzPRD structure that need to set with the period

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10    Date: DEC-2-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.20  Date: May, 23, 2003  By: Yingran Duan
    - add assert on input
    
Version: 1.30  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.40  Date: FEB-17-2005  By: Horst Maurer
    - changed function name
    - code review changes

*******************************************************************************/
#if( TMR_FN_GET_PRD_EN == TRUE )            //Function compile enable
tuiSTATUS TMRDRV_fnGetPrd( TMRDRV_tzTIMER *ptzTimer, 
                              TMRDRV_tzPRD *ptzTimerPrd)
{

    //check input
    assert( ptzTimer );
    assert( ptzTimerPrd );
        
    //check timer_ID
    assert( ( ptzTimer->ucTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->ucTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );

    // update *ptzTimerPrd
    ptzTimerPrd->uiPeriod = 
                        *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTprd );
    ptzTimerPrd->uiPrescaler = zTimers[ ptzTimer->ucTimerID ].uiTimerPrescaler;
    
    return( eSTATUS_OK );
    
}//end of TMRDRV_fnGetPrd
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnEnInt
  
PURPOSE: 
    enable a timer interrupt specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that interrupt need to be enable

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-1-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.20  Date: May, 26, 2003  By: Yingran Duan
    - call clear flag first
    - update uiTimerInterrupt at the same timer
    - use const array to compile various interrupt cases
    
Version: 1.30  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.40  Date: JAN-19-2005  By: Horst Maurer
    - moved IER enabling to constructor

Version: 1.41  Date: FEB-17-2005  By: Horst Maurer
    - added assert of input parameter

*******************************************************************************/
#if( TMR_FN_EN_INT_EN == TRUE )                 //Function compile enable
tuiSTATUS TMRDRV_fnEnInt( TMRDRV_tzTIMER *ptzTimer)
{
    tzTIMER_INT_REG *ptzTimerIntReg;

    assert( ptzTimer );

    //clear interrupt flag
    ( void )TMRDRV_fnClrIntFlag( ptzTimer ); 
        
    // make a local copy of the timer's interrupt register pointers
    ptzTimerIntReg = &( zTimers[ ptzTimer->ucTimerID ].zTimerIntRegs );

    //check teTimerInt define
    assert( ptzTimer->teTimerInt < TMRDRV_INT_TOTAL );

    //enable the interrupt in the EV Interrupt Mask Register
    *( ptzTimerIntReg->puiMask ) |= ( TimerIntBit[ ptzTimer->teTimerInt ]
                                    << ptzTimerIntReg->uiShift );
    //update uiTimerInterrupt
    ptzTimer->uiTimerInterrupt |= TimerIntStBit[ ptzTimer->teTimerInt ];                                

    return( eSTATUS_OK );

}//end of TMRDRV_fnEnInt
#endif


/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnDisInt
  
PURPOSE: 
    diable a timer interrupt specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that need to be disabled

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-1-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.20  Date: May, 26, 2003  By: Yingran Duan
    - call clear flag first
    - update uiTimerInterrupt at the same timer
    - use const array to compile various interrupt cases
    
Version: 1.30  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.31  Date: FEB-17-2005  By: Horst Maurer
    - added assert of input parameter

*******************************************************************************/
#if( TMR_FN_DIS_INT_EN == TRUE )                    //Function compile enable
tuiSTATUS TMRDRV_fnDisInt( TMRDRV_tzTIMER *ptzTimer)
{
    tzTIMER_INT_REG *ptzTimerIntReg;

    assert( ptzTimer );

    //clear interrupt flag
    ( void )TMRDRV_fnClrIntFlag( ptzTimer );

    // make a local copy of the timer's interrupt register pointers
    ptzTimerIntReg = &( zTimers[ ptzTimer->ucTimerID ].zTimerIntRegs );

    //check teTimerInt define
    assert( ptzTimer->teTimerInt < TMRDRV_INT_TOTAL );

    //disable the interrupt
    *( ptzTimerIntReg->puiMask ) &= ~( TimerIntBit[ ptzTimer->teTimerInt ]
                                    << ptzTimerIntReg->uiShift );
    //update uiTimerInterrupt
    ptzTimer->uiTimerInterrupt &= ~TimerIntStBit[ ptzTimer->teTimerInt ];                               
    
    return( eSTATUS_OK );

}//end of TMRDRV_fnDisInt
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnDisIntAll
  
PURPOSE: 
   disable all event manager interrupt 

INPUTS: 
    none

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-7-2002  By: Yingran Duan
    - new
    
Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Removed variable tuiStatus

Version: 1.10  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

*******************************************************************************/
#if( TMR_FN_DIS_INT_ALL_EN == TRUE )            //Function compile enable
void TMRDRV_fnDisIntAll( void )
{
    mTMRDRV_CLR_INT_EN_ALL();
}//end of TMRDRV_fnDisIntAll
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnClrIntFlag
    
PURPOSE: 
    clear a specified timer interrupt flag

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that flag need to be cleared

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-1-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure

Version: 1.20  Date: May, 26, 2003  By: Yingran Duan
    - check input
    - use const array to compile various interrupt cases
    
Version: 1.30  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

*******************************************************************************/
#if( TMR_FN_CLR_INT_FLAG_EN == TRUE )              //Function compile enable
tuiSTATUS TMRDRV_fnClrIntFlag( TMRDRV_tzTIMER *ptzTimer)
{
    tzTIMER_INT_REG *ptzTimerIntReg;

    //check input
    assert( ptzTimer );
        
    //check timer_ID
    assert( ( ptzTimer->ucTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->ucTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );

    ptzTimerIntReg = &( zTimers[ ptzTimer->ucTimerID ].zTimerIntRegs );

    //check teTimerInt define
    assert( ptzTimer->teTimerInt <= TMRDRV_INT_TOTAL );

    //clear flag 
    *( ptzTimerIntReg->puiFlag ) |= ( TimerIntBit[ ptzTimer->teTimerInt ]
                                    << ptzTimerIntReg->uiShift );

    return( eSTATUS_OK );
    
}//end of TMRDRV_fnClrIntFlag
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnClrIntFlagALL
  
PURPOSE: 
    clear all event manager interrupt flags

INPUTS: 
    none

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-4-2002  By: Yingran Duan
    - new
    
Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Removed variable tuiStatus

Version: 1.10  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

*******************************************************************************/
#if( TMR_FN_CLR_INT_FLAG_ALL_EN == TRUE )           //Function compile enable
void TMRDRV_fnClrIntFlagAll( void )
{
    mTMRDRV_CLR_INT_FLAG_ALL();
}
#endif
    
/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnReset
  
PURPOSE: 
    reset timer register to its default value

INPUTS: 
    none

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-4-2002  By: Yingran Duan
    - new
    
Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Removed variable tuiStatus

Version: 1.10  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

*******************************************************************************/
#if( TMR_FN_INIT_PKG_EN == TRUE )           //Function compile enable
tuiSTATUS TMRDRV_fnReset( void )
{
    TMRDRV_tzTIMER tzTimer = TMRDRV_DFLT;
    uint16 uiCounter;
    
    //clear compare registers
    mTMRDRV_CLR_CMPR_ALL();
    
    //clear period registers
    mTMRDRV_CLR_PRD_ALL();
    
    //disable all interrupt
    TMRDRV_fnDisIntAll();

    //clear all interrupt flag    
    TMRDRV_fnClrIntFlagAll();

    for( uiCounter = eTIMER1; uiCounter < eTIMER_TOTAL; uiCounter++ )
    {
        //init timer1~timer4 to default 
        tzTimer.ucTimerID = uiCounter;
        ( void )TMRDRV_fnSet( &tzTimer );
     }   
    
    return( eSTATUS_OK );
    
}//end of TMRDRV_fnReset
#endif    

/*******************************************************************************

FUNCTION NAME: 
    tmrdrv_fnCalcTimerParams
  
PURPOSE: 
    calculate the timer prescaler and period

INPUTS: 
    pointer to structure TMRDRV_tzPRD

OUTPUTS:
    tuiSTATUS    

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-4-2002  By: Yingran Duan
       -copied and modified from code for XDI 

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Change uiCount from uint16 to char8
    - Removed dead code.
    
Version: 1.10  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.11  Date: Jan 24, 2005  By: John Bellini
    - Swapped the call to INIT_fnGetClkSpeed() with SYSCLK_fnGetSysClk()
    
Version: 1.12  Date: FEB-17-2005  By: Horst Maurer
    - added assert of input parameter

Version: 1.20  Date: FEB-17-2005  By: Horst Maurer
    - changed function name
    - changed local var type
    - removed dead code (asserts)
    - code review changes

*******************************************************************************/
#if( TMR_FN_CALC_PARAM_EN == TRUE )             //Function compile enable
tuiSTATUS tmrdrv_fnCalcTimerParams( TMRDRV_tzPRD *ptzTimer )
{
    // TMS320F28x timer prescalers.  Each element corresponds to the exact bit
    // mask combination in the TxCON register
    const uchar8 PrescalerOption[ ]  = { 1, 2, 4, 8, 16, 32, 64, 128 };
    const uint16 PrescalerNum = sizeof( PrescalerOption ) / 
                                sizeof( PrescalerOption[ 0 ] );

    uint32 TargetClk;   // temporary storage for target clock speed
    uint32 TempPeriod;  // temporary storage for target period
    uchar8 Count = 0;

    assert( ptzTimer );

    //get CLKOUT
    TargetClk = SYSCLK_fnGetSysClk(); 
    TargetClk /= ptzTimer->ulFrequencyHz;
    TempPeriod = TargetClk;

    // Find the minimum prescaler value necessary to achieve the desired
    // frequency using the maximum count.
    // Ensure the maximum prescaler value is not exceeded.
    while(  ( TempPeriod > 0xFFFF  )
            && ( Count < PrescalerNum - 1 ) )
    {
        Count++;
        TempPeriod = TargetClk / PrescalerOption[ Count ];
    }

    // Return the prescaler mask
    ptzTimer->uiPrescaler = Count;

    // If the value exceeds the allowable period,
    // cap it and return an error.
    if( TempPeriod > TMRDRV_REG_MAX_PRD )
    {
        ptzTimer->uiPeriod = TMRDRV_REG_MAX_PRD;
        return( eSTATUS_ERR );
    }
    else if( TempPeriod < TMRDRV_REG_MIN_PRD )
    {
        ptzTimer->uiPeriod = TMRDRV_REG_MIN_PRD;
        return( eSTATUS_ERR );
    }
    else
    {
        ptzTimer->uiPeriod = ( uint16 )TempPeriod;
    }

    // Return the function status.
    return( eSTATUS_OK );
}//end of tmrdrv_fnCalcTimerParams
#endif

/*******************************************************************************

FUNCTION NAME: 
    tmrdrv_fnInitTimerReg
  
PURPOSE: 
    intial a timer Registers
INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that timer register need to initialized 
    a pointer to a tzTIMER_REGS structure that need to initialized

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-15-2002  By: Yingran Duan
    - create

Version: 1.00  Date: April 14, 2003 By: John Bellini
    - Replaced error checking with asserts where possible.
    - Removed dead code.

Version: 1.10  Date: May, 22, 2003  By: Yingran Duan
    - change on  tzTIMER_STATUS structure
    
Version: 1.20  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

Version: 1.21  Date: FEB-17-2005  By: Horst Maurer
    - added assert of input parameter

*******************************************************************************/
#if( TMR_FN_INIT_TMR_REG_EN == TRUE )           //Function compile enable
tuiSTATUS tmrdrv_fnInitTimerReg( TMRDRV_tzTIMER *ptzTimer )
{ 
    //temperary store GPTCON
    uint16 uiGptcon = *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiGptcon );  
    //temperary store TCON
    uint16 uiTcon   = *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTcon );  

    //called from fnSet only, must have input and id checked
        
    assert( ptzTimer );

    //init count mode
    //check setting validation
    assert( ptzTimer->teTimerCountMode <= TMRDRV_CNT_MODE_TOTAL );

    //set count mode
    uiTcon = uiTcon & ~F28X_TXCON_TMODE_MSK |
             TimerCntModeBit[ ptzTimer->teTimerCountMode ];

    //init ADC mode
#if( TMRDRV_ADC_EN == TRUE )
    //check setting validation
    assert( ptzTimer->teTimerAdcMode <= TMRDRV_ADC_MODE_TOTAL );

    uiGptcon = uiGptcon & ~TimerAdcModeMsk[ ptzTimer->teTimerAdcMode ] | 
               ( TimerAdcModeBits[ ptzTimer->teTimerAdcMode ] <<
               TimerAdcModeSft[ ptzTimer->teTimerAdcMode ] );
#endif  //TMRDRV_ADC_EN
    
#if( TMRDRV_COMPARE_OUT_EN == TRUE )
    //init compare output mode
    //check setting validation
    assert( ptzTimer->teTimerCompOutput <= TMRDRV_OUT_MODE_TOTAL );

    if( ptzTimer->teTimerCompOutput != TMRDRV_eNO_OUTPUT )
    {
        //turn io pin on if output is enabled
        *( zTimers[ ptzTimer->ucTimerID ].puiTmrIo ) |= 
                    TimerIoBits[ ptzTimer->ucTimerID ]; 
        *( zTimers[ ptzTimer->ucTimerID ].puiTmrMcr ) |= 
                    TimerMcrBits[ ptzTimer->ucTimerID ];

        //enable output
        uiGptcon |= F28X_GPTCON_TCOMPOE_EN;

        //set output    
        uiGptcon = uiGptcon & 
                   ~TimerOutModeMsk[ ptzTimer->teTimerCompOutput - 1 ] | 
                   ( TimerOutModeBits[ ptzTimer->teTimerCompOutput - 1 ] <<
                   TimerOutModeSft[ ptzTimer->teTimerCompOutput - 1 ] );
    }
    else
    {
        uiGptcon &= ~F28X_GPTCON_TCOMPOE_EN; 
    }
#endif  //TMRDRV_COMPARE_OUT_EN
    
#if( TMRDRV_EMU_MODE_EN == TRUE )
    //check setting validation
    assert( ptzTimer->teTimerEmu <= TMRDRV_EMU_MODE_TOTAL );

    //set emulation mode
    uiTcon = uiTcon & ~F28X_TXCON_EMMCTRL_MSK | 
             TimerEmuModeBits[ ptzTimer->teTimerEmu ];
#endif  //TMRDRV_EMU_MODE_EN

#if( TMRDRV_COMPARE_EN == TRUE )
    //timer compare reload mode initialization
    //check setting validation
    assert( ptzTimer->teTimerCompMode <= TMRDRV_COMP_MODE_TOTAL );

    // enable timer compare
    uiTcon |= F28X_TXCON_TECMPR_MSK;
    // set the compare register reload condition
    uiTcon &= ~F28X_TXCON_TCLD_MSK | 
              TimerCompModeBits[ ptzTimer->teTimerCompMode ];
#endif   //TMRDRV_COMPARE_EN

#if( TMRDRV_SYN_EN == TRUE )
    //init sync
    //check setting validation
    assert ( ptzTimer->teTimerSyn <= TMRDRV_SYN_MODE_TOTAL );
 
    //check sync timer setting
    assert( ( ptzTimer->teTimerSyn == TMRDRV_eNO_SYN ) ||
            ( ptzTimer->uiTimerIDSyn == TimerSync[ ptzTimer->ucTimerID ] ) );

    //set sync
    uiTcon = uiTcon & ~F28X_TXCON_SYN_MSK | 
             TimerSynModeBits[ ptzTimer->teTimerSyn ];  
    //set timer period
    if( ( ptzTimer->teTimerSyn == TMRDRV_eSYN_PERIOD ) ||
        ( ptzTimer->teTimerSyn == TMRDRV_eSYN_PRD_ST ) )
    {
        zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTprd = 
                 zTimers[ ptzTimer->uiTimerIDSyn ].zTimerRegs.puiTprd;
    }
#endif  //TMRDRV_SYN_EN

#if( TMRDRV_CLOCK_MODE_EN == TRUE )
    //init clock
    //check setting validation
    assert( ptzTimer->teTimerClock < TMRDRV_CLK_MODE_TOTAL );

    //timer clock source initialization
    uiTcon = uiTcon & ~F28X_TXCON_TCLKS_MSK | 
             TimerClkModeBits[ ptzTimer->teTimerClock ];
#endif //TMRDRV_CLOCK_MODE_EN
    
    //update registers
    *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiGptcon ) = uiGptcon;
    *( zTimers[ ptzTimer->ucTimerID ].zTimerRegs.puiTcon )   = uiTcon;

    return( eSTATUS_OK );
}//end of tmrdrv_fnInitTimerReg
#endif

/*******************************************************************************

FUNCTION NAME: 
    tmrdrv_fnGetId
  
PURPOSE: 
    assign timer id from function

INPUTS: 
    TMRDRV_tzTIMER

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 1.00 Date: May, 27, 2003  By: Yingran Duan
    - new
    
Version: 1.10  Date: DEC-3-2004  By: Horst Maurer
    - ported to F2812

*******************************************************************************/
tuiSTATUS tmrdrv_fnGetId( TMRDRV_tzTIMER *ptzTimer )
{
    //check input
    assert( ptzTimer );
          
    //check timer function and assign timer handler to it
    switch( ptzTimer->teTimeFunc )
    {
        //function assigned to timer1
        case( eTIMER1_FUNCTION ):
            ptzTimer->ucTimerID = eTIMER1;
            break;
            
        //function assigned to timer2
        case( eTIMER2_FUNCTION ):
            ptzTimer->ucTimerID = eTIMER2;
            break;
            
        //function assigned to timer3
        case( eTIMER3_FUNCTION ):
            ptzTimer->ucTimerID = eTIMER3;
            break;
            
        //function assigned to timer4
        case( eTIMER4_FUNCTION ):
            ptzTimer->ucTimerID = eTIMER4;
            break;
            
        default:  
            assert( 0 );
            break;
            
    }  
    
    return( eSTATUS_OK );
             
}//end of function tmrdrv_fnGetId

//end of file tmrdrv.c
