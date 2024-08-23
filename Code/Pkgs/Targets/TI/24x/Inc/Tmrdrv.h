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
    Tmrdrv.h

PURPOSE:
    Package header and definitions for the tmr driver.

FUNCTION(S):
    TMRDRV_fnCtor     - creat timer instances
    TMRDRV_fnDtor     - destroy timer instances
    TMRDRV_fnStart    - Start the timer
    TMRDRV_fnStop     - Stop the timer
    TMRDRV_fnHold     - Hold the timer
    TMRDRV_fnResume   - resume the timer from hold
    TMRDRV_fnSetDelay - set timer compare value with delay time

    TMRDRV_fnSet - Set timer control registers 
    TMRDRV_fnSetPeriod - Set a timer period value
    TMRDRV_fnSetPrdFast - set timer specific period registers
                        based on the input tzTMRDRV_PRD
    TMRDRV_fnSetCompare - Update a timer compare value
    TMRDRV_fnGetStatus - Returns the status of the timer.
    TMRDRV_fnGetPeriod - return timer period

    TMRDRV_fnClrIntFlag - clear a interrupt flag
    TMRDRV_fnEnInt - enable a interrupt
    TMRDRV_fnDisInt - disable a interrupt

    TMRDRV_fnClrIntFlagAll - clear all timer interrupt flag
    TMRDRV_fnResetPackage - reset timer registers
   
    TMRDRV_fnCalcTimerParams - calculation of timer prescaler and period register 
