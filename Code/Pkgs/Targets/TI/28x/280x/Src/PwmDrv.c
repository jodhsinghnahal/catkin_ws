/*==============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  
    PwmDrv.c
    
PURPOSE:
    PWM driver interface for Texas Instruments 2808 processor. This driver is
    specific to the 2808, and it is project specific. The feature set available
    on this DSP is too complex to create a code efficient generic PWM driver. 
    As such, the driver sets up each PWM module according to the project 
    requirements. It also provides public access functions for common tasks like
    setting compare values, and enabling and disabling PWM outputs. 
    
FUNCTION(S):
    PWMDRV_fnCtor                 - Create the driver
    PWMDRV_fnDtor                 - Destroy the driver
    PWMDRV_fnEventDis             - Disable a PWM triggered event    
    PWMDRV_fnEventEn              - Enable a PWM triggered event
    PWMDRV_fnGetCompareA          - Get the CMPA value
    PWMDRV_fnGetCompareB          - Get the CMPB value
    PWMDRV_fnGetCompareAHiRes     - Get the CMPA Hi Res value
    PWMDRV_fnGetMaxCompare        - Get the max compare value
    PWMDRV_fnGetMaxCompareHiRes   - Get the max Hi Res compare value
    PWMDRV_fnModuleOutputDis      - Disable both PWM pins attached to a module
    PWMDRV_fnModuleOutputEn       - Enable both PWM pins attached to a module
    PWMDRV_fnPinOutputDis         - Disable a PWM output pin
    PWMDRV_fnPinOutputEn          - Enable a PWM output pin
    PWMDRV_fnSetCompareA          - Set the CMPA value for a PWM module
    PWMDRV_fnSetCompareB          - Set the CMPB value for a PWM module                 
    PWMDRV_fnSetCompareAandB      - Set both CMPA and CMPB for a PWM module                         
    PWMDRV_fnSetCompareAHiRes     - Set the CMPA Hi Res value
    PWMDRV_fnSetCompareAandBHiRes - Set the CMPA Hi Res value and CMPB value
       
    local:
        pwmdrv_fnCtorPwm1              - Setup PWM 1
        pwmdrv_fnCtorPwm2              - Setup PWM 2
        pwmdrv_fnCtorPwm3              - Setup PWM 3
        pwmdrv_fnCtorPwm4              - Setup PWM 4
        pwmdrv_fnCtorPwm5              - Setup PWM 5
        pwmdrv_fnCtorPwm6              - Setup PWM 6
        pwmdrv_fnEventEnable           - Implements the event enable/disable
        pwmdrv_fnConfigGpioDisabMode   - Configs each modules pins for disabling
        pwmdrv_fnSetGpioMuxForTripZone - Configs specific trip zone pins 
   
NOTES:
    Refer to the .h file for instructions on how to use this driver.

CHANGE HISTORY:
$Log: PwmDrv.c $

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/25
    User: JeffF     Date: 01/31/07  Time: 05:39PM
    Put in another compile option for switching frequency (20khz) so the hardware
    team can test EMI improvements by varying the frequency. Switching is still
    formally set at 24.4khz.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/24
    User: JeffF     Date: 11/30/06  Time: 10:47PM
    Got rid of the interrupt used to handle OCP or NCD. Changed so that NCD
    no longer disables the HS FETs (only affects LS FETs)

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/23
    User: JeffF     Date: 11/24/06  Time: 12:15AM
    Changed how Over Current Detect and Negative Current detect are handled.
    They now both cause the HS and LS FETs to be disabled. There is also an
    interrupt generated when either of them is triggered.

    ***********************************************
    Revision: SurveyorPfrmA_HollyZ/1
    User: HollyZ     Date: 11/01/06  Time: 05:39PM
    Added SET/GET DEADBAND feature

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/22
    User: JeffF     Date: 08/08/06  Time: 05:20PM
    Changed the trip zone for over current to be a one shot trip instead of
    cycle by cycle.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/21
    User: JeffF     Date: 07/17/06  Time: 08:47PM
    Changed the trip zone for negative current to be a one shot trip instead
    of a cycle by cycle.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/20
    User: JeffF     Date: 05/18/06  Time: 09:52PM
    Changed the naming of a couple defines, and changed so that SOCB is also
    triggered off of CMPA

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/19
    User: JeffF     Date: 05/12/06  Time: 12:15AM
    Added a precompile directive to allow some features to be disabled for
    some of the DVT work.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/18
    User: JeffF     Date: 05/08/06  Time: 08:27PM
    Added a compile time directive to alter the setup for PWM 5 and 6 as the
    next rev of the board has different requirements. Set up for using two
    more of the trip zones to enforce a current limit on the backlight and
    the aux output. Also fixed a bug that was ca

    using the B outputs to not be initialized properly for their shutdown mode.

    
    ***********************************************
    Revision: SurveyorPfrmA_JeffF/17
    User: JeffF     Date: 04/21/06  Time: 12:26AM
    Added compile swith handling for 24-48khz operation. Changed so that all
    PWM triggered interrupts on PWM3 are disabled until the unit has been properly
    initialized.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/16
    User: JeffF     Date: 04/06/06  Time: 05:35PM
    Changed the rising edge and falling edge delay for the HS and LS FET drives.

    
    ***********************************************
    Revision: SurveyorPfrmA_JeffF/15
    User: JeffF     Date: 03/13/06  Time: 05:20PM
    Made some temporary changes to help with debugging the digital control
    algorithm

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/14
    User: JeffF     Date: 03/03/06  Time: 01:12AM
    Made a change to cause the interrupt to be based on CMPB instead of CMPA.
    This was a bug.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/13
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/12
    User: JeffF     Date: 01/30/06  Time: 11:13PM
    Code review was completed. Changes that came out of review were mostly
    to improve the comments in certain places, add or remove certain include
    files, and to use a compile option on one of the unused API functions.

    
    ***********************************************
    Revision: SurveyorPfrmA2_YingranD/1
    User: YingranD     Date: 01/18/06  Time: 07:39PM
    new digctrl package design and related file update

    ***********************************************
    Revision: SurveyorPfrmA_HenryC/1
    User: HenryC     Date: 11/01/05  Time: 05:16PM
    Added 48khz support: 

    Interrupt every other on SOC

    48khz period defines

    Moved Pwm3 SOC defines to .h file

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/11
    User: JeffF     Date: 06/13/05  Time: 09:36PM
    Inserted a few compile options to reduce the code size if driver options
    were not being used.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/10
    User: JeffF     Date: 06/04/05  Time: 12:03AM
    Fixed a typo bug that caused one register to be initialized twice instead
    of two registers being initialized.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/9
    User: JeffF     Date: 06/03/05  Time: 01:08AM
    Changed some code in the pwmdrv_fnEventEnable() function to a more code
    efficient implementation.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/8
    User: JeffF     Date: 05/31/05  Time: 12:42AM
    Fixed a typo bug in the local Ctor calls that would have been misleading,
    but not have caused any actual problems.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/7
    User: JeffF     Date: 05/27/05  Time: 04:10PM
    Changed the load mode for all the PWM's to load on Period instead of zero.
    

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/6
    User: JeffF     Date: 05/20/05  Time: 08:28PM
    Added Get functions for retrieving the compare A and B values

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/5
    User: JeffF     Date: 05/20/05  Time: 04:27PM
    This version is completely different from the previous version. This version
    is project specific but offers public access functions for changing compare
    values and enabling and disabling various outputs.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/2
    User: JeffF     Date: 05/10/05  Time: 11:13PM
    interim version

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/1
    User: JeffF     Date: 05/05/05  Time: 04:17PM
    Fixed the MgrCtor to work properly with HiRes mode. Added the recommended
    rounding constant in for Hires PWM calculation.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/6
    User: HueyD     Date: 04/27/05  Time: 05:13PM
    commented

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/5
    User: HueyD     Date: 04/26/05  Time: 02:52AM
    Attempt to reduce code size

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/4
    User: HueyD     Date: 04/25/05  Time: 05:54PM
    Added high res

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/3
    User: HueyD     Date: 04/23/05  Time: 01:12AM
    Used new defines and new paths

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/2
    User: HueyD     Date: 04/22/05  Time: 11:14PM
    Lots of changes
    
==============================================================================*/

// Define a shorter name for asserts
#define XASSERT_FILENAME "PwmD"

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    // _16BIT_ARCHITECTURE defined in Compiler Options
#include "LIB_stddefs.h"    // Standard definitions

#include "PwmDrv.h"         // Public Interface
#include "Device.h"         // Device definitions
#include "DeviceMacro.h"    // Macros for target device
#include "config.h"         // Hardware configuration
#include "sysclk.h"         // System clock functions
#include "xassert.h"        // Error handling

/*==============================================================================
                              Defines
==============================================================================*/
// Compile options
#define PWMDRV_USE_DIV_MACROS            TRUE // Use div macros for division
#define PWMDRV_FN_MODULE_OUTP_EN         FALSE
#define PWMDRV_FN_HI_RES_EN              FALSE
#define PWMDRV_FN_DTOR_EN                FALSE
#define PWMDRV_FN_SET_A_AND_B_EN         FALSE
#define PWMDRV_FN_SET_DEADBAND_EN        FALSE

// ***************************************************************
// Equation for Up or Down counter:
// Freq =              SYSCLK
//        --------------------------------------
//         ( TBPRD + 1 )( HSPCLKDIV )( CLKDIV )
// Note: for this driver, HSPCLKDIV and CLKDIV are set to div by 1
// ***************************************************************  
#if( CTRL_RUN_AT_20KHZ == TRUE )
    // This define sets the main buck switcher frequency
    #define PWMDRV_PERIOD_DIGITAL_CTRL    ( 5000 )  // F = 100Mhz/5000 = 20.0khz
    
    // This define sets the frequency for the DigCtrl ISR, which is set for
    // half the switching frequency of the PWM outputs    
    #define PWMDRV_PERIOD_DIGITAL_CTRL_ISR_TIMEBASE   ( 10000 ) // F = 10.0khz

#elif( CTRL_RUN_AT_24KHZ == TRUE )
    // This define sets the main buck switcher frequency
    #define PWMDRV_PERIOD_DIGITAL_CTRL    ( 4096 )  // F = 100Mhz/4096 = 24.4khz
    
    // This define sets the frequency for the DigCtrl ISR, which is set for
    // half the switching frequency of the PWM outputs
    #define PWMDRV_PERIOD_DIGITAL_CTRL_ISR_TIMEBASE   ( 8192 ) // F = 12.2khz 
    
#elif( CTRL_RUN_AT_48KHZ == TRUE )
    // This define sets the main buck switcher frequency 
    #define PWMDRV_PERIOD_DIGITAL_CTRL    ( 2048 )  // F = 100Mhz/2048 = 48.8khz
    
    // This define sets the frequency for the DigCtrl ISR, which is set for
    // half the switching frequency of the PWM outputs
    #define PWMDRV_PERIOD_DIGITAL_CTRL_ISR_TIMEBASE   ( 4096 ) // F = 24.4khz    

#else
    #error "Need to select a switching frequency"
#endif

 

// This define is for all the secondary PWM's like the Aux and the Relay drive
#define PWMDRV_PERIOD_SECONDARY_CTRL  ( 400  )   // F = 100Mhz/400 = 250khz

#define PWMDRV_PERIOD_REG_PWM1        PWMDRV_PERIOD_DIGITAL_CTRL   // HS FETs
#define PWMDRV_PERIOD_REG_PWM2        PWMDRV_PERIOD_DIGITAL_CTRL   // LS FETs
#define PWMDRV_PERIOD_REG_PWM3        PWMDRV_PERIOD_DIGITAL_CTRL_ISR_TIMEBASE
#define PWMDRV_PERIOD_REG_PWM4        PWMDRV_PERIOD_SECONDARY_CTRL // LCD cont
#define PWMDRV_PERIOD_REG_PWM5        PWMDRV_PERIOD_SECONDARY_CTRL // Rly & BL
#define PWMDRV_PERIOD_REG_PWM6        PWMDRV_PERIOD_SECONDARY_CTRL // Aux Outp

