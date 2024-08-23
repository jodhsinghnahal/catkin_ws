/*=============================================================================
  (c) 2003 - 2019 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

FILE NAME:  xbtplatform.c

PURPOSE:
    This file contains functions for accessing platform specific data.

FUNCTION(S):
    XBTPLATFORM_fnCtor         - Initialize package
    XBTPLATFORM_fnLoadPrefAddr - Load Preferred Address
    XBTPLATFORM_fnLoadName     - Load node name
    XBTPLATFORM_fnLoadHwRev    - Load Hardware Revision data
    XBTPLATFORM_fnLoadLoaderVersion - Loads the Loader Version Number

    local:
        none

=============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/

#include <string.h>         // uses memcpy
#include <limits.h>         // uses UINT_MAX
#include "config.h"         // Global project configuration
#include "Device.h"         // uses device related macros.
#include "LoaderLayout.h"   // uses the locations of data in NVM.
#include "xbudefs.h"        // Xanbus defines.
#include "xbtgdefs.h"       // Xanbus defines.
#include "NvmDrv.h"         // Non volatile memory common interface

/*=============================================================================
                               Defines
=============================================================================*/
// fnLoadName Defines
//
#define XBTPTFRM_NAME_RESVD_LOC     ( 6 )   // The byte of the name that contains
                                            //   the reserved bit.
#define XBTPTFRM_NAME_RESVD_BIT     ( 0x01 )// The name reserved bit location.

// Defines for making up default node name
#define UNIQUE_NUMBER   ( 0x1FFFFFUL )      // Unique Number (serial number)
#define MANU_CODE       ( XBT_MFG_XANTREX ) // Manufacturer Code
#define INST_LOWER      ( 0 )               // Instance Lower, should be 0
#define INST_UPPER      ( 0 )               // Instance Upper, should be 0
#define DEVICE_FUNC     ( 0xFF )            // Device Function
#define RESERVED_BIT    ( 0 )               // Reserved Bit, must be zero
#define DEVICE_CLASS    ( XB_eDEV_CLASS_POWER_MANAGEMENT ) // Device Class
#define SYSTEM_INST     ( 0 )               // System Instance
#define INDUSTRY_GROUP  ( XBT_INDUSTRY_GROUP )  // Industry Group
#define SELF_ADDRESS    ( 1 )               // Self Config Address, must be 1


// Random Unique Number Defines
#define XBTPTFRM_MEM_READ_START     ( 0x2000 )
#define XBTPTFRM_MEM_READ_END       ( 0x3F00 )

#define XBTPTFRM_DATA_NOT_INIT      ( 0 )
#define XBTPTFRM_DATA_DFLT          ( 0xFFFFFFFFUL )
#define XBTPTFRM_COUNT_MAX          ( 0xFFFF )


// fnLoadPrefAddr Defines
//
#define XBTPTFRM_PREF_ADX_DEFLT_VAL ( 0 )

#define XBTPTFRM_PAGE_MSK           ( TI_RAM_SIZE_OF_NVM_PAGE - 1 )


/*=============================================================================
                           Type Definitions
=============================================================================*/

// Layout of the NVM data shared between the main application and the Loader
// It is an exact copy of the DICT_tzLOADER_DATA defined in the
// Software\Code\SolarWave\Inc\dictionary.h file.
typedef struct zSHARED_DATA
{
    uchar8 aucXanbusName[ 8 ];   // 8 bytes of the Xanbus name
    uint16 uiHwModel;            // H/W model
    uint16 uiHwRev;              // H/W rev
    uchar8 ucPreferredAddr;      // Preferred Xanbus address
}XBTPTFRM_tzSHARED_DATA;


/*==============================================================================
                           Local/Private Constants
==============================================================================*/
// Default Node Name
static const uchar8 kaucDEFAULT_NAME[ 8 ] =
{
    (uchar8) ( XT_mMK_BYTE( UNIQUE_NUMBER ) ),
    (uchar8) ( XT_mMK_BYTE( UNIQUE_NUMBER >> 8 ) ),
    (uchar8) ( XT_mMK_BYTE( UNIQUE_NUMBER >> 16 )
             | XT_mMK_BYTE( MANU_CODE << 5 ) ),
    XT_mMK_BYTE( MANU_CODE >> 3 ),
    XT_mMK_BYTE( ( INST_LOWER ) | ( INST_UPPER << 3 ) ),
    XT_mMK_BYTE( DEVICE_FUNC ),
    XT_mMK_BYTE( ( RESERVED_BIT ) | ( DEVICE_CLASS << 1 ) ),
    ( XT_mMK_BYTE( SYSTEM_INST )
    | XT_mMK_BYTE( INDUSTRY_GROUP >> 4 )
    | XT_mMK_BYTE( SELF_ADDRESS << 7 ) )
};


