/*==============================================================================
Copyright © 2004 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  eedrv.c

PURPOSE:
    Provide functions to access an external eeprom.  Currently it is taylored
    for ATMEL EEPROMs

                         !!! IMPORTANT !!!
    This file is only written for 8bit external EEPROMS

FUNCTION(S):
    EEDRV_fnCtor      - EEPROM constructor. No other EEPROM functions can be
                         called unless the Ctor is first called.
    EEDRV_fnSecondCtor- Secondary constructor.
    EEDRV_fnDtor      - EEPROM destructor. If this is called, no other EEPROM 
                         function (except for Ctor) can be called.
    EEDRV_fnErase     - Erase device.
    EEDRV_fnReadByte  - Read a single byte from the EEPROM at the specified 
                         memory location.
    EEDRV_fnWriteByte - Write a single byte to the EEPROM at the specified
                         memory location.
    EEDRV_fnReadPage  - Read a bunch of bytes from the EEPROM starting at the
                         specified memory location. The only restriction is 
                         that all of the bytes must reside on the same page. 
    EEDRV_fnWritePage - Write a bunch of bytes to the EEPROM starting at the
                         specified memory location. The only restriction is 
                         that the bytes must reside on the same page. 
    EEDRV_fnIsReady   - Predicate to determine if EEPROM is ready for next 
                         operation (is ready to be written to).
    EEDRV_fnIsBlank   - Predicate to determine if EEPROM is blank.
                            
    local:
            eedrv_fnWriteEn         - Set the EEPROM to write enable.
            eedrv_fnTxAdxAndCommand - Send the appropriate command and address
                                       to the EEPROM.
            eedrv_ReadStatus        - Return the status of the user supplied
                                       parameter.
            eedrv_fnIsReady         - Check to see if the EEPROM is ready for
                                      the next operation                                       
   
NOTES:
    
    EEDRV_fnCtor() MUST be called before any other EEPROM functions are called,
    otherwise, they will not work.
    
    An EEPROM device MUST be defined before the driver can be compiled.
    
    Enabling and disabling the CS line is CPU/DSP specific. Change 
    EEDRV_CS_LINE if necessary to work with the correct I/O pin. 
    
    This module has function switch enables to not link in functions that
    aren't used.  If you don't want certain functions to be linked in, 
    declare the appropriate switch to be false in you project file.
    
CHANGE HISTORY:
$Log: EeDrv.c $

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/3
    User: HueyD     Date: 06/21/06  Time: 06:01PM
    moved spi ctor from main to eedrv

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/2
    User: HueyD     Date: 03/16/06  Time: 07:56PM
    Renamed DEVICE_mOSTIME_DELAY func to OS_mOSTIME_DELAY as it is moved to
    a new file

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/1
    User: HueyD     Date: 02/27/06  Time: 06:26PM
    changed dealay to use macro

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/1
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/9
    User: HueyD     Date: 02/08/06  Time: 02:45AM
    added cond comp cli commands

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/8
    User: HueyD     Date: 02/06/06  Time: 06:55PM
    Removed cond comp for BURNER cfg

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/7
    User: HueyD     Date: 01/28/06  Time: 01:03AM
    merged

    ***********************************************
    Revision: SurveyorPfrmA_TestBench_JeffF/1
    User: JeffF     Date: 01/26/06  Time: 11:48PM
    Changed calls to reset watchdog macros to the proper function call which
    handles toggling of the watchdog.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/6
    User: HueyD     Date: 01/17/06  Time: 12:54AM
    Removed unused code

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/5
    User: HueyD     Date: 01/11/06  Time: 07:40PM
    updated renaming of defines

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/4
    User: HueyD     Date: 01/04/06  Time: 03:56AM
    Updated driver to use words for both address and length.

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/3
    User: HueyD     Date: 12/12/05  Time: 10:11PM
    Updated EEDRV to allow writes over 1 page.

    Display has capability to change default home screen

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/2
    User: HueyD     Date: 12/12/05  Time: 10:06PM
    Automatically writes past more than one page

    ***********************************************
    Revision: SurveyorPfrmA_HueyD/1
    User: HueyD     Date: 12/09/05  Time: 01:25AM
    Added delay before de-activating CS line.

    This was an issue because the CS line would de-activate before the last
    falling edge of the SCK.

    

    Only happens when SPI is slowed down.  

    ***********************************************
    Revision: SurveyorPfrmA_HollyZ/1
    User: HollyZ     Date: 10/31/05  Time: 05:43PM
    Increased the retry limit in eedrv_fnIsReady()

    ***********************************************
    Revision: SurveyorPfrmA_JosephineT/4
    User: JosephineT     Date: 08/30/05  Time: 06:43PM
    - Conditionally compile out a number of functions while in loader.

    ***********************************************
    Revision: SurveyorPfrmA_JosephineT/3
    User: JosephineT     Date: 08/08/05  Time: 09:24PM
    - Conditional compile big retry limit for loader

    ***********************************************
    Revision: SurveyorPfrmA_JosephineT/2
    User: JosephineT     Date: 08/03/05  Time: 05:44PM
    - Added secondary constructor to enable interrupts, do not enable SPI interrupts
    in constructor.

    ***********************************************
    Revision: NovaPfrmB_HorstM/2
    User: HorstM     Date: 04/23/05  Time: 12:14AM
    set EepromCreated to FALSE in destructor so fbt can invoke eedrv multiple
    times

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/2
    User: BaldeeshK     Date: 03/30/05  Time: 05:45PM
    - Added function call enables/disable switches

    ***********************************************
    Revision: NovaPfrmB_TrevorM/4
    User: TrevorM     Date: 03/19/05  Time: 12:48AM
    Added some missing SPIDRV_fnReleaseLock function calls

   ***********************************************
   Revision: NovaPfrmB_TrevorM/3
   User: TrevorM     Date: 03/12/05  Time: 01:25AM
   modified EEDRV to remove it's reference to the OS timebase (OS_TICKS_PER_SEC).

   
   ***********************************************
   Revision: NovaPfrmB_TrevorM/2
   User: TrevorM     Date: 03/12/05  Time: 01:05AM
   modified to use SPI driver handle

    ***********************************************
    Revision: NovaPfrmB_TrevorM/1
    User: TrevorM     Date: 03/02/05  Time: 11:00PM
    updated for use with sysclk to calculate the SPI prescalar value

    

    ***********************************************
    Revision: NovaPfrmB_JohnB/2
    User: JohnB     Date: 01/24/05  Time: 09:14PM
    Updated for use with SYSCLK package. Removed INIT_fnGetSysClk()
    functionality.
    

    ***********************************************
    Revision: NovaPfrmB_JohnB/1
    User: JohnB     Date: 01/11/05  Time: 07:06PM
    Updated to coding standard. All associated files and
    test updated to accomodate coding standard update.
    
    

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/1
    User: BaldeeshK     Date: 12/09/04  Time: 12:50AM
    - Initial add to Accurev, modified to enable logging of history
    
    
    
==============================================================================*/