// Used to control the deadband time between high side and low side FET drive
// RED = Rising edge delay
#define PWMDRV_HIGH_SIDE_FET_RED      ( 30 ) // number of timer counts
#define PWMDRV_LOW_SIDE_FET_RED       ( 30 ) 

// These two defines are used to trigger ADC conversions at a certain point 
// in the PWM cycle. The values shown are the number of PWM counts to 
// wait after a new PWM cycle starts before triggering the ADC SOC
// Note that SOCB is only enabled 20 times per second, and in cycles where
// both SOCA and SOCB are enabled, Seq 1 will run first then Seq 2 will run.
#define PWMDRV_DFLT_TIME_SOCA_SOCB    ( 100 )
// Same thing for this define, but it is used to start the ADC Interrupt which
// we use to run the digital control ISR
#define PWMDRV_DFLT_TIME_CTRL_ISR     ( PWMDRV_PERIOD_DIGITAL_CTRL + \
                                        PWMDRV_DFLT_TIME_SOCA_SOCB ) 

#define PWMDRV_PWM_CLOCK_ENAB_BITS    ( 0x003F )

// ******************
// Hi Res definitions
// ******************
// Refer to TI document SPRU924 for information on how the Hi Res feature works
// Define the Micro Edge Positioning (MEP) size
#define PWMDRV_HIRES_MEP_SIZE_PS          ( 150L )

// Define MEP steps @ 100MHz
// ( This step is used for calculations if PWMDRV_USE_DIV_MACROS is TRUE
#define PWMDRV_HIRES_MEP_STEPS            ( 66 ) // 1 / ( 100E6 * 150E-12 )

// Rounding constant recommended by TI documentation (SPRU924)
#define PWMDRV_HIRES_ROUNDING_CONST       ( 0x180 )

/*==============================================================================
                               Macros
==============================================================================*/
// ***********
// Misc macros
// ***********
// Divide macro factor.  65536 / divisor.  The >> is for rounding.
#define PWMDRV_mDIV_FACTOR( Factor ) \
           ( ( ( 1L << 16 ) + ( Factor >> 1 ) ) / Factor )

// Divide macro
// The Diviosor MUST be a literal for this code to be efficient
#define PWMDRV_mDIV( Dividend, LitDivisor ) ( uint16 ) \
         ( ( ( uint32 )( Dividend ) \
           * PWMDRV_mDIV_FACTOR( LitDivisor ) ) >> 16 )
           
// ***********************
// Bit manipulation macros
// ***********************         
// Masks and configurations for GPAMUX1 reg (32-bit)
#define PWMDRV_GPAMUX1_GPIO_MSK           ( 0x03L )
#define PWMDRV_GPAMUX1_GPIO_GPIO          ( 0L )
#define PWMDRV_GPAMUX1_GPIO_PWM           ( 1L )
#define PWMDRV_GPAMUX1_GPIO_TZ            ( 1L )
#define PWMDRV_mGPAMUX1_GPIO_POS( Id )    ( ( uint16 )Id * 2 )
#define PWMDRV_mGPAMUX1_TZ_GPIO_POS( Id ) ( ( ( uint16 )Id + 12 ) * 2 )

// Masks and configurations for GPAMUX2 reg (32-bit)
#define PWMDRV_GPAMUX2_GPIO_MSK           ( 0x03L )
#define PWMDRV_GPAMUX2_GPIO_GPIO          ( 0L )
#define PWMDRV_GPAMUX2_GPIO_TZ            ( 3L )
#define PWMDRV_mGPAMUX2_TZ_GPIO_POS( Id ) ( ( uint32 )( Id - PWMDRV_eTZ_5 ) * 2 )

// Masks and configurations for GPADIR reg (32-bit)
#define PWMDRV_GPADIR_GPIO_MSK            ( 0x01L )
#define PWMDRV_GPADIR_GPIO_INPUT          ( 0x00L )
#define PWMDRV_GPADIR_GPIO_OUTPUT         ( 0x01L )
#define PWMDRV_mGPADIR_GPIO_POS( Id )     ( ( uint32 )Id )

// Masks and configurations for GPADAT reg (32-bit)
#define PWMDRV_GPADAT_GPIO_MSK            ( 0x01L )
#define PWMDRV_mGPADAT_GPIO_POS( Id )     ( ( uint32 )Id )

// Gets the mask and the configuration for the register
#define PWMDRV_mMSKCFG( MskResult, CfgResult, Msk, Cfg, Pos ) \
            MskResult = ( Msk << Pos ); \
            CfgResult = ( Cfg << Pos ); 

/*==============================================================================
                            Type Definitions
==============================================================================*/
// This variable is used to point to the PWM registers on the device
typedef volatile struct EPWM_REGS* tpzEPWM_REGS;

/*==============================================================================
                                Enums
==============================================================================*/
// Define disable state when PWMDRV_fnPinOutputDis() or 
// PWMDRV_fnModuleOutputDis() is called
typedef enum eDIS_MODE
{
    eDIS_MODE_NO_ACTION = 0, // No action is taken
    eDIS_MODE_HIGH_IMP,      // Changes pin/module to GPIO input to disable
    eDIS_MODE_FORCE_LOW,     // Changes pin/module to GPIO output with logic lo
    eDIS_MODE_FORCE_HIGH     // Changes pin/module to GPIO output with logic hi

}teDIS_MODE;

typedef enum ePIN_MODE
{
    ePIN_MODE_GPIO = 0,  // Pin is used as a GPIO (or something else - not PWM)
    ePIN_MODE_PWM        // Pin is used as a PWM output
    
} tePIN_MODE;

/*=============================================================================
                              Structures
==============================================================================*/
// Define structure to store all necessary PWM data
typedef struct zPWM
{
    tpzEPWM_REGS   tpzPwmRegs;  // Pointer to PWM registers
    PWMDRV_tePIN   tePinA;      // The enum value for the A output on the module
    teDIS_MODE     teDisModeA;  // Disable mode for output A
    teDIS_MODE     teDisModeB;  // Disable mode for output B   
    tePIN_MODE     tePinModeA;  // Output pin enabled as PWM or not
    tePIN_MODE     tePinModeB;  // Output pin enabled as PWM or not
        
}tzPWM;

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/
// Array of PWM structures - Each PWM module has one struct associated with it
static tzPWM atzPwmModule[ 6 ];

#if( PWMDRV_FN_HI_RES_EN == TRUE )
// Number of MEP steps (Micro Edge Positioning - used in HiRes mode)
static uint16 MepSteps;
#endif

/*==============================================================================
                       External/Public Function Protoypes
==============================================================================*/

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
static void pwmdrv_fnCtorPwm1( void );
static void pwmdrv_fnCtorPwm2( void );
static void pwmdrv_fnCtorPwm3( void );
static void pwmdrv_fnCtorPwm4( void );
static void pwmdrv_fnCtorPwm5( void );
static void pwmdrv_fnCtorPwm6( void );

static void pwmdrv_fnConfigGpioDisabMode( tzPWM* ptzPwm );
static void pwmdrv_fnSetGpioMuxForTripZone( PWMDRV_teTZ teTripZone );
static void pwmdrv_fnEventEnable( PWMDRV_teMODULE tePwmModule,
                                  PWMDRV_teETEN   teEventType,
                                  tucBOOL         tEnable );

/*==============================================================================
                           Function Definitions
==============================================================================*/
/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnCtor

PURPOSE:
    Setup the PWM driver file

INPUTS:
    none
   
OUTPUTS:
    PWMDRV_teSTATUS - function execution status
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
PWMDRV_teSTATUS PWMDRV_fnCtor( void )
{
#if( PWMDRV_FN_HI_RES_EN == TRUE )    
    uint32 Temp;

    // Calculate the number of mep steps
    // Care was taken to preserve resolution
    Temp = SYSCLK_fnGetSysClk( ) / 1000;
    Temp *= PWMDRV_HIRES_MEP_SIZE_PS;
    MepSteps = ( uint16 )( 1E9L / Temp );
    
    assert( MepSteps == PWMDRV_HIRES_MEP_STEPS );
#endif 
    
    // Enable the PWM module clocks
    DEVICE_mACCESS_EN( );    
    SysCtrlRegs.PCLKCR1.all |= PWMDRV_PWM_CLOCK_ENAB_BITS;
    DEVICE_mACCESS_DIS( );   

    // Setup all the PWM modules
    pwmdrv_fnCtorPwm1( );
    pwmdrv_fnCtorPwm2( );
    pwmdrv_fnCtorPwm3( );
    pwmdrv_fnCtorPwm4( );
    pwmdrv_fnCtorPwm5( );
    pwmdrv_fnCtorPwm6( );
    
    // Enable and sync all PWMs 
    DEVICE_mACCESS_EN( );
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    DEVICE_mACCESS_DIS( ); 
    
    return( PWMDRV_eSTATUS_SUCCESS );
} 

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnDtor

PURPOSE:
    Disable the PWM driver by shutting disabling all the PWM clocks

INPUTS:
    none
   
OUTPUTS:
    PWMDRV_teSTATUS - function execution status
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
#if( PWMDRV_FN_DTOR_EN == TRUE )
PWMDRV_teSTATUS PWMDRV_fnDtor( void )
{
    // Disable the PWM module clocks
    DEVICE_mACCESS_EN( );    
    SysCtrlRegs.PCLKCR1.all &= ~PWMDRV_PWM_CLOCK_ENAB_BITS;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    DEVICE_mACCESS_DIS( );
    
    return( PWMDRV_eSTATUS_SUCCESS );    
}
#endif

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnEventDis

PURPOSE:
    Allows the application to disable a PWM event such as SOCA or SOCB. 

INPUTS:
    tePwmModule - The module that was was causing the PWM event
    teEventType - SOCA, SOCB, PWM INT
   
OUTPUTS:
    none
   
NOTES:

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
void PWMDRV_fnEventDis( PWMDRV_teMODULE tePwmModule,
                        PWMDRV_teETEN   teEventType )
{
    // Call the local function for implementation
    pwmdrv_fnEventEnable( tePwmModule,
                          teEventType,
                          FALSE );
}

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnEventEn
    
PURPOSE:
    Allows the application to enable a PWM event such as SOCA or SOCB.
    
INPUTS:
    tePwmModule - The module that will cause the PWM event
    teEventType - SOCA, SOCB, PWM INT
   
OUTPUTS:
    none
   
NOTES: 
    The functionality provided by this function needs to be properly setup in
    the individual PWM constructors. All this function does is enable or 
    disable the event, it does not trigger the event.

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
void PWMDRV_fnEventEn( PWMDRV_teMODULE tePwmModule,
                       PWMDRV_teETEN   teEventType )
{
    // Call the local function for implementation
    pwmdrv_fnEventEnable( tePwmModule,
                          teEventType,
                          TRUE );
}

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnGetCompareA
    
PURPOSE:
    Get the CMPA value for a specific PWM module.
    
INPUTS:
    tePwmModule - The PWM module of interest
    
OUTPUTS:
    uint16 - The CMPA value
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
uint16 PWMDRV_fnGetCompareA( PWMDRV_teMODULE tePwmModule )
{
    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <  PWMDRV_eMODULE_TOTAL );
     
    return( atzPwmModule[ tePwmModule ].tpzPwmRegs->CMPA.half.CMPA );
}

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnGetCompareB
    
PURPOSE:
    Get the CMPB value for a specific PWM module.
    
INPUTS:
    tePwmModule - The PWM module of interest
    