/*==============================================================================
                             External Constants
==============================================================================*/
// From LoaderVersion.c
extern const schar8 LOADERVERSION_ucVersionByteVal;
extern const uint16 LOADERVERSION_uiBuildNumber;

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// Used for location of shared data and indication of whether the data is valid
static uint32 dataOffset = 0xFFFFFFFFUL;
// Includes XanBus loader name, preferred address, etc. See structure above.
static XBTPTFRM_tzSHARED_DATA tzSharedData;
// Loader Reserved Block
static LDR_tzLDR_RSVD tzLdrInfo;


/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    XBTPLATFORM_fnLoadPrefAddr

PURPOSE:
    Load the preferred address for this node from NVM

INPUTS:
    none

OUTPUTS:
    preferred address

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - Created placeholder version
Version 1.01  Date: 04/01/03  By: dalem
    - Made functional version using my NVM access
Version 1.02  Date: 04/04/03  By: dalem
    - NVM interface changed
Version 1.03  Date: 04/10/03  By: dalem
    - NVM interface changed again
Version 1.04  Date: 04/24/03  By: dalem
    - Changed function name
Version 1.10  Date: JUN-25-2003  By: Alan Travelbea
    - Data now in reserved area; loaded using NVM driver.
Version 1.11  Date: OCT-01-2003  By: Alan Travelbea
    - Protected NVM access from interrupts because if they occur while the
      NVM is on, the device may crash.
Version: 2.00  Date: MAR-28-2005  By:  Baldeesh S. Khaira
    - Changed location of where to get data in NVM
Version: 2.01  Date: MAY-02-2005  By:  Baldeesh S. Khaira
    - Adjusted so that you can get data directly from RAM

*******************************************************************************/
uchar8 XBTPLATFORM_fnLoadPrefAddr( void )
{
    if( dataOffset == XBTPTFRM_DATA_NOT_INIT )
    {
        // Either the read was unsuccessful or the name has not been programmed
        // yet.  Return the default Preferred Address
        return( XBTPTFRM_PREF_ADX_DEFLT_VAL );
    }

    // Return the Preferred Address loaded from NVM.
    return( tzSharedData.ucPreferredAddr );
}


/*******************************************************************************

FUNCTION NAME:
    XBTPLATFORM_fnLoadName

PURPOSE:
    Load the node name for this device from NVM

INPUTS:
    'pucName' - pointer to a buffer to copy the name into

OUTPUTS:
    none

NOTES:

VERSION HISTORY:

Version 1.00  Date: 03/24/03  By: dalem
    - Created placeholder version
Version 1.01  Date: 04/01/03  By: dalem
    - Made functional version using my NVM access
Version 1.02  Date: 04/04/03  By: dalem
    - NVM interface changed
Version 1.03  Date: 04/10/03  By: dalem
    - NVM interface changed again
Version 1.04  Date: 04/24/03  By: dalem
    - Changed function name
Version 1.10  Date: JUN-25-2003  By: Alan Travelbea
    - Data now in reserved area; loaded using NVM driver.
Version 1.11  Date: OCT-01-2003  By: Alan Travelbea
    - Protected NVM access from interrupts because if they occur while the
      NVM is on, the device may crash.
Version 1.12  Date: NOV-30-2003  By: Alan Travelbea
    - Attempt to randomize the unique number when the Loader name is undefined.
Version: 2.00  Date: MAR-28-2005  By:  Baldeesh S. Khaira
    - Changed location of where to get data in NVM
Version: 2.01  Date: MAY-02-2005  By:  Baldeesh S. Khaira
    - Adjusted so that you can get data directly from RAM

*******************************************************************************/
void XBTPLATFORM_fnLoadName( uchar8 *pucName )
{
    uint16  RndNum;
    uint16  Counter;

#if( defined ( F2837xS_LOADER_UPDATER ) )
    // Make the Updater appear as a Gateway device
    dataOffset = XBTPTFRM_DATA_NOT_INIT;
#endif

    if( ( dataOffset == XBTPTFRM_DATA_NOT_INIT ) ||
        ( ( tzSharedData.aucXanbusName[ XBTPTFRM_NAME_RESVD_LOC ]
            & XBTPTFRM_NAME_RESVD_BIT ) != 0 ) )
    {
        // Either the read was unsuccessful or the name has not been
        // programmed yet.  Load a default name.

        (void)memcpy( pucName, kaucDEFAULT_NAME, LDR_NAME_LENGTH );

        // Randomize Name
        //
        // Attempt to Randomize the Unique Number by summing RAM.
        for( Counter = XBTPTFRM_MEM_READ_START;
             Counter < XBTPTFRM_MEM_READ_END;
             ++Counter )
        {
            RndNum += *(uchar8 *)Counter;
        }

        // Assign 'unique number' to name.
        pucName[ 0 ] = XT_mMK_BYTE( RndNum );
        pucName[ 1 ] = XT_mMK_BYTE( RndNum >> XT_BITS_PER_BYTE );
    }
    else
    {
        ( void )memcpy( pucName,
                        tzSharedData.aucXanbusName,
                        LDR_NAME_LENGTH );
    }
}