// Whether to compile cli or not
#define EEDRV_COMPILE_CLI ( FALSE )

/*==============================================================================
                              Includes
==============================================================================*/
#include "eedrv.h"
#include "devicemacro.h"   // time burning macro
#include "eedevice.h"
#include "LoaderEEPROM.h"  // LDR_EEPROM_UNPROTECTED_ADX
#include "spidrv.h"        // EEPROM is SPI based so use the SPI driver
#include "spislavechnl.h"  // EEPROM is one of SPI slve channel
#include <errno.h>
#include <string.h>        // for memset, memcpy, etc
#include "xassert.h"       // for assert
#include "config.h"        // for cpu-specific defines
#include "Sysextra.h"      // Watchdog resets

#ifndef LOADER
#include "os_macro.h"      // Os macros
#endif

#if( EEDRV_COMPILE_CLI == TRUE )
#include "cli.h"              // Command interface
#include <stdlib.h>           // atoi, malloc
#endif

/*==============================================================================
                              Defines
==============================================================================*/

// Target specific defines
#if ( CHAR_BIT == 8 )
#define EEDRV_EMPTY_VALUE   ( 0xFF )
#elif (CHAR_BIT == 16 )
#define EEDRV_EMPTY_VALUE  ( 0xFFFF )
#endif


#define EEDRV_CS_ACTIVE         ( 0 )
#define EEDRV_CS_INACTIVE       ( 1 )

#define EEDRV_CS_LINE           ( GpioDataRegs.GPADAT.bit.GPIO19 )

#define EEDRV_mINIT()           EALLOW;\
                                GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;\
                                GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;\
                                EEDRV_CS_LINE = EEDRV_CS_INACTIVE; \
                                EDIS

#define EEDRV_mENABLE()         EEDRV_CS_LINE = EEDRV_CS_ACTIVE; \
                                DEVICE_mDELAY_NS( EEDEV_CS_EN_DLY_NS );                                
                                
#define EEDRV_mDISABLE()        DEVICE_mDELAY_NS( EEDEV_CS_HOLD_DLY_NS ); \
                                EEDRV_CS_LINE = EEDRV_CS_INACTIVE; \
                                DEVICE_mDELAY_NS( EEDEV_CS_DIS_DLY_NS );                                

#define EEDRV_LOCK_TIMEOUT      ( 20 ) // 200 mS (assuming 100 ticks per sec)
#define EEDRV_LOCK_NOWAIT       SPIDRV_LOCK_NOWAIT

/*==============================================================================
                              Function Enable Switches
==============================================================================*/

// include code for destructor
#define EEDRV_DTOR_ENABLE           ( FALSE )

// include code for reading a single character
#define EEDRV_READ_CHAR_ENABLE      ( FALSE )

// include code for writing a single character
#define EEDRV_WRITE_CHAR_ENABLE     ( FALSE )

// include code for determining if EEPROM is blank
#define EEDRV_IS_BLANK_ENABLE       ( FALSE )

/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/
// list of available status parameters 
typedef enum EEDRV_eSTATUS_PARAM_LIST
{
    EEDRV_eINVL_STATUS_PARAM = 0,
    EEDRV_eWIP_STATUS_PARAM,
    EEDRV_eWEL_STATUS_PARAM,
    EEDRV_eBP_STATUS_PARAM
} EEDRV_teSTATUS_PARAM;

// Status of the EEPROM
typedef enum EEDRV_eSTATUS_LIST
{
    EEDRV_eINVL_STATUS = 0,
    EEDRV_eWIP_WRITING,
    EEDRV_eWIP_NO_WRITE,
    EEDRV_eWEL_EN,
    EEDRV_eWEL_DIS,
    EEDRV_eBP_NONE,
    EEDRV_eBP_UPPER_QUARTER,
    EEDRV_eBP_UPPER_HALF,
    EEDRV_eBP_ALL
} EEDRV_teSTATUS;

typedef enum EEDRV_eCOMMANDS_LIST
{
    EEDRV_eINVL_INSTR = 0,
    EEDRV_eREAD_COMMAND,
    EEDRV_eWRITE_COMMAND,
    EEDRV_eREAD_STATUS_COMMAND,
    EEDRV_eWRITE_STATUS_COMMAND
} EEDRV_teCOMMANDS;