OUTPUTS:
    uint16 - The CMPB value
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
uint16 PWMDRV_fnGetCompareB( PWMDRV_teMODULE tePwmModule )
{
    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <  PWMDRV_eMODULE_TOTAL );
     
    return( atzPwmModule[ tePwmModule ].tpzPwmRegs->CMPB );
}

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnGetCompareAHiRes
    
PURPOSE:
    Get the hi res CMPA value for a specific PWM module.
    
INPUTS:
    tePwmModule - The PWM module of interest
    
OUTPUTS:
    uint32 - The CMPA Hi Res value
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
#if( PWMDRV_FN_HI_RES_EN == TRUE )
uint32 PWMDRV_fnGetCompareAHiRes( PWMDRV_teMODULE tePwmModule )
{
    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <= PWMDRV_eMODULE_4 );
     
    // TBD I need to come back to this one to see how it should be done
    assert( FALSE );
    
    return( ( uint32 )( atzPwmModule[ tePwmModule ].tpzPwmRegs->CMPA.all )
            * ( uint32 )MepSteps );
}
#endif 

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnGetMaxCompare

PURPOSE:
    Retrieve the max compare value that can be set for a particular PWM module.

INPUTS:
    tePwmModule - The PWM module of interest
   
OUTPUTS:
    uint16 - The max (non-high res) count that the CMPA or CMPB value should be
             set to. You can exceed this count, but it will be greater than the
             PWM duty reg and hence you would never get a CMPA or CMPB event.
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
uint16 PWMDRV_fnGetMaxCompare( PWMDRV_teMODULE tePwmModule )
{
    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <  PWMDRV_eMODULE_TOTAL );
     
    return( atzPwmModule[ tePwmModule ].tpzPwmRegs->TBPRD );
}

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnGetMaxCompareHiRes

PURPOSE:
    Retrieve the max Hi Res compare value that can be set for a particular PWM 
    module.

INPUTS:
    tePwmModule - The PWM module of interest
   
OUTPUTS:
    uint32 - The max (high res) count that the CMPA value should be set to. 
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
#if( PWMDRV_FN_HI_RES_EN == TRUE )
uint32 PWMDRV_fnGetMaxCompareHiRes( PWMDRV_teMODULE tePwmModule )
{
    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <= PWMDRV_eMODULE_4 );
     
    return( ( uint32 )( atzPwmModule[ tePwmModule ].tpzPwmRegs->TBPRD )
            * ( uint32 )MepSteps );
}
#endif

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnSetCompareA
    
PURPOSE:
    Set the CMPA value for a specific PWM module.
    
INPUTS:
    tePwmModule - The PWM module of interest
    CompVal     - The value to set CMPA to
    
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
void PWMDRV_fnSetCompareA( PWMDRV_teMODULE tePwmModule, 
                           uint16          CompVal )
{
    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <  PWMDRV_eMODULE_TOTAL );    
    
    // Set compare value
    atzPwmModule[ tePwmModule ].tpzPwmRegs->CMPA.half.CMPA = CompVal;
}                                      
  
/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnSetCompareB
    
PURPOSE:
    Set the CMPB value for a specific PWM module.
    
INPUTS:
    tePwmModule - The PWM module of interest
    CompVal     - The value to set CMPB to
    
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/                                    
void PWMDRV_fnSetCompareB( PWMDRV_teMODULE tePwmModule,
                           uint16          CompVal )
{
    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <  PWMDRV_eMODULE_TOTAL );    
    
    // Set compare value
    atzPwmModule[ tePwmModule ].tpzPwmRegs->CMPB = CompVal;
}
  
/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnSetDeadbands
    
PURPOSE:
    Set the deadband count for PWM1 and PWM2 
    
INPUTS:
    HiRDB - High side FET rising edge delay count
    HiRDB - Low  side FET rising edge delay count    
    
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY

Version: 1.00    Date: Oct-27-2006   By: Holly Zhou
   - Created
    
*******************************************************************************/                                    
#if( PWMDRV_FN_SET_DEADBAND_EN == TRUE )
void PWMDRV_fnSetDeadbands( uint16 HiRDB, uint16 LoRDB )
{

    EPwm1Regs.DBRED = HiRDB; 

    EPwm2Regs.CMPB = ( PWMDRV_PERIOD_REG_PWM2 - HiRDB ); 
    EPwm2Regs.DBRED = LoRDB; 
}

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnGetDeadbands
    
PURPOSE:
    Get the deadband count for PWM1 and PWM2 
    
INPUTS:
    *pHiRDB - pointer to High side FET rising edge delay count
    *pHiRDB - pointer to Low  side FET rising edge delay count    
    
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY

Version: 1.00    Date: Oct-27-2006   By: Holly Zhou
   - Created
    
*******************************************************************************/                                    

void PWMDRV_fnGetDeadbands( uint16 *pHiRDB, uint16 *pLoRDB )
{
    *pHiRDB = EPwm1Regs.DBRED;    
    *pLoRDB = EPwm2Regs.DBRED; 
}

#endif
   
/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnSetCompareAandB
    
PURPOSE:
    Set the CMPA and the CMPB value for a specific PWM module.
    
INPUTS:
    tePwmModule - The PWM module of interest
    CompAVal    - The value to set CMPA to
    CompBVal    - The value to set CMPB to    
    
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/                                     
#if( PWMDRV_FN_SET_A_AND_B_EN == TRUE )
void PWMDRV_fnSetCompareAandB( PWMDRV_teMODULE tePwmModule,
                               uint16          CompAVal, 
                               uint16          CompBVal )
{
    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <  PWMDRV_eMODULE_TOTAL );    
    
    // Set compare values
    atzPwmModule[ tePwmModule ].tpzPwmRegs->CMPA.half.CMPA = CompAVal;
    atzPwmModule[ tePwmModule ].tpzPwmRegs->CMPB           = CompBVal;
}
#endif

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnSetCompareAHiRes
    
PURPOSE:
    Set the Hi Res CMPA value for a specific PWM module.
    
INPUTS:
    tePwmModule - The PWM module of interest
    CompVal     - The value to set CMPA to
    
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/                                          
#if( PWMDRV_FN_HI_RES_EN == TRUE )
void PWMDRV_fnSetCompareAHiRes( PWMDRV_teMODULE tePwmModule,
                                uint32          CompVal )
{
    uint32 SetVal;
    uint32 Temp;

    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <= PWMDRV_eMODULE_4 );
        
    // Divide the CompVal by the MEP steps
#if ( PWMDRV_USE_DIV_MACROS == TRUE )
    Temp = PWMDRV_mDIV( CompVal, PWMDRV_HIRES_MEP_STEPS );
#else    
    Temp = CompVal / MepSteps; 
#endif    
    
    // Construct the 32 bit val for high res pwm cmp register
    SetVal = Temp << 16;
    SetVal |= ( ( ( CompVal - ( Temp * MepSteps ) ) << 8 ) 
                  + PWMDRV_HIRES_ROUNDING_CONST );     
    
    // Set compare value
    atzPwmModule[ tePwmModule ].tpzPwmRegs->CMPA.all = SetVal;
}
#endif

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnSetCompareAandBHiRes
    
PURPOSE:
    Set the Hi Res CMPA value and the non-high res CMPB value for a 
    specific PWM module.
    
INPUTS:
    tePwmModule - The PWM module of interest
    CompAVal    - The value to set CMPA to
    CompBVal    - The value to set CMPB to    
    
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
#if( PWMDRV_FN_HI_RES_EN == TRUE )
void PWMDRV_fnSetCompareAandBHiRes( PWMDRV_teMODULE tePwmModule,
                                    uint32          CompAVal, 
                                    uint16          CompBVal )
{
    uint32 SetVal;
    uint32 Temp;

    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <= PWMDRV_eMODULE_4 );
        
    // Divide the CompVal by the MEP steps
#if ( PWMDRV_USE_DIV_MACROS == TRUE )
    Temp = PWMDRV_mDIV( CompAVal, PWMDRV_HIRES_MEP_STEPS );
#else    
    Temp = CompAVal / MepSteps; 
#endif    
    
    // Construct the 32 bit val for high res pwm cmp register
    // See hi res document for explanation behind calculations
    SetVal = Temp << 16;
    SetVal |= ( ( ( CompAVal - ( Temp * MepSteps ) ) << 8 ) 
                  + PWMDRV_HIRES_ROUNDING_CONST );     
    
    // Set compare values
    atzPwmModule[ tePwmModule ].tpzPwmRegs->CMPA.all = SetVal;
    atzPwmModule[ tePwmModule ].tpzPwmRegs->CMPB     = CompBVal;
}
#endif

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnModuleOutputDis

PURPOSE:
    Disable the PWM outputs for a specific PWM module. If the pin(s) are not
    set for PWM control, this function will have no effect.

INPUTS:
    tePwmModule - The PWM module of interest
   
OUTPUTS:
    none
   
NOTES:
    There is no easy way to simply disable a PWM output by say making it high
    impedance. This function will handle it properly for you. 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
#if( PWMDRV_FN_MODULE_OUTP_EN == TRUE )
void PWMDRV_fnModuleOutputDis( PWMDRV_teMODULE tePwmModule )
{
    tzPWM*        ptzPwm;
    PWMDRV_tePIN  tePin;
    teDIS_MODE    teDisMode;
    tePIN_MODE    tePinMode;
    uint16        i;
    uint32        TempMsk;
    uint32        TempCfg;
    
    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <  PWMDRV_eMODULE_TOTAL );          
    
    // Assign a pointer to the PWM module being disabled
    ptzPwm = &atzPwmModule[ tePwmModule ];
    
    // Get the pin number, disable mode, and pin mode condition into local vars
    tePin      = ptzPwm->tePinA;
    teDisMode  = ptzPwm->teDisModeA;
    tePinMode  = ptzPwm->tePinModeA;    
    
    // This for loop is set to execute twice so that Pin A and B can be disabled
    for( i = 0; i <= 1; i++ )
    {
        // The following code chunk is executed only if the pin  
        // is meant for PWM use 
        if( tePinMode == ePIN_MODE_PWM )
        {            
            // Handle the disabling according to the constructor setup
            switch( teDisMode )
            {
                case eDIS_MODE_NO_ACTION:
                    // Change the disable mode and the pin for the B output 
                    teDisMode = ptzPwm->teDisModeB;
                    tePinMode = ptzPwm->tePinModeB;                 
                    tePin++;            
                    
                    // Skip the rest of the for loop and process the next pin
                    continue;
        
                case eDIS_MODE_FORCE_LOW:
                    // Set the data pin for an output low
                    GpioDataRegs.GPACLEAR.all = 
                        ( PWMDRV_GPADAT_GPIO_MSK <<
                          PWMDRV_mGPADAT_GPIO_POS( tePin ) );
                    break;
                
                case eDIS_MODE_FORCE_HIGH:
                    // Set the data pin for an output high
                    GpioDataRegs.GPASET.all = 
                        ( PWMDRV_GPADAT_GPIO_MSK << 
                          PWMDRV_mGPADAT_GPIO_POS( tePin ) );           
                    break;  
                                                
                case eDIS_MODE_HIGH_IMP:
                    // Do nothing because to set to high impedance, all you need
                    // to do is change the pin to be a GPIO (it will default
                    // to being an input pin), which will have high Z
                    break;
            
            }               
            // Get the mask and configuration for the gpio mux register
            // and set the GPIO mux for GPIO mode (ie. not as PWM anymore)
            PWMDRV_mMSKCFG( 
                TempMsk,
                TempCfg,
                PWMDRV_GPAMUX1_GPIO_MSK,
                PWMDRV_GPAMUX1_GPIO_GPIO,
                PWMDRV_mGPAMUX1_GPIO_POS( tePin ) );        
            
            DEVICE_mACCESS_EN( );
                    
            // Finally, setup the mux of the gpio
            GpioCtrlRegs.GPAMUX1.all &= ~TempMsk;
            GpioCtrlRegs.GPAMUX1.all |= TempCfg;
            
            DEVICE_mACCESS_DIS( );      
        }
            
        // Change the disable mode and the pin for the B output 
        teDisMode = ptzPwm->teDisModeB; 
        tePinMode = ptzPwm->tePinModeB;         
        tePin++;                            
    }           
}
#endif

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnModuleOutputEn

