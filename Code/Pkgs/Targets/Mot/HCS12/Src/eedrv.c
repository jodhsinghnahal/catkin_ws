/*==============================================================================
Copyright © 2003 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  eedrv.c

PURPOSE:
    This file provides access to the on-chip eeprom on the hcs12.

FUNCTION(S):
    EEDRV_fnCtor        - Configure the EEPROM interface
    EEDRV_fnDtor        - Disables the EEPROM interface
    EEDRV_fnReadChar    - Reads a single character from the EEPROM
    EEDRV_fnWriteChar   - Writes a single character from the EEPROM
    EEDRV_fnReadPage    - Read data from the EEPROM
    EEDRV_fnIsBlank     - checks to see if EEPROM is blank
    EEDRV_fnWritePage   - Write data to the EEPROM
    EEDRV_fnIsReady     - checks to see if EEPROM is ready
    EEDRV_fnErase       - Erase sectors on EEPROM
    EEDRV_fnGetBaseAddr - Get the base address
    
    local:
      eedrv_fnWriteSector - writes a single sector of eeprom
      eedrv_fnSendCommand - sends a command to the eeprom hardware module
      eedrv_fnMemcpy      - Copies memory (used instead of a memcpy call
                            which is in an unknown physical location)
NOTES:   
    The address input by the user is not an abosulte address for the EEPROM.
    The user should input an address in the range of 0 < Adx < EEPROM_MEM_SIZE.
    
    IMPORTANT: Asserts are used in this file and if they are located inside the
    address range of the EEPROM, any triggered will cause unknown behaviour of
    the code while the EEPROM is in.  Ensure that assert text is located away
    from EEPROM.

CHANGE HISTORY :
$Log: eedrv.c $

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/2
    User: DaleM     Date: 02/15/05  Time: 06:20PM
    Error names changed
    
    
    *****************  Version 10  *****************
    User: Alant        Date: 17/09/03   Time: 9:41a
    Updated in $/PD/Common/FieldReprogram/Loader/Code/Targets/Standalone/Motorola/HCS12/DG256/HAL/Src
    - Changed memcpy calls to local eedrv_fnMemcpy calls to ensure that the
    function is outside of the address range of the EEPROM.
    
    *****************  Version 9  *****************
    User: Alant        Date: 6/09/03    Time: 5:46p
    Updated in $/PD/Common/FieldReprogram/Loader/Code/Targets/Multi_Master/Motorola/HCS12/DG256/HAL/Src
    - Combined asserts to reduce code size when debugging.
    
    *****************  Version 8  *****************
    User: Alant        Date: 30/07/03   Time: 5:22p
    Updated in $/PD/Common/FieldReprogram/Loader/Test/XanBus/Motorola/HCS12
    - Readded fnGetBaseAddr.
    - Added CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV define to conditionally
    include turning on and off the EEPROM inside the driver.
    - Added CONFIG_EEDRV_DECLARE_CODE_SECTION define to allow the EEDRV
    routines to be all located together in a seperate code section.
    
    *****************  Version 6  *****************
    User: Baldeeshk    Date: 28/07/03   Time: 3:22p
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Src
    Changes as part of code review.  Addesses are now dealt with as an
    offset, not an absolute address.  Also, added assert statements to
    check if EE is turned on, added error code to fnErase, and added
    additional comments.
    
    *****************  Version 5  *****************
    User: Baldeeshk    Date: 23/07/03   Time: 10:03a
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Src
    Removed define for EEDRV_ERASE_ALL as its already in eedevice.h
    
    *****************  Version 4  *****************
    User: Baldeeshk    Date: 22/07/03   Time: 11:54a
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Src
    Modified include from hwcfg.h to config.h.  Changed assert location in
    fnErase function.
    
    *****************  Version 3  *****************
    User: Baldeeshk    Date: 9/07/03    Time: 9:32a
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Src
    Removed device instantiation check
    
    *****************  Version 2  *****************
    User: Baldeeshk    Date: 27/06/03   Time: 5:07p
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Src
    Implemented fnIsReady, fnIsBlank, fnReadChar, fnWriteChar.  Created
    local function fnSendCommand.  Redesigned fnWritePage, fnReadPage,
    fnErase functions.
    
    *****************  Version 1  *****************
    User: Baldeeshk    Date: 23/06/03   Time: 1:00p
    Created in $/PD/Network_Extensions/Code/Common/HAL/Src
    Initial checkin for EEPROM driver for Motorola HCS12
    

==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include "string.h"  // for memcopy
#include "eedrv.h"   // for defines 
#include "config.h"  // hardware configuration settings
#include "errno.h"   // to write errors
#include "xassert.h" // for assert statements

/*==============================================================================
                              Defines
==============================================================================*/

