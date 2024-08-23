/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.
 
This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.


FILE NAME:  
    capdrv.c

PURPOSE:
    provide all functions needed to operate Capture Driver: constructor,
    destructor, start, stop, set, get and interrupt enable and disable.

FUNCTION(S):
    CAPDRV_fnCtor( CAPDRV_tzOBJ * )-creat capture instances
    CAPDRV_fnDtor( CAPDRV_tzOBJ * )-destroy capture instances
    CAPDRV_fnStart( CAPDRV_tzOBJ * )-Start the capture operation
    CAPDRV_fnStop( CAPDRV_tzOBJ * )-Stop the capture operation

    CAPDRV_fnSet -Set capture registers
    CAPDRV_fnGetCnt( CAPDRV_tzOBJ * )-get captured timer counter value

    CAPDRV_fnClrIntFlag( CAPDRV_tzOBJ * )-clear a interrupt flag
    CAPDRV_fnEnInt( CAPDRV_tzOBJ * )-enable a interrupt
    CAPDRV_fnDisInt( CAPDRV_tzOBJ * )-disable a interrupt
   
    local:
        none
        
NOTES:
    none

CHANGE HISTORY:
$Log: Capdrv.c $

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/1
    User: HorstM     Date: 02/21/05  Time: 12:43AM
    changes made to timer driver interface (function names), and timer struct member
    names
    

    ***********************************************
    Revision: NovaPfrmB_HongY/4
    User: HongY     Date: 01/27/05  Time: 07:48PM
    Updated. Check if tmrdrv existed before calling TMRDRV_fnCtor().
    
    

    ***********************************************
    Revision: NovaPfrmB_HongY/3
    User: HongY     Date: 01/20/05  Time: 09:12PM
    Changed according to code review and code standard
    
    

    ***********************************************
    Revision: NovaPfrmB_HongY/2
    User: HongY     Date: 01/11/05  Time: 08:18PM
    changed tab to spaces
    

    ***********************************************
    Revision: NovaPfrmB_HongY/1
    User: HongY     Date: 12/07/04  Time: 11:42PM
    changes some variable definitions from static to
    const. Uncommented assert in function CAPDRV_fnGetCnt()
    
    
    *****************  Version 1  *****************
    User: Hongy        Date: 6/12/04    Time: 1:42p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Src
    
==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "Capdrv.h"
#include "device.h"        //register definitions
#include "devicemacro.h"        //register definitions
#include <errno.h>
#include "xassert.h"    

/*==============================================================================
                             Defines
==============================================================================*/
//compile options
#define CAPDRV_CTOR_EN         ( TRUE )    //function enable/disable
#define CAPDRV_START_EN        ( TRUE )    //function enable/disable
#define CAPDRV_SET_EN          ( TRUE )    //function enable/disable
#define CAPDRV_ENINT_EN        ( TRUE )    //function enable/disable
#ifdef GLBL_DTOR_EN 
#define CAPDRV_DISINT_EN       ( TRUE )   //function enable/disable
#else
#define CAPDRV_DISINT_EN       ( FALSE )   //function enable/disable
#endif
#define CAPDRV_CLRINTFLAG_EN   ( TRUE )    //function enable/disable
#ifdef GLBL_DTOR_EN 
#define CAPDRV_STOP_EN         ( TRUE )   //function enable/disable
#else
#define CAPDRV_STOP_EN         ( FALSE )   //function enable/disable
#endif
#define CAPDRV_GETCNT_EN       ( TRUE )    //function enable/disable

//false, capture not enabled, true, enabled
#define CAP1_EN     ( FALSE )          // reserved for Solar Wave  
#define CAP2_EN     ( FALSE )          // reserved for Solar Wave 
#define CAP3_EN     ( FALSE )          // reserved for Solar Wave 
#define CAP4_EN     ( TRUE )           // ZERO_CROSS1
#define CAP5_EN     ( TRUE )           // ZERO_CROSS2
#define CAP6_EN     ( FALSE )           

//CAP status
#define CAPDRV_NOT_USED    ( 0 )
#define CAPDRV_USED        ( 1 )

//Timer status
#define TMRDRV_NOT_USED    ( 0 )
#define TMRDRV_USED        ( 1 )

// Zero Cross
//macros to set io pins for capture operations
#define IOMAP_ZEROX_GEN1_PIN         ( GpioDataRegs.GPADAT.bit.GPIOA8  )
#define IOMAP_ZEROX_GEN1_MUX_REG     ( GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8)
#define IOMAP_ZEROX_GEN2_PIN         ( GpioDataRegs.GPADAT.bit.GPIOA9  )
#define IOMAP_ZEROX_GEN2_MUX_REG     ( GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9)
#define IOMAP_ZEROX_L1L2_PIN         ( GpioDataRegs.GPADAT.bit.GPIOA10  )
#define IOMAP_ZEROX_L1L2_MUX_REG     ( GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10)
#define IOMAP_ZERO_CROSS1_PIN        ( GpioDataRegs.GPBDAT.bit.GPIOB8  )
#define IOMAP_ZERO_CROSS1_MUX_REG    ( GpioMuxRegs.GPBMUX.bit.CAP4Q1_GPIOB8)
#define IOMAP_ZERO_CROSS2_PIN        ( GpioDataRegs.GPBDAT.bit.GPIOB9  )
#define IOMAP_ZERO_CROSS2_MUX_REG    ( GpioMuxRegs.GPBMUX.bit.CAP5Q2_GPIOB9)
#define IOMAP_CAP6_PIN               ( GpioDataRegs.GPBDAT.bit.GPIOB10  )
#define IOMAP_CAP6_REG               ( GpioMuxRegs.GPBMUX.bit.CAP6QI2_GPIOB10)

