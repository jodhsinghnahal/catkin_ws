/*==============================================================================
Copyright © 2002 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  
    tmrdrv.c

PURPOSE:
    timer driver of TMS3204C2407 DSP 

FUNCTION(S):
    TMRDRV_fnCtor   - creat timer instances
    TMRDRV_fnDtor   - destroy timer instances
    TMRDRV_fnStart  - Start the timer
    TMRDRV_fnStop   - Stop the timer
    TMRDRV_fnHold   - Hold the timer
    TMRDRV_fnResume - resume the timer from hold

    TMRDRV_fnSet - Set timer control registers 
    TMRDRV_fnSetPeriod   - Set a timer period value
    TMRDRV_fnSetPrdFast  - set timer specific period registers
                        based on the input TMRDRV_tzPRD
    TMRDRV_fnSetCompare  - Update a timer compare value
    TMRDRV_fnSetDelay    - set timer compare value with delay
                                        time
    TMRDRV_fnGetStatus - Returns the status of the timer.
    TMRDRV_fnGetPeriod - return timer period

    TMRDRV_fnClrIntFlag  - clear a interrupt flag
    TMRDRV_fnEnInt       - enable a interrupt
    TMRDRV_fnDisInt      - disable a interrupt

    TMRDRV_fnClrIntFlagAll   - clear all timer interrupt flag
    TMRDRV_fnResetPackage     - reset timer registers
   
    local:
    tmrdrv_fnCalcTimerParams - calculation of timer prescaler and period register
    timer_fnInitTimerReg            - init timer registers 
    timer_fnGetId - assign id from function
NOTES:
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use Code Wright or ignore 
    **the text.
    
    none

CHANGE HISTORY:
$History: Tmrdrv.c $
    
    *****************  Version 36  *****************
    User: Yingrand     Date: 9/18/03    Time: 12:07p
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/CapDrv
    correct compile error in fnDtor PRN#361
    
    *****************  Version 35  *****************
    User: Yingrand     Date: 7/29/03    Time: 3:37p
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/TmrDrv
    package name to TMRDRV
    
    *****************  Version 34  *****************
    User: Yingrand     Date: 7/29/03    Time: 11:08a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/TmrDrv
    update enum define to meet standard
    
    *****************  Version 33  *****************
    User: Johnb        Date: 6/30/03    Time: 1:44p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/HAL/Src
    - changed BITPOS to XT_BITPOS
    
    *****************  Version 32  *****************
    User: Yingrand     Date: 6/09/03    Time: 4:51p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/SwBase/Test
    modify assert checking to speedup ISR
    
    *****************  Version 30  *****************
    User: Yingrand     Date: 5/28/03    Time: 10:02a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/TmrDrv
    
    *****************  Version 29  *****************
    User: Yingrand     Date: 5/27/03    Time: 10:35a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/TmrDrv
    updates after code review
    use constant array define to combine setting for different timer
    instance.
    use assert to replace errno operation
    remove two not needed functions, getintreg and gettimereg,
    add checks on input parameters
    
    *****************  Version 27  *****************
    User: Yingrand     Date: 5/22/03    Time: 1:02p
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/TmrDrv
    ready for code review
    
    *****************  Version 26  *****************
    User: Johnb        Date: 4/14/03    Time: 11:39a
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/RV_Series/RS2000/C24X/HAL/Src
    Enabled fnGetPrd() and fnGetStatus()
    
    *****************  Version 25  *****************
    User: Johnb        Date: 4/14/03    Time: 11:22a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/TmrDrv
    The following changes were due to the need to put this package on a
    "diet" since it was larger in size than it needed to be:
    Updated function names from timer_ to TIMER_ for package prefixing.
    Updated to coding standard where possible.
    Added function compile enables.
    Updated TIMER_fnDisIntAll() and TIMER_fnClrIntAll() to return void.
    Updated timer_fnGetTimerReg() to return void instead of tuiStatus.
    Throughout this driver, error checking has been updated to use asserts
    instead of tuiStatus.
    
    *****************  Version 24  *****************
    User: Jasonc       Date: 2/24/03    Time: 3:47p
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Src
    Updated to reflect change in Syslvl function name change

Date:   Oct.31, 2002   By: Yingran Duan
    -   create
        Nov. 25, 2002  By: Yingran Duan
    -   optimize code size with compile optimization
        before optimization 5.7K, after optimization with all the compile option
        0-1.85K
        Dec. 2, 2002    By: Yingran Duan
    -   add two functions of get period register value and set compare register
        based on delay time from captured counter value
Date:   Dec. 16, 2002   By: Yingran Duan
    -   add function of fnSetPrdFast to update period register quickly without
        calculate prescaler and period value based on frequecy. The prescaler
        and preiod value is user defined.
    -   made function tmrdrv_fnCalcTimerParams public for user to use to 
        calcualate prescaler and period.
Date:   Jan. 13, 2003   By: Yingran Duan
    -   change from tmr.c to tmrdrv.c       
==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "Tmrdrv.h"
#include "LIB_stddefs.h"
#include "LF2407A.h"            //register definitions
#include "lf2407macro.h"        //macro to enable interrupt
#include "SysLvl.h"
#include "xassert.h"
#include <errno.h>

/*==============================================================================
                             Defines
==============================================================================*/
//------------------------- FEATURE OPTION ENABLES -----------------------------
#define TMRDRV_COMPARE_EN     ( FALSE )  //no compare feature
#define TMRDRV_COMPARE_OUT_EN ( FALSE )  //no compare output
#define TMRDRV_EMU_MODE_EN    ( FALSE )  //emu set to immediate stop
#define TMRDRV_SYN_EN         ( FALSE )  //no synchronize feature
#define TMRDRV_CLOCK_MODE_EN  ( FALSE )  //clock source always internal
#define TMRDRV_ADC_EN         ( FALSE )  //no ADC mode feature
                                