/*=============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/
static tucBOOL EepromCreated = FALSE;
static SpiDrvHandle hEeprom;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
static tuiSTATUS eedrv_ReadStatus( EEDRV_teSTATUS_PARAM Parameter, 
                                   EEDRV_teSTATUS       *Status );
                             
static tuiSTATUS eedrv_fnTxAdxAndCommand( uint16 Adx, 
                                          EEDRV_teCOMMANDS Command );                             
static tuiSTATUS eedrv_fnWriteEn( void );                                    
static tucBOOL eedrv_fnIsReady( void );


#if( EEDRV_COMPILE_CLI == TRUE )
static CLI_teSTATUS eedrv_fnCliRead( uint8 argC, char *argV[] );
static CLI_teSTATUS eedrv_fnCliWrite( uint8 argC, char *argV[] );
#endif

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

#if( EEDRV_COMPILE_CLI == TRUE )
// Cli commands
static const CLI_tzINFO katzEedrvCmds[ ] = 
{    
    { "EEREAD",   CLI_eACCESS_BASIC,  eedrv_fnCliRead  },
    { "EEWRITE",  CLI_eACCESS_BASIC,  eedrv_fnCliWrite },
    END_CMD_LIST
};
#endif

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnCtor
    
PURPOSE: 
    EEPROM constructor

INPUTS: 
    none                             
    
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)

NOTES:
    No other EEPROM functions can be called unless the Ctor is first called.
   
VERSION HISTORY:

Version: 1.00  Date: Oct. 1, 2004  By: Yingran Duan
    - Created.
    
Version: 1.10  Date: Mar. 2, 2005  By: Trevor Monk
    - changed prescalar assertion to be based on LSPCLK
    
Version: 1.20  Date: Mar 8, 2005   By: Trevor Monk
    - changed prescalar to BitRate
Version: 1.21  Date: Aug 3, 2005   By: Josephine Tsen
    - commented out enabling of SPI interrupts. Interrupts are enabled
      in the secondary constructor.
Version: 01.22    Date: 06-Feb-2006    By: Huey Duong
    - Added cli commands    
*******************************************************************************/
tuiSTATUS EEDRV_fnCtor( void )
{   

    if( SPIDRV_fnCtor() != eSTATUS_OK )
    {
        return( eSTATUS_ERR );
    }

    // ctor can only be called once if it hasn't been called before or if the
    //  Dtor was just called
    assert( EepromCreated == FALSE );

    // initialize the EEPROM chip select line
    EEDRV_mINIT();
    
    // instantiate an SPI driver instance
    if( ( hEeprom = SPIDRV_fnCreateHandle() ) == NULL )
    {
        return( eSTATUS_ERR );
    }

    // get an exclusive lock to the SPI device
    if( SPIDRV_fnAcquireLock( hEeprom, EEDRV_LOCK_TIMEOUT ) == eSTATUS_ERR )
    {
        // unable to acquire an exclusive lock within the timeout
        return( eSTATUS_ERR );
    }

    // initialize the SPI for the EEPROM function
    if (SPIDRV_fnSetCfg( hEeprom, 
                       EEDEV_COMM_MODE,
                       EEDEV_COMM_CLK_EDGE,
                       EEDEV_COMM_CLK_PHS,
                       EEDEV_COMM_BAUD,
                       EEDEV_INSTRUCTION_LEN ) == eSTATUS_ERR )
    {
        SPIDRV_fnReleaseLock( hEeprom );
        return( eSTATUS_ERR );
    }       

    // enable the SPI transmit
    if( SPIDRV_fnEnTx( hEeprom ) != eSTATUS_OK )
    {
        SPIDRV_fnReleaseLock( hEeprom );
        return( eSTATUS_ERR );
    }

    // This section of code is commenet out so that interrupts are not 
    // enabled in this constructor. 
    // They are enabled in the secondary constructor.
    // enable the SPI interrupt
    /* 
    if( SPIDRV_fnEnInt( hEeprom ) != eSTATUS_OK )
    {
        SPIDRV_fnReleaseLock( hEeprom );
        return( eSTATUS_ERR );
    }
    */
    
    
    // enable the EEPROM chip select line
    EEDRV_mENABLE();  

    // issue the "Write Status Register" command
    if( SPIDRV_fnWrite( hEeprom,  EEDEV_WRSR ) != eSTATUS_OK )
    {
        EEDRV_mDISABLE();
        SPIDRV_fnReleaseLock( hEeprom );
        return( eSTATUS_ERR );
    }
    
    // write the status register setting
    if( SPIDRV_fnWrite( hEeprom, EEDEV_STATUS_SETTING ) != eSTATUS_OK )
    {
        EEDRV_mDISABLE();
        SPIDRV_fnReleaseLock( hEeprom );
        return( eSTATUS_ERR );
    }

    // disable the EEPROM chip select line
    EEDRV_mDISABLE();    

    // release the SPI device lock
    SPIDRV_fnReleaseLock( hEeprom );

    EepromCreated = TRUE;
    
#if( EEDRV_COMPILE_CLI == TRUE )
    // register Eedrv commands
    CLI_fnAddCommands( katzEedrvCmds );
#endif

    return( eSTATUS_OK );
}

/*******************************************************************************

FUNCTION NAME:
    EEDRV_fnSecondCtor

PURPOSE:
    Secondary constructor for the EEDRV. This function calls the secondary Ctor for
    the SPI driver.

    This function must be called after the Global Interrupts have been enabled.

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: Aug 2, 2005  By: Josephine Tsen
    - Created.

*******************************************************************************/
tuiSTATUS EEDRV_fnCtor2( void )
{
    // Call SPIDRV second Ctor 
    return( SPIDRV_fnCtor2( hEeprom ));
    
} // EEDRV_fnCtor2

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnDtor

PURPOSE: 
    EEPROM destructor

INPUTS: 
    none                             
    
OUTPUTS:
    returns eSTATUS_OK if the EEPROM SPI handle was deleted ok
    returns eSTATUS_ERR if there was a problem deleting the EEPROM SPI handle

NOTES:
    none                             

VERSION HISTORY:

Version: 1.00  Date: Oct. 1, 2004  By: Yingran Duan
    - Created.

Version: 1.10  Date: Mar 16, 2005  By: Trevor Monk
    - modified to use EEPROM handle
    
Version: 1.11  Date: APR-22-2005  By: Horst Maurer
    - set EepromCreated to FALSE to allow multiple invocations of the 
      eedrv (for fbt)
    
*******************************************************************************/
#if ( EEDRV_DTOR_ENABLE == TRUE )
tuiSTATUS EEDRV_fnDtor( void )
{
    tuiSTATUS status;
    
    status = SPIDRV_fnDeleteHandle(hEeprom);
 
    hEeprom = NULL;
       
    EepromCreated = FALSE;
    
    return( status );
}
#endif // #if ( EEDRV_DTOR_ENABLE == TRUE )

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnReadChar

PURPOSE: 
    Read a single character from the EEPROM at the specified memory location.