//macros to set io pins for capture operations
#define mCAPDRV_SET_IO_CAP1()   DEVICE_mACCESS_EN(); \
                                IOMAP_ZEROX_GEN1_MUX_REG = F28X_CAPCONX_CAP_1_2_EN;\
                                DEVICE_mACCESS_DIS()
#define mCAPDRV_SET_IO_CAP2()   DEVICE_mACCESS_EN(); \
                                IOMAP_ZEROX_GEN2_MUX_REG = F28X_CAPCONX_CAP_1_2_EN;\
                                DEVICE_mACCESS_DIS()
#define mCAPDRV_SET_IO_CAP3()   DEVICE_mACCESS_EN(); \
                                IOMAP_ZEROX_L1L2_MUX_REG = F28X_CAPCONX_CAP_3_EN;\
                                DEVICE_mACCESS_DIS()
#define mCAPDRV_SET_IO_CAP4()   DEVICE_mACCESS_EN(); \
                                IOMAP_ZERO_CROSS1_MUX_REG = F28X_CAPCONX_CAP_1_2_EN;\
                                DEVICE_mACCESS_DIS()
#define mCAPDRV_SET_IO_CAP5()   DEVICE_mACCESS_EN(); \
                                IOMAP_ZERO_CROSS2_MUX_REG = F28X_CAPCONX_CAP_1_2_EN;\
                                DEVICE_mACCESS_DIS()
#define mCAPDRV_SET_IO_CAP6()   DEVICE_mACCESS_EN(); \
                                IOMAP_CAP6_REG = F28X_CAPCONX_CAP_3_EN;\
                                DEVICE_mACCESS_DIS()

//CAP FIFO status
#define CAPFIFO_EMPT          ( 0 )
#define CAPFIFO_ONE           ( 1 )
#define CAPFIFO_TWO           ( 2 )
#define CAPFIFO_OVRW          ( 3 )

//CAP RESET status
#define CAP_RESET_EN          ( FALSE )
#define CAP_RESET_DIS         ( TRUE )

//CAP Interrupt status
#define CAP_INT_EN            ( TRUE)
#define CAP_INT_DIS           ( FALSE )
#define CAP_INT_FLAG_RESET    ( TRUE)

#define TIMER_PRD_DFLT        ( 30 )
/*==============================================================================
                            File Variables
==============================================================================*/
//The capture units available to the device.
typedef enum eCAP_ID
{
    eCAP1 = 0,
    eCAP2,
    eCAP3,
    eCAP4,
    eCAP5,
    eCAP6,
    eCAP_TOTAL
}teCAP_ID;

//timer that associate with CAP, should be same sequence of timer id
typedef enum eCAP_TIMER_ID
{
    eCAP_TIMER_1 = 0,
    eCAP_TIMER_2,
    eCAP_TIMER_3,
    eCAP_TIMER_4
}teCAP_TIMER_ID;

//functions that assigned to each capture unit
typedef enum eCAP_FUNCTION_ASSIGNED
{
    eCAP1_FUNCTION = CAPDRV_eFN1,
    eCAP2_FUNCTION = CAPDRV_eFN2,
    eCAP3_FUNCTION = CAPDRV_eFN3,
    eCAP4_FUNCTION = CAPDRV_eFN5,
    eCAP5_FUNCTION = CAPDRV_eFN6,
    eCAP6_FUNCTION = CAPDRV_eFN4
}teCAP_FN_ASSIGNED;


// timer function of the associated timer
const uint16 CapTimeFn[ eCAP_TOTAL ] = 
{
    TMRDRV_eSW,   //CAP1, CAP2 use the same timer
    TMRDRV_eSW,   
    TMRDRV_eSW,
    TMRDRV_eCAP,     //CAP4, CAP5 use the same timer
    TMRDRV_eCAP,
    TMRDRV_eCAP
};          

// variables to store usage status of each capture unit
static uchar8 CapStatus[ eCAP_TOTAL ] = 
{ 
    CAPDRV_NOT_USED,
    CAPDRV_NOT_USED,  
    CAPDRV_NOT_USED,  
    CAPDRV_NOT_USED,  
    CAPDRV_NOT_USED,  
    CAPDRV_NOT_USED  
};

//timer selection ID for captures
const teCAP_TIMER_ID CapTimeSel[ eCAP_TOTAL ] =
{   
    eCAP_TIMER_2,       //cap1
    eCAP_TIMER_2,       //cap2
    eCAP_TIMER_2,       //cap3
    eCAP_TIMER_4,       //cap4
    eCAP_TIMER_4,       //cap5
    eCAP_TIMER_4        //cap6
};

//CapDrv used timer instance
TMRDRV_tzTIMER tzZeroXTimer;
TMRDRV_tzPRD   tzZeroXTimerPrd;

/*==============================================================================
                       Local Function Protoypes
==============================================================================*/

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    CAPDRV_fnCtor
  
PURPOSE: 
    create capture instances, initial capture variable ucCapID, according to 
    function defined if the timer is available and the capture unit is
    available
    init cap unit to the timer defined

