/*=============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.


FILE NAME:  iodrv.C
 
PURPOSE:
    This file contains generic I/O functions and Port definitions for the Motorola MC9S12DG128.
    
    The HCS12 has the following ports:
    
    	PORT A (multiplexed with address/data bus) 
    	PORT B (multiplexed with address/data bus)
    	PORT E (multiplexed with various control and interrupt lines)
    	PORT H (multiplexed with SPI)
    	PORT J (multiplexed with interrupt pins and I2C)
    	PORT K (multiplexed with address page pins)
    	PORT M (multiplexed with CAN, BDLC and Byte-Flight)
    	PORT P (multiplexed with PWMs)
    	PORT S (multiplexed with SCI and SPI)
    	PORT T (multiplexed with Input Camptures / Output Compares)
    	PORT AD (A/D channels and input port)
    	
    The I/O Driver has implemented the following functionality. A specific Initialization function
    must still be created to configure the I/O prior to accessing these functions.
    
    	PORT A Inputs  : none
    	PORT A Outputs : bits 0 to 3
    	PORT B Inputs  : none
    	PORT B Outputs : bits 0 to 5    	
    	PORT P Inputs  : bits 
    	PORT P Outputs : bits 0 to 2
    	PORT T Inputs  : bits 0 to 2
    	PORT T Outputs : bits 
    	
FUNCTION(S):
    IO_fnInit       - Initialize the I/O ports
    IO_fnSetIOPin	- Set the state of an Output Pin
    IO_fnGetIOPin	- Get the state of an Input Pin    

NOTES:
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use CodeWright or ignore 
    **the text.
    
    This code is written for the Motorola MC9S12DG128B
    It initializes the I/O ports which are used on the AGS and provides
    functions for setting the outpuuts and reading the inputs.

    ***********************************************************************
    ***********************************************************************
    Access DS2401 need change io pin direction, the common IO_fnRead and
    IO_fnSet function can not be used directly. 
    IO_fnSetDS2401 and IO_fnGetDS2401 will serve for DS2401 operation

HISTORY:
$History: iodrv.C $
    
    *****************  Version 10  *****************
    User: Ronm         Date: 3/30/04    Time: 3:09p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/HCS12/Packages/HAL/Src
    - PRN 1387: updated copyright notice
    
    *****************  Version 9  *****************
    User: Alant        Date: 12/09/03   Time: 11:05a
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Test/DIO
    - Updated to coding standard.
    - Replaced assert.h with Xantrex xassert.h
    - Removed unused errno.h
    
    *****************  Version 8  *****************
    User: Ronm         Date: 8/01/03    Time: 2:22p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Coprocessor/HAL/Src
    - Corrected fn names and parameter types.
    
    *****************  Version 7  *****************
    User: Ronm         Date: 7/24/03    Time: 4:16p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Coprocessor/HAL/Src
    - Corrected parameter and return value errors in some functions.
    
    *****************  Version 6  *****************
    User: Hairuozo     Date: 5/08/03    Time: 1:54p
    Updated in $/PD/Network_Extensions/Code/Management_Device/AGS/HAL/Src
    remove  IO_fnAGSCtor( void ) and IO_fnSCPCtor( void )
    
    *****************  Version 5  *****************
    User: Hairuozo     Date: 4/30/03    Time: 12:48p
    Updated in $/PD/Network_Extensions/Code/Management_Device/AGS/HAL/Src
    modified to meet common interface rquirement

    1. Added IO_fnAGSCtor()
    2.Change the parameter name to conform to coding standard:
           tucState --> State
    3.Change IO_fnGetIOPin so that it will return pin status instead of void

    *****************  Version 4  *****************
    User: Malcolmm     Date: 14/04/03   Time: 4:50p
    Updated in $/PD/Network_Extensions/Code/Management_Device/AGS/HAL/Src
    Added support for the DS2401 (PA7)
    
    *****************  Version 3  *****************
    User: Malcolmm     Date: 17/03/03   Time: 11:32a
    Updated in $/PD/Network_Extensions/Code/Management_Device/AGS/HAL/Src
    fixed bug in IO_fnGetIOPin function
    
    
    *****************  Version 2  *****************
    User: Malcolmm     Date: 5/03/03    Time: 5:23p
    Updated in $/PD/Network_Extensions/Code/Management_Device/AGS/HAL/Src
    split io driver into ags_io and iodrv
    
    *****************  Version 1  *****************
    User: Malcolmm     Date: 5/03/03    Time: 11:01a
    Created in $/PD/Network_Extensions/Code/Management_Device/AGS/HAL/Src
    
    *****************  Version 1  *****************
    User: Malcolmm     Date: 3/03/03    Time: 12:33p
    Created in $/PD/Network_Extensions/Code/Management_Device/AGS/Src
    
=============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/
#include "iodrv.h"
#include "xassert.h"    // assert


/*==============================================================================
                              Defines
==============================================================================*/


