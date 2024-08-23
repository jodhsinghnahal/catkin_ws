/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    eedrv.h

PURPOSE:
    Provide functions to access the eeprom (Microchip 25LC040/25AA180/160).

FUNCTION(S):
    EEDRV_fnCtor        - EEPROM constructor. No other EEPROM functions can be
                          called unless the Ctor is first called.
    EEDRV_fnDtor        - EEPROM destructor. If this is called, no other EEPROM 
                          function (except for Ctor) can be called.
    EEDRV_fnReadByte    - Read a single byte from the EEPROM at the specified 
                          memory location.
    EEDRV_fnWriteByte   - Write a single byte to the EEPROM at the specified
                          memory location.
    EEDRV_fnReadPage    - Read a bunch of bytes from the EEPROM starting at the
                          specified memory location. The only restriction is 
                          that all of the bytes must reside on the same page. 
    EEDRV_fnWritePage   - Write a bunch of bytes to the EEPROM starting at the
                          specified memory location. The only restriction is 
                          that all of the bytes must reside on the same page. 
    EEDRV_fnIsReady     - Predicate to determine if EEPROM is ready for next 
                          operation (is ready to be written to).
    EEDRV_fnIsBlank   - Predicate to determine if EEPROM is blank.
    EEDRV_fnErase     - Erase an address range of the EEPROM
    
NOTES:
    This file is a common interface to EEPROM module.  It is intended to work
    with any EEPROM, whether it be external or on-chip EEPROM
    
    The EEPROM interface requires the  file eedevice.h to be included into the 
    project that defines the various properties required for the particular 
    EEPROM being used( ie Microchip, Motorola on-chip).  The defines may 
    include EEPROM size, starting address, data size. or any other define that
    may be to use the EEPROM.
    These properties will also define which EEPROM this interface is for.
    The user should be aware of those defines when using this interface(ie data
    size). If eedevice.h is not included in your project, your project will not
    build.  
    
    EEDRV_fnCtor() MUST be called before any other EEPROM functions are called,
    otherwise, they will not work.
    
    An EEPROM device MUST be defined in eedevice.h before the driver can used
    correctly..
    
    
CHANGE HISTORY:
$Log: eedrv.h $

   ***********************************************
   Revision: NovaPfrmB_TrevorM/1
   User: TrevorM     Date: 03/12/05  Time: 01:05AM
   modified to use SPI driver handle

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/4
    User: BaldeeshK     Date: 02/22/05  Time: 05:49PM
    - Added fnErase comment to top of function list
    

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/3
    User: BaldeeshK     Date: 02/15/05  Time: 06:23PM
    - Fixed error in previous keep
    

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/2
    User: BaldeeshK     Date: 02/15/05  Time: 06:15PM
    - Added prototype for erasing the EEPROM
    

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/1
    User: BaldeeshK     Date: 12/09/04  Time: 12:50AM
    - Initial add to Accurev, modified to enable logging of history
    
    


==============================================================================*/
#ifndef EEDRV_INCL
#define EEDRV_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include <stddef.h>
#include "eedevice.h"       // configure EEPROM device parameters 
#include "LIB_stdtype.h"    // Requres and architecture defintion 
#include "LIB_stddefs.h"    // Standard defintions for all projects
#include "HALErrBase.h"
#include "limits.h"


/*==============================================================================
                              Defines
==============================================================================*/

