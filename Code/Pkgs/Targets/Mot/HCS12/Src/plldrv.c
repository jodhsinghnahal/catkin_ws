/*==============================================================================
Copyright © 2003 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  plldrv.c

PURPOSE:
    Driver for the Motorola HCS12 Phase Lock Loop (PLL) to speed up the system
    clock.

FUNCTION(S):
    PLLDRV_fnCtor          - Initialize PLL filter

     local:
        none   
   
NOTES:
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use CodeWright or ignore 
    **the text.
    
    Only automatic and self clock modes are implemented in this file.  In 
    addition, none of the stop conditions have been implemented.
    

CHANGE HISTORY:
$History: plldrv.c $
    
    *****************  Version 14  *****************
    User: Hueyd        Date: 2/09/04    Time: 1:57p
    Updated in $/PD/Inverter_Charger/Inverter/SolarStorm_Series/Code/HCS12/Project/Build
    Fixed compiler warning about unused argument.
    
    *****************  Version 13  *****************
    User: Hueyd        Date: 11/06/04   Time: 6:19p
    Updated in $/PD/Inverter_Charger/Inverter/SolarStorm_Series/Code/Res_Series/GT2500/HCS12/Project/Build
    Checked PLLSEL before setting SYNR and REFDV
    PRN 2156
    
    *****************  Version 12  *****************
    User: Baldeeshk    Date: 8/08/03    Time: 4:55p
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Src
    Changed as part of code review.  Moved error codes to plldrv.h, and
    removed error macro call, instead writes errno directly.
    
    *****************  Version 11  *****************
    User: Baldeeshk    Date: 6/08/03    Time: 3:47p
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Src
    Changed API and added additional error codes to reflect various
    possible errors.
    
    *****************  Version 10  *****************
    User: Baldeeshk    Date: 31/07/03   Time: 8:46a
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Src
    Removed function PLL_fnGetBusClk
    
    *****************  Version 9  *****************
    User: Alant        Date: 25/07/03   Time: 5:17p
    Updated in $/PD/Common/FieldReprogram/BootBlock/Test/Motorola/HCS12
    - fnCtor: Fixed timeout check.
    - fnCtor: Cleaned up bit testing and assignments.
    - fnGetPLLClkFreq: Changed bus defines to those defined in Config.h
    
    *****************  Version 8  *****************
    User: Peterd       Date: 7/24/03    Time: 11:32a
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/BattMonEx/HAL/Src
    Changed #ifdef MCU_xxxx selection to #include "config.h"
    
    *****************  Version 7  *****************
    User: Zoranm       Date: 1/05/03    Time: 5:34p
    Updated in $/PD/Network_Extensions/Code/UI/SCP/HAL/Src
    Added support for 6812DG256B
    
    *****************  Version 6  *****************
    User: Baldeeshk    Date: 10/04/03   Time: 2:18p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Now returns the OSCLK if PLL is not turned on.
    
    *****************  Version 5  *****************
    User: Baldeeshk    Date: 9/04/03    Time: 3:51p
    Updated in $/PD/Network_Extensions/Code/UI/SCP/HAL/Src
    Added function PLL_fnGetPLLClkFreq
    
    *****************  Version 4  *****************
    User: Baldeeshk    Date: 18/02/03   Time: 12:58p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Changed if-defined statements from DG256 to DP256
    
    *****************  Version 3  *****************
    User: Zoranm       Date: 2/17/03    Time: 4:51p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Added conditional compile for DG128B and DG256B targets

==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "plldrv.h"       // pll driver header file
#include "config.h"       // contains hardware profile and register map
#include "errno.h"        // for writing error codes

/*==============================================================================
                              Defines
==============================================================================*/
#define PLLDRV_TIMEOUT_VALUE		( 65535 )  // maximum timeout value
#define PLLDRV_CME_MSK              ( 0x80 )   // clock monitor enable mask      
#define PLLDRV_ON_MSK               ( 0x40 )   // pllon mask
#define PLLDRV_AUTO_MSK             ( 0x20 )   // automatic filter mask
#define PLLDRV_ACQ_MSK              ( 0x10 )   // acquisition mode mask
#define PLLDRV_SELF_CLK_MSK         ( 0x01 )   // self-clk enable mask

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    PLLDRV_fnCtor
  
PURPOSE: 
    Initialize PLL filter

INPUTS: 
    ucSynr - multiplier for PLL filter
    ucRefdv - divider for PLL filter
    teFilterMode - pll filter setting, currently only automatic setting is
                    supported
    teResetMode - using self clock mode or clock reset mode upon a failure,
                  or not clock monitoring at all ( and hence no failure ).
    ptzStopMode - stop conditions for cpu wait and pseudo stop modes

