/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the 
written approval of Xantrex is prohibited.

FILE NAME:  
    SysClk.c

PURPOSE:
    Provide access to set and retreive system clock speeds.

FUNCTION(S):
    SYSCLK_fnGetHiSpeedPeriphClk - Get high speed peripheral clock
    SYSCLK_fnSetHiSpeedPeriphClk - Set high speed peripheral clock
    SYSCLK_fnGetLoSpeedPeriphClk - Get low speed peripheral clock
    SYSCLK_fnSetLoSpeedPeriphClk - Set low speed peripheral clock
    SYSCLK_fnGetSysClk           - Get system clock
    SYSCLK_fnSetSysClk           - Set system clock
   
    local:
        none
   
NOTES:
    
CHANGE HISTORY:
$Log: SysClk.c $

    ***********************************************
    Revision: NovaPfrmB_TrevorM/1
    User: TrevorM     Date: 03/02/05  Time: 09:24PM
    Implemented the functions for setting SYSCLK, LSPCLK, and HSPCLK

    

    ***********************************************
    Revision: NovaPfrmB_JohnB/2
    User: JohnB     Date: 01/24/05  Time: 09:17PM
    Updated file header based on latest standards.
    
    
==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "SysClk.h"       // Public Access
#include "config.h"       // Hardware configuration
#include "device.h"       // 
#include "devicemacro.h"  // for enable/disable of protected registers
#include "xassert.h"      // Embedded assert

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


/*==============================================================================
                           Local/Private Constants
==============================================================================*/


/*==============================================================================
                           Local/Private Variables
==============================================================================*/


/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/


/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    SYSCLK_fnGetHiSpeedPeriphClk
  
PURPOSE: 
    Return high speed peripheral clock speed for TI TMS320F28x DSP.
    Affects EVA and EVB.  Ref TI SPRU078 Fig 3-1.

INPUTS: 
    None

OUTPUTS:
    return - High speed peripheral clock speed ( Hz )

NOTES:
    HSPCLK is based on SYSCLKOUT 
    if HISPCP > 0   
        low speed clk = (SYSCLKOUT / 2 * HISPCP)        
    else
        low speed clk = SYSCLKOUT
    
VERSION HISTORY:

Version: 0.01    Date: 02-Sep-2003    By: Huey Duong
    - Created
Version: 1.00    Date: 09-Oct-2003    By: Jeff Fieldhouse
    - Implemented    

*******************************************************************************/
uint32 SYSCLK_fnGetHiSpeedPeriphClk( void )
{
    uint32 SysClkSpeed;
    
    // Get the system clock speed in hertz
    SysClkSpeed = SYSCLK_fnGetSysClk();
    
    if( SysCtrlRegs.HISPCP.bit.HSPCLK > 0 )
    {
        // Return the high speed peripheral clock
        return( SysClkSpeed / ( 2 * SysCtrlRegs.HISPCP.bit.HSPCLK ) );
    }
    else
    {
        // Return the high speed peripheral clock
        return( SysClkSpeed );
    }
}