// EEPROM Commands
#define EEDRV_MASS_ERASE  0x41
#define EEDRV_ERASE_VER   0x05
#define EEDRV_WORD_PROG   0x20
#define EEDRV_ERASE       0x40
#define EEDRV_MODIFY      0x60

// EEPROM extended divisor bit mask -- hardware
#define EEDRV_EDIV8           0x40
#define EEDRV_EEON            1
#define EEDRV_EEOFF           0
#define EEDRV_ADDR_MSK        0xF0
#define EEDRV_HIGH_BYTE_SHIFT 8

// Divide Oscillator clock rate and Bus clock rate by 1000 to use in kiloHertz
// for calculation purposes
#define OSCCLK_KHZ      ( CONFIG_OSCCLK / 1000 )
#define BUSCLK_KHZ      ( CONFIG_BUSCLK / 1000 )
                                   

/*
    Macro that generates the EEPROM clock precaler as per data book.
    If the crystal frequency is above 12.8MHz then the EDIV bit must
    be set, this divides the OSCCLK frequency by 8 before the prescaler.
*/

// If oscillator is greater than 12.8 MHz
#if (OSCCLK_KHZ > 12800)

/* This macro calculates the prescaler, but also implements the EDIV8 bit */
#if ((OSCCLK_KHZ * (BUSCLK_KHZ + 200)) % (1600 * BUSCLK_KHZ) == 0)
#define EEDRVCLK_PRESCALER ((OSCCLK_KHZ * (BUSCLK_KHZ + 200) /   \
                    (1600 * BUSCLK_KHZ)) + NVR_EDIV8 - 1)
#else
#define EEDRVCLK_PRESCALER ((OSCCLK_KHZ * (BUSCLK_KHZ + 200) /   \
                        (1600 * BUSCLK_KHZ)) + NVR_EDIV8)
#endif /* OSCCLK_FREQ_KHZ * (BUSCLK_FREQ_KHZ + 200) % 
                                   (1600 * BUSCLK_FREQ_KHZ) == 0 */

/* Make sure EEDRVCLK is within specified range. */
#define EEDRVCLK_KHZ (OSCCLK_KHZ / (8 * (1 + EEDRVCLK_PRESCALER - EDIV8)))
#if ( ( EEDRVCLK_KHZ < 150 ) || ( EEDRVCLK_KHZ > 200 ) ||                   \
                    ( EEDRVCLK_PRESCALER > 0x7F ) )
#error "EEPROM prescaler or clock out of range."
#endif /* Incorrect EEDRVCLK frequency. */

#else

// This macro calculates the prescaler. OSC <= 12.8MHz
#if ( ( OSCCLK_KHZ *  (BUSCLK_KHZ + 200 ) ) % ( 200 * BUSCLK_KHZ ) == 0 )
#define EEDRVCLK_PRESCALER ( ( OSCCLK_KHZ * ( BUSCLK_KHZ + 200 ) /       \
                                 ( 200 * BUSCLK_KHZ ) ) - 1 )
#else
#define EEDRVCLK_PRESCALER ( OSCCLK_KHZ * ( BUSCLK_KHZ + 200 ) /         \
                                     ( 200 * BUSCLK_KHZ ) )
#endif

/* Make sure ECLK is within specified range. */
#define EEDRVCLK_KHZ ( OSCCLK_KHZ / ( 1 + EEDRVCLK_PRESCALER ) )
#if ( ( EEDRVCLK_KHZ < 150 ) || ( EEDRVCLK_KHZ > 200)  ||                   \
                     ( EEDRVCLK_PRESCALER > 0x3F ) )