INPUTS: 
    a pointer to a CAPDRV_tzOBJ structure that need to be created

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-28-2002  By: Yingran Duan
    - create

Version: 0.11  Date: FEB-20-2003  By: Jason Cumiskey
    - Updated to conform to latest coding standard

Version: 1.00  Date: April 14, 2003  By: John Bellini
    - Updated timer driver API calls from timer_ to TIMER_

Version: 1.10  Date: May 12, 2003  By: Yingran Duan
    - Some optimazation, assert instead of tuiStatus
Version: 1.20  Date: May 22, 2003  By: Yingran Duan
    - Some optimazation based on code review, use const to save setting for
        different cap channels
Version: 1.30  Date: Nov 24, 2004  By: Hong Yin
    - Ported from HCS12, changed IO map and operated IO pins
Version: 1.40  Date: Jan 18, 2005  By: Hong Yin
    - move the timer initialization to CAPDRV_fnCtor.
Version: 1.50  Date: Jan 27, 2005  By: Hong Yin
    - Check if the timer driver is already initialized before calling
      TMRDRV_fnCtor().

*******************************************************************************/
#if( CAPDRV_CTOR_EN == TRUE )
tuiSTATUS CAPDRV_fnCtor( CAPDRV_tzOBJ *ptzCap)
{
    static tucBOOL TimerStatus = TMRDRV_NOT_USED;

    //check input
    assert( ptzCap );

    //Enables the hight-speed clock(HSPCLK) within the EV_A and EV_B
    //peripheral.
    DEVICE_mACCESS_EN();
    SysCtrlRegs.PCLKCR.bit.EVAENCLK = F28X_PCLKCR_CLK_ON;
    SysCtrlRegs.PCLKCR.bit.EVBENCLK = F28X_PCLKCR_CLK_ON;
    DEVICE_mACCESS_DIS();

    if ( TimerStatus == TMRDRV_NOT_USED )
    {
        //create timer for CAP instance 
        tzZeroXTimer.teTimeFunc = TMRDRV_eCAP;
        if( TMRDRV_fnCtor( &tzZeroXTimer ) == eSTATUS_ERR )
        {
            assert( 0 );
            return( eSTATUS_ERR );
        }

        tzZeroXTimer.teTimerCountMode  = TMRDRV_eCONTUP;
        tzZeroXTimer.ulTimerFreq     = TIMER_PRD_DFLT;
        tzZeroXTimer.teTimerInt        = TMRDRV_eINT_PRD;
        tzZeroXTimer.teTimerAdcMode    = TMRDRV_eADC_SEPERATE;
        tzZeroXTimer.teTimerCompOutput = TMRDRV_eNO_OUTPUT;
        tzZeroXTimer.teTimerEmu        = TMRDRV_eEMU_STOP_IMM;
        tzZeroXTimer.teTimerCompMode   = TMRDRV_eNO_COMP;
        tzZeroXTimer.teTimerClock      = TMRDRV_eINTERNAL;
        tzZeroXTimer.teTimerSyn        = TMRDRV_eNO_SYN;
        if( TMRDRV_fnStart( &tzZeroXTimer ) == eSTATUS_ERR )
        {
            assert( 0 );
            return( eSTATUS_ERR );
        }

        //get timer period information
        if( TMRDRV_fnGetPrd( &tzZeroXTimer, &tzZeroXTimerPrd ) == eSTATUS_ERR )
        {
            assert( 0 );
            return( eSTATUS_ERR );
        }
        TimerStatus = TMRDRV_USED;
     }

    ptzCap->ptzTimer = &tzZeroXTimer;

    //check timer that associate to this cap is created
    //check timer counter mode setting 
    //use assert to replace
    //mWRITE_STATUS_AND_ERRNO( CAPDRV_eERRNO_TIMER_COUNT_NOT_MATCH );
    if( TMRDRV_fnGetStatus( ptzCap->ptzTimer ) == eSTATUS_ERR ||
        ptzCap->ptzTimer->teTimerCountMode != TMRDRV_eCONTUP )
    {
        //Assert here because this function is internal to this function but
        //it uses another packages function.  So, if there is something wrong
        //It will happen at development time, NOT run time.
        assert( 0 );
        return eSTATUS_ERR;
    }

    //check capture unit function 
    switch( ptzCap->teCapFunc )
    {
#if( CAP1_EN == TRUE )
        //function to assigned to Capture1
        case( eCAP1_FUNCTION ):
            ptzCap->ucCapID = eCAP1;
            //io pin configuration
            mCAPDRV_SET_IO_CAP1();
            break;
#endif

#if( CAP2_EN == TRUE )
        //function to assigned to Capture1
        case( eCAP2_FUNCTION ):
            ptzCap->ucCapID = eCAP2;
            //io pin configuration
            mCAPDRV_SET_IO_CAP2();
            break;
#endif
            
#if( CAP3_EN == TRUE )
        //function to assigned to Capture1
        case( eCAP3_FUNCTION ):
            ptzCap->ucCapID = eCAP3;
            //io pin configuration
            mCAPDRV_SET_IO_CAP3();
            break;
#endif

#if( CAP4_EN == TRUE )
        //function to assigned to Capture1
        case( eCAP4_FUNCTION ):
            ptzCap->ucCapID = eCAP4;
            //io pin configuration
            mCAPDRV_SET_IO_CAP4();
            break;
#endif

#if( CAP5_EN == TRUE )
        //function to assigned to Capture1
        case( eCAP5_FUNCTION ):
            ptzCap->ucCapID = eCAP5;
            //io pin configuration
            mCAPDRV_SET_IO_CAP5();
            break;
#endif

#if( CAP6_EN == TRUE )
        //function to assigned to Capture1
        case( eCAP6_FUNCTION ):
            ptzCap->ucCapID = eCAP6;
            //io pin configuration
            mCAPDRV_SET_IO_CAP6();
            break;
#endif

        default:
            assert( 0 );
            break;
            
    }
                    
    //check cap availibility, and init capID if avaliable
    assert( ( CapStatus[ ptzCap->ucCapID ] != CAPDRV_USED ) && 
            ( ptzCap->ptzTimer->teTimeFunc == CapTimeFn[ ptzCap->ucCapID ] ) );

    //set timer associated
    switch (ptzCap->ucCapID )
    {
        case eCAP1:
        case eCAP2:
            if( ptzCap->ptzTimer->ucTimerID == CapTimeSel[ ptzCap->ucCapID ] )
            {
                EvaRegs.CAPCONA.bit.CAP12TSEL = 0;     //Select GP timer 2
            }
            else
            {
                EvaRegs.CAPCONA.bit.CAP12TSEL = 1;    //Select GP timer 1
            }
            break;
#if( CAPDRV_ADC_EN == TRUE )
        case eCAP3:
            if( ptzCap->ptzTimer->ucTimerID == CapTimeSel[ ptzCap->ucCapID ] )
            {
                EvaRegs.CAPCONA.bit.CAP3TSEL = 0;     //Select GP timer 2
            }
            else
            {
                EvaRegs.CAPCONA.bit.CAP3TSEL = 1;     //Select GP timer 1
            }
            break;
#endif
        case eCAP4:
        case eCAP5:
            if( ptzCap->ptzTimer->ucTimerID == CapTimeSel[ ptzCap->ucCapID ] )
            {
                EvbRegs.CAPCONB.bit.CAP45TSEL = 0;  //Select GP timer 4
            }
            else
            {
                EvbRegs.CAPCONB.bit.CAP45TSEL = 1;   //Select GP timer 3
            }
            break;
#if( CAPDRV_ADC_EN == TRUE )
        case eCAP6:
            if( ptzCap->ptzTimer->ucTimerID == CapTimeSel[ ptzCap->ucCapID ] )
            {
                EvbRegs.CAPCONB.bit.CAP6TSEL = 0     //Select GP timer 4
            }
            else
            {
                EvbRegs.CAPCONB.bit.CAP6TSEL = 1     //Select GP timer 3
            }
            break;
#endif
        default:
            assert( 0 );
            break;
        
    }

    //set this cap is in use
    CapStatus[ ptzCap->ucCapID ] = CAPDRV_USED;

    return( eSTATUS_OK );

}//end of CAPDRV_fnCtor
#endif