PURPOSE:
    Enable the PWM outputs for a specific PWM module. If the pin(s) are not
    intended for PWM control, this function will have no effect.

INPUTS:
    tePwmModule - The PWM module of interest
   
OUTPUTS:
    none
       
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
#if( PWMDRV_FN_MODULE_OUTP_EN == TRUE )
void PWMDRV_fnModuleOutputEn( PWMDRV_teMODULE tePwmModule )
{
    tzPWM*        ptzPwm;
    PWMDRV_tePIN  tePin;
    tePIN_MODE    tePinMode;
    uint16        i;
    uint32        TempMsk;
    uint32        TempCfg;  
    
    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <  PWMDRV_eMODULE_TOTAL );    
    
    // Assign a pointer to the PWM module being enabled
    ptzPwm = &atzPwmModule[ tePwmModule ];
    
    // Get the pin number, and pin mode condition into local vars
    tePin     = ptzPwm->tePinA;
    tePinMode = ptzPwm->tePinModeA; 
    
    // This for loop is set to execute twice so that Pin A and B can be enabled
    for( i = 0; i <= 1; i++ )
    {
        if( tePinMode == ePIN_MODE_PWM )
        {        
            // Get the mask and configuration for the gpio mux register
            // and set the GPIO mux back to PWM mode
            PWMDRV_mMSKCFG( 
                TempMsk,
                TempCfg,
                PWMDRV_GPAMUX1_GPIO_MSK,
                PWMDRV_GPAMUX1_GPIO_PWM,
                PWMDRV_mGPAMUX1_GPIO_POS( tePin ) );            
            
            DEVICE_mACCESS_EN( );       
                                    
            // Finally, setup the mux of the gpio
            GpioCtrlRegs.GPAMUX1.all &= ~TempMsk;
            GpioCtrlRegs.GPAMUX1.all |= TempCfg;
            
            DEVICE_mACCESS_DIS( );          
        }
        
        // Change the disable mode and the pin for the B output 
        tePinMode = ptzPwm->tePinModeB;     
        tePin++; 
    }   
}
#endif

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnPinOutputDis
    
PURPOSE:
    Disable the PWM output for a specific PWM pin. If the pin is not
    set for PWM control, this function will have no effect.

INPUTS:
    tePwmPin - The PWM pin of interest
   
OUTPUTS:
    none
       
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
void PWMDRV_fnPinOutputDis( PWMDRV_tePIN tePwmPin )
{
    tzPWM*      ptzPwm;
    tePIN_MODE  tePinMode;
    teDIS_MODE  teDisMode;    
    uint32      TempMsk;
    uint32      TempCfg;
    
    // Perform range checking
    assert( tePwmPin >= PWMDRV_ePIN_1A );
    assert( tePwmPin <  PWMDRV_ePIN_TOTAL );    
    
    // Assign a pointer to the PWM module that contains the pin being disabled
    // Since there is 2 pins per module, just shift pin number right by one
    // to properly index the array
    ptzPwm = &atzPwmModule[ ( tePwmPin >> 1 ) ];
    
    // Check if it is Output pin A or B that is being disabled
    if( tePwmPin == ptzPwm->tePinA )
    {
        tePinMode = ptzPwm->tePinModeA;
        teDisMode = ptzPwm->teDisModeA;
    }
    else
    {
        tePinMode = ptzPwm->tePinModeB;
        teDisMode = ptzPwm->teDisModeB;        
    }
    
    // Only disable the PWM pin if it intended as a PWM output
    if( tePinMode == ePIN_MODE_PWM )
    {
        // Determine which type of disabling should be performed
        switch( teDisMode )
        {
            case eDIS_MODE_NO_ACTION:
                // Do nothing and return
                return;
    
            case eDIS_MODE_FORCE_LOW:
                // Set the data pin low
                GpioDataRegs.GPACLEAR.all = 
                    ( PWMDRV_GPADAT_GPIO_MSK << 
                      PWMDRV_mGPADAT_GPIO_POS( tePwmPin ) );
                break;
            
            case eDIS_MODE_FORCE_HIGH:
                // Set the data pin high
                GpioDataRegs.GPASET.all = 
                    ( PWMDRV_GPADAT_GPIO_MSK << 
                      PWMDRV_mGPADAT_GPIO_POS( tePwmPin ) );            
                break;  
                                            
            case eDIS_MODE_HIGH_IMP:
                // Do nothing but complete the rest of the function
                break;      
        }    
        
        // Get the mask and configuration for the gpio mux register
        // and set the GPIO mux for GPIO mode
        PWMDRV_mMSKCFG( 
            TempMsk,
            TempCfg,
            PWMDRV_GPAMUX1_GPIO_MSK,
            PWMDRV_GPAMUX1_GPIO_GPIO,
            PWMDRV_mGPAMUX1_GPIO_POS( tePwmPin ) );        
        
        DEVICE_mACCESS_EN( );
                
        // Finally, setup the mux of the gpio
        GpioCtrlRegs.GPAMUX1.all &= ~TempMsk;
        GpioCtrlRegs.GPAMUX1.all |= TempCfg;
        
        DEVICE_mACCESS_DIS( );
    }
    else
    {
        // There is no point calling this function if the pin is 
        // intended as a GPIO pin
        assert( FALSE );
    }                   
}