/*==============================================================================
                           Function Definitions
==============================================================================*/

/******************************************************************************

FUNCTION NAME:
    IO_fnSetIOPin

PURPOSE:
    This function turns an I/O Pin on or off according to the flag.

INPUTS:
    'PortPin' is an enumerated type identifying the specific port pin
    'State' is the desired state of the I/O pin

OUTPUTS:
    Nothing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 02/19/03  By: Malcolm McQueen
    - Initial Release
Version 1.01  Date: 03/06/03  By: Malcolm McQueen
    - Removed writes to data direction registers
    - Added functionality for PortB bits 0-5 to support the SCP
Version 1.02  Date: 03/07/03  By: Malcolm McQueen
    - Added functionality for PortP bit 3 for A model AGS

Version 1.03  Date: 04/15/03  By: Hairuo Zou
    - change parameter name to conform to coding standard
        tePortPin --> PortPin
        tucState --> State

Version 1.04  Date: Jul 24 2003  By: Ron Mok
    - Corrected parameter.

Version 1.05  Date: Jul 31 2003  By: Ron Mok
    - Corrected fn name.         
            
******************************************************************************/

void IODRV_fnSetPin( IODRVMAP_tuiPORT Port, 
                     IODRVMAP_tuiPIN  Pin, 
                     IODRV_tePIN_STATE State )
{
  
   IODRVMAP_tuiPIN Tmpport;
    
   Tmpport = *Port;     // Read Port
   
   if ( IODRV_ePIN_LOW == State )
   {
       Tmpport &= ~Pin;
       *Port = Tmpport;    // Update Port
        
   }
   else if ( IODRV_ePIN_HIGH == State )
   {
       Tmpport |= Pin;
       *Port = Tmpport;    // Update Port
   }
   else
   {            
       assert( 0 );
   }
}


/******************************************************************************

FUNCTION NAME:
    IO_fnGetIOPin

PURPOSE:
    This function returns the current state of an Input I/O pin.

INPUTS:
    'PortPin' is an enumerated type identifying the specific port pin

OUTPUTS:
	return the desired state of the I/O pin

NOTES:
	we will likely add a success/fail return value once they are defined
    
VERSION HISTORY:

Version 1.00  Date: 02/19/03  By: Malcolm McQueen
    - Initial Release
Version 1.01  Date: 03/13/03  By: Malcolm McQueen
    - changed to if else statements because casting to tucBOOL did not work   
Version 1.02  Date: 04/15/03  By: Hairuo Zou
    - using PORT address direct access IO pin, case statement removed.
    - change parameter name to conform to coding standard
        tePortPin --> PortPin

Version 1.03  Date: Jul 24 2003  By: Ron Mok
    - Corrected return type.         

Version 1.04  Date: Jul 31 2003  By: Ron Mok
    - Corrected fn name.         

******************************************************************************/
IODRV_tePIN_STATE IODRV_fnGetPin( IODRVMAP_tuiPORT Port, 
                                  IODRVMAP_tuiPIN  Pin )
{

    IODRVMAP_tuiPIN Tmpport;
    
    Tmpport = *Port;        //Read Port
    
    if( Pin == (Tmpport & Pin ) ) 
    {
        return IODRV_ePIN_HIGH;
    }
    else
    {
        return IODRV_ePIN_LOW;
    }

}
 