NOTES:
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use Code Wright or ignore 
    **the text.

    USAGE:
    ***set compile options for your application first to save code space.

    1. Before using most of the functions( except  fnClrIntFlagAll and 
            fnResetPackage), a timer instance has to be created. The only member
            variable within the tzTIMER structure that has to be initialized 
            before calling TMRDRV_fnCtor is teTimeFunc.
        Put a "dflt" initialization to tzTIMER structure when define a struc
            to avoid unprodictable situations.
        TMRDRV_fnCtor provides an ID to uitimerID in the tzTIMER. 

    2. Define the variable members properly: 
                        teTimerCountMode;   //counter mode
                        teTimerAdcMode;       //ADC start mode
                        teTimerCompOutput; //timer compare output mode
                        teTimerEmu;                //timer emulation mode
                        teTimerCompMode;     //timer compare mode
                        teTimerClock;            //timer clock mode
                        teTimerSyn;                //timer synchronize mode

       Refer to point6 for synchronize mode setting.
       fnInit initialize timer registers according to timer mode settings, 
            however, fnStart will initial registers before start the timer.
        You may want to initialize timer register before start the timer, eg, in
            syncronizing timer. Refer to point6 for details on these.
     
    3. Define period and compare(if compare feature is used)
        fnSetPeriod and fnSetCompare or fnSetDelay sets timer registers for 
                peirod and compare control respectively, 
                however, fnStart will initial compare
            and period registers before start the timer.
            ***fnSetPrdFast set period register with user input values of 
            prescaler and period, which can be get by TMRDRV_fnCalcTimerParams.
            ***set ulFrequencyHz in tzTMRDRV_PRD, and get uiPeriod and 
            uiPrescaler set by calling TMRDRV_fnCalcTimerParams
        You may want to initialize those timer register before start the timer,
            eg, in syncronizing timer. Refer to point6 for details on these.
    
    4. To enable a timer interrupt, define teTimerInt to that specific 
            interrupt, and call fnEnInt.
        Repeat the process if you want more than one timer interrupt.
        Global interrupt has to be enabled seperatly to get interrupt.
        Before return from timer interrupt routine, teTimerInt has to be 
            specified to that interrupt, and fnClrIntFlag has to be called to 
            clear that timer interrupt flag.
        If you want to disable a timer interrupt, specify teTimerInt to that 
            interrupt, and call fnDisInt.

    5. fnStart, fnStop, fnHold, fnResume control the timer run operation.
        Before start the timer, make sure
             all timer modes have been set properly, 
             period and compare value are set,
             all necessary interrupts have been enabled, global interrupt 
                    in enabled
        Refer to point6 if synchronize is used.
        
    6. Synchronize timers
        The base timer(other timer synchronize to) has to be created
            and initialized first:
            fnCtor ---must called
            fnInit ---not necessary
            fnPeriod --- if period synchronize, must called
            fnCompare --- not necessary
            fnEnInt ---if interrupts are used
         The synchronize timer is created afterwards,
            fnCtor ---must called
                set proper teTimerSyn, uiTimerIDSyn equals to the base timer's
                    uiTimerID
            fnInit ---not necessary
            fnperiod --- not necessary
            fnCompare --- not necessary
            fnEnInt ---if interrupts are used
            fnStart ---this timer first and then the base timer
                  
    7. To change compare duty ratio after timer is started, put new duty ratio
            to uiTimerDuty, and call fnSetCompare

    8. fnGetStatus checks the creation of a timer instance, and update the timer
            mode member variabls with the current timer register settings.
            fnGetPeriod gets the timer period registers.

    9. To setup and run a timer, consider following two example:
    ****  single timer
    tzTIMER tzTimerPwm = TMRDRV_DFLT;
    void my_main( void )
    {
        ........

        tzTimerPwm.teTimeFunc = eTMRDRV_PWM;
        tuiStatus = TMRDRV_fnCtor( &tzTimerPwm );

        //set period
        tzTimerPwm.ulTimerPeriod = 30;
        
        //set interrupts
        tzTimerPwm.teTimerInt = eTMRDRV_INT_PRD;
        tuiStatus = TMRDRV_fnEnInt( &tzTimerPwm );
        tzTimerPwm.teTimerInt = eTMRDRV_INT_COMPARE;
        tuiStatus = TMRDRV_fnEnInt( &tzTimerPwm );
        //enable global interrupt
        mGLOBAL_IRT_EN();
        
        //start timer
        tuiStatus = TMRDRV_fnStart( &tzTimerPwm );

        .......

    }
    void my_Period_Int( void )
    {
        ........

        tzTimerPwm.teTimerInt = eTMRDRV_INT_PRD;
        tuiStatus = TMRDRV_fnClrIntFlag( &tzTimerPwm );
    }

    **** synchronize two timer peradically
    tzTIMER tzTimerPwm = TMRDRV_DFLT;
    tzTIMER tzTimerSw = TMRDRV_DFLT;

    void my_main( void )
    {
        ...........

        //base timer
        tzTimerPwm.teTimeFunc = eTMRDRV_PWM;
        tuiStatus = TMRDRV_fnCtor( &tzTimerPwm );

        tzTimerPwm.ulTimerPeriod = 30;
        tuiStatus = TMRDRV_fnSetPeriod( &tzTimerPwm );
    
        
        //syncronizing timer
        tzTimerSw.teTimeFunc = eTMRDRV_SW;
        tuiStatus = TMRDRV_fnCtor( &tzTimerSw );
    
        tzTimerSw.teTimerSyn = eTMRDRV_SYN_PERIOD;

        //define synchronized timer
        tzTimerSw.uiTimerIDSyn = tzTimerPwm.uiTimerID;
    
        tzTimerSw.teTimerCompMode = eTMRDRV_ZERO_RELOAD;
        tzTimerSw.ulTimerPeriod = 20;
        tzTimerSw.uiTimerDuty = 5000;
    
        tzTimerSw.teTimerInt = eTMRDRV_INT_COMPARE;
        tuiStatus = TMRDRV_fnEnInt( &tzTimerSw );
        tzTimerSw.teTimerInt = eTMRDRV_INT_PRD;
        tuiStatus = TMRDRV_fnEnInt( &tzTimerSw );
        mGLOBAL_IRT_EN();
    
        //start timers
        tuiStatus = TMRDRV_fnStart( &tzTimerSw );
        tuiStatus = TMRDRV_fnStart( &tzTimerPwm );

        ...........
    }

    see timemain.c for more complete example.

    
CHANGE HISTORY:
$History: Tmrdrv.h $
    
    *****************  Version 26  *****************
    User: Yingrand     Date: 7/29/03    Time: 3:37p
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/TmrDrv
    package name to TMRDRV
    
    *****************  Version 25  *****************
    User: Yingrand     Date: 7/29/03    Time: 11:08a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/TmrDrv
    update enum define to meet standard
    
    *****************  Version 23  *****************
    User: Yingrand     Date: 5/27/03    Time: 10:36a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/TmrDrv
    update after code reviews;
    spelling mistakes and code formating
    
    *****************  Version 22  *****************
    User: Yingrand     Date: 5/22/03    Time: 12:52p
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/TmrDrv
    ready for code review
    
    *****************  Version 21  *****************
    User: Johnb        Date: 4/14/03    Time: 11:19a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/TmrDrv
    Updated to coding standard where possible.
    Updated function names from TMRDRV_ to TMRDRV_ for the package prefixing.
    Moved compile options to the .c file.
    Added eTIMER_ERRNO_BAD_OPTION to the teTIMER_ERRNO enum.
    Changed TIMER_fnDisIntAll() and TIMER_fnClrIntAll() to return void.

