/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.
 
This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.


FILE NAME:  
    Capdrv.h

PURPOSE:
    Defines enums and strunctures used by capture driver.
    Declares public functions

FUNCTION(S):
    CAPDRV_fnCtor -creat capture instances
    CAPDRV_fnDtor -destroy capture instances
    CAPDRV_fnStart -Start the capture operation
    CAPDRV_fnStop -Stop the capture operation

    CAPDRV_fnSet -Set capture registers
    CAPDRV_fnGetCnt -get captured timer counter value

    CAPDRV_fnClrIntFlag -clear a interrupt flag
    CAPDRV_fnEnInt -enable a interrupt
    CAPDRV_fnDisInt -disable a interrupt

      
NOTES:

    USAGE:
    ***set compile options for your application first to save code space.

    1. Before using any functions in this cap driver, a cap instance has to 
            be created. 
            Since cap driver is associated with a specific timer, the timer 
            that is used by the cap instance has to be created before hand.
            Variable members within the CAPDRV_tzOBJ structure that has to be 
            initialized before calling timer_fnCtor are teCapFunc, and 
            *ptzTimer.
        Put a "DEFAULT" initialization to CAPDRV_tzOBJ structure when define a struc
            to avoid unprodictable situations.
        cap_fnCtor provides an ID to ucCapID in the CAPDRV_tzOBJ. 

    2. Define the variable members properly: 
                        teCapAdcMode;           //ADC start mode
                        teCAP_EDGE teCapEdge;   //capture edge

        fnInit initialize cap registers according to cap mode settings, 
            however, fnStart will initial registers before start the cap unit.
        Call fnInit if you want to initialize cap register before start.
     
    3. Remember to start the associated timer as well to let the cap unit work
    
    4. To enable cap interrupt, call fnEnInt. To disable cap interrupt, call
        fnDisInt.
        Global interrupt has to be enabled seperatly to get interrupt.
        Before return from cap interrupt routine, call fnClrIntFlag to clear 
        cap interrupt flag.

    5. Call fnStop if you want to stop the cap operation.
         
    6. getCnt will update on uiCapCnt, and uiCapFreq if possible.
        teCapReady shows the Readiness of the two values.
                  
    7. To setup and run a timer, consider following example:

    TMRDRV_tzTIMER tzTimerGp = TIMER_DEFAULT;
    CAPDRV_tzOBJ   tzCap = CAPDRV_DEFAULT;

    void my_main( void )
    {
        ........

        //set and start associated timer
        tzTimerGp.teTimeFunc = eTIMER_GP;
        tuiStatus = timer_fnCtor( &tzTimerGp );
        tzTimerGp.teTimerCountMode = eTIMER_CONTUP;
        tzTimerGp.ulTimerFreq = 30;

        tuiStatus = timer_fnStart( &tzTimerGp );

        //set and enable interrupt of a cap unit
        tzCap.teCapFunc = CAPDRV_eZERO_X;
        tzCap.ptzTimer = &tzTimerGp;
        tzCap.teCapEdge = CAPDRV_eRISE_FALL;

        tuiStatus = cap_fnCtor( &tzCap );
        tuiStatus = cap_fnEnInt( &tzCap );

        //enable global interrupt
        mGLOBAL_IRT_EN();
        
        //start capture
        tuiStatus = cap_fnStart( &tzCap );
        .......

    }
    void my_Cap_Int( void )
    {
        ........
        tuiStatus = cap_fnGetCnt( &tzCap );

        tuiStatus = timer_fnClrIntFlag( &tzCap );
    }

    
CHANGE HISTORY:
$Log: Capdrv.h $

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/1
    User: HorstM     Date: 02/21/05  Time: 12:42AM
    changes made to timer driver interface (function name)
    

    ***********************************************
    Revision: NovaPfrmB_HongY/2
    User: HongY     Date: 01/20/05  Time: 09:05PM
    Changed according to code review and code standard
    
    
    
    *****************  Version 1  *****************
    User: Hongy        Date: 6/12/04    Time: 1:42p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Inc
    create header file for capture driver
    
==============================================================================*/

#ifndef CAPDRV_INCL
#define CAPDRV_INCL

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    //Requires an Architecture bit to be defined.
#include "LIB_stddefs.h"    //Standard definitions.
#include "HALErrBase.h"     //For return type error codes (using errno)
#include "Tmrdrv.h"            //include timer driver

/*==============================================================================
                              Defines
==============================================================================*/
//compile options
#define CAPDRV_ADC_EN      ( FALSE )   //false-cap do not have ADC trigger feature
                                    //true-cap can have ADC trigger feature
//define edge 
#define CAPDRV_FALL_EDGE   ( 0 )       //io pin is low if fall edge is captured
/*==============================================================================
                            Type Definitions
==============================================================================*/

