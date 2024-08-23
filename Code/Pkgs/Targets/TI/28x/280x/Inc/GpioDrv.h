/*==============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  
    GpioDrv.h

PURPOSE:
    Provide a class to handle general purpose IO pin.  This driver allows 
    a layer of abstraction to access low level IO pins. 

FUNCTION(S):
    GPIODRV_fnSetupMemory   - Initializes memory management system for package
    GPIODRV_fnCtor          - Constructor for a GPIO pin object
    GPIODRV_fnDtor          - Destructor for a GPIO pin
    GPIODRV_fnClear         - Clears the pin state
    GPIODRV_fnSet           - Sets the pin state
    GPIODRV_fnToggle        - Toggles the pin state
    GPIODRV_fnIsSet         - Queries whether the pin is set
    GPIODRV_fnUpdateDir     - Updates the direction of the pin
    GPIODRV_fnUpdateOptions - Updates the extended options of the pin
   
NOTES:
The constructors and destructors in this module create and destroy memory
dynamically.  The uCosII RTOS provides a memory management library which can
create partitions.  This feature allows for better memory management because
the allocated sections can be assigned to any sector independant of other 
dynamically allocated variables.  The memory allocation functions from 
<stdlib.h> library allocates all variables into a single heap.
A developer may want to map certain dynamically allocated vars to faster RAM.

******************************* IMPORTANT!!!!**********************************
*     GPIODRV_fnSetupMemory( ) must be called before any other function       *
*     is used.  It sets up the memory heap for the package.  Program will     *
*     crash if it is not called.                                              *
*******************************************************************************      

CHANGE HISTORY:
$Log: GpioDrv.h $

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/3
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/1
    User: HueyD     Date: 02/10/06  Time: 12:46AM
    added comments

    ***********************************************
    Revision: SurveyorPfrmASC60_JeffF/2
    User: JeffF     Date: 03/21/05  Time: 07:26PM
    Changed some of the enums to better match the register defines for the
    280x DSP.
    
==============================================================================*/

#ifndef GPIODRV_H_INCL
#define GPIODRV_H_INCL

/*==============================================================================
                              Includes
==============================================================================*/

#include "LIB_stddefs.h"  // Standard definitions
#include "LIB_stdtype.h"  // Standard types

/*==============================================================================
                              Defines
==============================================================================*/


/*==============================================================================
                            Type Definitions
==============================================================================*/

// Define gpio object handle
typedef void* GPIODRV_tpHANDLE;

/*==============================================================================
                                Enums
==============================================================================*/
// Define package error status
typedef enum GPIODRV_eSTATUS
{
    // Success ( 0 )
    GPIODRV_eSTATUS_SUCCESS                         = 0,
    
    // Failure ( negative )
    GPIODRV_eSTATUS_FAILURE                         = -1,
    GPIODRV_eSTATUS_FAILURE_PARM_INVALID            = -2,
    GPIODRV_eSTATUS_FAILURE_OUT_OF_MEM              = -3,
    GPIODRV_eSTATUS_FAILURE_PORT_NOT_SUPPORTED      = -4,

    // Warnings ( positive )
    GPIODRV_eSTATUS_WARNING                         = 1,
    GPIODRV_eSTATUS_WARNING_OPTION_NOT_SUPPORTED    = 2

} GPIODRV_teSTATUS;    
    
// Define input/output options
typedef enum GPIODRV_eDIR
{
    GPIODRV_eDIR_INPUT = 0,  // Pin is an input
    GPIODRV_eDIR_OUTPUT      // Pin is an output

} GPIODRV_teDIR;

// Define init value of output pin
typedef enum GPIODRV_eINIT
{
    GPIODRV_eINIT_CLEAR = 0,
    GPIODRV_eINIT_SET   = 1,
    GPIODRV_eINIT_DONT_CARE = 2
} GPIODRV_teINIT;

// Define pin qualification
typedef enum GPIODRV_eQUAL_SEL
{
    GPIODRV_eQUAL_SEL_SYSCLK = 0, // Syncronized with SYSCLKOUT
    GPIODRV_eQUAL_SEL_3_SAMPLES,  // Qualified with 3 samples
    GPIODRV_eQUAL_SEL_6_SAMPLES,  // Qualified with 6 samples
    GPIODRV_eQUAL_SEL_ASYNC       // No sync or qual (not valid for GPIO)

} GPIODRV_teQUAL_SEL;

// Define pullup option
typedef enum GPIODRV_ePULLUP
{
    GPIODRV_ePULLUP_ENAB = 0,     // Pullup resistor is enabled
    GPIODRV_ePULLUP_DISAB         // Pullup resistor is disabled (default state)

} GPIODRV_tePULLUP;