Date:   Oct 30 2002   By: John Bellini
    -   Created
Date:   Nov. 12 2002  By: yingran Duan
    -   Done with first draft
    
Date:   Nov 20, 2002  By:  John Bellini
    - Header file name changed from Errbase.h to HALErrBase.h

Date:   Nov. 25, 2002  By: Yingran Duan
    -   optimize code size with compile optimization
        before optimization 5.7K, after optimization with all the compile option
        0-1.85K
Date:   Dec. 2, 2002    By: Yingran Duan
    -   add two functions of get period register value and set compare register
        based on delay time from captured counter value
Date:   Dec. 16, 2002   By: Yingran Duan
    -   add function of fnSetPrdFast to update period register quickly without
        calculate prescaler and period value based on frequecy. The prescaler
        and preiod value is user defined.
    -   made function TIMER_fnCalcTimerParams public for user to use to 
        calcualate prescaler and period.
Date:   Jan. 13, 2003   By: Yingran Duan
    -   change name from tmr.h to tmrdrv.h
    
==============================================================================*/

#ifndef TMR_INCL
#define TMR_INCL

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    //Requires an Architecture bit to be defined.
#include "LIB_stddefs.h"    //Standard definitions.
#include "HALErrBase.h"        //For return type error codes (using errno)

/*==============================================================================
                              Defines
==============================================================================*/
//Timer interrupt status bit define
#define TMRDRV_INT_EN            ( 1 )       //interrupt enabled     

#define TMRDRV_INT_NONE          ( 0 )
#define TMRDRV_INT_PRD           ( 1 )
#define TMRDRV_INT_COMP          ( 2 )
#define TMRDRV_INT_OF            ( 4 )
#define TMRDRV_INT_UF            ( 8 )


//timer instance DFLT setting
#define TMRDRV_INTERUPT_DFLT         ( 0 )  
#define TMRDRV_NO_INT                ( TMRDRV_eNO_INT )
#define TMRDRV_ID_DFLT               ( 0 )
#define TMRDRV_PRD_DFLT              ( 0 )
#define TMRDRV_DUTY_DFLT             ( 0 )
#define TMRDRV_MODE_DFLT             ( TMRDRV_eCONTUPDOWN )
#define TMRDRV_ADC_DFLT              ( TMRDRV_eADC_SEPERATE )
#define TMRDRV_OUTPUT_DFLT           ( TMRDRV_eNO_OUTPUT )
#define TMRDRV_EMU_DFLT              ( TMRDRV_eEMU_STOP_IMM )
#define TMRDRV_COMP_DFLT             ( TMRDRV_eNO_COMP )
#define TMRDRV_SYN_DFLT              ( TMRDRV_eNO_SYN )
#define TMRDRV_CLOCK_DFLT            ( TMRDRV_eINTERNAL )
#define TMRDRV_FN_DFLT               ( TMRDRV_eFN_INVALID )

#define TMRDRV_DFLT {   TMRDRV_ID_DFLT,      \
                        TMRDRV_PRD_DFLT,     \
                        TMRDRV_DUTY_DFLT,    \
                        TMRDRV_NO_INT,       \
                        TMRDRV_MODE_DFLT,    \
                        TMRDRV_ADC_DFLT,     \
                        TMRDRV_OUTPUT_DFLT,  \
                        TMRDRV_EMU_DFLT,     \
                        TMRDRV_COMP_DFLT,    \
                        TMRDRV_SYN_DFLT,     \
                        TMRDRV_ID_DFLT,      \
                        TMRDRV_CLOCK_DFLT,   \
                        TMRDRV_FN_DFLT,      \
                        TMRDRV_INTERUPT_DFLT }


/*==============================================================================
                            Type Definitions
==============================================================================*/