/*******************************************************************************

FUNCTION NAME: 
    CAPDRV_fnDtor
  
PURPOSE: 
    destroy capture instances, stop a capture and clear the capture assignment

INPUTS: 
    a pointer to a CAPDRV_tzOBJ structure that need to destroy

OUTPUTS:
    tuiSTATUS

NOTES:
    None

VERSION HISTORY:

Version: 0.10  Date: Nov-29-2002  By: Yingran Duan
    - create

Version: 0.11  Date: FEB-20-2003  By: Jason Cumiskey
    - Updated to conform to latest coding standard

Version: 1.00  Date: May 12, 2003  By: Yingran Duan
    - Some optimazation, assert instead of tuiStatus
Version: 1.10  Date: May 22, 2003  By: Yingran Duan
    - Some optimazation based on code review, use const to save setting for
        different cap channels

*******************************************************************************/
#ifdef GLBL_DTOR_EN 
tuiSTATUS CAPDRV_fnDtor( CAPDRV_tzOBJ *ptzCap )
{
    
    //check input
    assert( ptzCap );

    //disable interrupt 
    //stop capture 
    if( CAPDRV_fnDisInt( ptzCap ) == eSTATUS_ERR || 
        CAPDRV_fnStop( ptzCap ) == eSTATUS_ERR  )
    {
        //Assert here because this function is internal to this function but
        //it uses another packages function.  So, if there is something wrong
        //It will happen at development time, NOT run time.
        assert( 0 );
    }

    //ucCapID should be valid    
    CapStatus[ ptzCap->ucCapID ] = CAPDRV_NOT_USED;

    return( eSTATUS_OK );
   
}//end of CAPDRV_fnDtor
#endif
/*******************************************************************************

FUNCTION NAME: 
    CAPDRV_fnSet
  
PURPOSE: 
    initial control register of a capture specified, ADC mode and edge that 
    need to detected

INPUTS: 
    a pointer to a CAPDRV_tzOBJ structure that need to be initialized

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10    Date: Nov-29-2002  By: Yingran Duan
    - create

Version: 0.11  Date: FEB-20-2003  By: Jason Cumiskey
    - Updated to conform to latest coding standard
Version: 1.00  Date: May 22, 2003  By: Yingran Duan
    - Some optimazation based on code review, use const to save setting for
        different cap channels
    - change name from init to set
Version: 1.20  Date: Nov 24, 2004  By: Hong Yin
    - Ported from HCS12, changed IO map and operated IO pins


*******************************************************************************/
#if( CAPDRV_SET_EN == TRUE )
tuiSTATUS CAPDRV_fnSet( CAPDRV_tzOBJ *ptzCap)
{

    //check input
    assert( ptzCap );
    
    //get control register
    //set detect edge
    switch (ptzCap->ucCapID )
    {
        case eCAP1:
            EvaRegs.CAPCONA.bit.CAP1EDGE =  ptzCap->teCapEdge;
            EvaRegs.CAPCONA.bit.CAPRES = CAP_RESET_DIS;
            break;
        case eCAP2:
            EvaRegs.CAPCONA.bit.CAP2EDGE =  ptzCap->teCapEdge;
            EvaRegs.CAPCONA.bit.CAPRES = CAP_RESET_DIS;
            break;
#if( CAPDRV_ADC_EN == TRUE )
        case eCAP3:
            EvaRegs.CAPCONA.bit.CAP3TOADC =  ptzCap->teCapAdcMode;
            EvaRegs.CAPCONA.bit.CAPRES = CAP_RESET_DIS;
            break;
#endif
        case eCAP4:
            EvbRegs.CAPCONB.bit.CAP4EDGE =  ptzCap->teCapEdge;
            EvbRegs.CAPCONB.bit.CAPRES = CAP_RESET_DIS;
            break;
        case eCAP5:
            EvbRegs.CAPCONB.bit.CAP5EDGE =  ptzCap->teCapEdge;
            EvbRegs.CAPCONB.bit.CAPRES = CAP_RESET_DIS;
            break;
#if( CAPDRV_ADC_EN == TRUE )
        case eCAP6:
            EvbRegs.CAPCONB.bit.CAP6TOADC =  ptzCap->teCapAdcMode;
            EvbRegs.CAPCONB.bit.CAPRES = CAP_RESET_DIS;
            break;
#endif
        default:
            assert( 0 );
            break;

    }

    return( eSTATUS_OK );
    
}//end of CAPDRV_fnSet
#endif

