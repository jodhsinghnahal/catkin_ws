/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  eedrv.c

PURPOSE:
    Provide functions to access the eeprom (Microchip 25LC040/25AA180/160).

FUNCTION(S):
    EEDRV_fnCtor      - EEPROM constructor. No other EEPROM functions can be
                         called unless the Ctor is first called.
    EEDRV_fnDtor      - EEPROM destructor. If this is called, no other EEPROM 
                         function (except for Ctor) can be called.
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
    
CHANGE HISTORY:
$Log: eedrv.c $

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


/*==============================================================================
                              Includes
==============================================================================*/
#include "eedrv.h"
#include "devicemacro.h"   // time burning macro
#include "eedevice.h"
#include "spidrv.h"        // EEPROM is SPI based so use the SPI driver
#include "spislavechnl.h"  // EEPROM is one of SPI slve channel
#include <errno.h>
#include <string.h>        // for memset, memcpy, etc
#include "xassert.h"       // for assert
#include "config.h"        // for cpu-specific defines

/*==============================================================================
                              Defines
==============================================================================*/

#define EEDRV_CS_ACTIVE         ( 0 )
#define EEDRV_CS_INACTIVE       ( 1 )

#define EEDRV_CS_LINE           ( GpioDataRegs.GPADAT.bit.GPIOA14 )

#define EEDRV_mINIT()           EALLOW;\
                                GpioMuxRegs.GPAMUX.bit.C2TRIP_GPIOA14 = 0;\
                                GpioMuxRegs.GPADIR.bit.GPIOA14 = 1;\
                                EEDRV_CS_LINE = EEDRV_CS_INACTIVE; \
                                EDIS

#define EEDRV_mENABLE()         EEDRV_CS_LINE = EEDRV_CS_ACTIVE; \
                                DEVICE_mDELAY_NS( EEDRV_CS_EN_DLY_NS );                                
                                
#define EEDRV_mDISABLE()        EEDRV_CS_LINE = EEDRV_CS_INACTIVE; \
                                DEVICE_mDELAY_NS( EEDRV_CS_DIS_DLY_NS );                                

#define EEDRV_LOCK_TIMEOUT      ( 20 ) // 200 mS (assuming 100 ticks per sec)
#define EEDRV_LOCK_NOWAIT       SPIDRV_LOCK_NOWAIT

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
                           Local/Private Constants
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
    
*******************************************************************************/
tuiSTATUS EEDRV_fnCtor( void )
{
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
                       EEDRV_COMM_MODE,
                       EEDRV_COMM_CLK_EDGE,
                       EEDRV_COMM_CLK_PHS,
                       EEDRV_COMM_BAUD,
                       EEDRV_INSTRUCTION_LEN ) == eSTATUS_ERR )
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
    
    // enable the SPI interrupt
    if( SPIDRV_fnEnInt( hEeprom ) != eSTATUS_OK )
    {
        SPIDRV_fnReleaseLock( hEeprom );
        return( eSTATUS_ERR );
    }
    
    // enable the EEPROM chip select line
    EEDRV_mENABLE();  

    // issue the "Write Status Register" command
    if( SPIDRV_fnWrite( hEeprom,  EEDRV_WRSR ) != eSTATUS_OK )
    {
        EEDRV_mDISABLE();
        SPIDRV_fnReleaseLock( hEeprom );
        return( eSTATUS_ERR );
    }
    
    // write the status register setting
    if( SPIDRV_fnWrite( hEeprom, EEDRV_STATUS_SETTING ) != eSTATUS_OK )
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
    
    return( eSTATUS_OK );
}

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
    
*******************************************************************************/
tuiSTATUS EEDRV_fnDtor( void )
{
    tuiSTATUS status;
    
    status = SPIDRV_fnDeleteHandle(hEeprom);
 
    hEeprom = NULL;
       
    return( status );
}

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnReadChar

PURPOSE: 
    Read a single character from the EEPROM at the specified memory location.

INPUTS: 
    Adx  - address to read in the EEPROM
    Data - the data read from the EEPROM
                             
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)

NOTES:
    Microchip EEPROM Read Byte Sequence:
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
tuiSTATUS EEDRV_fnReadChar( uint16 Adx,
                            void   *Data )
{
    tuiSTATUS     tuiStatus = eSTATUS_OK; // assume everything is ok upon entry
    
    // only run the function if the ctor was called
    assert( EepromCreated == TRUE );
    
    // check for NULL pointer
    assert( Data );
    
    // see if the adx is valid before we do anything
    assert( Adx <= EEDRV_LAST_ADX );

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

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnWriteChar
  
PURPOSE: 
    Write a single byte to the EEPROM at the specified memory location.

INPUTS: 
    Adx  - address to write to in the EEPROM
    Data - data to write to the EEPROM

OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)

