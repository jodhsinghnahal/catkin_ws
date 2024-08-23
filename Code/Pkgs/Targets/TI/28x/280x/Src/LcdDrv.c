/*==============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  
    LcdDrv.c

PURPOSE:
    Provides an interface for displaying information on a multiline 16 character
    wide LCD. Also provides contrast, and backlight control. This driver has 
    been tailored to work with a 2-line display and is setup to use a 4-bit wide 
    data bus interface.  

FUNCTION(S):
    LCDDRV_fnCtor                      - Setup the driver
    LCDDRV_fnBackLightBlinkOn          - Enable/disable backlight blinking
    LCDDRV_fnBacklightStayOn           - Enable/disable backlight on
    LCDDRV_fnBacklightTimedStart       - Turn on backlight for set time
    LCDDRV_fnBacklightTimeGet          - Returns timed duration for backlight
    LCDDRV_fnBacklightTimeSet          - Sets timed duration for backlight
    LCDDRV_fnBackLightBlinkPeriodGetCs - Returns backlight blink period
    LCDDRV_fnBrightnessGet             - Returns backlight brightness level
    LCDDRV_fnBrightnessSet             - Sets backlight brightness level
    LCDDRV_fnClearDisplay              - Clear all characters from the display
    LCDDRV_fnContrastGet               - Returns contrast level
    LCDDRV_fnContrastSet               - Sets contrast level
    LCDDRV_fnDisplayArray              - Display an array of characters
    LCDDRV_fnDisplayChar               - Display one character
    LCDDRV_fnDisplayString             - Display an entire string
    LCDDRV_fnRestoreFactoryDflt        - Restore the config settings back to def
    LCDDRV_fnReadString                - Read string from the dispaly
    LCDDRV_fnTurnCursorBlinkOff        - Turn off blinking cursor
    LCDDRV_fnTurnCursorBlinkOn         - Displays a blinking cursor
    LCDDRV_fnTurnDisplayOff            - Turn the dislplay off
    LCDDRV_fnTurnDisplayOn             - Turn the dislplay on

    Local:                  
        lcddrv_fnInit               - Initialize the liquid crystal display    
        lcddrv_fnSetBacklight       - CLI function to set backlight on
        lcddrv_fnSetDdramAddress    - Sets the location to write or read from
        lcddrv_fnWriteCmd           - Writes a command to the LCD
        lcddrv_fnWriteData          - Writes a char to a specific RAM location
        lcddrv_fnSendData           - Hardware level specific data writes    
        lcddrv_fnGetData            - Hardware level specific data reads
        lcddrv_fnSendNibble         - Writes one nibble
        lcddrv_fnGetNibble          - Reads one nibble
   
NOTES:    
    This is ported from the HC12 series code , now coded to run on TI's 
    TMS320280X series processors.
    
    Timing and initialization procedure is referenced from Samsung S6A0069
    LCD controller.
 
CHANGE HISTORY:
$Log: LcdDrv.c $

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/6
    User: HueyD     Date: 06/21/06  Time: 05:57PM
    Fixed LCD backlight bug

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/5
    User: HueyD     Date: 05/24/06  Time: 11:50PM
    fixed backlight always on setting not working on startup.

    Moved blink period to .h for xb

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/4
    User: HueyD     Date: 03/08/06  Time: 12:43AM
    Added explantion for busy flag.

    Changed all waitforbusyflag functions to execute before accessing the LCD
    instead of after. This results in more efficient use of time.

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/3
    User: HueyD     Date: 03/07/06  Time: 11:20PM
    fixed backlight timer bug

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/2
    User: HueyD     Date: 02/27/06  Time: 06:29PM
    Fixed waitforbusyflag to work properly.

    simplified read func

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/1
    User: HueyD     Date: 02/21/06  Time: 06:40PM
    Backlight doesn't turn on during init

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/8
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/24
    User: HueyD     Date: 02/10/06  Time: 12:46AM
    Init pins so that lcd init would work

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/23
    User: HueyD     Date: 02/09/06  Time: 11:08PM
    Changed how backlight works

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/22
    User: HueyD     Date: 02/08/06  Time: 02:46AM
    Overhauled interface to LCD.

    Timing and algos are referenced to Samsung S6A0069 LCD controller.

    There were mysterious functions that seemingly should not work.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/21
    User: HueyD     Date: 01/21/06  Time: 12:10AM
    Added cond comp for cli

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/7
    User: JeffF     Date: 01/17/06  Time: 09:48PM
    Implemented a function to restore the factory default settings for this
    package.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/20
    User: HueyD     Date: 11/30/05  Time: 11:58PM
    Removed unused functions

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/19
    User: HueyD     Date: 11/24/05  Time: 06:04PM
    Fixed error in PWM/USER value conversion.

    Tightened contrast window so that text doesn't disappear

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/18
    User: HueyD     Date: 11/02/05  Time: 09:55PM
    merged

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/17
    User: HueyD     Date: 11/02/05  Time: 07:41PM
    Reorganized file.

    Updated comments.

    Added display backlight blinking/stay on/timed on feature.  Order of precedence
    shown.

    ***********************************************
    Revision: SurveyorPfrmA_HenryC/4
    User: HenryC     Date: 11/02/05  Time: 03:52PM
    Restored RS232 Backlight control and added CLI priority for SC-View.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/16
    User: HueyD     Date: 10/20/05  Time: 10:43PM
    Added a lot more screens

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/15
    User: HueyD     Date: 10/06/05  Time: 06:40PM
    Working copy of first pass screens

    ***********************************************
    Revision: SurveyorPfrmA_HenryC/3
    User: HenryC     Date: 08/30/05  Time: 11:42PM
    Added RS232 contrast control commands (inc dec save)

    ***********************************************
    Revision: SurveyorPfrmA_HenryC/2
    User: HenryC     Date: 08/09/05  Time: 07:34PM
    Added RS232 control for Backlight

    ***********************************************
    Revision: SurveyorPfrmA_JosephineT/1
    User: JosephineT     Date: 07/28/05  Time: 10:47PM
    - Removed unused variable in function fnDisplayChar()

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/11
    User: HueyD     Date: 07/25/05  Time: 11:15PM
    Got text editing to work

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/10
    User: HueyD     Date: 07/14/05  Time: 11:16PM
    merged

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/6
    User: KellyS     Date: 07/14/05  Time: 04:54PM
    Files were mistakenly anchored in my stream so I wasn't getting updates
    properly. This keep and promote will resolve that.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/9
    User: HueyD     Date: 07/13/05  Time: 08:51PM
    Removed Init from ctor as it needs the rtos to start running first.  Must
    be called in main now.

    ***********************************************
    Revision: SurveyorPfrmA_WaylandY/1
    User: WaylandY     Date: 07/12/05  Time: 12:26AM
    - added LCDDRV_fnReadString and lcddrv_fnReadData
    
==============================================================================*/

#define XASSERT_FILENAME  "LcdDrv" // For assert()

#define LCDDRV_COMPILE_CLI ( FALSE )

/*==============================================================================
                              Includes
==============================================================================*/
#include <string.h>        // String handling library
#include "includes.h"      // RTOS header files
#include "LIB_stdtype.h"   // Requires architecture definition in the compiler
#include "LIB_stddefs.h"   // Standard data types
#include "DigCtrl.h"       // Device interface
#include "Device.h"        // Device specific header files
#include "devicemacro.h"   // Device specific macros
#include "PwmDrv.h"        // PWM control
#include "xassert.h"       // Assert functionality 
#include "LcdDrv.h"        // PWM control
#include "GpioDrv.h"       // GPIO pin control
#include "ItemDef.h"       // EEprom structure defines
#include "Timer.h"         // GPIO pin control

#if( LCDDRV_COMPILE_CLI == TRUE )
#include "Cli.h"           // Command Line Interface
#endif


/*==============================================================================
                               Defines
==============================================================================*/

// These values are command instructions for the LCD
#define LCDDRV_CLR_DISPLAY         ( 0x01 ) // Clear the entire display
#define LCDDRV_RETURN_HOME         ( 0x02 ) // Set address counter back to 0x00
#define LCDDRV_ENTRY_MODE          ( 0x06 ) // Auto increment, disab shifting
#define LCDDRV_DISPLAY_OFF         ( 0x08 ) // Turn display off
#define LCDDRV_DISPLAY_ON          ( 0x0C ) // Turn display on-cursor blink off
#define LCDDRV_CURSORBLINK_ON      ( 0x0D ) // Turn on the blinking cursor
#define LCDDRV_CURSORBLINK_OFF     ( 0x0C ) // Turn off the cursor
#define LCDDRV_FUNCTION_SET        ( 0x28 ) // 4-bit interface, 2-line display
                                            // 5x8 dots/character
