/*==============================================================================
Copyright © 2004 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  
    LcdDrv.h

PURPOSE:
    Provides an interface for displaying information on a multiline 16 character
    wide LCD. Also provides contrast, and backlight control.

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
   
NOTES:
    To use this driver, first call LCDDRV_fnInit(). RTOS needs to be operation 
    before the use of the module as large delays utilize the non-blocking time 
    delays

CHANGE HISTORY:
$Log: LcdDrv.h $

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/2
    User: HueyD     Date: 05/24/06  Time: 11:50PM
    fixed backlight always on setting not working on startup.

    Moved blink period to .h for xb

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/1
    User: HueyD     Date: 02/21/06  Time: 06:40PM
    Backlight doesn't turn on during init

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/3
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/13
    User: HueyD     Date: 02/10/06  Time: 12:45AM
    Changed disp cont and brightness to suite revb boards

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/12
    User: HueyD     Date: 02/09/06  Time: 11:08PM
    Changed how backlight works

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/11
    User: HueyD     Date: 02/08/06  Time: 02:47AM
    Removed init func

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/2
    User: JeffF     Date: 01/17/06  Time: 09:48PM
    Implemented a function to restore the factory default settings for this
    package.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/10
    User: HueyD     Date: 11/30/05  Time: 11:58PM
    Removed unused functions

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/9
    User: HueyD     Date: 11/24/05  Time: 06:04PM
    Fixed error in PWM/USER value conversion.

    Tightened contrast window so that text doesn't disappear

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/8
    User: HueyD     Date: 11/02/05  Time: 09:56PM
    merged

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/7
    User: HueyD     Date: 11/02/05  Time: 07:41PM
    Reorganized file.

    Updated comments.

    Added display backlight blinking/stay on/timed on feature.  Order of precedence
    shown.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/6
    User: HueyD     Date: 10/20/05  Time: 10:43PM
    Added a lot more screens

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/5
    User: HueyD     Date: 10/06/05  Time: 06:40PM
    Working copy of first pass screens

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/4
    User: HueyD     Date: 07/25/05  Time: 11:15PM
    Got text editing to work

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/3
    User: HueyD     Date: 07/13/05  Time: 09:49PM
    Removed line invalid enum.  Set line 1 to 0

    ***********************************************
    Revision: SurveyorPfrmA_WaylandY/2
    User: WaylandY     Date: 07/12/05  Time: 12:30AM
    - Updated the function list in the header

    ***********************************************
    Revision: SurveyorPfrmA_WaylandY/1
    User: WaylandY     Date: 07/11/05  Time: 11:44PM
    - added LCDDRV_fnReadString

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/1
    User: HueyD     Date: 06/08/05  Time: 10:45PM
    Updated driver to do blinking cursor

==============================================================================*/

#ifndef LCDDRV_H_INCL
#define LCDDRV_H_INCL            

/*==============================================================================
                              Includes
==============================================================================*/ 
#include "LIB_stddefs.h"     // Standard libaray definitions
#include "LIB_stdtype.h"     // Standard type definitions
                                 
/*==============================================================================
                              Defines
==============================================================================*/

//**** CONTRAST DEFINITIONS ****
// Define limits for contrast for both the user and pwm levels
#define LCDDRV_CONTRAST_USER_DEF   ( 7 )
#define LCDDRV_CONTRAST_USER_LIGHT ( 1 )
#define LCDDRV_CONTRAST_USER_DARK  ( 9 )
#define LCDDRV_CONTRAST_PWM_LIGHT  ( 350 )
#define LCDDRV_CONTRAST_PWM_DARK   ( 100 )

// Define the actual maximum values
#define LCDDRV_CONTRAST_USER_MIN ( LCDDRV_CONTRAST_USER_LIGHT )
#define LCDDRV_CONTRAST_USER_MAX ( LCDDRV_CONTRAST_USER_DARK )


