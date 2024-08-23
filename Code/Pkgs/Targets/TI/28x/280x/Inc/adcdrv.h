/*==============================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:
    adcdrv.h

PURPOSE:
    ADC Interface for a TI 28x device.  Provides functionality for
    initialization, starting conversions and retrieving data.

FUNCTION(S):
    ADCDRV_fnStartConv        - Start the conv process
    ADCDRV_fnReset            - Resets the adc module
    ADCDRV_fnCtor             - Setup the ADC for this implemenation
    ADCDRV_fnDtor             - Shuts down the ADC module
    ADCDRV_fnSetMaxConv       - Sets the number of conversions
    ADCDRV_fnGetResultReg     - Returns the desired result register
    ADCDRV_fnGetResultChnl    - Returns the result of the desired channel
    ADCDRV_fnGetBoardRevision - Returns the version number for the ctrl board
    ADCDRV_fnGetSeqCnt        - Returns which conversion is taking place
    ADCDRV_fnGetSeqBusyStatus - Checks if sequencer is busy
    ADCDRV_fnSeq1Isr          - Interrupt service routine for Sequence 1
    ADCDRV_fnSeq2Isr          - Interrupt service routine for Sequence 2    

NOTES:
    DRIVER DESIGN
    -------------
    Due to the nature of this device and its features, making this interface
    generic is costly in terms of size.  ADC's are also usually used very
    specifically in applications running at high speeds.  Hence this driver is
    TARGET SPECIFIC.

CHANGE HISTORY:
$Log: adcdrv.h $

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/4
    User: JeffF     Date: 05/10/06  Time: 12:27AM
    Added code to read the voltage that corresponds to a hardware version number
    for the control board. Added a new function to convert the digital readout
    into a board version number for use by other files.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/3
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/2
    User: JeffF     Date: 05/05/05  Time: 12:17AM
    Fairly extensive modifications. Switched from a cascaded sequencer setup
    to a dual sequencer. Enabled interrupts for both sequence 1 and 2 upon
    end of conversion. Enabled sequencers to be started by PWM events. 

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
#include "LIB_stdtype.h"  // Requires an architecture definition in the compiler
#include "LIB_stddefs.h"  // Standard data types
#include "device.h"       // processor specific registers of ADC 

/*==============================================================================
                              Defines
==============================================================================*/

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
    ADCDRV_eCHNL_0 = 0, //Adc channel 0
    ADCDRV_eCHNL_1,     //Adc channel 1
    ADCDRV_eCHNL_2,     //Adc channel 2
    ADCDRV_eCHNL_3,     //Adc channel 3
    ADCDRV_eCHNL_4,     //Adc channel 4
    ADCDRV_eCHNL_5,     //Adc channel 5
    ADCDRV_eCHNL_6,     //Adc channel 6
    ADCDRV_eCHNL_7,     //Adc channel 7
    ADCDRV_eCHNL_8,     //Adc channel 8
    ADCDRV_eCHNL_9,     //Adc channel 9
    ADCDRV_eCHNL_10,    //Adc channel 10
    ADCDRV_eCHNL_11,    //Adc channel 11
    ADCDRV_eCHNL_12,    //Adc channel 12
    ADCDRV_eCHNL_13,    //Adc channel 13
    ADCDRV_eCHNL_14,    //Adc channel 14
    ADCDRV_eCHNL_15,    //Adc channel 15
    ADCDRV_eCHNL_MAX    //The maximum number of channels defined

}ADCDRV_teCHNLS;