#define LCDDRV_8_DATA_LINES        ( 0x30 ) // Set LCD into 8-bit interface mode
#define LCDDRV_4_DATA_LINES        ( 0x20 ) // Set LCD into 4-bit interface mode

// Masks used for isolation one nibble at a time
#define LCDDRV_LOW_NIB_MSK         ( 0x0F )
#define LCDDRV_UPP_NIB_MSK         ( 0xF0 )

// Mask for checking the busy flag on the LCD
#define LCDDRV_BUSY_FLAG_MSK       ( 0x80 )

// The number of bits in a nibble
#define LCDDRV_SHIFT_NIBBLE        ( 4 )

// DDRAM addresses start at 0x80
#define LCDDRV_DDRAM_MSK           ( 0x80 )

// CGRAM address start at 0x40
#define LCDDRV_CGRAM_MSK           ( 0x40 )

// RAM locations on the second line are offset by 0x40
#define LCDDRV_1ST_LINE_MSK        ( 0 )
#define LCDDRV_2ND_LINE_MSK        ( 0x40 )

// Used to ensure the driver doesn't hang if the LCD isn't functioning properly
// if more than 3 milliseconds , something is wrong with the hardware
#define LCDDRV_BUSY_FLAG_TIMEOUT_FAST_OP_LP ( 10 ) // 1 while loop each count
#define LCDDRV_BUSY_FLAG_TIMEOUT_SLOW_OP_TK (  2 ) // non-blocking delay
                                                   // MUST BE > 1
                                                   // see lcddrv_fnWaitForBusyFlag()
												   // for explanation.
#define LCDDRV_BUSY_FLAG_TIMEOUT_CNTS ( LCDDRV_BUSY_FLAG_TIMEOUT_FAST_OP_LP \
                                      + LCDDRV_BUSY_FLAG_TIMEOUT_SLOW_OP_TK )

// Defines for backlight PWM
// The voltage to the LCD backlight should not exceed 4.51V; Cap the duty cycle.
#define LCDDRV_BACKLIGHT_OFF       ( 0 )


// Defines for TMS320280X Port

// GPIO mask for the LCD data bits
// The high bit is the MSB.

// Contains two bits of input for each field
// 3130 2928 2726 2524 2322 2120 1918 1716
// 0000 0000 1111 1111 0000 0000 0000 0000

#define LCDDRV_DATA_BITS_2FIELD_MASK ( 0x00FF0000 )

// Contains 1 bits of input for each field
// 31-28 27-24 23-20 19-16 15-12 11-8 7-4  3-0
// 0000  1111  0000  0000  0000  0000 0000 0000
#define LCDDRV_DATA_BITS_1FIELD_MASK  ( 0x0F000000 )

// Masks used for isolation one nibble at a time
#define LCDDRV_UPP_NIB_MSK         ( 0xF0 )
#define LCDDRV_LOW_NIB_MSK         ( 0x0F )


// Mask for checking the busy flag on the LCD
#define LCDDRV_BUSY_FLAG_MSK       ( 0x80 )

// Bool constants
#define LCDDRV_RAM                 ( 0x01 )
#define LCDDRV_REG                 ( 0x00 )

#define LCD_BACKLIGHT_PIN           PWMDRV_ePIN_5B
#define LCD_CONTRAST_PIN            PWMDRV_ePIN_4A
#define LCD_BACKLIGHT_PWM_MODULE    PWMDRV_eMODULE_5
#define LCD_CONTRAST_PWM_MODULE     PWMDRV_eMODULE_4


// This is an a simple linear relationship
#define LCDDRV_CONTRAST_SLOPE_RISE \
         ( sint32 )( LCDDRV_CONTRAST_PWM_DARK - LCDDRV_CONTRAST_PWM_LIGHT )
#define LCDDRV_CONTRAST_SLOPE_RUN  \
         ( sint32 )( LCDDRV_CONTRAST_USER_DARK - LCDDRV_CONTRAST_USER_LIGHT )
#define LCDDRV_CONTRAST_SLOPE \
         ( sint32 )( LCDDRV_CONTRAST_SLOPE_RISE / LCDDRV_CONTRAST_SLOPE_RUN )
#define LCDDRV_CONTRAST_OFFSET \
         ( sint32 )( LCDDRV_CONTRAST_PWM_DARK - \
         ( LCDDRV_CONTRAST_SLOPE * LCDDRV_CONTRAST_USER_DARK ) )

// Macros to calculate PWM and USER
#define LCDDRV_mCONTRAST_GET_PWM( User ) ( uint16 ) \
        ( ( sint32 ) User * LCDDRV_CONTRAST_SLOPE + LCDDRV_CONTRAST_OFFSET )
#define LCDDRV_mCONTRAST_PWM_TO_USER( Pwm ) ( uint16 ) \
        ( ( ( sint32 ) Pwm - LCDDRV_CONTRAST_OFFSET ) / LCDDRV_CONTRAST_SLOPE )

// Define the actual maximum values
#define LCDDRV_CONTRAST_PWM_MIN \
         ( LCDDRV_mCONTRAST_GET_PWM( LCDDRV_CONTRAST_PWM_DARK ) )
#define LCDDRV_CONTRAST_PWM_MAX \
         ( LCDDRV_mCONTRAST_GET_PWM( LCDDRV_CONTRAST_USER_LIGHT ) )


// This is an a simple linear relationship
// Get the slope.
// The equation will be y = mx + b where y=pwm and x=user
#define LCDDRV_BRIGHTNESS_SLOPE_RISE \
         ( sint32 )( LCDDRV_BRIGHTNESS_PWM_DIM - LCDDRV_BRIGHTNESS_PWM_BRIGHT )
#define LCDDRV_BRIGHTNESS_SLOPE_RUN \
         ( sint32 )( LCDDRV_BRIGHTNESS_USER_DIM - LCDDRV_BRIGHTNESS_USER_BRIGHT )
#define LCDDRV_BRIGHTNESS_SLOPE \
         ( sint32 )( LCDDRV_BRIGHTNESS_SLOPE_RISE / LCDDRV_BRIGHTNESS_SLOPE_RUN )
#define LCDDRV_BRIGHTNESS_OFFSET \
         ( sint32 )( LCDDRV_BRIGHTNESS_PWM_DIM - \
         ( LCDDRV_BRIGHTNESS_SLOPE * LCDDRV_BRIGHTNESS_USER_DIM ) )

// Macros to calculate PWM and USER
#define LCDDRV_mBRIGHTNESS_GET_PWM( User ) ( uint16 ) \
         ( ( sint32 ) User * LCDDRV_BRIGHTNESS_SLOPE + LCDDRV_BRIGHTNESS_OFFSET )
#define LCDDRV_mBRIGHTNESS_PWM_TO_USER( Pwm ) ( uint16 )\
         ( ( ( sint32 ) Pwm - LCDDRV_BRIGHTNESS_OFFSET ) / LCDDRV_BRIGHTNESS_SLOPE )

// Define the actual maximum values
#define LCDDRV_BRIGHTNESS_PWM_MIN \
         ( LCDDRV_mBRIGHTNESS_GET_PWM( LCDDRV_BRIGHTNESS_PWM_DARK  )  )
#define LCDDRV_BRIGHTNESS_PWM_MAX \
         ( LCDDRV_mBRIGHTNESS_GET_PWM( LCDDRV_BRIGHTNESS_USER_LIGHT ) )


/*==============================================================================
                           Type Definitions
==============================================================================*/
   
/*==============================================================================
                                Enums
==============================================================================*/

/*=============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                          Local/Private Constants
==============================================================================*/

#if( LCDDRV_COMPILE_CLI == TRUE )
static LCDDRV_teSTATUS lcddrv_fnSetBacklight( uint8 argC, char* argV[] );

static const CLI_tzINFO katzLcdCtrlCmds[ ] = 
{    
    { "BACKLIGHT",      CLI_eACCESS_BASIC,  lcddrv_fnSetBacklight    },
    END_CMD_LIST
};
#endif

/*==============================================================================
                          Local/Private Variables
==============================================================================*/    

static GPIODRV_tpHANDLE tpRegisterSelect;
static GPIODRV_tpHANDLE tpReadWrite;
static GPIODRV_tpHANDLE tpEclock;

