/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    iodrv.h

PURPOSE:
    Controls access to the serial to parallel output driver on the ACB

FUNCTION(S):
    IODRV_fnCtor           - Initialize the serial to parallel output driver
    IODRV_Write            - Write the output register to the output latch

    local:
        None

NOTES:


CHANGE HISTORY:
$History: iodrv.h $

==============================================================================*/

#ifndef IODRV_INCL
#define IODRV_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"   //Requres and architecture defintion in the compiler
#include "LIB_stddefs.h"

/*==============================================================================
                              Defines
==============================================================================*/
#define IODRV_ENABLE 1
#define IODRV_DISABLE 0

#define IODRV_CHG_STATUS_LED        ( IodrvRegs.OUTREG.bit.CHG_STATUS_LED )
#define IODRV_INV_STATUS_LED        ( IodrvRegs.OUTREG.bit.INV_STATUS_LED )
#define IODRV_FAULT_LED             ( IodrvRegs.OUTREG.bit.FAULT_LED )
#define IODRV_BAT_LO_LED            ( IodrvRegs.OUTREG.bit.BAT_LO_LED )
#define IODRV_INV_ON_LED            ( IodrvRegs.OUTREG.bit.INV_ON_LED )
#define IODRV_ACIN_CHARGE_LED       ( IodrvRegs.OUTREG.bit.ACIN_CHARGE_LED )
#define IODRV_NETWORK_LED           ( IodrvRegs.OUTREG.bit.NETWORK_LED )
#define IODRV_TP39                  ( IodrvRegs.OUTREG.bit.TP39 )
#define IODRV_GP_LED1               ( IodrvRegs.OUTREG.bit.GP_LED1 )
#define IODRV_GP_LED2               ( IodrvRegs.OUTREG.bit.GP_LED2 )
#define IODRV_GP_LED3               ( IodrvRegs.OUTREG.bit.GP_LED3 )
#define IODRV_GP_LED4               ( IodrvRegs.OUTREG.bit.GP_LED4 )
#define IODRV_GP_LED5               ( IodrvRegs.OUTREG.bit.GP_LED5 )
#define IODRV_DC_UV_RLY             ( IodrvRegs.OUTREG.bit.DC_UV_RLY )
#define IODRV_TP10                  ( IodrvRegs.OUTREG.bit.TP10 )
#define IODRV_TP11                  ( IodrvRegs.OUTREG.bit.TP11 )

/*==============================================================================
                              Macros
==============================================================================*/
#define IODRV_mUpdateOutput() ( IODRV_Write( IodrvRegs.OUTREG.all ) )

// set the status LED on or off
#define IODRV_mCHG_STATUS_LED( state )  ( IODRV_CHG_STATUS_LED = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the inverter LED on or off
#define IODRV_mINV_STATUS_LED( state )  ( IODRV_INV_STATUS_LED = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the fault LED on or off
#define IODRV_mINV_FAULT_LED( state )   ( IODRV_FAULT_LED = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the Battery Low LED on or off
#define IODRV_mBAT_LO_LED( state )      ( IODRV_BAT_LO_LED = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the Inverter On LED on or off
#define IODRV_mINV_ON_LED( state )      ( IODRV_INV_ON_LED = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the ACIN / Charge LED on or off
#define IODRV_mACIN_CHARGE_LED( state ) ( IODRV_ACIN_CHARGE_LED = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the Network LED on or off                                      
#define IODRV_mNETWORK_LED( state )     ( IODRV_NETWORK_LED = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the TP39 test point on or off
#define IODRV_mTP39( state )            ( IODRV_TP39 = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the TP10 test point on or off
#define IODRV_mTP10( state )            ( IODRV_TP10 = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the TP11 test point on or off
#define IODRV_mTP11( state )            ( IODRV_TP11 = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the GP1 LED on or off
#define IODRV_mGP_LED1( state )         ( IODRV_GP_LED1 = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the GP2 LED on or off
#define IODRV_mGP_LED2( state )         ( IODRV_GP_LED2 = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the GP3 LED on or off
#define IODRV_mGP_LED3( state )         ( IODRV_GP_LED3 = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the GP4 LED on or off
#define IODRV_mGP_LED4( state )         ( IODRV_GP_LED4 = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the GP5 LED on or off
#define IODRV_mGP_LED5( state )         ( IODRV_GP_LED5 = state; \
                                         IODRV_mUpdateOutput(); ) 
// set the DC UV Relay on or off
#define IODRV_mDC_UV_RLY( state )       ( IODRV_DC_UV_RLY = state; \
                                         IODRV_mUpdateOutput(); ) 

/*==============================================================================
                            Type Definitions
==============================================================================*/

// IO driver output bits
typedef struct zOUT_BITS 
{                            // bits   description
   uint16 CHG_STATUS_LED:1;  // 0      Charge Status LED
   uint16 INV_STATUS_LED:1;  // 1      Inverter Status LED
   uint16 FAULT_LED:1;       // 2      Fault LED
   uint16 BAT_LO_LED:1;      // 3      Low Battery LED
   uint16 INV_ON_LED:1;      // 4      Inverter On LED
   uint16 ACIN_CHARGE_LED:1; // 5      AC in / Charge LED
   uint16 NETWORK_LED:1;     // 6      Network LED
   uint16 TP39:1;            // 7      Test Point 39
   
   uint16 GP_LED1:1;         // 8      General Purpose LED 1
   uint16 GP_LED4:1;         // 9      General Purpose LED 4
   uint16 GP_LED2:1;         // 10     General Purpose LED 2
   uint16 GP_LED5:1;         // 11     General Purpose LED 5
   uint16 GP_LED3:1;         // 12     General Purpose LED 3
   uint16 DC_UV_RLY:1;       // 13     DC Under Voltage Relay
   uint16 TP10:1;            // 14     Test Point 10
   uint16 TP11:1;            // 15     Test Point 11
} IODRV_tzOUT_BITS;

// IO driver output register
union IODRV_OUTREG 
{
   Uint16            all;
   IODRV_tzOUT_BITS  bit;
};

// IO driver Register File:
struct  IODRV_REGS 
{
   union IODRV_OUTREG   OUTREG;    // IO driver output register
};


/*==============================================================================
                                Enums
==============================================================================*/

/*=============================================================================
                              Structures
==============================================================================*/
//None

/*==============================================================================
                          External/Public Constants
==============================================================================*/
extern IODRV_REGS IodrvRegs;

/*==============================================================================
                          External/Public Variables
==============================================================================*/
//None

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
//Create/initialization of this module.
//
void IODRV_fnCtor( void );
void IODRV_Write( uint16 outval );

#endif  // ADCDRV_INCL