/*------------------------ Function Enables ----------------------------------*/
                                    //---------------- Public ------------------
#define TMR_FN_CTOR_EN          ( TRUE  )//Include code for constructor
#define TMR_FN_SET_EN           ( TRUE  )//Include code for 

#define TMR_FN_SET_PRD_EN       ( TRUE  )//Include code for set period
#define TMR_FN_SET_PRD_FAST_EN  ( TRUE  )//Include code for set period fast
#define TMR_FN_GET_PRD_EN       ( TRUE  )//Include code for get period

#define TMR_FN_SET_COMP_EN      ( FALSE )//Exclude code for set compare
#define TMRDRV_COMPARE_DUTY      ( FALSE )//compare register define by delay
#define TMRDRV_COMPARE_PER       ( FALSE )//compare reg define with duty

#define TMR_FN_START_EN         ( TRUE  )//Include code for start
#ifdef GLBL_DTOR_EN 
#define TMR_FN_STOP_EN          ( TRUE )//Exclude code for stop
#else
#define TMR_FN_STOP_EN          ( FALSE )//Exclude code for stop
#endif

#define TMR_FN_DIS_INT_EN       ( TRUE  )//Include code for interrupt disable
#define TMR_FN_EN_INT_EN        ( TRUE  )//Include code for interrupt enable
#define TMR_FN_CLR_INT_FLAG_EN  ( TRUE  )//Include code for interrupt cleraing

#define TMR_FN_GET_STATUS_EN    ( TRUE  )//Include code for getting the status

#ifdef GLBL_DTOR_EN 
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

#define TMRDRV_INT_TOTAL         ( 5 )   //total valid interrupt cases
#define TMRDRV_CNT_MODE_TOTAL    ( 4 )   //total valid count mode
#define TMRDRV_ADC_MODE_TOTAL    ( 4 )   //total valid adc mode
#define TMRDRV_CLK_MODE_TOTAL    ( 4 )   //total valid clock mode    
#define TMRDRV_OUT_MODE_TOTAL    ( 5 )   //total valid compare output mode
#define TMRDRV_COMP_MODE_TOTAL   ( 4 )   //total valid compare mode
#define TMRDRV_EMU_MODE_TOTAL    ( 3 )   //total valid emulation mode
#define TMRDRV_SYN_MODE_TOTAL    ( 4 )   //total valid synchronization mode

//define for bit operation
#define TOFINT          ( XT_BITPOS3 )
#define TUFINT          ( XT_BITPOS2 )
#define TCINT           ( XT_BITPOS1 )
#define TPINT           ( XT_BITPOS0 )
#define T_INT_NONE      ( 0 ) 
#define TPINT_BIT       ( 1 << TPINT  )
#define TCINT_BIT       ( 1 << TCINT  )
#define TOFINT_BIT      ( 1 << TOFINT )
#define TUFINT_BIT      ( 1 << TUFINT )

#define GPTCON_TXTOADC_DISAB    ( 0x0000 )
#define GPTCON_TXTOADC_COMP     ( 0x0003 )
#define GPTCON_TXTOADC_PRD      ( 0x0002 )
#define GPTCON_TXTOADC_UNDER    ( 0x0001 )

#define GPTCON_TCOMPOE_EN       ( 0x0040 )
#define GPTCON_TXPIN_DISAB      ( 0x0000 )  
#define GPTCON_TXPIN_FRC_LOW    ( 0x0000 )  
#define GPTCON_TXPIN_FRC_HI     ( 0x0003 )  
#define GPTCON_TXPIN_ACT_LOW    ( 0x0001 )  
#define GPTCON_TXPIN_ACT_HI     ( 0x0002 )

#define TXCON_NO_COMP           ( 0x0000 )
#define TXCON_ZERO_RELOAD       ( 0x0002 )
#define TXCON_EQUAL_RELOAD      ( 0x0006 )
#define TXCON_IMM_RELOAD        ( 0x000A )

#define TXCON_TCLK_INT          ( 0x0000 )
#define TXCON_TCLK_EXT          ( 0x0010 )
    
#define TXCON_SYN_MSK           ( 0x0081 )
#define TXCON_SYN_SEP           ( 0x0000 )
#define TXCON_SYN_PRD           ( 0x0001 )
#define TXCON_SYN_START         ( 0x0080 )
#define TXCON_SYN_PRDST         ( 0x0081 )

//define for general operation
#define TMRDRV_DOUBLE_FREQ   ( 1 )           //shift left by 1 bit
#define TMRDRV_INTEGER_GAIN  ( 10000 )       //100%

#define TMRDRV_REG_MAX_PRD   ( 0xFFFF )      //prd register maxmum value
#define TMRDRV_REG_MIN_PRD   ( 5 )           //prd register minimum value

