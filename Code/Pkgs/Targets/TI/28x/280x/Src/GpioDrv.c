/*==============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  
    GpioDrv.c

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
   
    local:
   
NOTES:
    This module is written for the TI 280x DSP.
    
    The RTOS's memory library can be used in this package because of it's 
    partition capabilities and also the fact that this memory segment can be 
    relocated to a different section other than dynamically allocated variables.
    
    The partition ability allows better allocation/deallocation of similar
    blocks and avoids memory fragmentation.
    
    If the standard library is used for dynamic allocation, all the memory
    will go to the same pool: the heap, and it can only be located to one
    section of memory.  By partitioning, each partition can be allocated
    independantly to make use of faster or slower RAM.

CHANGE HISTORY:
$Log: GpioDrv.c $

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/1
    User: HueyD     Date: 02/14/06  Time: 06:16PM
    Implemented change dir function

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/2
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/1
    User: JeffF     Date: 03/30/05  Time: 06:20PM
    Started using device macros to enable and disable writes to the protected
    registers.

    ***********************************************
    Revision: SurveyorPfrmASC60_JeffF/2
    User: JeffF     Date: 03/21/05  Time: 05:30PM
    Added casting in one place to avoid integer overflow. Added numerous comments
    to improve readability.

    
==============================================================================*/

// Define a shorter name for asserts
#define XASSERT_FILENAME "GpDr"

// Specify whether to use uCosII's memory library or to use standard library
// uCosII advantages: - Allows partitioning
//                    - Allows better memory allocation
// uCosII disadvantages: - May violate layer definitions
//                       - Mem allocated can only be used by this package
//                       - Less efficient code in terms of ROM and RAM
// stdlib advantages: - Does not violate layer definitions
//                    - Uses heap for mem which can be used by all packages
//                    - More efficient code in terms of ROM and RAM
// stdlib disadvantages: - Does not allow special memory reallocation for pkg
// Set to 1 if uCosII mem library is to be used
// Set to 0 if standard mem library is to be used
// Note: TRUE/FALSE was not used due to definition conflicts.
#define GPIODRV_USE_RTOS_MEM  ( 0 )

/*==============================================================================
                              Includes
==============================================================================*/

#if( GPIODRV_USE_RTOS_MEM == 1 )
#include "includes.h"       // RTOS libraries
#else
#include "stdlib.h"         // Standard library
#endif

#include "GpioDrv.h"        // Public Interface
#include "Device.h"         // Header for device specifics
#include "devicemacro.h"    // Header file for device macros
#include "config.h"         // Hardware configuration
#include "xassert.h"        // Error handling

/*==============================================================================
                              Defines
==============================================================================*/
#if( GPIODRV_USE_RTOS_MEM == 1 )
// Using RTOS mem allocation

    // Define maximum number of pin objects to allow.  This impacts the amount
    // of memory this module allocates
    #define GPIODRV_MAX_PINS  12

#endif
/*==============================================================================
                              Defines
==============================================================================*/

/*==============================================================================
                            Type Definitions
==============================================================================*/

/*==============================================================================
                                Enums
==============================================================================*/

/*=============================================================================
                              Structures
==============================================================================*/

// Define a structure for a GPIO pin object
typedef struct zGPIO_PIN
{
    GPIODRV_teGPIONUM teGpio;  // GPIO Pin number
    uint32            Mask;    // Bit mask of pin data
} tzGPIO_PIN;


/*==============================================================================
                           Local/Private Constants
==============================================================================*/


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

#if( GPIODRV_USE_RTOS_MEM == 1 )
// Using RTOS mem allocation

// Pointer to memory heap for GPIOs
static OS_MEM *tpMemBuf;


// Allocate the memory block for GPIOs
static uchar8 aucMemBuf[ GPIODRV_MAX_PINS ][ sizeof( tzGPIO_PIN ) ];

#endif

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
                                

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    GPIODRV_fnSetupMemory
  
PURPOSE: 
    Initialize the memory management system for the GPIO package

INPUTS: 
    None

OUTPUTS:
    Return - GPIODRV_eSTATUS_SUCCESS              : Success
             GPIODRV_eSTATUS_FAILURE_PARM_INVALID : Invalid parameter

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 01-Dec-2003    By: Huey Duong
    - Created
Version: 01.10    Date: 10-Dec-2003    By: Huey Duong
    - Added conditional compile for mem library
    
*******************************************************************************/

