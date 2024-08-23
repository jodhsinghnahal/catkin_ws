/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    PwmDrv.h

PURPOSE:
    To define a common interface for a PWM module regardless of the target.

FUNCTION(S):
    PWMDRV_fnCtor            - Constructor
    PWMDRV_fnDtor            - Destructor
    PWMDRV_fnEn              - En the PWM(op's en)
    PWMDRV_fnDis             - Dis the PWM(op's hi-Z)
    PWMDRV_fnUpdateDutyCycle - How to update the PWM
    PWMDRV_fnSetOpt          - Set PWM options 
    PWMDRV_fnGetMaxDutyCycle - Get the maximum value for the duty cycle
   
    local:
        Private
   
NOTES:
    This file is a common interface for a PWM.  The intent of this interface
    is for it to remain constant amongst as many projects as possible.  This
    will maintain a consistant level of good abstraction amongst projects.

    USAGE:
    The following describes how to obtain a PWM.
    
    -----
    SETUP
    -----
    - Create a tzPWM structure.
    - Create a structure of your own and fill it up with options that are
      specific to your device (TI, MOT, etc).  If you don't need any extra 
      options, set the pointer value to NULL.  Assign this structure to 
      ptzPwmOpt in the tzPWM struct.

    Now the two structure are ready to go, fill them in.  Set the options in 
    the structure you created as needed.  For the rest of the data in tzPWM,
    do the following:
    - Select which of the waveform types you want to work with from the enum
      PWMDRV_eWF_TYPE and assign it to tePwmType in the struct.
    - Set the initial desired period of the pwm by setting ulDesiredFreqHz if 
      you need to 
    - Init the duty cycle value as well to either zero or your first know val.
    - Don't do anything with uiPwmId.

    -----
    CTOR
    -----
    - Pass your tzPWM to the function: PWMDRV_fnCtor().  This fn will setup the
      PWM based on the options you've entered.

    - To update the PWM call PWMDRV_fnUpdateDutyCycle( ) passing it your tzPWM
      if the new duty cycle value already updated.
    - To enable or disable the outputs of your PWM call, PWM_fnDis() or 
      PWM_fnEn() passing your tzPWM structure.


CHANGE HISTORY:
$Log: pwmdrv.h $
    
==============================================================================*/

#ifndef PWMDRV_INCL
#define PWMDRV_INCL

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    //_16BIT_ARCHITECTURE defined in Compiler Options
#include "LIB_stddefs.h"    //Standard definitions.
#include "HALErrBase.h"     //For return type error codes (using errno)

/*==============================================================================
                              Defines
==============================================================================*/
//None

/*==============================================================================
                            Type Definitions
==============================================================================*/
//None

/*=============================================================================
                                Enums
==============================================================================*/
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
**The following are options used to setup the PWM
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
//The types of PWM waveform generation available.  Each requires diff setups.
//The following types will give back 1 pair of complimentary pair of PWM's
//except for the SPV options which uses all three.  If the user requires a
//second set of PWM's (a second pair), then another call to the driver is req.
typedef enum PWMDRV_eTYPE
{
    PWMDRV_eTYPE_INVL = 0   ,  //So zero is not used.
    PWMDRV_eTYPE_ASYMMETRIC ,  //Left align
    PWMDRV_eTYPE_SYMMETRIC  ,  //Center align
    PWMDRV_eTYPE_SPV        ,  //Space vector switching scheme
    PWMDRV_eTYPE_MAX           //Used to help type checking
    
}PWMDRV_teTYPE;

//The PWM ID's available by this driver
typedef enum ePWM_ID
{
    ePWM_ID_INVL = 0 ,   //So zero is not used.
    ePWM_ID_1_2      ,   //PWM  1 & 2 complimentary pairs
    ePWM_ID_3_4      ,   //PWM  3 & 4 complimentary pairs
    ePWM_ID_5_6      ,   //PWM  5 & 6 complimentary pairs
    ePWM_ID_7_8      ,   //PWM  7 & 8 complimentary pairs
    ePWM_ID_9_10     ,   //PWM  9 & 10 complimentary pairs
    ePWM_ID_11_12    ,   //PWM 11 & 12 complimentary pairs
    ePWM_ID_SPV_A    ,   //SPV PWM for EVA
    ePWM_ID_SPV_B    ,   //SPV PWM for EVB
    ePWM_ID_MAX          //Max for limit checking.
}tePWM_ID;

/*^^^^^^^^^^^^^
COMPARE OPTIONS
^^^^^^^^^^^^^*/
//Compare Enable Options
typedef enum PWMDRV_eCOMP_EN
{
    PWMDRV_eCOMP_EN_OFF = 0 ,//Disables compare operation.
    PWMDRV_eCOMP_EN_ON       //Enables compare operation
    
}PWMDRV_teCOMP_EN;

//Compare register reload conditions
typedef enum PWMDRV_eCOMP_RELOAD
{
    PWMDRV_eCOMP_RELOAD_UF = 0    ,//When timer = 0, (that is, on underflow)
    PWMDRV_eCOMP_RELOAD_UF_OR_PRD ,//When timer = 0 or timer = timer period
                                //(that is, on underflow or period match)
    PWMDRV_eCOMP_RELOAD_IMM        //Immediately
    
}PWMDRV_teCOMP_RELOAD;

//Action control register reload condition.
typedef enum PWMDRV_eACTION_RELOAD
{
    PWMDRV_eACTION_RELOAD_UF = 0    ,//When timer = 0, (that is, on underflow)
    PWMDRV_eACTION_RELOAD_UF_OR_PRD ,//When timer = 0 or timer timer = period
                                  //(that is, on underflow or period match)
    PWMDRV_eACTION_RELOAD_IMM        //Immediately
    
}PWMDRV_teACTION_RELOAD;

//Compare output enable.
typedef enum PWMDRV_eCOMP_OP
{
    PWMDRV_eCOMP_OP_DIS = 0 ,//PWM op pins are hi-Z state that is they are dis
    PWMDRV_eCOMP_OP_EN       //PWM op pins are not hi-Z state that is they are en
    
}PWMDRV_teCOMP_OP;

//Size of compare and count register
typedef enum PWMDRV_eCOMP_SIZE
{
    PWMDRV_e8BIT   = 0,    //Compare size 8 bits
    PWMDRV_e16BIT          //Compare size 16 bits
    
}PWMDRV_teCOMP_SIZE;


/*^^^^^^^^^^^^^
ACTION OPTIONS
^^^^^^^^^^^^^*/
//Space vector PWM rotation direction. 
//Used only in space vector PWM output generation.
typedef enum PWMDRV_eSPV_ROTATION
{
    PWMDRV_eSPV_ROTATION_CCW = 0 , //Positive counter clockwise direction
    PWMDRV_eSPV_ROTATION_CW        //Negative clockwise direction
    
}PWMDRV_teSPV_ROTATION;

//Polarity options for the output of the PWM's.
typedef enum PWMDRV_ePOLARITY
{
    PWMDRV_ePOL_FORCE_LOW = 0 , //Forced low
    PWMDRV_ePOL_ACTIVE_LOW    , //Active low/low polarity 
    PWMDRV_ePOL_ACTIVE_HI     , //Active high/high polarity
    PWMDRV_ePOL_FORCED_HI     , //Forced high
    PWMDRV_ePOL_NOT_USED        //Used to note this PWM output is not used
    
}PWMDRV_tePOLARITY;


/*^^^^^^^^^^^^^^^
DEAD BAND OPTIONS
^^^^^^^^^^^^^^^*/
//Dead band timer enable
typedef enum PWMDRV_eDB_TMR
{
    PWMDRV_eDB_TMR_DIS = 0 ,   //Dead band Timer disabled
    PWMDRV_eDB_TMR_EN          //Dead band Timer enabled
    
}PWMDRV_teDB_TMR;


/*==============================================================================
                              Structures
==============================================================================*/
//PWM data structure.
//Contains various options that must be set before calling the Ctor. The Ctor
//will returnn an error if the structure is not initialized properly.
typedef struct PWMDRV_zDATA
{
    //The following are set at construction time by the user.
    //The PWM ID based on the enum PWMDRV_eID
    tePWM_ID      tePwmId;
    //PWM wf type for referencing
    PWMDRV_teTYPE tePwmType;
    //Desired Prd in Hz
    uint32        ulDesiredFreqHz;
    //Val to write to the PWM
    uint16        uiDutyCycle;
    
    //In order to allow use of this driver with multiple targets, the following
    //structure is assigned, by the user, to point to a structure of target
    //specific options.  The rest is handled in the pwmdrv.c.  These options
    //should be in a file called PwmDrv[TARGET]Opt.h where [TARGET] is replaced
    //with for example: C24x or HCS12128G etc.
    void  *ptzPwmOpt;       
    
}PWMDRV_tzDATA;


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
//-------- Constructor -------
tuiSTATUS PWMDRV_fnCtor( PWMDRV_tzDATA *ptzPwm ); //Users PWM struct
void      PWMDRV_fnDtor( PWMDRV_tzDATA *ptzPwm ); //Users PWM struct

//-------- PWM Enable/Disable --------
tuiSTATUS PWMDRV_fnEn(  PWMDRV_tzDATA *ptzPwm );  //Users PWM struct
tuiSTATUS PWMDRV_fnDis( PWMDRV_tzDATA *ptzPwm );  //PWM outputs become Hi-Z

//---------- Dead Band -------
tuiSTATUS PWMDRV_fnSetDeadBand( PWMDRV_tzDATA *ptzPwm ); //Users PWM struct
tuiSTATUS PWMDRV_fnEnDeadBand(  PWMDRV_tzDATA *ptzPwm ); //Users PWM struct

//-------- Updates the PWM's duty cycle -------
tuiSTATUS PWMDRV_fnUpdateDutyCycle( PWMDRV_tzDATA *ptzPwm );//Users PWM struct

//-------- Updates the options for PWM -------
//This func takes the PWMDRV_tzDATA struct, however it will only use the
//value in the option pointer "ptzPwmOpt".  The values here will be applied
//as needed.  Therefore, the options need to be set/updated before this
//function is called.
//Notes:
//  - Since the Ctor can setup the PWM options, this function is intended 
//    to be called during run-time.
//  - This function can disable the PWM's so they may need to be enabled
//    after a call to this fn.
//  - The dead band is not handled by this fn.  Instead, use the API
//    PWMDRV_fnSetDeadBand().
tuiSTATUS PWMDRV_fnSetOpt( PWMDRV_tzDATA *ptzPwm );  //Users PWM struct

//-------- Gets the maximum value that can be put into the duty register  ----
//-------- based on the current value of the frequency and clock settings ----
uint16 PWMDRV_fnGetMaxDutyCycle( PWMDRV_tzDATA *ptzPwm ); //Users PWM struct

#endif
