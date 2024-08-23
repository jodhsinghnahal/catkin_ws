/*==============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without 
the express written permission of Xantrex International.

FILE NAME:  
    device.h

PURPOSE:
    The file includes processor specific defines.  It includes the appropriate
    register mapping file.  It also contains basic device macros for enabling/
    disabling interrupts and the watchdog timer

FUNCTION(S):
     none
   
    local:
     none
   
NOTES:
    
HISTORY:
$Log: device.h $

    ***********************************************
    Revision: ScpPfrmB1_HollyZ/2
    User: HollyZ     Date: 02/25/06  Time: 12:32AM
    Device header file for HCS12X

    ***********************************************
    Revision: ScpPfrmB1_HollyZ/1
    User: HollyZ     Date: 01/21/06  Time: 12:14AM
    copyright updated

    
==============================================================================*/

#ifndef DEVICE_INCL
#define DEVICE_INCL

/*==============================================================================
                              Includes
==============================================================================*/

/*==============================================================================
                              Defines
==============================================================================*/

#if defined( MC9S12XDT512 ) 
  #include "mc9s12xdt512.h"
#elif defined( MC9S12XDP512 ) 
  #include "mc9s12xdp512.h"  
#else
#error "Processor type not defined/recognized."
#endif


// define the watchdog reset keys
#define WDKEY_FIRST			(0x55)
#define WDKEY_SECOND		(0xAA)

/*==============================================================================
                              Macros
==============================================================================*/

// enable interrupts
#define DEVICE_mGLOBAL_INT_EN( )     	 _asm( CLI )

// disable interrupts
#define DEVICE_mGLOBAL_INT_DIS( )    	 _asm( SEI )


// enables the watchdog timer with the rate supplied by user
// Options:  disable windowed COP mode, stop watchdog in Active BDM mode
// Note:     Valid values of Rate are 0...7
#define DEVICE_mWD_EN( Rate ) 	         COPCTL_CR = Rate & 0x07

// disables the watchdog timer
// writes a value to the divider register, effectively disabling it                                 
#define DEVICE_mWD_DIS( )     	    	 COPCTL_CR = 0

// reset the watchdog timer
#define DEVICE_mWD_RST()	        	 ARMCOP = WDKEY_FIRST; \
                                    	 ARMCOP = WDKEY_SECOND

// Shutdown the Device - in the HCS12 Case,  the CLKSEL register
// specifies what clocks are disabled when this command is executed.
#define DEVICE_mSHUTDOWN()        		 _asm( WAI )

// trigger a Watchdog Reset
#define DEVICE_mWD_TRIGGER()        	 DEVICE_mWD_EN( 1 ); \
                                        for( ;; ) \
                                    	   ARMCOP = 1

// reset the Device - in the HCS12 case, timeout the Watchdog
#define DEVICE_mCPU_RST()          		 COPCTL_CR = 1;\
                                         for(;;)       \
                                         ARMCOP = 0x0000


#endif  // DEVICE_INCL