#error "EEPROM prescaler or clock out of range."
#endif /* Incorrect EEDRVCLK frequency. */

#endif /* OSCCLK_FREQ_KHZ > 12800 */


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

#pragma DATA_SEG EEPROM_DATA

// This is the NV data -- should be located in EEPROM_DATA section by linker
uchar8 aucEepromData[ EEDRV_MEM_SIZE ];

#pragma DATA_SEG DEFAULT

/*==============================================================================
                          Local Function Prototypes
==============================================================================*/
// writes a single sector of data
static tuiSTATUS eedrv_fnWriteSector( uint16* puiAddress,
                                      uint8 * pucSectorData );
                               
// sends a command to the eeprom hardware module
static tuiSTATUS eedrv_fnSendCommand( uchar8 ucCommand,
                                      uint16 * Adx,
                                      uint16 uiData );                               

// Copies memory (used instead of a memcpy call which is in an unknown physical 
// location)
void eedrv_fnMemcpy( void *Dest, 
                     void *Source, 
                     uchar8 Count );

 
/*==============================================================================
                           Function Definitions
==============================================================================*/
#ifdef CONFIG_EEDRV_DECLARE_CODE_SECTION
// Move EEDRV Code to Special Section and Disable Section Redeclaration Warning
//
// Declare the EEDRV code to be in a special section so we can control where it
// is placed.  The Warning is disabled because the .h file declares the code to
// be in the default section.  This pragma will cause the code to be reassigned, 
// but will produce a warning which we can ignore.
#pragma CODE_SEG EEDRV_CODE_SECTION
#pragma MESSAGE WARNING DISABLE C4200
#endif

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnCtor
  
PURPOSE: 
    This function is used to initialize the non-volatile memory.  

INPUTS:
    none

OUTPUTS:
    eSTATUS_OK - initialized correctly

NOTES:
    The EEPROM data is segmented above and the starting location is determined
    by finding the pointer to the first piece of data.

VERSION HISTORY:

Version: 1.00  Date: 04/04/03  By: Dalem
    - Created.
Version: 1.01  Date: 04/10/03  By: Dalem
    - Set NVRam starting address (hardware)
Version: 1.02  Date: 06/18/03  By: BaldeeshK
    - New version for eedrv.c, borrowed from old nvram.c, added return
      status 
Version: 1.03  Date: 06/27/03  By: BaldeeshK
    - Added instantiation check and assignment
Version: 1.04  Date: 07/18/03  By: BaldeeshK
    - Removed instantiation check and assignment               
Version: 1.10  Date: JUL-29-2003  By: Alan Travelbea
    - Added conditionally defined EEPROM off code.

*******************************************************************************/

tuiSTATUS EEDRV_fnCtor( void )
{
    uint16 uiNVAddr;
    uchar8 ucNVPosition;

    // If EEPROM clock divider not loaded yet
    if ( ECLKDIV == 0 )
    {
        // EEPROM clock divider
        ECLKDIV = ( uchar8 )EEDRVCLK_PRESCALER;
    }

    // Get the NVRam address as a word (any address in NVRam space should do)
    uiNVAddr = ( uint16 )aucEepromData;

    // Use the upper 4 bits of the NVRam address as NV position byte
    ucNVPosition = ( uchar8 )( uiNVAddr >> EEDRV_HIGH_BYTE_SHIFT )
                                             & EEDRV_ADDR_MSK;

    // Set NV position with EEON bit turned on
    INITEE = ucNVPosition | EEDRV_EEON;

#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn Off EEPROM
    INITEE_EEON = EEDRV_EEOFF;
#endif
        
    // return ok
    return ( eSTATUS_OK );
}

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnDtor
  
PURPOSE: 
    This function is used to disable the interface to non-volatile memory  

INPUTS:
    none

OUTPUTS:
    eSTATUS_OK - disabled correctly

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 06/26/03  By: Baldeeshk
    - Created.

*******************************************************************************/
tuiSTATUS EEDRV_fnDtor( void )
{

    // turn of the eeprom by negating the turn on value
    INITEE_EEON = EEDRV_EEOFF;
    
    // return ok
    return ( eSTATUS_OK );
}