//macro define
#define mTMRDRV_CLR_INT_FLAG_ALL()  *EVAIFRA = TMRDRV_INT_FLAG_CLEAR_ALL;    \
                                    *EVAIFRB = TMRDRV_INT_FLAG_CLEAR_ALL;    \
                                    *EVAIFRC = TMRDRV_INT_FLAG_CLEAR_ALL;    \
                                    *EVBIFRA = TMRDRV_INT_FLAG_CLEAR_ALL;    \
                                    *EVBIFRB = TMRDRV_INT_FLAG_CLEAR_ALL;    \
                                    *EVBIFRC = TMRDRV_INT_FLAG_CLEAR_ALL
        
#define mTMRDRV_CLR_INT_EN_ALL()    *EVAIMRA = TMRDRV_INT_DISABLE_ALL;   \
                                    *EVAIMRB = TMRDRV_INT_DISABLE_ALL;   \
                                    *EVAIMRC = TMRDRV_INT_DISABLE_ALL;   \
                                    *EVBIMRA = TMRDRV_INT_DISABLE_ALL;   \
                                    *EVBIMRB = TMRDRV_INT_DISABLE_ALL;   \
                                    *EVBIMRC = TMRDRV_INT_DISABLE_ALL
                                    
#define mTMRDRV_CLR_CMPR_ALL()      *T1CMPR = TMRDRV_COMPARE_DFLT;   \
                                    *T2CMPR = TMRDRV_COMPARE_DFLT;   \
                                    *T3CMPR = TMRDRV_COMPARE_DFLT;   \
                                    *T4CMPR = TMRDRV_COMPARE_DFLT

#define mTMRDRV_CLR_PRD_ALL()       *T1PR = TMRDRV_PERIOD_DFLT;  \
                                    *T2PR = TMRDRV_PERIOD_DFLT;  \
                                    *T3PR = TMRDRV_PERIOD_DFLT;  \
                                    *T4PR = TMRDRV_PERIOD_DFLT

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
    eTIMER2_FUNCTION = TMRDRV_eRTOS,
    eTIMER3_FUNCTION = TMRDRV_eSW,
    eTIMER4_FUNCTION = TMRDRV_eGP
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
    volatile uint16 *puiMask;   //point to timer interrupt mask registe
    uint16 uiShift;
}tzTIMER_INT_REG;

//timer status structure
typedef struct zTIMER_STATUS
{
    uint16 uiTimerUsed;         //1-this timer is used, 0-available
    uint16 uiTimerPrescaler;    //mirror the timer prescaler value
    volatile uint16 *puiTmrIo;  //points to timer associate io register
    volatile uint16 *puiTmrMcr; //points to timer io mcr register
    tzTIMER_REGS zTimerRegs;
    tzTIMER_INT_REG zTimerIntRegs;
}tzTIMER_STATUS;

//timer variables to store usage of each timer
static tzTIMER_STATUS zTimers[ eTIMER_TOTAL ] = 
{ 
    TMRDRV_NOT_USED, 0, PBDATDIR, MCRA, 
        GPTCONA, T1CON, T1CNT, T1PR, T1CMPR, 
        EVAIFRA, EVAIMRA, XT_BITPOS7,                  //timer1
    TMRDRV_NOT_USED, 0, PBDATDIR, MCRA, 
        GPTCONA, T2CON, T2CNT, T2PR, T2CMPR,
        EVAIFRB, EVAIMRB, XT_BITPOS0,                  //timer2
    TMRDRV_NOT_USED, 0, PFDATDIR, MCRC, 
        GPTCONB, T3CON, T3CNT, T3PR, T3CMPR,
        EVBIFRA, EVBIMRA, XT_BITPOS7,                  //timer3
    TMRDRV_NOT_USED, 0, PFDATDIR, MCRC,      
        GPTCONB, T4CON, T4CNT, T4PR, T4CMPR, 
        EVBIFRB, EVBIMRB, XT_BITPOS0                   //timer4
};

#if( TMRDRV_SYN_EN == TRUE )
//synchronize timer
const uint16 TimerSync[ eTIMER_TOTAL ] =
{ 
    eTIMER2,    //timer1
    eTIMER1,    //timer2
    eTIMER4,    //timer3
    eTIMER3     //timer4
};
#endif

//timer enable bit 
const uint16 TimerEnable[ eTIMER_TOTAL ] =
{ 
    T1CON_TENABLE_EN,   //timer1
    T2CON_TENABLE_EN,   //timer2
    T3CON_TENABLE_EN,   //timer3
    T4CON_TENABLE_EN    //timer4
};

//imr bit for each timer
const uint16 TimerImrBit[ eTIMER_TOTAL ] =
{
    IMR_INT2_ENA,   //timer1
    IMR_INT3_ENA,   //timer2
    IMR_INT2_ENA,   //timer3
    IMR_INT3_ENA    //timer4
};   

//timer prescaler msk bit 
const uint16 TimerPrescalerMsk[ eTIMER_TOTAL ] =
{
    T1CON_TPS_MSK,  //timer1
    T2CON_TPS_MSK,  //timer2
    T3CON_TPS_MSK,  //timer3
    T4CON_TPS_MSK   //timer4
};   

//timer prescaler shift bits 
const uint16 TimerPrescalerSft[ eTIMER_TOTAL ] =
{
    T1CON_TPS0,     //timer1
    T2CON_TPS0,     //timer2
    T3CON_TPS0,     //timer3
    T4CON_TPS0      //timer4
}; 