GPIODRV_teSTATUS GPIODRV_fnSetupMemory( void )
{
#if( GPIODRV_USE_RTOS_MEM == 1 )
// Using RTOS mem allocation

    uchar8 Error;   // OS error
    
    // Create a memory pool for gpio pins only
    tpMemBuf = OSMemCreate( aucMemBuf,
                            GPIODRV_MAX_PINS,
                            sizeof( tzGPIO_PIN ),
                            &Error );
    
    // Return status
    if( Error == OS_NO_ERR )
    {
        return( GPIODRV_eSTATUS_SUCCESS );
    }
    else
    {
        return( GPIODRV_eSTATUS_FAILURE_PARM_INVALID );
    }
#else
// Using standard library mem allocation
    // Do nothing for setup
    return( GPIODRV_eSTATUS_SUCCESS );
#endif    
}

/*******************************************************************************

FUNCTION NAME: 
    GPIODRV_fnCtor
  
PURPOSE: 
    Creates and initializes a GPIO object as an input/output pin.

INPUTS: 
    ptpHandle - Handle to created GPIO object
    tePin     - GPIO number
    teDir     - Direction of pin (input or output)
    teEnable  - Whether the pullup is enabled or disabled
    teQual    - How many samples are used to determine a logic transition

OUTPUTS:
    Return - GPIODRV_eSTATUS_SUCCESS
             GPIODRV_eSTATUS_FAILURE_PARM_INVALID
             GPIODRV_eSTATUS_FAILURE_OUT_OF_MEM
             GPIODRV_eSTATUS_FAILURE_PORT_NOT_SUPPORTED

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 14-Mar-2005    By: Jeff Fieldhouse
    - Created

*******************************************************************************/