/*******************************************************************************

FUNCTION NAME: 
    eedrv_fneedrv_fnMemcpy
  
PURPOSE: 
    This function is used to copy data in a similar fashion to the ANSI memcpy.
    It is located here so that the application can be assured that the copy 
    call will not jump to an address inside the physical location of EEPROM.

INPUTS:
    void *Dest   - Location data will be copied to.
    void *Source - Location data will be copied from.
    uchar8 Count - Number of byte to be copied.

OUTPUTS:
    None.

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 06/26/03  By: Baldeeshk
    - Created.

*******************************************************************************/
void eedrv_fnMemcpy( void *Dest, void *Source, uchar8 Count )
{
    while( Count-- )
    {
        *( (uchar8 *)Dest )++ = *( (const uchar8 *)Source )++;
    } 
}



/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnGetBaseAddress
  
PURPOSE: 
    This function returns a pointer to beginning address of the EEPROM.

INPUTS:
    none

OUTPUTS:
    Pointer to first location of EEPROM

NOTES:
    Get base address by returning pointer to first location in memory.

VERSION HISTORY:

Version: 1.00  Date: 06/18/03  By: BaldeeshK
    - Created.

*******************************************************************************/
uint16 EEDRV_fnGetBaseAddr( void )
{
   return ( ( uint16 )aucEepromData );
}


/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnReadChar
  
PURPOSE: 
    This function reads a single character from the non-volatile memory

INPUTS:
    Adx - Address in EEPROM from which we want to read
    Data - pointer to where data is to be stored

OUTPUTS:
    eSTATUS_ERR if read was unsuccessful
    eSTATUS_OK  if read was successful

NOTES:
    The address input by the user is not an abosulte address for the EEPROM.
    The user should input an address in the range of 0 < Adx < EEPROM_MEM_SIZE.

VERSION HISTORY:

Version: 1.00  Date: 06/26/03  By: BaldeeshK
    - Created
Version: 1.01  Date: 07/28/03  By: BaldeeshK
    - Modified address so that only an offset is input.
Version: 1.10  Date: JUL-29-2003  By: Alan Travelbea
    - Added conditionally defined EEPROM on/off code.

*******************************************************************************/

tuiSTATUS EEDRV_fnReadChar( uint16 Adx,
                            void *Data )
{
#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn On EEPROM
    INITEE_EEON = EEDRV_EEON;
#endif

    // Make sure that the eeprom is turned on, that Data buffer is not NULL,
    // and that the address is valid.
    assert(( INITEE_EEON == EEDRV_EEON )
            && ( Data != 0 )
            && ( Adx < EEDRV_MEM_SIZE ));
    
    // read the data
    *( ( uchar8*)( Data ) ) = aucEepromData[ Adx ];

#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn Off EEPROM
    INITEE_EEON = EEDRV_EEOFF;
#endif
    
    // return ok
    return ( eSTATUS_OK );

}


/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnWriteChar
  
PURPOSE: 
    This function writes a single character to the non-volatile memory

INPUTS:
    Adx - Address in EEPROM from which we want to write
    Data - pointer to character data that is to be stored

OUTPUTS:
    eSTATUS_ERR if an error occurred
    eSTATUS_OK is write was ok

NOTES:
    The address input by the user is not an abosulte address for the EEPROM.
    The user should input an address in the range of 0 < Adx < EEPROM_MEM_SIZE.

VERSION HISTORY:

Version: 1.00  Date: 06/26/03  By: BaldeeshK
    - Created.
Version: 1.01  Date: 07/28/03  By: BaldeeshK
    - Modified address so that only an offset is input    
Version: 1.10  Date: JUL-29-2003  By: Alan Travelbea
    - Added conditionally defined EEPROM on/off code.
Version: 1.11  Date: SEP-17-2003  By: Alan Travelbea
    - Changed memcpy call to local function to prevent a call into the ON EEPROM
      memory space.
      