INPUTS: 
    !!! Adx and Data are the same type as the processor.  ie.. if CHAR_BIT ==
    16 then they are words.
    Adx  - address to read in the EEPROM
    Data - the data read from the EEPROM
                             
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)

NOTES:
    EEPROM Read Byte Sequence:
        - send Read instruction
        - send address
        - receive data    

    It is up to the user to schedule reads. Remember, there is a max. 5ms write
    time for most Microchip EEPROMs. Check the data sheet for exact figures. It
    is not possible to read the EEPROM during an internal write cycle.

   
VERSION HISTORY:

Version: 1.00  Date: Oct. 1, 2004  By: Yingran Duan
    - Created.

Version: 1.10  Date: Mar  8, 2005  By: Trevor Monk
    - Added SPI device locking
    
*******************************************************************************/
#if ( EEDRV_READ_CHAR_ENABLE == TRUE )
tuiSTATUS EEDRV_fnReadChar( uint16 Adx,
                            void   *Data )
{
    tuiSTATUS     tuiStatus = eSTATUS_OK; // assume everything is ok upon entry
    
    // only run the function if the ctor was called
    assert( EepromCreated == TRUE );
    
    // check for NULL pointer
    assert( Data );
    
    // see if the adx is valid before we do anything
    assert( Adx <= EEDEV_END_ADX );

    // get an exclusive lock to the SPI device
    if( SPIDRV_fnAcquireLock( hEeprom, EEDRV_LOCK_TIMEOUT ) == eSTATUS_ERR )
    {
        // unable to acquire an exclusive lock within the timeout
        return( eSTATUS_ERR );
    }
     
    // make sure there isn't an internal write operation in progress
    if( eedrv_fnIsReady() != TRUE )
    {
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_READ );
        SPIDRV_fnReleaseLock( hEeprom );
        return( tuiStatus );    
    }
 
    // send the appropriate command and address
    if( eedrv_fnTxAdxAndCommand( Adx, 
                                 EEDRV_eREAD_COMMAND ) != eSTATUS_OK )
    {
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_READ );
        SPIDRV_fnReleaseLock( hEeprom );        
        return( tuiStatus );    
    }                                  

    // read the data
    tuiStatus = SPIDRV_fnRead( hEeprom, Data );

    // finished reading, so disable the EEPROM
    EEDRV_mDISABLE();

    // release the SPI device lock
    SPIDRV_fnReleaseLock( hEeprom );

    return( tuiStatus );
}
#endif // #if ( EEDRV_READ_CHAR_ENABLE == TRUE )                             

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnWriteChar
  
PURPOSE: 
    Write a single byte to the EEPROM at the specified memory location.

INPUTS: 
    !!! Adx and Data are the same type as the processor.  ie.. if CHAR_BIT ==
    16 then they are words.

    Adx  - address to write to in the EEPROM
    Data - data to write to the EEPROM

OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)

NOTES:
    EEPROM Write Byte Sequence:
        - send Write instruction
        - send address
        - send data  
        
    It is up to the user to schedule writes. Remember, there is a max. 5ms write
    time for most Microchip EEPROMs. The EEPROM cannot be written to if there is 
    an internal write cycle in process. Check the data sheet for exact figures.  
  
VERSION HISTORY:

Version: 1.00  Date: Oct. 1, 2004  By: Yingran Duan
    - Created.

Version: 1.10  Date: Mar 16, 2005  By: Trevor Monk
    - added exclusive locking
    
*******************************************************************************/
#if ( EEDRV_WRITE_CHAR_ENABLE == TRUE )
tuiSTATUS EEDRV_fnWriteChar( uint16 Adx,
#if( CHAR_BIT == 8 )
                                uchar8 Data )
#elif( CHAR_BIT == 16 )
                                uint16 Data )
#endif
{
    tuiSTATUS     tuiStatus = eSTATUS_OK; // assume everything is ok upon entry
        
    // only run the function if the ctor was called
    assert( EepromCreated == TRUE );
    
    // see if the adx is valid before we do anything
    assert( Adx <= EEDEV_END_ADX );

    // get an exclusive lock to the SPI device
    if( SPIDRV_fnAcquireLock( hEeprom, EEDRV_LOCK_TIMEOUT ) == eSTATUS_ERR )
    {
        // unable to acquire an exclusive lock within the timeout
        return( eSTATUS_ERR );
    }
        
    // make sure there isn't an internal write operation in progress
    if( eedrv_fnIsReady() != TRUE )
    {
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_WRITE );

        // release the SPI device lock
        SPIDRV_fnReleaseLock( hEeprom );
        
        return( tuiStatus );    
    }
             
    // send the appropriate command and address
    if( eedrv_fnTxAdxAndCommand( Adx, 
                                 EEDRV_eWRITE_COMMAND ) != eSTATUS_OK )
    {
        // release the SPI device lock
        SPIDRV_fnReleaseLock( hEeprom );

        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_WRITE );
        return( tuiStatus );    
    }
        
    // send the data
    tuiStatus = SPIDRV_fnWrite( hEeprom, Data );
            
    // finished writing, so disable the EEPROM
    EEDRV_mDISABLE();

    // release the SPI device lock
    SPIDRV_fnReleaseLock( hEeprom );

    return( tuiStatus );
}                              
#endif // #if ( EEDRV_WRITE_CHAR_ENABLE == TRUE )

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnReadPage
                                   
PURPOSE: 
    Read a bunch of characters from the EEPROM starting at the specified memory
    location.
     

INPUTS: 
    !!! Adx, Data and Length are the same type as the processor.  
    ie.. if CHAR_BIT == 16 then they are words.

    Adx       - where to start reading
    Data      - where the read data is stored
    length    - number of characters to read
     
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)
    
NOTES:
    
VERSION HISTORY:

Version: 1.00  Date: Oct. 1, 2004  By: Yingran Duan
    - Created.

Version: 1.10  Date: Mar 16, 2005  By: Trevor Monk
    - added exclusive locking
Version: 01.11    Date: 08-Dec-2005    By: Huey Duong
    - Allow reading across pages    
    - Allow reading more than one page
