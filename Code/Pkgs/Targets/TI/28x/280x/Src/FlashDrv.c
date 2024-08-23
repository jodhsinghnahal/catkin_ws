/*=============================================================================
Copyright © 2004 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  
    FlashDrv.c

PURPOSE:
    Provide functions to access on-chip Flash memory on the TI TMS320LF28xx 
    DSP.

FUNCTION(S):
    FLASHDRV_fnCtor         - FLASH constructor. No other FLASH functions can 
                              be called unless the Ctor is first called.
    FLASHDRV_fnDtor         - FLASH destructor. If this is called, no other 
                              FLASH function (except for Ctor) can be called.
    FLASHDRV_fnProgramBlock - Write multiple 16-bit words to the FLASH starting 
                              at the specified memory location.
    FLASHDRV_fnEraseRange   - Allows an application to clear/erase a range of
                              code without knowing the sector information of the
                              device. 
    FLASHDRV_fnDetermineSector - Provides sector information when an address
                                 in input to the function
    FLASHDRV_fnLock         - Locks the device                                 
                              
    local:
        flashdrv_fnUnlock   - unlock the device so the flash can be erased
                              or programmed

   
NOTES:
    
    FLASHDRV_fnCtor() MUST be called before any other FLASH functions are called,
    otherwise, they will not work.

    Flash is a single ported memory type which means that while being 
    erased, or programmed, it cannot also be executed.  Interrupts are
    disabled  by the library API during the flash operations so no OS rechedules 
    or other operation will be able to take place.
    
    This driver uses the Flash API library provided by Texas Instruments.  In
    order to be able to flash the device, you must include the flash API library
    in your project, and you must properly select a device in the configuration
    FlashAPIConfig.h.  In addition, the command linker file must contain the
    load start, load end, and run start addresses for the library so that the
    driver can copy the functions into the RAM address.  Note: this driver must
    copy the library into Zero Wait state SARAM to execute.

        
CHANGE HISTORY:  
$Log: FlashDrv.c $

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/1
    User: HueyD     Date: 06/21/06  Time: 05:53PM
    Added a lock function.

    Device is not unlocked in ctor anymore as this is unnecessary.  If code
    executes in secured memory, it has full access even if device is locked.

    
    ***********************************************
    Revision: SurveyorPfrmA_JeffF/1
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_HollyZ/1
    User: HollyZ     Date: 11/26/05  Time: 12:54AM
    Added new function FLASHDRV_fnInstallCb() 

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/5
    User: BaldeeshK     Date: 04/26/05  Time: 10:29PM
    - Added real passwords handling mechanisim

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/4
    User: BaldeeshK     Date: 03/30/05  Time: 11:25PM
    - In function fnErase, added initialization of sector mask

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/3
    User: BaldeeshK     Date: 03/04/05  Time: 07:51PM
    - In function fnErase, Modified so that we pass all the sectors that we want
    to erase at once to the API 

    

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/2
    User: BaldeeshK     Date: 01/24/05  Time: 05:31PM
    - Code review changes.  Moved driver instantiation
    to bottom of function in Ctor
    

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/1
    User: BaldeeshK     Date: 01/07/05  Time: 07:49PM
    - Removed tabs and converted them to spaces
    

    ***********************************************
    Revision: FieldPrgm_BaldeeshK/4
    User: BaldeeshK     Date: 12/08/04  Time: 12:25AM
    - Removed previous change
    

    ***********************************************
    Revision: FieldPrgm_BaldeeshK/2
    User: BaldeeshK     Date: 12/07/04  Time: 07:48PM
    - Initial add to Accurev for flash driver source file
    

  
==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/
#include <string.h>                // for memcopy
#include "FlashDrv.h"              // package header
#include "Flash280x_API_Library.h" // Flash API library, to program and erase
                                   // flash, as well as verify 
#include "FlashSectorMap.h"        // Sector Map
#include "device.h"                // Processor IO Port defines.
#include "devicemacro.h"           // macros for ellow and edis
#include "xassert.h"               // Uses assert.
#include <errno.h>                 // Uses errno to pass error codes.


/*==============================================================================
                              Defines
==============================================================================*/

#define FLASHDRV_NUM_WORDS_PSWD         ( 8 ) // number of password locations
                                              // to write to when unsecuring
                                              // the memory