*******************************************************************************/
tuiSTATUS EEDRV_fnWriteChar( uint16 Adx,
                             uchar8 Data )
{
    uint16 * Address;                   // sector boundary address
    uchar8 Sector[ EEDRV_SECTOR_SIZE ]; // buffer to send to write sector
    tuiSTATUS tuiStatus;                      // err variable
    

#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn On EEPROM
    INITEE_EEON = EEDRV_EEON;
#endif

    // Make sure that the eeprom is turned on, that Data buffer is not NULL,
    // and that the address is valid.
    assert(( INITEE_EEON == EEDRV_EEON )
            && ( Data != 0 )
            && ( Adx < EEDRV_MEM_SIZE ));
    
    // cast the starting address of the eeprom 
    Address = ( uint16* )( ( Adx + ( uint16 )aucEepromData ) &
                          ~( EEDRV_SECTOR_SIZE - 1 ) );
    
    // preserve old data
    ( void ) eedrv_fnMemcpy( ( void* )Sector, 
                             ( void* )Address, 
                             EEDRV_SECTOR_SIZE );

    // add new data into sector
    Sector[ Adx & ( EEDRV_SECTOR_SIZE - 1 ) ] = Data;
    
    // write sector
    if( eedrv_fnWriteSector( Address, Sector ) != eSTATUS_OK )
    {
        mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_WRITE );    
    }

#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn Off EEPROM
    INITEE_EEON = EEDRV_EEOFF;
#endif
    
    return ( tuiStatus );    
} 



/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnReadPage
  
PURPOSE: 
    This function reads data from the eeprom

INPUTS:
    Adx - Address in EEPROM from which we want to read
    Data - pointer to data buffer where data is to be stored
    Length - how many bytes to read

OUTPUTS:
    eSTATUS_ERR if read was unsuccessful
    eSTATUS_OK  if read was successful

NOTES:
    The address input by the user is not an abosulte address for the EEPROM.
    The user should input an address in the range of 0 < Adx < EEPROM_MEM_SIZE.

VERSION HISTORY:

Version: 1.00  Date: 06/18/03  By: BaldeeshK
    - Created.
Version: 1.01  Date: 07/28/03  By: BaldeeshK
    - Modified address so that only an offset is input    
Version: 1.10  Date: JUL-29-2003  By: Alan Travelbea
    - Added conditionally defined EEPROM on/off code.

*******************************************************************************/
tuiSTATUS EEDRV_fnReadPage( uint16 Adx,
                             void *Data, 
                             size_t Length )
{
 
#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn On EEPROM
    INITEE_EEON = EEDRV_EEON;
#endif

    // Make sure that the eeprom is turned on, that Data buffer is not NULL,
    // and that the address is valid.
    assert(( INITEE_EEON == EEDRV_EEON )
            && ( Data != 0 )
            && ( Adx < EEDRV_MEM_SIZE ));
   
    // copy data into user buffer  
    while( Length -- )
    {
        *( ( uchar8* )Data )++ = aucEepromData[ Adx++ ];
    }

#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn Off EEPROM
    INITEE_EEON = EEDRV_EEOFF;
#endif
  
    return ( eSTATUS_OK );

} 



/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnIsBlank
  
PURPOSE: 
    This function verifies whether the EEPROM is blank, ie fully erased or not

INPUTS:
    none

OUTPUTS:
    TRUE - EEPROM is erased
    FALSE - EEPROM is not erased

NOTES:
   Uses hardware ERASE_VERIFY command

VERSION HISTORY:

Version: 1.00  Date: 06/26/03  By: BaldeeshK
    - Created.
Version: 1.10  Date: JUL-29-2003  By: Alan Travelbea
    - Added conditionally defined EEPROM on/off code.

*******************************************************************************/
tucBOOL EEDRV_fnIsBlank( void )
{
    tucBOOL Result = FALSE;
    
#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn On EEPROM
    INITEE_EEON = EEDRV_EEON;
#endif

    // send erase verify command   
    if(( eedrv_fnSendCommand( EEDRV_ERASE_VER,
                              (uint16 *)aucEepromData,
                              0 ) 
                              == eSTATUS_OK )
            && ( ESTAT_BLANK == 1 ))
    {
        Result = TRUE;
    }
                         
#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn Off EEPROM
    INITEE_EEON = EEDRV_EEOFF;
#endif
    
    return( Result );                         
}
                           

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnWritePage
  
PURPOSE: 
    This function writes a user-supplied number of bytes to EEPROM.