*******************************************************************************/
tuiSTATUS EEDRV_fnReadPage( uint16 Adx,
                             void *Data,
                             size_t Length )
{
// !!! TREAT EVERYTHING IN HERE AS WORDS

    tuiSTATUS     tuiStatus = eSTATUS_OK; // assume everything is ok upon entry
    uchar8        Count;
    
    // only run the function if the ctor was called
    assert( EepromCreated == TRUE );

    // check for NULL pointer
    assert( Data );

    // see if the adx is valid before we do anything
    assert( Adx <= EEDEV_END_ADX );

    // get an exclusive lock to the SPI device
    if( SPIDRV_fnAcquireLock( hEeprom, EEDRV_LOCK_TIMEOUT ) == eSTATUS_ERR )
    {
        // unable to acquire an exclusive lock within the timeout
        return( eSTATUS_ERR );
    }
       
    // make sure there isn't an internal write operation in progress
    if( eedrv_fnIsReady() != TRUE )
    {
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_PAGE_READ );
        SPIDRV_fnReleaseLock( hEeprom );
        return( tuiStatus );    
    }
       
    // send the appropriate command and address
    if( eedrv_fnTxAdxAndCommand( Adx, 
                                 EEDRV_eREAD_COMMAND ) != eSTATUS_OK )
    {
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_PAGE_READ );
        SPIDRV_fnReleaseLock( hEeprom );
        return( tuiStatus );    
    }                                  

    // read the data 
    for( Count = 0; Count < Length; Count++ )
    {
#if( CHAR_BIT == 16 )
        if( SPIDRV_fnRead( hEeprom, ( uint16 *)Data + Count ) != eSTATUS_OK )
#elif( CHAR_BIT == 8 )
        if( SPIDRV_fnRead( hEeprom, ( uchar8 *)Data + Count ) != eSTATUS_OK )
#endif
        {
            EEDRV_mDISABLE();
            SPIDRV_fnReleaseLock( hEeprom );            
            return( eSTATUS_ERR );
        }
    }
        
    // all done, so disable the EEPROM
    EEDRV_mDISABLE();

    // release the exclusive lock to the SPI device
    SPIDRV_fnReleaseLock( hEeprom );
    
    return( tuiStatus );
}

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnWritePage
                                   
PURPOSE: 
    Write a page to the EEPROM starting at the specified memory location.

INPUTS: 
    !!! Adx, Data and Length are the same type as the processor.  
    ie.. if CHAR_BIT == 16 then they are words.

    Adx    - where to start writing
    Data   - the data to write
             NULL - to erase
    length - number of words to write
     
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)
    
NOTES:
    The only restriction is that all of the characters must reside on the same 
    page. But this driver should handle the writes across the pages
    
VERSION HISTORY:

Version: 1.00  Date: Oct. 4, 2004  By: Yingran Duan
    - Created.

Version: 1.10  Date: Mar 16, 2005  By: Trevor Monk
    - Added exclusive locking
Version: 02.00    Date: 08-Dec-2005    By: Huey Duong
    - Can write pass pages.
    - Can erase as well        
*******************************************************************************/
tuiSTATUS EEDRV_fnWritePage( uint16 Adx,          
                              void *Data,       
                              size_t Length )
{
// !!! TREAT EVERYTHING IN HERE AS WORDS

    tuiSTATUS  tuiStatus = eSTATUS_OK; // assume everything is ok upon entry
    uchar8     Count;
    uint16     NextPageAdx;
    uint16     WriteAdx;
    uint16     EndAdx;
    uint16     WriteLen;
#if( CHAR_BIT == 16 )
    uint16*    pData;
#elif( CHAR_BIT == 8 )
    uchar8*     pData;
#endif
    
    // only run the function if the ctor was called
    assert( EepromCreated == TRUE );

    // see if the adx is valid before we do anything
    assert( Adx <= EEDEV_END_ADX );
       
    // get an exclusive lock to the SPI device
    if( SPIDRV_fnAcquireLock( hEeprom, EEDRV_LOCK_TIMEOUT ) == eSTATUS_ERR )
    {
        // unable to acquire an exclusive lock within the timeout
        return( eSTATUS_ERR );
    }

    // Get the end address and init data pointer
    EndAdx = Adx + Length;

#if( CHAR_BIT == 16 )
    pData  = ( uint16* )Data;
#elif( CHAR_BIT == 8 )
    pData  = ( uchar8* )Data;
#endif

    // Start writing at the beginning       
    WriteAdx = Adx;

    do
    {
        // Get the next page's start address
        NextPageAdx = ( WriteAdx / EEDEV_PAGE_SIZE + 1 ) * EEDEV_PAGE_SIZE;

        // Write one or less page
        if( NextPageAdx < EndAdx )
        {
            WriteLen = NextPageAdx - WriteAdx;
        }
        else
        {
            WriteLen = EndAdx - WriteAdx ;
        }
        
        // make sure there isn't an internal write operation in progress
        if( eedrv_fnIsReady() != TRUE )
        {
            mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_PAGE_WRITE );
            SPIDRV_fnReleaseLock( hEeprom );        
            return( tuiStatus );    
        }

        // send the appropriate command and address
        if( eedrv_fnTxAdxAndCommand( WriteAdx, 
                                     EEDRV_eWRITE_COMMAND ) != eSTATUS_OK )
        {
            mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_PAGE_WRITE );
            SPIDRV_fnReleaseLock( hEeprom );        
            return( tuiStatus );    
        }                                  

        // send the data 
        for( Count = 0; Count < WriteLen; Count++ )
        {
            // If pointer is a null write empty values
            if( Data == NULL )
            {
                tuiStatus = SPIDRV_fnWrite( hEeprom, EEDRV_EMPTY_VALUE );
            }
            else
            {
                // If pointer is not null then write data
                tuiStatus = SPIDRV_fnWrite( hEeprom, *pData );
                pData++;
            }

            // Check status
            if( tuiStatus != eSTATUS_OK )
            {
                EEDRV_mDISABLE();
                SPIDRV_fnReleaseLock( hEeprom );            
                return( eSTATUS_ERR );
            }
        } // end for
        
        // finish writing one page, so disable the EEPROM
        EEDRV_mDISABLE();

        // Update the new address to write to
        WriteAdx += WriteLen;

        // Reset watchdog as this may take some time
        SYS_fnResetWatchdog( );

    }while( WriteAdx < EndAdx );
    // Keep writing till the end address is reached

    // release the exclusive lock
    SPIDRV_fnReleaseLock( hEeprom );
    
    return( tuiStatus );
}                             