/*******************************************************************************

FUNCTION NAME:
    PWMDRV_fnPinOutputEn
    
PURPOSE:
    Enable the PWM output for a specific PWM pin. If the pin is not
    intended for PWM control, this function will have no effect.

INPUTS:
    tePwmPin - The PWM pin of interest
   
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
void PWMDRV_fnPinOutputEn ( PWMDRV_tePIN tePwmPin )
{
    tzPWM*     ptzPwm;
    tePIN_MODE tePinMode;
    uint32     TempMsk;
    uint32     TempCfg;
    
    // Perform range checking
    assert( tePwmPin >= PWMDRV_ePIN_1A );
    assert( tePwmPin <  PWMDRV_ePIN_TOTAL );    
    
    // Assign a pointer to the PWM module that contains the pin being disabled
    // Since there is 2 pins per module, just shift pin number right by one
    // to properly index the array    
    ptzPwm = &atzPwmModule[ ( tePwmPin >> 1 ) ];
    
    // Check if it is Output pin A or B that is being enabled    
    if( tePwmPin == ptzPwm->tePinA )
    {
        tePinMode = ptzPwm->tePinModeA;
    }
    else
    {
        tePinMode = ptzPwm->tePinModeB;
    }
    
    // Only enable the PWM pin if it intended as a PWM output
    if( tePinMode == ePIN_MODE_PWM )
    {        
        // Get the mask and configuration for the gpio mux register
        // and set the GPIO mux back to PWM mode
        PWMDRV_mMSKCFG( 
            TempMsk,
            TempCfg,
            PWMDRV_GPAMUX1_GPIO_MSK,
            PWMDRV_GPAMUX1_GPIO_PWM,
            PWMDRV_mGPAMUX1_GPIO_POS( tePwmPin ) );             
        
        DEVICE_mACCESS_EN( );       
                                
        // Finally, setup the mux of the gpio
        GpioCtrlRegs.GPAMUX1.all &= ~TempMsk;
        GpioCtrlRegs.GPAMUX1.all |= TempCfg;
        
        DEVICE_mACCESS_DIS( );          
    }
    else
    {
        // There is no point calling this function if the pin is 
        // intended as a GPIO pin
        assert( FALSE );    
    }
}

/*******************************************************************************

FUNCTION NAME:
    pwmdrv_fnCtorPwm1

PURPOSE:
    Setup PWM 1 module

INPUTS:
    none
   
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
static void pwmdrv_fnCtorPwm1( void )
{    
    tzPWM* ptzPwm;
    
    // Assign a pointer to the PWM module being intialized
    ptzPwm = &atzPwmModule[ PWMDRV_eMODULE_1 ];
    
    // Configure the structure variables as required for the module
    ptzPwm->tpzPwmRegs = &EPwm1Regs;
    ptzPwm->tePinA     = PWMDRV_ePIN_1A;
    ptzPwm->teDisModeA = eDIS_MODE_FORCE_LOW;
    ptzPwm->teDisModeB = eDIS_MODE_NO_ACTION; 
    ptzPwm->tePinModeA = ePIN_MODE_PWM;
    ptzPwm->tePinModeB = ePIN_MODE_GPIO;
    
    // ***************************
    // Set up the Time Base Module
    // ***************************    
    EPwm1Regs.TBCTL.all = 
        ( F280X_mCFGMSK( TBCTL, FREESOFT,  FREE        )
        | F280X_mCFGMSK( TBCTL, PHSDIR,    COUNT_UP    )
        | F280X_mCFGMSK( TBCTL, CLKDIV,    DIVBY_1     )
        | F280X_mCFGMSK( TBCTL, HSPCLKDIV, DIVBY_1     ) 
        | F280X_mCFGMSK( TBCTL, SYNCOSEL,  CTR_EQU_ZRO )
        | F280X_mCFGMSK( TBCTL, PRDLD,     SHDW        )
        | F280X_mCFGMSK( TBCTL, PHSEN,     DIS         )
        | F280X_mCFGMSK( TBCTL, CTRMODE,   COUNT_UP    )
        );
        
    // Set the period register
    EPwm1Regs.TBPRD = PWMDRV_PERIOD_REG_PWM1;
    EPwm1Regs.TBPHS.all = 0;
    
    // *********************************
    // Set up the Counter Compare Module
    // *********************************          
    EPwm1Regs.CMPCTL.all =     
        ( F280X_mCFGMSK( CMPCTL, SHDWBMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, SHDWAMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, LOADBMODE, PRD  )
        | F280X_mCFGMSK( CMPCTL, LOADAMODE, PRD  )
        );      
    
    // **********************************
    // Set up the Action Qualifier Module
    // **********************************           
    EPwm1Regs.AQCTLA.all = 
        ( F280X_mCFGMSK( AQCTLA, CBD, DIS )
        | F280X_mCFGMSK( AQCTLA, CBU, DIS )
        | F280X_mCFGMSK( AQCTLA, CAD, DIS )
        | F280X_mCFGMSK( AQCTLA, CAU, CLR )
        | F280X_mCFGMSK( AQCTLA, PRD, DIS )
        | F280X_mCFGMSK( AQCTLA, ZRO, SET )
        );
    
    EPwm1Regs.AQCTLB.all = 
        ( F280X_mCFGMSK( AQCTLB, CBD, DIS )
        | F280X_mCFGMSK( AQCTLB, CBU, DIS )
        | F280X_mCFGMSK( AQCTLB, CAD, DIS )
        | F280X_mCFGMSK( AQCTLB, CAU, DIS )
        | F280X_mCFGMSK( AQCTLB, PRD, DIS )
        | F280X_mCFGMSK( AQCTLB, ZRO, DIS )
        );    
        
    EPwm1Regs.AQSFRC.all =     
        ( F280X_mCFGMSK( AQSFRC, RLDCSF, LOAD_ON_ZRO )
        | F280X_mCFGMSK( AQSFRC, ACTSFB, DIS         )
        | F280X_mCFGMSK( AQSFRC, ACTSFA, DIS         )
        );
        
    EPwm1Regs.AQCSFRC.all =     
        ( F280X_mCFGMSK( AQCSFRC, CSFB, DIS )
        | F280X_mCFGMSK( AQCSFRC, CSFA, DIS )
        ); 
        
    // **************************
    // Setup the Dead Band Module
    // **************************
    EPwm1Regs.DBCTL.all = 
        ( F280X_mCFGMSK( DBCTL, POLSEL,   ACTIVE_HIGH )
        | F280X_mCFGMSK( DBCTL, OUT_MODE, ENA )
        );
    EPwm1Regs.DBRED = PWMDRV_HIGH_SIDE_FET_RED; 
        
    // ********************************
    // Not using the PWM Chopper Module
    // ********************************
    
    // ***************************
    // Set up the Trip Zone Module
    // *************************** 
    // Trip zone registers are EALLOW protected
    DEVICE_mACCESS_EN( );
    
    EPwm1Regs.TZSEL.all = 
        ( F280X_mCFGMSK( TZSEL, OSHT6, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT5, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT4, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT3, ENA ) // HS FETs forced low on OCP
        | F280X_mCFGMSK( TZSEL, OSHT2, DIS ) // HS FETs no action on NCD
        | F280X_mCFGMSK( TZSEL, OSHT1, DIS )
        | F280X_mCFGMSK( TZSEL, CBC6,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC5,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC4,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC3,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC2,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC1,  DIS )
        );
        
    EPwm1Regs.TZCTL.all = 
        ( F280X_mCFGMSK( TZCTL, TZB, DIS        ) 
        | F280X_mCFGMSK( TZCTL, TZA, FORCED_LOW )
        );
        
    EPwm1Regs.TZEINT.all = 
        ( F280X_mCFGMSK( TZEINT, OST, DIS )
        | F280X_mCFGMSK( TZEINT, CBC, DIS )
        );    
        
    DEVICE_mACCESS_DIS( );
        
    // ******************************
    // Setup the Event Trigger Module
    // ******************************  
    EPwm1Regs.ETSEL.all = 
        ( F280X_mCFGMSK( ETSEL, SOCBEN,  DIS           )
        | F280X_mCFGMSK( ETSEL, SOCBSEL, CTRU_EQU_CMPB )
        | F280X_mCFGMSK( ETSEL, SOCAEN,  DIS           )
        | F280X_mCFGMSK( ETSEL, SOCASEL, CTRU_EQU_CMPA )
        | F280X_mCFGMSK( ETSEL, INTEN,   DIS           )
        | F280X_mCFGMSK( ETSEL, INTSEL,  CTRU_EQU_CMPA )
        );
    
    EPwm1Regs.ETPS.all = 
        ( F280X_mCFGMSK( ETPS, SOCBPRD, 1 )
        | F280X_mCFGMSK( ETPS, SOCAPRD, 1 )
        | F280X_mCFGMSK( ETPS, INTPRD,  1 )
        );  

    // Setup the GPIO mux registers for proper disabling
    pwmdrv_fnConfigGpioDisabMode( ptzPwm ); 
    
#if( defined( DVT_OVERRIDE ) )
    // don't setup the trip zone
#else
    // Configure a trip zone pin for this module
    pwmdrv_fnSetGpioMuxForTripZone( PWMDRV_eTZ_3 );
#endif    
}

/*******************************************************************************

FUNCTION NAME:
    pwmdrv_fnCtorPwm2

PURPOSE:
    Setup PWM 2 module

INPUTS:
    none
   
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
static void pwmdrv_fnCtorPwm2( void )
{
    tzPWM* ptzPwm;
    
    // Assign a pointer to the PWM module being intialized
    ptzPwm = &atzPwmModule[ PWMDRV_eMODULE_2 ];
    
    // Configure the structure variables as required for the module
    ptzPwm->tpzPwmRegs = &EPwm2Regs;
    ptzPwm->tePinA     = PWMDRV_ePIN_2A;
    ptzPwm->teDisModeA = eDIS_MODE_FORCE_LOW;
    ptzPwm->teDisModeB = eDIS_MODE_NO_ACTION;
    ptzPwm->tePinModeA = ePIN_MODE_PWM;
    ptzPwm->tePinModeB = ePIN_MODE_GPIO;    
    
    // ***************************
    // Set up the Time Base Module
    // ***************************    
    EPwm2Regs.TBCTL.all = 
        ( F280X_mCFGMSK( TBCTL, FREESOFT,  FREE        )
        | F280X_mCFGMSK( TBCTL, PHSDIR,    COUNT_UP    )
        | F280X_mCFGMSK( TBCTL, CLKDIV,    DIVBY_1     )
        | F280X_mCFGMSK( TBCTL, HSPCLKDIV, DIVBY_1     ) 
        | F280X_mCFGMSK( TBCTL, SYNCOSEL,  EPWMXSYNCI  )
        | F280X_mCFGMSK( TBCTL, PRDLD,     SHDW        )
        | F280X_mCFGMSK( TBCTL, PHSEN,     ENA         )
        | F280X_mCFGMSK( TBCTL, CTRMODE,   COUNT_UP    )
        );
        
    // Set the period register
    EPwm2Regs.TBPRD = PWMDRV_PERIOD_REG_PWM2;
    EPwm2Regs.TBPHS.all = 0;      
      
    // *********************************
    // Set up the Counter Compare Module
    // *********************************          
    EPwm2Regs.CMPCTL.all =     
        ( F280X_mCFGMSK( CMPCTL, SHDWBMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, SHDWAMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, LOADBMODE, PRD  )
        | F280X_mCFGMSK( CMPCTL, LOADAMODE, PRD  )
        ); 
        
    // Since PWM 2 is set high when the count reaches CMPA, 
    // Set CMPA to the period reg plus 1 so that it doesn't trigger
    // until a proper value has been written to it. This stops the 
    // low side FET from coming on before we want it to
    EPwm2Regs.CMPA.half.CMPA = ( PWMDRV_PERIOD_REG_PWM1 + 1 );    
    EPwm2Regs.CMPB = ( PWMDRV_PERIOD_REG_PWM2 - PWMDRV_HIGH_SIDE_FET_RED );     
    
    // **********************************
    // Set up the Action Qualifier Module
    // **********************************           
    EPwm2Regs.AQCTLA.all = 
        ( F280X_mCFGMSK( AQCTLA, CBD, DIS )
        | F280X_mCFGMSK( AQCTLA, CBU, DIS )
        | F280X_mCFGMSK( AQCTLA, CAD, DIS )
        | F280X_mCFGMSK( AQCTLA, CAU, SET )
        | F280X_mCFGMSK( AQCTLA, PRD, CLR )
        | F280X_mCFGMSK( AQCTLA, ZRO, CLR )
        );
        
    EPwm2Regs.AQCTLB.all = 
        ( F280X_mCFGMSK( AQCTLB, CBD, DIS )
        | F280X_mCFGMSK( AQCTLB, CBU, DIS )
        | F280X_mCFGMSK( AQCTLB, CAD, DIS )
        | F280X_mCFGMSK( AQCTLB, CAU, DIS )
        | F280X_mCFGMSK( AQCTLB, PRD, DIS )
        | F280X_mCFGMSK( AQCTLB, ZRO, DIS )
        );          
        
    EPwm2Regs.AQSFRC.all =     
        ( F280X_mCFGMSK( AQSFRC, RLDCSF, LOAD_ON_ZRO )
        | F280X_mCFGMSK( AQSFRC, ACTSFB, DIS         )
        | F280X_mCFGMSK( AQSFRC, ACTSFA, DIS         )
        );
        
    EPwm2Regs.AQCSFRC.all =     
        ( F280X_mCFGMSK( AQCSFRC, CSFB, DIS )
        | F280X_mCFGMSK( AQCSFRC, CSFA, DIS )
        ); 
        
    // **************************
    // Setup the Dead Band Module
    // **************************
    EPwm2Regs.DBCTL.all = 
        ( F280X_mCFGMSK( DBCTL, POLSEL,   ACTIVE_HIGH )
        | F280X_mCFGMSK( DBCTL, OUT_MODE, ENA )
        );
        
    EPwm2Regs.DBRED = PWMDRV_LOW_SIDE_FET_RED; 
            
    // ********************************
    // Not using the PWM Chopper Module
    // ********************************
    
    // ***************************
    // Set up the Trip Zone Module
    // *************************** 
    // Trip zone registers are EALLOW protected
    DEVICE_mACCESS_EN( );
    
    EPwm2Regs.TZSEL.all = 
        ( F280X_mCFGMSK( TZSEL, OSHT6, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT5, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT4, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT3, ENA ) // LS FETs forced low on OCP
        | F280X_mCFGMSK( TZSEL, OSHT2, ENA ) // LS FETs forced low on NCD
        | F280X_mCFGMSK( TZSEL, OSHT1, DIS )
        | F280X_mCFGMSK( TZSEL, CBC6,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC5,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC4,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC3,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC2,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC1,  DIS )
        );
        
    EPwm2Regs.TZCTL.all = 
        ( F280X_mCFGMSK( TZCTL, TZB, DIS        ) 
        | F280X_mCFGMSK( TZCTL, TZA, FORCED_LOW )
        );
        
    EPwm2Regs.TZEINT.all = 
        ( F280X_mCFGMSK( TZEINT, OST, DIS )
        | F280X_mCFGMSK( TZEINT, CBC, DIS )
        );        
        
    DEVICE_mACCESS_DIS( );
        
    // ******************************
    // Setup the Event Trigger Module
    // ******************************  
    EPwm2Regs.ETSEL.all = 
        ( F280X_mCFGMSK( ETSEL, SOCBEN,  DIS           )
        | F280X_mCFGMSK( ETSEL, SOCBSEL, CTRU_EQU_CMPB )
        | F280X_mCFGMSK( ETSEL, SOCAEN,  DIS           )
        | F280X_mCFGMSK( ETSEL, SOCASEL, CTRU_EQU_CMPA )
        | F280X_mCFGMSK( ETSEL, INTEN,   DIS           )
        | F280X_mCFGMSK( ETSEL, INTSEL,  CTRU_EQU_CMPA )
        );
    
    EPwm2Regs.ETPS.all = 
        ( F280X_mCFGMSK( ETPS, SOCBPRD, 1 )
        | F280X_mCFGMSK( ETPS, SOCAPRD, 1 )
        | F280X_mCFGMSK( ETPS, INTPRD,  1 )
        );

    // Setup the GPIO mux registers for proper disabling
    pwmdrv_fnConfigGpioDisabMode( ptzPwm );

#if( defined( DVT_OVERRIDE ) )
    // don't setup the trip zone
#else    
    // Configure a trip zone pin for this module
    pwmdrv_fnSetGpioMuxForTripZone( PWMDRV_eTZ_2 );
#endif    
}

/*******************************************************************************

FUNCTION NAME:
    pwmdrv_fnCtorPwm3

PURPOSE:
    Setup PWM 3 module

INPUTS:
    none
   
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
static void pwmdrv_fnCtorPwm3( void )
{
    tzPWM* ptzPwm;
    
    // Assign a pointer to the PWM module being intialized
    ptzPwm = &atzPwmModule[ PWMDRV_eMODULE_3 ];
    
    // Configure the structure variables as required for the module
    ptzPwm->tpzPwmRegs = &EPwm3Regs;
    ptzPwm->tePinA     = PWMDRV_ePIN_3A;
    ptzPwm->teDisModeA = eDIS_MODE_NO_ACTION;
    ptzPwm->teDisModeB = eDIS_MODE_NO_ACTION;
    ptzPwm->tePinModeA = ePIN_MODE_GPIO;
    ptzPwm->tePinModeB = ePIN_MODE_GPIO;    
    
    // ***************************
    // Set up the Time Base Module
    // ***************************    
    EPwm3Regs.TBCTL.all = 
        ( F280X_mCFGMSK( TBCTL, FREESOFT,  FREE     )
        | F280X_mCFGMSK( TBCTL, PHSDIR,    COUNT_UP )
        | F280X_mCFGMSK( TBCTL, CLKDIV,    DIVBY_1  )
        | F280X_mCFGMSK( TBCTL, HSPCLKDIV, DIVBY_1  ) 
        | F280X_mCFGMSK( TBCTL, SYNCOSEL,  DIS      )
        | F280X_mCFGMSK( TBCTL, PRDLD,     SHDW     )
        | F280X_mCFGMSK( TBCTL, PHSEN,     ENA      )
        | F280X_mCFGMSK( TBCTL, CTRMODE,   COUNT_UP )
        );
        
    // Set the period register
    EPwm3Regs.TBPRD = PWMDRV_PERIOD_REG_PWM3;
    EPwm3Regs.TBPHS.all = 0;      
      
    // *********************************
    // Set up the Counter Compare Module
    // *********************************          
    EPwm3Regs.CMPCTL.all =     
        ( F280X_mCFGMSK( CMPCTL, SHDWBMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, SHDWAMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, LOADBMODE, PRD  )
        | F280X_mCFGMSK( CMPCTL, LOADAMODE, PRD  )
        );  
        
    // Set the default time in the period to cause SOCA and SOCB 
    // for ADC sequence 1 and 2. If desired, times can be changed
    // by calling PWMDRV_fnSetCompareAandB() for this PWM module
    EPwm3Regs.CMPA.half.CMPA = PWMDRV_DFLT_TIME_SOCA_SOCB;
    EPwm3Regs.CMPB           = PWMDRV_DFLT_TIME_CTRL_ISR;
        
    // **********************************
    // Set up the Action Qualifier Module
    // **********************************           
    EPwm3Regs.AQCTLA.all = 
        ( F280X_mCFGMSK( AQCTLA, CBD, DIS )
        | F280X_mCFGMSK( AQCTLA, CBU, DIS )
        | F280X_mCFGMSK( AQCTLA, CAD, DIS )
        | F280X_mCFGMSK( AQCTLA, CAU, DIS )
        | F280X_mCFGMSK( AQCTLA, PRD, DIS )
        | F280X_mCFGMSK( AQCTLA, ZRO, DIS )
        );
        
    EPwm3Regs.AQCTLB.all = 
        ( F280X_mCFGMSK( AQCTLB, CBD, DIS )
        | F280X_mCFGMSK( AQCTLB, CBU, DIS )
        | F280X_mCFGMSK( AQCTLB, CAD, DIS )
        | F280X_mCFGMSK( AQCTLB, CAU, DIS )
        | F280X_mCFGMSK( AQCTLB, PRD, DIS )
        | F280X_mCFGMSK( AQCTLB, ZRO, DIS )
        );      
        
    EPwm3Regs.AQSFRC.all =     
        ( F280X_mCFGMSK( AQSFRC, RLDCSF, LOAD_ON_ZRO )
        | F280X_mCFGMSK( AQSFRC, ACTSFB, DIS         )
        | F280X_mCFGMSK( AQSFRC, ACTSFA, DIS         )
        );
        
    EPwm3Regs.AQCSFRC.all =     
        ( F280X_mCFGMSK( AQCSFRC, CSFB, DIS )
        | F280X_mCFGMSK( AQCSFRC, CSFA, DIS )
        ); 
        
    // **************************
    // Setup the Dead Band Module
    // **************************
    EPwm3Regs.DBCTL.all = 
        ( F280X_mCFGMSK( DBCTL, POLSEL,   ACTIVE_HIGH )
        | F280X_mCFGMSK( DBCTL, OUT_MODE, DIS )
        );
    
    // ********************************
    // Not using the PWM Chopper Module
    // ********************************
    
    // ***************************
    // Set up the Trip Zone Module
    // *************************** 
    // Trip zone registers are EALLOW protected
    DEVICE_mACCESS_EN( );
    
    EPwm3Regs.TZSEL.all = 
        ( F280X_mCFGMSK( TZSEL, OSHT6, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT5, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT4, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT3, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT2, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT1, DIS )
        | F280X_mCFGMSK( TZSEL, CBC6,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC5,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC4,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC3,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC2,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC1,  DIS )
        );
        
    EPwm3Regs.TZCTL.all = 
        ( F280X_mCFGMSK( TZCTL, TZB, DIS ) 
        | F280X_mCFGMSK( TZCTL, TZA, DIS )
        );
        
    EPwm3Regs.TZEINT.all = 
        ( F280X_mCFGMSK( TZEINT, OST, DIS )
        | F280X_mCFGMSK( TZEINT, CBC, DIS )
        );        
        
    DEVICE_mACCESS_DIS( );
        
    // ******************************
    // Setup the Event Trigger Module
    // ******************************  
    EPwm3Regs.ETSEL.all = 
        ( F280X_mCFGMSK( ETSEL, SOCBEN,  DIS           ) // Enabled when needed
        | F280X_mCFGMSK( ETSEL, SOCBSEL, CTRU_EQU_CMPA )
        | F280X_mCFGMSK( ETSEL, SOCAEN,  DIS           ) // Enabled in FS Ctor
        | F280X_mCFGMSK( ETSEL, SOCASEL, CTRU_EQU_CMPA )
        | F280X_mCFGMSK( ETSEL, INTEN,   DIS           ) // Enabled in DigCtrl Ctor
        | F280X_mCFGMSK( ETSEL, INTSEL,  CTRU_EQU_CMPB )
        );
    
    EPwm3Regs.ETPS.all = 
        ( F280X_mCFGMSK( ETPS, SOCBPRD, 1 )
        | F280X_mCFGMSK( ETPS, SOCAPRD, 1 )
        | F280X_mCFGMSK( ETPS, INTPRD,  1 )
        );

    // Setup the GPIO mux registers for proper disabling
    pwmdrv_fnConfigGpioDisabMode( ptzPwm );
}

/*******************************************************************************

FUNCTION NAME:
    pwmdrv_fnCtorPwm4

PURPOSE:
    Setup PWM 4 module

INPUTS:
    none
   
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
static void pwmdrv_fnCtorPwm4( void )
{
    tzPWM* ptzPwm;
    
    // Assign a pointer to the PWM module being intialized
    ptzPwm = &atzPwmModule[ PWMDRV_eMODULE_4 ];
    
    // Configure the structure variables as required for the module
    ptzPwm->tpzPwmRegs = &EPwm4Regs;
    ptzPwm->tePinA     = PWMDRV_ePIN_4A;
    ptzPwm->teDisModeA = eDIS_MODE_HIGH_IMP;
    ptzPwm->teDisModeB = eDIS_MODE_NO_ACTION;
    ptzPwm->tePinModeA = ePIN_MODE_PWM;
    ptzPwm->tePinModeB = ePIN_MODE_GPIO;    
    
    // ***************************
    // Set up the Time Base Module
    // ***************************    
    EPwm4Regs.TBCTL.all = 
        ( F280X_mCFGMSK( TBCTL, FREESOFT,  FREE     )
        | F280X_mCFGMSK( TBCTL, PHSDIR,    COUNT_UP )
        | F280X_mCFGMSK( TBCTL, CLKDIV,    DIVBY_1  )
        | F280X_mCFGMSK( TBCTL, HSPCLKDIV, DIVBY_1  ) 
        | F280X_mCFGMSK( TBCTL, SYNCOSEL,  DIS      )
        | F280X_mCFGMSK( TBCTL, PRDLD,     SHDW     )
        | F280X_mCFGMSK( TBCTL, PHSEN,     DIS      )
        | F280X_mCFGMSK( TBCTL, CTRMODE,   COUNT_UP )
        );
        
    // Set the period register
    EPwm4Regs.TBPRD = PWMDRV_PERIOD_REG_PWM4;
    EPwm4Regs.TBPHS.all = 0;         
      
    // *********************************
    // Set up the Counter Compare Module
    // *********************************          
    EPwm4Regs.CMPCTL.all =     
        ( F280X_mCFGMSK( CMPCTL, SHDWBMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, SHDWAMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, LOADBMODE, PRD  )
        | F280X_mCFGMSK( CMPCTL, LOADAMODE, PRD  )
        );      
    
    // **********************************
    // Set up the Action Qualifier Module
    // **********************************           
    EPwm4Regs.AQCTLA.all = 
        ( F280X_mCFGMSK( AQCTLA, CBD, DIS )
        | F280X_mCFGMSK( AQCTLA, CBU, DIS )
        | F280X_mCFGMSK( AQCTLA, CAD, DIS )
        | F280X_mCFGMSK( AQCTLA, CAU, CLR )
        | F280X_mCFGMSK( AQCTLA, PRD, DIS )
        | F280X_mCFGMSK( AQCTLA, ZRO, SET )
        );
    
    EPwm4Regs.AQCTLB.all = 
        ( F280X_mCFGMSK( AQCTLB, CBD, DIS )
        | F280X_mCFGMSK( AQCTLB, CBU, DIS )
        | F280X_mCFGMSK( AQCTLB, CAD, DIS )
        | F280X_mCFGMSK( AQCTLB, CAU, DIS )
        | F280X_mCFGMSK( AQCTLB, PRD, DIS )
        | F280X_mCFGMSK( AQCTLB, ZRO, DIS )
        );    
        
    EPwm4Regs.AQSFRC.all =     
        ( F280X_mCFGMSK( AQSFRC, RLDCSF, LOAD_ON_ZRO )
        | F280X_mCFGMSK( AQSFRC, ACTSFB, DIS         )
        | F280X_mCFGMSK( AQSFRC, ACTSFA, DIS         )
        );
        
    EPwm4Regs.AQCSFRC.all =     
        ( F280X_mCFGMSK( AQCSFRC, CSFB, DIS )
        | F280X_mCFGMSK( AQCSFRC, CSFA, DIS )
        ); 
        
    // **************************
    // Setup the Dead Band Module
    // **************************
    EPwm4Regs.DBCTL.all = 
        ( F280X_mCFGMSK( DBCTL, POLSEL,   ACTIVE_HIGH )
        | F280X_mCFGMSK( DBCTL, OUT_MODE, DIS )
        );
    
    // ********************************
    // Not using the PWM Chopper Module
    // ********************************
    
    // ***************************
    // Set up the Trip Zone Module
    // *************************** 
    // Trip zone registers are EALLOW protected
    DEVICE_mACCESS_EN( );
    
    EPwm4Regs.TZSEL.all = 
        ( F280X_mCFGMSK( TZSEL, OSHT6, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT5, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT4, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT3, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT2, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT1, DIS )
        | F280X_mCFGMSK( TZSEL, CBC6,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC5,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC4,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC3,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC2,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC1,  DIS )
        );
        
    EPwm4Regs.TZCTL.all = 
        ( F280X_mCFGMSK( TZCTL, TZB, DIS ) 
        | F280X_mCFGMSK( TZCTL, TZA, DIS )
        );
        
    EPwm4Regs.TZEINT.all = 
        ( F280X_mCFGMSK( TZEINT, OST, DIS )
        | F280X_mCFGMSK( TZEINT, CBC, DIS )
        );        
        
    DEVICE_mACCESS_DIS( );
        
    // ******************************
    // Setup the Event Trigger Module
    // ******************************  
    EPwm4Regs.ETSEL.all = 
        ( F280X_mCFGMSK( ETSEL, SOCBEN,  DIS           )
        | F280X_mCFGMSK( ETSEL, SOCBSEL, CTRU_EQU_CMPB )
        | F280X_mCFGMSK( ETSEL, SOCAEN,  DIS           )
        | F280X_mCFGMSK( ETSEL, SOCASEL, CTRU_EQU_CMPA )
        | F280X_mCFGMSK( ETSEL, INTEN,   DIS           )
        | F280X_mCFGMSK( ETSEL, INTSEL,  CTRU_EQU_CMPA )
        );
    
    EPwm4Regs.ETPS.all = 
        ( F280X_mCFGMSK( ETPS, SOCBPRD, 1 )
        | F280X_mCFGMSK( ETPS, SOCAPRD, 1 )
        | F280X_mCFGMSK( ETPS, INTPRD,  1 )
        );

    // Setup the GPIO mux registers for proper disabling
    pwmdrv_fnConfigGpioDisabMode( ptzPwm );
}

/*******************************************************************************

FUNCTION NAME:
    pwmdrv_fnCtorPwm5

PURPOSE:
    Setup PWM 5 module

INPUTS:
    none
   
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
static void pwmdrv_fnCtorPwm5( void )
{
    tzPWM* ptzPwm;
    
    // Assign a pointer to the PWM module being intialized
    ptzPwm = &atzPwmModule[ PWMDRV_eMODULE_5 ];
    
    // Configure the structure variables as required for the module
    ptzPwm->tpzPwmRegs = &EPwm5Regs;
    ptzPwm->tePinA     = PWMDRV_ePIN_5A;
    ptzPwm->teDisModeA = eDIS_MODE_FORCE_LOW;
    ptzPwm->teDisModeB = eDIS_MODE_FORCE_LOW;
    ptzPwm->tePinModeA = ePIN_MODE_PWM;
    ptzPwm->tePinModeB = ePIN_MODE_PWM;    
    
    // ***************************
    // Set up the Time Base Module
    // ***************************    
    EPwm5Regs.TBCTL.all = 
        ( F280X_mCFGMSK( TBCTL, FREESOFT,  FREE     )
        | F280X_mCFGMSK( TBCTL, PHSDIR,    COUNT_UP )
        | F280X_mCFGMSK( TBCTL, CLKDIV,    DIVBY_1  )
        | F280X_mCFGMSK( TBCTL, HSPCLKDIV, DIVBY_1  ) 
        | F280X_mCFGMSK( TBCTL, SYNCOSEL,  DIS      )
        | F280X_mCFGMSK( TBCTL, PRDLD,     SHDW     )
        | F280X_mCFGMSK( TBCTL, PHSEN,     DIS      )
        | F280X_mCFGMSK( TBCTL, CTRMODE,   COUNT_UP )
        );
        
    // Set the period register
    EPwm5Regs.TBPRD = PWMDRV_PERIOD_REG_PWM5;
    EPwm5Regs.TBPHS.all = 0;         
      
    // *********************************
    // Set up the Counter Compare Module
    // *********************************          
    EPwm5Regs.CMPCTL.all =     
        ( F280X_mCFGMSK( CMPCTL, SHDWBMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, SHDWAMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, LOADBMODE, PRD  )
        | F280X_mCFGMSK( CMPCTL, LOADAMODE, PRD  )
        );      
    
    // **********************************
    // Set up the Action Qualifier Module
    // **********************************           
    EPwm5Regs.AQCTLA.all = 
        ( F280X_mCFGMSK( AQCTLA, CBD, DIS )
        | F280X_mCFGMSK( AQCTLA, CBU, DIS )
        | F280X_mCFGMSK( AQCTLA, CAD, DIS )
        | F280X_mCFGMSK( AQCTLA, CAU, CLR )
        | F280X_mCFGMSK( AQCTLA, PRD, DIS )
        | F280X_mCFGMSK( AQCTLA, ZRO, SET )
        );
    
    EPwm5Regs.AQCTLB.all = 
        ( F280X_mCFGMSK( AQCTLB, CBD, DIS )
        | F280X_mCFGMSK( AQCTLB, CBU, CLR )
        | F280X_mCFGMSK( AQCTLB, CAD, DIS )
        | F280X_mCFGMSK( AQCTLB, CAU, DIS )
        | F280X_mCFGMSK( AQCTLB, PRD, DIS )
        | F280X_mCFGMSK( AQCTLB, ZRO, SET )
        );    
        
    EPwm5Regs.AQSFRC.all =     
        ( F280X_mCFGMSK( AQSFRC, RLDCSF, LOAD_ON_ZRO )
        | F280X_mCFGMSK( AQSFRC, ACTSFB, DIS         )
        | F280X_mCFGMSK( AQSFRC, ACTSFA, DIS         )
        );
        
    EPwm5Regs.AQCSFRC.all =     
        ( F280X_mCFGMSK( AQCSFRC, CSFB, DIS )
        | F280X_mCFGMSK( AQCSFRC, CSFA, DIS )
        ); 
        
    // **************************
    // Setup the Dead Band Module
    // **************************
    EPwm5Regs.DBCTL.all = 
        ( F280X_mCFGMSK( DBCTL, POLSEL,   ACTIVE_HIGH )
        | F280X_mCFGMSK( DBCTL, OUT_MODE, DIS )
        );
    
    // ********************************
    // Not using the PWM Chopper Module
    // ********************************
    
    // ***************************
    // Set up the Trip Zone Module
    // *************************** 
    // Trip zone registers are EALLOW protected
    DEVICE_mACCESS_EN( );
    
    EPwm5Regs.TZSEL.all = 
        ( F280X_mCFGMSK( TZSEL, OSHT6, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT5, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT4, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT3, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT2, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT1, DIS )
        | F280X_mCFGMSK( TZSEL, CBC6,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC5,  DIS )
#if( defined( CTRL_CONFIG_OPTION_SURVEYOR_A ) \
  || defined( CTRL_CONFIG_OPTION_SURVEYOR_B ) )        
        | F280X_mCFGMSK( TZSEL, CBC4,  DIS )
#else
        | F280X_mCFGMSK( TZSEL, CBC4,  ENA )
#endif        
        | F280X_mCFGMSK( TZSEL, CBC3,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC2,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC1,  DIS )
        );
        
    EPwm5Regs.TZCTL.all = 
#if( defined( CTRL_CONFIG_OPTION_SURVEYOR_A ) \
  || defined( CTRL_CONFIG_OPTION_SURVEYOR_B ) )    
        ( F280X_mCFGMSK( TZCTL, TZB, DIS )
#else
        ( F280X_mCFGMSK( TZCTL, TZB, FORCED_LOW )
#endif         
        | F280X_mCFGMSK( TZCTL, TZA, DIS )
        );
        
    EPwm5Regs.TZEINT.all = 
        ( F280X_mCFGMSK( TZEINT, OST, DIS )
        | F280X_mCFGMSK( TZEINT, CBC, DIS )
        );        
        
    DEVICE_mACCESS_DIS( );
        
    // ******************************
    // Setup the Event Trigger Module
    // ******************************  
    EPwm5Regs.ETSEL.all = 
        ( F280X_mCFGMSK( ETSEL, SOCBEN,  DIS           )
        | F280X_mCFGMSK( ETSEL, SOCBSEL, CTRU_EQU_CMPB )
        | F280X_mCFGMSK( ETSEL, SOCAEN,  DIS           )
        | F280X_mCFGMSK( ETSEL, SOCASEL, CTRU_EQU_CMPA )
        | F280X_mCFGMSK( ETSEL, INTEN,   DIS           )
        | F280X_mCFGMSK( ETSEL, INTSEL,  CTRU_EQU_CMPA )
        );
    
    EPwm5Regs.ETPS.all = 
        ( F280X_mCFGMSK( ETPS, SOCBPRD, 1 )
        | F280X_mCFGMSK( ETPS, SOCAPRD, 1 )
        | F280X_mCFGMSK( ETPS, INTPRD,  1 )
        );

    // Setup the GPIO mux registers for proper disabling
    pwmdrv_fnConfigGpioDisabMode( ptzPwm );
    
#if( defined( CTRL_CONFIG_OPTION_SURVEYOR_A ) \
  || defined( CTRL_CONFIG_OPTION_SURVEYOR_B ) )
    // Do nothing
#else
    // Configure a trip zone pin for this module
    pwmdrv_fnSetGpioMuxForTripZone( PWMDRV_eTZ_4 );
#endif      
}

/*******************************************************************************

FUNCTION NAME:
    pwmdrv_fnCtorPwm6

PURPOSE:
    Setup PWM 6 module

INPUTS:
    none
   
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
static void pwmdrv_fnCtorPwm6( void )
{
    tzPWM* ptzPwm;
    
    // Assign a pointer to the PWM module being intialized
    ptzPwm = &atzPwmModule[ PWMDRV_eMODULE_6 ];
    
    // Configure the structure variables as required for the module
    ptzPwm->tpzPwmRegs = &EPwm6Regs;
    ptzPwm->tePinA     = PWMDRV_ePIN_6A;
    ptzPwm->teDisModeA = eDIS_MODE_FORCE_LOW;
    ptzPwm->tePinModeA = ePIN_MODE_PWM;
    
#if( defined( CTRL_CONFIG_OPTION_SURVEYOR_A ) \
  || defined( CTRL_CONFIG_OPTION_SURVEYOR_B ) )    
    ptzPwm->teDisModeB = eDIS_MODE_NO_ACTION;
    ptzPwm->tePinModeB = ePIN_MODE_GPIO;
      
#else
    ptzPwm->teDisModeB = eDIS_MODE_FORCE_LOW;
    ptzPwm->tePinModeB = ePIN_MODE_PWM;     
#endif      
    
    // ***************************
    // Set up the Time Base Module
    // ***************************    
    EPwm6Regs.TBCTL.all = 
        ( F280X_mCFGMSK( TBCTL, FREESOFT,  FREE     )
        | F280X_mCFGMSK( TBCTL, PHSDIR,    COUNT_UP )
        | F280X_mCFGMSK( TBCTL, CLKDIV,    DIVBY_1  )
        | F280X_mCFGMSK( TBCTL, HSPCLKDIV, DIVBY_1  ) 
        | F280X_mCFGMSK( TBCTL, SYNCOSEL,  DIS      )
        | F280X_mCFGMSK( TBCTL, PRDLD,     SHDW     )
        | F280X_mCFGMSK( TBCTL, PHSEN,     DIS      )
        | F280X_mCFGMSK( TBCTL, CTRMODE,   COUNT_UP )
        );
        
    // Set the period register
    EPwm6Regs.TBPRD = PWMDRV_PERIOD_REG_PWM6;
    EPwm6Regs.TBPHS.all = 0;         
      
    // *********************************
    // Set up the Counter Compare Module
    // *********************************          
    EPwm6Regs.CMPCTL.all =     
        ( F280X_mCFGMSK( CMPCTL, SHDWBMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, SHDWAMODE, SHDW )
        | F280X_mCFGMSK( CMPCTL, LOADBMODE, PRD  )
        | F280X_mCFGMSK( CMPCTL, LOADAMODE, PRD  )
        );      
    
    // **********************************
    // Set up the Action Qualifier Module
    // **********************************           
    EPwm6Regs.AQCTLA.all = 
        ( F280X_mCFGMSK( AQCTLA, CBD, DIS )
        | F280X_mCFGMSK( AQCTLA, CBU, DIS )
        | F280X_mCFGMSK( AQCTLA, CAD, DIS )
        | F280X_mCFGMSK( AQCTLA, CAU, CLR )
        | F280X_mCFGMSK( AQCTLA, PRD, DIS )
        | F280X_mCFGMSK( AQCTLA, ZRO, SET )
        );
    
#if( defined( CTRL_CONFIG_OPTION_SURVEYOR_A ) \
  || defined( CTRL_CONFIG_OPTION_SURVEYOR_B ) )    
    EPwm6Regs.AQCTLB.all = 
        ( F280X_mCFGMSK( AQCTLB, CBD, DIS )
        | F280X_mCFGMSK( AQCTLB, CBU, DIS )
        | F280X_mCFGMSK( AQCTLB, CAD, DIS )
        | F280X_mCFGMSK( AQCTLB, CAU, DIS )
        | F280X_mCFGMSK( AQCTLB, PRD, DIS )
        | F280X_mCFGMSK( AQCTLB, ZRO, DIS )
        ); 
#else
    EPwm6Regs.AQCTLB.all = 
        ( F280X_mCFGMSK( AQCTLB, CBD, DIS )
        | F280X_mCFGMSK( AQCTLB, CBU, CLR )
        | F280X_mCFGMSK( AQCTLB, CAD, DIS )
        | F280X_mCFGMSK( AQCTLB, CAU, DIS )
        | F280X_mCFGMSK( AQCTLB, PRD, DIS )
        | F280X_mCFGMSK( AQCTLB, ZRO, SET )
        );
#endif           
        
    EPwm6Regs.AQSFRC.all =     
        ( F280X_mCFGMSK( AQSFRC, RLDCSF, LOAD_ON_ZRO )
        | F280X_mCFGMSK( AQSFRC, ACTSFB, DIS         )
        | F280X_mCFGMSK( AQSFRC, ACTSFA, DIS         )
        );
        
    EPwm6Regs.AQCSFRC.all =     
        ( F280X_mCFGMSK( AQCSFRC, CSFB, DIS )
        | F280X_mCFGMSK( AQCSFRC, CSFA, DIS )
        ); 
        
    // **************************
    // Setup the Dead Band Module
    // **************************
    EPwm6Regs.DBCTL.all = 
        ( F280X_mCFGMSK( DBCTL, POLSEL,   ACTIVE_HIGH )
        | F280X_mCFGMSK( DBCTL, OUT_MODE, DIS )
        );
    
    // ********************************
    // Not using the PWM Chopper Module
    // ********************************
    
    // ***************************
    // Set up the Trip Zone Module
    // *************************** 
    // Trip zone registers are EALLOW protected
    DEVICE_mACCESS_EN( );
    
    EPwm6Regs.TZSEL.all = 
        ( F280X_mCFGMSK( TZSEL, OSHT6, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT5, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT4, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT3, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT2, DIS )
        | F280X_mCFGMSK( TZSEL, OSHT1, DIS )
        | F280X_mCFGMSK( TZSEL, CBC6,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC5,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC4,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC3,  DIS )
        | F280X_mCFGMSK( TZSEL, CBC2,  DIS )
#if( defined( CTRL_CONFIG_OPTION_SURVEYOR_A ) \
  || defined( CTRL_CONFIG_OPTION_SURVEYOR_B ) )        
        | F280X_mCFGMSK( TZSEL, CBC1,  DIS )
#else        
        | F280X_mCFGMSK( TZSEL, CBC1,  ENA )
#endif                
        );
        
    EPwm6Regs.TZCTL.all = 
        ( F280X_mCFGMSK( TZCTL, TZB, DIS )
#if( defined( CTRL_CONFIG_OPTION_SURVEYOR_A ) \
  || defined( CTRL_CONFIG_OPTION_SURVEYOR_B ) )         
        | F280X_mCFGMSK( TZCTL, TZA, DIS )
#else
        | F280X_mCFGMSK( TZCTL, TZA, FORCED_LOW )
#endif        
        );
        
    EPwm6Regs.TZEINT.all = 
        ( F280X_mCFGMSK( TZEINT, OST, DIS )
        | F280X_mCFGMSK( TZEINT, CBC, DIS )
        );        
        
    DEVICE_mACCESS_DIS( );
        
    // ******************************
    // Setup the Event Trigger Module
    // ******************************  
    EPwm6Regs.ETSEL.all = 
        ( F280X_mCFGMSK( ETSEL, SOCBEN,  DIS           )
        | F280X_mCFGMSK( ETSEL, SOCBSEL, CTRU_EQU_CMPB )
        | F280X_mCFGMSK( ETSEL, SOCAEN,  DIS           )
        | F280X_mCFGMSK( ETSEL, SOCASEL, CTRU_EQU_CMPA )
        | F280X_mCFGMSK( ETSEL, INTEN,   DIS           )
        | F280X_mCFGMSK( ETSEL, INTSEL,  CTRU_EQU_CMPA )
        );
    
    EPwm6Regs.ETPS.all = 
        ( F280X_mCFGMSK( ETPS, SOCBPRD, 1 )
        | F280X_mCFGMSK( ETPS, SOCAPRD, 1 )
        | F280X_mCFGMSK( ETPS, INTPRD,  1 )
        );

    // Setup the GPIO mux registers for proper disabling
    pwmdrv_fnConfigGpioDisabMode( ptzPwm );

#if( defined( CTRL_CONFIG_OPTION_SURVEYOR_A ) \
  || defined( CTRL_CONFIG_OPTION_SURVEYOR_B ) )
    // do nothing
#else
    // Configure a trip zone pin for this module
    pwmdrv_fnSetGpioMuxForTripZone( PWMDRV_eTZ_1 );    
#endif
}

/*******************************************************************************

FUNCTION NAME:
    pwmdrv_fnEventEnable
    
PURPOSE:
    Allows the application to enable/disable a PWM event such as SOCA or SOCB.
    
INPUTS:
    tePwmModule - The module that will cause the PWM event
    teEventType - SOCA, SOCB, PWM INT
    tEnable     - TRUE causes event to be enabled
                - FALSE causes event to be disabled
   
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
static void pwmdrv_fnEventEnable( PWMDRV_teMODULE tePwmModule,
                                  PWMDRV_teETEN   teEventType,
                                  tucBOOL         tEnable )
{
    uint16 CfgValue;
    volatile struct ETSEL_BITS* pEtselBits;
    
    // Perform range checking
    assert( tePwmModule >= PWMDRV_eMODULE_1 );
    assert( tePwmModule <  PWMDRV_eMODULE_TOTAL );
    assert( teEventType >= PWMDRV_eETEN_INT );
    assert( teEventType <= PWMDRV_eETEN_SOCB );
        
    // Assign a pointer to the ETSEL register
    pEtselBits = &( atzPwmModule[ tePwmModule ].tpzPwmRegs->ETSEL.bit );
    
    // Set a configuration value based on whether enabling or disablig
    if( tEnable != FALSE )
    {
        CfgValue = 1;
    }
    else
    {
        CfgValue = 0;
    }
    
    // Enable the specific event based on what was requested
    if( teEventType == PWMDRV_eETEN_INT )
    {
        pEtselBits->INTEN = CfgValue;
    }
    else if( teEventType == PWMDRV_eETEN_SOCA )
    {
        pEtselBits->SOCAEN = CfgValue; 
    }
    else
    {
        pEtselBits->SOCBEN = CfgValue; 
    }
}

/*******************************************************************************

FUNCTION NAME:
    pwmdrv_fnConfigGpioDisabMode
    
PURPOSE:
    Based on the requirements for the individual PWM pin, this function configs
    the GPIO data direction register so that when the PWM is disabled, it is
    converted to either a GPIO input (high impedance), or a GPIO output with
    a logic high or a logic low (configured in the Ctor call).

INPUTS:
    ptzPwm - pointer to the PWM module being configured
   
OUTPUTS:
    none
   
NOTES: 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
Version: 1.01    Date: May-06-2005   By: Jeff Fieldhouse
   - fixed for loop bug that caused it to only run once   
    
*******************************************************************************/
static void pwmdrv_fnConfigGpioDisabMode( tzPWM* ptzPwm )
{
    PWMDRV_tePIN  tePin;
    teDIS_MODE    teDisMode;
    uint16        i;
    uint32        TempMsk;
    uint32        TempCfg;    
    
    // Get the Pin and disable mode into local vars
    tePin      = ptzPwm->tePinA;
    teDisMode  = ptzPwm->teDisModeA;
    
    // Run this loop twice to configure both A and B pins
    for( i = 0; i <= 1; i++ )
    {               
        // Handle based on the desired disabling mode
        switch( teDisMode )
        {
            case eDIS_MODE_NO_ACTION:
                // Change the disable mode and the pin for the B output 
                teDisMode = ptzPwm->teDisModeB;     
                tePin++;                            
            
                // Redo the loop for the next pin               
                continue;
                            
            case eDIS_MODE_FORCE_LOW:
            case eDIS_MODE_FORCE_HIGH:
                // Get the mask and configuration for the gpio data direction 
                // register and set it as an output pin
                PWMDRV_mMSKCFG( 
                    TempMsk,
                    TempCfg,
                    PWMDRV_GPADIR_GPIO_MSK,
                    PWMDRV_GPADIR_GPIO_OUTPUT,
                    PWMDRV_mGPADIR_GPIO_POS( tePin ) );
                break;              
            
            case eDIS_MODE_HIGH_IMP:
                // Get the mask and configuration for the gpio data direction 
                // register and set it as an input pin
                PWMDRV_mMSKCFG( 
                    TempMsk,
                    TempCfg,
                    PWMDRV_GPADIR_GPIO_MSK,
                    PWMDRV_GPADIR_GPIO_INPUT,
                    PWMDRV_mGPADIR_GPIO_POS( tePin ) );
                break;          
        }               
        
        DEVICE_mACCESS_EN( );
                
        // Finally, setup the mux of the gpio
        GpioCtrlRegs.GPADIR.all &= ~TempMsk;
        GpioCtrlRegs.GPADIR.all |= TempCfg;
        
        DEVICE_mACCESS_DIS( );      
           
        // Change the disable mode and the pin for the B output 
        teDisMode = ptzPwm->teDisModeB;
        tePin++;                    
    }           
}

