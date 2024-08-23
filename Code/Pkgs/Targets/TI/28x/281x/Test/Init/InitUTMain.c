/*==============================================================================
Copyright © 2004 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in
whole or in part, to anyone outside of Xantrex without the written approval of
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of
Xantrex who has not previously obtained written authorization for access from
the individual responsible for the source code, will have a significant
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:
    InitUT.c

PURPOSE:
    Simple unit test for the Init package.  There isn't much here, since most
    of functionality is in GEL Scripts.

FUNCTION(S):

    local:
        none

NOTES:

    To perform the unit tests define the following:

    1.  Select Unit_Test_0 from the drop down menu of project configurations.

        This selection configures the system for compatibilty with the pin-outs
        in UnitTest0.gel

        Rebuild the project and the press run.  It will stop at the ESTOP command.
        You can now use the GEL functions in the menu GEL>>Solar Wave Init Unit Test
        to test the results of the package.

    2.  Select Unit_Test_1 from the drop down menu of project configurations.

        This selection configures the system for compatibilty with the pin-outs
        in UnitTest1.gel

        Rebuild the project and the press run.  It will stop at the ESTOP command.
        You can now use the GEL functions in the menu GEL>>Solar Wave Init Unit Test
        to test the results of the package.

    3.  Select Unit_Test_2 from the drop down menu of project configurations.

        This selection configures the system for compatibilty with the pin-outs
        in UnitTest2.gel

        Rebuild the project and the press run.  It will stop at the ESTOP command.
        You can now use the GEL functions in the menu GEL>>Solar Wave Init Unit Test
        to test the results of the package.

    NOTE: Any GEL Function that doesn't have a number in the name (ie. GPIO_Registers_Config_0)
          should work regardless of the build options.

CHANGE HISTORY:
$Log: InitUTMain.c $

    ***********************************************
    Revision: NovaPfrmB_TrevorM/1
    User: TrevorM     Date: 02/09/05  Time: 03:44PM
    Modified to keep the processor running after initialization
    in order to measure memory cycle timing
    
    
    *****************  Version 2  *****************
    User: Brentt       Date: 6/09/04    Time: 5:05p
    Updated in $/PD/Common/Targets/TI/c28x/HAL/Test/Init
    - Converted all tabs to 4 spaces - as per the coding standard

    *****************  Version 1  *****************
    User: Brentt       Date: 6/08/04    Time: 2:32p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Test/Init
    Created - First Check-in

==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/
#include "init.h"
/*==============================================================================
                              Defines
==============================================================================*/

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
                           Public Variables
==============================================================================*/

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

/*==============================================================================
                           Function Definitions
==============================================================================*/
main(void)
{
    INIT_fnSysInit();

    while( TRUE )
    {
        // processor remains active for memory timing measurements
       	asm ("      NOP");
    }
}