OUTPUTS:
    eSTATUS_OK everything goes well 
    eSTATUS_ERR is there was an error is setting up the Pll

NOTES:
    This function must be called before any module.
    
    Only automatic and self clock modes are implemented in this function.  In 
    addition, none of the stop conditions have been implemented

VERSION HISTORY:

Version: 0.01  Date: 27/01/03  By: Zoran Miletic
    - Created
Version: 0.02  Date: 28/01/03  By: Zoran Miletic
    - Enhanced comments
Version: 1.00  Date: JUL-25-2003  By: Alan Travelbea
    - Fixed timeout check.
    - Cleaned up bit testing and assignments.
Version: 1.01  Date: AUG-05-2003  By: Baldeesh Khaira
    - Redesigned as part of code review
    - Changed API to make it more intuitive to the user     
Version: 1.02  Date: 11-Jun-2004  By: Huey Duong
    - Ensured PLLSEL is cleared before attempting to set SYNR and REFDV
    - Changed all comparisons to TRUE and FALSE to 1 and 0 because not all
      systems have TRUE as 1 and FALSE as 0.
Version: 1.03  Date: 02-Sep-2004  By: Huey Duong
    - Avoid compiler warning.      
*******************************************************************************/
tuiSTATUS PLLDRV_fnCtor( uchar8 ucSynr,     
                         uchar8 ucRefdv,    
                         PLLDRV_teFILTER_MODE teFilterMode,
                         PLLDRV_teCLK_FAIL_MODE teResetMode,  
                         PLLDRV_tzSTOP_MODE *ptzStopCond     
                    )
{

    uint16 TimeoutDwnCntr = PLLDRV_TIMEOUT_VALUE;  // timeout count
	
    // Avoid compiler warning
    *ptzStopCond = *ptzStopCond;

    // See if SYNR or REFDV needs to change
    if( ( SYNR != ucSynr ) || ( REFDV != ucRefdv ) )
    {
        // Cannot change SYNR nor REFDV if PLLSEL = 1
        if( CLKSEL_PLLSEL == 1 )
        {
            // If LOCK=0 and AUTO=1, or TRACK=0 and AUTO=0 PLLSEL cannot be
            // changed.  See CRG block user guide 3.3.4 to 3.3.6
            if(  ( ( CRGFLG_LOCK  == 0 ) && ( PLLCTL_AUTO == 1 ) )
              || ( ( CRGFLG_TRACK == 0 ) && ( PLLCTL_AUTO == 0 ) ) )
            {
                // There is no possible way to change the PLL
                return( eSTATUS_ERR );
            }
            
            // Allow PLL to selection
            CLKSEL_PLLSEL = 0;
        }
            
 
        // Change the PLL
        // PLLCLK = 2*OSCCLK*(SYNR+1)/(REFDV+1)
    	// assign mutiplier and divider values
    	SYNR = ucSynr;
    	REFDV = ucRefdv;
    }
	
    // currently only auto mode is supported and self clock mode is supported
    if( ( teFilterMode == PLLDRV_eFILTER_AUTO_MODE ) &&
        ( teResetMode == PLLDRV_eCLK_SELF_CLK_MODE ) )
    {
        // Configure PLL control register
        // for this implementation, only the clock monitoring bit,
        // pll on bit, auto filter bit, and self clock mode are turned on
        // all the rest are the same
        PLLCTL |= ( PLLDRV_CME_MSK | 
                    PLLDRV_ON_MSK | 
                    PLLDRV_AUTO_MSK |
                    PLLDRV_SELF_CLK_MSK );
     		 
    	// wait while PLL reaches desired clock or timeout occures 
	    while( CRGFLG_LOCK == FALSE )
	    {
		    if( --TimeoutDwnCntr == 0 )
		    {
		        // PLL is not locked, timeout error condition, system clock
		        // is running off the crystal, SCI driver on high speeds
		        // will not work	
		        errno = PLLDRV_eERROR_TIMEOUT;
		        return ( eSTATUS_ERR );
		    }
		}
	
	    // lock PLL, set PLLSET bit, PLLSET bit is cleared when the MCU
	    // enters Self Clock Mode, Stop Mode or Wait Mode with PLLWAI bit set
        CLKSEL_PLLSEL = TRUE;
	
        // PLL is locked! System clock is running of the PLL filter
        return ( eSTATUS_OK );
    }
    
    // other modes not supported
    errno = PLLDRV_eERROR_UNSUPPORTED_MODE; 
    return ( eSTATUS_ERR );
}