/*******************************************************************************

FUNCTION NAME:
    XBTPLATFORM_fnLoadHwRev

PURPOSE:
    Load the node name for this node from NVM

INPUTS:
    'puiModelNum' - pointer to location to store model number
    'puiHwModRev' - pointer to location to store revision data

OUTPUTS:
    none

NOTES:

VERSION HISTORY:
Version 1.00  Date: 04/24/03  By: dalem
    - New
Version 1.10  Date: JUN-25-2003  By: Alan Travelbea
    - Data now in reserved area; loaded using NVM driver.
Version 1.11  Date: OCT-01-2003  By: Alan Travelbea
    - Protected NVM access from interrupts because if they occur while the
      NVM is on, the device may crash.
Version 1.12  Date: DEC-09-2003  By: Alan Travelbea
    - Changed unknown revision number value to 0xFFFF from 0xFFFE (PRN#719).

Version: 2.00  Date: MAR-28-2005  By:  Baldeesh S. Khaira
    - Changed location of where to get data in NVM

Version: 2.01  Date: MAY-02-2005  By:  Baldeesh S. Khaira
    - Adjusted so that you can get data directly from RAM

*******************************************************************************/

void XBTPLATFORM_fnLoadHwRev( uint16 *puiModelNum,
                              uint16 *puiHwModRev )
{
    if( dataOffset == XBTPTFRM_DATA_NOT_INIT )
    {
        // If the NVM is empty or the read was unsuccessful,
        // set the HW Rev to a default value.
        *puiModelNum = UINT_MAX;
        *puiHwModRev = UINT_MAX;
    }
    else
    {
        // The NVM contains the current HW Revision.  Copy the data to
        // the locations specified by the input parameters.
        *puiModelNum = tzSharedData.uiHwModel;
        *puiHwModRev = tzSharedData.uiHwRev;
    }
}


/*******************************************************************************

FUNCTION NAME:
    XBTPLATFORM_fnLoadLoaderVersion

PURPOSE:
    Check NVM and verify the Loader Version byte is stored correctly.

INPUTS:
    None.

OUTPUTS:
    tuiSTATUS  - Status code ( either eSTATUS_OK or eSTATUS_ERR ).

VERSION HISTORY:

Version: 1.00  Date: APR-29-2003  By:  Alan Travelbea
    - Created.

Version: 2.00  Date: MAR-28-2005  By:  Baldeesh S. Khaira
    - Changed location of where to get data in NVM

Version: 2.10  Date: JUN-26-2006  By: Trevor Monk
    - Added Loader Build Number

Version: 2.11  Date: April 23, 2018  By: Doru Botez
    - Used the XBTPTFRM_tzLDR_RSVD to read the version and build numbers
      with just one NVM_fnReadPage() call.

*******************************************************************************/
tuiSTATUS XBTPLATFORM_fnLoadLoaderVersion( void )
{
    uint16      nTries;
    uint16      uiLockId = 0;
    sint32      Count = XBTPTFRM_COUNT_MAX; // General counter
    tuiSTATUS   NvmDrvResult = eSTATUS_ERR; // NVM result code - assume error

    // Note:  All accesses to the NVM have their address multiplied by 2
    // because the size of a char is 16-bits in the TI 320F28xyz family of
    // processors, whereas in the NVM a char is 8-bits.

    // Loader Info Block: contains version and build numbers
    // Initialize the Loader Info Block
    memset( (void*)&tzLdrInfo, 0, sizeof( tzLdrInfo ) );

    // Read NVM
    //
    // Get the Loader Version from NVM.
    nTries = 3;
    while( nTries-- )
    {
        // wait until NVM is ready to read
        while( Count-- > 0 )
        {
            if( NVM_fnIsReady() != FALSE )
            {
                NvmDrvResult = eSTATUS_OK;
                break;
            }
        }

        // Read the Loader Version number
        NVM_fnReadPage( LDR_INFO_START << 1,
                        (void *)&tzLdrInfo,
                        sizeof( tzLdrInfo ),
                        uiLockId );

        // If version and build numbers match
        if( ( tzLdrInfo.uiLoaderVersion == LOADERVERSION_ucVersionByteVal ) &&
            ( tzLdrInfo.uiLoaderBuildNum == LOADERVERSION_uiBuildNumber ) )
        {
            // exit
            break;
        }

        // Else write the new Version and Build numbers
        //
        // Store the intended version number in a buffer
        tzLdrInfo.uiLoaderVersion = LOADERVERSION_ucVersionByteVal;
        tzLdrInfo.uiLoaderBuildNum = LOADERVERSION_uiBuildNumber;

        // Write the buffer to NVM
        NVM_fnWritePage( LDR_INFO_START << 1,
                         (void *)&tzLdrInfo,
                         sizeof( tzLdrInfo ),
                         uiLockId );
    }

    return( NvmDrvResult );
}