//**** BRIGHTNESS DEFINITIONS ****
// Define limits for brightness for both the user and pwm levels
#define LCDDRV_BRIGHTNESS_USER_DEF    ( 7 )
#define LCDDRV_BRIGHTNESS_USER_DIM    ( 1 )
#define LCDDRV_BRIGHTNESS_USER_BRIGHT ( 9 )
#define LCDDRV_BRIGHTNESS_PWM_DIM     ( 40 )
#define LCDDRV_BRIGHTNESS_PWM_BRIGHT  ( 85 )

// Define the actual maximum values
#define LCDDRV_BRIGHTNESS_USER_MIN ( LCDDRV_BRIGHTNESS_USER_DIM )
#define LCDDRV_BRIGHTNESS_USER_MAX ( LCDDRV_BRIGHTNESS_USER_BRIGHT )

//#define LCDDRV_BACKLIGHT_MIN_S ( LCDDRV_BACKLIGHT_OFF )
#define LCDDRV_BACKLIGHT_DEF_S ( 30 )

// Backlight blink period
#define LCDDRV_BACKLIGHT_BLINK_HALF_PERIOD ( 500 ) // 500ms

/*==============================================================================
                            Type Definitions
==============================================================================*/
 
/*==============================================================================
                                Enums
==============================================================================*/

// Backlight on time options for user to set
typedef enum LCDDRV_eBACKLIGHT
{
    LCDDRV_eBACKLIGHT_OFF = 0, // Never on
    LCDDRV_eBACKLIGHT_5S,
    LCDDRV_eBACKLIGHT_10S,
    LCDDRV_eBACKLIGHT_30S,
    LCDDRV_eBACKLIGHT_1M,
	LCDDRV_eBACKLIGHT_5M,
    LCDDRV_eBACKLIGHT_ON,

    LCDDRV_eBACKLIGHT_DEF = LCDDRV_eBACKLIGHT_30S
}LCDDRV_teBACKLIGHT;
    

// Used for setting the line of interest on the LCD
// If your LCD doesn't have 4 lines, LcdDrv.c will handle that
typedef enum LCDDRV_eLINE
{
    LCDDRV_eLINE_1 = 0,
    LCDDRV_eLINE_2,
    LCDDRV_eLINE_3,
    LCDDRV_eLINE_4

} LCDDRV_teLINE;

// Used for setting the character location that the LCD will start writing
// the display string to 
typedef enum LCDDRV_ePOSITION
{
    LCDDRV_ePOSITION_0 = 0,
    LCDDRV_ePOSITION_1,
    LCDDRV_ePOSITION_2,
    LCDDRV_ePOSITION_3,
    LCDDRV_ePOSITION_4,
    LCDDRV_ePOSITION_5,
    LCDDRV_ePOSITION_6,
    LCDDRV_ePOSITION_7,
    LCDDRV_ePOSITION_8,
    LCDDRV_ePOSITION_9,
    LCDDRV_ePOSITION_10,
    LCDDRV_ePOSITION_11,
    LCDDRV_ePOSITION_12,
    LCDDRV_ePOSITION_13,
    LCDDRV_ePOSITION_14,
    LCDDRV_ePOSITION_15,
    LCDDRV_ePOSITION_MAX

} LCDDRV_tePOSITION;

// Used for reporting the status of an LCD display request
typedef enum LCDDRV_eSTATUS
{
    // Success ( 0 )
    LCDDRV_eSTATUS_SUCCESS                   = 0,  // Operation was successful
    
    // Failure ( negative )
    LCDDRV_eSTATUS_FAILURE                   = -1, // Operation failed
    LCDDRV_eSTATUS_FAILURE_INVLD_POSITION    = -2, // Char position was invalid
    LCDDRV_eSTATUS_FAILURE_INVLD_LINE_NUM    = -3, // Line number was invalid
    LCDDRV_eSTATUS_FAILURE_STR_TOO_LONG      = -4, // Display str was too long
    LCDDRV_eSTATUS_FAILURE_INVLD_PWM_SETUP   = -5, // PWM coding error

    // Warnings ( positive )
    LCDDRV_eSTATUS_WARNING                   = 1,  // Problem was encountered
    LCDDRV_eSTATUS_WARNING_ZERO_STR_LEN      = 2,  // Trying to disp 0 len str
    LCDDRV_eSTATUS_WARNING_LCD_BUSY_FLAG     = 3,  // LCD busy flag timed out
    LCDDRV_eSTATUS_WARNING_CONTRAST_FIXED    = 4,  // Contrast isn't variable
    LCDDRV_eSTATUS_WARNING_MAX_CONTRAST      = 5,  // The contrast is max'd out
    LCDDRV_eSTATUS_WARNING_MIN_CONTRAST      = 6,  // The contrast is at the min
    LCDDRV_eSTATUS_WARNING_BL_BLINKING       = 7,  // The backlight is blinking
    LCDDRV_eSTATUS_WARNING_INV_CONTRAST      = 8,  // Invalid value for contrast
    LCDDRV_eSTATUS_WARNING_INV_BACKLIGHT     = 9   // Invalid value for backlight

} LCDDRV_teSTATUS;  