static TIMER_tpHANDLE tpBacklightBlinkTimer;    // Backlight blinking timer
static TIMER_tpHANDLE tpBacklightTimedTimer;    // Backlight timed on timer
static uint16 ContrastLevel;                    // Display contrast Level
static uint16 BrightnessLevel;                  // Backlight brightness level
static tucBOOL BacklightOn;                     // Backlight is on or off
static tucBOOL tBacklightBlinkOn;               // Backlight blink enabled
static LCDDRV_teBACKLIGHT tBacklightTime;       // Backlight on time
               
/*==============================================================================
                      Local/Private Function Protoypes
==============================================================================*/

static void lcddrv_fnInit( void );
static void lcddrv_fnBackLightBlinkTimeout( void );
static void lcddrv_fnBacklightSet( tucBOOL Enable );
static void lcddrv_fnBackLightTimedOut( void );
static uchar8 lcddrv_fnGetData( tucBOOL Ram);
static LCDDRV_teSTATUS lcddrv_fnReadData( uchar8 *Data );
static void lcddrv_fnSendData( tucBOOL Ram, uchar8 Data );    
static void lcddrv_fnSetBacklightTimeout( LCDDRV_teBACKLIGHT Setting );
static LCDDRV_teSTATUS lcddrv_fnSetDdramAddress( LCDDRV_teLINE     LineNum,
                                                 LCDDRV_tePOSITION Position );
static LCDDRV_teSTATUS lcddrv_fnWaitForBusyFlag( void );
static LCDDRV_teSTATUS lcddrv_fnWriteCmd( uchar8 Command );
static LCDDRV_teSTATUS lcddrv_fnWriteData( uchar8 Data );

static void lcddrv_fnSendNibble( uchar8 Nibble );
static uchar8 lcddrv_fnGetNibble( void );
/*==============================================================================
                           Function Definitions
==============================================================================*/
/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnCtor   
  
PURPOSE: 
    Sets up the PWM's that control the LCD backlight, and the contrast 

INPUTS: 
    None

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 23-Dec-2003    By: Jeff Fieldhouse
    - Created
Version: 1.01    Date: 06-Jan-2004    By: Jeff Fieldhouse
    - Incorporated conditional compilation for proper backlight and contrast
      control. These two control board functions are dependent on the which
      rev of the control board is being used.
Version: 1.02    Date: 08-Jan-2004    By: Jeff Fieldhouse
    - Changed some stuff around to improve readability
    - Moved the calls to PWMMGR_fnCtor() to another file
Version: 1.03    Date: 23-Jun-2005    By: Huey Duong
    - Moved init of GPIOs from SendData to Ctor.  No need to call every time
Version: 1.04    Date: 13-Jul-2005    By: Huey Duong
    - Removed Init as it needs the rtos to be running first    
Version: 01.05    Date: 20-Jan-2006    By: Huey Duong
    - Cond comp cli
Version: 01.06    Date: 06-Mar-2006    By: Huey Duong
    - Fixed backlight timer bug    
Version: 01.07    Date: 23-May-2006    By: Huey Duong
    - Turned lcd on if setting was on    