#if( TMRDRV_COMPARE_OUT_EN == TRUE )
//timer compare output io pin function bits 
const uint16 TimerMcrBits[ eTIMER_TOTAL ] =
{
    MCRA_T1PWM_T1CMP,   //timer1
    MCRA_T2PWM_T2CMP,   //timer2
    MCRC_T3PWM_T3CMP,   //timer3
    MCRC_T4PWM_T4CMP    //timer4
}; 
#endif

#if( TMRDRV_COMPARE_OUT_EN == TRUE )
//timer compare output io pin direction bits 
const uint16 TimerIoBits[ eTIMER_TOTAL ] =
{
    PBDATDIR_B4DIR_OUTPUT,  //timer1
    PBDATDIR_B5DIR_OUTPUT,  //timer2
    PFDATDIR_F2DIR_OUTPUT,  //timer3
    PFDATDIR_F3DIR_OUTPUT   //timer4
}; 
#endif

//timer interrupt bit, same sequence with TMRDRV_teADC_MODE
const uint16 TimerIntBit[ TMRDRV_INT_TOTAL ] =
{
    T_INT_NONE,     //no interrupt
    TPINT_BIT,      //period interrupt
    TCINT_BIT,      //compare interrupt
    TOFINT_BIT,     //overflow interrupt
    TUFINT_BIT      //overflow interrupt
};  

//timer interrupt status bit, same sequence with TMRDRV_teADC_MODE
const uint16 TimerIntStBit[ TMRDRV_INT_TOTAL ] =
{
    TMRDRV_INT_NONE,     //no interrupt
    TMRDRV_INT_PRD,      //period interrupt
    TMRDRV_INT_COMP,     //compare interrupt
    TMRDRV_INT_OF,       //overflow interrupt
    TMRDRV_INT_UF        //overflow interrupt
}; 

//timer counter mode setting bit with same sequence with TMRDRV_teCOUNT_MODE
const uint16 TimerCntModeBit[ TMRDRV_CNT_MODE_TOTAL ] =
{
    TXCON_TMODE_CONTUPDOWN, //continuous up down mode
    TXCON_TMODE_CONTUP,     //continuous up mode 
    TXCON_TMODE_DIRUPDOWN,  //direction up down mode
    TXCON_TMODE_STOPHOLD    //stophold mode
};

#if( TMRDRV_ADC_EN == TRUE )
//timer adc mode mask bit 
const uint16 TimerAdcModeMsk[ eTIMER_TOTAL ]=
{
    GPTCONA_T1TOADC_MSK,    //timer1
    GPTCONA_T2TOADC_MSK,    //timer2
    GPTCONB_T3TOADC_MSK,    //timer3
    GPTCONB_T4TOADC_MSK     //timer4
};
#endif

#if( TMRDRV_ADC_EN == TRUE )
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
    GPTCON_TXTOADC_DISAB,   //separate
    GPTCON_TXTOADC_COMP,    //compare match
    GPTCON_TXTOADC_PRD,     //period match
    GPTCON_TXTOADC_UNDER    //under flow match
};
#endif

#if( TMRDRV_COMPARE_OUT_EN == TRUE )
//timer output mode mask bit 
const uint16 TimerOutModeMsk[ eTIMER_TOTAL ]=
{
    GPTCONA_T1PIN_MSK,  //timer1
    GPTCONA_T2PIN_MSK,  //timer2
    GPTCONB_T3PIN_MSK,  //timer3
    GPTCONB_T4PIN_MSK   //timer4
};
#endif

#if( TMRDRV_COMPARE_OUT_EN == TRUE )
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
    GPTCON_TXPIN_DISAB,     //no output
    GPTCON_TXPIN_FRC_LOW,   //force low
    GPTCON_TXPIN_FRC_HI,    //force high
    GPTCON_TXPIN_ACT_LOW,   //active low
    GPTCON_TXPIN_ACT_HI     //active high
};
#endif

#if( TMRDRV_CLOCK_MODE_EN == TRUE )
//timer clock mode setting bits, same sequence with TMRDRV_teCLOCK
const uint16 TimerClkModeBits[ TMRDRV_CLK_MODE_TOTAL ] =
{
    TXCON_TCLK_INT,     //internal
    TXCON_TCLK_EXT,     //external
    TXCON_TCLK_INT      //internal
};
#endif
    
#if( TMRDRV_EMU_MODE_EN == TRUE )
//timer emulation mode setting bits, same sequence with TMRDRV_teEMU
const uint16 TimerEmuModeBits[ TMRDRV_EMU_MODE_TOTAL ] =
{
    TXCON_EMMCTRL_STOPIMM,      //stop immediatly
    TXCON_EMMCTRL_NOSTOP,       //free run
    TXCON_EMMCTRL_STOPIMED      //stop after
};
#endif

#if( TMRDRV_SYN_EN == TRUE )
//timer sync mode setting bits, same sequence with TMRDRV_teSYN
const uint16 TimerSynModeBits[ TMRDRV_SYN_MODE_TOTAL ] =
{
    TXCON_SYN_SEP,      //no syn
    TXCON_SYN_PRD,      //synchronize peirod
    TXCON_SYN_START,    //synchronize start point
    TXCON_SYN_PRDST     //period and start all synchronized
};
#endif