INPUTS:
    Adx - Address in EEPROM from which we want to write
    Data - pointer to data buffer where data is to be stored
    Length - how many bytes to write

OUTPUTS:
    eSTATUS_ERR if an error occurred
    eSTATUS_OK is write was ok

NOTES:
    Within the loop, the program checks to see if it needs to preserve the
    old data.  If it does, it reads it accordingly and overwrites the parts
    that need overwriting.
    
    The address input by the user is not an abosulte address for the EEPROM.
    The user should input an address in the range of 0 < Adx < EEPROM_MEM_SIZE.    

VERSION HISTORY:

Version: 1.00  Date: 06/18/03  By: BaldeeshK
    - Created.
Version: 1.01  Date: 06/27/03  By: BaldeeshK
    - Changed design to make code more compact
Version: 1.02  Date: 07/28/03  By: BaldeeshK
    - Modified address so that only an offset is input        
Version: 1.10  Date: JUL-29-2003  By: Alan Travelbea
    - Added conditionally defined EEPROM on/off code.
Version: 1.11  Date: SEP-17-2003  By: Alan Travelbea
    - Changed memcpy call to local function to prevent a call into the ON EEPROM
      memory space.

*******************************************************************************/

tuiSTATUS EEDRV_fnWritePage( uint16 Adx, 
                             void *Data,
                             size_t Length )
{
   
    uchar8 Sector[ EEDRV_SECTOR_SIZE ]; // buffer to send to write sector fn
    uint16 *SectorStartPtr;             // pointer to beginning of the sector
    uint16 SectorOffset;                // offset of sector to which we write
    uint16 OvrWriteDataLen;             // length of data we write in each sec
    tuiSTATUS tuiStatus = eSTATUS_OK;   // return error


#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn On EEPROM
    INITEE_EEON = EEDRV_EEON;
#endif

    // Make sure that the eeprom is turned on, that Data buffer is not NULL,
    // and that the address is valid.
    assert(( INITEE_EEON == EEDRV_EEON )
            && ( Data != 0 )
            && ( Adx < EEDRV_MEM_SIZE ));

    
    // while we still have data to write
    while( Length )
    {
        // get the pointer of the beginning of the sector
        SectorStartPtr = ( uint16* )( ( uint16 )aucEepromData   +
                         ( Adx & ( ~( EEDRV_SECTOR_SIZE - 1 ) ) ) );

        // get the position in the sector where we start overwriting
        // the user data
        SectorOffset = Adx & ( EEDRV_SECTOR_SIZE - 1 );
        
        // if there not enough room for the data in this sector
        if( ( EEDRV_SECTOR_SIZE - SectorOffset) <= Length )
        {
            // the length how much room is left in the sector
            OvrWriteDataLen = EEDRV_SECTOR_SIZE - SectorOffset;
        }

        else
        {
            // the length is how much data is left
            OvrWriteDataLen = Length;
        }
        
        // if data is not the whole sector, preserve old data
        if( OvrWriteDataLen < EEDRV_SECTOR_SIZE )
        {
            // read old data
            ( void )eedrv_fnMemcpy( ( void* )Sector, 
                                    SectorStartPtr, 
                                    EEDRV_SECTOR_SIZE );            

        }
        
        // overwrite new data into sector 
        ( void )eedrv_fnMemcpy( ( void* )( (uint16)Sector + SectorOffset), 
                                Data, 
                                (uchar8)OvrWriteDataLen );

        // write the sector
        if( eedrv_fnWriteSector( SectorStartPtr, Sector ) != eSTATUS_OK )
        { 
            mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_WRITE );
            break;
        }
        
        // increment pointer to the data
        ( uchar8* )Data += OvrWriteDataLen;
        // decrement the length
        Length -= OvrWriteDataLen;
        // increment the destination address location
        Adx += OvrWriteDataLen; 

    }

#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn Off EEPROM
    INITEE_EEON = EEDRV_EEOFF;
#endif
    
    // return status
    return( tuiStatus );
 
}

                          

/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnIsReady
  
PURPOSE: 
    This function determines if the EEPROM is ready to be accessed

INPUTS:
    
OUTPUTS:
    TRUE - EEPROM is ready
    FALSE - EEPROM is not ready

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 06/26/03  By: BaldeeshK
    - Created.