GPIODRV_teSTATUS GPIODRV_fnCtor( GPIODRV_tpHANDLE*  ptpHandle,
                                 GPIODRV_teGPIONUM  tePin,
                                 GPIODRV_teDIR      teDir,
                                 GPIODRV_tePULLUP   teEnable,                                 
                                 GPIODRV_teQUAL_SEL teQual )
{
#if( GPIODRV_USE_RTOS_MEM == 1 )
// Using RTOS mem library
    uchar8 Error;   // OS error
#endif

    tzGPIO_PIN* pzHandle;  // Handle for gpio object
    uint32 BitMask;        // Mask of bit representing pin
    uint32 TempMask;       // Used to create the MuxMask
    uint32 MuxMask;        // The mux mask for each GPIO is 2 bits long
    uint32 QualMask;       // The qual mask is also 2 bits long

    // Initialize handle
    pzHandle = NULL;
    *ptpHandle = NULL;
    
    // Check for valid GPIO pin number
    // ********************************************************************
    // If this driver is ported for a different 280x DSP, with more or less
    // GPIO, this if() statement should be changed accordingly
    // ********************************************************************
    if( ( tePin < 0 ) || ( tePin > GPIODRV_eGPIO34 ) )
    {
        assert( FALSE );
        return( GPIODRV_eSTATUS_FAILURE_PARM_INVALID );
    }

    // To set the proper bit mask, determine which GPIO port is in use
    // It can only be port A or port B. Both are 32-bit/pin ports, but 
    // port B on the 280x only has 4 pins on it, the rest are reserved
    if( tePin < GPIODRV_eGPIO32 )
    {
        // Setup mask of bit positions
        BitMask = ( ( uint32 )0x00000001 << ( uint32 )tePin );
    }
    else
    {
        // Setup mask of bit positions by subtracting away the first 32 bits
        BitMask = ( ( uint32 )0x00000001 << 
                    ( uint32 )( tePin - GPIODRV_eGPIO32 ) );
    }
    
    // Check for overflow
    if( BitMask == 0 )
    {
        return( GPIODRV_eSTATUS_FAILURE_PARM_INVALID );
    }

    // Check for valid direction
    if(  ( teDir < GPIODRV_eDIR_INPUT )
      || ( teDir > GPIODRV_eDIR_OUTPUT ) )
    {
        return( GPIODRV_eSTATUS_FAILURE_PARM_INVALID );
    }

#if( GPIODRV_USE_RTOS_MEM == 1 )
// Using RTOS mem library

    // Allocate memory for object
    pzHandle = ( tzGPIO_PIN* ) OSMemGet( tpMemBuf, &Error );
#else
// Using standard mem library
    
    // Allocate memory for object
    pzHandle = ( tzGPIO_PIN* ) malloc( sizeof( tzGPIO_PIN ) );
#endif    
              
    // Ensure memory allocation was successful
    if( pzHandle == NULL )
    {
        return( GPIODRV_eSTATUS_FAILURE_OUT_OF_MEM );
    }

    // Assign the pin number and the corresponding bit mask to the GPIO object
    pzHandle->teGpio = tePin;
    pzHandle->Mask   = BitMask; 

    // *****************************************************************
    // The following block of code sets up the bit masks used to set the 
    // Mux bits and the qualification bits. Since these features have 4
    // potential settings, they use 2 bits to determine their behavior
    // *****************************************************************
    if( tePin < GPIODRV_eGPIO16 )
    {
        TempMask = BitMask;
    }
    else if( tePin < GPIODRV_eGPIO32 )
    {
        TempMask = ( BitMask >> 16 );
    }
    else if( tePin < GPIODRV_eGPIO48 )
    {
        TempMask = BitMask;
    }
    else
    {
        TempMask = ( BitMask >> 16 );
    }
        
    // Calculate the Mux mask based on the adjusted value of the bitmask
    MuxMask = ( TempMask * TempMask );
    MuxMask = ( ( MuxMask << 1 ) + MuxMask );

    // Calculate the Qualification mask based on the pin number and the 
    // desired qualification setting
    QualMask = ( ( uint32 )teQual << ( uint32 )( ( tePin % 16 ) * 2 ) );

    // *****************************************************************
    // The following block of code sets the physical register values.
    // *****************************************************************

    // Allow write access to the GPIO control registers for proper setup
    DEVICE_mACCESS_EN();

    // Determine which port the pin is on.
    if( tePin < GPIODRV_eGPIO32 )
    {
        // Clear the bits in the Mux register to ensure pin is set for GPIO
        // and set the Qualification mask to whatever the user selected     
        if( tePin < GPIODRV_eGPIO16 )
        {
            GpioCtrlRegs.GPAMUX1.all &= ~MuxMask;

            GpioCtrlRegs.GPAQSEL1.all &= ~MuxMask;
            GpioCtrlRegs.GPAQSEL1.all |= QualMask;
        }
        else
        {
            GpioCtrlRegs.GPAMUX2.all &= ~MuxMask;

            GpioCtrlRegs.GPAQSEL2.all &= ~MuxMask;
            GpioCtrlRegs.GPAQSEL2.all |= QualMask;

        }

        // Set the data direction register for input or output
        if( teDir == GPIODRV_eDIR_OUTPUT )
        {
            // It's an output, so set the pin to a 1            
            GpioCtrlRegs.GPADIR.all |= BitMask;
        }
        else
        {
            // It's an input, so clear the pin to a 0           
            GpioCtrlRegs.GPADIR.all &= ~BitMask;
        }

        // Set the pull up enable register according to user request
        if( teEnable == GPIODRV_ePULLUP_ENAB )
        {
            // Pull up is enabled by clearing the bit
            GpioCtrlRegs.GPAPUD.all &= ~BitMask; 
        }
        else
        {
            // Pull up is disabled by setting the bit
            GpioCtrlRegs.GPAPUD.all |= BitMask;
        }
    }
    else 
    {
        // Clear the bits in the Mux register to ensure pin is set for GPIO
        // and set the Qualification mask to whatever the user selected     
        if( tePin < GPIODRV_eGPIO48 )
        {
            GpioCtrlRegs.GPBMUX1.all &= ~MuxMask;

            GpioCtrlRegs.GPBQSEL1.all &= ~MuxMask;
            GpioCtrlRegs.GPBQSEL1.all |= QualMask;
        }
        else
        {
            GpioCtrlRegs.GPBMUX2.all &= ~MuxMask;

            GpioCtrlRegs.GPBQSEL2.all &= ~MuxMask;
            GpioCtrlRegs.GPBQSEL2.all |= QualMask;
        }

        // Set the data direction register for input or output
        if( teDir == GPIODRV_eDIR_OUTPUT )
        {
            // It's an output, so set the pin to a 1
            GpioCtrlRegs.GPBDIR.all |= BitMask;
        }
        else
        {
            // It's an input, so clear the pin to a 0
            GpioCtrlRegs.GPBDIR.all &= ~BitMask;
        }

        // Set the pull up enable register according to user request        
        if( teEnable == GPIODRV_ePULLUP_ENAB )
        {
            // Pull up is enabled by clearing the bit            
            GpioCtrlRegs.GPBPUD.all &= ~BitMask; 
        }
        else
        {
            // Pull up is disabled by setting the bit
            GpioCtrlRegs.GPBPUD.all |= BitMask;
        }
    }

    // *************************************************************************
    // Setup is complete, so disallow write access to the GPIO control registers
    // *************************************************************************
    DEVICE_mACCESS_DIS();
    
    // Return a handle to the new object
    *ptpHandle = pzHandle;

    // Success in setting up port attributes
    // Return the handle to the GPIO object created
    return( GPIODRV_eSTATUS_SUCCESS );
}