/*******************************************************************************

FUNCTION NAME: 
    CAPDRV_fnStart
  
PURPOSE: 
    start a cap specified,  call cap_fnSet before start

INPUTS: 
    a pointer to a CAPDRV_tzOBJ structure that need to be start

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-29-2002  By: Yingran Duan
    - create

Version: 0.11  Date: FEB-20-2003  By: Jason Cumiskey
    - Updated to conform to latest coding standard

Version: 1.00  Date: May 12, 2003  By: Yingran Duan
    - Some optimazation, assert instead of tuiStatus
Version: 1.10  Date: May 22, 2003  By: Yingran Duan
    - Some optimazation based on code review, use const to save setting for
        different cap channels
Version: 1.20  Date: Nov 24, 2004  By: Hong Yin
    - Ported from HCS12, changed IO map and operated IO pins

*******************************************************************************/
#if( CAPDRV_START_EN == TRUE )
tuiSTATUS CAPDRV_fnStart( CAPDRV_tzOBJ *ptzCap )
{
    
    //check input
    assert( ptzCap );

    //update capture control registers
    ( void )CAPDRV_fnSet( ptzCap ); 
    
    //ucCapID should be valid
    //enable capture     
    //clear FIFO status register
    switch (ptzCap->ucCapID )
    {
        case eCAP1:
            EvaRegs.CAPCONA.bit.CAPRES = CAP_RESET_DIS;
            EvaRegs.CAPFIFOA.bit.CAP1FIFO = CAPFIFO_EMPT;
            EvaRegs.CAPCONA.bit.CAP12EN =  F28X_CAPCONX_CAP_1_2_EN;
            break;
        case eCAP2:
            EvaRegs.CAPCONA.bit.CAPRES = CAP_RESET_DIS;
            EvaRegs.CAPFIFOA.bit.CAP2FIFO = CAPFIFO_EMPT;
            EvaRegs.CAPCONA.bit.CAP12EN =  F28X_CAPCONX_CAP_1_2_EN;
            break;
        case eCAP3:
            EvaRegs.CAPCONA.bit.CAPRES = CAP_RESET_DIS;
            EvaRegs.CAPFIFOA.bit.CAP3FIFO = CAPFIFO_EMPT;
            EvaRegs.CAPCONA.bit.CAP3EN =  F28X_CAPCONX_CAP_3_EN;
            break;
        case eCAP4:
            EvbRegs.CAPCONB.bit.CAPRES = CAP_RESET_DIS;
            EvbRegs.CAPFIFOB.bit.CAP4FIFO = CAPFIFO_EMPT;
            EvbRegs.CAPCONB.bit.CAP45EN =  F28X_CAPCONX_CAP_1_2_EN;
            break;
        case eCAP5:
            EvbRegs.CAPCONB.bit.CAPRES = CAP_RESET_DIS;
            EvbRegs.CAPFIFOB.bit.CAP5FIFO = CAPFIFO_EMPT;
            EvbRegs.CAPCONB.bit.CAP45EN =  F28X_CAPCONX_CAP_1_2_EN;
            break;
        case eCAP6:
            EvbRegs.CAPCONB.bit.CAPRES = CAP_RESET_DIS;
            EvbRegs.CAPFIFOB.bit.CAP6FIFO = CAPFIFO_EMPT;
            EvbRegs.CAPCONB.bit.CAP6EN =  F28X_CAPCONX_CAP_3_EN;
            break;
        default:            
            assert( 0 );
            break;
        
    }

    return( eSTATUS_OK );
   
}//end of CAPDRV_fnStart
#endif