/*******************************************************************************

FUNCTION NAME:
    XBTPLATFORM_fnCtor

PURPOSE:
    Load the NVM offset to the LOADER_DATA page. The offset is stored within
    a structure of type DICT_tzLDR_RSVD located at the start of the NVM.
    See the Software\Code\SolarWave\Inc\dictionary.h file.

INPUTS:
    None.

OUTPUTS:
    None.

VERSION HISTORY:

Version: 1.00  Date: APR-29-2003  By:  Alan Travelbea
    - Created.

Version: 2.00  Date: MAR-28-2005  By:  Baldeesh S. Khaira
    - Changed location of where to get data in NVM

Version: 2.01  Date: MAY-02-2005  By:  Baldeesh S. Khaira
    - Adjusted so that it can read data from multiple pages

Version: 2.02  Date: MAY-05-2005  By:  Baldeesh S. Khaira
    - Modified so that it checks to see if NVMDRV is ready prior to reading

*******************************************************************************/
void XBTPLATFORM_fnCtor( void )
{
    sint16 Count;                               // General counter
    uchar8 DataCounter;                         // Counter to traverse NVM addresses
    uchar8 ReadLength;                          // How much to read from the NVM
    uint16 uiLockId = 0;
    uint16 *ReadData = ( uint16* )&tzSharedData;// Pointer to read data buffer
    tuiSTATUS NvmDrvResult = eSTATUS_OK;        // NVM operation result code

    // Note:  All accesses to the NVM have their address multiplied by 2
    // because the size of a char is 16-bits in the TI 320F28xyz family of
    // processors, whereas for the NVM a char is 8-bits.

    // Read first the Loader Reserved Block from NVM and ensure it
    // contains the correct version and build numbers.
    NvmDrvResult = XBTPLATFORM_fnLoadLoaderVersion();

    dataOffset = tzLdrInfo.uiLdrDataOffset;
    // make count equal to the size of the data we want to read
    Count = sizeof( XBTPTFRM_tzSHARED_DATA );

    // data not valid or a read error occurred
    if( ( dataOffset == XBTPTFRM_DATA_DFLT ) ||
        ( dataOffset == 0 ) ||
        ( NvmDrvResult != eSTATUS_OK ) )
    {
        dataOffset = XBTPTFRM_DATA_NOT_INIT;
    }
    // read the shared data
    else
    {
        // assign counter to the offset number
        DataCounter = dataOffset;

        // while we still have data to read
        while( Count > 0 )
        {
            // if the amount to read is less than the amount left to read in the
            // current page
            if( Count < ( TI_RAM_SIZE_OF_NVM_PAGE - ( DataCounter & XBTPTFRM_PAGE_MSK ) ) )
            {
                ReadLength = Count;
            }
            // else the amount to read is just the number of words left in the
            // current page
            else
            {
                ReadLength = TI_RAM_SIZE_OF_NVM_PAGE - ( DataCounter & XBTPTFRM_PAGE_MSK );
            }

            // read the data from the first page
            if( NVM_fnReadPage( DataCounter << 1,
                                (void *)ReadData,
                                ReadLength,
                                uiLockId ) != eSTATUS_OK )
            {
                // data not valid, just exit
                dataOffset = XBTPTFRM_DATA_NOT_INIT;
                break;
            }

            // adjust our counters for the next read
            Count -= ReadLength;
            DataCounter += ReadLength;
            ReadData += ReadLength;
        }
    }
}
