/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    PwmDrv.c

PURPOSE:
    This the pwm driver implementation.  The purpose is to allow a user to get
    a PWM and have it setup for them.  
    
    Currently this files implemenation is geared towards use with of a full 
    H-Bridge with 4 pwms (2 complimentary pairs) for each leg of the bridge

FUNCTION(S):
    PWMDRV_fnCtor            - Constructor
    PWMDRV_fnDtor            - Destructor
    PWMDRV_fnEn              - En the PWM(op's en)
    PWMDRV_fnDis             - Dis the PWM(op's hi-Z)
    PWMDRV_fnUpdateDutyCycle - How to update the PWM
    PWMDRV_fnSetOpt          - Set PWM options 
   
    local:
        pwmdrv_fnSetupSpv          - Setup space vector pwming
        pwmdrv_fnSetupSymmetric    - Setup a  symmetric PWM
        pwmdrv_fnSetupAsymmetric   - Setup a asymmetric PWM
        pwmdrv_fnCalcDeadBand      - Calc the dead band based on device options
        pwmdrv_fnSetComCon         - Setup the ComCon registers.
   
NOTES:
    This driver is built to not Include any Space Vector PWMing code (SPV), 
    however, the hooks are in place for it to be inserted when needed.
    
    USING THIS FILE - DRIVER CONFIG:
    The first set of #defines like PWM_EN_EVA and PWM_EN_EVB need to be setup
    to include which Event manager you are using (EVA, B or both).  Then since 
    only one waveform type per event manager can be setup, the waveform types 
    need to be enabled or disabled using  PWM_EN_SPV_SETUP, 
    PWM_EN_ASYMMETRIC_SETUP, PWM_EN_SYMMETRIC_SETUP.
    
    When this file was created, the target was using PWM's 1 to 4 
    inclusively of EVA.  EVA PWM's 5 and 6 were not intended for use. Hence,
    the code for PWM 5 and6 where possible, has been blocked out.  
    
    Functions can be enabled or disable with the first set of defines.  Use this
    section in exclude functions that will not be used by your application.

    zPWM_INIT:  This structure should not exceed 16 bits.    
    
    TESTING:
    If you want to test all of the EVA, then define the following: 
        PWMDRV_UNIT_TEST
    Otherwise, only pairs 1,2 and 3,4 are compiled, NOT 5,6.  PWM's for EVB are
    not tested since they are not used at this time.    
    
    !!!!IMPORTANT NOTE!!!!
    When this package was last tested, PWM pairs 1 and 2 work simultaneously.  
    However, if PWM pair 3 is setup as well (the pairs 1, 2, and 3 of EVA), 
    PWM #2 will stop working, but 1 and 3 will work.  Since at the time this
    package was design only 1 and 2 mattered, then the code was left as is.


CHANGE HISTORY:
$Log: Pwmdrv.c $

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/2
    User: HorstM     Date: 02/21/05  Time: 12:48AM
    added EALLOW/EDIS to protected register macro defn's
    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/1
    User: HorstM     Date: 02/17/05  Time: 07:24PM
    added EALLOW/EDIS to protected register macro defn's
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/3
    User: JohnB     Date: 01/28/05  Time: 09:53PM
    Removed define for GLBL_DTOR_EN
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/2
    User: JohnB     Date: 01/24/05  Time: 09:14PM
    Updated for use with SYSCLK package. Removed INIT_fnGetSysClk()
    functionality.
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/1
    User: JohnB     Date: 01/11/05  Time: 09:26PM
    Added return to Ctor to quiet a warning.
    
    
==============================================================================*/
/*==============================================================================
                              Includes
==============================================================================*/
#include "PwmDrv.h"         //This Packages header file
#include "PwmDrvC28xOpt.h"  //Device specific implementation details
#include "sysclk.h"         //System level access to the device
#include "config.h"
#include "Device.h"        //Device header file
#include <errno.h>          //Ansi error functions
#include "xassert.h"        //assert


/*==============================================================================
                              Defines
==============================================================================*/
/* ^^^^^^^^^^^^^^^^^^^^^^^ Function Enables ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
                                    //---------------- Public ------------------
#define PWM_FN_CTOR_EN          ( TRUE  )//Include code for constructor
#define PWM_FN_SET_DB_EN        ( TRUE  )//Include code for set dead band
#define PWM_FN_ENABLE_DB_EN     ( TRUE  )//Include code for set dead band
#define PWM_FN_DISABLE_DB_EN    ( TRUE  )//Exclude code for set dead band
#define PWM_FN_ENABLE_EN        ( TRUE  )//Include code for pwm enable
#define PWM_FN_DISABLE_EN       ( TRUE  )//Include code for pwm disable
#define PWM_FN_UPDATE_DC_EN     ( TRUE  )//Include code for update duty cycle
            
                                    //--------------- Private ------------------
#define PWM_FN_SETUP_SPV_EN     ( FALSE )//Exclude code for space vect setup
#define PWM_FN_SETUP_SYM_EN     ( TRUE  )//Include code for setup symmetric pwm
#define PWM_FN_SETUP_ASYM_EN    ( FALSE )//Exclude code for setup asymmetric pwm
#define PWM_FN_SET_COM_CON_EN   ( TRUE  )//Include code for setup the COMCON reg
#define PWM_FN_CALC_DB_EN       ( TRUE  )//Include code for calc the dead band


//--------------------------- Event Manager Enables ----------------------------
//NOTE: THE FOLLOWING MUST BE SETUP FOR THIS DRIVER TO COMPILE CORRECTLY.
//Choose which event managers you want to deal with, EVA (PWM_EN_EVA) or
//EVB (PWM_EN_EVA) or both.  Set these #defines to TRUE as per your requirements.
#define PWM_EN_EVA  ( TRUE )
#define PWM_EN_EVB  ( TRUE )

//Make sure the user has setup the event manager(s) to be used.
#if( (PWM_EN_EVA == FALSE) && (PWM_EN_EVB == FALSE) )
#error Must select at least one event manager to use
#endif


//---------------------------- Waveform Config ---------------------------------
//Choose which waveform scheme is desired for the pwm application by setting its 
//#define to true (PWM_EN_SPV_SETUP, PWM_EN_ASYMMETRIC_SETUP, or 
//PWM_EN_SYMMETRIC_SETUP).  If two event managers are being used, then two 
//different options are allowed.  Note, If SPV is selected, an entire event 
//managers PWM's are used.
#define PWM_EN_SPV_SETUP        ( FALSE )   //Don't include this code
#define PWM_EN_ASYMMETRIC_SETUP ( FALSE )   //Don't include this code
#define PWM_EN_SYMMETRIC_SETUP  ( TRUE  )   //Do include this code

//Make sure the user has setup the wave form types.
#if( (PWM_EN_SPV_SETUP        == FALSE) && \
     (PWM_EN_ASYMMETRIC_SETUP == FALSE) && \
     (PWM_EN_SYMMETRIC_SETUP  == FALSE) )
#error Must select at least one wave type to use
#endif




//------------------------- General PWM Defines --------------------------------
#define DB_MAX_PERIOD_SCALER    ( 15    )   //Max num for dead-band prescaler
#define DB_DFLT_TIME_NS         ( 1000  )   //Dflt dead band time in ns
#define DB_MIN_NS               ( 0     )   //0ns min time for dead band
#define DB_ONE_BILLION          ( 1000000000L )// 1,000,000,000 (1 Billion)

#define PWM_ALL_USED            ( 0xFFFF )  //All the available PWM's are used.
#define PWM_INITIALIZED         ( TRUE   )  //Use w/ the struct zPWM_INIT
#define PWM_NOT_INITIALIZED     ( !PWM_INITIALIZED  )
#define PWM_NO_TIMER_PRD        ( 0     )   //Used to turn off the period.

//In order to hopefully make the code a little reasier to read, this
//define is used instead of what it is defined as.
#define PWM_TARGET_OPT          ( (PWMDRV_tzOPTIONS_TI_C28X*)ptzPwm->ptzPwmOpt )
/*==============================================================================
                              Macro's
==============================================================================*/
//Set the pins available as pwms for this driver.
#define mEN_PWM_PAIR_1_PINS() \
EALLOW; \
GpioMuxRegs.GPAMUX.all |= ( F28X_GPAMUX_MSK_PWM1 | F28X_GPAMUX_MSK_PWM2 ); \
EDIS

#define mEN_PWM_PAIR_2_PINS() \
EALLOW; \
GpioMuxRegs.GPAMUX.all |= ( F28X_GPAMUX_MSK_PWM3 | F28X_GPAMUX_MSK_PWM4 ); \
EDIS

#define mEN_PWM_PAIR_3_PINS() \
EALLOW; \
GpioMuxRegs.GPAMUX.all |= ( F28X_GPAMUX_MSK_PWM5 | F28X_GPAMUX_MSK_PWM6 ); \
EDIS

#define mEN_PWM_PAIR_4_PINS() \
EALLOW; \
GpioMuxRegs.GPBMUX.all |= ( F28X_GPBMUX_MSK_PWM7 | F28X_GPBMUX_MSK_PWM8 ); \
EDIS

#define mEN_PWM_PAIR_5_PINS() \
EALLOW; \
GpioMuxRegs.GPBMUX.all |= ( F28X_GPBMUX_MSK_PWM9 | F28X_GPBMUX_MSK_PWM10 ); \
EDIS

#define mEN_PWM_PAIR_6_PINS() \
EALLOW; \
GpioMuxRegs.GPBMUX.all |= ( F28X_GPBMUX_MSK_PWM11 | F28X_GPBMUX_MSK_PWM12 ); \
EDIS

//Disable the PWM pins.                                        
#define mDIS_PWM_PAIR_1_PINS() \
EALLOW; \
GpioMuxRegs.GPAMUX.all &= ( ~( F28X_GPAMUX_MSK_PWM1 | F28X_GPAMUX_MSK_PWM2 ) ); \
EDIS

#define mDIS_PWM_PAIR_2_PINS() \
EALLOW; \
GpioMuxRegs.GPAMUX.all &= ( ~( F28X_GPAMUX_MSK_PWM3 | F28X_GPAMUX_MSK_PWM4 ) ); \
EDIS

#define mDIS_PWM_PAIR_3_PINS() \
EALLOW; \
GpioMuxRegs.GPAMUX.all &= ( ~( F28X_GPAMUX_MSK_PWM5 | F28X_GPAMUX_MSK_PWM6 ) ); \
EDIS

#define mDIS_PWM_PAIR_4_PINS() \
EALLOW; \
GpioMuxRegs.GPBMUX.all &= ( ~( F28X_GPBMUX_MSK_PWM7 | F28X_GPBMUX_MSK_PWM8 ) ); \
EDIS

#define mDIS_PWM_PAIR_5_PINS() \
EALLOW; \
GpioMuxRegs.GPBMUX.all &= ( ~( F28X_GPBMUX_MSK_PWM9 | F28X_GPBMUX_MSK_PWM10 ) ); \
EDIS

#define mDIS_PWM_PAIR_6_PINS() \
EALLOW; \
GpioMuxRegs.GPBMUX.all &= ( ~( F28X_GPBMUX_MSK_PWM11 | F28X_GPBMUX_MSK_PWM12 ) ); \
EDIS

//Enable PWM compare outputs.
#define mEN_EVA_ALL()        ( EvaRegs.COMCONA.all |= F28X_COMCON_MSK_FCMPOE )
#define mEN_EVB_ALL()        ( EvbRegs.COMCONB.all |= F28X_COMCON_MSK_FCMPOE )
#define mEN_SPVA_ALL()       ( EvaRegs.COMCONA.all |= F28X_COMCON_MSK_SVENABLE )
#define mEN_SPVB_ALL()       ( EvbRegs.COMCONB.all |= F28X_COMCON_MSK_SVENABLE )

//Dis PWM compare outputs.
#define mDIS_EVA_ALL()      ( EvaRegs.COMCONA.all &= ~F28X_COMCON_MSK_FCMPOE )
#define mDIS_EVB_ALL()      ( EvbRegs.COMCONB.all &= ~F28X_COMCON_MSK_FCMPOE )
#define mDIS_SPVA_ALL()     ( EvaRegs.COMCONA.all &= ~F28X_COMCON_MSK_SVENABLE )
#define mDIS_SPVB_ALL()     ( EvbRegs.COMCONB.all &= ~F28X_COMCON_MSK_SVENABLE )

//For disabling dead band operation
#define mDIS_DB_PWM_PAIR_1()   ( EvaRegs.DBTCONA.all &= ~F28X_DBTCON_MSK_EDBT1 )
#define mDIS_DB_PWM_PAIR_2()   ( EvaRegs.DBTCONA.all &= ~F28X_DBTCON_MSK_EDBT2 )
#define mDIS_DB_PWM_PAIR_3()   ( EvaRegs.DBTCONA.all &= ~F28X_DBTCON_MSK_EDBT3 )
#define mDIS_DB_PWM_PAIR_4()   ( EvbRegs.DBTCONB.all &= ~F28X_DBTCON_MSK_EDBT1 )
#define mDIS_DB_PWM_PAIR_5()   ( EvbRegs.DBTCONB.all &= ~F28X_DBTCON_MSK_EDBT2 )
#define mDIS_DB_PWM_PAIR_6()   ( EvbRegs.DBTCONB.all &= ~F28X_DBTCON_MSK_EDBT3 )

/*==============================================================================
                            Type Definitions
==============================================================================*/
//None


/*==============================================================================
                                Enums
==============================================================================*/

/*=============================================================================
                              Structures
==============================================================================*/
//The init status for each PWM.
//This struct should not exceed 16 bits or some of the code in this driver will 
//not evaluate correctly.
typedef struct zPWM_INIT
{
    uint16 bPwm12Init    : 1;    //PWM  1 and 2 Init or not
    uint16 bPwm34Init    : 1;    //PWM  3 and 2 Init or not
    uint16 bPwm56Init    : 1;    //PWM  5 and 2 Init or not
    uint16 bPwm78Init    : 1;    //PWM  7 and 2 Init or not
    uint16 bPwm910Init   : 1;    //PWM  9 and 2 Init or not
    uint16 bPwm1112Init  : 1;    //PWM 11 and 2 Init or not
    uint16 bPwmSPVAInit  : 1;    //SPV PWM A Init or not
    uint16 bPwmSPVBInit  : 1;    //SPV PWM B Init or not
    uint16 bPwmReserved  : 8;    //Reserved so the compiler fills a uint16
   
}tzPWM_INIT;

//Based on the option set for the way this file is to compile, the Init struct 
//needs to be setup properly for the code to work properly. 
//Example: 
//  Assume SPV will not be enabled
//  If only EVA is enabled, then the init flags for EVB need to be SET (act like 
//  they are all used).  This will make the code that checks if all the PWM's 
//  check for only all 3 PWM's available in EVA are all taken.
#if( PWM_EN_EVA == TRUE && PWM_EN_EVB == FALSE )
static tzPWM_INIT tzPwmInit = { 0,          //PWM 1 and 2 available
                                0,          //PWM 3 and 4 available
                                0,          //PWM 5 and 6 available
                                1,          //PWM 7 and 8 not available
                                1,          //PWM 9 and 10 not available
                                1,          //PWM 11 and 12 not available
                                1,          //SPV A not available
                                1,          //SPV B not available
                                0xFF };
                                
//Handle the case where EVB is only manager enabled.                                
#elif( PWM_EN_EVA == FALSE && PWM_EN_EVB == TRUE )
static tzPWM_INIT tzPwmInit = { 1,          //PWM 1 and 2 not available
                                1,          //PWM 3 and 4 not available
                                1,          //PWM 5 and 6 not available
                                0,          //PWM 7 and 8 available
                                0,          //PWM 9 and 10 available
                                0,          //PWM 11 and 12 available
                                1,          //SPV A not available
                                1,          //SPV B not available
                                0xFF };
                                
//Handle the case were both EV's are enabled.
#elif( PWM_EN_EVA == TRUE && PWM_EN_EVB == TRUE )
static tzPWM_INIT tzPwmInit = { 0,          //PWM 1 and 2 available
                                0,          //PWM 3 and 4 available
                                0,          //PWM 5 and 6 available
                                0,          //PWM 7 and 8 available
                                0,          //PWM 9 and 10 available
                                0,          //PWM 11 and 12 available
                                1,          //SPV A not available
                                1,          //SPV B not available
                                0xFF };
#endif //End #if( PWM_EN_EVA == TRUE && PWM_EN_EVB == FALSE && PWM_EN_SPV_SETUP

/*==============================================================================
                           Local/Private Variables
==============================================================================*/
static uint16 DbPrescMsk;    //The dead-band prescaler
static uint16 DbPrdMsk;      //The dead-band period

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
#if( PWM_FN_SETUP_SPV_EN == TRUE )   //Compile Enable for this function
//-------- SPV Setup --------
static void pwmdrv_fnSetupSpv( PWMDRV_tzDATA *ptzPwm );          //PWM from the user
#endif
                                 
#if( PWM_FN_SETUP_SYM_EN == TRUE )   //Compile Enable for this function
//---- Symmetrical Setup ----
static void pwmdrv_fnSetupSymmetric( PWMDRV_tzDATA *ptzPwm );    //PWM from the user
#endif

#if( PWM_FN_SETUP_ASYM_EN == TRUE )   //Compile Enable for this function
//--- Asymmetrical Setup ----                                       
static void pwmdrv_fnSetupAsymmetric( PWMDRV_tzDATA *ptzPwm );
#endif

#if( PWM_FN_CALC_DB_EN == TRUE )   //Compile Enable for this function
//------ Dead Band Calculator ------
//Calculate the dead band, dead band prescaler and the dead band period
static void pwmdrv_fnCalcDeadBand( uint16  DeadBandNs ); //Deadband in nanosec
#endif

#if( PWM_FN_SET_COM_CON_EN == TRUE )   //Compile Enable for this function
//---- Setup for the COMCON Reg ----                                     
static void pwmdrv_fnSetComCon( PWMDRV_tzDATA *ptzPwm );         //User PWM data
#endif


/*==============================================================================
                           Public Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    PWMDRV_fnCtor
  
PURPOSE: 
    Create, init and assign a PWM for the user.

INPUTS: 
    A pointer to the users already created PWMDRV_tzDATA structure.  This struct must
    have the desired user options set.

OUTPUTS:
    tuiStatus result.
    The users pwm struct is also updated with an ID for ONE PWM PAIR.  

NOTES:
    This function can be called more than once to get more than one PWM option.
    Since the user may require more than one PWM, the pairs are assigned in the 
    following order:
    
        EVA is given first
            PAIR 1 & 2
            PAIR 3 & 4
            PAIR 5 & 6
        EVB is given second
            PAIR 7 & 8
            PAIR 9 & 10
            PAIR 11 & 12
            

VERSION HISTORY:

Version: 1.00  Date: Nov 28, 2002  By: John Bellini
    - Created.

Version: 1.01  Date: Jan 22, 2003  By: Jason Cumiskey
    - Capitalized pwm to conform to latest coding standard.
    
Version: 2.00  Date: May 23, 2003  By: John Bellini
    - Replace switch statement with if statements since its simpler and in
      most cases each EVA or EVB will be setup for the same type of operation
      (asymmetrical, symmetrical, etc).
    - Made better use of asserts.
    
Version: 2.10  Date: Nov 5, 2003  By: John Bellini
    - Moved pwmdrv_fnGetAvailPwmPair here from within the setup functions
      so that the setup fn's can be called from PWMDRV_fnSetOpt().

*******************************************************************************/
#if( PWM_FN_CTOR_EN == TRUE )   //Compile Enable for this function
tuiSTATUS PWMDRV_fnCtor( PWMDRV_tzDATA *ptzPwm )
{
    //Make sure the pass parameter is cool.
    assert( ptzPwm );
    
    /*^^^^^^^^^^^^^^^^^^^^^
    Special setup handling.
    ^^^^^^^^^^^^^^^^^^^^^*/
    //None
    
    /*^^^^^^^^^^^^^^^^^^^^^^
    Some general init stuff.
    ^^^^^^^^^^^^^^^^^^^^^^*/
    //Check if all the PWM's are taken, if so, don't assign any more, set the
    //errno code and get out.  Since the timer init flags are in this struct
    //as well, its important to know that as soon as the first PWM ID is taken
    //for an event mananger, the timer init flag for that EV is set as well.
    //So when all all the PWM's are taken the two timer init flags are set
    //as well.
    //!!!NOTE:  tzPwmInit is a struct of 16 bits.  Instead of checking each bit 
    //!!!NOTE:  individually, look at the struct as a int.  So the following 
    //!!!NOTE:  casts tzPwmInit to a uint16 and then dereferences it to do the 
    //!!!NOTE:  compare.
    if( *( ( uint16 * )&tzPwmInit ) == PWM_ALL_USED )
    {
        assert( 0 );
    }

    // PWM setup - NOTE that API is changed for F2812.  It is expected that any
    // calls to this package will specify the application for the requested
    // PWM by pre-initializing the PwmId appropriately
    // register PWM as used
    switch( ptzPwm->tePwmId )
    {
        #if( PWM_EN_EVA == TRUE )
        case ePWM_ID_1_2:
            if( tzPwmInit.bPwm12Init == FALSE )
            {
                tzPwmInit.bPwm12Init = TRUE;
                break;
            }
            assert( 0 );

        case ePWM_ID_3_4:
            if( tzPwmInit.bPwm34Init == FALSE )
            {
                tzPwmInit.bPwm34Init = TRUE;
                break;
            }
            assert( 0 );

        case ePWM_ID_5_6:
            if( tzPwmInit.bPwm56Init == FALSE )
            {
                tzPwmInit.bPwm56Init = TRUE;
                break;
            }
            assert( 0 );

        case ePWM_ID_SPV_A:
            if( tzPwmInit.bPwmSPVAInit == FALSE )
            {
                //All PWM's are used in SPV mode.  Init them all so if another mode
                //is requested, this block of PWM's won't be assigned.
                tzPwmInit.bPwm12Init   = TRUE;
                tzPwmInit.bPwm34Init   = TRUE;
                tzPwmInit.bPwm56Init   = TRUE;
                tzPwmInit.bPwmSPVAInit = TRUE;
                break;
            }
            assert( 0 );
        #endif  //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )
        case ePWM_ID_7_8:
            if( tzPwmInit.bPwm78Init == FALSE )
            {
                tzPwmInit.bPwm78Init = TRUE;
                break;
            }
            assert( 0 );

        case ePWM_ID_9_10:
            if( tzPwmInit.bPwm910Init == FALSE )
            {
                tzPwmInit.bPwm910Init = TRUE;
                break;
            }
            assert( 0 );

        case ePWM_ID_11_12:
            if( tzPwmInit.bPwm1112Init == FALSE )
            {
                tzPwmInit.bPwm1112Init = TRUE;
                break;
            }
            assert( 0 );

        case ePWM_ID_SPV_B:
            if( tzPwmInit.bPwmSPVBInit == FALSE )
            {
                //All PWM's are used in SPV mode.  Init them all so if another mode
                //is requested, this block of PWM's won't be assigned.
                tzPwmInit.bPwm78Init   = TRUE;
                tzPwmInit.bPwm910Init  = TRUE;
                tzPwmInit.bPwm1112Init = TRUE;
                tzPwmInit.bPwmSPVBInit = TRUE;
                break;
            }
            assert( 0 );
        #endif  //End #if( PWM_EN_EVB == TRUE )

        case ePWM_ID_INVL:
        case ePWM_ID_MAX:
        default:
            assert( 0 );
    }
    /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    PWM Setup Section
    Of the following three types of PWM's, you can use either of the three types
    for any EVA and mix and match.  However, since its most likely that one will
    be used, the following compile enables allow some flexibility for different
    implementations.
    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
    #if( PWM_EN_ASYMMETRIC_SETUP == TRUE )  
    /*^^^^^^^^^^^^^^^^^^^^^^^
    **Asymmetric
    ^^^^^^^^^^^^^^^^^^^^^^^*/  
    if( ptzPwm->tePwmType == PWMDRV_eTYPE_ASYMMETRIC )
    {
        //Setup the pwms for Asymmetric.
        pwmdrv_fnSetupAsymmetric( ptzPwm );
        return( eSTATUS_OK );
    }
    else
    {
        //If this is what is setup to compile then the above if statement
        //shouldn't fail.  If it does, do this.
        assert( 0 );
    }
    #endif  //End #if( PWM_EN_ASYMMETRIC_SETUP == TRUE )

    #if( PWM_EN_SYMMETRIC_SETUP == TRUE )
    /*^^^^^^^^^^^^^^^^^^^^^^^
    **Symmetric
    ^^^^^^^^^^^^^^^^^^^^^^^*/            
    if( ptzPwm->tePwmType == PWMDRV_eTYPE_SYMMETRIC )
    {
        //Setup the pwms for Symmetric.
        pwmdrv_fnSetupSymmetric( ptzPwm );
        return( eSTATUS_OK );
    }
    else
    {
        //If this is what is setup to compile then the above if statement
        //shouldn't fail.  If it does, do this.
        assert( 0 );
    }
    
    #endif  //End #if( PWM_EN_SYMMETRIC_SETUP == TRUE )

    #if( PWM_EN_SPV_SETUP == TRUE )        
    /*^^^^^^^^^^^^^^^^^^^^^^^
    **Space Vector
    ^^^^^^^^^^^^^^^^^^^^^^^*/ 
    if( ptzPwm->tePwmType == PWMDRV_eTYPE_SYMMETRIC )
    {   
        case ePWM_TYPE_SPV:
            //Setup the pwms for Space Vector.
            pwmdrv_fnSetupSpv( ptzPwm );
            return( eSTATUS_OK );
    }
    else
    {
        //If this is what is setup to compile then the above if statement
        //shouldn't fail.  If it does, do this.
        assert( 0 );
    }
    #endif  //End #if( PWM_EN_SPV_SETUP == TRUE )

    //Should never get to this point
    assert( 0 );
    //To satisfy compile warnings.
    return( eSTATUS_ERR );
}
#endif //End #if( PWM_FN_CTOR_EN == TRUE )


/*******************************************************************************

FUNCTION NAME: 
    PWMDRV_fnDtor
  
PURPOSE: 
    Destroy a PWM driver have it de-initialized and make this PWM's pins to
    be GP.

INPUTS: 
    A pointer to the users PWM structure that is to be destroyed

OUTPUTS:
    None

NOTES:
    This is a user callable function.
    
    Based on the ID from the user, de-initialize the users pwm by setting its
    init flag to FALSE and update the critical user options to be invalid
    or a safe value.
    
    Note that no faults a given here.  So if a PWM that doesn't exist or is
    messed up somehow is passed into this Dtor, the Dtor will just return
    and not do anything about it.  Hence no return type.

VERSION HISTORY:

Version: 1.00  Date: Nov 27, 2002  By: John Bellini
    - Created the stub.

Version: 1.01  Date: Jan 22, 2003  By: Jason Cumiskey
    - Capitalized pwm to conform to latest coding standard.

*******************************************************************************/
#if( defined( GLBL_DTOR_EN ) || defined( FBT ) )
void PWMDRV_fnDtor( PWMDRV_tzDATA *ptzPwm )
{
    //Switching on the ID to determine which init flag should be cleared.
    //Only want to de-init the PWM that is desired to be destroyed.
    switch( ptzPwm->tePwmId )
    {
        #if( PWM_EN_EVA == TRUE )
        case ePWM_ID_1_2:
            tzPwmInit.bPwm12Init = FALSE;   //Pwm no longer initialized.
            mDIS_PWM_PAIR_1_PINS();
            break;
            
        case ePWM_ID_3_4:
            tzPwmInit.bPwm34Init = FALSE;   //Pwm no longer initialized.
            mDIS_PWM_PAIR_2_PINS();
            break;
            
        case ePWM_ID_5_6:
            tzPwmInit.bPwm56Init = FALSE;   //Pwm no longer initialized.
            mDIS_PWM_PAIR_3_PINS();
            break;
        #endif //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )            
        case ePWM_ID_7_8:
            tzPwmInit.bPwm78Init = FALSE;   //Pwm no longer initialized.
            mDIS_PWM_PAIR_4_PINS();
            break;
            
        case ePWM_ID_9_10:
            tzPwmInit.bPwm910Init = FALSE;  //Pwm no longer initialized.
            mDIS_PWM_PAIR_5_PINS();
            break;
            
        case ePWM_ID_11_12:
            tzPwmInit.bPwm1112Init = FALSE; //Pwm no longer initialized.
            mDIS_PWM_PAIR_6_PINS();
            break;
        #endif //End #if( PWM_EN_EVB == TRUE )

        #if( PWM_EN_SPV_SETUP == TRUE )
        #if( PWM_EN_EVA == TRUE )            
        case ePWM_ID_SPV_A:
            tzPwmInit.bPwmSPVAInit = FALSE; //Pwm no longer initialized.
            break;
        #endif //End #if( PWM_EN_EVA == TRUE )
        #if( PWM_EN_EVB == TRUE )
        case ePWM_ID_SPV_B:
            tzPwmInit.bPwmSPVBInit = FALSE; //Pwm no longer initialized.
            break;
        #endif //End #if( PWM_EN_EVB == TRUE )
        #endif //End #if( PWM_EN_SPV_SETUP == TRUE )
            
        default:
            assert( 0 );
            return;
    }

    //Disable it.  
    //NOTE:  The PWMDRV_fnDis() returns an error code only if there is an Invalid 
    //       ID.  Since we are destroying this anyway, and the PWMDRV_fnDis() won't
    //       access any illegal memory, the error is ignored.  In the same 
    //       instance, if the PWM ID is invalid, the above switch statement
    //       would have returned already and not done anything.
    (void)PWMDRV_fnDis( ptzPwm );
        
    //Now that the user's PWM is unitialized, clear there ID and TYPE to make
    //sure there PWM can no longer be used.  For safe keeping, the dead band,
    //perdiod and duty cycle are all set to safe values.
    ptzPwm->tePwmId               = ePWM_ID_INVL;
    ptzPwm->tePwmType             = PWMDRV_eTYPE_INVL;
    PWM_TARGET_OPT->uiDesiredDbNs = DB_MIN_NS;
    ptzPwm->ulDesiredFreqHz       = PWM_NO_TIMER_PRD;
    ptzPwm->uiDutyCycle           = 0;                 //No duty cycle
}
#endif


/*******************************************************************************

FUNCTION NAME: 
    PWMDRV_fnDis
  
PURPOSE: 
    Disable a users PWM driver by making the output high impedance.  Not this 
    disables the entire event managers PWM's.

INPUTS: 
    A pointer to the users PWM structure.

OUTPUTS:
    tuiSTATUS

NOTES:
    The pwm still belongs to the user, its just disabled.
    
    This fn disables ALL the pwms on this event manager by turning off the 
    PWM's output(s).  In the case of SPV, it uses the SPV enable and disable 
    flags in the reg.
    
    To enabled the PWM after calling the function, use PWMDRV_fnEn
    
    Don't need to check if the PWM has been initialized in order to turn it off
    since if it is NOT initialied, its probably OFF anyway and if its not OFF
    and its NOT INITialized, then it should be disabled.
    
VERSION HISTORY:
Version: 1.00  Date: Nov 27, 2002  By: John Bellini
    - Created.

Version: 1.01  Date: Jan 22, 2003  By: Jason Cumiskey
    - Capitalized pwm to conform to latest coding standard.
    
Version: 1.10  Date: May 23, 2003  By: John Bellini
    - Removed tuiSTATUS declaration
    - Updated all cases to "break" instead of return and put one return at
      the end of the function
    - Made use of asserts where possible
    - Made use of macro's where possible

*******************************************************************************/
#if( PWM_FN_DISABLE_EN == TRUE )   //Compile Enable for this function
tuiSTATUS PWMDRV_fnDis( PWMDRV_tzDATA *ptzPwm )
{
    //Verify the user struct
    assert( ptzPwm );
    
    //Disable the users PWM based on the users PWM ID.
    switch( ptzPwm->tePwmId )
    {

        #if( PWM_EN_EVA == TRUE )
        //EVA pwm's all share the same output enable.
        case ePWM_ID_1_2:
        case ePWM_ID_3_4:
        case ePWM_ID_5_6:
            //Dis PWM compare outputs.  This clrs the FCOMPOE bit in COMCONA by
            //writing a 0 to the bit pos.  Note the '~' in front of the mask.
            mDIS_EVA_ALL();
            break;

        #endif  //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )
        //EVB pwm's all share the same output enable.    
        case ePWM_ID_7_8:
        case ePWM_ID_9_10:
        case ePWM_ID_11_12:
            
            mDIS_EVB_ALL();
            break;

        #endif  //End #if( PWM_EN_EVB == TRUE )

        #if( PWM_EN_SPV_SETUP == TRUE )
        #if( PWM_EN_EVA == TRUE )
        //EVA SPV pwm's all share the same enable.    
        case ePWM_ID_SPV_A:
            mDIS_SPVA_ALL();
            break;

        #endif //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )            
        //EVB SPV pwm's all share the same enable.
        case ePWM_ID_SPV_B:
            mDIS_SPVB_ALL();
            break;

        #endif //End #if( PWM_EN_EVB == TRUE )
        #endif //End if( PWM_EN_SPV_SETUP == TRUE )

        default:
            assert( 0 );
            break;
    }

    return( eSTATUS_OK );
}
#endif //End #if( PWM_FN_ == TRUE )


/*******************************************************************************

FUNCTION NAME: 
    PWMDRV_fnEn
  
PURPOSE: 
    Enable a users PWM by enabling there output pins.  Not this enables the
    entire event managers PWM's.

INPUTS:
    A pointer to the users PWMDRV_tzDATA structure.

OUTPUTS:
    tuiStatus

NOTES:
    This fn enables the users PWM by turning on the PWM's output(s).
    In the case of SPV, it uses the SPV enable and disable flags in the reg.
    
    To disable the PWM after calling the function, use PWMDRV_fnDis
    
    Need to make sure the PWM has been initialed before we allow it to be 
    enabled.
    
VERSION HISTORY:
Version: 1.00  Date: Nov 27, 2002  By: John Bellini
    - Created.

Version: 1.01  Date: Jan 22, 2003  By: Jason Cumiskey
    - Capitalized pwm to conform to latest coding standard.

Version: 1.10  Date: May 23, 2003  By: John Bellini
    - Removed tuiSTATUS declaration
    - Updated all cases to "break" instead of return and put one return at
      the end of the function
    - Made use of asserts where possible
    - Made use of macro's where possible
    
*******************************************************************************/
#if( PWM_FN_ENABLE_EN == TRUE )   //Compile Enable for this function
tuiSTATUS PWMDRV_fnEn( PWMDRV_tzDATA *ptzPwm )
{
    assert( ptzPwm );
    
    //Enable the users PWM based on there PWM ID.
    switch( ptzPwm->tePwmId )
    {

        #if( PWM_EN_EVA == TRUE )
        //EVA pwm's all share the same output enable.
        case ePWM_ID_1_2:
        case ePWM_ID_3_4:
        case ePWM_ID_5_6:
            assert( tzPwmInit.bPwm12Init == TRUE || 
                    tzPwmInit.bPwm34Init == TRUE ||
                    tzPwmInit.bPwm56Init == TRUE  );

            mEN_EVA_ALL();
            break;

        #endif //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )
        //EVB pwm's all share the same output enable.
        case ePWM_ID_7_8:
        case ePWM_ID_9_10:
        case ePWM_ID_11_12:
            assert( tzPwmInit.bPwm78Init   == TRUE || 
                    tzPwmInit.bPwm910Init  == TRUE ||
                    tzPwmInit.bPwm1112Init == TRUE  );

            mEN_EVB_ALL();
            break;

        #endif //#if( PWM_EN_EVB == TRUE )

        #if( PWM_EN_SPV_SETUP == TRUE )
        #if( PWM_EN_EVA == TRUE )        
        //EVA SPV pwm's all share the same enable.    
        case ePWM_ID_SPV_A:
            assert( tzPwmInit.bPwmSPVAInit == TRUE );

            mEN_SPVA_ALL();
            break;

        #endif //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )            
        //EVB SPV pwm's all share the same enable.
        case ePWM_ID_SPV_B:
            assert( tzPwmInit.bPwmSPVBInit == TRUE );

            mEN_SPVB_ALL();
            break;

        #endif //End #if( PWM_EN_EVB == TRUE )
        #endif //End if( PWM_EN_SPV_SETUP == TRUE )            

        default:
            assert( 0 );
            break;
    }

    return( eSTATUS_OK );
}
#endif //End #if( PWM_FN_ == TRUE )   //Compile Enable for this function


/*******************************************************************************

FUNCTION NAME: 
    PWMDRV_fnUpdateDutyCycle
  
PURPOSE: 
    Write/Update the PWM with a new value.  This really updates the compare reg
    that the PWM uses to generate its values.

INPUTS: 
    A pointer to the users PWM structure.

OUTPUTS:
    tuiSTATUS

NOTES:
    No need to check if the pwm has been initialized here since it doesn't
    matter if you write to this register if the pwm is not initialized.

VERSION HISTORY:
Version: 1.00  Date: Nov 27, 2002  By: John Bellini
    - Created.

Version: 1.01  Date: Jan 22, 2003  By: Jason Cumiskey
    - Capitalized pwm to conform to latest coding standard.

Version: 1.10  Date: May 23, 2003  By: John Bellini
    - Removed tuiSTATUS declaration
    - Updated all cases to "break" instead of return and put one return at
      the end of the function
    - Made use of asserts where possible
    - Removed the setting of the PWM pins to there main function since it should
      already be done.
    
*******************************************************************************/
#if( PWM_FN_UPDATE_DC_EN == TRUE )   //Compile Enable for this function
tuiSTATUS PWMDRV_fnUpdateDutyCycle( PWMDRV_tzDATA *ptzPwm )
{
    //Verify the user struct
    assert( ptzPwm );
    
    //Update the users duty cycle based on there PWM ID.
    switch( ptzPwm->tePwmId )
    {

        #if( PWM_EN_EVA == TRUE )
        case ePWM_ID_1_2:
            EvaRegs.CMPR1 = ptzPwm->uiDutyCycle;
            break;

        case ePWM_ID_3_4:
            EvaRegs.CMPR2 = ptzPwm->uiDutyCycle;
            break;

        case ePWM_ID_5_6:
            EvaRegs.CMPR3 = ptzPwm->uiDutyCycle;
            break;

        #endif //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )
        case ePWM_ID_7_8:
            EvbRegs.CMPR4 = ptzPwm->uiDutyCycle;
            break;

        case ePWM_ID_9_10:
            EvbRegs.CMPR5 = ptzPwm->uiDutyCycle;
            break;

        case ePWM_ID_11_12:
            EvbRegs.CMPR6 = ptzPwm->uiDutyCycle;
            break;

        #endif //End #if( PWM_EN_EVB == TRUE )

        #if( PWM_EN_SPV_SETUP == TRUE )
        #if( PWM_EN_EVA == TRUE )
        case ePWM_ID_SPV_A:
            assert( 0 );
            break;

        #endif //End #if( PWM_EN_EVA == TRUE )
        #if( PWM_EN_EVB == TRUE )            
        case ePWM_ID_SPV_B:
            assert( 0 );
            break;

        #endif //End #if( PWM_EN_EVB == TRUE )
        #endif //End if( PWM_EN_SPV_SETUP == TRUE )

        default:
            assert( 0 );
            break;
    }

    return( eSTATUS_OK );
}
#endif //End #if( PWM_FN_ == TRUE )

/*******************************************************************************

FUNCTION NAME: 
    PWMDRV_fnSetDeadBand
  
PURPOSE: 
    Sets the dead band based on what the user sets for a dead band value
    in there pwm structure and enables the dead band feature.

INPUTS: 
    A pointer to the users PWM structure.

OUTPUTS:
    tuiSTATUS

NOTES:
    The fn is a mask to call the real dead band calculator.  Some of the value
    for prescaler and period, the user doesn't need to know about.
    
    The returned prescaler and period masks from the dead band calculation are
    already setup to be OR'd into the dead band register, hence there is no
    "<<" shifting done as there is for other variables.

VERSION HISTORY:

Version: 1.00  Date: Nov 25, 2002  By: John Bellini
    - Created.
    
Version: 1.10  Date: Dec 20, 2002  By: John Bellini
    - Created a En and Dis dead band functions.  This function now calls
      En dead band and the code that was here to do that was moved to the 
      EnDeadBand function.

Version: 1.11  Date: Jan 22, 2003  By: Jason Cumiskey
    - Capitalized pwm to conform to latest coding standard.
    
Version: 1.12  Date: Feb 14, 2003  By: John Bellini
    - Function didn't return a parameter.  Added return( eSTATUS_OK );
      This was found by lint.

Version: 1.20  Date: May 23, 2003  By: John Bellini
    - Removed tuiSTATUS declaration
    - Made use of asserts where possible
    - Removed some checking of returned values based on use of asserts

*******************************************************************************/
#if( PWM_FN_SET_DB_EN == TRUE )   //Compile Enable for this function
tuiSTATUS PWMDRV_fnSetDeadBand( PWMDRV_tzDATA *ptzPwm )
{
    //Make sure the parameter is cool
    assert( ptzPwm );
    //Make sure the user has set there own target options up
    assert( ptzPwm->ptzPwmOpt );
    
    //Check limits on the dead band value.
    //If the value for DB_MIN_NS is zero (0) then the else if(..) isn't really 
    //needed since these cases deal with unsigned values.  However, in order to 
    //accomodate change, the check is left in.
    assert(PWM_TARGET_OPT->uiDesiredDbNs <= PWMDRV_mMAX_DEADBAND_NS() );
    assert(PWM_TARGET_OPT->uiDesiredDbNs >= (uint16)DB_MIN_NS );

    
    //First step is to calculate the dead band based on what the user has in
    //their pwm struct.  This function will provide the proper prescaler and
    //period value to write to the dead band registers.    
    //Don't care about the return because I checked the values with the above
    //asserts and so this function will always return proper values.
    (void)pwmdrv_fnCalcDeadBand( PWM_TARGET_OPT->uiDesiredDbNs );

    ///Enable the dead band feature.
    //Don't care about the return because is alway returns eSTATUS_OK
    (void)PWMDRV_fnEnDeadBand( ptzPwm );
    
    //To complete the API
    return( eSTATUS_OK );
}
#endif  //#if( PWM_FN_SET_DB_EN == TRUE )


/*==============================================================================
                            Private Driver Functions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    pwmdrv_fnSetComCon
  
PURPOSE: 
    Writes the appropriate values to the COMCONA register

INPUTS: 
    Users Pwm structure

OUTPUTS:
    None

NOTES:
    Initially implemented to save code space so this function can be called
    instead of repeating the code.

VERSION HISTORY:

Version: 1.00  Date: Nov 27, 2002  By: John Bellini
    - Created

*******************************************************************************/
#if( PWM_FN_SET_COM_CON_EN == TRUE )   //Compile Enable for this function
void pwmdrv_fnSetComCon( PWMDRV_tzDATA *ptzPwm )
{
    switch( ptzPwm->tePwmId )
    {

        #if( PWM_EN_EVA == TRUE )    
        case ePWM_ID_1_2:
        case ePWM_ID_3_4:
        case ePWM_ID_5_6:
            //The following handles the Compare Enable, Reload condition,
            //Disables the SPV operation, setups the Action reload option and
            //makes sure the PWM's are disabled (in this order as you read down)
            EvaRegs.COMCONA.all = ( 
                   ( PWM_TARGET_OPT->teCompEn << F28X_COMCON_POS_CENABLE )
                 | ( PWM_TARGET_OPT->teCompReload << F28X_COMCON_POS_CLD0 )
                 | ( F28X_COMCON_SVENABLE_DIS )
                 | ( PWM_TARGET_OPT->teActionReload << F28X_COMCON_POS_ACTRLD0 )
                 | ( F28X_COMCON_FCMPOE_DIS ) );
            return;

        #endif //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )            
        case ePWM_ID_7_8:
        case ePWM_ID_9_10:
        case ePWM_ID_11_12:
            //The following handles the Compare Enable, Reload condition,
            //Disables the SPV operation, setups the Action reload option and
            //makes sure the PWM's are disabled (in this order as you read down)
            EvbRegs.COMCONB.all = ( 
                   ( PWM_TARGET_OPT->teCompEn << F28X_COMCON_POS_CENABLE )
                 | ( PWM_TARGET_OPT->teCompReload << F28X_COMCON_POS_CLD0 )
                 | ( F28X_COMCON_SVENABLE_DIS )
                 | ( PWM_TARGET_OPT->teActionReload << F28X_COMCON_POS_ACTRLD0 )
                 | ( F28X_COMCON_FCMPOE_DIS  ) );
            return;

        #endif //End #if( PWM_EN_EVB == TRUE )            

        default:
            assert( 0 );  
            return;
    }
}
#endif //End #if( PWM_FN__EN == TRUE )


/*******************************************************************************

FUNCTION NAME: 
    pwmdrv_fnSetupSymmetric
  
PURPOSE: 
    This fn kicks off the setup for a symmetrical type of the PWM scheme.
    The setup occurs based on the ID which shows if the PWM's are part of EVA
    or EVB.

INPUTS: 
    A pointer to a user PWMDRV_tzDATA structure.

OUTPUTS:
    tuiSTATUS

NOTES:
    This code is only included if this driver has been setup to use Symmetric
    PWMing by setting the following define to TRUE:  PWM_EN_SYMMETRIC_SETUP
    
    Each PWM can have a different setup (ACTIVE HIGH, LOW, FORCED HIGH, LOW,
    COMPARE EN, etc).  In order to accomodate this flexiblity in this driver,
    some common code that is common to an Event manager, but not individual
    PWM's needed to be moved to functions.  This saves some code space and
    improves readability.
    
    The ACTRx registers cannot be written to if the COMCONx Bit 15 Compare 
    Enable is ON or ENABLED.  COMCONx Bit 15 MUST be 0 (OFF, DISABLED) in order
    to write to ACTRx.  Hence the order of writing to these registers is
    important.
    
    Writing to COMCONx is the same for an event manager.  In order to save some
    space, it was moved to a function.  However, it is ONLY EFFICIENT if more
    than one PWM is being used, otherwise, you might as well, copy and paste it
    where the function call is made.
    
    Timer is created external to the PWM driver.  Hence, the timer needs to be
    created before this is called.

VERSION HISTORY:

Version: 1.00  Date: Nov 28, 2002  By: John Bellini
    - Created.
    
Version: 1.10  Date: Dec 20, 2002  By: John Bellini
    - Removed timer check.

Version: 1.20  Date: May 24, 2003  By: John Bellini
    - Made use of asserts where possible
    - Change function API so return type is void
    - Moved call to pwmdrv_fnSetComCon( ptzPwm ) from each case to end of fn
    - Removed variable for SPV operation
    - Update the settings of ACTRA to save some space
    
Version: 1.30  Date: Nov 4, 2003   By: John Bellini
    - PRN570:  Added the disabling of the COMCONA registers CVENABLE bit 
      before writing to the ACTR register to make sure the write will be 
      successful.
      
Version: 1.40  Date: Nov 6, 2003   By: John Bellini
    - PRN606:  While testing the fixes for this PRN, it was noted that
      forcing PIN2 low and PIN1 Active Low did not work.  Pwm's 1 and 3 were
      not switching.  However, forcing PIN2 low and PIN1 Active High did work.
      What was happening is before writing to ACTRA, the desired bits are
      cleared to zero first.  Well only 2 of the 4 bits where being cleared
      since this feature uses 2 bits per pin.  So the clearing of the bits
      needed for the option to be set has been updated.

*******************************************************************************/
#if( PWM_FN_SETUP_SYM_EN == TRUE )   //Compile Enable for this function
void pwmdrv_fnSetupSymmetric( PWMDRV_tzDATA *ptzPwm )
{
    //Depending on the ID, some of the registers are different event managers.
    switch( ptzPwm->tePwmId )
    {
        #if( PWM_EN_EVA == TRUE )
        case ePWM_ID_1_2:
            // Compare Action Control Register A
            // Actual pin outputs to the bridge.  Note that the order of here
            // is important.  You CANNOT write to the ACTRA register if the
            // compare enable is ENABLED in COMCONx bit 15.  Therefore, make 
            // sure its disabled before we write to ACTRx.  It will be enabled
            // later.
            EvaRegs.COMCONA.all &= ~F28X_COMCON_MSK_CENABLE;

            //Make the bit locations that will be updated zero in order
            //to be properly updated by the next line.  This needs to be done
            //since we don't know whether the values being set are a zero or a
            //one.
            EvaRegs.ACTRA.all &=  ~( ( 1 << F28X_ACTR_POS_CMP2ACT1 ) |
                                     ( 1 << F28X_ACTR_POS_CMP2ACT0 ) |
                                     ( 1 << F28X_ACTR_POS_CMP1ACT1 ) |
                                     ( 1 << F28X_ACTR_POS_CMP1ACT0 ) );

            //The bit locations for the following must be cleared (zero) before
            //ORing in the desired values.  This is necessary since it is 
            //not known what values are being written to this bit locations.
            EvaRegs.ACTRA.all |=  ( 
                    ( PWM_TARGET_OPT->tePin2State << F28X_ACTR_POS_CMP2ACT0 ) |
                    ( PWM_TARGET_OPT->tePin1State << F28X_ACTR_POS_CMP1ACT0 ) );

            //Setup the pins to be PWM's
            mEN_PWM_PAIR_1_PINS();
            break;
                      
        case ePWM_ID_3_4:
            // Compare Action Control Register A
            // Actual pin outputs to the bridge.  Note that the order of here
            // is important.  You CANNOT write to the ACTRA register if the
            // compare enable is ENABLED in COMCONx bit 15.  Therefore, make 
            // sure its disabled before we write to ACTRx.  It will be enabled
            // later.
            EvaRegs.COMCONA.all &= ~F28X_COMCON_MSK_CENABLE;

            //Make the bit locations that will be updated equal to zero in order
            //to be properly updated by the next line.
            EvaRegs.ACTRA.all &= ~( ( 1 << F28X_ACTR_POS_CMP4ACT1 ) |
                                    ( 1 << F28X_ACTR_POS_CMP4ACT0 ) | 
                                    ( 1 << F28X_ACTR_POS_CMP3ACT1 ) |
                                    ( 1 << F28X_ACTR_POS_CMP3ACT0 ) );

            EvaRegs.ACTRA.all |=  ( 
                    ( PWM_TARGET_OPT->tePin2State << F28X_ACTR_POS_CMP4ACT0 ) | 
                    ( PWM_TARGET_OPT->tePin1State << F28X_ACTR_POS_CMP3ACT0 ) );

            //Setup the pins to be PWM's
            mEN_PWM_PAIR_2_PINS();
            break;

        case ePWM_ID_5_6:
            // Compare Action Control Register A
            // Actual pin outputs to the bridge.  Note that the order of here
            // is important.  You CANNOT write to the ACTRA register if the
            // compare enable is ENABLED in COMCONx bit 15.  Therefore, make 
            // sure its disabled before we write to ACTRx.  It will be enabled
            // later.
            EvaRegs.COMCONA.all &= ~F28X_COMCON_MSK_CENABLE;

            //Make the bit locations that will be update equal to zero in order
            //to be properly updated by the next line.
            EvaRegs.ACTRA.all &= ~( ( 1 << F28X_ACTR_POS_CMP6ACT1 ) |
                                    ( 1 << F28X_ACTR_POS_CMP6ACT0 ) | 
                                    ( 1 << F28X_ACTR_POS_CMP5ACT1 ) |
                                    ( 1 << F28X_ACTR_POS_CMP5ACT0 ) );

            EvaRegs.ACTRA.all |=  ( 
                    ( PWM_TARGET_OPT->tePin2State << F28X_ACTR_POS_CMP6ACT0 ) | 
                    ( PWM_TARGET_OPT->tePin1State << F28X_ACTR_POS_CMP5ACT0 ) );

            //Setup the pins to be PWM's            
            mEN_PWM_PAIR_3_PINS();
            break;
        #endif // End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )
        case ePWM_ID_7_8:
            // Compare Action Control Register B
            // Actual pin outputs to the bridge.  Note that the order of here
            // is important.  You CANNOT write to the ACTRA register if the
            // compare enable is ENABLED in COMCONx bit 15.  Therefore, make 
            // sure its disabled before we write to ACTRx.  It will be enabled
            // later.
            EvbRegs.COMCONB.all &= ~F28X_COMCON_MSK_CENABLE;

            //Make the bit locations that will be update equal to zero in order
            //to be properly updated by the next line.
            EvbRegs.ACTRB.all &= ~( ( 1 << F28X_ACTR_POS_CMP8ACT1 ) |
                                    ( 1 << F28X_ACTR_POS_CMP8ACT0 ) |
                                    ( 1 << F28X_ACTR_POS_CMP7ACT1 ) |
                                    ( 1 << F28X_ACTR_POS_CMP7ACT0 ) ); 

            EvbRegs.ACTRB.all |=  ( 
                    ( PWM_TARGET_OPT->tePin2State << F28X_ACTR_POS_CMP8ACT0 ) |
                    ( PWM_TARGET_OPT->tePin1State << F28X_ACTR_POS_CMP7ACT0 ) );

            //Setup the pins to be PWM's
            mEN_PWM_PAIR_4_PINS();
            break;

        case ePWM_ID_9_10:
            // Compare Action Control Register B
            // Actual pin outputs to the bridge.  Note that the order of here
            // is important.  You CANNOT write to the ACTRA register if the
            // compare enable is ENABLED in COMCONx bit 15.
            EvbRegs.COMCONB.all &= ~F28X_COMCON_MSK_CENABLE;

            //Make the bit locations that will be update equal to zero in order
            //to be properly updated by the next line.
            EvbRegs.ACTRB.all &= ~( ( 1 << F28X_ACTR_POS_CMP10ACT1 ) |
                                    ( 1 << F28X_ACTR_POS_CMP10ACT0 ) |
                                    ( 1 << F28X_ACTR_POS_CMP9ACT1 ) |
                                    ( 1 << F28X_ACTR_POS_CMP9ACT0 ) ); 

            EvbRegs.ACTRB.all |=  ( 
                    ( PWM_TARGET_OPT->tePin2State << F28X_ACTR_POS_CMP10ACT0 ) |
                    ( PWM_TARGET_OPT->tePin1State << F28X_ACTR_POS_CMP9ACT0 ) );

            //Setup the pins to be PWM's
            mEN_PWM_PAIR_5_PINS();
            break;

        case ePWM_ID_11_12:
            // Compare Action Control Register B
            // Actual pin outputs to the bridge.  Note that the order of here
            // is important.  You CANNOT write to the ACTRA register if the
            // compare enable is ENABLED in COMCONx bit 15.Therefore, make 
            // sure its disabled before we write to ACTRx.  It will be enabled
            // later.
            EvbRegs.COMCONB.all &= ~F28X_COMCON_MSK_CENABLE;

            //Make the bit locations that will be update equal to zero in order
            //to be properly updated by the next line.
            EvbRegs.ACTRB.all &= ~( ( 1 << F28X_ACTR_POS_CMP12ACT1 ) |
                                    ( 1 << F28X_ACTR_POS_CMP12ACT0 ) |
                                    ( 1 << F28X_ACTR_POS_CMP11ACT1 ) |
                                    ( 1 << F28X_ACTR_POS_CMP11ACT0 ) ); 

            EvbRegs.ACTRB.all |=  ( 
                   ( PWM_TARGET_OPT->tePin2State << F28X_ACTR_POS_CMP12ACT0 ) |
                   ( PWM_TARGET_OPT->tePin1State << F28X_ACTR_POS_CMP11ACT0 ) );

            //Setup the pins to be PWM's
            mEN_PWM_PAIR_6_PINS();
            break;

        #endif //End #if( PWM_EN_EVB == TRUE )

        #if( PWM_EN_SPV_SETUP == TRUE )
        #if( PWM_EN_EVA == TRUE )
        //When setting up a symmetric PWM, the ID cannot be for a SPV PWM.
        case ePWM_ID_SPV_A:
            assert( 0 );
            break;
        #endif //End #if( PWM_EN_EVA == TRUE )
        #if( PWM_EN_EVB == TRUE )        
        case ePWM_ID_SPV_B:
            assert( 0 );
            break;
        #endif //End #if( PWM_EN_EVB == TRUE )
        #endif        
        default:
            assert( 0 );
            break;
    }

    //Setup a common register.  This register is common to all PWM pairs
    //for each EV.
    pwmdrv_fnSetComCon( ptzPwm );
}

#endif //End #if( PWM_FN__EN == TRUE )


/*******************************************************************************

FUNCTION NAME: 
    pwmdrv_fnSetupAsymmetric
  
PURPOSE: 
    Performs the setup operations needed to create and setup a Asymmetric
    pwming scheme.

INPUTS: 
    A pointer to the users tzPMW structure with specific data.

OUTPUTS:
    tuiSTATUS

NOTES:
    This code is only included if this driver has been setup to use Symmetric
    PWMing by setting the following define to TRUE:  PWM_EN_ASYMMETRIC_SETUP

VERSION HISTORY:

Version: 0.01  Date: Nov 6 2002  By: John Bellini
    - Created

Version: 1.00  Date: May 24, 2003  By: John Bellini
    - Made use of asserts where possible
    - Change function API so return type is void

*******************************************************************************/
#if( PWM_FN_SETUP_ASYM_EN == TRUE )   //Compile Enable for this function
void pwmdrv_fnSetupAsymmetric( PWMDRV_tzDATA *ptzPwm )
{
    //Depending on the ID, some of the registers are different event managers.
    switch( ptzPwm->tePwmId )
    {

        #if( PWM_EN_EVA == TRUE )    
        case ePWM_ID_1_2:
        case ePWM_ID_3_4:
        case ePWM_ID_5_6:
            break;
        #endif //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )
        case ePWM_ID_7_8:
        case ePWM_ID_9_10:
        case ePWM_ID_11_12:
            break;
        #endif //End #if( PWM_EN_EVB == TRUE )

        #if( PWM_EN_SPV == TRUE )
        #if( PWM_EN_EVA == TRUE )
        case ePWM_ID_SPV_A:
        #endif //End #if( PWM_EN_EVA == TRUE )
        #if( PWM_EN_EVB == TRUE )
        case ePWM_ID_SPV_B:
        #endif //End #if( PWM_EN_EVB == TRUE )
        #endif
        default:
            assert( 0 );
            break;
    }
}
#endif //End #if( PWM_FN__EN == TRUE )


/*******************************************************************************

FUNCTION NAME: 
    pwmdrv_fnSetupSpv
  
PURPOSE: 
    Performs the setup operations needed to create and setup a Space Vector
    pwming scheme.

INPUTS: 
    A pointer to the users tzPMW structure with specific data.

OUTPUTS:
    void

NOTES:
    This code is only included if this driver has been setup to use Symmetric
    PWMing by setting the following define to TRUE:  PWM_EN_ASYMMETRIC_SETUP

VERSION HISTORY:

Version: 0.01  Date: Nov 6 2002  By: John Bellini
    - Created

Version: 1.00  Date: May 24, 2003 By: John Bellini
    - Made use of assert where possible
    - Changed return type to be void

*******************************************************************************/
#if( PWM_FN_SETUP_SPV_EN == TRUE )   //Compile Enable for this function
void pwmdrv_fnSetupSpv( PWMDRV_tzDATA *ptzPwm )
{
    //Depending on the ID, some of the registers are different event managers.
    switch( ptzPwm->tePwmId )
    {
        #if( PWM_EN_EVA == TRUE )    

        case ePWM_ID_SPV_A:
            break;

        #endif //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )
            
        case ePWM_ID_SPV_B:
            break;

        #endif //#if( PWM_EN_EVB == TRUE )
            
        //When setting up a SPV PWM, you cannot have an ID for pwm pairs 1 
        //through 12
        case ePWM_ID_1_2:
        case ePWM_ID_3_4:
        case ePWM_ID_5_6:
        case ePWM_ID_7_8:
        case ePWM_ID_9_10:
        case ePWM_ID_11_12:
        default:
            assert( 0 );
            break;
    }
}
#endif //End #if( PWM_FN__EN == TRUE )   //Compile Enable for this function

/******************************************************************************\

FUNCTION NAME: 
    pwmdrv_fnCalcDeadBand

PURPOSE:
    Take into consideration the DSP clock speed and the prescalers to calculate
    the required masks for PWM dead-band setup.  The reasulting dead-band will
    be the desired ns (nanoseconds). This function tries to maximize on the
    period instead of the prescaler to improve accuracy.

INPUTS:
    uiDeadBandNs - Desired dead-band time in nanoseconds

OUTPUTS:
    None

    Modified via pointers:
    puiDbPres - Dead-band prescaler mask necessary to achieve dead-band time
    puiDbPrd  - Dead-band period mask necessary to achieve dead-band time

NOTES:
    Assumption here is that only TI's TMS320LF2xxx devices are used.
    This forces the bit masks to be constant and prescaler options to be
    constant.

    Assumes "DeadBandNs" is already verified before this function is called
    since this function is local to this package.

VERSION HISTORY:
Version: 3.00   Date:  Oct 31 2002  By: John Bellini
    - Borrow from XDI project version 2.02
    - Added calls to get system info like clock, prescaler, etc

Version: 3.01   Date:  Jan 22 2003  By: Jason Cumiskey
    - Removed prefixes on local variables to conform to latest coding standard.

Version: 3.02   Date:  Feb 25 2003  By: Jason Cumiskey
    - Changed call of syslvl_fnGetClkSpeed to SYSLVL_fnGetClkSpeed

Version: 3.10   Date:  May 24, 2003 By: John Bellini
    - Made use of assert where possible
    - Removed prefixing on local variables
    - Improved use of sizeof() for calc of PRESCALER_NUM
    - Made return type void
    - Removed passing of DbPres and DbPrd

Version: 3.20   Date:  June 2, 2003  By: John Bellini
    - Removed const arrays used to determine the prescaler mask.
    - Replaced used of const arrays with shifts to save some code space and 
      executiont time.
    - Changed TempPeriod from float32 to uint16
    - Added define and undefine for NUM_PRES_OPTIONS

Version: 3.21   Date: June 30, 2003  By: John Bellini
    - changed BITPOS to XT_BITPOS
    
Version: 3.22   Date: Jan 24, 2005  By: John Bellini
    - Changed INIT_fnGetClkSpeed() to SYSCLK_fnGetSysClk().
    
\******************************************************************************/
#if( PWM_FN_CALC_DB_EN == TRUE )   //Compile Enable for this function
void pwmdrv_fnCalcDeadBand( uint16 DeadBandNs )   //Dead-band value
{

#define NUM_PRES_OPTIONS    ( 6 )       //The number of options available.

    /*^^^^^^^^^^
    Declarations
    ^^^^^^^^^^*/
    float32 PresTarget;       //Target prescaler
    uint16  TempPeriod;
    uchar8  Count = 0;            //Local GP counter

    uint32 DspClk = SYSCLK_fnGetSysClk(); //Clk speed req. for DB calcs.
    assert( DspClk );                       //Make sure the clock is not zero
   
    /*^^^^^^^^^^^^^
    Calc Prescaler
    ^^^^^^^^^^^^^^*/
    //Calculate the dead-band timer prescaler mask
    //When reviewing the data sheet, there is a table which shows how to set the
    //values of prescaler and period to get the desired dead-band time.  To get
    //the best resolution, you want to make your way through the table by going 
    //down and to the right as much as possible.  This part is going down the 
    //columns in the table as much as possible.
    //ulPresTarget = ( ( fDeadBandNs / 1,000,000,000 ) * CLKOUT )
    //               /           Maximum period
    PresTarget  =   ( float32 )DspClk;
    PresTarget *= ( ( float32 )DeadBandNs ) / DB_ONE_BILLION;
    //Save this for the period calc below.
    TempPeriod  = PresTarget; 
    PresTarget /= DB_MAX_PERIOD_SCALER;

    // Find the highest resolution prescaler that can be used to achieve this
    // dead-band.
    while( ( PresTarget > ( 1 << Count ) ) && ( Count < NUM_PRES_OPTIONS ) )
    {
        Count++;
    }

    //Save the prescaler mask.  Mask starts at bit position 2.
    DbPrescMsk = ( Count << F28X_DBTCON_POS_DBTPS0 );

    /*^^^^^^^^^^^^^
    Calc Period
    ^^^^^^^^^^^^^^*/
    // Using the calculated prescaler, find the exact period necessary
    // to get the desired frequency.
    TempPeriod >>= Count;

    //If the value exceeds the allowable period, cap it and return an error.
    assert( TempPeriod <= DB_MAX_PERIOD_SCALER );
    
    //If the above assert passes, then all is good, so write the value.
    DbPrdMsk = TempPeriod << F28X_DBTCON_POS_DBT0;
    
#undef NUM_PRES_OPTIONS
}
#endif //End #if( PWM_FN__EN == TRUE )


/*******************************************************************************

FUNCTION NAME: 
    PWMDRV_fnEnDeadBand
  
PURPOSE: 
    Enable the dead band feature of the PWM's.

INPUTS: 
    Users PWM struct

OUTPUTS:
    Status of the functions operation.

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: Dec 20, 2002  By: John Bellini
    -  Created

Version: 1.01  Date: Jan 22, 2003  By: Jason Cumiskey
    - Capitalized pwm to conform to latest coding standard.

Version: 1.10  Date: May 24, 2003  By: John Bellini
    - Made use of assert
    - Made all cases break instead of return and put one return at the end
      of the function

*******************************************************************************/
#if( PWM_FN_ENABLE_DB_EN == TRUE )  //Function compile enable
tuiSTATUS PWMDRV_fnEnDeadBand( PWMDRV_tzDATA *ptzPwm )
{
    assert( ptzPwm );

    //Different timers for each pwm pair are setup for the user based
    //on their PWM ID.  Setup the dead band depending on the ID of the pwm
    //who called this function.
    switch( ptzPwm->tePwmId )
    {
        #if( PWM_EN_EVA == TRUE )
        case ePWM_ID_1_2:
            // Dead-Band timer Control Register A
            //The bit locations for the following must be cleared (zero) before
            //ORing in the desired values.  This is necessary since it is 
            //not known what values are being written to this bit locations.
            EvaRegs.DBTCONA.all &= ~( F28X_DBTCON_MSK_DBTPS |
                                      F28X_DBTCON_EDBT1_EN |
                                      F28X_DBTCON_DBT_15 );

            //Now that the bit locations are cleared, we write to them safely.
            //So anything that is suppose to be zero, stays as zero while
            //anything that is suppose to be a one, is set to a one.
            EvaRegs.DBTCONA.all |= ( DbPrescMsk |
                        ( PWM_TARGET_OPT->teDbTmrEn << F28X_DBTCON_POS_EDBT1 ) |
                        DbPrdMsk );
            break;

        case ePWM_ID_3_4:
            // Dead-Band timer Control Register A
            //The bit locations for the following must be cleared (zero) before
            //ORing in the desired values.  This is necessary since it is 
            //not known what values are being written to this bit locations.
            EvaRegs.DBTCONA.all &= ~( F28X_DBTCON_MSK_DBTPS |
                                      F28X_DBTCON_EDBT2_EN |
                                      F28X_DBTCON_DBT_15 );

            //Now that the bit locations are cleared, we write to them safely.
            //So anything that is suppose to be zero, stays as zero while
            //anything that is suppose to be a one, is set to a one.
            EvaRegs.DBTCONA.all |= ( DbPrescMsk |
                        ( PWM_TARGET_OPT->teDbTmrEn << F28X_DBTCON_POS_EDBT2 ) |
                        DbPrdMsk );
            break;

        case ePWM_ID_5_6:
            // Dead-Band timer Control Register A
            //The bit locations for the following must be cleared (zero) before
            //ORing in the desired values.  This is necessary since it is 
            //not known what values are being written to this bit locations.
            EvaRegs.DBTCONA.all &= ~( F28X_DBTCON_MSK_DBTPS |
                                      F28X_DBTCON_EDBT3_EN |
                                      F28X_DBTCON_DBT_15 );

            //Now that the bit locations are cleared, we write to them safely.
            //So anything that is suppose to be zero, stays as zero while
            //anything that is suppose to be a one, is set to a one.    
            EvaRegs.DBTCONA.all |= ( DbPrescMsk |
                        ( PWM_TARGET_OPT->teDbTmrEn << F28X_DBTCON_POS_EDBT3 ) |
                        DbPrdMsk );
            break;
        #endif //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )
        case ePWM_ID_7_8:
            // Dead-Band timer Control Register B
            //The bit locations for the following must be cleared (zero) before
            //ORing in the desired values.  This is necessary since it is 
            //not known what values are being written to this bit locations.
            EvbRegs.DBTCONB.all &= ~( F28X_DBTCON_MSK_DBTPS |
                                      F28X_DBTCON_EDBT1_EN |
                                      F28X_DBTCON_DBT_15 );

            //Now that the bit locations are cleared, we write to them safely.
            //So anything that is suppose to be zero, stays as zero while
            //anything that is suppose to be a one, is set to a one.    
            EvbRegs.DBTCONB.all |= ( DbPrescMsk |
                        ( PWM_TARGET_OPT->teDbTmrEn << F28X_DBTCON_POS_EDBT1 ) |
                        DbPrdMsk );
            break;

        case ePWM_ID_9_10:
            // Dead-Band timer Control Register B
            //The bit locations for the following must be cleared (zero) before
            //ORing in the desired values.  This is necessary since it is 
            //not known what values are being written to this bit locations.
            EvbRegs.DBTCONB.all &= ~( F28X_DBTCON_MSK_DBTPS |
                                      F28X_DBTCON_EDBT2_EN |
                                      F28X_DBTCON_DBT_15 );

            //Now that the bit locations are cleared, we write to them safely.
            //So anything that is suppose to be zero, stays as zero while
            //anything that is suppose to be a one, is set to a one.
            EvbRegs.DBTCONB.all |= ( DbPrescMsk |
                        ( PWM_TARGET_OPT->teDbTmrEn << F28X_DBTCON_POS_EDBT2 ) |
                        DbPrdMsk );
            break;

        case ePWM_ID_11_12:
            // Dead-Band timer Control Register B
            //The bit locations for the following must be cleared (zero) before
            //ORing in the desired values.  This is necessary since it is 
            //not known what values are being written to this bit locations.
            EvbRegs.DBTCONB.all &= ~( F28X_DBTCON_MSK_DBTPS |
                                      F28X_DBTCON_EDBT3_EN |
                                      F28X_DBTCON_DBT_15 );

            //Now that the bit locations are cleared, we write to them safely.
            //So anything that is suppose to be zero, stays as zero while
            //anything that is suppose to be a one, is set to a one.
            EvbRegs.DBTCONB.all |= ( DbPrescMsk |
                        ( PWM_TARGET_OPT->teDbTmrEn << F28X_DBTCON_POS_EDBT3 ) |
                        DbPrdMsk );
            break;

        #endif //End #if( PWM_EN_EVB == TRUE )

        default:
            assert( 0 );
            break;
    }

    return( eSTATUS_OK );
}
#endif

/*******************************************************************************

FUNCTION NAME: 
    PWMDRV_fnDisDeadBand
  
PURPOSE: 
    Disable the dead band feature of the PWM's.

INPUTS: 
    Users PWM struct

OUTPUTS:
    Status of the functions operation.

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: Dec 20, 2002  By: John Bellini
    -  Created

Version: 1.01  Date: Jan 22, 2003  By: Jason Cumiskey
    - Capitalized pwm to conform to latest coding standard.

Version: 1.10  Date: May 24, 20033  By: John Bellini
    - Made use of assert
    - Made use of macro's

*******************************************************************************/
#if( PWM_FN_DISABLE_DB_EN == TRUE )  //Function compile enable
tuiSTATUS PWMDRV_fnDisDeadBand( PWMDRV_tzDATA *ptzPwm )
{
    assert( ptzPwm );
    
    //Different timers for each pwm pair are setup for the user based
    //on their PWM ID.  Setup the dead band depending on the ID of the pwm
    //who called this function.
    switch( ptzPwm->tePwmId )
    {

        #if( PWM_EN_EVA == TRUE )
        case ePWM_ID_1_2:
            //Disable deadband
            mDIS_DB_PWM_PAIR_1();
            break;

        case ePWM_ID_3_4:
            //Disable deadband
            mDIS_DB_PWM_PAIR_2();
            break;

        case ePWM_ID_5_6:
            //Disable deadband
            mDIS_DB_PWM_PAIR_3();
            break;

        #endif //End #if( PWM_EN_EVA == TRUE )

        #if( PWM_EN_EVB == TRUE )
        case ePWM_ID_7_8:
            //Disable deadband
            mDIS_DB_PWM_PAIR_4();
            break;

        case ePWM_ID_9_10:
            //Disable deadband
            mDIS_DB_PWM_PAIR_5();
            break;

        case ePWM_ID_11_12:
            //Disable deadband
            mDIS_DB_PWM_PAIR_6();
            break;

        #endif //End #if( PWM_EN_EVB == TRUE )

        default:
            assert( 0 );
            break;
    }

    return( eSTATUS_OK );
}
#endif

/*******************************************************************************

FUNCTION NAME: 
    PWMDRV_fnSetOpt
  
PURPOSE: 
    Update the options for the PWM as per what's in the option pointer
    which is located within the users PWM structure.

INPUTS: 
    User's PWM structure.

OUTPUTS:
    tuiSTATUS - eSTATUS_ERR if not successful
              - eSTATUS_OK  if successful

NOTES:
    None

VERSION HISTORY:

Version: 1.00    Date: Nov 5, 2003    By: John Bellini
    - Created for PRN570

*******************************************************************************/
tuiSTATUS PWMDRV_fnSetOpt( PWMDRV_tzDATA *ptzPwm )
{
    //Make sure the users pointer is valid.
    //
    //Assert is used in case the return parameter is not checked.
    assert( ptzPwm );
    //Run time check to make sure the users pointer is valid.
    if( ptzPwm == ( void * )0           ||
        ptzPwm->tePwmId == ePWM_ID_INVL || //(redundant, same as previous test?)
        ptzPwm->tePwmId >= ePWM_ID_MAX  )
    {
        return( eSTATUS_ERR );
    }

    #if( PWM_EN_SYMMETRIC_SETUP == TRUE )
    /*^^^^^^^^^^^^^^^^^^^^^^^
    **Symmetric
    ^^^^^^^^^^^^^^^^^^^^^^^*/            
    if( ptzPwm->tePwmType == PWMDRV_eTYPE_SYMMETRIC )
    {
        //Setup all the options for 
        pwmdrv_fnSetupSymmetric( ptzPwm );
        return( eSTATUS_OK );
    }
    else
    {
        //If this is what is setup to compile then the above if statement
        //shouldn't fail.  If it does, do this.
        assert( 0 );
    }
    #endif  //End #if( PWM_EN_SYMMETRIC_SETUP == TRUE )

    #if( PWM_EN_ASYMMETRIC_SETUP == TRUE )  
    /*^^^^^^^^^^^^^^^^^^^^^^^
    **Asymmetric
    ^^^^^^^^^^^^^^^^^^^^^^^*/  
    if( ptzPwm->tePwmType == PWMDRV_eTYPE_ASYMMETRIC )
    {
        //Setup all the options for 
        pwmdrv_fnSetupAsymmetric( ptzPwm );
        return( eSTATUS_OK );
    }
    else
    {
        //If this is what is setup to compile then the above if statement
        //shouldn't fail.  If it does, do this.
        assert( 0 );
    }
    #endif

    #if( PWM_EN_SPV_SETUP == TRUE )        
    /*^^^^^^^^^^^^^^^^^^^^^^^
    **Space Vector
    ^^^^^^^^^^^^^^^^^^^^^^^*/ 
    if( ptzPwm->tePwmType == PWMDRV_eTYPE_SYMMETRIC )
    {   
        case ePWM_TYPE_SPV:
            pwmdrv_fnSetupSpv( ptzPwm );
            return( eSTATUS_OK );
    }
    else
    {
        //If this is what is setup to compile then the above if statement
        //shouldn't fail.  If it does, do this.
        assert( 0 );
    }
    #endif  //End #if( PWM_EN_SPV_SETUP == TRUE )

    return( eSTATUS_OK );
}