/*******************************************************************************

FUNCTION NAME: 
    SYSCLK_fnSetHiSpeedPeriphClk
  
PURPOSE: 
    Set high speed peripheral clock speed for TI TMS320F28x DSP.
    Affects EVA and EVB.  Ref TI SPRU078 Fig 3-1.

INPUTS: 
    ClkHz - High speed peripheral clock speed ( Hz )

OUTPUTS:
    None

NOTES:

    This function calculates the low speed peripheral clock prescalar based
    on the current system clock (SYSCLK) and the requested low speed peripheral
    clock speed in Hz.  
    
    Note that only the following prescalar values are supported:
    
        1, 2, 4, 6, 8, 10, 12, and 14.
    
    The function uses integer division to calculate the prescalar, so unless
    an exact fraction of the system clock is requested, the actual LSPCLK
    frequency may not be exactly as requested.
        
    The system clock frequency must be initialized before calling this function
    
VERSION HISTORY:

Version: 0.01    Date: 02-Sep-2003    By: Huey Duong
    - Created
Version: 0.02    Date: 09-Oct-2003    By: Jeff Fieldhouse
    - changed to do nothing
    - not needed at this stage of the development     
Version: 1.00    Date: 02-Mar-2005    By: Trevor Monk
    - implemented function to calculate the HSPCLK prescalar based on SYSCLK

*******************************************************************************/
void SYSCLK_fnSetHiSpeedPeriphClk( uint32 ClkHz )
{
    uint16 prescalar;  // prescalar for the low speed peripheral clock
    uint32 SysClk;     // system clock frequency

    // Check the limits
    assert( ClkHz <= CTRL_CPU_MAXSPEED );

    // enable write access to protected registers
    DEVICE_mACCESS_EN();

    // Get the current system clock frequency
    SysClk = SYSCLK_fnGetSysClk();
        
    // Set the low speed peripheral clock
    prescalar = (uint16) ( SysClk / ClkHz );
    
    // only the most significant 3 bits of the prescalar are used
    prescalar = prescalar >> 1;

    // write the HSPCLK prescalar register
    SysCtrlRegs.HISPCP.bit.HSPCLK = prescalar;
    
    // disable write access to the protected registers.
    DEVICE_mACCESS_DIS();
}

/*******************************************************************************

FUNCTION NAME: 
    SYSCLK_fnGetLoSpeedPeriphClk
  
PURPOSE: 
    Return low speed peripheral clock speed for TI TMS320F28x DSP.
    Affects SCI, SPI and McBSP.  Ref TI SPRU078 fig 3-1.

INPUTS: 
    None

OUTPUTS:
    return - Low speed peripheral clook speed ( Hz )

NOTES:
    LSPCLK is based on SYSCLKOUT 
    if LOSPCP > 0   
        low speed clk = (SYSCLKOUT / 2 * LOSPCP)        
    else
        low speed clk = SYSCLKOUT
    
VERSION HISTORY:

Version: 0.01    Date: 02-Sep-2003    By: Huey Duong
    - Created
Version: 1.00    Date: 09-Oct-2003    By: Jeff Fieldhouse
    - Implemented      

*******************************************************************************/
uint32 SYSCLK_fnGetLoSpeedPeriphClk( void )
{
    uint32 SysClkSpeed;
    
    // Get the system clock speed in hertz
    SysClkSpeed = SYSCLK_fnGetSysClk();
    
    if( SysCtrlRegs.LOSPCP.bit.LSPCLK > 0 )
    {
        // Return the low speed peripheral clock
		return ( SysClkSpeed / ( SysCtrlRegs.LOSPCP.bit.LSPCLK * 2 ) );
    }
    else
    {
        // Return the low speed peripheral clock
        return ( SysClkSpeed );
    }
}

/*******************************************************************************

FUNCTION NAME: 
    SYSCLK_fnSetLoSpeedPeriphClk
  
PURPOSE: 
    Set low speed peripheral clock speed for TI TMS320F28x DSP.
    Affects SCI, SPI and McBSP.  Ref TI SPRU078 fig 3-1.

INPUTS: 
    ClkHz - Low speed peripheral clock speed ( Hz )

OUTPUTS:
    None

NOTES:
    
    This function calculates the low speed peripheral clock prescalar based
    on the current system clock (SYSCLK) and the requested low speed peripheral
    clock speed in Hz.  
    
    Note that only the following prescalar values are supported:
    
        1, 2, 4, 6, 8, 10, 12, and 14.
    
    The function uses integer division to calculate the prescalar, so unless
    an exact fraction of the system clock is requested, the actual LSPCLK
    frequency may not be exactly as requested.
        
    The system clock frequency must be initialized before calling this function
    
VERSION HISTORY:

Version: 0.01    Date: 02-Sep-2003    By: Huey Duong
    - Created
Version: 0.02    Date: 09-Oct-2003    By: Jeff Fieldhouse
    - changed to do nothing
    - not needed at this stage of the development     
Version: 1.00    Date: 02-Mar-2005    By: Trevor Monk
    - implemented function to calculate the LSPCLK prescalar based on SYSCLK

*******************************************************************************/
void SYSCLK_fnSetLoSpeedPeriphClk( uint32 ClkHz )
{
    uint16 prescalar;  // prescalar for the low speed peripheral clock
    uint32 SysClk;     // system clock frequency
        
    // Check the limits
    assert( ClkHz <= CTRL_CPU_MAXSPEED );

    // enable write access to protected registers
    DEVICE_mACCESS_EN();

    // Get the current system clock frequency
    SysClk = SYSCLK_fnGetSysClk();
        
    // Set the low speed peripheral clock
    prescalar = (uint16) ( SysClk / ClkHz );
    
    // only the most significant 3 bits of the prescalar are used
    prescalar = prescalar >> 1;

    // write the LSPCLK prescalar register
    SysCtrlRegs.LOSPCP.bit.LSPCLK = prescalar;
    
    // disable write access to the protected registers.
    DEVICE_mACCESS_DIS();
}