/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/
// enumeration to distinguish whether driver constructor is called
typedef enum FLASHDRV_DRIVER_INSTANCE_LIST
{
    eFLASHDRV_DRIVER_NOT_INSTANTIATED = 0,  // driver not instantiated
    eFLASHDRV_DRIVER_INSTANTIATED           // driver is ready to use
} flashdrv_teDRIVER_INSTANCE;


/*=============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                           Local/Private Constants
==============================================================================*/
//---------------------------------------------------------------------------
//   These key values are used to unlock the CSM by this example
//   They are defined in FlashKey.asm
// --------------------------------------------------------------------------
extern uint16 PRG_key0;         // Password Key 0
extern uint16 PRG_key1;         // Password Key 1
extern uint16 PRG_key2;         // Password Key 2
extern uint16 PRG_key3;         // Password Key 3
extern uint16 PRG_key4;         // Password Key 4
extern uint16 PRG_key5;         // Password Key 5
extern uint16 PRG_key6;         // Password Key 6
extern uint16 PRG_key7;         // Password Key 7
 
/*==============================================================================
                           Local/Private Variables
==============================================================================*/
// state of the instantiation of the driver
static flashdrv_teDRIVER_INSTANCE FlashDriverInstance = 
                                            eFLASHDRV_DRIVER_NOT_INSTANTIATED;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

// unlock the processor to allow for reprogramming
/* Currently not used, but do not delete
tuiSTATUS flashdrv_fnUnlock( void );
*/

/*==============================================================================
                              Macro Definitions
==============================================================================*/

/*==============================================================================
                              Extern Definitions
==============================================================================*/

// only if external flash memory exists and is enabled
#ifdef CONFIG_EXTFLASHDRV_SECONDARY_MEM_EN
extern tuiSTATUS EXTFLASHDRV_fnCtor( void );
extern tuiSTATUS EXTFLASHDRV_fnDtor( void );
extern tuiSTATUS EXTFLASHDRV_fnDetermineSector( uint32 Adx, 
                                             FLASHDRV_tzSECTORINFO *SectorInfo );
extern tuiSTATUS EXTFLASHDRV_fnProgramBlock( uint32 Adx,          
                                             uint16 *Data,
                                             uint16 WordsToWrite );
extern tuiSTATUS EXTFLASHDRV_fnEraseRange( uint32 StartAdx,
                                           uint32 EndAdx );
#endif


   
/*==============================================================================
                             Function Definitions
==============================================================================*/


/*******************************************************************************

FUNCTION NAME: 
    tuiSTATUS FLASHDRV_fnCtor( void )
    
PURPOSE: 
    Flash constructor

INPUTS: 
    none                             
    
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)

NOTES:
    No other Flash functions can be called unless the Ctor is first called.
   
VERSION HISTORY:

Version: 0.01  Date: Mar 7, 2003  By: Alan Travelbea
    - Created.

Version: 1.00  Date: JUL-04-2003  By: Alan Travelbea
    - Added external flash support.
    - Removed instance checking.
    
Version: 2.00  Date: NOV-04-2004  By: Baldeesh Khaira
    - Branched and ported to new processor, C28x.  Included new Flash API 
      library.
Version: 2.01    Date: 19-Jun-2006    By: Huey Duong
    - Removed code to unlock device because there is no need.  The loader
      runs out of secured ram and should have full access to the fal    
*******************************************************************************/
tuiSTATUS FLASHDRV_fnCtor( void )
{

        
    // This globabl variable must be initialized to the macro define 
    // SCALE_FACTOR prior to any calls to the flash API library
    // This scale factor indicates the speed at which the CPU will attempt
    // to erase and reprogram the flash
    Flash_CPUScaleFactor = SCALE_FACTOR;

#ifndef LOADER
    // Don't need to copy if it is in loader. Loader is already running out of RAM.
    // Copy the Flash API functions to SARAM
    ( void )memcpy( &Flash28_API_RunStart,
                    &Flash28_API_LoadStart, 
                    ( uint32 )&Flash28_API_LoadEnd - 
                    ( uint32 )&Flash28_API_LoadStart );
#endif

#ifdef CONFIG_EXTFLASHDRV_SECONDARY_MEM_EN

    if( EXTFLASHDRV_fnCtor( ) != eSTATUS_OK )
    {
        return ( eSTATUS_ERR );
    }
    
#endif

/*  There is no need to unlock the device before accessing flash, as all the
    code should be residing in secured RAM
    if( flashdrv_fnUnlock() != eSTATUS_OK )
    {
        return ( eSTATUS_ERR );
    }
*/    

    // instantiate driver
    FlashDriverInstance = eFLASHDRV_DRIVER_INSTANTIATED;
    // return the status of unlocking the flash
    return( eSTATUS_OK );

}



