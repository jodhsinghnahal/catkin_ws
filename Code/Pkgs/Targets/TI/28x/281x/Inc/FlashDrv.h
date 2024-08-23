/*=============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    FlashDrv.h

PURPOSE:
    This is a header file containing all the data definitions for the flash
    driver.

FUNCTION(S):
    FLASHDRV_fnCtor          - FLASH constructor. No other FLASH functions can be
                            called unless the Ctor is first called.
    FLASHDRV_fnDtor          - FLASH destructor. If this is called, no other FLASH 
                            function (except for Ctor) can be called.
    FLASHDRV_fnProgramBlock  - Write multiple 16-bit words to the FLASH starting 
                            at the specified memory location.
    FLASHDRV_fnEraseRange    - Allows an application to erase a range of
                            code without knowing the sector information of the
                            device.
    FLASHDRV_fnDetermineSector
                          - Used to determine the sector information of an
                            address.
   
NOTES:
    
    FLASHDRV_fnCtor() MUST be called before any other FLASH functions are called,
    otherwise, they will not work.

    Internal flash is a single ported memory type which means that while being 
    cleared, erased, or programmed, it cannot also be executed.  Interrupts are
    disabled during the Flash operations so no OS rechedules or other operation
    will be able to take place.

    To enable use of the external flash interface, globally define 
    EXTFLASHDRV_SECONDARY_MEM_EN in the build options and include 
    ExtFlashDrv.c in your project.  Calls made to the FLASH_ prefixed routines
    will be routed to the external flash routines if they fall outside of the 
    internal flash's sectors.
    

CHANGE HISTORY:
$Log: FlashDrv.h $

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/1
    User: BaldeeshK     Date: 01/24/05  Time: 05:22PM
    - Modified enums to conform to latest coding standard.
    Removed tabs by converting them to spaces.
    

    ***********************************************
    Revision: FieldPrgm_BaldeeshK/3
    User: BaldeeshK     Date: 12/07/04  Time: 11:41PM
    - Added additional line breaks to end of file to eliminate warnings from Code Composer Studio Compiler
    

    ***********************************************
    Revision: FieldPrgm_BaldeeshK/2
    User: BaldeeshK     Date: 12/07/04  Time: 07:47PM
    - Initial add to Accurev for flash driver header file
    


==============================================================================*/ 
#ifndef FLASHDRV_INC
#define FLASHDRV_INC

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stddefs.h"
#include "LIB_stdtype.h"


/*==============================================================================
                              Defines
==============================================================================*/
// fnDetermineSector Defines
#define FLASHDRV_EXTERNAL_BIT_MSK   ( 0x8000 )  // If set in Sector number,
                                                // it signals external flash 
                                                // being used.


/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/

//errno codes based on errbase.h  Note:  errno is an ANSI standard and can be
//found in "The C Programming Language" by K & R for reference.
typedef enum eFLASHDRV_ERRNO
{
    FLASHDRV_eERRNO_CTOR,           // Cannot construct the FLASH driver
    FLASHDRV_eERRNO_DTOR,           // Error removing flash driver
    FLASHDRV_eERRNO_NO_INSTANCE,    // The FLASH driver was not instantiated
    FLASHDRV_eERRNO_ILLEGAL_ADX,    // Adx does not exist in the FLASH
    FLASHDRV_eERRNO_PROT_SECTOR,    // The sector that was to be changed is
                                    //   protected.
    FLASHDRV_eERRNO_CLEAR,          // There was an error setting sector to 0
    FLASHDRV_eERRNO_ERASE,          // The sector failed to erase (Pulse.
    FLASHDRV_eERRNO_COMPACTION,     // Compaction failed during erase.  This
                                    //   could be serious as flash bits now 
                                    //   have depleted charge levels.
    FLASHDRV_eERRNO_PROGRAM,        // Program verification unsuccessful.
    FLASHDRV_eERRNO_PROG_NOT_ERASED,// During Programming, a 0 was found where
                                    //   a 1 was to be programmed.
    FLASHDRV_eERRNO_ALIGNMENT,      // Attempt to write was not properly aligned
    FLASHDRV_eERRNO_ACCESS_ERROR,   // An access error has been flagged during
                                    //   the last operation.
    FLASHDRV_eERRNO_ILLEGAL_CMD,    // An unrecognized command has been issued
                                    //   to the Flash.
    FLASHDRV_eERRNO_CROSS_BOUNDARY, // A program or erase has crossed a boundary
                                    //   that is not permitted.
                                    
    FLASHDRV_eERRNO_MAX             // Max error val - not to be used
} FLASHDRV_teERRNO;


/*=============================================================================
                               Macros
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/
typedef struct FLASHDRV_zSECTORINFOSTRUCT
{
    uint16  uiNumber;           // The number given to this area of flash.
    uint32  ulStart;            // The starting address of the sector.
    uint32  ulEnd;              // The last address belonging in the sector.
} FLASHDRV_tzSECTORINFO;
    
    
/*==============================================================================
                          External/Public Constants
==============================================================================*/


/*==============================================================================
                          External/Public Variables
==============================================================================*/


/*==============================================================================
                      External/Public Function Protoypes
=============================================================================*/
tuiSTATUS FLASHDRV_fnCtor( void );
tuiSTATUS FLASHDRV_fnDtor( void );

// FLASHDRV_fnDetermineSector
//
// Purpose: 
//    Use to determine in which flash sector an address is located in.
// Inputs: 
//    uint32 ulAdx  - Address in unknown sector   
// Outputs: 
//    FLASHDRV_tzSECTORINFO *pzSectorInfo     
//                  - Returned with the details of the sector in which the 
//                    above address is located.
tuiSTATUS FLASHDRV_fnDetermineSector( uint32 ulAdx, 
                                      FLASHDRV_tzSECTORINFO *pzSectorInfo );

// FLASHDRV_fnProgramBlock
//
// Purpose: 
//    Write multiple bytes to the FLASH starting at the specified memory 
//    location.
// Inputs: 
//    uint32 ulAdx          - Destination address
//    uint16 uiData[]       - Source data array.
//    uint16 uiWordsToWrite - The number of words to program.
tuiSTATUS FLASHDRV_fnProgramBlock( uint32 ulAdx,          
                                   uint16 *puiData,       
                                   uint16 uiWordsToWrite );

// FLASHDRV_fnEraseRange
//
// Purpose: 
//    To allow an application to clear/erase a range of code without knowing
//    the sector information of the device. 
// Inputs: 
//    uint32 ulStartAdx     - The start address of the range.
//    uint32 ulEndAdx       - The last address in the range. (This address will
//                          be included in the erase.)
tuiSTATUS FLASHDRV_fnEraseRange( uint32 ulStartAdx,
                                 uint32 ulEndAdx );

#endif  // FLASHDRV_INC