/*******************************************************************************

FUNCTION NAME: 
    GPIODRV_fnDtor
  
PURPOSE: 
    Destroys the GPIO object created from the constructor.  Frees the memory
    from the pool.

INPUTS: 
    ptpHandle  - Pointer to handle to destroy GPIO pin

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 01-Dec-2003    By: Huey Duong
    - Created
Version: 01.10    Date: 10-Dec-2003    By: Huey Duong
    - Added conditional compile for memory allocation
    - Changed parameter name
    
*******************************************************************************/

void GPIODRV_fnDtor( GPIODRV_tpHANDLE* ptpHandle )
{
    // Flag an error for debugging purposes
    assert( *ptpHandle != NULL );
    
    // Free memory if it was previously allocated
    if( *ptpHandle != NULL )
    {

#if( GPIODRV_USE_RTOS_MEM == 1 )
// Using RTOS mem lib

        // Return the previously allocated memory
        OSMemPut( tpMemBuf,
                  *ptpHandle );
#else
// Using standard mem lib
        // Return allocated memory to pool
        free( *ptpHandle );
#endif                  

        // Set the pointer to NULL
        *ptpHandle = NULL;
    }
}


/*******************************************************************************

FUNCTION NAME: 
    GPIODRV_fnClear
  
PURPOSE: 
    Sets the pin's state to 0.

INPUTS: 
    tpGpio - Handle to output pin to clear

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 19-Mar-2005    By: Jeff Fieldhouse
    - Modified driver from HCS12 implementation to 280x DSP implementation

*******************************************************************************/

void GPIODRV_fnClear( GPIODRV_tpHANDLE tpHandle )
{
    // Ensure valid handle
    assert( tpHandle != NULL );

    // Clear the pin
    if( ( ( tzGPIO_PIN* )tpHandle )->teGpio < GPIODRV_eGPIO32 )
    {   
        // Pin is on port A
        GpioDataRegs.GPACLEAR.all = ( ( tzGPIO_PIN* )tpHandle )->Mask;
    }
    else 
    {   
        // Pin is on port B
        GpioDataRegs.GPBCLEAR.all = ( ( tzGPIO_PIN* )tpHandle )->Mask;
    }
}


/*******************************************************************************

FUNCTION NAME: 
    GPIODRV_fnSet
  
PURPOSE: 
    Sets the pin's state to 1.

INPUTS: 
    tpGpio - Handle to output pin to set

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 19-Mar-2005    By: Jeff Fieldhouse
    - Modified driver from HCS12 implementation to 280x DSP implementation

*******************************************************************************/

void GPIODRV_fnSet( GPIODRV_tpHANDLE tpHandle )
{
    // Ensure valid handle
    assert( tpHandle != NULL );

    // Set the pin
    if( ( ( tzGPIO_PIN* )tpHandle )->teGpio < GPIODRV_eGPIO32 )
    {   
        // Pin is on port A        
        GpioDataRegs.GPASET.all = ( ( tzGPIO_PIN* )tpHandle )->Mask;
    }
    else 
    {   
        // Pin is on port B        
        GpioDataRegs.GPBSET.all = ( ( tzGPIO_PIN* )tpHandle )->Mask;
    }
}


/*******************************************************************************

FUNCTION NAME: 
    GPIODRV_fnToggle
  
PURPOSE: 
    Toggles the state of the pin.

INPUTS: 
    tpGpio - Handle to output pin to toggle

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 19-Mar-2005    By: Jeff Fieldhouse
    - Modified driver from HCS12 implementation to 280x DSP implementation

*******************************************************************************/
void GPIODRV_fnToggle( GPIODRV_tpHANDLE tpHandle )
{
    // Ensure valid handle
    assert( tpHandle != NULL );

    // Toggle the pin
    if( ( ( tzGPIO_PIN* )tpHandle )->teGpio < GPIODRV_eGPIO32 )
    {   
        // Pin is on port A        
        GpioDataRegs.GPATOGGLE.all = ( ( tzGPIO_PIN* )tpHandle )->Mask;
    }
    else 
    {   
        // Pin is on port B        
        GpioDataRegs.GPBTOGGLE.all = ( ( tzGPIO_PIN* )tpHandle )->Mask;
    }
}