/*******************************************************************************

FUNCTION NAME: 
    tuiSTATUS FLASHDRV_fnDtor( void )

PURPOSE: 
    Flash destructor

INPUTS: 
    none                             
    
OUTPUTS:                                               
    tuiSTATUS - Status code (either OK or an error code)

NOTES:
    If this is called, no other Flash func (except for Ctor) can be called.   

VERSION HISTORY:

Version: 0.01  Date: Mar 7, 2003  By: Alan Travelbea
    - Created.

Version: 1.00  Date: JUL-04-2003  By: Alan Travelbea
    - External flash support added.
    
Version: 2.00  Date: NOV-04-2004  By: Baldeesh Khaira
    - Branched and ported to new processor, C28x. 
Version: 2.01  Date: JAN-23-2004  By: Baldeesh Khaira
    - Code review changes, moved driver instantiation to bottom of function
*******************************************************************************/
#if( defined( GLBL_DTOR_EN ) || defined( FLASHDRV_DTOR_EN ))
tuiSTATUS FLASHDRV_fnDtor( void )
{
    // Dtor can only be called if Ctor was called
    assert( FlashDriverInstance == eFLASHDRV_DRIVER_INSTANTIATED );

    // uninstantiate the driver
    FlashDriverInstance = eFLASHDRV_DRIVER_NOT_INSTANTIATED;

#if( defined( CONFIG_EXTFLASHDRV_SECONDARY_MEM_EN ) && defined( EXTFLASHDRV_fnDtor ))
    return EXTFLASHDRV_fnDtor();
#else
    
    return( eSTATUS_OK );
#endif
}
#endif



