/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    eedevice.h

PURPOSE:
    Provide configuration parameters for Microchip SPI EEPROMs  

FUNCTION(S):

   
NOTES:
    
    This file must be included whenever the user wants to use eedrv.c/eedrv.h
    for the Microchip EEPROM MC25AA160 module. It specifies various parameters
    that are required to interface to this particular EEPROM.
    
    For microchip eeprom, enabling and disabling the CS line is CPU/DSP specific.
    Go to eepromdrv.c to change which I/O line is used as the CS line 
    
    
CHANGE HISTORY:
$Log: eedevice.h $

   ***********************************************
   Revision: NovaPfrmB_TrevorM/2
   User: TrevorM     Date: 03/12/05  Time: 01:27AM
   changed EEPROM baud rate to 1.5 Mbits/sec

    ***********************************************
    Revision: NovaPfrmB_TrevorM/1
    User: TrevorM     Date: 03/02/05  Time: 09:48PM
    Replaced EEDRV_COMM_BAUD_DIVISOR with EEDRV_COMM_BAUD

    

    ***********************************************
    Revision: NovaPfrmB_JohnB/1
    User: JohnB     Date: 01/11/05  Time: 07:06PM
    Updated to coding standard. All associated files and
    test updated to accomodate coding standard update.
    
    
    
    *****************  Version 1  *****************
    User: Yingrand     Date: 10/12/04   Time: 12:00p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Test/eedrv
    

==============================================================================*/
#ifndef EEDEVICE_INCL
#define EEDEVICE_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include "limits.h"         // needed for definition of CHAR_BIT
#include "LIB_stddefs.h"
#include "spidrv.h"         // spi communication setting
#include "config.h"

/*==============================================================================
                              Defines
==============================================================================*/
//EEPROM specific definitions
//MC25XX160
#if( CTRL_CONFIG_EEDRV == CTRL_EEDRV_MICROCHIP_MC25AA160 )
#define EEDRV_UPPER_HALF_STARTING_ADX     ( 0x0400 )
#define EEDRV_UPPER_QUARTER_STARTING_ADX  ( 0x0600 )
#define EEDRV_LAST_ADX              ( 0x07FF )
#define EEDRV_PAGE_FIRST_ADX_MSK    ( 0xFFF0 )

#if ( CHAR_BIT == 8 )
#define EEDRV_PAGE_SIZE    ( 16 )
#define EEDRV_MEM_SIZE     ( EEDRV_LAST_ADX - EEDRV_STARTING_ADX + 1 ) 
#elif (CHAR_BIT == 16 )
#define EEDRV_PAGE_SIZE    ( 8 )
#define EEDRV_MEM_SIZE     (( EEDRV_LAST_ADX - EEDRV_STARTING_ADX + 1 ) / 2) 
#endif

#define EEDRV_PAGE_ADX_SPAN ( 16 )  // number of addresses in a page

//MC25XX320
#elif( CTRL_CONFIG_EEDRV == CTRL_EEDRV_MICROCHIP_MC25AA320 )
#define EEDRV_UPPER_HALF_STARTING_ADX     ( 0x0800 )
#define EEDRV_UPPER_QUARTER_STARTING_ADX  ( 0x0C00 )
#define EEDRV_LAST_ADX              ( 0x0FFF )
#define EEDRV_PAGE_FIRST_ADX_MSK    ( 0xFFE0 )

#if ( CHAR_BIT == 8 )
#define EEDRV_PAGE_SIZE    ( 32 )
#define EEDRV_MEM_SIZE     ( EEDRV_LAST_ADX - EEDRV_STARTING_ADX + 1 ) 
#elif (CHAR_BIT == 16 )
#define EEDRV_PAGE_SIZE    ( 16 )
#define EEDRV_MEM_SIZE     (( EEDRV_LAST_ADX - EEDRV_STARTING_ADX + 1 ) / 2) 
#endif

#define EEDRV_PAGE_ADX_SPAN ( 32 )  // number of addresses in a page

//MC25XX640
#elif(  CTRL_CONFIG_EEDRV == CTRL_EEDRV_MICROCHIP_MC25AA640 )
#define EEDRV_UPPER_HALF_STARTING_ADX     ( 0x1000 )
#define EEDRV_UPPER_QUARTER_STARTING_ADX  ( 0x1800 )
#define EEDRV_LAST_ADX              ( 0x1FFF )
#define EEDRV_PAGE_FIRST_ADX_MSK    ( 0xFFE0 )