/*=============================================================================
                                Enums
==============================================================================*/
//errno codes based on errbase.h
typedef enum TMRDRV_eERRNO
{
    TMRDRV_eERRNO_SOMETHING = ERR_TMR_DRV ,  //Start of errno's for the timer.
    TMRDRV_eERRNO_TIMER_NO_DEF,              //timer ID is not a valid number
                                            //get correct ID by fnGetState
                                            //or create instance by fnCtor
    TMRDRV_eERRNO_TIMER_NO_FN,             //timer function not defined
                                            //give a eTIMER_FN to teTimeFunc 
    TMRDRV_eERRNO_TIMER_NOT_VALID,           //timer for this function has been
                                            //used, use fnGetStatus to get 
                                            //information for this timer's 
                                            //status
    TMRDRV_eERRNO_TIMER_TO_CONUPDOWN_MODE,   //timer mode is set to continous
                                            //up and down mode; set to proper
                                            //count mode, call fnInit again
    TMRDRV_eERRNO_TIMER_TO_ADC_SEPERATE_MODE,//timer ADC mode is set to ADC
                                            //seperate mode; set to proper
                                            //ADC mode, call fnInit again
    TMRDRV_eERRNO_TIMER_TO_NO_OUTPUT_MODE,   //timer output mode is set to No
                                            //output mode; set to proper
                                            //output mode, call fnInit again
    TMRDRV_eERRNO_TIMER_TO_EMU_STOPIMM_MODE, //timer emulation mode is set to 
                                            //stop immediately mode; set to 
                                            //proper emulation mode, call 
                                            //fnInit again
    TMRDRV_eERRNO_TIMER_TO_NO_COMP_MODE,     //timer compare mode is set to 
                                            //no compare mode; set to proper
                                            //compare mode, call fnInit again
    TMRDRV_eERRNO_TIMER_TO_NO_SYN_MODE,      //timer synchronize mode is set to 
                                            //no synchronize mode; set to proper
                                            //synchronize mode, call fnInit again
    TMRDRV_eERRNO_TIMER_TO_INTERNAL_MODE,    //timer set to internal clock mode;
                                            //set to proper clock mode, call 
                                            //fnInit again
    TMRDRV_eERRNO_TIMER_ADC_MODE_INVALID,    //ADC mode invalid; set to proper
                                            //ADC mode, call fnInit again
    TMRDRV_eERRNO_TIMER_SYN_ERR,             //timer synchronize set is wrong; 
                                            //set proper uiTimerIDSyn, call
                                            //fnInit again 
    TMRDRV_eERRNO_TIMER_COUNT_MODE_INVALID,  //timer count mode invalid; set 
                                            //proper count mode, call 
                                            //fnInit again
    TMRDRV_eERRNO_TIMER_OUT_MODE_INVALID,    //timer output mode invalid; set 
                                            //proper clock mode, call 
                                            //fnInit again
    TMRDRV_eERRNO_TIMER_EMU_MODE_INVALID,    //timer emulation mode is invalid;
                                            //set proper emulation mode, call 
                                            //fnInit again
    TMRDRV_eERRNO_COMP_MODE_INVALID,         //timer compare mode invalid, set
                                            //proper compare mode, call fnInit
    TMRDRV_eERRNO_CLOCK_MODE_INVALID,        //timer clock mode invalid, set 
                                            //proper clock mode, call fnInit
    TMRDRV_eERRNO_TIMER_FN_ASSIGNED,       //function ID missing, and had been
                                            //assigned according to timerID
    TMRDRV_eERRNO_TIMER_NOT_CREATED,         //this timer instance is not 
                                            //created, call fnCtor first
    TMRDRV_eERRNO_TIMER_ERR_ASSIGNED,        //timer assignment is wrong, fnDtor
                                            //current instance and create 
                                            //one again by fnCtor
    TMRDRV_eERRNO_TIMER_NOT_ASSIGNED,        //timer not assigned, call fnCtor
                                            //first
    TMRDRV_eERRNO_TIMER_NO_INT,              //timer interrupt not defined, give
                                            //a interrupt ID to teTimerInt, 
                                            //call fnEnInt again 
    TMRDRV_eERRNO_PERIOD_MAX,                //period set to max value
    TMRDRV_eERRNO_PERIOD_MIN,                //period set to min value
    TMRDRV_eERRNO_BAD_OPTION,                //specifies when an option is bad.
    TMRDRV_eERRNO_MAX = ERR_TMR_DRV_MAX      //Max val not to be used.

}TMRDRV_teERRNO ;