#if( TMRDRV_COMPARE_EN == TRUE )
//timer compare mode setting bits, same sequence with TMRDRV_teCOMP_MODE
const uint16 TimerCompModeBits[ TMRDRV_COMP_MODE_TOTAL ] =
{
    TXCON_NO_COMP,          //no compare
    TXCON_ZERO_RELOAD,      //reload at zero
    TXCON_EQUAL_RELOAD,     //reload at period
    TXCON_IMM_RELOAD        //reload immediatly
};
#endif
/*==============================================================================
                       Local Function Protoypes
==============================================================================*/
//calculate timer prescaler and period
tuiSTATUS tmrdrv_fnCalcTimerParams( TMRDRV_tzPRD *ptzTimer );

//Init timer registers
tuiSTATUS tmrdrv_fnInitTimerReg( TMRDRV_tzTIMER *ptzTimer );

//assign id from function
tuiSTATUS tmrdrv_fnGetId( TMRDRV_tzTIMER *ptzTimer );
/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnCtor
  
PURPOSE: 
    create timer instances, initial timer variable uiTimerID, according to 
    function defined

INPUTS: 
    a pointer to a tzTIMER structure that need to be created

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

*******************************************************************************/
#if( TMR_FN_CTOR_EN == TRUE )                   //Function compile enable
tuiSTATUS TMRDRV_fnCtor( TMRDRV_tzTIMER *ptzTimer)
{

    //get id
    ( void )tmrdrv_fnGetId( ptzTimer);

    //check timer availibility, and init timerID if avaliable
    assert( zTimers[ ptzTimer->uiTimerID ].uiTimerUsed == TMRDRV_NOT_USED );

    zTimers[ ptzTimer->uiTimerID ].uiTimerUsed = TMRDRV_USED;
            
    return( eSTATUS_OK );

}//end of TMRDRV_fnCtor
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnDtor
  
PURPOSE: 
    destroy timer instances, stop a timer and clear the timer assignment

INPUTS: 
    a pointer to a tzTIMER structure that need to destroy

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
    
*******************************************************************************/
#ifdef  GLBL_DTOR_EN                       //Function compile enable
tuiSTATUS TMRDRV_fnDtor( TMRDRV_tzTIMER *ptzTimer )
{

    //stop timer
    ( void )TMRDRV_fnStop( ptzTimer );
    ( void )TMRDRV_fnDisInt( ptzTimer ); 
    
    //tmrdrv_ID checked already
    zTimers[ ptzTimer->uiTimerID ].uiTimerUsed   = TMRDRV_NOT_USED; 
   
    return( eSTATUS_OK );
   
}//end of TMRDRV_fnDtor
#endif   //TMRDRV_DTOR_EN

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnSet
  
PURPOSE: 
    initial control register of a timer specified

INPUTS: 
    a pointer to a tzTIMER structure that need to be initialized

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
    
*******************************************************************************/
#if( TMR_FN_SET_EN == TRUE )
tuiSTATUS TMRDRV_fnSet( TMRDRV_tzTIMER *ptzTimer )
{

    //check input
    assert( ptzTimer );
    
    //check timer_ID
    assert( ( ptzTimer->uiTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->uiTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
    
    //clear counter
    *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTcnt ) = 0;
    
    //set control registers
    return( tmrdrv_fnInitTimerReg( ptzTimer) );
    
}//end of TMRDRV_fnSet
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnSetPeriod
  
PURPOSE: 
    set period register of a timer specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that need to set period

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
    
*******************************************************************************/
#if( TMR_FN_SET_PRD_EN == TRUE )                    //Function compile enable
tuiSTATUS TMRDRV_fnSetPeriod( TMRDRV_tzTIMER *ptzTimer)
{
    TMRDRV_tzPRD tzTimerPrd;

    //check input
    assert( ptzTimer );
    
    //get period setting
    tzTimerPrd.ulFrequencyHz = ptzTimer->ulTimerPeriod;
    
    //frequency has to be doubled if in continous up and down mode
    if( ptzTimer->teTimerCountMode == TMRDRV_eCONTUPDOWN )
    {
        tzTimerPrd.ulFrequencyHz <<= TMRDRV_DOUBLE_FREQ;
    }   
    
    //calculate prescaler and period value
    ( void )tmrdrv_fnCalcTimerParams( &tzTimerPrd );
    
    //call fast period update function
    return( TMRDRV_fnSetPrdFast( ptzTimer, &tzTimerPrd ));

}//end of TMRDRV_fnSetPeriod
#endif


/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnSetPrdFast
  
PURPOSE: 
    set period register of a timer specified with input period values

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that need to set period
    a pointer to a TMRDRV_tzPRD structure that take period values

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
    
*******************************************************************************/
#if( TMR_FN_SET_PRD_FAST_EN == TRUE )           //Function compile enable
tuiSTATUS TMRDRV_fnSetPrdFast( TMRDRV_tzTIMER *ptzTimer, 
                               TMRDRV_tzPRD *ptzTimerPrd )
{
    //check input
    assert( ptzTimer );
    assert( ptzTimerPrd );
    
    //check timer_ID
    assert( ( ptzTimer->uiTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->uiTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
 
    //set period
    *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTprd ) = 
                        ptzTimerPrd->uiPeriod;   
    
    //set prescaler
    *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTcon ) = 
                        *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTcon ) &
                        ~( TimerPrescalerMsk[ ptzTimer->uiTimerID ] ) |
                        ( ptzTimerPrd->uiPrescaler << 
                        TimerPrescalerSft[ ptzTimer->uiTimerID ] );                  
    zTimers[ ptzTimer->uiTimerID ].uiTimerPrescaler = ptzTimerPrd->uiPrescaler;
    return( eSTATUS_OK );
    
}//end of TMRDRV_fnSetPrdFast
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnGetPeriod
  
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
    