/*******************************************************************************

FUNCTION NAME: 
    CAPDRV_fnEnInt
  
PURPOSE: 
    enable a cap interrupt specified, set status of that cap to 01-one entry
    already to make the next edge capture triggers interrupt
    enable corresponding int mask bits as well

INPUTS: 
    a pointer to a CAPDRV_tzOBJ structure that interrupt need to be enable

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-29-2002  By: Yingran Duan
    - create

Version: 0.11  Date: FEB-20-2003  By: Jason Cumiskey
    - Updated to conform to latest coding standard

Version: 1.00  Date: May 12, 2003  By: Yingran Duan
    - Some optimazation, assert instead of tuiStatus
Version: 1.10  Date: May 22, 2003  By: Yingran Duan
    - Some optimazation based on code review, use const to save setting for
        different cap channels
Version: 1.20  Date: Nov 24, 2004  By: Hong Yin
    - Ported from HCS12, changed IO map and operated IO pins
Version: 1.40  Date: Jan 18, 2005  By: Hong Yin
    - Setting PIE register in order to simply the implementation.

*******************************************************************************/
#if( CAPDRV_ENINT_EN == TRUE )
tuiSTATUS CAPDRV_fnEnInt( CAPDRV_tzOBJ *ptzCap )
{
    //check input
    assert( ptzCap );

    //clear int flag
    ( void )CAPDRV_fnClrIntFlag( ptzCap ); 
    
    PieCtrlRegs.PIECRTL.bit.ENPIE = F28X_PIECTRL_ENPIE_TRUE;  // Enable the PIE block

    //ucCapID should be valid
    //enable interrupt
    switch (ptzCap->ucCapID )
    {
        case eCAP1:
            EvaRegs.EVAIMRC.bit.CAP1INT =  CAP_INT_EN;
            PieCtrlRegs.PIEACK.bit.ACK3 = TRUE;          
            PieCtrlRegs.PIEIER3.bit.INTx5 = TRUE;
            DEVICE_mINT3_EN();
            break;
        case eCAP2:
            EvaRegs.EVAIMRC.bit.CAP2INT =  CAP_INT_EN;
            PieCtrlRegs.PIEACK.bit.ACK3 = TRUE;          
            PieCtrlRegs.PIEIER3.bit.INTx6 = TRUE;
            DEVICE_mINT3_EN();
            break;
        case eCAP3:
            EvaRegs.EVAIMRC.bit.CAP3INT =  CAP_INT_EN;
            PieCtrlRegs.PIEACK.bit.ACK3 = TRUE;          
            PieCtrlRegs.PIEIER3.bit.INTx7 = TRUE;
            DEVICE_mINT3_EN();
            break;
        case eCAP4:
            EvbRegs.EVBIMRC.bit.CAP4INT =  CAP_INT_EN;
            PieCtrlRegs.PIEACK.bit.ACK5 = TRUE;          
            PieCtrlRegs.PIEIER5.bit.INTx5 = TRUE;
            DEVICE_mINT5_EN();
            break;
        case eCAP5:
            EvbRegs.EVBIMRC.bit.CAP5INT =  CAP_INT_EN;
            PieCtrlRegs.PIEACK.bit.ACK5 = TRUE;          
            PieCtrlRegs.PIEIER5.bit.INTx6 = TRUE;
            DEVICE_mINT5_EN();
            break;
        case eCAP6:
            EvbRegs.EVBIMRC.bit.CAP6INT =  CAP_INT_EN;
            PieCtrlRegs.PIEACK.bit.ACK5 = TRUE;          
            PieCtrlRegs.PIEIER5.bit.INTx7 = TRUE;
            DEVICE_mINT5_EN();
            break;
        default:    
            assert( 0 );
            break;

    }    

    return( eSTATUS_OK );
    
}//end of CAPDRV_fnEnInt
#endif

/*******************************************************************************

FUNCTION NAME: 
    CAPDRV_fnDisInt
  
PURPOSE: 
    diable a capture interrupt specified

INPUTS: 
    a pointer to a CAPDRV_tzOBJ structure that need to be disabled

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-29-2002  By: Yingran Duan
    - create

Version: 0.11  Date: FEB-20-2003  By: Jason Cumiskey
    - Updated to conform to latest coding standard
Version: 1.10  Date: May 22, 2003  By: Yingran Duan
    - Some optimazation based on code review, use const to save setting for
        different cap channels
Version: 1.20  Date: Nov 24, 2004  By: Hong Yin
    - Ported from HCS12, changed IO map and operated IO pins

*******************************************************************************/
#if( CAPDRV_DISINT_EN == TRUE )
tuiSTATUS CAPDRV_fnDisInt( CAPDRV_tzOBJ *ptzCap )
{

    //check input
    assert( ptzCap );

    //clear int flag
    ( void )CAPDRV_fnClrIntFlag( ptzCap ); 
    
    //disable capture interrupt
    switch (ptzCap->ucCapID )
    {
        case eCAP1:
            EvaRegs.EVAIMRC.bit.CAP1INT =  CAP_INT_DIS;
            break;
        case eCAP2:
            EvaRegs.EVAIMRC.bit.CAP2INT =  CAP_INT_DIS;
            break;
        case eCAP3:
            EvaRegs.EVAIMRC.bit.CAP3INT =  CAP_INT_DIS;
            break;
        case eCAP4:
            EvbRegs.EVBIMRC.bit.CAP4INT =  CAP_INT_DIS;
            break;
        case eCAP5:
            EvbRegs.EVBIMRC.bit.CAP5INT =  CAP_INT_DIS;
            break;
        case eCAP6:
            EvbRegs.EVBIMRC.bit.CAP6INT =  CAP_INT_DIS;
            break;
        default:    
            assert( 0 );
            break;

    }  
    
    return( eSTATUS_OK );
    
}//end of CAPDRV_fnDisInt
#endif