/*******************************************************************************

FUNCTION NAME: 
    eedrv_fnCliRead
  
PURPOSE: 
    Read a page from the specified offset

INPUTS: 
    none

OUTPUTS:
    CLI_teSTATUS -  CLI_eSTATUS_SUCCESS : command accepted
                 -  CLI_eSTATUS_FAILURE : command failed

NOTES:
    Syntax: EEREAD Offset PageSize

VERSION HISTORY:

Version: 1.00  Date: 2005-07-07  By: Josephine Tsen
    - Created
Version: 01.01    Date: 06-Feb-2006    By: Huey Duong
    - Moved from nvem test project    

*******************************************************************************/
#if( EEDRV_COMPILE_CLI == TRUE )
static CLI_teSTATUS eedrv_fnCliRead( uint8 argC, char* argV[] )
{    
   tuiSTATUS Status;
   uint16    Offset;
   uint16    PageSize;
   uint16    i;

#if( CHAR_BIT == 8 )
    uint8* pData;
#elif( CHAR_BIT == 16 )
    uint16* pData;
#endif
    
    // This function should only execute if 2 arguments were sent to it
    if( argC != 3 )
    {
        return( CLI_eSTATUS_FAILURE );
    }

    Offset   = atoi( argV[1] );
    PageSize = atoi( argV[2] );

    if( ( pData = malloc( PageSize ) ) == NULL )
    {
        return( CLI_eSTATUS_FAILURE );
    }
    
    // Fill data with known pattern
    for( i = 0; i < PageSize; i++ )
    {
#if( CHAR_BIT == 8 )
        pData[ i ] = 0xFF;
#elif( CHAR_BIT == 16 )
        pData[ i ] = 0xFFFF;        
#endif        
    }
    
    // Read data
    Status = EEDRV_fnReadPage( Offset, ( void * )pData, PageSize );

    if( eSTATUS_OK == Status )
    {
        for( i = 0; i < PageSize; i++ )
        {
            printf( "%04x ", pData[ i ] );
        }
        
        printf( "\r" );
    }
    
    free( pData );

    if( Status == eSTATUS_OK )
    {
        return( CLI_eSTATUS_SUCCESS ); 
    }
    else
    {
        return( CLI_eSTATUS_FAILURE );
    }
}
#endif

/*******************************************************************************

FUNCTION NAME: 
    eedrv_fnCliWrite
  
PURPOSE: 
       Write a page of data

INPUTS: 
    none

OUTPUTS:
    CLI_teSTATUS -  CLI_eSTATUS_SUCCESS : command accepted
                 -  CLI_eSTATUS_FAILURE : command failed

NOTES:
    Syntax: EEWRITE Offset WORD1 WORD2 ...

VERSION HISTORY:

Version: 1.00  Date: 2005-07-07  By: Josephine Tsen
    - Created
Version: 01.01    Date: 06-Feb-2006    By: Huey Duong
    - Moved from nvem test project    

*******************************************************************************/
#if( EEDRV_COMPILE_CLI == TRUE )
static CLI_teSTATUS eedrv_fnCliWrite( uint8 argC, char* argV[] )
{    
   tuiSTATUS Status;
   uint16    Offset;
   uint16    PageSize;
   int       i;

#if( CHAR_BIT == 8 )
    uint8* pData;
#elif( CHAR_BIT == 16 )
    uint16* pData;
#endif
    
    // Must have at least 2 argments
    if( argC < 2 )
    {
        return( CLI_eSTATUS_FAILURE );
    }

    Offset = atoi( argV[1] );
    PageSize = argC - 2;

#if( CHAR_BIT == 8 )
    if( ( pData = ( uint8*) malloc( (size_t) PageSize ) )== NULL )
#elif( CHAR_BIT == 16 )
    if( ( pData = ( uint16*) malloc( (size_t) PageSize ) ) == NULL )
#endif
    {
        return( CLI_eSTATUS_FAILURE );
    }

    // initialize data
    for( i = 0; i < PageSize; i++ )
    {
        pData[ i ] = atoi( argV[ 2 + i ] );
    }
    
    // Write Data
    Status = EEDRV_fnWritePage( Offset, ( void * )pData, PageSize );

    free( pData );

    if( Status == eSTATUS_OK )
    {
        return( CLI_eSTATUS_SUCCESS ); 
    }
    else
    {
        return( CLI_eSTATUS_FAILURE );
    }
} // eedrvtest_fnWritePage
#endif

/*******************************************************************************

FUNCTION NAME: 
    eedrv_ReadStatus
                                   
PURPOSE: 
    Return the status of the user supplied parameter.

INPUTS: 
    EEDRV_STATUS_PARAM Parameter - parameter that the user wants to check on
    EEDRV_STATUS *Status         - status of the given parameter

OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)
    
NOTES:
    
VERSION HISTORY:

Version: 1.00  Date: Oct. 4, 2004  By: Yingran Duan
    - Created.
    
*******************************************************************************/
static tuiSTATUS eedrv_ReadStatus( EEDRV_teSTATUS_PARAM Parameter, 
                                   EEDRV_teSTATUS       *Status )
{
    uint16 ReadStatusRegister;
    tuiSTATUS tuiStatus = eSTATUS_OK; // assume everything is ok upon entry

    // check for NULL pointer
    assert( Status );
 
    // enable the EEPROM
    EEDRV_mENABLE();

    // make sure we are set up for 8 bits
    SPIDRV_fnSetCharLength( hEeprom, EEDEV_INSTRUCTION_LEN );
    
    // send the read status register command to the EEPROM
    if( SPIDRV_fnWrite( hEeprom, EEDEV_RDSR ) != eSTATUS_OK )
    {
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_RDSR );
        EEDRV_mDISABLE();
        return( tuiStatus );
    }                     
    
    // read the status register
    if( SPIDRV_fnRead( hEeprom, &ReadStatusRegister ) != eSTATUS_OK )
    {
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_RDSR );
        EEDRV_mDISABLE();
        return( tuiStatus );
    }

    // done using the EEPROM so disable it.
    EEDRV_mDISABLE();

    switch( Parameter )
    {
        case EEDRV_eWIP_STATUS_PARAM:
            if( (ReadStatusRegister & EEDEV_SR_WIP_MSK) == 0x00 )
            {
                *Status = EEDRV_eWIP_NO_WRITE; 
            }
            else
            {
                *Status = EEDRV_eWIP_WRITING;
            }
            break;

        default:
            assert( 0 );
    } //switch( Parameter )   
    
    // everything's ok
    return( tuiStatus );
}