/*******************************************************************************

FUNCTION NAME: 
    SYSCLK_fnGetSysClk
  
PURPOSE: 
    Return system clock speed for TI TMS320F28x DSP.
    Affects all modules of DSP.  Ref TI SPRU078 fig 3-1.

INPUTS: 
    None

OUTPUTS:
    return - System clock speed ( Hz )

NOTES:
    SYSCLKOUT and CLKIN are the same signal, so they are always the same 
    frequency. The formula for deriving the clock speed depends on whether the
    PLL is enabled, or is being bypassed:
    if PLLCR > 0   
        // PLL is enalbed
        SYSCLKOUT = (OSCCLK * PLLCR) / 2        
    else
        // PLL is bypassed
        SYSCLKOUT = OSCCLK / 2
        
    This is of course assuming that the PLL is enabled.
    
VERSION HISTORY:

Version: 0.01    Date: 02-Sep-2003    By: Huey Duong
    - Created
Version: 1.00    Date: 09-Oct-2003    By: Jeff Fieldhouse
    - Implemented      

*******************************************************************************/
uint32 SYSCLK_fnGetSysClk( void )
{
    // Determine if the PLL is in bypass mode or not
    if( SysCtrlRegs.PLLCR.all > 0 )
    {
        // Return the system clock speed based on PLL enabled formula
        return( ( CTRL_CONFIG_CRYSTAL * \
                ( uint32 )SysCtrlRegs.PLLCR.bit.DIV ) >> 1 );
    }
    else
    {
        // Return the system clock speed based on PLL bypass formula 
        return( CTRL_CONFIG_CRYSTAL >> 1 );
    }
}

/*******************************************************************************

FUNCTION NAME: 
    SYSCLK_fnSetSysClk
  
PURPOSE: 
    Set system clock speed for TI TMS320F28x DSP.
    Affects all modules of DSP.  Ref TI SPRU078 fig 3-1.

INPUTS: 
    ClkHz - System clock speed ( Hz )

OUTPUTS:
    None

NOTES:
    
VERSION HISTORY:

Version: 0.01    Date: 02-Sep-2003    By: Huey Duong
    - Created
Version: 0.02    Date: 09-Oct-2003    By: Jeff Fieldhouse
    - changed to do nothing
    - not needed at this stage of the development      
Version: 1.00    Date: 02-Mar-2005    By: Trevor Monk
    - implemented
    
*******************************************************************************/
void SYSCLK_fnSetSysClk( uint32 ClkHz )
{
    uint16 i;

    // Check the limits
    assert( ClkHz <= CTRL_CPU_MAXSPEED );

    // enable write access to protected registers
    DEVICE_mACCESS_EN();

    // Configure the PLL system clock divider
    SysCtrlRegs.PLLCR.bit.DIV = ( ( ClkHz / CTRL_CONFIG_CRYSTAL ) * 2u );

    // Wait for PLL to lock
    //
    // This value chosen from an example used by TI
    for( i = 0; i < 5000; i++ )
    {
    }
    
    // disable write access to the protected registers.
    DEVICE_mACCESS_DIS();
}

