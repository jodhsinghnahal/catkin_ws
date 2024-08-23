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
    device.h

PURPOSE:
    The file includes processor specific defines.  It includes the appropriate
    register mapping file.  It also contains basic device macros for enabling/
    disabling interrupts and the watchdog timer

FUNCTION(S):
     none

    local:
     none

NOTES:
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use CodeWright or ignore
    **the text.

HISTORY:
$Log: device.h $

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 01/20/06  Time: 11:34PM
    Added defines for processor HCS12X

    *****************  Version 15  *****************
    User: Albertl      Date: 5.26.04    Time: 4:47p
    Updated in $/PD/Network_Extensions/Code/UI/SCP/baseline/Inc
    safety check on the DEVICE_mWD_EN() macro

    *****************  Version 14  *****************
    User: Baldeeshk    Date: 26/05/04   Time: 4:35p
    Updated in $/PD/Network_Extensions/Code/UI/SCP/baseline/Inc
    - Fixed DEVICE_mWD_EN so that it only writes to the register once

    *****************  Version 12  *****************
    User: Ronm         Date: 1/09/04    Time: 3:13p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/HCS12/Inc
    - Removed erroneous parantheses () from reset macro

    *****************  Version 11  *****************
    User: Ronm         Date: 1/09/04    Time: 3:07p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/HCS12/Inc
    - Fixed the reset macro so that it will actually reset.

    *****************  Version 10  *****************
    User: Albertl      Date: 1/06/04    Time: 11:39a
    Updated in $/PD/Network_Extensions/Code/UI/SCP/baseline/Inc
    made the watchdog reset an infinite loop

    *****************  Version 9  *****************
    User: Albertl      Date: 1/06/04    Time: 9:57a
    Updated in $/PD/Network_Extensions/Code/UI/SCP/baseline/Inc
    - added in the proper prefixing names to the macros
    - added in macros to shutdown and reset device

    *****************  Version 8  *****************
    User: Hueyd        Date: 19/12/03   Time: 1:28p
    Updated in $/PD/Inverter_Charger/Inverter/SolarStorm_Series/Code/Res_Series/SS2500/HCS12/Packages/PowSeq/Test
    Removed include for HCS12RegBitDefs.h for Solar Storm project

    *****************  Version 7  *****************
    User: Hueyd        Date: 18/12/03   Time: 5:15p
    Updated in $/PD/Inverter_Charger/Inverter/SolarStorm_Series/Code/Res_Series/SS2500/HCS12/PreAmdl/Build
    included hcs12 regbitdefs

    *****************  Version 4  *****************
    User: Dalem        Date: 7/29/03    Time: 11:11a
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/Inc
    Removed WIN32 - no longer included in that environment

    *****************  Version 3  *****************
    User: Dalem        Date: 7/24/03    Time: 2:35p
    Updated in $/PD/Common/Protocols/XanBus/Code/Windows/CANCardX/Test/DiagEx/Inc
    Added a WIN32 check for windows environment

    *****************  Version 2  *****************
    User: Alant        Date: 24/07/03   Time: 11:03a
    Updated in $/PD/Common/FieldReprogram/Loader/Test/XanBus/Motorola/HCS12
    - Added old defines to processor header includes
    - Fixed macros

    *****************  Version 1  *****************
    User: Baldeeshk    Date: 21/07/03   Time: 1:29p
    Created in $/PD/Network_Extensions/Code/Common/HAL/Inc
    Initial checkin for device.h

==============================================================================*/

#ifndef DEVICE_INCL
#define DEVICE_INCL

/*==============================================================================
                              Includes
==============================================================================*/

/*==============================================================================
                              Defines
==============================================================================*/

/*==============================================================================
                              Macros
==============================================================================*/
static int cpu_sr;

extern int OS_CPU_SR_Save();
extern void OS_CPU_SR_Restore(int cpu_sr);

// enable interrupts
#define DEVICE_mGLOBAL_INT_EN( )     	{OS_CPU_SR_Restore(cpu_sr);}

// disable interrupts
#define DEVICE_mGLOBAL_INT_DIS( )    	{cpu_sr = OS_CPU_SR_Save();}

#endif  // DEVICE_INCL