/*******************************************************************************

FUNCTION NAME: 
    eedrv_fnTxAdxAndCommand
                                       
PURPOSE: 
    Send the appropriate command and address to the EEPROM.

INPUTS: 
    uint16 Adx              - 16 bit adx
    EEDRV_COMMANDS Command - type of command we want to send to the EEPROM
    
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)
    
NOTES:
    If there are no errors, this function returns with the EEPROM enabled.
        
VERSION HISTORY:

Version: 1.00  Date: Oct. 4, 2004  By: Yingran Duan
    - Created.
    
*******************************************************************************/
static tuiSTATUS eedrv_fnTxAdxAndCommand( uint16 Adx,
                                          EEDRV_teCOMMANDS Command )
{
    tuiSTATUS tuiStatus = eSTATUS_OK;
    tuiSTATUS Status;
     
    // enable the EEPROM
    EEDRV_mENABLE();

    SPIDRV_fnSetCharLength( hEeprom, EEDEV_INSTRUCTION_LEN ); 
    
#if ( ( CTRL_CONFIG_EEDRV == CTRL_EEDRV_ATMEL_AT25256A ) || \
      ( CTRL_CONFIG_EEDRV == CTRL_EEDRV_ATMEL_AT25640A ) )
    switch( Command )
    {
        case EEDRV_eWRITE_COMMAND: 
            // write-enable the EEPROM before we can write
            if( eedrv_fnWriteEn() != eSTATUS_OK )
            {
                mWRITE_STATUS_AND_ERRNO(  EEDRV_eERRNO_ADX_COMMAND );
                return( tuiStatus );
            }

            // send the instruction to write to the upper bank
            Status = SPIDRV_fnWrite( hEeprom, EEDEV_WRITE );
            break;
            
        case EEDRV_eREAD_COMMAND:
            // send the instruction to write to the upper bank
            Status = SPIDRV_fnWrite( hEeprom, EEDEV_READ );
            break;
            
        default:
            assert( 0 );
    } // switch( Command )
    
    if( Status != eSTATUS_OK )
    {
        EEDRV_mDISABLE();
        return( tuiStatus );
    }
    
    // the adx is transmitted as a 16bit chunk for big EEPROMs
    if( SPIDRV_fnSetCharLength( hEeprom, EEDEV_ADX_LENGTH ) != eSTATUS_OK)
    {
        EEDRV_mDISABLE();
        return( tuiStatus );
    }
        
    // send the adx to write to.
    // Adjust the address.  If processor is 16bit then all address will be
    // in word, but the EEPROM uses 8bit addresses so this needs to be adjusted.
#if( CHAR_BIT == 16 )
    Adx <<= 1;
#endif
    if( SPIDRV_fnWrite( hEeprom, Adx ) != eSTATUS_OK )
    {
        EEDRV_mDISABLE();
        return( tuiStatus );
    }                     

    // data is read n bits at a time
    SPIDRV_fnSetCharLength( hEeprom, EEDEV_DATA_LENGTH );
    
#else
#error A valid EEPROM device has not been selected. See config.h   
#endif

    return( tuiStatus );
}


/*******************************************************************************

FUNCTION NAME: 
    eedrv_fnWriteEn
                                   
PURPOSE: 
    Set the EEPROM to write enable.

INPUTS: 
    none.
        
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)
    
NOTES:
    Sends the write enable command to the EEPROM and then delays for >250ns. 
    
VERSION HISTORY:

Version: 1.00  Date: Oct. 4, 2004  By: Yingran Duan
    - Created.
    
*******************************************************************************/
static tuiSTATUS eedrv_fnWriteEn( void )
{
    // tx the write enable command to the EEPROM
    if( SPIDRV_fnWrite( hEeprom, EEDEV_WREN ) != eSTATUS_OK )
    {
        EEDRV_mDISABLE();
        return( eSTATUS_ERR );
    }
                     
    // must do a CS disable after the WREN instruction
    EEDRV_mDISABLE(); 

    // re-enable the EEPROM to write data to it
    EEDRV_mENABLE();
    
    return( eSTATUS_OK );
}
    
/*******************************************************************************

FUNCTION NAME: 
    eedrv_fnIsReady
                                   
PURPOSE: 
    Let the user know if the EEPROM is ready to be written to.

INPUTS: 
    none.
        
OUTPUTS:
    tucBOOL - status of the EEPROM
              true: ready to write
              false: not ready to write

    
NOTES:
    
    
VERSION HISTORY:

Version: 1.00  Date: Oct. 9, 2004  By: Yingran Duan
    - Created.
Version: 1.01  Date: Jul-08-2005   By: Josephine Tsen
    - Added retries
Version: 1.02  Date: Oct-27-2005   By: Holly Zhou
    - Increased the retry limit. Longer delay is required when the new data is
      loaded for the first time.
Version: 01.03    Dat: 24-Feb-2006    By: Huey Duong
    - Changed delay to use macro    
*******************************************************************************/
tucBOOL eedrv_fnIsReady( void )
{
    EEDRV_teSTATUS Status;
    uint16         DelayCounts = 0;
    
// This retry limit is obtained from testing. Retry is normally not required 
// except when the whole EEPROM is erased. This delay is not an issue when the 
// whole EEPROM is erased because after an erase, the micro is reset.   
#ifdef LOADER
#define RETRY_LIMIT  ( 0xFFFF)
#else
#define RETRY_LIMIT  ( 0x0200 )  
#endif
    // only run the function if the ctor was called
    assert( EepromCreated == TRUE );

    while( DelayCounts++ < RETRY_LIMIT )
    {
        eedrv_ReadStatus( EEDRV_eWIP_STATUS_PARAM,
                          &Status );

        if( EEDRV_eWIP_NO_WRITE == Status )
        {
            return( TRUE );
        }
        else
        {
#ifndef LOADER
            // delay 10 ms before retrying
            OS_mOSTIME_DELAY_MS( 10 );
#endif
        }
    }

    return( FALSE );

}