NOTES:
    Microchip EEPROM Write Byte Sequence:
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
tuiSTATUS EEDRV_fnWriteChar( uint16 Adx,
                              uchar8 Data )
{
    tuiSTATUS     tuiStatus = eSTATUS_OK; // assume everything is ok upon entry
        
    // only run the function if the ctor was called
    assert( EepromCreated == TRUE );
    
    // see if the adx is valid before we do anything
    assert( Adx <= EEDRV_LAST_ADX );

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


/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnReadPage
                                   
PURPOSE: 
    Read a bunch of bytes from the EEPROM starting at the specified memory
    location.

INPUTS: 
    Adx       - where to start reading
    Data      - where the read data is stored
    length    - number of characters to read
     
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)
    
NOTES:
    The only restriction is that all of the bytes must reside on the same 
    page. 
    
    It is up to the user to schedule reads. Remember, there is a max. 5ms write
    time for most Microchip EEPROMs. Check the data sheet for exact figures. It
    is not possible to read the EEPROM during an internal write cycle.

    
VERSION HISTORY:

Version: 1.00  Date: Oct. 1, 2004  By: Yingran Duan
    - Created.

Version: 1.10  Date: Mar 16, 2005  By: Trevor Monk
    - added exclusive locking
    
*******************************************************************************/
tuiSTATUS EEDRV_fnReadPage( uint16 Adx,
                             void *Data,
                             size_t Length )
{
    tuiSTATUS     tuiStatus = eSTATUS_OK; // assume everything is ok upon entry
    uchar8        Count;
    
    // only run the function if the ctor was called
    assert( EepromCreated == TRUE );

    // check for NULL pointer
    assert( Data );

    // can't write too few bytes
    assert( Length >= EEDRV_MIN_PAGE_SIZE );
    
    // can't read too many bytes from the EE
    assert( Length <= EEDRV_PAGE_SIZE );

    // see if the adx is valid before we do anything
    assert( Adx <= EEDRV_LAST_ADX );

    // make sure we're not reading beyond the page boudary
#if( CHAR_BIT == 16 )
    assert( ( ( EEDRV_PAGE_LAST_ADX_MSK & Adx ) + Length * 2 ) <=
          (   ( EEDRV_PAGE_FIRST_ADX_MSK & Adx ) + EEDRV_PAGE_ADX_SPAN ) );

#elif( CHAR_BIT == 8 )
    assert( ( ( EEDRV_PAGE_LAST_ADX_MSK & Adx ) + Length ) <=
          (   ( EEDRV_PAGE_FIRST_ADX_MSK & Adx ) + EEDRV_PAGE_ADX_SPAN ) );
        
#else
#error Invalid CHAR_BIT. Cannot read from EEPROM.
#endif

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

#if( CHAR_BIT == 16 )
    // read the data 
    for( Count = 0; Count < Length; Count++ )
    {
        if( SPIDRV_fnRead( hEeprom, ( uint16 *)Data + Count ) != eSTATUS_OK )
        {
            EEDRV_mDISABLE();
            SPIDRV_fnReleaseLock( hEeprom );            
            return( eSTATUS_ERR );
        }
    }
#elif( CHAR_BIT == 8 )
    // read the data 
    for( Count = 0; Count < Length; Count++ )
    {
        if( SPIDRV_fnRead( hEeprom, ( uchar8 *)Data + Count ) != eSTATUS_OK )
        {
            EEDRV_mDISABLE();
            SPIDRV_fnReleaseLock( hEeprom );            
            return( eSTATUS_ERR );
        }
    }
#else
#error Invalid CHAR_BIT. Cannot read from EEPROM.
#endif

        
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
    Adx    - where to start writing
    Data   - the data to write
    length - number to write
     
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)
    
NOTES:
    The only restriction is that all of the bytes must reside on the same 
    page. 
    
    It is up to the user to schedule writes. Remember, there is a max. 5ms write
    time for most Microchip EEPROMs. The EEPROM cannot be written to if there is 
    an internal write cycle in process. Check the data sheet for exact figures.  

    
VERSION HISTORY:

Version: 1.00  Date: Oct. 4, 2004  By: Yingran Duan
    - Created.

Version: 1.10  Date: Mar 16, 2005  By: Trevor Monk
    - Added exclusive locking
        