/*=============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                          External/Public Constants
==============================================================================*/
  
/*==============================================================================
                          External/Public Variables
==============================================================================*/
   
/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

// Creation of package
LCDDRV_teSTATUS LCDDRV_fnCtor( void );

LCDDRV_teSTATUS LCDDRV_fnBackLightBlinkOn( tucBOOL Enable );
LCDDRV_teSTATUS LCDDRV_fnBacklightTimedStart( void );
LCDDRV_teSTATUS LCDDRV_fnBacklightTimeSet( LCDDRV_teBACKLIGHT Setting );
LCDDRV_teBACKLIGHT LCDDRV_fnBacklightTimeGet( void );
uint16 LCDDRV_fnBackLightBlinkPeriodGetCs( void );

uint16 LCDDRV_fnBrightnessGet( void );
LCDDRV_teSTATUS LCDDRV_fnBrightnessSet( uint16  Level,
                                        tucBOOL Save );

// Clear all the text on the display
LCDDRV_teSTATUS LCDDRV_fnClearDisplay( void );

uint16 LCDDRV_fnContrastGet( void );

LCDDRV_teSTATUS LCDDRV_fnContrastSet( uint16  Level,
                                      tucBOOL Save );

//Write an entire array to the display
LCDDRV_teSTATUS LCDDRV_fnDisplayArray( 
                const uchar8*     DispArray, // Character string to display
                uchar8            Count,      // Number of characters to display
                LCDDRV_teLINE     LineNum,    // Display line for string
                LCDDRV_tePOSITION Position ); // Location for first char in str

// Display a character
LCDDRV_teSTATUS LCDDRV_fnDisplayChar( schar8            Character,
                                      LCDDRV_teLINE     LineNum,
                                      LCDDRV_tePOSITION Position );

// Write an entire string to the display
LCDDRV_teSTATUS LCDDRV_fnDisplayString( 
                const uchar8*     DispString, // Character string to display
                LCDDRV_teLINE     LineNum,    // Display line for string
                LCDDRV_tePOSITION Position ); // Location for first char in str

// Restore the configurable settings back to factory default
void LCDDRV_fnRestoreFactoryDflt( void );

// Read an entire string from the display
LCDDRV_teSTATUS LCDDRV_fnReadString( 
                uchar8*           ReadString, // Character string to read
                LCDDRV_teLINE     LineNum,    // Display line for string
                LCDDRV_tePOSITION Position,   // Location for first char in str
                uint16            BuffSize ); // Number of characters to read

// Displays a blinking cursor
LCDDRV_teSTATUS LCDDRV_fnTurnCursorBlinkOn( 
                LCDDRV_teLINE     LineNum,    // Line number
                LCDDRV_tePOSITION Position ); // Location for cursor

// Turns off blinking cursor
LCDDRV_teSTATUS LCDDRV_fnTurnCursorBlinkOff( void );

// Turn the LCD on
LCDDRV_teSTATUS LCDDRV_fnTurnDisplayOn( void );

// Turn the LCD off
LCDDRV_teSTATUS LCDDRV_fnTurnDisplayOff( void );

#endif // LCDDRV_H_INCL
