/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    PwmDrvC28xOpt.h

PURPOSE:
    To define specific target information.  This is done so the PWMDRV.h file
    can be more generic while still allowing the driver to have a target 
    specific component.

FUNCTION(S):
    PWMDRV_mUPDATE_COMP1_FAST
    PWMDRV_mUPDATE_COMP2_FAST
    PWMDRV_mUPDATE_COMP3_FAST
    PWMDRV_mDIS_EVA_ALL
    PWMDRV_mEN_EVA_ALL
    PWMDRV_mDIS_EVB_ALL
    PWMDRV_mEN_EVB_ALL
    PWMDRV_mMAX_DEADBAND_NS
   
    local:
        None
   
NOTES:
    None

CHANGE HISTORY:
$Log: PwmDrvC28xOpt.h $

    ***********************************************
    Revision: NovaPfrmB_JohnB/2
    User: JohnB     Date: 01/25/05  Time: 12:24AM
    Fixed dead band calculation with () around the denominator
    so it calc's correctly.
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/1
    User: JohnB     Date: 01/24/05  Time: 09:14PM
    Updated for use with SYSCLK package. Removed INIT_fnGetSysClk()
    functionality.
    

    ***********************************************
    Revision: NovaPfrmB_HorstM/2
    User: HorstM     Date: 01/06/05  Time: 12:56AM
    modified file header (PRN 1835)
    
    
==============================================================================*/
#ifndef PWMDRV_C28X_OPT_INCL
#define PWMDRV_C28X_OPT_INCL

/*==============================================================================
                              Includes
==============================================================================*/
#include "pwmdrv.h" // Package interface
#include "config.h" // for get clock speed function
#include "Device.h"	// Target definitions

/*==============================================================================
                              Defines
==============================================================================*/
//The following macro's should be used where speed is an issue.  If speed is
//not an issue, use the PWMDRV API as designed.
#define PWMDRV_mUPDATE_COMP1_FAST( Val )    EvaRegs.CMPR1 = ( Val )
#define PWMDRV_mUPDATE_COMP2_FAST( Val )    EvaRegs.CMPR2 = ( Val )
#define PWMDRV_mUPDATE_COMP3_FAST( Val )    EvaRegs.CMPR3 = ( Val )
#define PWMDRV_mDIS_EVA_ALL()  ( EvaRegs.COMCONA.all &= ~F28X_COMCON_FCMPOE_EN )
#define PWMDRV_mEN_EVA_ALL()   ( EvaRegs.COMCONA.all |= F28X_COMCON_FCMPOE_EN )
#define PWMDRV_mDIS_EVB_ALL()  ( EvbRegs.COMCONB.all &= ~F28X_COMCON_FCMPOE_EN )
#define PWMDRV_mEN_EVB_ALL()   ( EvbRegs.COMCONB.all |= F28X_COMCON_FCMPOE_EN )
// maximum possible deadband in nanoseconds
#define PWMDRV_mMAX_DEADBAND_NS() ((uint16)(480000/(CTRL_CPU_MAXSPEED/1000000)))

/*==============================================================================
                            Type Definitions
==============================================================================*/
//None

/*==============================================================================
                                Enums
==============================================================================*/
//None

/*=============================================================================
                              Structures
==============================================================================*/
//Target specific information for a TI C28x device
typedef struct PWMDRV_zOPTIONS_TI_C28X
{
    uint16 uiDesiredDbNs;               //DeadBand in ns

    //Compare options
    PWMDRV_teCOMP_EN       teCompEn;       //Compare Enable Option
    PWMDRV_teCOMP_RELOAD   teCompReload;   //Compare Reload conditions
    PWMDRV_teACTION_RELOAD teActionReload; //Action ctrl reg reload cond
    
    //Action options
    PWMDRV_tePOLARITY tePin1State;  //Pin active state on compares
    PWMDRV_tePOLARITY tePin2State;  //Pin active state on compares
    PWMDRV_teDB_TMR   teDbTmrEn;    //Whether the dead band tmr is en

}PWMDRV_tzOPTIONS_TI_C28X;

/*==============================================================================
                          External/Public Constants
==============================================================================*/
//None

/*==============================================================================
                          External/Public Variables
==============================================================================*/
//None

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
//None

#endif  // PWMDRV_C28X_OPT_INCL