//should be same sequence with TIMER_INT_PRD,TIMER_INT_COMP,
//TIMER_INT_OF, TIMER_INT_UF
typedef enum TMRDRV_eINT         //timer interrupt
{
    TMRDRV_eNO_INT = 0,          //no interrupt
    TMRDRV_eINT_PRD,             //period interrupt
    TMRDRV_eINT_COMPARE,         //compare match interrupt
    TMRDRV_eINT_OVERFLOW,        //counter overflow interrupt
    TMRDRV_eINT_UNDERFLOW        //counter underflow interrupt
}TMRDRV_teINT;

typedef enum TMRDRV_eCOUNT_MODE      //counting modes
{
    TMRDRV_eCONTUPDOWN = 0,          //continuous up and down
    TMRDRV_eCONTUP,                  //continuous up
    TMRDRV_eDIRUPDOWN,               //directional up and down
    TMRDRV_eSTOPHOLD                 //counter hold
}TMRDRV_teCOUNT_MODE;

typedef enum TMRDRV_eADC_MODE        //timer can start ADC mode
{
    TMRDRV_eADC_SEPERATE = 0,        //no ADC is triggered by timer
    TMRDRV_eADC_COMPARE,             //ADC triggered by compare match
    TMRDRV_eADC_PERIOD,              //ADC triggered by period match
    TMRDRV_eADC_UNDERFLOW            //ADC triggered at counter underflow
}TMRDRV_teADC_MODE;

typedef enum TMRDRV_eCOMP_OUTPUT //timer compare output set
{
    TMRDRV_eNO_OUTPUT = 0,       //no compare output
    TMRDRV_eFORCE_LOW,           //force output to be low
    TMRDRV_eFORCE_HIGH,          //force output to be high
    TMRDRV_eACTIVE_LOW,          //put output to be active low
    TMRDRV_eACTIVE_HIGH          //put output to be active high
}TMRDRV_teCOMP_OUTPUT;

typedef enum TMRDRV_eEMU         //timer emulation mode
{
    TMRDRV_eEMU_STOP_IMM = 0,    //stop immediatly at emulation suspend
    TMRDRV_eEMU_FREE,            //free run
    TMRDRV_eEMU_STOP_AFTER       //stop after current timer period at 
                                //emulation suspend
}TMRDRV_teEMU;

typedef enum TMRDRV_eCOMP_MODE   //timer compare mode
{
    TMRDRV_eNO_COMP = 0,         //no compare 
    TMRDRV_eZERO_RELOAD,         //compare reload at counter==0
    TMRDRV_eEQUAL_RELOAD,        //compare reload at counter==O or period
    TMRDRV_eIMM_RELOAD           //compare reload immediatly
}TMRDRV_teCOMP_MODE;

typedef enum TMRDRV_eSYN         // timer synchronize
{
    TMRDRV_eNO_SYN = 0,          //no synchronize
    TMRDRV_eSYN_PERIOD,          //synchronize prd
    TMRDRV_eSYN_START,           //synchronize starting
    TMRDRV_eSYN_PRD_ST           //synchronixe period and starting
}TMRDRV_teSYN;
    
typedef enum TMRDRV_eCLOCK       //timer clock source
{
    TMRDRV_eINTERNAL = 0,        //use internal clock source
    TMRDRV_eEXTERNAL,            //use external clock
    TMRDRV_eQEP                  //use QEP as clock
}TMRDRV_teCLOCK;


typedef enum TMRDRV_eFN          //timer function enum
{
    TMRDRV_eFN_INVALID = 0,
    TMRDRV_ePWM,                 //for digital control and PWM operation 
    TMRDRV_eSW,                  //to generate sine wave reference
    TMRDRV_eRTOS,                //rtos clock tick
    TMRDRV_eGP                   //general purpose
}TMRDRV_teFN;

/*==============================================================================
                              Structures
==============================================================================*/

//structure defined for timer period and prescaler calculation
typedef struct zTMRDRV_PRD
{
    uint32 ulFrequencyHz;       //frequency in Hz
    uint16 uiPeriod;            //calculated period
    uint16 uiPrescaler;         //calculated prescaler, shift bits
}TMRDRV_tzPRD;