*******************************************************************************/
#if( TMR_FN_GET_PRD_EN == TRUE )            //Function compile enable
tuiSTATUS TMRDRV_fnGetPeriod( TMRDRV_tzTIMER *ptzTimer, 
                              TMRDRV_tzPRD *ptzTimerPrd)
{

    //check input
    assert( ptzTimer );
    assert( ptzTimerPrd );
        
    //check timer_ID
    assert( ( ptzTimer->uiTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->uiTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );

    // update *ptzTimerPrd
    ptzTimerPrd->uiPeriod = 
                        *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTprd );
    ptzTimerPrd->uiPrescaler = zTimers[ ptzTimer->uiTimerID ].uiTimerPrescaler;
    
    return( eSTATUS_OK );
    
}//end of TMRDRV_fnGetPeriod
#endif


/*******************************************************************************
FUNCTION NAME: 
    TMRDRV_fnSetCompare
  
PURPOSE: 
    set compare register of a timer specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that need to set compare register

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
    
*******************************************************************************/
#if( TMR_FN_SET_COMP_EN == TRUE  )           //Function compile enable
#if( TMRDRV_COMPARE_PER == TRUE ) 
tuiSTATUS TMRDRV_fnSetCompare( TMRDRV_tzTIMER *ptzTimer)
{
    //check input
    assert( ptzTimer );
    
    //check timer_ID
    assert( ( ptzTimer->uiTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->uiTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
    
    
    //update compare register
    //Multiply first to save resolution.  Divide last(shift).
    *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTcmpr ) = 
        ( ( uint32 ) *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTprd ) ) *
        ( ptzTimer->uiTimerDuty ) / TMRDRV_INTEGER_GAIN; 
            

    return( eSTATUS_OK );
    
}//end of TMRDRV_fnSetCompare
#endif //TMRDRV_COMPARE_DUTY
#endif //Function compile enable


/*******************************************************************************
FUNCTION NAME: 
    TMRDRV_fnSetDelay
  
PURPOSE: 
    set compare register of a timer specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that need to set compare register
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
    
*******************************************************************************/
#if( TMR_FN_SET_COMP_EN == TRUE )              //Function compile enable
#if( TMRDRV_COMPARE_DUTY  == TRUE )
tuiSTATUS TMRDRV_fnSetDelay( TMRDRV_tzTIMER *ptzTimer, 
                             uint16 uiTimeDelay)
{
    
    //check input
    assert( ptzTimer );

    //check timer_ID
    assert( ( ptzTimer->uiTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->uiTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
    
    //update compare register
    *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTcmpr ) = uiTimeDelay;
                        
    return( eSTATUS_OK );
    
}//end of TMRDRV_fnSetDelay
#endif //TMRDRV_COMPARE_DUTY
#endif  //TMRDRV_COMPARE_EN

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnStart
  
PURPOSE: 
    start a timer specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that need to be start

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
        
*******************************************************************************/
#if( TMR_FN_START_EN == TRUE )                      //Function compile enable
tuiSTATUS TMRDRV_fnStart( TMRDRV_tzTIMER *ptzTimer)
{
    //update timer control registers
    //set timer period return if error happens
    ( void )TMRDRV_fnSet( ptzTimer );
    ( void )TMRDRV_fnSetPeriod( ptzTimer );

#if( TMR_FN_SET_COMP_EN == TRUE  )
#if( TMRDRV_COMPARE_DUTY == FALSE )
    //set compare registers return if error happens
    ( void )TMRDRV_fnSetCompare( ptzTimer );
#endif   //TMRDRV_COMPARE_DUTY
#endif  //TMRDRV_COMPARE_EN

    //set timer enable bit in TCON
    *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTcon ) |= 
                                      TimerEnable[ ptzTimer->uiTimerID ];

    return( eSTATUS_OK );
    
}//end of TMRDRV_fnStart
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
    
