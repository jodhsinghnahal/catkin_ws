/*=============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  
    FlashSectorMap.h

PURPOSE:
    To define the number and range of the flash memory sectors.

FUNCTION(S):
    none
   
NOTES:
	The flash sectors here are listed in reverse order ( address wise ) but 
	are in order by the name of the sector( ie Sector A ) is coming first.
	
	The definition FLASHDRV_SECURE_SECTORS is project specific and is used
	to define what part of the flash you do NOT want to be erased or programmed.	    

CHANGE HISTORY:
$History: FlashSectorMap.h $
    
    *****************  Version 6  *****************
    User: Baldeeshk    Date: 18/11/04   Time: 3:58p
    Updated in $/PD/Common/Targets/TI/c28x/Inc
    - New sector map for the c28x processor.  This sector map will work
    with the F2811 and F2812 DSPs.
    
    *****************  Version 4  *****************
    User: Alant        Date: 7/07/03    Time: 11:30a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/FlashDrv
    - Renamed prefixes from FLASH_to FLASHDRV_.
    
    *****************  Version 3  *****************
    User: Alant        Date: 19/03/03   Time: 5:02p
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/FlashDrv
    - Fixed comment.
    
    *****************  Version 2  *****************
    User: Alant        Date: 12/03/03   Time: 11:29a
    Updated in $/PD/Common/Targets/TI/c24x/HAL/Test/FlashDrv
    - Added sector protection to help prevent accidental trashing of
    application code.
    
    *****************  Version 1  *****************
    User: Alant        Date: 10/03/03   Time: 11:33a
    Created in $/PD/Common/Targets/TI/c24x/HAL/Test/FlashDrv

Date:   Mar 7, 2003   By: Alan Travelbea
    -   Created

==============================================================================*/

#ifndef FLASHDRV_FLASHSECTORMAP_INCL
#define FLASHDRV_FLASHSECTORMAP_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"	//Requres and architecture defintion in the compiler
#include "LIB_stddefs.h"    //Standard defintions for all projects


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
                           Local Constants
==============================================================================*/

// This array describes the sectors of on-chip flash.  The first column is the
// start address of the sector, the second column the end address.  Enter the
// sectors in order that they appear in the memory to keep the numbering 
// consistant.
// ex. Sector A starts at adx 0x003F6000 and ends at adx 0x003F7F7F.  These 
// values are then entered into the table as below.
// Refer to the SPRS174K.pdf, "TMS320F2810, TMS320F2811, TMS320F2812,
// TMS320C2810, TMS320C2811, TMS320C2812 Digital Signal Processors Data Manual"
// for the complete memory map


const uint32 FLASHDRV_SectorMap[][2] =
{
	{0x003F6000,  0x003F7F7F},    // Sector A, ~8K
	{0x003F4000,  0x003F5FFF},    // Sector B, 8K
	{0x003F0000,  0x003F3FFF},    // Sector C, 16K
	{0x003EC000,  0x003EFFFF},    // Sector D, 16K
	{0x003E8000,  0x003EBFFF},    // Sector E, 16K
	{0x003E4000,  0x003E7FFF},	  // Sector F, 16K
	{0x003E0000,  0x003E3FFF},    // Sector G, 16K
	{0x003DC000,  0x003DFFFF},    // Sector H, 16K
	{0x003DA000,  0x003DBFFF},    // Sector I, 8K
	{0x003D8000,  0x003D9FFF},    // Sector J, 8K

};

/*==============================================================================
                              Defines
==============================================================================*/
// Defines the number of sectors in the sector map
#define FLASHDRV_NUM_OF_SECTORS	sizeof( FLASHDRV_SectorMap ) / \
								sizeof( FLASHDRV_SectorMap[0][0] ) / 2

// Defines the sectors which cannot be cleared, erased, or programmed.
//  Each bit represents a sector. ie Bit 0 = Sector 0, Bit 3 = Sector 3
//  Currently, only sector 0 is protected.
#define FLASHDRV_SECURE_SECTORS	0x0001


#endif // FLASHDRV_FLASHSECTORMAP_INCL