*******************************************************************************/
tuiSTATUS EEDRV_fnWritePage( uint16 Adx,          
                              void *Data,       
                              size_t Length )
{
    tuiSTATUS  tuiStatus = eSTATUS_OK; // assume everything is ok upon entry
    uchar8     Count;

    // only run the function if the ctor was called
    assert( EepromCreated == TRUE );

    // check for NULL pointer
    assert( Data );
    
    // can't write too few bytes
    assert( Length >= EEDRV_MIN_PAGE_SIZE );
    
    // can't write too many bytes to the EE
    assert( Length <= EEDRV_PAGE_SIZE );

    // see if the adx is valid before we do anything
    assert( Adx <= EEDRV_LAST_ADX );
       
    // make sure we're not writing beyond the page boudary
#if( CHAR_BIT == 16 )
    assert( ( ( EEDRV_PAGE_LAST_ADX_MSK & Adx ) + Length * 2 ) <=
          (   ( EEDRV_PAGE_FIRST_ADX_MSK & Adx ) + EEDRV_PAGE_ADX_SPAN ) );

#elif( CHAR_BIT == 8 )
    assert( ( ( EEDRV_PAGE_LAST_ADX_MSK & Adx ) + Length ) <=
          (   ( EEDRV_PAGE_FIRST_ADX_MSK & Adx ) + EEDRV_PAGE_ADX_SPAN ) );
        
#else
#error Invalid CHAR_BIT. Cannot read from EEPROM.
#endif

    // get an exclusive lock to the SPI device
    if( SPIDRV_fnAcquireLock( hEeprom, EEDRV_LOCK_TIMEOUT ) == eSTATUS_ERR )
    {
        // unable to acquire an exclusive lock within the timeout
        return( eSTATUS_ERR );
    }

    // make sure there isn't an internal write operation in progress
    if( eedrv_fnIsReady() != TRUE )
    {
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_PAGE_WRITE );
        SPIDRV_fnReleaseLock( hEeprom );        
        return( tuiStatus );    
    }
       
    // send the appropriate command and address
    if( eedrv_fnTxAdxAndCommand( Adx, 
                                 EEDRV_eWRITE_COMMAND ) != eSTATUS_OK )
    {
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_PAGE_WRITE );
        SPIDRV_fnReleaseLock( hEeprom );        
        return( tuiStatus );    
    }                                  


#if( CHAR_BIT == 16 )        
    // send the data 
    for( Count = 0; Count < Length; Count++ )
    {
        if( SPIDRV_fnWrite( hEeprom, *( ( uint16 *)Data + Count ) ) != eSTATUS_OK )
        {
            EEDRV_mDISABLE();
            SPIDRV_fnReleaseLock( hEeprom );            
            return( eSTATUS_ERR );
        }
    }
#elif( CHAR_BIT == 8 )    
    // send the data 
    for( Count = 0; Count < Length; Count++ )
    {
        if( SPIDRV_fnWrite( hEeprom, *( ( uchar8 *)Data + Count ) ) != eSTATUS_OK )
        {
            EEDRV_mDISABLE();
            SPIDRV_fnReleaseLock( hEeprom );
            return( eSTATUS_ERR );
        }
    }
#else
#error Invalid CHAR_BIT. Cannot write to EEPROM.
#endif
        
    // all done, so disable the EEPROM
    EEDRV_mDISABLE();

    // release the exclusive lock
    SPIDRV_fnReleaseLock( hEeprom );
    
    return( tuiStatus );
}                             


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
    SPIDRV_fnSetCharLength( hEeprom, EEDRV_INSTRUCTION_LEN );
    
    // send the read status register command to the EEPROM
    if( SPIDRV_fnWrite( hEeprom, EEDRV_RDSR ) != eSTATUS_OK )
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
            if( (ReadStatusRegister & EEDRV_SR_WIP_MSK) == 0x00 )
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

    SPIDRV_fnSetCharLength( hEeprom, EEDRV_INSTRUCTION_LEN ); 
    