Version: 1.01  Date: 07/28/03  By: BaldeeshK
    - Added check on INITEE_EEON bit    

*******************************************************************************/
tucBOOL EEDRV_fnIsReady( void )
{
    tuiSTATUS tuiStatus;
    
    // check to see if its on
    if( INITEE_EEON == EEDRV_EEON )
    {
        // check cbeif flag to see if it is ready
        if( ESTAT_CBEIF == 1 )
        {
           return ( TRUE );
        }
    }

    mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_BUSY );
    return ( FALSE );
}


/*******************************************************************************

FUNCTION NAME: 
    EEDRV_fnErase
  
PURPOSE: 
    This function erases a user supplied amount of sectors

INPUTS:
    Adx is address if first sector to erase.
    Length is number of bytes to erase
    Opt - 0 = Simple erase of Length bytes
          1 = Erase whole eeprom

OUTPUTS:
    eSTATUS_ERR if there is an error
    eSTATUS_OK if erase succeeded

NOTES:
    If the user doesn't supply an address on a sector boundary, the function 
    will convert address so that it will be.  Only whole sectors will be 
    erased, and if any part of the sector that does not lie on the length
    of data specified the user will also be erased.

    The address input by the user is not an abosulte address for the EEPROM.
    The user should input an address in the range of 0 < Adx < EEPROM_MEM_SIZE.    

VERSION HISTORY:

Version: 1.00  Date: 06/18/03  By: BaldeeshK
    - Created.
Version: 1.01  Date: 06/26/03  By: BaldeeshK
    - Used the functino eedrv_fnSendCommand to deal with sending actual 
      commands to the hardware  
Version: 1.02  Date: 07/16/03  By: BaldeeshK
    - Moved assert checks to only non-mass erase area
Version: 1.03  Date: 07/28/03  By: BaldeeshK
    - Modified address so that only an offset is input, added error code           
Version: 1.10  Date: JUL-29-2003  By: Alan Travelbea
    - Added conditionally defined EEPROM on/off code.

*******************************************************************************/

tuiSTATUS EEDRV_fnErase( uint16 Adx,
                         size_t Length,
                         uint8  Opt )
{
    uchar8 EraseCommand;     // mass erase or simple erase
    uchar8 NumSectors;       // how many sectors to erase
    uint16 * Address;        // eeprom address
    tuiSTATUS tuiStatus = eSTATUS_OK; // status variable
  
  
#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn On EEPROM
    INITEE_EEON = EEDRV_EEON;
#endif

    assert( INITEE_EEON == EEDRV_EEON );
    
    // if it is a mass erase
    if( Opt == EEDRV_ERASE_ALL )
    {
        Address = ( uint16* )aucEepromData;
        NumSectors = 1;                    // provide max count value
        EraseCommand = EEDRV_MASS_ERASE;  // set command to mass erase
    }
    else
    {
        // check to see if Address is valid
        assert( Adx < EEDRV_MEM_SIZE );

        // convert address to sector boundary
        Address = ( uint16* )( ( uint16 )aucEepromData + 
                               Adx & ~( EEDRV_SECTOR_SIZE - 1 ) );
     
        // calculate number of sectors
        NumSectors = ( uchar8 )( ( Length + EEDRV_SECTOR_SIZE - 1 ) 
                                              / EEDRV_SECTOR_SIZE ); 
        // increment number of sectors if it does not lie on sector boundary
	    if( ( Adx & ( EEDRV_SECTOR_SIZE - 1 ) ) && 
	      ( ( Adx & ( EEDRV_SECTOR_SIZE - 1 ) ) + Length > EEDRV_SECTOR_SIZE ) )
        {
	        NumSectors++;
	    }

        EraseCommand = EEDRV_ERASE;
	} 

    // loop through number of sectors
    while( NumSectors-- )
    {
        if( eedrv_fnSendCommand( EraseCommand, Address, NumSectors ) != 
                                                            eSTATUS_OK )
        {   
           mWRITE_STATUS_AND_ERRNO( EEDRV_eERRNO_ADX_COMMAND );
           break;
        }
        
        ( uchar8* )Address += EEDRV_SECTOR_SIZE;
                 
    }
    
    // wait for the last command to finish executing
    while( ESTAT_CCIF != 1 )
    {
    }

#ifdef CONFIG_EEDRV_EEPROM_ON_ONLY_IN_DRV
    // Turn Off EEPROM
    INITEE_EEON = EEDRV_EEOFF;
#endif
    
    return ( tuiStatus );  
}