*******************************************************************************/
#if( TMR_FN_EN_INT_EN == TRUE )                 //Function compile enable
tuiSTATUS TMRDRV_fnEnInt( TMRDRV_tzTIMER *ptzTimer)
{
    tzTIMER_INT_REG *ptzTimerIntReg;
    
    //clear interrupt flag
    ( void )TMRDRV_fnClrIntFlag( ptzTimer ); 
        
    //enable interrupt in IMR
    *IMR |= TimerImrBit[ ptzTimer->uiTimerID ];
    
    ptzTimerIntReg = &( zTimers[ ptzTimer->uiTimerID ].zTimerIntRegs );
    
    //check teTimerInt define
    assert( ptzTimer->teTimerInt < TMRDRV_INT_TOTAL );

    //enable the interrupt
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
    
*******************************************************************************/
#if( TMR_FN_DIS_INT_EN == TRUE )                    //Function compile enable
tuiSTATUS TMRDRV_fnDisInt( TMRDRV_tzTIMER *ptzTimer)
{
    tzTIMER_INT_REG *ptzTimerIntReg;

    //clear interrupt flag
    ( void )TMRDRV_fnClrIntFlag( ptzTimer );

    ptzTimerIntReg = &( zTimers[ ptzTimer->uiTimerID ].zTimerIntRegs );
    
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
    
*******************************************************************************/
#if( TMR_FN_CLR_INT_FLAG_EN == TRUE )              //Function compile enable
tuiSTATUS TMRDRV_fnClrIntFlag( TMRDRV_tzTIMER *ptzTimer)
{
    tzTIMER_INT_REG *ptzTimerIntReg;

    //check input
    assert( ptzTimer );
        
    //check timer_ID
    assert( ( ptzTimer->uiTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->uiTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );

    ptzTimerIntReg = &( zTimers[ ptzTimer->uiTimerID ].zTimerIntRegs );

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
    TMRDRV_fnStop
  
PURPOSE: 
    stop a timer specified

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that need to be stop

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
    
*******************************************************************************/
#if( TMR_FN_STOP_EN == TRUE )                   //Function compile enable
tuiSTATUS TMRDRV_fnStop( TMRDRV_tzTIMER *ptzTimer )
{
    
    //check timer_ID
    ( void )TMRDRV_fnHold( ptzTimer );

    //clear counter
    *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTcnt ) = 0;

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
    a pointer to a TMRDRV_tzTIMER structure that need to be hold

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
    
*******************************************************************************/
#if( TMR_FN_HOLD_EN == TRUE )                       //Function compile enable
tuiSTATUS TMRDRV_fnHold( TMRDRV_tzTIMER *ptzTimer)
{
    
    //check input
    assert( ptzTimer );
    
    //check timer_ID
    assert( ( ptzTimer->uiTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->uiTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
    
    //clear enable bit
    *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTcon ) &=
                ~( TimerEnable[ ptzTimer->uiTimerID ] );

    return( eSTATUS_OK );
}//end of TMRDRV_fnHold
#endif

/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnResume
  
PURPOSE: 
    restart timer from hold, without clear counter, update register and period
    compare.

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that need to be restart

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

*******************************************************************************/
#if( TMR_FN_RESUME_EN == TRUE )                   //Function compile enable
tuiSTATUS TMRDRV_fnResume( TMRDRV_tzTIMER *ptzTimer )
{
    
    //CHECK INPUT
    assert( ptzTimer );
    
    //check timer_ID
    assert( ( ptzTimer->uiTimerID < eTIMER_TOTAL ) && 
            ( zTimers[ ptzTimer->uiTimerID ].uiTimerUsed != TMRDRV_NOT_USED ) );
    
    //enable enable bit
    *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTcon ) |=
                ( TimerEnable[ ptzTimer->uiTimerID ] );

    return( eSTATUS_OK );

}//end of TMRDRV_fnResume
#endif   //TMRDRV_HOLD_RESUME_EN

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
    
*******************************************************************************/
#if( TMR_FN_INIT_TMR_REG_EN == TRUE )           //Function compile enable
tuiSTATUS tmrdrv_fnInitTimerReg( TMRDRV_tzTIMER *ptzTimer )
{ 
    //temperary store GPTCON
    uint16 uiGptcon = *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiGptcon );  
    //temperary store TCON
    uint16 uiTcon   = *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTcon );  

    //called from fnSet only, must have input and id checked
        
    //init count mode
    //check setting validation
    assert( ptzTimer->teTimerCountMode <= TMRDRV_CNT_MODE_TOTAL );

    //set count mode
    uiTcon = uiTcon & ~TXCON_TMODE_MSK |
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
        *( zTimers[ ptzTimer->uiTimerID ].puiTmrIo ) |= 
                    TimerIoBits[ ptzTimer->uiTimerID ]; 
        *( zTimers[ ptzTimer->uiTimerID ].puiTmrMcr ) |= 
                    TimerMcrBits[ ptzTimer->uiTimerID ];

        //enable output
        uiGptcon |= GPTCON_TCOMPOE_EN;

        //set output    
        uiGptcon = uiGptcon & 
                   ~TimerOutModeMsk[ ptzTimer->teTimerCompOutput ] | 
                   ( TimerOutModeBits[ ptzTimer->teTimerCompOutput ] <<
                   TimerOutModeSft[ ptzTimer->teTimerCompOutput ] );
    }
    else
    {
        uiGptcon &= ~GPTCON_TCOMPOE_EN; 
    }
#endif  //TMRDRV_COMPARE_OUT_EN
    
#if( TMRDRV_EMU_MODE_EN == TRUE )
    //check setting validation
    assert( ptzTimer->teTimerEmu <= TMRDRV_EMU_MODE_TOTAL );

    //set emulation mode
    uiTcon = uiTcon & ~TXCON_EMMCTRL_MSK | 
             TimerEmuModeBits[ ptzTimer->teTimerEmu ];
#endif  //TMRDRV_EMU_MODE_EN

#if( TMRDRV_COMPARE_EN == TRUE )
    //timer compare reload mode initialization
    //check setting validation
    assert( ptzTimer->teTimerCompMode <= TMRDRV_COMP_MODE_TOTAL );

    uiTcon = uiTcon & ~TXCON_TCLD_MSK | 
             TimerCompModeBits[ ptzTimer->teTimerCompMode ];    
#endif   //TMRDRV_COMPARE_EN

#if( TMRDRV_SYN_EN == TRUE )
    //init sync
    //check setting validation
    assert ( ptzTimer->teTimerSyn <= TMRDRV_SYN_MODE_TOTAL );
 
    //check sync timer setting
    assert( ( ptzTimer->teTimerSyn == TMRDRV_eNO_SYN ) ||
            ( ptzTimer->uiTimerIDSyn == TimerSync[ ptzTimer->uiTimerID ] ) );

    //set sync
    uiTcon = uiTcon & ~TXCON_SYN_MSK | 
             TimerSynModeBits[ ptzTimer->teTimerSyn ];  
    //set timer period
    if( ( ptzTimer->teTimerSyn == TMRDRV_eSYN_PERIOD ) ||
        ( ptzTimer->teTimerSyn == TMRDRV_eSYN_PRD_ST ) )
    {
        zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTprd = 
                 zTimers[ ptzTimer->uiTimerIDSyn ].zTimerRegs.puiTprd;
    }
#endif  //TMRDRV_SYN_EN

#if( TMRDRV_CLOCK_MODE_EN == TRUE )
    //init clock
    //check setting validation
    assert( ptzTimer->teTimerClock < TMRDRV_CLK_MODE_TOTAL );

    //timer clock source initialization
    uiTcon = uiTcon & ~TXCON_TCLKS_MSK | 
             TimerClkModeBits[ ptzTimer->teTimerClock ];
    }
#endif //TMRDRV_CLOCK_MODE_EN
    
    //update registers
    *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiGptcon ) = uiGptcon;
    *( zTimers[ ptzTimer->uiTimerID ].zTimerRegs.puiTcon )   = uiTcon;

    return( eSTATUS_OK );
}//end of tmrdrv_fnInitTimerReg
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
    