/*******************************************************************************

FUNCTION NAME: 
tuiSTATUS FLASHDRV_fnProgramBlock( uint32 Adx,          
                                uint16 Data[],       
                                uint16 WordsToWrite )
                                   
PURPOSE: 
    Write a bunch of bytes to the FLASH starting at the specified memory 
    location.

INPUTS: 
    uint32 Adx          - Destination address
    uint16 Data[]       - Source data array.
    uint16 WordsToWrite - The number of words to program.
     
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)
    
NOTES:
    
    
VERSION HISTORY:
Version: 0.01  Date: Mar 7, 2003  By: Alan Travelbea
    - Created.

Version: 0.11  Date: MAR-11-2003  By: Alan Travelbea
    - Now allows cross-boundary writes.

Version: 1.00  Date: JUL-11-2003  By: Alan Travelbea
    - Now allows cross-boundary writes.
    
Version: 2.00  Date: OCT-26-2004  By: Baldeesh Khaira
    - Branched and ported to new processor, C28x, included the use of new
      Flash API library to program
    
*******************************************************************************/
tuiSTATUS FLASHDRV_fnProgramBlock( uint32 Adx,          
                                   uint16 Data[],       
                                   uint16 WordsToWrite )
{
    FLASH_ST                ProgStatus;    // FlashAPI program status
    FLASHDRV_tzSECTORINFO   SectorInfo;    // info about current sector
    uint16                  ProgramLength; // length to write


    // Only run the function if the ctor was called
    assert( FlashDriverInstance == eFLASHDRV_DRIVER_INSTANTIATED );
 
    
    // Perform loop once below for each sector the programming will effect.
    // In other words, a sequential write that crosses one sector boundary
    // will cause this while loop to execute twice.
    while( WordsToWrite != 0 )
    {
        // Determine the sector of the destination address in flash
        if( FLASHDRV_fnDetermineSector( Adx, &SectorInfo ) != eSTATUS_OK )
        {   
            // The address that was requested was not defined as valid flash
            //  section.
            errno = FLASHDRV_eERRNO_ILLEGAL_ADX;
            return( eSTATUS_ERR );
        }
#ifdef CONFIG_EXTFLASHDRV_SECONDARY_MEM_EN
        else if( SectorInfo.uiNumber & FLASHDRV_EXTERNAL_BIT_MSK )
        {
            // Program External Flash
            //
            // This address is not located in Internal flash so the external
            // flash driver is called.
            return( EXTFLASHDRV_fnProgramBlock( Adx,          
                                                Data,       
                                                WordsToWrite ));
        }
#endif
        else if(( 1 << SectorInfo.uiNumber ) & FLASHDRV_SECURE_SECTORS )
        {
            // This sector is marked as protected so do not perform clear and
            // return with an error.
            errno = FLASHDRV_eERRNO_PROT_SECTOR;
            return( eSTATUS_ERR );
        }
        else
        {
            // ** Range check the program destination.
            // The check is done as below to prevent over/under flow.
            // If the available locations (SectorInfo.ulEnd - Adx) is less than 
            // the number of locations to write (WordsToWrite), this write
            // will cross a sector boundary so calculate the length remaining
            // in the current sector.
            // 1 is added to this number because the subtraction doesn't take
            // into account that the first address is a valid location to write to
            if(( ( (uint16)SectorInfo.ulEnd - (uint16)Adx ) + 1 ) < WordsToWrite )
            {   
                ProgramLength = (uint16)SectorInfo.ulEnd - (uint16)Adx + 1;
                WordsToWrite -= ProgramLength;
            }
            else
            {   // All the remaining words can be written to the current sector
                // so assign all of the length to be programmed.
                ProgramLength = WordsToWrite;
                // There will be no words remaining.
                WordsToWrite  = 0;
            }
            
            // program the block by calling Flash API library           
            if( Flash_Program( ( uint16* )Adx, Data, ProgramLength, &ProgStatus ) 
                                                              != STATUS_SUCCESS ) 
            {
                return ( eSTATUS_ERR );
            }
            
            // Do an additional check on the recently programmed data
            if( Flash_Verify( ( uint16* )Adx, Data, ProgramLength, &ProgStatus )
                                                             != STATUS_SUCCESS )
            {
                return ( eSTATUS_ERR );
            }           
          
            // Add the length offset to the address and data pointers so they 
            // point to the next location to write
            Adx  += ProgramLength;
            Data += ProgramLength;
        }
    } // while( WordsToWrite != 0 )

    return( eSTATUS_OK );
}                             