// Target specific defines
#if ( CHAR_BIT == 8 )
#define EEDRV_EMPTY_VALUE   ( 0xFF )
#elif (CHAR_BIT == 16 )
#define EEDRV_EMPTY_VALUE  ( 0xFFFF )
#endif
            
            
/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/
//errno codes based on errbase.h  Note:  errno is an ANSI standard and can be
//found in "The C Programming Language" by K & R for reference.
typedef enum EEDRV_eERRNO
{
    EEDRV_eERRNO_CTOR = ERR_EEPROM_DRV, // cannot construct the EEPROM driver
    EEDRV_eERRNO_DTOR,
    EEDRV_eERRNO_NO_INSTANCE,        // the EEPROM driver was not instantiated
    EEDRV_eERRNO_ILLEGAL_ADX,        // adx does not exist in the EEPROM
    EEDRV_eERRNO_ILLEGAL_NUM_BYTES,  // too manty bytes to read/write
    EEDRV_eERRNO_PAGE_OVERWRITE,     // writing too many bytes to a page
    EEDRV_eERRNO_PAGE_OVERREAD,      // reading too many bytes from a page
    EEDRV_eERRNO_BUSY,               // EEPROM is in a write cycle
    EEDRV_eERRNO_WRITE,              // there was an error writing to the EE
    EEDRV_eERRNO_READ,               // there was an error reading the EE
    EEDRV_eERRNO_PAGE_WRITE,         // an error writing a page to the EE    
    EEDRV_eERRNO_PAGE_READ,          // an error reading a page from the EE    
    EEDRV_eERRNO_ADX_COMMAND,        // error sending the adx & command
    EEDRV_eERRNO_RDSR,               // error reading the status register
    EEDRV_eERRNO_MAX = ERR_EEPROM_DRV_MAX //Max val not to be used
} EEDRV_teERRNO;

/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                          External/Public Constants
==============================================================================*/


/*==============================================================================
                          External/Public Variables
==============================================================================*/


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
//EEDRV_fnCtor
//      EEPRON constructor
//
//Input: None
//Output: tuiSTATUS
//Note: No other EEPROM functions can be called unless the Ctor is first called
tuiSTATUS EEDRV_fnCtor( void ); 

//EEDRV_fnDtor
//      EEPRON distructor
//
//Input: None
//Output: tuiSTATUS
//Note: If this is called, no other EEPROM function (except for Ctor) can be 
//      called.
tuiSTATUS EEDRV_fnDtor( void );

//EEDRV_fnReadChar
//      Read a single byte from the EEPROM at the specified memory location.
//
//Input: Adx    - data address
//       *Data  - to store the reading data from EEPROM
//Output: tuiSTATUS
tuiSTATUS EEDRV_fnReadChar( uint16 Adx,
                            void   *Data );

//EEDRV_fnWriteChar
//      Write a single byte to the EEPROM at the specified memory location.
//
//Input: Adx    - data address
//       Data   - data to write to the EEPROM
//Output: tuiSTATUS
tuiSTATUS EEDRV_fnWriteChar( uint16 Adx,
                             uchar8 Data );

//EEDRV_fnReadPage
//      Read a bunch of bytes from the EEPROM starting at the specified 
//      memory location. The only restriction is that all of the bytes 
//      must reside on the same page. 
//
//Input: Adx    - data address
//       *Data  - to store the reading data from EEPROM
//       Length - Length of the data to read from EEPROM
//Output: tuiSTATUS
tuiSTATUS EEDRV_fnReadPage( uint16 Adx,
                            void   *Data, 
                            size_t Length );
                             
//EEDRV_fnWritePage
//      Write a bunch of bytes to the EEPROM starting at the specified 
//      memory location. The only restriction is that all of the bytes 
//      must reside on the same page. 
//
//Input: Adx    - data address
//       *Data  - to store the writing data to EEPROM
//       Length - Length of the data to write to EEPROM
//Output: tuiSTATUS
tuiSTATUS EEDRV_fnWritePage( uint16 Adx, 
                             void   *Data, 
                             size_t Length );

//EEDRV_fnIsReady
//      Predicate to determine if EEPROM is ready for next operation 
//      (is ready to be written to).
//
//Input: None
//Output: TRUE: is ready
//        FALSE: not yet
tucBOOL EEDRV_fnIsReady( void );

//EEDRV_fnIsBlank
//      Predicate to determine if EEPROM is blank.
//
//Input: None
//Output: TRUE: is blank
//        FALSE: not blank
tucBOOL EEDRV_fnIsBlank( void );

//EEDRV_fnErase
//      Erase part or all of the EEPROM
//
//Input: Adx - Address to start erase
//       Length - length of area to erase
//       Opt    - Erase options
//Output: tuiSTATUS indicating success or failure
tuiSTATUS EEDRV_fnErase( uint16 Adx,
                         size_t Length,
                         uint8 Opt );
    
#endif  // EEDRV_INCL