*******************************************************************************/
#if( TMR_FN_CALC_PARAM_EN == TRUE )             //Function compile enable
tuiSTATUS tmrdrv_fnCalcTimerParams( TMRDRV_tzPRD *ptzTimer )
{
    // TMS3202407 timer prescalers.  Each element corresponds to the exact bit
    // mask combination in the TxCON register
    const uint16 PrescalerOption[ ]  = { 1, 2, 4, 8, 16, 32, 64, 128 };
    const uint16 PrescalerNum = sizeof( PrescalerOption ) / 
                                sizeof( PrescalerOption[ 0 ] );

    uint32 TargetClk;   // temporary storage for target clock speed
    uint32 TempPeriod;  // temporary storage for target period
    uchar8 Count = 0;

    //get CLKOUT
    TargetClk = SYSLVL_fnGetClkSpeed(); 
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
        assert( 0 ); 
    }
    else if( TempPeriod < TMRDRV_REG_MIN_PRD )
    {
        ptzTimer->uiPeriod = TMRDRV_REG_MIN_PRD;
        assert( 0 );
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
    
*******************************************************************************/
#if( TMR_FN_GET_STATUS_EN == TRUE )                 //Function compile enable
tuiSTATUS TMRDRV_fnGetStatus( TMRDRV_tzTIMER *ptzTimer )
{
    tuiSTATUS tuiStatus = eSTATUS_OK;

    ( void )tmrdrv_fnGetId( ptzTimer ); 
    
    //if timer is not used, this timer instance is not created yet
    //return with error
    if( zTimers[ ptzTimer->uiTimerID ].uiTimerUsed == TMRDRV_NOT_USED )
    {
        mWRITE_STATUS_AND_ERRNO( TMRDRV_eERRNO_TIMER_NOT_ASSIGNED );
    }
            
    return( tuiStatus );

}//end of TMRDRV_fnGetStatus
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

*******************************************************************************/
#if( TMR_FN_CLR_INT_FLAG_ALL_EN == TRUE )           //Function compile enable
void TMRDRV_fnClrIntFlagAll( void )
{
    mTMRDRV_CLR_INT_FLAG_ALL();
}
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

*******************************************************************************/
#if( TMR_FN_DIS_INT_ALL_EN == TRUE )            //Function compile enable
void TMRDRV_fnDisIntAll( void )
{
    mTMRDRV_CLR_INT_EN_ALL();
}//end of TMRDRV_fnDisIntAll
#endif


/*******************************************************************************

FUNCTION NAME: 
    TMRDRV_fnResetPackage
  
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

*******************************************************************************/
#if( TMR_FN_INIT_PKG_EN == TRUE )           //Function compile enable
tuiSTATUS TMRDRV_fnResetPackage( void )
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
        tzTimer.uiTimerID = uiCounter;
        ( void )TMRDRV_fnSet( &tzTimer );
     }   
    
    return( eSTATUS_OK );
    
}//end of TMRDRV_fnResetPackage
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
    
*******************************************************************************/
tuiSTATUS tmrdrv_fnGetId( TMRDRV_tzTIMER *ptzTimer )
{
    //check input
    assert( ptzTimer );
          
    //check timer function and assign timer handler to it
    switch( ptzTimer->teTimeFunc )
    {
        //function to assigned to timer1
        case( eTIMER1_FUNCTION ):
            ptzTimer->uiTimerID = eTIMER1;
            break;
            
        //function to assigned to timer2
        case( eTIMER2_FUNCTION ):
            ptzTimer->uiTimerID = eTIMER2;
            break;
            
        //function to assigned to timer3
        case( eTIMER3_FUNCTION ):
            ptzTimer->uiTimerID = eTIMER3;
            break;
            
        //function to assigned to timer4
        case( eTIMER4_FUNCTION ):
            ptzTimer->uiTimerID = eTIMER4;
            break;
            
        default:  
            assert( 0 );
            break;
            
    }  
    
    return( eSTATUS_OK );
             
}//end of function tmrdrv_fnGetId


//end of file tmrdrv.c