/*******************************************************************************

FUNCTION NAME:
    pwmdrv_fnSetGpioMuxForTripZone

PURPOSE:
    Configures the GPIO mux registers to set up specific pins as Trip Zone pins.

INPUTS:
    teTripZone - The trip zone pin to enable
   
OUTPUTS:
    none
   
NOTES:
    This function assumes that if you want to use Trip Zone 5 or 6 that you are
    using them on GPIO 16 and 17, not 28 and 29. 

VERSION HISTORY
Version: 1.00    Date: May-19-2005   By: Jeff Fieldhouse
   - Created
    
*******************************************************************************/
static void pwmdrv_fnSetGpioMuxForTripZone( PWMDRV_teTZ teTripZone )
{
    uint32 TempMsk;
    uint32 TempCfg;    
    
    DEVICE_mACCESS_EN( );  
      
    // The first 4 trip zone pins are on a different mux than the other 2
    if( teTripZone < PWMDRV_eTZ_5 )
    {
        // Get the mask and configuration for the gpio mux register
        // Set the gpio mux to pwm mode
        PWMDRV_mMSKCFG( 
            TempMsk,
            TempCfg,
            PWMDRV_GPAMUX1_GPIO_MSK,
            PWMDRV_GPAMUX1_GPIO_TZ,
            PWMDRV_mGPAMUX1_TZ_GPIO_POS( teTripZone ) );        
        
        // Setup the mux of the gpio
        GpioCtrlRegs.GPAMUX1.all &= ~TempMsk;
        GpioCtrlRegs.GPAMUX1.all |= TempCfg;
    }
    else
    {
        // Get the mask and configuration for the gpio mux register
        // Set the gpio mux to pwm mode
        PWMDRV_mMSKCFG( 
            TempMsk,
            TempCfg,
            PWMDRV_GPAMUX2_GPIO_MSK,
            PWMDRV_GPAMUX2_GPIO_TZ,
            PWMDRV_mGPAMUX2_TZ_GPIO_POS( teTripZone ) );        
        
        // Setup the mux of the gpio
        GpioCtrlRegs.GPAMUX2.all &= ~TempMsk;
        GpioCtrlRegs.GPAMUX2.all |= TempCfg;    
    }
        
    DEVICE_mACCESS_DIS( );
}