// Define available ports
typedef enum GPIODRV_eGPIONUM
{
    GPIODRV_eGPIO0 = 0,
    GPIODRV_eGPIO1,
    GPIODRV_eGPIO2,
    GPIODRV_eGPIO3,
    GPIODRV_eGPIO4,
    GPIODRV_eGPIO5,
    GPIODRV_eGPIO6,
    GPIODRV_eGPIO7,
    GPIODRV_eGPIO8,
    GPIODRV_eGPIO9,
    GPIODRV_eGPIO10,
    GPIODRV_eGPIO11,
    GPIODRV_eGPIO12,
    GPIODRV_eGPIO13,
    GPIODRV_eGPIO14,
    GPIODRV_eGPIO15,
    GPIODRV_eGPIO16,
    GPIODRV_eGPIO17,
    GPIODRV_eGPIO18,
    GPIODRV_eGPIO19,
    GPIODRV_eGPIO20,
    GPIODRV_eGPIO21,
    GPIODRV_eGPIO22,
    GPIODRV_eGPIO23,
    GPIODRV_eGPIO24,
    GPIODRV_eGPIO25,
    GPIODRV_eGPIO26,
    GPIODRV_eGPIO27,
    GPIODRV_eGPIO28,
    GPIODRV_eGPIO29,
    GPIODRV_eGPIO30,
    GPIODRV_eGPIO31,
    GPIODRV_eGPIO32,
    GPIODRV_eGPIO33,
    GPIODRV_eGPIO34,
    GPIODRV_eGPIO35,
    GPIODRV_eGPIO36,
    GPIODRV_eGPIO37,
    GPIODRV_eGPIO38,
    GPIODRV_eGPIO39,
    GPIODRV_eGPIO40,
    GPIODRV_eGPIO41,
    GPIODRV_eGPIO42,
    GPIODRV_eGPIO43,
    GPIODRV_eGPIO44,
    GPIODRV_eGPIO45,
    GPIODRV_eGPIO46,
    GPIODRV_eGPIO47,
    GPIODRV_eGPIO48,
    GPIODRV_eGPIO49,
    GPIODRV_eGPIO50,
    GPIODRV_eGPIO51,
    GPIODRV_eGPIO52,
    GPIODRV_eGPIO53,
    GPIODRV_eGPIO54,
    GPIODRV_eGPIO55,
    GPIODRV_eGPIO56,
    GPIODRV_eGPIO57,
    GPIODRV_eGPIO58,
    GPIODRV_eGPIO59,
    GPIODRV_eGPIO60,
    GPIODRV_eGPIO61,
    GPIODRV_eGPIO62,
    GPIODRV_eGPIO63, 
    GPIODRV_eMAX_PINS

} GPIODRV_teGPIONUM;

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

// IMPORTANT!!!  This function must be called before any other function to
//               setup the memory pool.
GPIODRV_teSTATUS GPIODRV_fnSetupMemory( void );

// Params: ptpHandle - Pointer to handle to the created pin object
//         tePin     - GPIO pin number
//         teDir     - Direction of pin (input or output)
//         teEnable  - Enable or disable the pullup resistor  
//         teQual    - Select the number of samples for input qualification
GPIODRV_teSTATUS GPIODRV_fnCtor( GPIODRV_tpHANDLE*  ptpHandle,
                                 GPIODRV_teGPIONUM  tePin,
                                 GPIODRV_teDIR      teDir,
                                 GPIODRV_tePULLUP   teEnable,                                 
                                 GPIODRV_teQUAL_SEL teQual );
                                 
// Params: ptpHandle - Pointer to handle of object to destroy
void GPIODRV_fnDtor( GPIODRV_tpHANDLE* ptpHandle );  

// Params: tpHandle - Handle of output pin to clear
void GPIODRV_fnClear( GPIODRV_tpHANDLE tpHandle );

// Params: tpHandle - Handle of output pin to set
void GPIODRV_fnSet( GPIODRV_tpHANDLE tpHandle );  // Set the pin

// Params: tpHandle - Handle of output pin to toggle
void GPIODRV_fnToggle( GPIODRV_tpHANDLE tpHandle );

// Params: tpHandle - Handle of gpio pin to query
tucBOOL GPIODRV_fnIsSet( GPIODRV_tpHANDLE tpHandle );

// Params: tpHandle - Handler of gpio pin to update options
//         teNewDir - New direction of port
GPIODRV_teSTATUS GPIODRV_fnUpdateDir( GPIODRV_tpHANDLE tpHandle,
                                      GPIODRV_teDIR    teNewDir );

// Params: tpHandle - Handler of gpio pin to update options
//         teNewDir - New direction of port
//         teEnable - Enable or disable the pullup resistor  
//         teQual   - Select the number of samples for input qualification
GPIODRV_teSTATUS GPIODRV_fnUpdateOptions( GPIODRV_tpHANDLE   tpHandle,
                                          GPIODRV_teDIR      teNewDir,
                                          GPIODRV_tePULLUP   teEnable,                                 
                                          GPIODRV_teQUAL_SEL teQual );                                        

#endif  // GPIODRV_H_INCL