#if ( CHAR_BIT == 8 )
#define EEDRV_PAGE_SIZE    ( 32 )
#define EEDRV_MEM_SIZE     ( EEDRV_LAST_ADX - EEDRV_STARTING_ADX + 1 ) 
#elif (CHAR_BIT == 16 )
#define EEDRV_PAGE_SIZE    ( 16 )
#define EEDRV_MEM_SIZE     (( EEDRV_LAST_ADX - EEDRV_STARTING_ADX + 1 ) / 2) 
#endif

#define EEDRV_PAGE_ADX_SPAN ( 32 )  // number of addresses in a page

#else
#error no EEPROM device defined
#endif

//common definition for Microchip SPI EEPROM
// EEPROM SPI communication setting
#define EEDRV_COMM_MODE             ( SPIDRV_eMASTER )
#define EEDRV_COMM_CLK_EDGE         ( SPIDRV_eRISING_EDGE )
#define EEDRV_COMM_CLK_PHS          ( SPIDRV_eWITH_DELAY )
#define EEDRV_COMM_BAUD             ( 1500000 )
#define EEDRV_DATA_LENGTH           ( CHAR_BIT ) // # bits to transmit
#define EEDRV_ADX_LENGTH            ( 16 )       // # bits to transmit the adx
#define EEDRV_INSTRUCTION_LEN       ( 8 )   // # bits to transmit for the instr

#define EEDRV_STARTING_ADX          ( 0x0000 )   // memory map
#define EEDRV_PAGE_LAST_ADX_MSK     ( 0xFFFF )  
#define EEDRV_WREN                  ( 0x06 )     // EEPROM instructions
#define EEDRV_WRDI                  ( 0x04 )
#define EEDRV_RDSR                  ( 0x05 )
#define EEDRV_WRSR                  ( 0x01 )
#define EEDRV_READ                  ( 0x03 )
#define EEDRV_WRITE                 ( 0x02 )
#define EEDRV_SR_WPEN               ( XT_BITPOS7 )   // specific bit positions
#define EEDRV_SR_WPEN_MSK           ( 1 << EEDRV_SR_WPEN ) // write protect enable
// common Microchipd EEPROM definitions
#define EEDRV_MIN_PAGE_SIZE ( 1 )   // minimum # bytes for a page read/write 
#define EEDRV_SR_RESERVED_6       ( XT_BITPOS6 )  // reserved by Microchip
#define EEDRV_SR_RESERVED_5       ( XT_BITPOS5 )  // reserved by Microchip
#define EEDRV_SR_RESERVED_4       ( XT_BITPOS4 )  // reserved by Microchip
#define EEDRV_SR_BP1              ( XT_BITPOS3 )  // block protection 1
#define EEDRV_SR_BP0              ( XT_BITPOS2 )  // block protection 0
#define EEDRV_SR_WEL              ( XT_BITPOS1 )  // write enable latch
#define EEDRV_SR_WIP              ( XT_BITPOS0 )  // write in progress

#define EEDRV_SR_BP1_MSK  ( 1 << EEDRV_SR_BP1  ) // block protection 1
#define EEDRV_SR_BP0_MSK  ( 1 << EEDRV_SR_BP0  ) // block protection 0
#define EEDRV_SR_WEL_MSK  ( 1 << EEDRV_SR_WEL  ) // write enable latch
#define EEDRV_SR_WIP_MSK  ( 1 << EEDRV_SR_WIP  ) // write in progress
            
// Write status register (masked by the page protection bits)
#define EEDRV_NO_WRITE_PROTECT      ( 0x00 )
#define EEDRV_PROTECT_ALL           ( 0x0C )
#define EEDRV_PROTECT_UPPER_HALF    ( 0x08 )
#define EEDRV_PROTECT_UPPER_QUARTER ( 0x04 )

//EEPROM status setting
#define EEDRV_STATUS_SETTING        ( 0x00 )

// Hold the CS line in the enable state for x number of ns. See eeprom
//  spec sheet.
#define EEDRV_CS_EN_DLY_NS          ( 250 ) // amount of time we want to burn in ns

// Hold the CS line in the disable state for x number of ns. See eeprom
//  spec sheet.
#define EEDRV_CS_DIS_DLY_NS         ( 500 ) 

#endif  // EEDEVICE_INCL