/*******************************************************************************

FUNCTION NAME: 
    CAPDRV_fnClrIntFlag
    
PURPOSE: 
    clear a specified capture interrupt flag

INPUTS: 
    a pointer to a TMRDRV_tzTIMER structure that flag need to be cleared

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-1-2002  By: Yingran Duan
    - create

Version: 0.11  Date: FEB-20-2003  By: Jason Cumiskey
    - Updated to conform to latest coding standard
Version: 1.00  Date: May 22, 2003  By: Yingran Duan
    - Some optimazation based on code review, use const to save setting for
        different cap channels
Version: 1.20  Date: Nov 24, 2004  By: Hong Yin
    - Ported from HCS12, changed IO map and operated IO pins
Version: 1.30  Date: Jan 18, 2005  By: Hong Yin
    - Clear PIE ACK in order to simply the implementation.

*******************************************************************************/
#if( CAPDRV_CLRINTFLAG_EN == TRUE )
tuiSTATUS CAPDRV_fnClrIntFlag( CAPDRV_tzOBJ *ptzCap )
{

    //check input
    assert( ptzCap );
    
    //check id validation
    assert( ( ptzCap->ucCapID < eCAP_TOTAL ) &&
            ( CapStatus[ ptzCap->ucCapID ] != CAPDRV_NOT_USED ) );
    
    //clear capture interrupt flag
    switch (ptzCap->ucCapID )
    {
        case eCAP1:
            EvaRegs.EVAIFRC.bit.CAP1INT =  CAP_INT_FLAG_RESET;
            PieCtrlRegs.PIEACK.bit.ACK3 = TRUE;          
            break;
        case eCAP2:
            EvaRegs.EVAIFRC.bit.CAP2INT =  CAP_INT_FLAG_RESET;
            PieCtrlRegs.PIEACK.bit.ACK3 = TRUE;          
            break;
        case eCAP3:
            EvaRegs.EVAIFRC.bit.CAP3INT =  CAP_INT_FLAG_RESET;
            PieCtrlRegs.PIEACK.bit.ACK3 = TRUE;          
            break;
        case eCAP4:
            EvbRegs.EVBIFRC.bit.CAP4INT =  CAP_INT_FLAG_RESET;
            PieCtrlRegs.PIEACK.bit.ACK5 = TRUE;          
            break;
        case eCAP5:
            EvbRegs.EVBIFRC.bit.CAP5INT =  CAP_INT_FLAG_RESET;
            PieCtrlRegs.PIEACK.bit.ACK5 = TRUE;          
            break;
        case eCAP6:
            EvbRegs.EVBIFRC.bit.CAP6INT =  CAP_INT_FLAG_RESET;
            PieCtrlRegs.PIEACK.bit.ACK5 = TRUE;          
            break;
        default:    
            assert( 0 );
            break;

    }   
    return( eSTATUS_OK );
    
}//end of CAPDRV_fnClrIntFlag
#endif

/*******************************************************************************

FUNCTION NAME: 
    CAPDRV_fnStop
  
PURPOSE: 
    stop a capture specified, disable cap if cap 3 and cap 6
    set capture edge to none, if other cap units, since
    cap1 and cap2, cap4 and cap5 can not disable individually

INPUTS: 
    a pointer to a CAPDRV_tzOBJ structure that need to be stop

OUTPUTS:
    tuiSTATUS

NOTES:
    cap1,cap2 and cap4, 5 are tied together, disable one of them will
    disable two cap 

VERSION HISTORY:

Version: 0.10  Date: Nov-29-2002  By: Yingran Duan
    - create

Version: 0.11  Date: FEB-20-2003  By: Jason Cumiskey
    - Updated to conform to latest coding standard
Version: 1.00  Date: May 22, 2003  By: Yingran Duan
    - Some optimazation based on code review, use const to save setting for
        different cap channels
Version: 1.20  Date: Nov 24, 2004  By: Hong Yin
    - Ported from HCS12, changed IO map and operated IO pins

*******************************************************************************/
#if( CAPDRV_STOP_EN == TRUE )
tuiSTATUS CAPDRV_fnStop( CAPDRV_tzOBJ *ptzCap )
{
    //check input
    assert( ptzCap );
    
    //check id validation
    assert( ( ptzCap->ucCapID < eCAP_TOTAL ) &&
            ( CapStatus[ ptzCap->ucCapID ] != CAPDRV_NOT_USED ) );
    
    //clear uiCapReady
    ptzCap->teCapReady = CAPDRV_eDATA_INVALID;
    
    //disable capture
    //reset capture     
    switch (ptzCap->ucCapID )
    {
        case eCAP1:
            EvaRegs.CAPCONA.bit.CAP12EN =  F28X_CAPCONX_CAP_1_2_DIS;
            EvaRegs.CAPCONA.bit.CAPRES = CAP_RESET_EN;
             break;
        case eCAP2:
            EvaRegs.CAPCONA.bit.CAP12EN =  F28X_CAPCONX_CAP_1_2_DIS;
            EvaRegs.CAPCONA.bit.CAPRES = CAP_RESET_EN;
            break;
        case eCAP3:
            EvaRegs.CAPCONA.bit.CAP3EN =  F28X_CAPCONX_CAP_3_DIS;
            EvaRegs.CAPCONA.bit.CAPRES = CAP_RESET_EN;
            break;
        case eCAP4:
            EvbRegs.CAPCONB.bit.CAP45EN =  F28X_CAPCONX_CAP_1_2_DIS;
            EvbRegs.CAPCONB.bit.CAPRES = CAP_RESET_EN;
            break;
        case eCAP5:
            EvbRegs.CAPCONB.bit.CAP45EN =  F28X_CAPCONX_CAP_1_2_DIS;
            EvbRegs.CAPCONB.bit.CAPRES = CAP_RESET_EN;
            break;
        case eCAP6:
            EvbRegs.CAPCONB.bit.CAP6EN =  F28X_CAPCONX_CAP_3_DIS;
            EvbRegs.CAPCONB.bit.CAPRES = CAP_RESET_EN;
            break;
        default:    
            assert( 0 );
            break;

    }

    return( eSTATUS_OK );

}//end of CAPDRV_fnStop
#endif