#if CTRL_CONFIG_EEDRV == CTRL_EEDRV_MICROCHIP_MC25LC040
    if( Adx >= UPPER_HALF_STARTING_ADX )
    {
        switch( Command )
        {
            case EEDRV_WRITE_COMMAND:
                // write-enable the EEPROM before we can write
                if( eedrv_fnWriteEn() != eSTATUS_OK )
                {
                    mWRITE_STATUS_AND_ERRNO(  eEEDRV_ERRNO_ADX_COMMAND );
                    return( tuiStatus );
                }
                // send the instruction to write to the upper bank
                Status = SPIDRV_fnWrite( hEeprom, EEDRV_WRITE_UPPER_BANK );
                break;
                
            case EEDRV_READ_COMMAND:
                // send the instruction to write to the upper bank
                Status = SPIDRV_fnWrite( hEeprom, EEDRV_READ_UPPER_BANK );
                break;
                
            default:
                assert( 0 );
        } // switch( Command )
    } // if( Adx >= UPPER_HALF_STARTING_ADX )
    else
    {
        switch( Command )
        {
            case EEDRV_eWRITE_COMMAND:
                // write-enable the EEPROM before we can write
                if( eedrv_fnWriteEn() != eSTATUS_OK )
                {
                    mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_ADX_COMMAND );
                    return( tuiStatus );
                }
                // send the instruction to write to the upper bank
                Status = SPIDRV_fnWrite( hEeprom, EEDRV_WRITE_LOWER_BANK );
                break;
                
            case EEDRV_eREAD_COMMAND:
                // send the instruction to write to the upper bank
                Status = SPIDRV_fnWrite( hEeprom, EEDRV_READ_LOWER_BANK );
                break;
                
            default:
                assert( 0 );
                
        } // switch( Command )
    }

    if( Status != eSTATUS_OK )
    {
        EEDRV_mDISABLE();
        return( tuiStatus );
    }
    
    // send the lower byte of the adx to write to.
    if( SPIDRV_fnWrite( hEeprom, 0x00FF & Adx ) != eSTATUS_OK )
    {
        EEDRV_mDISABLE();
        return( tuiStatus );
    }
#elif ( ( CTRL_CONFIG_EEDRV == CTRL_EEDRV_MICROCHIP_MC25AA080 ) ||  \
        ( CTRL_CONFIG_EEDRV == CTRL_EEDRV_MICROCHIP_MC25AA160 ) ||  \
        ( CTRL_CONFIG_EEDRV == CTRL_EEDRV_MICROCHIP_MC25AA320 ) ||  \
        ( CTRL_CONFIG_EEDRV == CTRL_EEDRV_MICROCHIP_MC25AA640 ) )
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
            Status = SPIDRV_fnWrite( hEeprom, EEDRV_WRITE );
            break;
            
        case EEDRV_eREAD_COMMAND:
            // send the instruction to write to the upper bank
            Status = SPIDRV_fnWrite( hEeprom, EEDRV_READ );
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
    if( SPIDRV_fnSetCharLength( hEeprom, EEDRV_ADX_LENGTH ) != eSTATUS_OK)
    {
        EEDRV_mDISABLE();
        return( tuiStatus );
    }
        
    // send the adx to write to.
    if( SPIDRV_fnWrite( hEeprom, Adx ) != eSTATUS_OK )
    {
        EEDRV_mDISABLE();
        return( tuiStatus );
    }                     

    // data is read n bits at a time
    SPIDRV_fnSetCharLength( hEeprom, EEDRV_DATA_LENGTH );
    
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
    if( SPIDRV_fnWrite( hEeprom, EEDRV_WREN ) != eSTATUS_OK )
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

*******************************************************************************/
tucBOOL eedrv_fnIsReady( void )
{
    EEDRV_teSTATUS Status;

    // only run the function if the ctor was called
    assert( EepromCreated == TRUE );

    eedrv_ReadStatus( EEDRV_eWIP_STATUS_PARAM,
                      &Status );

    return( Status == EEDRV_eWIP_NO_WRITE );
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
*******************************************************************************/
tucBOOL EEDRV_fnIsBlank( void )
{
    uint16 EeAdx;
    uint16 ReadBuffer[ EEDRV_PAGE_SIZE ];
    uint16 CmpBuffer[ EEDRV_PAGE_SIZE ];

    memset( CmpBuffer, EEDRV_EMPTY_VALUE, sizeof( CmpBuffer ) ); 

    for( EeAdx = EEDRV_STARTING_ADX; 
         EeAdx < EEDRV_LAST_ADX; 
         EeAdx += EEDRV_PAGE_ADX_SPAN )
    {
        
        EEDRV_fnReadPage( EeAdx, 
                          ReadBuffer,
                          EEDRV_PAGE_SIZE );

        // memcmp < 0 if ReadBuffer < CmpBuffer
        //        > 0 if ReadBuffer > CmpBuffer
        //        = 0 if ReadBuffer = CmpBuffer
        if( memcmp( ReadBuffer, CmpBuffer, sizeof( CmpBuffer ) ) != 0 )  
        {
            return FALSE;
        }                        
    }
     
    return TRUE; 
}