/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnIsReady
                                   
PURPOSE: 
    Let the user know if the EEPROM is ready to be written to.

INPUTS: 
    none.
        
OUTPUTS:
    tucBOOL - status of the EEPROM
              true: ready to write
              false: not ready to write

    
NOTES:
    
    
VERSION HISTORY:

Version: 1.00  Date: Oct. 4, 2004  By: Yingran Duan
    - Created.

Version: 1.10  Date: Oct. 9, 2005  By: Trevor Monk
    - Modified to call eedrv_fnIsReady()
    
*******************************************************************************/
tucBOOL EEDRV_fnIsReady( void )
{
    tucBOOL IsReady;

    // only run the function if the ctor was called
    assert( EepromCreated == TRUE );

    // try to get an exclusive lock, but don't wait if the SPI driver is
    // busy with another task.
    if( SPIDRV_fnAcquireLock( hEeprom, EEDRV_LOCK_NOWAIT ) == eSTATUS_ERR )
    { 
        return FALSE;
    }
  
    IsReady = eedrv_fnIsReady();
          
    // release the exclusive lock
    SPIDRV_fnReleaseLock( hEeprom );
    
    return( IsReady );
}

/*******************************************************************************

FUNCTION NAME: 
    NVMEM_fnIsBlank

PURPOSE:
    Predicate to determine if EEPROM is blank 

INPUTS:
    none.
   
OUTPUTS:
    TRUE if EEPROM is blank, FALSE otherwise
   
NOTES: 

VERSION HISTORY:

Version: 1.00  Date: Apr 24 2003  By: Karl Isham
    - Created 
Version: 1.01  Date: jul-08-205   By: Josephine Tsen
    - Handle 8 and 16 bit processors
*******************************************************************************/
#if ( EEDRV_IS_BLANK_ENABLE == TRUE )
tucBOOL EEDRV_fnIsBlank( void )
{
//TBD MUST USE SMALLER BUFFER

    uint16 EeAdx;
#if( CHAR_BIT == 8 )
    uint8 ReadBuffer[ EEDEV_PAGE_SIZE ];
    uint8 CmpBuffer[ EEDEV_PAGE_SIZE ];
#elif( CHAR_BIT == 16 )
    uint16 ReadBuffer[ EEDEV_PAGE_SIZE ];
    uint16 CmpBuffer[ EEDEV_PAGE_SIZE ];
#endif

    memset( CmpBuffer, EEDRV_EMPTY_VALUE, sizeof( CmpBuffer ) ); 

    for( EeAdx = EEDEV_START_ADX; 
         EeAdx <= EEDEV_END_ADX; 
         EeAdx += EEDEV_PAGE_SIZE )
    {
        
        EEDRV_fnReadPage( EeAdx, 
                          ReadBuffer,
                          EEDEV_PAGE_SIZE );

        // memcmp < 0 if ReadBuffer < CmpBuffer
        //        > 0 if ReadBuffer > CmpBuffer
        //        = 0 if ReadBuffer = CmpBuffer
        if( memcmp( ReadBuffer, CmpBuffer, sizeof( CmpBuffer ) ) != 0 )  
        {
            return( FALSE );
        }                        

        // Reset watchdog as this may take some time
        SYS_fnResetWatchdog( );
    }
     
    return( TRUE ); 
}
#endif // #if ( EEDRV_IS_BLANK_ENABLE == TRUE )

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnErase
  
PURPOSE: 
    This function erases a user supplied amount of sectors.

    This function currently only supports erasing the EEDRV_ERASE_TO_END and
    EEDRV_ERASE_ALL options.

INPUTS:
    Adx is address of first sector to erase 
    Length is number of words to erase
    Opt - EEDRV_ERASE_SECTORS = Simple erase of Length bytes (unsupported)
          EEDRV_ERASE_ALL     = Erase whole eeprom
          EEDRV_ERASE_TO_END  = Erase to end starting at Adx

OUTPUTS:
    eSTATUS_ERR if there is an error
    eSTATUS_OK if erase succeeded

NOTES:
      
VERSION HISTORY:

Version: 1.00 Date: JUN-29-2005  By: Josephine Tsen
    - Ported from SolarStorm
Version: 02.00    Date: 08-Dec-2005    By: Huey Duong
    - Uses write func to erase.. this saves the stack one page worth of words.
      In Surveyor's case that's 128 bytes for every stack that uses this func
*******************************************************************************/
tuiSTATUS EEDRV_fnErase( uint16 Adx,
                         size_t Length,
                         EEDRV_teERASE Opt )
{
    tuiSTATUS tuiStatus = eSTATUS_OK;
    
    // Can only erase the whole unprotected section
    if( Opt == EEDRV_eERASE_TO_END )
    { 
        Length = EEDEV_END_ADX - Adx + 1;
    }
    else if( Opt == EEDRV_eERASE_ALL )
    {
        Adx    = EEDEV_START_ADX;
        Length = EEDEV_SIZE;
    }
    else if( Opt == EERDV_eERASE_LENGTH )
    {
        // Do nothing
    }
    else
    {
        assert( FALSE );
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_WRITE );
        return( tuiStatus );
    }
    
    // Write blanks to the EEPROM
    if( EEDRV_fnWritePage( Adx, 
                           NULL,
                           Length ) != eSTATUS_OK )
    {   
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_WRITE );
       return( tuiStatus );
    }                 
    
    return ( eSTATUS_OK );  
}