/*******************************************************************************

FUNCTION NAME: 
    eedrv_fnWriteSector
  
PURPOSE: 
    This function writes a sector (two words) of NVRam.

INPUTS:
    'puiSectorAddress' is word pointer to lower word of two word sector
    'pucSectorData' is word pointer of data to write

OUTPUTS:
    eSTATUS_ERR if write does not complete properly
    eSTATUS_OK if write succeeds

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 04/04/03  By: Dalem
    - Created.
Version: 1.01  Date: 04/10/03  By: Dalem
    - Parameters changed
    - Added check for data matching sector data
Version: 1.02  Date  06/16/03  By: BaldeeshK
    - New function for eedrv.c, taken from old nvram
Version: 1.03  Data  06/25/03  By: BaldeeshK
    - now calls fnSendCommand function        

*******************************************************************************/

static tuiSTATUS eedrv_fnWriteSector( uint16* puiSectorAddress,
                                      uint8 * pucSectorData )
{
    uint16 *DataAddress;
    tuiSTATUS Err;
    
    DataAddress = ( uint16 *)pucSectorData;

    // if the old data and new data is the same, don't bother writing
    if ( ( puiSectorAddress[ 0 ] == DataAddress[ 0 ] ) 
         && ( puiSectorAddress[ 1 ] == DataAddress[ 1 ] ) )
    {
         // Do not need to write this sector
         return ( eSTATUS_OK );
    }

    // send the first word ( 16-bits) out to write
    Err = eedrv_fnSendCommand( EEDRV_MODIFY,
                               puiSectorAddress++, 
                               DataAddress[ 0 ] );
    
    // check status
    if( Err != eSTATUS_OK )
    {
       return ( Err );
    }

    // send the second word ( 16-bits ) out to write
    Err = eedrv_fnSendCommand( EEDRV_WORD_PROG,
                               puiSectorAddress,
                               DataAddress[ 1 ] );

    // wait for the last command to finish executing
    while( ESTAT_CCIF != 1 )
    {
    }
    
    return ( Err );

}                                


/*******************************************************************************

FUNCTION NAME: 
    eedrv_fnSendCommand
  
PURPOSE: 
    This function sends a command to the EEPROM module.

INPUTS:
    ucCommand - Command to send to the module
    Address - Address that the command should operate on
    uiData - Data that corresponds to the address

OUTPUTS:
    eSTATUS_ERR if write does not complete properly
    eSTATUS_OK if write succeeds

NOTES:

VERSION HISTORY:

Version: 1.00  Date: 06/25/03  By: BaldeeshK
    - Created.

*******************************************************************************/
static tuiSTATUS eedrv_fnSendCommand( uchar8 ucCommand,
                                      uint16 * Adx,
                                      uint16 uiData )
{
    // clear the error flags by writing a 1 to the register bits
    ESTAT_ACCERR = 1;
    ESTAT_PVIOL = 1;
    
    // Wait until the command buffer is empty
    while ( ESTAT_CBEIF != 1 )
    {
    }
    
    // Set the NV data
    *Adx = uiData;

    // Set sector command
    ECMD = ucCommand;
    
    // Launch the command
    ESTAT_CBEIF = 1;
    
    // If the access error flag or protection violation flags are set
    if ( ( ESTAT_ACCERR == 1 ) || ( ESTAT_PVIOL == 1 ) )
    {
        return( eSTATUS_ERR );
    }

    // Wait for last command to be sent to the buffer
    while( ESTAT_CBEIF != 1 )
    {
    }
    
    return ( eSTATUS_OK );
}                               


#ifdef CONFIG_EEDRV_DECLARE_CODE_SECTION
// End EEDRV Code Section and Re-enable Section Warning
#pragma MESSAGE WARNING DEFAULT C4200
#pragma CODE_SEG DEFAULT
#endif