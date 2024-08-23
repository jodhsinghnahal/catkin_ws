/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    spislavechnl.h

PURPOSE:
    define spi communication slave device channels and its operations

FUNCTION(S):
    None
                   
    local:
        None
           
NOTES:
        This is processor specific that needed for multichannel SPI 
            communication.  This is for the TI 281x DSP.

        SPI master device provide number of IO pins to enable and disable
            the communiation of a slave device.

        Define the IO register that can operate for SPI enable/disable.

        
CHANGE HISTORY:
$Log: spislavechnl.h $

    ***********************************************
    Revision: NovaPfrmB_TrevorM/2
    User: TrevorM     Date: 03/02/05  Time: 10:30PM
    Moved EE Chip select to GPIOA14

    

    ***********************************************
    Revision: NovaPfrmB_TrevorM/1
    User: TrevorM     Date: 03/01/05  Time: 11:24PM
    changed EEPROM chip select pin to get the eedrv unit test to work.  This will
    need to be modified again when the hardware design is finalized.

    

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/1
    User: HorstM     Date: 02/17/05  Time: 07:12PM
    added EALLOW/EDIS to protected register macro def'n
    

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/2
    User: BaldeeshK     Date: 02/04/05  Time: 10:56PM
    - Code review changes 04/02/2005.  Added comments and
    changed #ifndef label
    

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/1
    User: BaldeeshK     Date: 12/08/04  Time: 05:01PM
    - Initial version for Accurev
    
    
    
   
==============================================================================*/
#ifndef SPISLAVECHNL_HL
#define SPISLAVECHNL_H


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    //Requres and architecture defintion in the compiler
#include "LIB_stddefs.h"    //Standard defintions for all projects
#include "Device.h"         // TI device headers
 
/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/


/*==============================================================================
                              Defines
==============================================================================*/
//enable or disable a slave channel communcation
#define SPISLAVECHNL_ENABLE     ( 0 )   // state to enable a slave channel
#define SPISLAVECHNL_DISABLE    ( 1 )   // state to disable a slave channel

//slave channel IDs
#define SPISLAVECHNL_ID_1       ( 0 )
#define SPISLAVECHNL_ID_2       ( 1 )
#define SPISLAVECHNL_ID_3       ( 2 )
#define SPISLAVECHNL_TOTAL      ( 3 )

//SPI slave channels vs. GPIO pin/registers
#define SPISLAVECHNL_1          ( GpioDataRegs.GPADAT.bit.GPIOA14 )
#define SPISLAVECHNL_2          ( GpioDataRegs.GPFDAT.bit.GPIOF9 )
#define SPISLAVECHNL_3          ( GpioDataRegs.GPFDAT.bit.GPIOF10 )

//initial the slave channels to disable state
#define SPISLAVECHNL_mDISABLE_ALL() ( SPISLAVECHNL_1 = SPISLAVECHNL_DISABLE ); \
                                    ( SPISLAVECHNL_2 = SPISLAVECHNL_DISABLE ); \
                                    ( SPISLAVECHNL_3 = SPISLAVECHNL_DISABLE )                                

//initialize the io mux for slavel channel io that used for slave selection
// NOTE:  The user must call the macro to allow for register access before
//        this macro can be used
#define SPISLAVECHNL_mINIT()    EALLOW;\
                                GpioMuxRegs.GPAMUX.bit.C2TRIP_GPIOA14 = 0;\
                                GpioMuxRegs.GPFMUX.bit.MCLKRA_GPIOF9 = 0;\
                                GpioMuxRegs.GPFMUX.bit.MFSXA_GPIOF10 = 0;\
                                GpioMuxRegs.GPADIR.bit.GPIOA14 = 1;\
                                GpioMuxRegs.GPFDIR.bit.GPIOF9 = 1;\
                                GpioMuxRegs.GPFDIR.bit.GPIOF10 = 1;\
                                EDIS

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


#endif  // SPISLAVECHNL_INCL

