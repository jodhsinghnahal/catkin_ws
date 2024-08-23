/*=============================================================================

(c) 2004 - 2018 Schneider Electric SE. All rights reserved.
All trademarks are owned or licensed by Schneider Electric SE,
its subsidiaries or affiliated companies.

=============================================================================*/

/***************************************************************************//**

  @addtogroup crc16 CRC16
  @{

    @brief CRC16 calculation functions

    @page crc16_design CRC16 calculation library

    This module provides functions for CRC16 calculation using
    the CCITT CRC-16 polynomial, i.e., 0x1021.

*******************************************************************************/

/*==============================================================================
                              Includes
==============================================================================*/
#include "CRC16.h"
#include "xassert.h"        // Assert function.

/*==============================================================================
                              Defines
==============================================================================*/

//! CRC polynomial definition
#define CRC16_POLYNOMIAL    ( 0x1021 )


//! Define the number of bytes in each source word.
//!
//! @note Cannot use sizeof() because for the TI compiler sizeof(uint16) is the
//!       same as sizeof(uchar8) but each uint16 contains two bytes of data.
#define CRC16_NUM_OF_SOURCE_BYTES   ( 2 )

/*==============================================================================
                              Defines
==============================================================================*/

#if defined( XT_BIG_ENDIAN ) || defined( __TMS320C28XX__ )
//! The __TMS320C28XX__ is defined for a C28X target as specified in the
//! TMS320C28x Optimizing C/C++ Compiler v18.1.0.LTS User's Guide in section
//! "2.5.1 Predefined Macro Names, Table 2-30. Predefined C28x Macro Names"
//! Note: TI parts use this macro even though they are declared as Little Endian
//! because any storage type equal to or smaller than 16 bits is actually Big
//! Endian.
#define mASSIGN_SOURCE_TO_BYTES()   SourceLSB = SourceData & 0xFF;\
                                                SourceMSB = SourceData >> 8

#elif defined( XT_LITTLE_ENDIAN )

#define mASSIGN_SOURCE_TO_BYTES()   SourceLSB = SourceData >> 8;    \
                                                SourceMSB = SourceData & 0xFF

#else
#error "CRC16.c - No endian type defined."
#endif


/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                           Local/Private Constants
==============================================================================*/

//! Lookup table used for CRC-16 calculation.
static const uint16 crc16_tbl[ 256 ] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

/*==============================================================================
                           Local/Private Variables
==============================================================================*/


/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/


/*==============================================================================
                           Function Definitions
==============================================================================*/

/*============================================================================*/
/*!

\fn
    CRC16_fnCalculateBlock

\brief
    Calculates the CRC on the specified block of data

\param[in]
    SourceArrayPtr - pointer to source array
    ArrayLength    - the number of elements in the source array

\param[in/out]
    CrcPtr         - pointer to the CRC


\note
    The 16-bit CRC is set up as a left-shifting CRC with no inversions.

\code

REVISION HISTORY:

VERSION HISTORY:
Version: 1.00  Date: MAR-28-2003  By: Alan Travelbea
    - Created.
Version: 1.10  Date: JUN-04-2003  By: Alan Travelbea
    - Added the fnBuildTable call to reduce code size.
Version: 1.20  Date: SEP-19-2003  By: Alan Travelbea
    - Changed algorithm.  Now mathematically correct.

\endcode

*/
/*============================================================================*/

void CRC16_fnCalculate( uint16 *SourceArrayPtr,
                        uint16 ArrayLength,
                        uint16 *CrcPtr )
{
    uint16 WorkingCrc;      // Current CRC.
    uint16 SourceData;
    uint16 Counter;
    uchar8 CurByte;
    uchar8 SourceMSB;
    uchar8 SourceLSB;

    // Ensure that the pointers passed in are not NULL.
    assert( SourceArrayPtr && CrcPtr );

    // Initialize the temporary CRC variable.
    WorkingCrc = *CrcPtr;

    // Calculate the CRC of the buffer
    //
    while( ArrayLength-- )
    {

        // Retrieve the next piece of data to include in the calculation.
        SourceData = *SourceArrayPtr;

        // Transfer the high and low bytes of the source data to individual
        // chars.  (Union doesn't work on TI because of their 'alternate'
        // method of calling chars 2 bytes long)
        mASSIGN_SOURCE_TO_BYTES();

        // Do the Actual Calcualation
        //
        // Perform the CRC calculation on each byte, starting with the most
        // significant byte.
        CurByte = SourceMSB;

        for( Counter = 0; Counter < CRC16_NUM_OF_SOURCE_BYTES; ++Counter )
        {
            WorkingCrc  = (uchar8)( WorkingCrc >> 8 ) | ( WorkingCrc << 8 );
            WorkingCrc ^= CurByte;
            WorkingCrc ^= (uchar8)( WorkingCrc & 0xFF ) >> 4;
            WorkingCrc ^= (uint16)( WorkingCrc << 8 ) << 4;
            WorkingCrc ^= (( WorkingCrc & 0xFF ) << 4 ) << 1;

            // We only iterate once so use the second byte in the second and
            // last go around.
            CurByte = SourceLSB;
        }

        ++SourceArrayPtr;
    }

    // Store CRC to be returned to the User.
    //
    *CrcPtr = WorkingCrc;
}

/*============================================================================*/
/*!

\fn
    CRC16_fnAddByte

\brief
    Adds one byte into the calculation of the CRC

\param[in]
    crc   - the current value of the CRC
    data  - the number of elements in the source array.

\param[out]
    uint16  - the new calculated value of the CRC

\code

REVISION HISTORY:

Version: 1.00  Date: Mar 14, 2017  By: Doru Botez
    - First revision.

\endcode

*/
/*============================================================================*/
uint16 CRC16_fnAddByte( uint16 uiCrc, uchar8 ucData )
{
    return (uiCrc << 8) ^ crc16_tbl[ (uiCrc >> 8) ^ ucData ];
}


/*============================================================================*/
/*!

\fn
    CRC16_fnCalculateBlock

\brief
    Calculates the CRC on the specified block of data

\param[in]
    uiCrc   - the current value of the CRC
    pucData - pointer to data block
    uiLen   - the number of elements in the data block

\param[out]
    uint16  - the new calculated value of the CRC

\note
    The 16-bit CRC is set up as a left-shifting CRC with no inversions.

\code

REVISION HISTORY:

Version: 1.00  Date: Mar 14, 2017  By: Doru Botez
    - First revision.

\endcode

*/
/*============================================================================*/
uint16 CRC16_fnCalculateBlock( volatile uint16 uiCrc,
                               uchar8 *pucData,
                               uint16 uiLen )
{
    int i;

    for( i = 0; i < uiLen; i++ )
    {
        uiCrc = CRC16_fnAddByte( uiCrc, pucData[i] );
    }

    return uiCrc;
}

//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