/*=============================================================================
                                Enums
==============================================================================*/
//errno codes based on errbase.h
typedef enum CAPDRV_eERRNO
{
    CAPDRV_eERRNO_SOMETHING = ERR_CAP_DRV, //Start of errno's for the cap.
    CAPDRV_eERRNO_CAP_NO_DEF,              //CAP ID is not a valid number
                                           // create instance by fnCtor
    CAPDRV_eERRNO_CAP_NO_FN,               //CAP function not defined
                                           //give a eTIMER_FUNC to teTimeFunc 
    CAPDRV_eERRNO_CAP_INVALID,             //cap for this function has been used 
    CAPDRV_eERRNO_TIMER_NOT_MATCH,         //CAP associated timer is not defined
                                           //properly, change to another timer
    CAPDRV_eERRNO_CAP_ADC_MODE,            //ADC mode setting is inproper
    CAPDRV_eERRNO_NO_CAPEVENT,             //no capture event happend
    CAPDRV_eERRNO_FREQ_CALC_ERROR,         //error during frquency calculation
    CAPDRV_eERRNO_TIMER_COUNT_NOT_MATCH,   //CAP associated timer's count mode
                                           // has to be continous up
    CAPDRV_eERRNO_MAX = ERR_CAP_DRV_MAX    //Max val not to be used.

}CAPDRV_teERRNO ;

//capture start ADC mode
#if( CAPDRV_ADC_EN == TRUE )
typedef enum CAPDRV_eADC_MODE    //CAP can start ADC mode
{
    CAPDRV_eADC_SEPERATE = 0,      //no ADC is triggered by timer
    CAPDRV_eADC_TIED               //ADC triggered by capture
}CAPDRV_teADC_MODE;
#endif

//capture edge 
typedef enum CAPDRV_eEDGE        //capture edge
{
    CAPDRV_eNO_DETECT = 0,         //no detect
    CAPDRV_eRISE,                  //detects rising edge
    CAPDRV_eFALL,                  //detects falling edge
    CAPDRV_eRISE_FALL              //detects rising and falling edges
}CAPDRV_teEDGE;

//capture function
typedef enum CAPDRV_eFN       //capture function enum
{
    CAPDRV_eFN_INVALID = 0,
    CAPDRV_eFN1,
    CAPDRV_eFN2,               //these CAP function is not been used   
    CAPDRV_eFN3,               //maximum 6 CAPs can be defined for now
    CAPDRV_eFN4,
    CAPDRV_eFN5,               //detect AC input zero-crossing for line 1
    CAPDRV_eFN6                //detect AC input zero-crossing for line 2
}CAPDRV_teFN;

//capture data condition
typedef enum CAPDRV_eDATA_READY      //capture data validation
{
    CAPDRV_eDATA_INVALID = 0,        // uiCapCnt is invalid
    CAPDRV_eDATA_OVRW,               // capture overflow
    CAPDRV_eCNT_VALID                // uiCapCnt is valid 
}CAPDRV_teDATA_READY;

/*==============================================================================
                              Structures
==============================================================================*/
//capture instance structure
typedef struct CAPDRV_zOBJ
{
    uchar8 ucCapID;                 //capture ID, assigned by 'fnCtor'
    TMRDRV_tzTIMER *ptzTimer;       //timer structure pointer, points to 
                                    //the timer that associate with
                                    //this capture instance
#if( CAPDRV_ADC_EN == TRUE )
    CAPDRV_teADC_MODE teCapAdcMode; //ADC start mode
#endif
    CAPDRV_teEDGE teCapEdge;        //capture edge
    CAPDRV_teFN teCapFunc;          //capture function, has to defined 
                                    //before 'fnCtor' is called
    uint16 uiCapCnt;                //captured timer counter value, init to 0, 
                                    //updated by calling fnGetCnt
    uint16 uiRiseFall;              //fall( CAPDRV_FALL_EDGE) edge or rise edge
    CAPDRV_teDATA_READY teCapReady; //updated by calling fnGetCnt
}CAPDRV_tzOBJ;

/*==============================================================================
                             Constants
==============================================================================*/
                        
/*==============================================================================
                               Variables
==============================================================================*/

/*==============================================================================
                            Function Protoypes
==============================================================================*/
//create capture instances, assign a capture unit to ucCapID according to the 
//function defined, pteTimer points to the associated timer 
tuiSTATUS CAPDRV_fnCtor( CAPDRV_tzOBJ *ptzCap ); 
 
//destroy capture instances, free a capture from this function use, set 
//pteTimer to 0 
tuiSTATUS CAPDRV_fnDtor( CAPDRV_tzOBJ *ptzCap ); 

//Start the capture, it update with all capture
//registers with current instance and enable the capture, timer associated with
//has to start seperately
tuiSTATUS CAPDRV_fnStart( CAPDRV_tzOBJ *ptzCap );  

//Stop the capture, timer associated with this has to stop seperatly 
tuiSTATUS CAPDRV_fnStop( CAPDRV_tzOBJ *ptzCap ); 
 

//Init the unit registers according to user setting 
tuiSTATUS CAPDRV_fnSet( CAPDRV_tzOBJ *ptzCap ); 

//get captured timer counter value
tuiSTATUS CAPDRV_fnGetCnt( CAPDRV_tzOBJ *ptzCap ); 

//clear a interrupt flag, this function has to be called at the end of each 
//capture interrupt service routine
tuiSTATUS CAPDRV_fnClrIntFlag( CAPDRV_tzOBJ *ptzCap ); 

//enable a interrupt, this function has to be called to enable the 
//capture interrupt
tuiSTATUS CAPDRV_fnEnInt( CAPDRV_tzOBJ *ptzCap ); 

//disable a interrupt, this function has to be called to disable the 
//capture interrupt
tuiSTATUS CAPDRV_fnDisInt( CAPDRV_tzOBJ *ptzCap ); 

#endif    // CAPDRV_INCL

