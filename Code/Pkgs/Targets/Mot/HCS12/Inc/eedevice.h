/*==============================================================================
Copyright © 2003 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  
    eedevice.h

PURPOSE:
    Provide configuration parameters for the eeprom driver.  

FUNCTION(S):

   
NOTES:
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use CodeWright or ignore 
    **the text.
    
    This file must be included whenever the user wants to use eedrv.c/eedrv.h.
    
    For microchip eeprom, enabling and disabling the CS line is CPU/DSP specific.
    Go to eepromdrv.c to change which I/O line is used as the CS line 
    
    
CHANGE HISTORY:
$History: eedevice.h $
    
    *****************  Version 4  *****************
    User: Ronm         Date: 10/30/03   Time: 1:54p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Coprocessor/Xanbus/Test/Src
    - make this work for the 128 and 256 processors
    
    *****************  Version 3  *****************
    User: Ronm         Date: 10/30/03   Time: 1:43p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Coprocessor/Xanbus/Test/Src
    Changed memory size to match DG128.
    
    *****************  Version 2  *****************
    User: Baldeeshk    Date: 18/07/03   Time: 9:22a
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Inc
    Added defines for erase type, made specfiic for DG256
    
    *****************  Version 1  *****************
    User: Baldeeshk    Date: 17/07/03   Time: 5:01p
    Created in $/PD/Network_Extensions/Code/Common/HAL/Inc
    EEPROM configuration file for Motorola on-chip.
    

==============================================================================*/
#ifndef EEDEVICE_INCL
#define EEDEVICE_INCL


/*==============================================================================
                              Includes
==============================================================================*/

/*==============================================================================
                              Defines
==============================================================================*/


#define EEDRV_SECTOR_SIZE     4

// match the EE size to the corresponding processor.
#if defined( MC9S12DG128B )
#define EEDRV_MEM_SIZE        2048
#elif defined( MC9S12DG256B ) || defined( MC9S12DP256B )
#define EEDRV_MEM_SIZE        4096
#else
#error Invalid device.
#endif

#define EEDRV_ERASE_SECTORS   0
#define EEDRV_ERASE_ALL       1


#endif  // EEDEVICE_INCL