//timer instance structure
typedef struct zTIMER
{
    uchar8 uiTimerID;                       //timer ID, assigned by 'fnCtor'
    uint32 ulTimerPeriod;                   //timer period in Hz
    uint16 uiTimerDuty;                     //timer compare value, dutyratio
                                            //eg. if 50%, put 5000 here
    TMRDRV_teINT teTimerInt;                 //timer interrupt ID, call 'fnEnInt'
                                            //to enable this interrupt
    TMRDRV_teCOUNT_MODE teTimerCountMode;    //counter mode
    TMRDRV_teADC_MODE teTimerAdcMode;        //ADC start mode
    TMRDRV_teCOMP_OUTPUT teTimerCompOutput;  //timer compare output mode
    TMRDRV_teEMU teTimerEmu;                 //timer emulation mode
    TMRDRV_teCOMP_MODE teTimerCompMode;      //timer compare mode
    TMRDRV_teSYN teTimerSyn;                 //timer synchronize mode
    uint16 uiTimerIDSyn;                    //synchronize with this timer
    TMRDRV_teCLOCK teTimerClock;             //timer clock mode
    TMRDRV_teFN teTimeFunc;                  //timer function, has to defined 
                                            //before 'fnCtor' is called
    uint16 uiTimerInterrupt;                //timer interupts enable status
}TMRDRV_tzTIMER;

/*==============================================================================
                             Constants
==============================================================================*/
                        
/*==============================================================================
                               Variables
==============================================================================*/

/*==============================================================================
                            Function Protoypes
==============================================================================*/
//creat timer instance, assign a timer to timerID
tuiSTATUS TMRDRV_fnCtor( TMRDRV_tzTIMER *ptzTimer );  

//destroy timer instances, free a timer from this function use  
tuiSTATUS TMRDRV_fnDtor( TMRDRV_tzTIMER *ptzTimer );   

//Returns the status of the timer, all the timer variable is set according to 
//timer register settings, period and compare are not included
tuiSTATUS TMRDRV_fnGetStatus( TMRDRV_tzTIMER *ptzTimer); 

//reset timer registers  
tuiSTATUS TMRDRV_fnResetPackage( void );  

//Start the tmr, all the timer reg settings are updated before starting the tmr
tuiSTATUS TMRDRV_fnStart( TMRDRV_tzTIMER *ptzTimer );  

//Stop the timer
tuiSTATUS TMRDRV_fnStop( TMRDRV_tzTIMER *ptzTimer ); 

//Hold the timer, without clear the counter register 
tuiSTATUS TMRDRV_fnHold( TMRDRV_tzTIMER *ptzTimer );   

//Resume the timer from hold, no update on register, compare and period 
tuiSTATUS TMRDRV_fnResume( TMRDRV_tzTIMER *ptzTimer );   

//Init the timer registers according to using setting 
tuiSTATUS TMRDRV_fnSet( TMRDRV_tzTIMER *ptzTimer ); 

//Set a timer period register                                
tuiSTATUS TMRDRV_fnSetPeriod( TMRDRV_tzTIMER *ptzTimer ); 

//set a timer period register with readied prescaler and period value
//****NOTE, frequency value in tzTIMER does not change accordingly with this.
tuiSTATUS TMRDRV_fnSetPrdFast( TMRDRV_tzTIMER *ptzTimer, 
                               TMRDRV_tzPRD *ptzTimerPrd );

//return the period setting of the timer specified
tuiSTATUS TMRDRV_fnGetPeriod( TMRDRV_tzTIMER *ptzTimer, 
                              TMRDRV_tzPRD *ptzTimerPrd );

//Update a timer compare register
tuiSTATUS TMRDRV_fnSetCompare( TMRDRV_tzTIMER *ptzTimer ); 

//set timer compare reg specified based on delay time from captured cntr value
tuiSTATUS TMRDRV_fnSetDelay( TMRDRV_tzTIMER *ptzTimer, 
                             uint16 uiTimeDelay );

//clear a interrupt flag, this function has to be called at the end of each 
//timer interrupt service routine
tuiSTATUS TMRDRV_fnClrIntFlag( TMRDRV_tzTIMER *ptzTimer ); 

//enable a interrupt, this fn has to be called to enable a specific timer int
tuiSTATUS TMRDRV_fnEnInt( TMRDRV_tzTIMER *ptzTimer ); 

//disable a interrupt, this fn has to be called to disable a specific timer int
tuiSTATUS TMRDRV_fnDisInt( TMRDRV_tzTIMER *ptzTimer ); 

//disable all timer interrupt
void TMRDRV_fnDisIntAll( void ); 

//clear all timer interrupt flag
void TMRDRV_fnClrIntFlagAll( void ); 

#endif    // TMR_INCL
