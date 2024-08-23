/*==============================================================================
Copyright © 2003 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  
    plldrv.h

PURPOSE:
    Include file for using the Phase Lock Loop (PLL) clock multiplier to
    speed up the system clock.

FUNCTION(S):
    PLLDRV_fnCtor          - Initialize PLL filter
 

NOTES:
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use CodeWright or ignore 
    **the text.
    
    
HISTORY:
$History: plldrv.h $
    
    *****************  Version 10  *****************
    User: Ronm         Date: 9/11/03    Time: 11:30a
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/RV_Series/RS2000_1520B/HCS12/Inc
    - Changed the name of an enum so it wouldn't conflict with a Xanbus
    enum of the same name.
    
    *****************  Version 9  *****************
    User: Baldeeshk    Date: 8/08/03    Time: 4:23p
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Inc
    Added error codes in header file, changed naming of enums
    
    *****************  Version 8  *****************
    User: Baldeeshk    Date: 6/08/03    Time: 3:46p
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Inc
    Changed API of the constructor and added additional enumerations for
    setting filter and clock reset modes
    
    *****************  Version 7  *****************
    User: Baldeeshk    Date: 31/07/03   Time: 8:46a
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Inc
    Removed function PLL_fnGetBusClk
    
    *****************  Version 6  *****************
    User: Dalem        Date: 4/11/03    Time: 11:46a
    Updated in $/PD/Common/HAL/Inc
    Added automated history
    Reinserted PLL defines

Date:   01-27-2003   By: Zoran Miletic
    -   Created
Date:   01-27-2003   By: Zoran Miletic
    -   Enhanced comments

==============================================================================*/

#ifndef PLLDRV_INCL
#define PLLDRV_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"	// Requires architecture definition in compiler
#include "LIB_stddefs.h"    // Standard defintions file


/*==============================================================================
                                Structures
==============================================================================*/

// error enum
typedef enum ePLLDRV_ERROR
{
    PLLDRV_eERROR_NONE,             // no error
    PLLDRV_eERROR_TIMEOUT,          // timeout error
    PLLDRV_eERROR_UNSUPPORTED_MODE  // mode not supported error
}PLLDRV_teERROR;

// operating conditions in wait mode
typedef struct zWAIT
{
   uchar8 StopSysClk  : 1;     // stop system clock in wait mode
   uchar8 ReduceOsClk : 1;     // reduce pll clock in wait mode
   uchar8 StopPll     : 1;     // pll stops in wait mode
   uchar8 StopCore    : 1;     // core stops in wait mode
   uchar8 StopCOP     : 1;     // cop stops in wait mode
   uchar8 StopRTI     : 1;     // rti stop in wait mode
} PLLDRV_tzWAIT;

// operating conditions in pseudo-stop mode
typedef struct zPSEUDO_STOP
{
   uchar8 StopSysClk  : 1;     // stop system clock in pseudo-stop mode
   uchar8 StopCOP     : 1;     // stop cop in pseudo-stop mode
   uchar8 StopRTI     : 1;     // stop rti in pseudo-stop mode
} PLLDRV_tzPSEUDO_STOP;

// struct for stopping certain features in wait and pseudo-stop modes
typedef struct zSTOP_MODE
{
    PLLDRV_tzWAIT tzWaitCond;              // conditions in wait mode
    PLLDRV_tzPSEUDO_STOP tzPseudoStopCond; // conditions in pseudo-stop mode
} PLLDRV_tzSTOP_MODE;

/*==============================================================================
                                Enums
==============================================================================*/

// filter mode options
typedef enum PLLDRV_eFILTER_MODE
{
    PLLDRV_eFILTER_AUTO_MODE,         // automatic pll filter setting
    PLLDRV_eFILTER_HIGH_BANDWIDTH,    // manual high bandwidth filter setting
    PLLDRV_eFILTER_LOW_BANDWIDTH      // manual low bandwidth filter setting
}PLLDRV_teFILTER_MODE;

// clock failure options
typedef enum eCLK_FAIL_MODE
{
    PLLDRV_eCLK_MONITOR_DISABLED,   // clock monitoring disabled
    PLLDRV_eCLK_MONITOR_RESET,  // clcok failure detection causes reset
    PLLDRV_eCLK_SELF_CLK_MODE       // clock failure causes clock to run in 
                                // self clock mode
}PLLDRV_teCLK_FAIL_MODE;

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

// initialize the pll
tuiSTATUS PLLDRV_fnCtor( uchar8 ucSynr,     // pll loop multiplication value
                         uchar8 ucRefdv,    // reference clock divider value
                         PLLDRV_teFILTER_MODE teFilterMode,  // filter mode 
                                                             // is auto/ high/
                                                             // low bandwidth
                         PLLDRV_teCLK_FAIL_MODE teResetMode, // reset detection/
                                                            // handling options
                         PLLDRV_tzSTOP_MODE *ptzStopCond   // stop conditions
                    );
                    


#endif  // PLLDRV_INCL