/*******************************************************************************

FUNCTION NAME: 
    CAPDRV_fnGetCnt
  
PURPOSE: 
    check whether capture event happens, if not return
    get captured timer counter value
    call function to calculate frequency of two captured edges
    update teCapReady accordingly

INPUTS: 
    pointer to structure CAPDRV_tzOBJ

OUTPUTS:
    tuiSTATUS    

NOTES:
    none

VERSION HISTORY:

Version: 0.10  Date: Nov-29-2002  By: Yingran Duan
       -new 

Version: 0.11  Date: FEB-20-2003  By: Jason Cumiskey
    - Updated to conform to latest coding standard
Version: 0.20  Date: Nov 24, 2004  By: Hong Yin
    - Ported from HCS12, changed IO map and operated IO pins

*******************************************************************************/
#if( CAPDRV_GETCNT_EN == TRUE )
tuiSTATUS CAPDRV_fnGetCnt( CAPDRV_tzOBJ *ptzCap )
{
    uint16 uiCapfifo;

    //check input
    assert( ptzCap );
    
    //check id validation
    assert( ( ptzCap->ucCapID < eCAP_TOTAL ) &&
            ( CapStatus[ ptzCap->ucCapID ] != CAPDRV_NOT_USED ) );

    //get pin information
    //get capture status
     switch (ptzCap->ucCapID )
    {
        case eCAP1:
            ptzCap->uiRiseFall =  IOMAP_ZEROX_GEN1_PIN;
            uiCapfifo = EvaRegs.CAPFIFOA.bit.CAP1FIFO;
            ptzCap->uiCapCnt = EvaRegs.CAP1FBOT;
            break;
        case eCAP2:
            ptzCap->uiRiseFall =  IOMAP_ZEROX_GEN2_PIN;
            uiCapfifo = EvaRegs.CAPFIFOA.bit.CAP2FIFO;
            ptzCap->uiCapCnt = EvaRegs.CAP2FBOT;
            break;
        case eCAP3:
            ptzCap->uiRiseFall =  IOMAP_ZEROX_L1L2_PIN;
            uiCapfifo = EvaRegs.CAPFIFOA.bit.CAP3FIFO;
            ptzCap->uiCapCnt = EvaRegs.CAP3FBOT;
            break;
        case eCAP4:
            ptzCap->uiRiseFall =  IOMAP_ZERO_CROSS1_PIN;
            uiCapfifo = EvbRegs.CAPFIFOB.bit.CAP4FIFO;
            ptzCap->uiCapCnt = EvbRegs.CAP4FBOT;
            break;
        case eCAP5:
            ptzCap->uiRiseFall =  IOMAP_ZERO_CROSS2_PIN;
            uiCapfifo = EvbRegs.CAPFIFOB.bit.CAP5FIFO;
            ptzCap->uiCapCnt = EvbRegs.CAP5FBOT;
            break;
        case eCAP6:
            ptzCap->uiRiseFall =  IOMAP_CAP6_PIN;
            uiCapfifo = EvbRegs.CAPFIFOB.bit.CAP6FIFO;
            ptzCap->uiCapCnt = EvbRegs.CAP6FBOT;
            break;
        default:    
            assert( 0 );
            break;

    }
                        
    //check capture status, return with error if it is less than 
    //two entries
      
    assert( ( uiCapfifo != CAPFIFO_EMPT ) &&
            ( uiCapfifo != CAPFIFO_ONE ) ); 

    //if more than two entries happend, or if no count value have 
    //been saved, 
    //save the last one to the uiCapCnt
    if( uiCapfifo == CAPFIFO_OVRW )
    {
        ptzCap->teCapReady = CAPDRV_eDATA_OVRW;
    }
    else 
    {
        ptzCap->teCapReady = CAPDRV_eCNT_VALID;
    }

    return( eSTATUS_OK );
}//end of CAPDRV_fnGetCnt
#endif

//end of file capdrv.c

