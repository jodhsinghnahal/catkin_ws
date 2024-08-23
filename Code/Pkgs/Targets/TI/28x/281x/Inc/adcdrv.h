/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    adcdrv.h

PURPOSE:
    ADC Interface for TI c24x device.  Provides functionality for
    initialization, starting conversions and retrieving data.

FUNCTION(S):
    ADCDRV_fnStartConv     - Start the conv process
    ADCDRV_fnReset         - Resets the adc module
    ADCDRV_fnCtor          - Setup the ADC for this implemenation
    ADCDRV_fnSetMaxConv    - Sets the number of conversions
    ADCDRV_fnGetResultReg  - Returns the desired result register
    ADCDRV_fnGetResultChnl - Returns the result of the desired channel
    ADCDRV_fnGetSeqCnt     - Returns which conversion is taking place


    local:
        None

NOTES:
    DRIVER DESIGN
    -------------
    Due to the nature of this device and its features, making this interface
    generic is costly in terms of size.  ADC's are also usually used very
    specifically in applications running at high speeds.  Hence this driver is
    TARGET SPECIFIC.


    GENERAL NOTES
    -------------
    The results from the ADC are default as left justified by the hardware.
    There is a compile time switch in the .c to have the result left or right
    justified.  If the result is left justified, then right shift by
    ADC_RIGHT_JUSTIFY_SHIFT_VAL ( 4 ) to get a more meaningful number.

    This driver sets the ADC into cascade mode where the two available
    sequencers are put together to make one big one.

    In order to save some code space, this driver has no options available to
    the user.  However, changing the way the AD operates is fairly isolated to
    the function ADCDRV_fnCtor().

    USING THIS DRIVER
    -----------------
    To use this driver, call ADCDRV_fnCtor() to setup the module.
    After than, ADCDRV_fnStartConv() is used to start the conversion process as
    needed.

    To read a result, call ADCDRV_fnGetResult( ResultReg ), with the
    desired result register as specified by the enum ADCDRV_teRESULT_REG. You
    can also use ADCDRV_fnGetResultChnl() with the desired channel number as
    specified by ADCDRV_teCHNLS.  ADCDRV_fnGetResultChnl() takes a bit longer
    but is better for abstraction and limits changes to application code if
    AD signals get moved to different channels.


CHANGE HISTORY:
$Log: adcdrv.h $

    ***********************************************
    Revision: NovaPfrmB_Karli/2
    User: Karli     Date: 03/09/05  Time: 01:21AM
    Depricated macros to maintain abstraction.
    
    
    *****************  Version 3  *****************
    User: Yingrand     Date: 10/21/04   Time: 4:07p
    Updated in $/PD/Common/Targets/TI/c28x/HAL/Test/adcdrv
    add #include "device.h", a processor specific register is used
    
    *****************  Version 2  *****************
    User: Johnb        Date: 6/23/04    Time: 11:46a
    Updated in $/PD/Common/Targets/TI/c28x/HAL/Test/adcdrv
    Updated copyright notice.
    
    *****************  Version 1  *****************
    User: Brentt       Date: 6/22/04    Time: 2:45p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Test/adcdrv

Date:   Jan. 17, 2003   By: John Bellini
    -   Created.


==============================================================================*/

#ifndef ADCDRV_INCL
#define ADCDRV_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"   //Requres and architecture defintion in the compiler
#include "LIB_stddefs.h"
#include "device.h"        // processor specific register of ADC is used in
                           //macro defines

/*==============================================================================
                              Defines
==============================================================================*/

// NOTE: the use of the following macros is depricated. The equivalent function
// calls in the driver should be used to maintain the proper abstraction.
//
// ADCDRV_mGET_SEQ_CNT()            - use ADCDRV_fnGetSeqCnt
// ADCDRV_mGET_RESULT_REG( RegNum ) - use ADCDRV_fnGetResultReg

/*==============================================================================
                            Type Definitions
==============================================================================*/
//None