/*******************************************************************************

FUNCTION NAME: 
tuiSTATUS FLASHDRV_fnEraseRange( uint32 StartAdx,
                                 uint32 EndAdx )
                                   
PURPOSE: 
    To allow an application to clear/erase a range of code without knowing the
    sector information of the device.  
    
INPUTS: 
    uint32 StartAdx     - The start address of the range.
    uint32 EndAdx       - The last address in the range.
     
OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)
    
VERSION HISTORY:
Version: 0.01  Date: Mar 12, 2003  By: Alan Travelbea
    - Created.

Version: 1.00  Date: JUL-04-2003  By: Alan Travelbea
    - Added external flash support.
    - Instance checking changed from run-time to an assert.
    
Version: 2.00  Date: OCT-26-2004  By: Baldeesh Khaira
    - Modified for c28x processor, changed to include TI flash library
      functions
Version: 2.01  Date: FEB-28-2004  By: Baldeesh S. Khaira
   - Modified so that we pass all the sectors that we want to erase at once
     to the API   
Version: 2.02  Date: MAR-30-2004  By: Baldeesh S. Khaira
   - Modified so that we pass all the sectors that we want to erase at once
     to the API   
Version: 2.03  Date: MAR-30-2004  By: Baldeesh S. Khaira
   - Added initialization of uiSectorMask 
       
*******************************************************************************/
tuiSTATUS FLASHDRV_fnEraseRange( uint32 StartAdx,
                                 uint32 EndAdx )
{
    FLASHDRV_tzSECTORINFO  SectorInfo; // Sector Information
    FLASH_ST EraseStatus;       // return status for library API calls
    uint16   uiSectorMask = 0;

    // Only run the function if the ctor was called
    assert( FlashDriverInstance == eFLASHDRV_DRIVER_INSTANTIATED );

    // Erase until we have past the end address
    while( EndAdx >= StartAdx )
    {
        // Determine the sector of the destination address in flash
        if( FLASHDRV_fnDetermineSector( StartAdx, 
                                     &SectorInfo ) 
                                     != eSTATUS_OK )
        {   
            // The address that was requested was not defined as valid flash
            //  section.
            errno = FLASHDRV_eERRNO_ILLEGAL_ADX;
            return( eSTATUS_ERR );
        }
#ifdef CONFIG_EXTFLASHDRV_SECONDARY_MEM_EN
        else if( SectorInfo.uiNumber & FLASHDRV_EXTERNAL_BIT_MSK )
        {
            // Erase External Flash
            //
            // This address is not located in Internal flash so the external
            // flash driver is called.
            return EXTFLASHDRV_fnEraseRange( StartAdx,
                                             EndAdx );
        }
#endif
        else if(( 1 << SectorInfo.uiNumber ) & FLASHDRV_SECURE_SECTORS )
        {
            // This sector is marked as protected so do not perform clear and
            // return with an error.
            errno = FLASHDRV_eERRNO_PROT_SECTOR;
            return( eSTATUS_ERR );
        }
        else
        {
            // add this sector to the one to erase
            uiSectorMask |= 1 << SectorInfo.uiNumber;

            // Move the start marker to the end of the last clear/erase cycle
            // then increment it to equal the next address in the sequence.
            StartAdx = SectorInfo.ulEnd;
            ++StartAdx;
        }
    }

    // Attempt to erase the sectors.
    if( Flash_Erase( uiSectorMask, &EraseStatus ) != STATUS_SUCCESS ) 
    {
       return ( eSTATUS_ERR );
    }

    // erase was successfull
    return( eSTATUS_OK );
}


/*******************************************************************************

FUNCTION NAME: 
    tuiSTATUS FLASHDRV_fnDetermineSector(   uint32 Adx,     
                                            FLASHDRV_tzSECTORINFO *SectorInfo )  
                                   
PURPOSE: 
    Used to determine in which flash sector an address is located in.

INPUTS: 
    uint32 Adx      - Address in unknown sector
     
OUTPUTS:
    FLASHDRV_tzSECTORINFO *SectorInfo     
                    - Returned with the details of the sector in which the 
                      above address is located.
    tuiSTATUS       - Status code (either OK or an error code)
    
NOTES:
    
VERSION HISTORY:
Version: 0.01  Date: Mar 7, 2003  By: Alan Travelbea
    - Created.

Version: 1.00  Date: JUL-04-2003  By: Alan Travelbea
    - Added external flash support.
    
Version: 2.00  Date: OCT-26-2004  By: Baldeesh Khaira
    - New version for TI c28x DSP
    
*******************************************************************************/
tuiSTATUS FLASHDRV_fnDetermineSector( uint32 Adx, 
                                      FLASHDRV_tzSECTORINFO *SectorInfo )
{
    uint32 SectorStart;
    uint32 SectorEnd;
    uchar8 Counter = 0;
        
    // Search each entry in the sector map to find where the address is located.
    while ( Counter < FLASHDRV_NUM_OF_SECTORS )
    {
        SectorStart = FLASHDRV_SectorMap[ Counter ][0];
        SectorEnd   = FLASHDRV_SectorMap[ Counter ][1];
        
        // If SectorStartAdx <= Adx < SectorEndAdx, the address is in this 
        // sector.  Set the sector parameter and return with no error.
        if(( (uint32)SectorStart <= Adx ) && ( Adx <= (uint32)SectorEnd ))
        {
            SectorInfo->uiNumber = Counter;
            SectorInfo->ulStart  = (uint32)SectorStart;
            SectorInfo->ulEnd    = (uint32)SectorEnd;
            
            return( eSTATUS_OK );
        }   
        // The address was not located in this sector.  Try the next one.
        ++Counter;
    }

#ifdef CONFIG_EXTFLASHDRV_SECONDARY_MEM_EN
    // Check External Flash
    //
    // Check if this address is located in external flash.  If it isn't,
    // the external flash drv will take care of setting the illegal
    // address error.
    return( EXTFLASHDRV_fnDetermineSector( Adx, 
                                           SectorInfo ));
                                           
#else   
    // The address was not located inside any range described by the sector map.  
    // Return an error code signalling the Sector parameter is invalid.
    return( eSTATUS_ERR );
#endif
}

