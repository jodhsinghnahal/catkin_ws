/*==============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  
    PwmDrv.h

PURPOSE:
    PWM driver interface for Texas Instruments 280x processor.

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
   
NOTES:
    How to use this driver:
    1) Ensure that the PWMDRV_fnCtor() function is called once at the beginning
       of the application. This will configure all the PWM modules according to
       the project requirements.
    2) Call the PWMDRV_fnGetMaxCompare() or PWMDRV_fnGetMaxCompareHiRes() 
       function to obtain the max compare value for a particular module. This
       value should be used to limit the values sent for PWM generation. The
       value returned by these functions should never change, so it only needs
       to be called once per module. Also note that the max compare value is 
       the same for CMPA and CMPB, unless the module is being used in Hi Res
       mode. In that case, the max CMPA value will be 32-bit value that is 
       higher than the 16-bit max CMPB value.
    3) When you are ready for the PWM to run, call PWMDRV_fnModuleOutputEn()
       or PWMDRV_fnPinOutputEn() to configure the pin(s) for PWM control. Note
       that if the pin is not intended for PWM control, these functions will 
       have no effect as there is a check for this in the .c file.
    4) Call one of the PWMDRV_fnSetCompare... functions to change the compare
       values, and hence change the output duty cycle. The compare values can
       also be used for reference PWM's that are useful for causing PWM events
       like ADC SOCA (Start Of Conversion on ADC Sequence A).

CHANGE HISTORY:
$Log: Pwmdrv.h $

    ***********************************************
    Revision: SurveyorPfrmA_HollyZ/1
    User: HollyZ     Date: 11/01/06  Time: 07:09PM
    Added prototypes for changing the deadbands

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/5
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA2_YingranD/1
    User: YingranD     Date: 01/18/06  Time: 07:39PM
    new digctrl package design and related file update

    ***********************************************
    Revision: SurveyorPfrmA_HenryC/1
    User: HenryC     Date: 11/01/05  Time: 05:17PM
    Moved Pwm3 SOC defines to .h file

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/4
    User: JeffF     Date: 05/20/05  Time: 08:28PM
    Added Get functions for retrieving the compare A and B values

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/3
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
    User: JeffF     Date: 05/05/05  Time: 04:20PM
    Changed one of the input parameter types for PWMDRV_fnConfigureEvent to
    use a predefined enum for ease of use concerns.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/5
    User: HueyD     Date: 04/27/05  Time: 05:13PM
    commented

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/4
    User: HueyD     Date: 04/26/05  Time: 02:52AM
    Attempt to reduce code size

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/3
    User: HueyD     Date: 04/25/05  Time: 05:54PM
    Added high res

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/2
    User: HueyD     Date: 04/22/05  Time: 11:14PM
    Lots of changes
    
==============================================================================*/

#ifndef PWMDRV_INCL
#define PWMDRV_INCL

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    //_16BIT_ARCHITECTURE defined in Compiler Options
#include "Device.h"         // Register bits

/*==============================================================================
                              Defines
==============================================================================*/

/*==============================================================================
                            Type Definitions
==============================================================================*/

/*=============================================================================
                                Enums
==============================================================================*/

// *****************************************************************************
// *************************** !!! IMPORTANT !!! *******************************
// *** DO NOT MODIFY ENUMS AS THEIR VALUES ARE USED TO MANIPULATE BITS *********
// ***************** CODE WILL BREAK IF MODIFIED INCORRECTLY *******************
// *****************************************************************************

// Define values used to refer to specific PWM modules. Although there are 12 
// PWM output pins, there are only 6 independent PWM modules.
typedef enum PWMDRV_eMODULE
{
    PWMDRV_eMODULE_1 = 0,   
    PWMDRV_eMODULE_2 = 1,
    PWMDRV_eMODULE_3 = 2,
    PWMDRV_eMODULE_4 = 3,
    PWMDRV_eMODULE_5 = 4, 
    PWMDRV_eMODULE_6 = 5, 
    
    PWMDRV_eMODULE_TOTAL  // Total number of modules
} PWMDRV_teMODULE;

// Define all the available PWMs
typedef enum PWMDRV_ePIN
{
    PWMDRV_ePIN_1A = 0,
    PWMDRV_ePIN_1B = 1,
    PWMDRV_ePIN_2A = 2,
    PWMDRV_ePIN_2B = 3,
    PWMDRV_ePIN_3A = 4,
    PWMDRV_ePIN_3B = 5,
    PWMDRV_ePIN_4A = 6,
    PWMDRV_ePIN_4B = 7,
    PWMDRV_ePIN_5A = 8,
    PWMDRV_ePIN_5B = 9,
    PWMDRV_ePIN_6A = 10,
    PWMDRV_ePIN_6B = 11,
    
    PWMDRV_ePIN_TOTAL    // Total number of Pins
} PWMDRV_tePIN;
  