*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnCtor( void )
{
    // Setup GPIO objects to toggle the LCD control pins as required
    if( GPIODRV_fnCtor( &tpRegisterSelect,
                        GPIODRV_eGPIO21,
                        GPIODRV_eDIR_OUTPUT,
                        GPIODRV_ePULLUP_DISAB,                                 
                        GPIODRV_eQUAL_SEL_6_SAMPLES ) < 0 )
    {
        assert( FALSE );
    }
    
    if( GPIODRV_fnCtor( &tpReadWrite,
                        GPIODRV_eGPIO22,
                        GPIODRV_eDIR_OUTPUT,
                        GPIODRV_ePULLUP_DISAB,                                 
                        GPIODRV_eQUAL_SEL_6_SAMPLES ) < 0 )
    {
        assert( FALSE );
    }
    
    if( GPIODRV_fnCtor( &tpEclock,
                        GPIODRV_eGPIO23,
                        GPIODRV_eDIR_OUTPUT,
                        GPIODRV_ePULLUP_DISAB,                                 
                        GPIODRV_eQUAL_SEL_6_SAMPLES ) < 0 )
    {
        assert( FALSE );
    }

    // Turn on pwm's for Backlight
    PWMDRV_fnPinOutputEn( LCD_BACKLIGHT_PIN );

    // Turn on pwm's for Contrast
    PWMDRV_fnPinOutputEn( LCD_CONTRAST_PIN );   
    
    // Setup GPIO pins
    DEVICE_mACCESS_EN();                                        
     
    // Set the LCD data bits to GPIO pins
    GpioCtrlRegs.GPAMUX2.all &= ~LCDDRV_DATA_BITS_2FIELD_MASK;

    // The pullup resistors are disabled ( disable = 1 )     
    GpioCtrlRegs.GPAPUD.all |= LCDDRV_DATA_BITS_1FIELD_MASK;

    // Set the input qualification type to be sync to SYSCLKOUT
    GpioCtrlRegs.GPAQSEL2.all &= ~LCDDRV_DATA_BITS_2FIELD_MASK;

    DEVICE_mACCESS_DIS();
    
#if( LCDDRV_COMPILE_CLI == TRUE )
    // register the commands with the CMD module
    ( void )CLI_fnAddCommands( katzLcdCtrlCmds );
#endif


    // Initialize all backlight settings to off
    tBacklightBlinkOn = FALSE;

    lcddrv_fnBacklightSet( FALSE );

    // Create a timer object to control backlight blinking when enabled
    if( TIMER_fnCtor( &tpBacklightBlinkTimer,
                      TIMER_eMODE_CONTINUOUS,
                      TIMER_eCALLBACK_TASK,
                      LCDDRV_BACKLIGHT_BLINK_HALF_PERIOD,
                      lcddrv_fnBackLightBlinkTimeout ) < 0 )
    {
        assert( FALSE );
        return( LCDDRV_eSTATUS_FAILURE );
    }

    // Create the backlight timer with default settings
    // Set value to something other than 0
    if( TIMER_fnCtor( &tpBacklightTimedTimer,
                      TIMER_eMODE_SINGLESHOT,
                      TIMER_eCALLBACK_TASK,
                      1000,
                      lcddrv_fnBackLightTimedOut ) < 0 )
    {
        assert( FALSE );
    } 

    // Retrieve the contrast setting from the eeprom
    if(  ( NVMEM_fnRead( ITEMDEF_eID_LCDDRV_CONTRAST, &ContrastLevel ) < 0 )
	  || ( ContrastLevel < LCDDRV_CONTRAST_USER_LIGHT )
	  || ( ContrastLevel > LCDDRV_CONTRAST_USER_DARK  ) )
	   
    {
        // Set contrast to default and save to EEPROM
        LCDDRV_fnContrastSet( LCDDRV_CONTRAST_USER_DEF, TRUE );
    }
    else
    {
        // Set contrast to saved setting
        LCDDRV_fnContrastSet( ContrastLevel, FALSE );
    }

    // Retrieve the brightness level from nvmem
    if(  ( NVMEM_fnRead( ITEMDEF_eID_LCDDRV_BRIGHTNESS, &BrightnessLevel ) < 0 )
	  || ( BrightnessLevel < LCDDRV_BRIGHTNESS_USER_DIM )
	  || ( BrightnessLevel > LCDDRV_BRIGHTNESS_USER_BRIGHT ) )
    {
         // Set the brightness level to default and save to NVMEM
         LCDDRV_fnBrightnessSet( LCDDRV_BRIGHTNESS_USER_DEF, TRUE );
    }
    else
    {
        // Set brightness to saved setting
        LCDDRV_fnBrightnessSet( BrightnessLevel, FALSE );
    }

    // Get the backlight hold time
    if(  ( NVMEM_fnRead( ITEMDEF_eID_LCDDRV_BACKLIGHT_S, &tBacklightTime ) <  0 )
	  || ( tBacklightTime > LCDDRV_eBACKLIGHT_ON ) )
    {
        // Set the backlight hold to default and save to NVMEM
        tBacklightTime = LCDDRV_eBACKLIGHT_DEF;
        ( void )NVMEM_fnWrite( ITEMDEF_eID_LCDDRV_BACKLIGHT_S, 
                               &tBacklightTime );     
    }

    // Update the timeout for the backlight so that when it is started,
    // the correct time is set.  If the setting is always on, then turn on
    // backlight
    if( tBacklightTime == LCDDRV_eBACKLIGHT_ON )
    {
        lcddrv_fnBacklightSet( TRUE );        
    }
    else
    {
        lcddrv_fnSetBacklightTimeout( tBacklightTime );
    }
    
    GpioDataRegs.GPADAT.bit.GPIO21 = 0;
    GpioDataRegs.GPADAT.bit.GPIO22 = 0;
    GpioDataRegs.GPADAT.bit.GPIO23 = 0;
    GpioDataRegs.GPADAT.bit.GPIO24 = 0;
    GpioDataRegs.GPADAT.bit.GPIO25 = 0;
    GpioDataRegs.GPADAT.bit.GPIO26 = 0;
    GpioDataRegs.GPADAT.bit.GPIO27 = 0;

    // Run the initializtion routing for the LCD
    lcddrv_fnInit( );

    return( LCDDRV_eSTATUS_SUCCESS ); 
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnBackLightBlinkOn   
  
PURPOSE: 
    Enables/disables the display's backlight to blink

INPUTS: 
    Enable - Whether to enable/disable blinking

OUTPUTS:
    return - LCDDRV_eSTATUS_SUCCESS on success

NOTES:
    3 functions are closely related in terms of display backlight
        LCDDRV_fnBacklightBlinkOn
        LCDDRV_fnBacklightStayOn
        LCDDRV_fnBacklightTimedStart

    They have a hierchial relationship where Blinking has higher priority than
    staying on than timed backlight

VERSION HISTORY:

Version: 01.00    Date: 02-Nov-2005    By: Huey Duong
    - Created

*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnBackLightBlinkOn( tucBOOL Enable )
{
    if( tBacklightBlinkOn == Enable )
    {
        // already in the state, do nothing
        return( LCDDRV_eSTATUS_SUCCESS );
    }

    // Update the blink state
    tBacklightBlinkOn = Enable;
    
    if( Enable == TRUE )
    {
        // Turn on the backlight and start the timer no matter what.
        // Blink has highest priority
        lcddrv_fnBacklightSet( TRUE );
        TIMER_fnStart( tpBacklightBlinkTimer );
    }
    else
    {
        // Stop 
        TIMER_fnStop( tpBacklightBlinkTimer );

        // Turn back on if stayon is set or timedout is enabled
        if(  ( tBacklightTime == LCDDRV_eBACKLIGHT_ON ) 
          || ( TIMER_fnIsActive( tpBacklightTimedTimer ) ) )
        {
            lcddrv_fnBacklightSet( TRUE );
        }
        else
        {
            // No other features are keeping the light on
            lcddrv_fnBacklightSet( FALSE );
        }
    }

    return( LCDDRV_eSTATUS_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnBacklightTimedStart   
  
PURPOSE: 
    Tells the display to turn on for a predeterminted duration if configured
    for timed on

INPUTS: 
    none

OUTPUTS:
    return - LCDDRV_eSTATUS_SUCCESS on success

NOTES:
    2 functions are closely related in terms of display backlight
        LCDDRV_fnBacklightBlinkOn
        LCDDRV_fnBacklightTimedStart

    They have a hierchial relationship where Blinking has higher priority than
    staying on than timed backlight

VERSION HISTORY:

Version: 01.00    Date: 02-Nov-2005    By: Huey Duong
    - Created
Version: 02.00    Date: 09-Feb-2006    By: Huey Duong
    - Changed structure of backlight on timer
*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnBacklightTimedStart( void )
{
    // Return if not configured for timed on
    // Return if blinking is enabled
    if(  ( tBacklightTime == LCDDRV_eBACKLIGHT_OFF )
      || ( tBacklightTime == LCDDRV_eBACKLIGHT_ON  )
      || ( tBacklightBlinkOn == TRUE ) )
    {
        return( LCDDRV_eSTATUS_SUCCESS );
    }
    
    // Start the timer and turn on the backlight
    lcddrv_fnBacklightSet( TRUE );
    TIMER_fnStart( tpBacklightTimedTimer );

    return( LCDDRV_eSTATUS_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnBacklightTimerGetS   
  
PURPOSE:
    Return the backlight hold time

INPUTS: 
    None

OUTPUTS:
    return - backlight hold time in seconds.  0 = off
    
NOTES:

VERSION HISTORY:

Version: 01.00    Date: 01-Nov-2005    By: Huey Duong
    - Created

*******************************************************************************/
LCDDRV_teBACKLIGHT LCDDRV_fnBacklightTimeGet( void )
{
    return( tBacklightTime );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnBacklightTimerSetS   
  
PURPOSE:
    Sets the backlight hold time in seconds

INPUTS: 
    Seconds - Backlight hold time.  0 for off

OUTPUTS:
    None
    
NOTES:

VERSION HISTORY:

Version: 01.00    Date: 01-Nov-2005    By: Huey Duong
    - Moved and renamed from display
Version: 01.01    Date: 20-Feb-2006    By: Huey Duong
    - Removed saving option    
Version: 01.02    Date: 06-Mar-2006    By: Huey Duong
    - Used common func to update timeout    
*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnBacklightTimeSet( LCDDRV_teBACKLIGHT Setting )
{
    // ensure value is valid
    tBacklightTime = Setting;
    
    // Save to NVMEM
    ( void )NVMEM_fnWrite( ITEMDEF_eID_LCDDRV_BACKLIGHT_S, 
                           &tBacklightTime );     

    // if we are blinking, do nothing
    if( tBacklightBlinkOn == TRUE )
    {
        return( LCDDRV_eSTATUS_SUCCESS );
    }
    
    switch( Setting )
    {
        case LCDDRV_eBACKLIGHT_OFF:
            // Turn off backlight
			TIMER_fnStop( tpBacklightTimedTimer );
            lcddrv_fnBacklightSet( FALSE );
            break;
            
        case LCDDRV_eBACKLIGHT_ON:
            // Turn on backlight
			TIMER_fnStop( tpBacklightTimedTimer );
            lcddrv_fnBacklightSet( TRUE );
            break;

        // For all other settings, turn on the backlight,
        // update the off timer and start it
        case LCDDRV_eBACKLIGHT_5S:            
        case LCDDRV_eBACKLIGHT_10S:
        case LCDDRV_eBACKLIGHT_30S:            
        case LCDDRV_eBACKLIGHT_1M:            
        case LCDDRV_eBACKLIGHT_5M:            
            lcddrv_fnBacklightSet( TRUE );
            lcddrv_fnSetBacklightTimeout( Setting );
			TIMER_fnStart( tpBacklightTimedTimer );
            break;

        default:
            return( LCDDRV_eSTATUS_FAILURE );
    }
    
    return( LCDDRV_eSTATUS_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnBackLightBlinkPeriodGetCs
  
PURPOSE: 
    Return the backlight blink (XanBus identify) in centi seconds

INPUTS: 
    None

OUTPUTS:
    return - the blink period in centi seconds

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 28-Apr-2004    By: Jeff Fieldhouse
    - Created
Version: 01.01    Date: 02-Nov-2005    By: Huey Duong
    - Changed function name

*******************************************************************************/
uint16 LCDDRV_fnBackLightBlinkPeriodGetCs( void )
{
    // Return the blink period by doubling the half period, and dividing by 10
    // ie. 100 = 1 sec
    return( ( 2 * LCDDRV_BACKLIGHT_BLINK_HALF_PERIOD ) / 10  );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnBrightnessGet   
  
PURPOSE: 
    Returns the user brightness level

INPUTS: 
    none

OUTPUTS:
    return - user brightness level

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 11-Oct-2005    By: Huey Duong
    - Created

*******************************************************************************/
uint16 LCDDRV_fnBrightnessGet( void )
{
    return( BrightnessLevel );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnBrightnessSet   
  
PURPOSE: 
    Sets the user brighness level

INPUTS: 
    Level - User brightness level to set
    Save  - Whether to save value to NVMEM or not

OUTPUTS:
    return - LCDDRV_teSTATUS_SUCCESS
             LCDDRV_teSTATUS_FAILURE on parameter error

NOTES:
    TBD.. Must consider how this interacts with blinking

VERSION HISTORY:

Version: 01.00    Date: 11-Oct-2005    By: Huey Duong
    - Created

*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnBrightnessSet( uint16  Level,
                                        tucBOOL Save )
{
    // Check parameter
    if(  ( Level < LCDDRV_BRIGHTNESS_USER_MIN )
      || ( Level > LCDDRV_BRIGHTNESS_USER_MAX ) )
    {
        return( LCDDRV_eSTATUS_FAILURE );
    }

    // Update the contrast
    BrightnessLevel = Level;

    // Update the brightness if the backlight is on
    if( BacklightOn == TRUE )
    {
        PWMDRV_fnSetCompareB( LCD_BACKLIGHT_PWM_MODULE, 
                              LCDDRV_mBRIGHTNESS_GET_PWM( BrightnessLevel ) );
    }

    // Save to NVMEM if requested
    if( Save == TRUE )
    {
        ( void )NVMEM_fnWrite( ITEMDEF_eID_LCDDRV_BRIGHTNESS, &BrightnessLevel );     
    }

    return( LCDDRV_eSTATUS_SUCCESS );
    
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnClearDisplay  
  
PURPOSE: 
    Writes the ' ' character to all RAM locations on the LCD, which effectively
    clears the display of all existing text.

INPUTS: 
    None

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 08-Dec-2003    By: Jeff Fieldhouse
    - Created

*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnClearDisplay( void )
{
    // Send the command to clear the display
    return( lcddrv_fnWriteCmd( LCDDRV_CLR_DISPLAY ) );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnContrastGet   
  
PURPOSE: 
    Returns the contrast level

INPUTS: 
    none

OUTPUTS:
    return - contrast level

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 11-Oct-2005    By: Huey Duong
    - Created
*******************************************************************************/
uint16 LCDDRV_fnContrastGet( void )
{
    // Return the contrast level
    return( ContrastLevel );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnContrastSet   
  
PURPOSE: 
    Sets the contrast level

INPUTS: 
    Level - User level of contrast
    Save  - Whether to store value in EEPROM or not

OUTPUTS:
    LCDDRV_eSTATUS_SUCCESS - Function success
    LCDDRV_eSTATUS_FAILURE - Parameter error

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 11-Oct-2005    By: Huey Duong
    - Created
*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnContrastSet( uint16  Level,
                                      tucBOOL Save )
{
    // Check parameter
    if(  ( Level < LCDDRV_CONTRAST_USER_MIN )
      || ( Level > LCDDRV_CONTRAST_USER_MAX ) )
    {
        return( LCDDRV_eSTATUS_FAILURE );
    }

    // Update the contrast
    ContrastLevel = Level;
    PWMDRV_fnSetCompareA( LCD_CONTRAST_PWM_MODULE, 
                          LCDDRV_mCONTRAST_GET_PWM( ContrastLevel ) );

    // Save to NVMEM if requested
    if( Save == TRUE )
    {
        ( void )NVMEM_fnWrite( ITEMDEF_eID_LCDDRV_CONTRAST, &ContrastLevel );     
    }

    return( LCDDRV_eSTATUS_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnDisplayArray   
  
PURPOSE: 
    Write an entire array to the LCD. The user specifies which line and the
    starting position on that line as well as the number of characters

INPUTS: 
    DispArray - A pointer to an array
    Count     - Number of characters to display
    LineNum   - Specifies which line to display the array on
    Position  - Specifies the starting position of the array

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 13-Sep-2003    By: Huey Duong
    - Created

*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnDisplayArray( 
                const uchar8*     DispArray,
                uchar8            Count,    
                LCDDRV_teLINE     LineNum,  
                LCDDRV_tePOSITION Position )
{
    uchar8 CharCounter;
    LCDDRV_teSTATUS Status;

    // Check for an appropriate string length
    if( Count == 0 )
    {
        // Nothing to display so return with a warning
        return( LCDDRV_eSTATUS_WARNING_ZERO_STR_LEN );
    }
    else if( ( Count + Position ) > LCDDRV_ePOSITION_MAX )
    {
        // The string is too long to display from that starting position
        assert( FALSE );
        return( LCDDRV_eSTATUS_FAILURE_STR_TOO_LONG );
    }  
    
    // Set the RAM location to start writing to
    Status = lcddrv_fnSetDdramAddress( LineNum, Position );
    
    if( Status != LCDDRV_eSTATUS_SUCCESS )
    {
        // The location was invalid, so return with an error
        assert( FALSE );
        return( Status );
    }

    // Start sending the string to the display
    for( CharCounter = 0; CharCounter < Count; CharCounter++ )
    {
        // Write one character at a time
        Status = lcddrv_fnWriteData( DispArray[ CharCounter ] );

        // Check status of the write operation
        if( Status < 0 )
        {
            // Break out of the for loop if there was a failure
            // Any warnings are ignored
            break;
        }
    }

    // Reset ram address
    lcddrv_fnSetDdramAddress( LineNum, Position );

    // Return status
    return( Status );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnDisplayChar   
  
PURPOSE: 
    Display a character only

INPUTS: 
    Character  - Character to dislay
    LineNum    - Specifies which line to display the character
    Position   - Specifies the position of the character

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 07-25-2005    By: Huey Duong
    - Created

*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnDisplayChar( schar8            Character,
                                      LCDDRV_teLINE     LineNum,
                                      LCDDRV_tePOSITION Position )
{
    LCDDRV_teSTATUS Status;
    
    if( Position > LCDDRV_ePOSITION_MAX )
    {
        // The string is too long to display from that starting position
        assert( FALSE );
        return( LCDDRV_eSTATUS_FAILURE_INVLD_POSITION );
    }  
    
    // Set the RAM location to start writing to
    Status = lcddrv_fnSetDdramAddress( LineNum, Position );
    
    if( Status != LCDDRV_eSTATUS_SUCCESS )
    {
        // The location was invalid, so return with an error
        assert( FALSE );
        return( Status );
    }

    // Write one character
    Status = lcddrv_fnWriteData( Character );

    // Check status of the write operation
    if( Status < 0 )
    {
        return( Status );
    }

    // Reset ram address
    lcddrv_fnSetDdramAddress( LineNum, Position );

    return( Status );
} // LCDDRV_fnDisplayCharacter

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnDisplayString   
  
PURPOSE: 
    Write an entire string to the LCD. The user specifies which line and the
    starting position on that line.

INPUTS: 
    DispString - A pointer to the beginning of the null terminated string
    LineNum    - Specifies which line to display the string on
    Position   - Specifies the starting position of the string

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:
    The string must be NULL terminated

VERSION HISTORY:

Version: 1.00    Date: 08-Dec-2003    By: Jeff Fieldhouse
    - Created
Version: 1.01    Date: 17-Dec-2003    By: Jeff Fieldhouse
    - Improved error handling

*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnDisplayString( const uchar8*     DispString,
                                        LCDDRV_teLINE     LineNum,
                                        LCDDRV_tePOSITION Position )
{
    uchar8 StrLength;

    StrLength = ( uchar8 )strlen( ( char* )DispString );

    // Display the string
    return( LCDDRV_fnDisplayArray( DispString, 
                                   StrLength, 
                                   LineNum, 
                                   Position ) );

} // LCDDRV_fnDisplayString

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnRestoreFactoryDflt   
  
PURPOSE: 
    Restores all the user configurable settings back to the factory default.

INPUTS: 
    None

OUTPUTS:
    None 

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 20-Apr-2004    By: Jeff Fieldhouse
    - Created
Version: 1.01    Date: 17-Jan-2006    By: Jeff Fieldhouse
    - Implemented with function calls to reset the configurable settings    

*******************************************************************************/
void LCDDRV_fnRestoreFactoryDflt( void )
{    
    // Set contrast to default and save to EEPROM
    LCDDRV_fnContrastSet( LCDDRV_CONTRAST_USER_DEF, TRUE );

    // Set the brightness level to default and save to NVMEM
    LCDDRV_fnBrightnessSet( LCDDRV_BRIGHTNESS_USER_DEF, TRUE );

    // Set the backlight hold to default and save to NVMEM
    LCDDRV_fnBacklightTimeSet( LCDDRV_eBACKLIGHT_DEF ); 
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnReadString   
  
PURPOSE: 
    Read an entire string to the LCD. The user specifies which line, the
    starting position on that line and the number of characters to read. 

INPUTS: 
    ReadString - A pointer to the beginning of the string to be read
    LineNum    - Specifies which line to display the string on
    Position   - Specifies the starting position of the string
    BuffSize   - Specifies the number of characters to read

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:
    The string must be NULL terminated

    Memory must be allocated for the char pointer.

VERSION HISTORY:

Version: 1.00    Date: 11-JUL-2005    By: Wayland Yu
    - Created

*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnReadString( uchar8*           ReadString,
                                     LCDDRV_teLINE     LineNum,
                                     LCDDRV_tePOSITION Position,
                                     uint16            BuffSize )
{
    // local variables
    uchar8 CharCounter;
    LCDDRV_teSTATUS Status;

    // Validate the pointer
    if( ReadString == NULL )
    {
        assert( FALSE );
        return( LCDDRV_eSTATUS_FAILURE );
    }
            
    if( ( Position + BuffSize ) > ( LCDDRV_ePOSITION_MAX + 1 ))
    {
        // The string is too long to read from that starting position
        assert( FALSE );
        return( LCDDRV_eSTATUS_FAILURE_STR_TOO_LONG );
    }  
    
    // Set the RAM location to start reading from
    Status = lcddrv_fnSetDdramAddress( LineNum, Position );
    
    if( Status != LCDDRV_eSTATUS_SUCCESS )
    {
        // The location was invalid, so return with an error
        assert( FALSE );
        return( Status );
    }

    // Start sending the string to the display
    for( CharCounter = 0; CharCounter < BuffSize-1; CharCounter++ )
    {
        // Read in one character
        Status = lcddrv_fnReadData( &ReadString[ CharCounter ] );

        // Check status of the write operation
        if( Status < 0 )
        {
            // Break out of the for loop if there was a failure
            // Any warnings are ignored
            break;
        }
    }
    
    // Reset RAM location
    lcddrv_fnSetDdramAddress( LineNum, Position );
    
    // Terminate with a NULL character
    ReadString[ CharCounter ] = '\0';

    return( Status );
} // LCDDRV_fnReadString

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnTurnCursorBlinkOff
  
PURPOSE: 
    Turn the cursor blink off

INPUTS: 
    none

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:
    This automatically turns on the display

VERSION HISTORY:

Version: 1.00    Date: 08-Jun-2005    By: Huey Duong
    - Created

*******************************************************************************/

LCDDRV_teSTATUS LCDDRV_fnTurnCursorBlinkOff( void )
{
    // Turn the cursor off
    return( lcddrv_fnWriteCmd( LCDDRV_CURSORBLINK_OFF ) );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnTurnCursorBlinkOn
  
PURPOSE: 
    Displays a blinking curor

INPUTS: 
    LineNum    - Specifies which line to display the cursor
    Position   - Specifies the horizontal position to display the cursor

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:
    This automatically turns on the display

VERSION HISTORY:

Version: 1.00    Date: 08-Jun-2005    By: Huey Duong
    - Created

*******************************************************************************/

LCDDRV_teSTATUS LCDDRV_fnTurnCursorBlinkOn( LCDDRV_teLINE     LineNum,    
                                            LCDDRV_tePOSITION Position )
{
    LCDDRV_teSTATUS Status;
    
    // Set the RAM location to diaplay the cursor
    Status = lcddrv_fnSetDdramAddress( LineNum, Position );
    
    if( Status != LCDDRV_eSTATUS_SUCCESS )
    {
        // The location was invalid, so return with an error
        assert( FALSE );
        return( Status );
    }

    // Set the blinking cursor there
    // Node if we are going to display the cursor then turn on the display
    // as well.  No sense in displaying curor if the display is not.
    // Anyways, we don't have a choice.
    Status = lcddrv_fnWriteCmd( LCDDRV_CURSORBLINK_ON );
    
    return( Status );

}                                             

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnTurnDisplayOff   
  
PURPOSE: 
    Turn the display off. Essentially, this makes the display blank.

INPUTS: 
    None

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the command was successfully executed

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 08-Dec-2003    By: Jeff Fieldhouse
    - Created
Version: 1.01    Date: 17-Dec-2003    By: Jeff Fieldhouse
    - Changed function so it only turns the LCD off

*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnTurnDisplayOff( void )
{  
    // Turn the display off
    return( lcddrv_fnWriteCmd( LCDDRV_DISPLAY_OFF ) );
}

/*******************************************************************************

FUNCTION NAME: 
    LCDDRV_fnTurnDisplayOn   
  
PURPOSE: 
    Turn the display on. This allows any text written to the display to be
    visible.

INPUTS: 
    None

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the command was successfully executed

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 08-Dec-2003    By: Jeff Fieldhouse
    - Created
Version: 1.01    Date: 17-Dec-2003    By: Jeff Fieldhouse
    - Changed function so it only turns the LCD on

*******************************************************************************/
LCDDRV_teSTATUS LCDDRV_fnTurnDisplayOn( void )
{  
    // Turn the display on
    return( lcddrv_fnWriteCmd( LCDDRV_DISPLAY_ON ) );
}



// *****************************************************************************
// ---------------------------- Local functions --------------------------------
// *****************************************************************************

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnInit  
  
PURPOSE: 
    Initialize the character LCD by setting the appropriate registers on the 
    microcontroller, and sending specific commands to the LCD to prepare it
    for display data.

INPUTS: 
    None

OUTPUTS:
    None

NOTES:
    None
    
VERSION HISTORY:

Version: 1.01    Date: 20-May-2005    By: Henry Cutler
    - Ported
Version: 1.02    Date: 23-Jun-2005    By: Huey Duong
    - Removed unnecessar code at end    
Version: 02.01    Date: 07-Feb-2006    By: Huey Duong
    - Changed to local function
    - Removed unnecessary function calls.
    - First few writes were writing the wrong value.
    - Reduced the delay times.
    - Initialized properly to what the spec sheet states
Version: 02.02    Date: 09-Feb-2006    By: Huey Duong
    - Pins had to be initialized otherwise this won't work
*******************************************************************************/
void lcddrv_fnInit( void )
{    

    // !!!! lcddrv_fnWaitForBusyFlag() is not used here for 2 reasons.
    // 1.  It is unavailable till half way through this function.
    // 2.  It uses the RTOS' delay func before RTOS is enabled

    // Clear all pins
    GPIODRV_fnClear( tpRegisterSelect );
    GPIODRV_fnClear( tpReadWrite ); 
    GPIODRV_fnSet( tpEclock );
    GpioCtrlRegs.GPADIR.all &= ~LCDDRV_DATA_BITS_1FIELD_MASK;

    // *************************************
    // Start the LCD initialization sequence
    // *************************************     

    // Allow 40ms after LCD has risen to 2.7V
    DEVICE_mDELAY_US( 40000 );

    // Set to function to 4-bit mode.  
    // Send only the upper nibble
    // Data sheet states a min of 39us for operation
    //  !!! DO NOT DELETE !!!
/*    
    DEVICE_mACCESS_EN();                                        
    GpioCtrlRegs.GPADIR.all |= LCDDRV_DATA_BITS_1FIELD_MASK;
    DEVICE_mACCESS_DIS();

    GPIODRV_fnClear( tpRegisterSelect );
    GPIODRV_fnClear( tpReadWrite ); 
    DEVICE_mDELAY_NS( 80 );

    lcddrv_fnSendNibble( ( LCDDRV_FUNCTION_SET >> 4 ) & 0x0F );
*/
    // Send the function.  Set to 4-bit mode 2 line display.
    // Spec sheet states only the upper nibble should be sent, but 
    // sending both seems to be ok as well.  This is done to save code space.
    // If in future there is a problem, revert back to the above commented code.
    lcddrv_fnSendData( LCDDRV_REG , LCDDRV_FUNCTION_SET );
    DEVICE_mDELAY_US( 50 );

    // Send function again, this time with both nibbles.
    // Set to 4-bit mode 2 line display
    // 39us for operation
    lcddrv_fnSendData( LCDDRV_REG , LCDDRV_FUNCTION_SET );
    DEVICE_mDELAY_US( 50 );
 
    // Turn on display on and set cursor and blink off
    // 39us for operation
    lcddrv_fnSendData( LCDDRV_REG , LCDDRV_DISPLAY_ON );
    DEVICE_mDELAY_US( 50 );
    
    // Clear Display
    // 1.52ms for operation
    lcddrv_fnSendData( LCDDRV_REG , LCDDRV_CLR_DISPLAY );
    DEVICE_mDELAY_US( 2000 );

    // Set Entry Mode to increment RAM adx and shift off
    // 39us for operation
    lcddrv_fnSendData( LCDDRV_REG , LCDDRV_ENTRY_MODE );
    DEVICE_mDELAY_US( 50 );

}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnBackLightBlinkTimeout
  
PURPOSE: 
    Used as a callback function that blinks the LCD backlight on and off. This 
    functionality is useful for providing a means of physically identifying a 
    specific unit on the XanBus network. 

INPUTS: 
    None

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version: 1.01    Date: 21-May-2005    By: Henry Cutler
    - Ported
Version: 01.02    Date: 01-Nov-2005    By: Huey Duong
    - Rewritten
*******************************************************************************/
static void lcddrv_fnBackLightBlinkTimeout( void )
{   
    // Toggle the backlight
    lcddrv_fnBacklightSet( !BacklightOn );
}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnBacklightSet   
  
PURPOSE: 
    Sets the backlight on/off

INPUTS: 
    Enable - Whether to turn backlight on or off

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 11-Oct-2005    By: Huey Duong
    - Created
Version: 01.01    Date: 01-Nov-2005    By: Huey Duong
    - Made local
*******************************************************************************/
static void lcddrv_fnBacklightSet( tucBOOL Enable )
{
    // Update variable
    BacklightOn = Enable;   

    if( Enable == TRUE )
    {
        // Set brightness to on level
        PWMDRV_fnSetCompareB( LCD_BACKLIGHT_PWM_MODULE, 
                              LCDDRV_mBRIGHTNESS_GET_PWM( BrightnessLevel ) );
    }
    else
    {
        // Turn backlight off
        PWMDRV_fnSetCompareB( LCD_BACKLIGHT_PWM_MODULE, 
                              LCDDRV_BACKLIGHT_OFF );
    }
}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnBackLightTimedOut
  
PURPOSE: 
    Used as a callback function that blinks the LCD backlight on and off. This 
    functionality is useful for providing a means of physically identifying a 
    specific unit on the XanBus network. 

INPUTS: 
    None

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 01-Nov-2005    By: Huey Duong
    - Created
*******************************************************************************/
static void lcddrv_fnBackLightTimedOut( void )
{   
    // Turn off backlight if blinking is disabled
    if( tBacklightBlinkOn == FALSE ) 
    {
        lcddrv_fnBacklightSet( FALSE );
    }
}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnGetData   
  
PURPOSE: 
    Read data from the LCD

INPUTS: 
     
    Ram , 1 = Ram , 0 = Register (For Busy Flag)

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:
  
    Typically only used for reading the busy state

VERSION HISTORY:

Version: 1.00    Date: 20-May-2005    By: Henry Cutler
    - Created
Version: 02.00    Date: 07-Feb-2006    By: Huey Duong
    - Modified to use function calls to get nibbles

*******************************************************************************/
static uchar8  lcddrv_fnGetData( tucBOOL Ram ) 
{
    uchar8 Data;
    
    DEVICE_mACCESS_EN();                                        
                                        
    // Set the LCD data pins to be input    
    GpioCtrlRegs.GPADIR.all &= ~LCDDRV_DATA_BITS_1FIELD_MASK;
    
    DEVICE_mACCESS_DIS();

    // Ram returns ram address, otherwise we get Busy Flag and CG address
    if( Ram == LCDDRV_RAM )
    {   
        GPIODRV_fnSet( tpRegisterSelect );
    }
    else
    {
        GPIODRV_fnClear( tpRegisterSelect );
    }
    
    // Set R/W to read
    GPIODRV_fnSet( tpReadWrite );

    // Min setup time is 60ns
    DEVICE_mDELAY_NS( 80 );
    

    // Get high nibble followed by low nibble
    Data  = lcddrv_fnGetNibble( ) << LCDDRV_SHIFT_NIBBLE;
    Data |= lcddrv_fnGetNibble( );

    return( Data );
}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnGetNibble   
  
PURPOSE: 
    Reads one nibble for LCD

INPUTS: 
    None

OUTPUTS:
    Return - Value of 4-bit nibble in LSB

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 07-Feb-2006    By: Huey Duong
    - Created

*******************************************************************************/
static uchar8 lcddrv_fnGetNibble( void )
{
    uchar8 Nibble;
    
    // Supply rising edge to E-Clock
    GPIODRV_fnSet( tpEclock );
    
    // Max data setup time is 350ns
    DEVICE_mDELAY_NS( 400 );
        
    // Read the LCD data register. Bits 7-4
    Nibble = (uchar8)( ( GpioDataRegs.GPADAT.all & LCDDRV_DATA_BITS_1FIELD_MASK ) 
              >> XT_BITPOS24 ) & LCDDRV_LOW_NIB_MSK;       
        
    // Supply falling edge to E-Clock
    GPIODRV_fnClear( tpEclock );

    // Min E clock cycle time is 1000ns.  But we've already waited 
    // 400ns since E-Clock rising edge.
    DEVICE_mDELAY_NS( 700 );

    return( Nibble );
}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnReadData   
  
PURPOSE: 
    Read data to the previously selected RAM location. 

INPUTS: 
    Data - The value or character to display

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:
    none    

VERSION HISTORY:

Version: 1.00    Date: 11-JUL-2005    By: Wayland Yu
    - Created
Version: 01.01    Date: 24-Feb-2006    By: Huey Duong
    - Called wait for busy flag func
Version: 01.02    Date: 07-Mar-2006    By: Huey Duong
    - Check busy flag first
*******************************************************************************/
static LCDDRV_teSTATUS lcddrv_fnReadData( uchar8 *Data )
{     
    // Check busy flag.
	if( lcddrv_fnWaitForBusyFlag( ) == LCDDRV_eSTATUS_FAILURE )
	{
	    return( LCDDRV_eSTATUS_FAILURE );
	}

    // Read data from RAM
    *Data = lcddrv_fnGetData( LCDDRV_RAM );
    
    return( LCDDRV_eSTATUS_SUCCESS );
} // lcddrv_fnReadData

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnSendData   
  
PURPOSE: 
    Write data to the previously selected RAM location. The value written will
    be displayed on the LCD if the display is on.

INPUTS: 

    Ram  - TRUE = Ram , FALSE = Register (for control / setup ect)
    Data - The value or character to display

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:      

VERSION HISTORY:

Version: 1.00    Date: 20-May-2005    By: Henry Cutler
    - Created
Version: 1.01    Date: 23-Jun-2005    By: Huey Duong
    - Moved init of GPIO to ctor
*******************************************************************************/
void lcddrv_fnSendData( tucBOOL Ram, uchar8 Data )
{  
    DEVICE_mACCESS_EN();                                        
     
    // Specify the direction of the pins to be output (output = 1)
    // Direction of the data register.    
    GpioCtrlRegs.GPADIR.all |= LCDDRV_DATA_BITS_1FIELD_MASK;

    DEVICE_mACCESS_DIS();

    // Set Data for upper nibble for LCD write
    if( Ram == LCDDRV_RAM )
    {   
        GPIODRV_fnSet( tpRegisterSelect );
    }
    else
    {
        GPIODRV_fnClear( tpRegisterSelect );
    }

    // Low for write cycle
    GPIODRV_fnClear( tpReadWrite ); 

    // Wait min 60ns
    DEVICE_mDELAY_NS( 80 );

    lcddrv_fnSendNibble( ( Data >> LCDDRV_SHIFT_NIBBLE ) & LCDDRV_LOW_NIB_MSK );
    lcddrv_fnSendNibble( Data & LCDDRV_LOW_NIB_MSK );
}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnSendNibble   
  
PURPOSE: 
    Write a nibble to the LCD

INPUTS: 
    Nibble - 4-bit nibble in LSB to write

OUTPUTS:
    none

NOTES:      

VERSION HISTORY:

Version: 01.00    Date: 07-Feb-2006    By: Huey Duong
    - Created
*******************************************************************************/
static void lcddrv_fnSendNibble( uchar8 Nibble )
{
    uint32 Scratch;

    // Shift the nibble into position in prep for the GPIO data reg
    Scratch = ( uint32 )( Nibble ) << XT_BITPOS24;

    // Write data out the GPIO pins
    GpioDataRegs.GPADAT.all = 
                ( GpioDataRegs.GPADAT.all & ~LCDDRV_DATA_BITS_1FIELD_MASK ) |
                ( Scratch & LCDDRV_DATA_BITS_1FIELD_MASK );

    // Set E clock to high
    GPIODRV_fnSet( tpEclock );

    // Minimum E clock high time is 450ns
    DEVICE_mDELAY_NS( 500 );
    
    // Clear E Clock data. Falling edge latches data
    GPIODRV_fnClear( tpEclock );
    
    // E cycle time is min 1000ns but we've already done 80 + 500 + 50ns.
    DEVICE_mDELAY_NS( 600 ); //
}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnSetBacklight
  
PURPOSE: 
    RS232 control of Backlight

INPUTS: 
    none

OUTPUTS:
    CLI_teSTATUS -  CLI_eSTATUS_SUCCESS : command accepted
                 -  CLI_eSTATUS_FAILURE : command failed

NOTES:
    none

VERSION HISTORY:

Version: 1.00  Date: AUG-06-2005  By: Henry Cutler
    - Created
Version: 01.01   Date: 02-Nov-2005    By: Huey Duong
    - Updated function to work with new function calls
*******************************************************************************/
#if( LCDDRV_COMPILE_CLI == TRUE )
static CLI_teSTATUS lcddrv_fnSetBacklight( uint8 argC, char* argV[] )
{
    // This function should only execute if 2 arguments were sent to it
    if( argC != 2 )
    {
        return( CLI_eSTATUS_FAILURE );
    }

    if( CLI_fnIsParmOFF( argV[1] ) == TRUE )
    {
         LCDDRV_fnBacklightTimeSet( LCDDRV_eBACKLIGHT_OFF );
    }
    else if( CLI_fnIsParmON( argV[1] ) == TRUE )
    {
         LCDDRV_fnBacklightTimeSet( LCDDRV_eBACKLIGHT_ON );
    }
    else
    {
        return( CLI_eSTATUS_FAILURE );    
    }
       
    return( CLI_eSTATUS_SUCCESS );

} // lcddrv_fnSetBacklight
#endif

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnSetBacklightTimeout
  
PURPOSE: 
    Sets the timeout value for the backlight timer

INPUTS: 
    Setting - How long to set the backlight timer

OUTPUTS:
    none

NOTES:
    none

VERSION HISTORY:

Version: 01.00  Date: 06-Mar-2006    By: Huey Duong
    - Created
*******************************************************************************/
static void lcddrv_fnSetBacklightTimeout( LCDDRV_teBACKLIGHT Setting )
{
    switch( Setting )
    {
        case LCDDRV_eBACKLIGHT_5S:            
            TIMER_fnSetTimeout( tpBacklightTimedTimer, 5000L );
            break;

        case LCDDRV_eBACKLIGHT_10S:
            TIMER_fnSetTimeout( tpBacklightTimedTimer, 10000L );
            break;

        case LCDDRV_eBACKLIGHT_30S:            
            TIMER_fnSetTimeout( tpBacklightTimedTimer, 30000L );
            break;

        case LCDDRV_eBACKLIGHT_1M:            
            TIMER_fnSetTimeout( tpBacklightTimedTimer, 60000L );
            break;
        
        case LCDDRV_eBACKLIGHT_5M:            
            TIMER_fnSetTimeout( tpBacklightTimedTimer, 300000L );
            break;
    }
}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnSetDdramAddress
  
PURPOSE: 
    Set the RAM location that you want to write data to.

INPUTS: 
    LineNum  - The display line of interest
    Position - The postion in the line to write the data to

OUTPUTS:
    LCDDRV_teSTATUS - function success or failure

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 08-Dec-2003    By: Jeff Fieldhouse
    - Created

*******************************************************************************/
static LCDDRV_teSTATUS lcddrv_fnSetDdramAddress( LCDDRV_teLINE     LineNum,
                                                 LCDDRV_tePOSITION Position )
{
    uchar8 Address; // Stores the actual RAM address

    // Check that the character position is valid
    if( ( Position <  LCDDRV_ePOSITION_0 )    
     || ( Position >= LCDDRV_ePOSITION_MAX ) )
    {
        // Invalid character position, so return an error
        assert( FALSE );
        return( LCDDRV_eSTATUS_FAILURE_INVLD_POSITION );
    }

    // Check the line number requested, and ensure its a valid position
    if( LineNum == LCDDRV_eLINE_1 ) 
    {
        // Build the RAM address for an item stored in the first line
        Address = ( Position | LCDDRV_DDRAM_MSK | LCDDRV_1ST_LINE_MSK );

        // Send the command to set the RAM address
        ( void )lcddrv_fnWriteCmd( Address ); 

        // Return an all good signal
        return( LCDDRV_eSTATUS_SUCCESS );
    }
    else if( LineNum == LCDDRV_eLINE_2 ) 
    {
        // Build the RAM address for an item stored in the second line
        Address = ( Position | LCDDRV_DDRAM_MSK | LCDDRV_2ND_LINE_MSK );

        // Send the command to set the RAM address
        ( void )lcddrv_fnWriteCmd( Address ); 

        // Return an all good signal
        return( LCDDRV_eSTATUS_SUCCESS );
    }

    // Invalid line number, so return an error
    assert( FALSE );
    return( LCDDRV_eSTATUS_FAILURE_INVLD_LINE_NUM );
}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnWaitForBusyFlag  
  
PURPOSE: 
    Waits for the busy flag to go down

INPUTS: 
    none;

OUTPUTS:
    LCDDRV_teSTATUS - Whether we timeout out too long

NOTES:
        
VERSION HISTORY:

Version: 01.00    Date: 29-Jun-2005    By: Huey Duong
    - Created
Version: 01.01    Date: 27-Feb-2006    By: Huey Duong
    - Fixed code to work properly
*******************************************************************************/
static LCDDRV_teSTATUS lcddrv_fnWaitForBusyFlag( void )
{
    uint16 Timeout = 0;

    // !!! Note, emperical data
	// Read/Write ops ( no OSTimeDly ) = 4 loops
	// Clear Screen ops ( with OSTimeDly ) = 2 OSTimeDly
	// The reason for 2 Delays is because the first one can be anywhere, even
	// really close to the timeout, hence it may not be enough to clear the
	// screen.

    // Check the busy flag and add a loop timeout
	// Allow it to loop one extra time as a max value so we can send an 
	// error afterwards
    while ( ( lcddrv_fnGetData( LCDDRV_REG ) > 0x7F ) 
         && ( Timeout <= LCDDRV_BUSY_FLAG_TIMEOUT_CNTS ) )
    {
        // Don't delay for most fast operations, just loop through
        if( Timeout >= LCDDRV_BUSY_FLAG_TIMEOUT_FAST_OP_LP )
        {
            // Come back in 1 tick
            OSTimeDly( 1 );
        }

        // Initially Timout is the time to execute a loop. 
        // When Timeout reaches the SLOW_OP TIMEOUT each count is worth 1 tick.
        Timeout++;  
    } 

    // Check for timeout
    if( Timeout > LCDDRV_BUSY_FLAG_TIMEOUT_CNTS )
    {
        // Something is wrong with busy flag.
        assert( FALSE );
        return( LCDDRV_eSTATUS_SUCCESS );
    }
    
    return( LCDDRV_eSTATUS_SUCCESS );

}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnWriteCmd  
  
PURPOSE: 
    Sends a command to the LCD

INPUTS: 
    Command - Which command to send

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:
        
VERSION HISTORY:

Version: 1.01    Date: 20-May-2005    By: Henry Cutler
    - Ported
Version: 1.02    Date: 29-Jun-2005    By: Huey Duong
    - Fixed timeout checking algorithm.  Did not make sense
Version: 01.02    Date: 07-Mar-2006    By: Huey Duong
    - Check busy flag first
*******************************************************************************/
static LCDDRV_teSTATUS lcddrv_fnWriteCmd( uchar8 Command )
{
    // Check busy flag.
	if( lcddrv_fnWaitForBusyFlag( ) == LCDDRV_eSTATUS_FAILURE )
	{
	    return( LCDDRV_eSTATUS_FAILURE );
	}

    // Send data to RAM
    lcddrv_fnSendData( LCDDRV_REG , Command );

    return( LCDDRV_eSTATUS_SUCCESS );    
}

/*******************************************************************************

FUNCTION NAME: 
    lcddrv_fnWriteData   
  
PURPOSE: 
    Write data to the previously selected RAM location. The value written will
    be displayed on the LCD if the display is on.

INPUTS: 
    Data - The value or character to display

OUTPUTS:
    LCDDRV_teSTATUS - Indicates if the function executed properly

NOTES:
  
    

VERSION HISTORY:

Version: 1.01    Date: 20-May-2005    By: Henry Cutler
    - Ported
Version: 1.02    Date: 29-Jun-2005    By: Huey Duong
    - Fixed timeout checking algorithm.  Did not make sense
Version: 01.02    Date: 07-Mar-2006    By: Huey Duong
    - Check busy flag first
*******************************************************************************/
static LCDDRV_teSTATUS lcddrv_fnWriteData( uchar8 Data )
{     
    // Check busy flag.
	if( lcddrv_fnWaitForBusyFlag( ) == LCDDRV_eSTATUS_FAILURE )
	{
	    return( LCDDRV_eSTATUS_FAILURE );
	}

    // Send data to RAM
    lcddrv_fnSendData( LCDDRV_RAM, Data );
    
    return( LCDDRV_eSTATUS_SUCCESS );
}

// Undefine the filename in case someone #includes this .c file
#undef XASSERT_FILENAME