/*==============================================================================
                                Enums
==============================================================================*/
//Adc channels.  If your device has more, add them, if your device has less,
//just leave the list as is and use only what you need.
typedef enum ADCDRV_eCHNLS
{
    ADCDRV_eCHNL_0 ,    //Adc channel 0
    ADCDRV_eCHNL_1 ,    //Adc channel 1
    ADCDRV_eCHNL_2 ,    //Adc channel 2
    ADCDRV_eCHNL_3 ,    //Adc channel 3
    ADCDRV_eCHNL_4 ,    //Adc channel 4
    ADCDRV_eCHNL_5 ,    //Adc channel 5
    ADCDRV_eCHNL_6 ,    //Adc channel 6
    ADCDRV_eCHNL_7 ,    //Adc channel 7
    ADCDRV_eCHNL_8 ,    //Adc channel 8
    ADCDRV_eCHNL_9 ,    //Adc channel 9
    ADCDRV_eCHNL_10,    //Adc channel 10
    ADCDRV_eCHNL_11,    //Adc channel 11
    ADCDRV_eCHNL_12,    //Adc channel 12
    ADCDRV_eCHNL_13,    //Adc channel 13
    ADCDRV_eCHNL_14,    //Adc channel 14
    ADCDRV_eCHNL_15,    //Adc channel 15
    ADCDRV_eCHNL_MAX    //The maximum number of channels defined

}ADCDRV_teCHNLS;

//Each ADC's channel result register.  If your device on has one, just use
//REG_0 or REG_1 as needed.
typedef enum ADCDRV_eRESULT_REG
{
   ADCDRV_eRESULT_REG_0 = 0 ,      //Adc result register 0
   ADCDRV_eRESULT_REG_1     ,      //Adc result register 1
   ADCDRV_eRESULT_REG_2     ,      //Adc result register 2
   ADCDRV_eRESULT_REG_3     ,      //Adc result register 3
   ADCDRV_eRESULT_REG_4     ,      //Adc result register 4
   ADCDRV_eRESULT_REG_5     ,      //Adc result register 5
   ADCDRV_eRESULT_REG_6     ,      //Adc result register 6
   ADCDRV_eRESULT_REG_7     ,      //Adc result register 7
   ADCDRV_eRESULT_REG_8     ,      //Adc result register 8
   ADCDRV_eRESULT_REG_9     ,      //Adc result register 9
   ADCDRV_eRESULT_REG_10    ,      //Adc result register 10
   ADCDRV_eRESULT_REG_11    ,      //Adc result register 11
   ADCDRV_eRESULT_REG_12    ,      //Adc result register 12
   ADCDRV_eRESULT_REG_13    ,      //Adc result register 13
   ADCDRV_eRESULT_REG_14    ,      //Adc result register 14
   ADCDRV_eRESULT_REG_15,          //Adc result register 15
   ADCDRV_eRESULT_REG_MAX          //The maximum number of result regs defined

}ADCDRV_teRESULT_REG;


/*=============================================================================
                              Structures
==============================================================================*/
//None

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
//Create/initialization of this module.
//
void ADCDRV_fnCtor( void ); 

//Starts the conversion process.
//
void ADCDRV_fnStartConv( void );

//Reset the entire ADC module.  If this is used, the Ctor must be called again
//for this package to operate.
//
void ADCDRV_fnReset( void );

//Get Result Chnl:  gets the result based on the channel number.  This is the
//recommended function to use to maintain abstraction and limit changes if
//signals gets moved.
//
uint16 ADCDRV_fnGetResultChnl( ADCDRV_teCHNLS teAdcChnl );

//Get Result Reg: gets the result right out of the result reg.
//This is dependant on how the read order is setup.
//
uint16 ADCDRV_fnGetResultReg(  ADCDRV_teRESULT_REG teAdcResult );

//Returns a number representing where the module is currently reading in its
//seq.  Zero means its finished.  Hence, the number is the active conversion
//
uchar8 ADCDRV_fnGetSeqCnt( void );

//Sets the maximum number of allowed conversions to take place.
//
void ADCDRV_fnSetMaxConv( uint16 uiNumConv );

#endif  // ADCDRV_INCL