// Define all the Trip Zones      
typedef enum PWMDRV_eTZ
{
    PWMDRV_eTZ_1 = 0,
    PWMDRV_eTZ_2,
    PWMDRV_eTZ_3,
    PWMDRV_eTZ_4,
    PWMDRV_eTZ_5,
    PWMDRV_eTZ_6,
    
    PWMDRV_eTZ_TOTAL    // Total number of Trip Zones
} PWMDRV_teTZ;

 // Define Event Enables
typedef enum PWMDRV_eETEN
{
    PWMDRV_eETEN_INT = 0,  // Enable interrupt
    PWMDRV_eETEN_SOCA,     // Enable SOCA pulse
    PWMDRV_eETEN_SOCB      // Enable SOCB pulse
} PWMDRV_teETEN;   

/*==============================================================================
                              Structures
==============================================================================*/

// Define return status
typedef enum PWMDRV_eSTATUS
{
    PWMDRV_eSTATUS_SUCCESS =  0,  // Success

    PWMDRV_eSTATUS_WARNING =  1,  // Generic warning

    PWMDRV_eSTATUS_FAILURE = -1  // Generic failure
}PWMDRV_teSTATUS;


/*==============================================================================
                          External/Public Constants
==============================================================================*/

/*==============================================================================
                          External/Public Variables
==============================================================================*/

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
// Driver constructor and destructor.
PWMDRV_teSTATUS PWMDRV_fnCtor( void ); 
PWMDRV_teSTATUS PWMDRV_fnDtor( void );

// PWM interrupt, SOCA and SOCB enables and disables.
void PWMDRV_fnEventDis( PWMDRV_teMODULE tePwmModule,
                        PWMDRV_teETEN   teEventType );                       
void PWMDRV_fnEventEn ( PWMDRV_teMODULE tePwmModule,
                        PWMDRV_teETEN   teEventType );                         

// Get functions for actual compare values (CMPA and CMPB)
uint16 PWMDRV_fnGetCompareA( PWMDRV_teMODULE tePwmModule );
uint16 PWMDRV_fnGetCompareB( PWMDRV_teMODULE tePwmModule );
uint32 PWMDRV_fnGetCompareAHiRes( PWMDRV_teMODULE tePwmModule );

// Get functions for max compare values (CMPA and CMPB).
// NOTE: that only CMPA can be assigned a HI Res value.
uint16 PWMDRV_fnGetMaxCompare( PWMDRV_teMODULE tePwmModule );
uint32 PWMDRV_fnGetMaxCompareHiRes( PWMDRV_teMODULE tePwmModule );

// PWM module output enables and disables.
// Disable sets pins to either GPIO inputs (high impedance), or to GPIO ouputs 
// with logic low or logic high outputs (set in the .c file).
// Enable sets the pins for PWM control (if that's how they normally operate).
void PWMDRV_fnModuleOutputDis( PWMDRV_teMODULE tePwmModule );
void PWMDRV_fnModuleOutputEn ( PWMDRV_teMODULE tePwmModule );

// PWM pin output enables and disables.
// Disable sets the pin to either a GPIO input (high impedance), or to a GPIO  
// ouput with a logic low or logic high output (set in the .c file).
// Enable sets the pin for PWM control (if that's how it normally operates).
void PWMDRV_fnPinOutputDis( PWMDRV_tePIN tePwmPin );
void PWMDRV_fnPinOutputEn ( PWMDRV_tePIN tePwmPin );

// Sets the CMPA or CMPB or both values for a particular PWM module.
void PWMDRV_fnSetCompareA( PWMDRV_teMODULE tePwmModule,
                           uint16          CompVal );
void PWMDRV_fnSetCompareB( PWMDRV_teMODULE tePwmModule,
                           uint16          CompVal );                           
void PWMDRV_fnSetCompareAandB( PWMDRV_teMODULE tePwmModule,
                               uint16          CompAVal,
                               uint16          CompBVal );

// Sets the Hi Res CMPA value or both that and the CMPB value.                               
void PWMDRV_fnSetCompareAHiRes( PWMDRV_teMODULE tePwmModule,
                                uint32          CompVal );
void PWMDRV_fnSetCompareAandBHiRes( PWMDRV_teMODULE tePwmModule,
                                    uint32          CompAVal,
                                    uint16          CompBVal );

// Set/get the deadband for Hi FET and Low FET rising edge                                    
void PWMDRV_fnGetDeadbands( uint16 *pHiRDB, uint16 *pLoRDB );
void PWMDRV_fnSetDeadbands( uint16 HiRDB, uint16 LoRDB );                                   

#endif