/*******************************************************************************

FUNCTION NAME: 
    GPIODRV_fnIsSet
  
PURPOSE: 
    Query the status of the GPIO pin

INPUTS: 
    tpGpio - Handle to GPIO pin to query

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 19-Mar-2005    By: Jeff Fieldhouse
    - Modified driver from HCS12 implementation to 280x DSP implementation

*******************************************************************************/

tucBOOL GPIODRV_fnIsSet( GPIODRV_tpHANDLE tpHandle )
{
    // Ensure valid handle
    assert( tpHandle != NULL );

    // Return the pin state
    if( ( ( tzGPIO_PIN* )tpHandle )->teGpio < GPIODRV_eGPIO32 )
    {   
        // Pin is on port A        
        return(( GpioDataRegs.GPADAT.all & ((tzGPIO_PIN*)tpHandle)->Mask ) > 0);
    }
    else 
    {   
        // Pin is on port B        
        return(( GpioDataRegs.GPBDAT.all & ((tzGPIO_PIN*)tpHandle)->Mask ) > 0);
    }    
}


/*******************************************************************************

FUNCTION NAME: 
    GPIODRV_fnUpdateDir
  
PURPOSE: 
    Changes direction of the pin.

INPUTS: 
    tpHandle - Handler of gpio pin to update options
    teNewDir - New direction of port

OUTPUTS:
    Return - GPIODRV_eSTATUS_SUCCESS
             GPIODRV_eSTATUS_FAILURE
             GPIODRV_eSTATUS_FAILURE_PARM_INVALID

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 10-Dec-2003    By: Huey Duong
    - Created
Version: 01.01    Date: 19-Jan-2004    By: Huey Duong
    - Avoid compiler warning
    
*******************************************************************************/
GPIODRV_teSTATUS GPIODRV_fnUpdateDir( GPIODRV_tpHANDLE tpHandle,
                                      GPIODRV_teDIR    teNewDir )
{
    // Ensure valid handle
    assert( tpHandle != NULL );

    DEVICE_mACCESS_EN();

    // Return the pin state
    if( ( ( tzGPIO_PIN* )tpHandle )->teGpio < GPIODRV_eGPIO32 )
    {   
        // Pin is on port A        
        if( teNewDir == GPIODRV_eDIR_OUTPUT )
        {
            GpioCtrlRegs.GPADIR.all |= ((tzGPIO_PIN*)tpHandle)->Mask;
        }
        else
        {
            GpioCtrlRegs.GPADIR.all &= ~((tzGPIO_PIN*)tpHandle)->Mask;
        }
    }
    else 
    {   
        // Pin is on port B        
        if( teNewDir == GPIODRV_eDIR_OUTPUT )
        {
            GpioCtrlRegs.GPBDIR.all |= ((tzGPIO_PIN*)tpHandle)->Mask;
        }
        else
        {
            GpioCtrlRegs.GPBDIR.all &= ~((tzGPIO_PIN*)tpHandle)->Mask;
        }
    }    
    
    DEVICE_mACCESS_DIS();

    return( GPIODRV_eSTATUS_SUCCESS );
}    


/*******************************************************************************

FUNCTION NAME: 
    GPIODRV_fnUpdateOptions
  
PURPOSE: 
    Updates extended options of the pin

INPUTS: 
    tpHandle - Handler of gpio pin to update options
    teNewDir - New direction of port

OUTPUTS:
    Return - GPIODRV_eSTATUS_SUCCESS
             GPIODRV_eSTATUS_FAILURE
             GPIODRV_eSTATUS_FAILURE_PARM_INVALID

NOTES:

VERSION HISTORY:

Version: 01.00    Date: 10-Dec-2003    By: Huey Duong
    - Created
Version: 01.01    Date: 19-Jan-2004    By: Huey Duong
    - Avoid compiler warning

*******************************************************************************/
GPIODRV_teSTATUS GPIODRV_fnUpdateOptions( GPIODRV_tpHANDLE   tpHandle,
                                          GPIODRV_teDIR      teNewDir,
                                          GPIODRV_tePULLUP   teEnable,                                 
                                          GPIODRV_teQUAL_SEL teQual )
{
    // TBD this is temporary code to avoid compiler warnings
    volatile GPIODRV_tpHANDLE tpHandleScratch;
    volatile GPIODRV_teDIR teDirScratch;
    
    tpHandleScratch = tpHandle;
    teDirScratch = teNewDir;

    // Unimplemented function
    return( GPIODRV_eSTATUS_FAILURE );
}   