/*******************************************************************************

FUNCTION NAME: 
    FLASHDRV_fnInstallCb 
                                   
PURPOSE: 
    Install the callback function for Flash API to handle communication messages 
    at safe times during Flash erase, program and verify 

INPUTS: 
    FLASHDRV_tpfnCALLBACK - pointer to callback function
     
OUTPUTS:
    eSTATUS_OK if everything is OK.
    eSTATUS_ERR otherwise
    
NOTES:
    
VERSION HISTORY:

Version: 0.01  Date: NOV-24-05  By: Holly Zhou
    - Created.
    
******************************************************************************/
tuiSTATUS FLASHDRV_fnInstallCb(  FLASHDRV_tpfnCALLBACK tpfnFunc )
{
    if( tpfnFunc == NULL )
    {
        assert( FALSE );
        return ( eSTATUS_ERR );
    }    

    // specify the callback function pointer
    Flash_CallbackPtr = tpfnFunc;
    
    return ( eSTATUS_OK );
}    
     
/*******************************************************************************

FUNCTION NAME: 
void FLASHDRV_fnLock( void ) 
                                   
PURPOSE: 
    Lock the processor so that it cannot be erased, programmed, or read

INPUTS: 
    none
     
OUTPUTS:
    none
    
NOTES:
    
VERSION HISTORY:

Version: 1.00  Date: JUN-12-2006  By: Trevor Monk
    - Created.

******************************************************************************/

void FLASHDRV_fnLock( void )
{

    // Allow writes to the code security module control register
    DEVICE_mACCESS_EN();

    // force secure mode    
    CsmRegs.CSMSCR.bit.FORCESEC = 1;

    // Disallow any modifications to the registers
    DEVICE_mACCESS_DIS();
}

/*******************************************************************************

FUNCTION NAME: 
tuiSTATUS flashdrv_fnUnlock( void ) 
                                   
PURPOSE: 
    Unlock the processor so that it can be erased or programmed

INPUTS: 
    none
     
OUTPUTS:
    tuiSTATUS       - Status code (either OK or an error code)
    
NOTES:
    
VERSION HISTORY:
Version: 0.01  Date: NOV-05-04  By: Baldeesh S. Khaira
    - Created.
Version: 1.00  Date: JAN-23-04  By: Baldeesh S. Khaira
    - Changed EALLOW and EDIS to macros defined in devicemacro.h.
******************************************************************************/
/* Currently not used, but do not delete
tuiSTATUS flashdrv_fnUnlock( void )
{
    volatile uint16 DummyRead;              // temporary variable to read pswd
    volatile uint16 * PswdLocation = &CsmPwl.PSWD0;  //  first password location
    
    // loop through all the password location to do a dummy read of each 
    // password;  this step is necessary to unlock the flash, even though
    // we are not interested in the actual value of the read    
    while( PswdLocation <= &CsmPwl.PSWD7 )
    {
        DummyRead = *PswdLocation++;
    }
    
    
    // Allow writes to the code security module key registers
    DEVICE_mACCESS_EN();

    // Assign the password locations to the keys to unlock the registers
    CsmRegs.KEY0 = CsmPwl.PSWD0;
    CsmRegs.KEY1 = CsmPwl.PSWD1;
    CsmRegs.KEY2 = CsmPwl.PSWD2;
    CsmRegs.KEY3 = CsmPwl.PSWD3;
    CsmRegs.KEY4 = CsmPwl.PSWD4;
    CsmRegs.KEY5 = CsmPwl.PSWD5;
    CsmRegs.KEY6 = CsmPwl.PSWD6;
    CsmRegs.KEY7 = CsmPwl.PSWD7;

    // Disallow any modifications to the registers
    DEVICE_mACCESS_DIS();

    // If the CSM unlocked, return succes, otherwise return
    // failure.
    if ( CsmRegs.CSMSCR.bit.SECURE != 0) 
    {
        return ( eSTATUS_ERR );
    }

    // return okay
    return ( eSTATUS_OK );
}
*/