//Each ADC's channel result register.  If your device has one, just use
//REG_0 or REG_1 as needed.
typedef enum ADCDRV_eRESULT_REG
{
   ADCDRV_eRESULT_REG_0 = 0,   //Adc result register 0
   ADCDRV_eRESULT_REG_1,       //Adc result register 1
   ADCDRV_eRESULT_REG_2,       //Adc result register 2
   ADCDRV_eRESULT_REG_3,       //Adc result register 3
   ADCDRV_eRESULT_REG_4,       //Adc result register 4
   ADCDRV_eRESULT_REG_5,       //Adc result register 5
   ADCDRV_eRESULT_REG_6,       //Adc result register 6
   ADCDRV_eRESULT_REG_7,       //Adc result register 7
   ADCDRV_eRESULT_REG_8,       //Adc result register 8
   ADCDRV_eRESULT_REG_9,       //Adc result register 9
   ADCDRV_eRESULT_REG_10,      //Adc result register 10
   ADCDRV_eRESULT_REG_11,      //Adc result register 11
   ADCDRV_eRESULT_REG_12,      //Adc result register 12
   ADCDRV_eRESULT_REG_13,      //Adc result register 13
   ADCDRV_eRESULT_REG_14,      //Adc result register 14
   ADCDRV_eRESULT_REG_15,      //Adc result register 15
   ADCDRV_eRESULT_REG_MAX      //The maximum number of result regs defined

}ADCDRV_teRESULT_REG;

// Allows access to either of the sequencers
typedef enum ADCDRV_eSEQ
{
    ADCDRV_eSEQ_1,
    ADCDRV_eSEQ_2
    
} ADCDRV_teSEQ;

// Used to tell the sequencer how many conversions it is to perform
typedef enum ADCDRV_eNUM_CONV
{
    ADCDRV_eNUM_CONV_1 = 0,   //Performs 1 conversion
    ADCDRV_eNUM_CONV_2,       //Performs 2 conversions
    ADCDRV_eNUM_CONV_3,       //Performs 3 conversions
    ADCDRV_eNUM_CONV_4,       //Performs 4 conversions
    ADCDRV_eNUM_CONV_5,       //Performs 5 conversions
    ADCDRV_eNUM_CONV_6,       //Performs 6 conversions
    ADCDRV_eNUM_CONV_7,       //Performs 7 conversions
    ADCDRV_eNUM_CONV_8,       //Performs 8 conversions
    ADCDRV_eNUM_CONV_9,       //Performs 9 conversions
    ADCDRV_eNUM_CONV_10,      //Performs 10 conversions
    ADCDRV_eNUM_CONV_11,      //Performs 11 conversions
    ADCDRV_eNUM_CONV_12,      //Performs 12 conversions
    ADCDRV_eNUM_CONV_13,      //Performs 13 conversions
    ADCDRV_eNUM_CONV_14,      //Performs 14 conversions
    ADCDRV_eNUM_CONV_15,      //Performs 15 conversions
    ADCDRV_eNUM_CONV_16,      //Performs 16 conversions
    ADCDRV_eNUM_CONV_MAX      //Used for range checking

} ADCDRV_teNUM_CONV;


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

//Shuts down the ADC module
//
void ADCDRV_fnDtor( void );

//Starts the conversion process.
//
void ADCDRV_fnStartConv( ADCDRV_teSEQ teSeqNum );

//ISR for ADC sequence 1 complete interrupt
//
void ADCDRV_fnSeq1Isr( void );

//ISR for ADC sequence 2 complete interrupt
void ADCDRV_fnSeq2Isr( void );

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

// Get the control boards version number
//
uint16 ADCDRV_fnGetBoardRevision( void );

//Returns a number representing where the module is currently reading in its
//seq.  Zero means either it is finished or it has 1 conversion to go.  
//Check the ADCDRV_fnGetSeqBusyStatus() to see if the sequencer is really done
//
uchar8 ADCDRV_fnGetSeqCnt( void );

// Checks if the selected sequence is busy or not - TRUE means busy
//
tucBOOL ADCDRV_fnGetSeqBusyStatus( ADCDRV_teSEQ teSeqNum );

//Sets the maximum number of allowed conversions to take place.
//
void ADCDRV_fnSetMaxConv( ADCDRV_teSEQ teSeqNum, ADCDRV_teNUM_CONV teNumConv );

#endif  // ADCDRV_INCL
