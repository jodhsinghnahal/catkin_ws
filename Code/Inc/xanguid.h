/*=============================================================================

Copyright © 2008 Xantrex Technology Inc.

This file is the property of Xantrex Technology Inc. and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without the
express written permission of Xantrex Technology Inc.

================================================================================*/
/*!
\file  xanguid.h

\brief
    Xantrex Global User ID.

\details

\code

CHANGE HISTORY:
$Log: xanguid.h $

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/114
    User: TrevorM     Date: 09/02/11  Time: 01:36AM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_LianneM/16
    User: LianneM     Date: 08/10/11  Time: 10:56PM
    Review #409 - Code review updates

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_LianneM/15
    User: LianneM     Date: 08/10/11  Time: 01:32AM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/112
    User: TrevorM     Date: 08/08/11  Time: 07:01PM
    Added the new KSYSLOG module and linked it to the event manager and fault
    manager modules.

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_PeterH_SoftEng/2
    User: SoftEng     Date: 07/27/11  Time: 06:05PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_LianneM/14
    User: LianneM     Date: 07/21/11  Time: 06:52PM
    Review #410,#411 - Completed

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/31
    User: HongY     Date: 06/27/11  Time: 06:29PM
    Applied different sysvar ids for DCDC.

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/30
    User: HongY     Date: 06/27/11  Time: 06:29PM
    Applied different sysvar id for DCDC

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/29
    User: HongY     Date: 06/24/11  Time: 10:53PM
    Renamed some warning sysvar names

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/28
    User: HongY     Date: 06/24/11  Time: 09:13PM
    Rearranged fault sysvar ids for dcdc

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HollyZ/14
    User: HollyZ     Date: 06/23/11  Time: 10:37PM
    Added more ids for agency package

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HollyZ/13
    User: HollyZ     Date: 06/22/11  Time: 10:58PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/27
    User: HongY     Date: 06/22/11  Time: 10:00PM
    Added fault Hysteresis sysids

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/26
    User: HongY     Date: 06/21/11  Time: 11:31PM
    Added more fault configurations to dcdc

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/25
    User: HongY     Date: 06/20/11  Time: 10:50PM
    Added new fault ids for DCDC.

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_ClioL/8
    User: ClioL     Date: 06/20/11  Time: 09:03PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HollyZ/12
    User: HollyZ     Date: 06/20/11  Time: 08:49PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HollyZ/11
    User: HollyZ     Date: 06/17/11  Time: 12:08AM
    Added more fault ids for AC DSP protection module

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HollyZ/10
    User: HollyZ     Date: 06/16/11  Time: 10:10PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/24
    User: HongY     Date: 06/14/11  Time: 10:36PM
    Added a new flag for ppv low

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/23
    User: HongY     Date: 06/14/11  Time: 09:16PM
    Moved new fault checkcases from faults.c to checkcase_dc.c

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/22
    User: HongY     Date: 06/14/11  Time: 09:15PM
    Renamed PV low sysvar

    ***********************************************
    Revision: GTTL_ctrl_YingranD/58
    User: YingranD     Date: 06/10/11  Time: 06:54PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/21
    User: HongY     Date: 06/09/11  Time: 11:01PM
    Added some PV and DC related fault detections(stage 1)

    ***********************************************
    Revision: GTTL_ctrl_YingranD/57
    User: YingranD     Date: 05/30/11  Time: 10:06PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/20
    User: HongY     Date: 05/26/11  Time: 10:21PM
    merged

    ***********************************************
    Revision: GTTL_ctrl_YingranD/56
    User: YingranD     Date: 05/24/11  Time: 09:38PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/19
    User: HongY     Date: 05/20/11  Time: 11:35PM
    Added a sysvar to start munge test

    ***********************************************
    Revision: GT-TL-CTRL_TrevorM/4
    User: TrevorM     Date: 05/11/11  Time: 04:36PM
    merged

    ***********************************************
    Revision: GTTL_ctrl_YingranD/55
    User: YingranD     Date: 05/10/11  Time: 11:11PM
    project clear up with filesys change.

    ***********************************************
    Revision: GT-TL-CTRL_TrevorM/2
    User: TrevorM     Date: 05/10/11  Time: 02:17PM
    merged

    ***********************************************
    Revision: GTTL_ctrl_YingranD/45
    User: YingranD     Date: 01/20/11  Time: 08:03PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/104
    User: TrevorM     Date: 12/22/10  Time: 10:33PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_LianneM/3
    User: LianneM     Date: 12/17/10  Time: 12:32AM
    Added support for Modbus function Read Device Identification (0x2E)

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_LianneM/2
    User: LianneM     Date: 12/09/10  Time: 09:51PM
    Added support for modbus single register write function

    (code 0x06) MODBUS_FUN_CODE_WRITE_SINGLE_REG. Removed

    MBSERVER_USE_MULTIPLIER code. Added sysvar for MBSERVER

    debug level.

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_LianneM/1
    User: LianneM     Date: 12/07/10  Time: 05:57PM
    Added sysvar for SNTP debug level.  Some minor cleanup.

    ***********************************************
    Revision: EMS_TrevorM/8
    User: TrevorM     Date: 08/26/10  Time: 05:24PM
    Added system variables to support XBVERSTS PGNs

    ***********************************************
    Revision: EMS_TrevorM/7
    User: TrevorM     Date: 08/26/10  Time: 04:21AM
    Added transmit PGN handlers for the PROD_INFO_STS, HW_REV_STS, and SW_VER_STS
    PGNs.

    ***********************************************
    Revision: EMS_TrevorM/3
    User: TrevorM     Date: 08/05/10  Time: 07:45PM
    Created project specific reboot module

    ***********************************************
    Revision: EMS_TrevorM/2
    User: TrevorM     Date: 08/05/10  Time: 07:29PM
    Added real time clock hardware simulator for the LM3S2965 evaluation board


    ***********************************************
    Revision: GT-TL-COMM_TrevorM/103
    User: TrevorM     Date: 08/05/10  Time: 02:56AM
    Created the portable Kernel Time module.

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/101
    User: TrevorM     Date: 08/03/10  Time: 07:39PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/100
    User: TrevorM     Date: 08/03/10  Time: 07:39PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/99
    User: TrevorM     Date: 08/03/10  Time: 07:39PM
    merged

    ***********************************************
    Revision: EMS_TrevorM/1
    User: TrevorM     Date: 08/03/10  Time: 06:56PM
    Changes to Kernel to improve portability

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/98
    User: TrevorM     Date: 08/03/10  Time: 07:04PM
    uC/FS and uC/OS-II upgrade

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/97
    User: TrevorM     Date: 06/08/10  Time: 04:53PM
    Created the I2C2 POSIX based Multi-Master I2C driver

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/96
    User: TrevorM     Date: 05/18/10  Time: 09:20PM
    System changes to support USB Keyboard

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/95
    User: TrevorM     Date: 05/06/10  Time: 03:41PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Loader_Working_TrevorM/2
    User: TrevorM     Date: 05/06/10  Time: 03:35PM
    Added module identifiers to boot loader LIBMGR declarations.  Added missing
    SYSVAR_fnFindByID declaration to sysvars.h

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/94
    User: TrevorM     Date: 04/12/10  Time: 09:26PM
    Completed LUA integration

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_RyanS/17
    User: RyanS     Date: 03/11/10  Time: 04:55PM
    Added IDs for permissions module.

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_RyanS/16
    User: RyanS     Date: 03/08/10  Time: 09:11PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/93
    User: TrevorM     Date: 02/26/10  Time: 04:33PM
    Added a globally unique module identifier in each module's LIBMGR_mINFO
    structure.

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/92
    User: TrevorM     Date: 02/25/10  Time: 05:43PM
    Modified the FILESYS Kernel module to add functions: FILESYS_fnMap(), and
    FILESYS_fnGetDeviceInfoFromHandle()

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/91
    User: TrevorM     Date: 02/05/10  Time: 05:22PM
    Added support for firmware upgrades from different sources

    ***********************************************
    Revision: GT_TL_PfrmA_Loader_Working_TrevorM/1
    User: TrevorM     Date: 01/19/10  Time: 05:59PM
    Adding loader to revision control

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/90
    User: TrevorM     Date: 11/26/09  Time: 07:06PM
    upgraded to uC/FSv4 and uC/USBv3.20

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/89
    User: TrevorM     Date: 11/04/09  Time: 11:08PM
    Added DUI trigger functionality

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/88
    User: TrevorM     Date: 11/02/09  Time: 06:19PM
    Added DUI functionality

    ***********************************************
    Revision: GTTL_comm_YingranD/2
    User: YingranD     Date: 08/27/09  Time: 12:03AM
    changes based on energy logging test:

    add battery RAM defintion in linker file

    include energy logging file in project file

    some new global variable id definition added

    ***********************************************
    Revision: GTTL_comm_YingranD/1
    User: YingranD     Date: 08/18/09  Time: 06:36PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_Laptop1_SurabyK/2
    User: SurabyK     Date: 08/14/09  Time: 08:54AM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/87
    User: TrevorM     Date: 07/22/09  Time: 01:49PM
    Added GUIDs for the oscillography module

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/86
    User: TrevorM     Date: 07/17/09  Time: 09:00PM
    New comm card integration

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/85
    User: TrevorM     Date: 07/13/09  Time: 03:22PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Ctrl_Working_HongY/11
    User: HongY     Date: 07/09/09  Time: 11:33PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/84
    User: TrevorM     Date: 07/09/09  Time: 11:10PM
    Update event manager to use a task and an event queue.

    ***********************************************
    Revision: GTTL_comm_BinD/16
    User: BinD     Date: 07/08/09  Time: 09:54PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/83
    User: TrevorM     Date: 07/08/09  Time: 05:48PM
    Added CLI debug capabilities

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/82
    User: TrevorM     Date: 07/07/09  Time: 05:12PM
    Added static DNS support to TCP/IP

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/81
    User: TrevorM     Date: 06/29/09  Time: 11:31PM
    Fixed GUID bug with 8100012A (VAR_FLT_GTVVHIGH_RECV)

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_TomL/6
    User: TomL     Date: 06/25/09  Time: 09:45PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/80
    User: TrevorM     Date: 06/25/09  Time: 08:36PM
    Added active fault list (accessable via rendering of FLTMGR.ACTIVEFAULT)


    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_TomL/5
    User: TomL     Date: 06/23/09  Time: 11:30PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/79
    User: TrevorM     Date: 06/23/09  Time: 03:55PM
    Added support for system configuration on the new NAND flash drive.

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_TomL/4
    User: TomL     Date: 06/22/09  Time: 09:36PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/78
    User: TrevorM     Date: 06/22/09  Time: 06:43PM
    Added APS uptime monitoring

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/77
    User: TrevorM     Date: 06/22/09  Time: 04:58PM
    Updated File System to v3.14

    Added NAND flash driver.

    ***********************************************
        Revision: GT_TL_PfrmA_Comm_Working_TomL/3
    User: TomL     Date: 06/10/09  Time: 09:09PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/76
    User: TrevorM     Date: 06/09/09  Time: 11:16PM
    Changes to fault email and test email generation.

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_TomL/2
    User: TomL     Date: 06/08/09  Time: 11:36PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/75
    User: TrevorM     Date: 06/08/09  Time: 06:04PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_TomL/1
    User: TomL     Date: 06/03/09  Time: 12:12AM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_ChrisS/8
    User: ChrisS     Date: 06/02/09  Time: 07:03PM
    merged

    ***********************************************
    Revision: GTTL_comm_BinD/14
    User: BinD     Date: 06/02/09  Time: 12:28AM
    Added cache (of images and css) for web server

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_DriveC_JosephineT/2
    User: JosephineT     Date: 05/30/09  Time: 12:32AM
    Added aggregates for energy logs.

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_DriveC_JosephineT/1
    User: JosephineT     Date: 05/30/09  Time: 12:12AM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_RyanS/13
    User: RyanS     Date: 05/28/09  Time: 03:53PM
    fixed ID conflict with CMD_GT_LCD_SEND and CMD_GT_MBSERVER_DUMP_MAP

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_RyanS/12
    User: RyanS     Date: 05/26/09  Time: 08:39PM
    merged

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/74
    User: TrevorM     Date: 05/26/09  Time: 06:46PM
    Added support for multiple groups of DSP status variables.

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/73
    User: TrevorM     Date: 05/22/09  Time: 02:13PM
    Added dry contact relay feature.

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/72
    User: TrevorM     Date: 05/21/09  Time: 02:43PM
    Added pty0 port selection and enable/disable functionality via system variables
    PTY0.PORT and PTY0.ENABLED

    ***********************************************
    Revision: GT-TL-COMM_TrevorM/71
    User: TrevorM     Date: 05/20/09  Time: 09:51PM
    Updated the diagnostic data logger to support dynamic start/stop, log purging,
    log item masking, and real-time clock synchronization.

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_JosephineT/3
    User: JosephineT     Date: 05/11/09  Time: 09:02PM
    merged

    ***********************************************
    Revision: GT_TL_PfrmA_Comm_Working_JosephineT/2
    User: JosephineT     Date: 05/11/09  Time: 08:55PM
    Updated copyright header.

\endcode
==================================================================================*/
#ifndef XANGUID_H
#define XANGUID_H

/* GUID definitions should follow the convention TYPE_PRODUCT_ID */
/* Types can be one of:

    MBOX    - Mailboxes             (0x20000000 - 0x3FFFFFFF )
    MSG     - Messages              (0x40000000 - 0x5FFFFFFF )
    CMD     - Commands              (0x60000000 - 0x7FFFFFFF )
    VAR     - Variables             (0x80000000 - 0x9FFFFFFF )
    FLT/WRN - Faults and Warnings   (0xF0000000 - 0xFFFFFFFF )

*/

/* file system driver identifiers */
#define FS_DRV_NOR                                 ( 0x00002000L )
#define FS_DRV_SDCARD                              ( 0x00002001L )
#define FS_DRV_EEFS                                ( 0x00002002L )
#define FS_DRV_RAMDISK                             ( 0x00002003L )
#define FS_DRV_NAND                                ( 0x00002004L )
#define FS_DRV_USBHOST                             ( 0x00002005L )

/* software module identifiers */
#define MID_GTCMDS                                 ( 0x01000000L )
#define MID_DATALOG                                ( 0x01000001L )
#define MID_EVTMGR                                 ( 0x01000002L )
#define MID_FLTMGR                                 ( 0x01000003L )
#define MID_LOG                                    ( 0x01000004L )
#define MID_MAILER                                 ( 0x01000005L )
#define MID_SYSCFG                                 ( 0x01000006L )
#define MID_UPLOADMGR                              ( 0x01000007L )
#define MID_USBMSD                                 ( 0x01000008L )
#define MID_HTTPSERVER                             ( 0x01000009L )
#define MID_FTPSERVER                              ( 0x0100000AL )
#define MID_REBOOT                                 ( 0x0100000BL )
#define MID_WEBSEVER                               ( 0x0100000CL )
#define MID_PHYSCAN                                ( 0x0100000DL )
#define MID_FSTOOLS                                ( 0x0100000EL )
#define MID_SENDMAIL                               ( 0x0100000FL )
#define MID_USBCLI                                 ( 0x01000010L )
#define MID_APPSETTINGS                            ( 0x01000011L )
#define MID_FAULTTAB                               ( 0x01000012L )
#define MID_GTTL                                   ( 0x01000013L )
#define MID_STDCMDS                                ( 0x01000014L )
#define MID_BSP                                    ( 0x01000015L )
#define MID_I2C_IO                                 ( 0x01000016L )
#define MID_LCDDRV                                 ( 0x01000017L )
#define MID_RTC                                    ( 0x01000018L )
#define MID_CLI                                    ( 0x01000019L )
#define MID_FILESYS                                ( 0x0100001AL )
#define MID_IPC                                    ( 0x0100001BL )
#define MID_KDBG                                   ( 0x0100001CL )
#define MID_KERNEL                                 ( 0x0100001DL )
#define MID_KEVENT                                 ( 0x0100001EL )
#define MID_KFAULT                                 ( 0x0100001FL )
#define MID_MAILBOX                                ( 0x01000020L )
#define MID_SYSTICK                                ( 0x01000021L )
#define MID_SYSVARS                                ( 0x01000022L )
#define MID_APSMON                                 ( 0x01000023L )
#define MID_ASSERT                                 ( 0x01000024L )
#define MID_CLI0                                   ( 0x01000025L )
#define MID_CLI1                                   ( 0x01000026L )
#define MID_CLI2                                   ( 0x01000027L )
#define MID_DBGTIME                                ( 0x01000028L )
#define MID_DISCOVERY                              ( 0x01000029L )
#define MID_DSPCONFIG                              ( 0x0100002AL )
#define MID_DSPSTATUS                              ( 0x0100002BL )
#define MID_DUI                                    ( 0x0100002CL )
#define MID_EEFS_I2C                               ( 0x0100002DL )
#define MID_ENERGY_LOG                             ( 0x0100002EL )
#define MID_EXECVAR                                ( 0x0100002FL )
#define MID_FILEVARS                               ( 0x01000030L )
#define MID_UCFS                                   ( 0x01000031L )
#define MID_FSNAND                                 ( 0x01000032L )
#define MID_FSNOR                                  ( 0x01000033L )
#define MID_FSRAMDISK                              ( 0x01000034L )
#define MID_GTRTC                                  ( 0x01000035L )
#define MID_INFOVARS                               ( 0x01000036L )
#define MID_IPC1                                   ( 0x01000037L )
#define MID_IPC2                                   ( 0x01000038L )
#define MID_IPCCMDS                                ( 0x01000039L )
#define MID_IPINFO                                 ( 0x0100003AL )
#define MID_LSDEV                                  ( 0x0100003BL )
#define MID_LSYSLIB                                ( 0x0100003CL )
#define MID_LUA                                    ( 0x0100003DL )
#define MID_MBOXTEST                               ( 0x0100003FL )
#define MID_MBTCPIP                                ( 0x01000040L )
#define MID_MD5CHECK                               ( 0x01000041L )
#define MID_MGET                                   ( 0x01000042L )
#define MID_MINFO                                  ( 0x01000043L )
#define MID_NETLOGGER                              ( 0x01000044L )
#define MID_OSCILLOGRAPHY                          ( 0x01000045L )
#define MID_PTY0                                   ( 0x01000047L )
#define MID_PVSCAN                                 ( 0x01000048L )
#define MID_RSS                                    ( 0x0100004AL )
#define MID_SCHEDULER                              ( 0x0100004BL )
#define MID_SCRIPTER                               ( 0x0100004CL )
#define MID_SDCARD                                 ( 0x0100004DL )
#define MID_SNTP                                   ( 0x0100004FL )
#define MID_SYSINFO                                ( 0x01000051L )
#define MID_SYSVARSTEST                            ( 0x01000052L )
#define MID_TCPCLI                                 ( 0x01000053L )
#define MID_TCPIP                                  ( 0x01000054L )
#define MID_UCPROBE                                ( 0x01000056L )
#define MID_UDPDEV                                 ( 0x01000057L )
#define MID_DBGSTREAM                              ( 0x01000058L )
#define MID_USBDEV                                 ( 0x0100005AL )
#define MID_USB0                                   ( 0x0100005BL )
#define MID_USBHOST                                ( 0x0100005CL )
#define MID_USERVARS                               ( 0x0100005EL )
#define MID_VARMATH                                ( 0x0100005FL )
#define MID_VER                                    ( 0x01000060L )
#define MID_WDT                                    ( 0x01000061L )
#define MID_WEBSERV                                ( 0x01000062L )
#define MID_CONDITION                              ( 0x01000063L )
#define MID_MBSERVER                               ( 0x01000064L )
#define MID_AUTHCMDS                               ( 0x01000065L )
#define MID_AUTHENTICATION                         ( 0x01000066L )
#define MID_PERMISSIONS                            ( 0x01000067L )
#define MID_TESTMOD                                ( 0x01000068L )
#define MID_LOSLIB                                 ( 0x01000069L )
#define MID_CTRIG                                  ( 0x0100006AL )
#define MID_BOOTLOADER                             ( 0x0100006BL )
#define MID_KEYBOARD                               ( 0x0100006CL )
#define MID_UART0                                  ( 0x0100006DL )
#define MID_KTIME                                  ( 0x0100006EL )
#define MID_RTC_LM3SX9XX                           ( 0x0100006FL )
#define MID_AUTHCLI                                ( 0x01000070L )
#define MID_WDT_HAL_LM3SX9XX                       ( 0x01000071L )
#define MID_BSP_I2C2                               ( 0x01000072L )
#define MID_USBKBD                                 ( 0x01000073L )
#define MID_KBDCLI                                 ( 0x01000074L )
#define MID_NULDEV                                 ( 0x01000075L )
#define MID_I2C2TEST                               ( 0x01000076L )
#define MID_WDT_HAL_LPC2468                        ( 0x01000077L )
#define MID_RTC_LPC2468                            ( 0x01000078L )
#define MID_DLIB_TIME                              ( 0x01000079L )
#define MID_RTC_LM3SX9XX_EVAL                      ( 0x0100007AL )
#define MID_EMS_REBOOT                             ( 0x0100007BL )
#define MID_XANBUS                                 ( 0x0100007CL )
#define MID_XBTIME                                 ( 0x0100007DL )
#define MID_XBCLI                                  ( 0x0100007EL )
#define MID_XBVERSTS                               ( 0x0100007FL )
#define MID_IPCRX1                                 ( 0x01000080L )
#define MID_IPCTEST                                ( 0x01000081L )
#define MID_KSIGNAL                                ( 0x01000082L )
#define MID_KEYDRV                                 ( 0x01000083L )
#define MID_LEDDRV                                 ( 0x01000084L )
#define MID_GENERALCTRL                            ( 0x01000085L )
#define MID_CONDITION_HDL                          ( 0x01000086L )
#define MID_TDD_CATS                               ( 0x01000087L )
#define MID_AGENCY                                 ( 0x01000088L )
#define MID_HSM_DCAC                               ( 0x01000089L )
#define MID_EVDISP                                 ( 0x0100008AL )
#define MID_HSM_DCDC                               ( 0x0100008BL )
#define MID_MEAS_CMDS                              ( 0x0100008CL )
#define MID_MPPT_CMDS                              ( 0x0100008DL )
#define MID_APPTEST_CMDS                           ( 0x0100008EL )
#define MID_MPPT                                   ( 0x0100008FL )
#define MID_ACPWM_CMDS                             ( 0x01000090L )
#define MID_DCPWM_CMDS                             ( 0x01000091L )
#define MID_MEAS_AC                                ( 0x01000092L )
#define MID_LEDDISPLAY                             ( 0x01000093L )
#define MID_RTCDRV                                 ( 0x01000094L )
#define MID_RTCTEST                                ( 0x01000095L )
#define MID_UCCLK                                  ( 0x01000096L )
#define MID_SSP0_DRV                               ( 0x01000097L )
#define MID_IPCVARS                                ( 0x01000098L )
#define MID_IPCVARCFG                              ( 0x01000099L )
#define MID_MBSERIAL                               ( 0x0100009AL )
#define MID_UART2                                  ( 0x0100009BL )
#define MID_DIG_CTRL_AC                            ( 0x0100009CL )
#define MID_NVCFG                                  ( 0x0100009DL )
#define MID_RELAY                                  ( 0x0100009EL )
#define MID_RELAY_CMDS                             ( 0x0100009FL )
#define MID_AGENCY_CMDS                            ( 0x010000A0L )
#define MID_BSP_I2C                                ( 0x010000A1L )
#define MID_POWER_LIM_DC                           ( 0x010000A2L )
#define MID_DC_MODULE                              ( 0x010000A3L )
#define MID_APP_CMDS                               ( 0x010000A4L )
#define MID_KEYSERVER                              ( 0x010000A5L )
#define MID_DPWS                                   ( 0x010000A6L )
#define MID_DPWS_AL_TEST1                          ( 0x010000A7L )
#define MID_DPWS_AL_TEST2                          ( 0x010000A8L )
#define MID_DPWS_AL_TEST3                          ( 0x010000A9L )
#define MID_DPWS_AL_TEST4                          ( 0x010000AAL )
#define MID_DPWS_AL_TEST5                          ( 0x010000ABL )
#define MID_DPWS_AL_TEST6                          ( 0x010000ACL )     
#define MID_DPWS_AL_TEST7                          ( 0x010000ADL ) 
#define MID_DPWS_AL_TEST8                          ( 0x010000AEL ) 
#define MID_DPWS_AL_TEST9                          ( 0x010000AFL )
#define MID_DPWS_AL_TEST8_CLIENT                   ( 0x010000B0L )
#define MID_DPWS_AL_TEST9_CLIENT                   ( 0x010000B1L )
#define MID_KSYSLOG                                ( 0x010000B2L )
#define MID_PMDBDRV                                ( 0x010000B3L )
#define MID_FONT                                   ( 0x010000B4L )


/* events */
#define EVT_GT_USB_CONNECT                         ( 0x10000000L )
#define EVT_GT_USB_DISCONNECT                      ( 0x10000001L )
#define EVT_SYSTEM_STARTUP                         ( 0x10000002L )
#define EVT_UPLOAD_START                           ( 0x10000003L )
#define EVT_UPLOAD_OK                              ( 0x10000004L )
#define EVT_UPLOAD_FAILED                          ( 0x10000005L )
#define EVT_CANNOT_INVERT                          ( 0x10000006L )
#define EVT_OPERATION_STATE_CHG                    ( 0x10000007L )
#define EVT_MPPT_MODE_CHG                          ( 0x10000008L )
#define EVT_GT_SUN_UP                              ( 0x10000009L )
#define EVT_GT_SUN_DOWN                            ( 0x1000000AL )
#define EVT_REBOOT                                 ( 0x1000000BL )
#define EVT_DSP_UP                                 ( 0x1000000CL )
#define EVT_DSP_DOWN                               ( 0x1000000DL )
#define EVT_DSP_RUNNING                            ( 0x1000000EL )
#define EVT_IPC_MESSAGE_LOST                       ( 0x1000000FL )
#define EVT_ENRGLOG_CONFIG_CONFLICT                ( 0x10000010L )
#define EVT_ENRGLOG_FAILED_TO_GET_SYSVAR           ( 0x10000011L )
#define EVT_ENRGLOG_LOGFILE_WRITE_FAILED           ( 0x10000012L )
#define EVT_ENRGLOG_EVTFILE_WRITE_FAILED           ( 0x10000013L )
#define EVT_SYSTEM_TEST_START                      ( 0x10000014L )
#define EVT_SYSTEM_TEST_END_PASS                   ( 0x10000015L )
#define EVT_SYSTEM_TEST_END_FAIL                   ( 0x10000016L )
#define EVT_DRIVE_FORMAT_FAILED                    ( 0x10000017L )
#define EVT_WARNING_TRIP                           ( 0x10000018L )
#define EVT_WARNING_GONE                           ( 0x10000019L )
#define EVT_ENRGLOG_LOGFILE_READ_FAILED            ( 0x1000001AL )
#define EVT_USBH_CONNECT                           ( 0x1000001BL )
#define EVT_USBH_DISCONNECT                        ( 0x1000001CL )
#define EVT_CONFIRM_UPGRADE                        ( 0x1000001DL )
#define EVT_UPGRADE_CONFIRMED                      ( 0x1000001EL )
#define EVT_UPGRADE_DENIED                         ( 0x1000001FL )
#define EVT_FTP_SESSION_START                      ( 0x10000020L )
#define EVT_FTP_SESSION_END                        ( 0x10000021L )
#define EVT_HID_CONNECTED                          ( 0x10000022L )
#define EVT_HID_DISCONNECTED                       ( 0x10000023L )
#define EVT_KEYBOARD_CONNECTED                     ( 0x10000024L )

#define EVT_KBD_BUTTON_UP                          ( 0x10000050L )
#define EVT_KBD_BUTTON_DOWN                        ( 0x10000051L )
#define EVT_KBD_BUTTON_LEFT                        ( 0x10000052L )
#define EVT_KBD_BUTTON_RIGHT                       ( 0x10000053L )
#define EVT_KBD_BUTTON_SELECT                      ( 0x10000054L )


/* mailboxes */
#define MBOX_GT_LCD                                ( 0x20000000L )
#define MBOX_GT_CLI                                ( 0x20000001L )
#define MBOX_GT_CLI0                               ( 0x20000002L )
#define MBOX_GT_CLI1                               ( 0x20000003L )
#define MBOX_GT_TCPCLI                             ( 0x20000004L )
#define MBOX_GT_USBCLI                             ( 0x20000005L )
#define MBOX_GT_WEBCLI                             ( 0x20000006L )
#define MBOX_GT_MAILER                             ( 0x20000007L )
#define MBOX_GT_IPC1                               ( 0x20000008L )
#define MBOX_GT_DATALOG                            ( 0x20000009L )
#define MBOX_GT_NETLOGGER                          ( 0x2000000AL )
#define MBOX_GT_IPC0                               ( 0x2000000BL )
#define MBOX_GT_DSPCFG                             ( 0x2000000CL )
#define MBOX_GT_CLI2                               ( 0x2000000DL )
#define MBOX_GT_IPC2                               ( 0x2000000EL )
#define MBOX_GT_CLI3                               ( 0x2000000FL )
#define MBOX_GT_SCHEDULER                          ( 0x20000010L )
#define MBOX_GT_SCRIPTER                           ( 0x20000011L )
#define MBOX_GT_LUA                                ( 0x20000012L )
#define MBOX_GT_DUI                                ( 0x20000013L )
#define MBOX_GT_GTTL                               ( 0x20000014L )
#define MBOX_USB_DEV                               ( 0x20000015L )
#define MBOX_LOADER                                ( 0x20000016L )
#define MBOX_UPLOADMGR                             ( 0x20000017L )
#define MBOX_CTRIG                                 ( 0x20000018L )
#define MBOX_KBDCLI                                ( 0x20000019L )
#define MBOX_XBCLI                                 ( 0x2000001AL )
#define MBOX_IPCTEST                               ( 0x2000001BL )
#define MBOX_LED                                   ( 0x2000001CL )
#define MBOX_IPCVARS                               ( 0x2000001DL )
#define MBOX_IPCVARCFG                             ( 0x2000001EL )
#define MBOX_HSM_DCDC                              ( 0x2000001FL )
#define MBOX_HSM_DCAC                              ( 0x20000020L )
#define MBOX_AGENCY_REPORT                         ( 0x20000021L )
#define MBOX_KEYSERVER                             ( 0x20000022L )

/* faults */
#define FLT_PV_DCV_GREATER_THAN_MPPT_HIGH          ( 0x30000000L )
#define FLT_PV_DCV_LESS_THAN_MPPT_LOW              ( 0x30000001L )
#define FLT_NIGHT_TIME                             ( 0x30000002L )
#define FLT_PV_HIGH                                ( 0x30000003L )
#define FLT_APS                                    ( 0x30000004L )
#define FLT_GRID_DELAY                             ( 0x30000005L )
#define FLT_COMM_FAIL                              ( 0x30000006L )
#define FLT_OVER_TEMPERATURE                       ( 0x30000007L )
#define FLT_TEMPERATURE_SENSOR                     ( 0x30000008L )
#define FLT_DC_INJECTION_FAILURE                   ( 0x30000009L )
#define FLT_GRID_VOLTAGE_HIGH                      ( 0x3000000AL )
#define FLT_GRID_VOLTAGE_VERY_HIGH                 ( 0x3000000BL )
#define FLT_GRID_VOLTAGE_LOW                       ( 0x3000000CL )
#define FLT_GRID_VOLTAGE_VERY_LOW                  ( 0x3000000DL )
#define FLT_GRID_FREQUENCY_HIGH                    ( 0x3000000EL )
#define FLT_GRID_FREQUENCY_LOW                     ( 0x3000000FL )
#define FLT_GRID_FREQUENCY_VERY_LOW                ( 0x30000010L )
#define FLT_OVER_VOLTAGE                           ( 0x30000011L )
#define FLT_OVER_CURRENT                           ( 0x30000012L )
#define FLT_PV_GROUND                              ( 0x30000013L )
#define FLT_MEMORY                                 ( 0x30000014L )
#define FLT_DCDC_VOLTAGE_HIGH                      ( 0x30000015L )
#define FLT_AC_CURRENT_SENSOR                      ( 0x30000016L )
#define FLT_RESIDUAL_CURRENT_DETECTOR              ( 0x30000017L )
#define FLT_OUTPUT_RELAY                           ( 0x30000018L )
#define FLT_PV_GROUND_OFFLINE                      ( 0x30000019L )
#define FLT_FAN                                    ( 0x3000001AL )
#define FLT_END_OF_LIFE                            ( 0x3000001BL )
#define FLT_3_PHASE_CONFIGURATION                  ( 0x3000001CL )
#define FLT_ACV_SENSOR                             ( 0x3000001DL )
#define FLT_SYSTEM_INITIALIZATION                  ( 0x3000001EL )
#define FLT_COMM_FAIL_COMM                         ( 0x3000001FL )
#define FLT_COMM_FAIL_RCD                          ( 0x30000020L )
#define FLT_DCDC_VOLTAGE_LOW                       ( 0x30000021L )
#define FLT_ACI_ERROR_ONLINE                       ( 0x30000022L )
//DCDC
/* faults */
#define FLT_PV_OV                                  ( 0x31000000L )
#define FLT_DCLINK_OV                              ( 0x31000001L )
#define FLT_DCLINK_BOTTOM_OV                       ( 0x31000002L )
#define FLT_DCLINK_TOP_OV                          ( 0x31000003L )
#define FLT_DCLINK_IMBALANCE                       ( 0x31000004L )
#define FLT_DCLINK_UV                              ( 0x31000005L )
#define FLT_DC_DCLINK_VOLT_SENSOR                  ( 0x31000006L ) 
#define FLT_PV_OC_RMS                              ( 0x31000007L )
#define FLT_PV_OC_INST                             ( 0x31000008L )
#define FLT_DC_OT                                  ( 0x31000009L ) 
#define FLT_APS_LOW                                ( 0x3100000AL ) 
#define FLT_RCMC                                   ( 0x3100000BL ) 
#define FLT_DC_HW_TRIP                             ( 0x3100000CL ) 
#define FLT_DC_COPROCESSOR_FAIL                    ( 0x3100000DL ) 
#define FLT_DC_INTER_COMM_FAIL                     ( 0x3100000EL ) 
#define FLT_DC_INIT_FAIL                           ( 0x3100000FL ) 
#define FLT_DC_BIST_FAIL                           ( 0x31000010L ) 
//DCDC
/* warnings */
#define WRN_PV1_LOW                                ( 0x31000020L )
#define WRN_PV2_LOW                                ( 0x31000021L )
#define WRN_PV3_LOW                                ( 0x31000022L )
#define WRN_PV4_LOW                                ( 0x31000023L )
#define WRN_PV1_OC_RMS                             ( 0x31000024L ) 
#define WRN_PV2_OC_RMS                             ( 0x31000025L ) 
#define WRN_PV3_OC_RMS                             ( 0x31000026L ) 
#define WRN_PV4_OC_RMS                             ( 0x31000027L ) 
#define WRN_PV1_OC_INST                            ( 0x31000028L ) 
#define WRN_PV2_OC_INST                            ( 0x31000029L ) 
#define WRN_PV3_OC_INST                            ( 0x3100002AL ) 
#define WRN_PV4_OC_INST                            ( 0x3100002BL ) 
#define WRN_DC12_OT                                ( 0x3100002CL ) 
#define WRN_DC34_OT                                ( 0x3100002DL ) 
#define WRN_TEMP_SENSOR_LOSS                       ( 0x3100002EL ) 
#define WRN_FAN_REPLACEMENT                        ( 0x3100002FL ) 
#define WRN_COOLING_LOSS                           ( 0x31000030L ) 
#define WRN_PV_MISSING                             ( 0x31000031L ) 

//DCAC
#define FLT_DCLINK_VOLT_SENSOR                     ( 0x30000040L ) 
#define FLT_OVER_CURRENT_RMS                       ( 0x30000041L ) 
#define FLT_GND_LEAKAGE_CURRENT                    ( 0x30000042L ) 
#define FLT_VREF_INVALID                           ( 0x30000043L ) 
#define FLT_EFFICIENCY_LOSS                        ( 0x30000044L ) 
#define FLT_PHASE_ERROR                            ( 0x30000045L ) 
#define FLT_PLL_LOST_LOCK                          ( 0x30000046L ) 
#define FLT_HARDWARE_TRIP                          ( 0x30000047L ) 
#define FLT_PWM_ERROR                              ( 0x30000048L ) 
#define FLT_INTER_COMM_FAIL                        ( 0x30000049L ) 
#define FLT_COPROCESSOR_FAIL                       ( 0x3000004AL ) 


/* messages */
#define MSG_NONE                                   ( 0x40000000L )
#define MSG_GT_KEYSTROKE                           ( 0x40000001L )
#define MSG_KERNEL_SYSTICK                         ( 0x40000002L )
#define MSG_KERNEL_CLI_CMD                         ( 0x40000003L )
#define MSG_GT_MAIL_REQUEST                        ( 0x40000004L )
#define MSG_GT_IPC_NACK                            ( 0x40000005L )
#define MSG_GT_IPC_ACK                             ( 0x40000006L )
#define MSG_GT_IPC_TX_MSG                          ( 0x40000007L )
#define MSG_GT_SET_OK                              ( 0x40000008L )
#define MSG_GT_SET_FAIL                            ( 0x40000009L )
#define MSG_IPC                                    ( 0x4000000AL )
#define MSG_GT_RUN_OK                              ( 0x4000000BL )
#define MSG_GT_RUN_FAIL                            ( 0x4000000CL )
#define MSG_SCRIPT_EXEC                            ( 0x4000000DL )
#define MSG_GT_PB_INT                              ( 0x4000000EL )
#define MSG_SYSVAR_NOTIFY                          ( 0x4000000FL )
#define MSG_EVENT_NOTIFY                           ( 0x40000010L )
#define MSG_FAULT_NOTIFY                           ( 0x40000011L )
#define MSG_GT_DATALOG_START                       ( 0x40000012L )
#define MSG_GT_DATALOG_STOP                        ( 0x40000013L )
#define MSG_USB_DEV_REMOVED                        ( 0x40000014L )
#define MSG_USB_DEV_ATTACHED                       ( 0x40000015L )
#define MSG_USB_DEV_CONFIGURED                     ( 0x40000016L )
#define MSG_SIGNAL_NOTIFY                          ( 0x40000017L )
#define MSG_IPCVARCFG_SEND_SHARE_REQUEST           ( 0x40000018L )

/* signals */
#define SIG_KEY_CODE                               ( 0x50000000L )
#define SIG_KEY_DOWN_PRESSED                       ( 0x50000001L )
#define SIG_KEY_DOWN_RELEASED                      ( 0x50000002L )
#define SIG_KEY_UP_PRESSED                         ( 0x50000003L )
#define SIG_KEY_UP_RELEASED                        ( 0x50000004L )
#define SIG_KEY_STATUS_PRESSED                     ( 0x50000005L )
#define SIG_KEY_STATUS_RELEASED                    ( 0x50000006L )
#define SIG_KEY_SETUP_PRESSED                      ( 0x50000007L )
#define SIG_KEY_SETUP_RELEASED                     ( 0x50000008L )
#define SIG_LED_OFF                                ( 0x50000009L )
#define SIG_LED_ON                                 ( 0x5000000AL )
#define SIG_LED_FLASH0                             ( 0x5000000BL )
#define SIG_LED_FLASH1                             ( 0x5000000CL )
#define SIG_LED_CODE                               ( 0x5000000DL )

/* event channel used as bit definition */
#define XANGUID_CHNL_MSK                           ( 0x0000001FL )
#define EVENT_CHANNEL_HSM                          ( 0x50100000L )
#define EVENT_CHANNEL_EV                           ( 0x50100001L )
#define EVENT_CHANNEL_AGENCY                       ( 0x50100002L )

/*hsm related signal/event  */
#define HSMBASE_SIGNAL_EMPTY                       ( 0x51000000L )
#define HSMBASE_SIGNAL_ENTRY                       ( 0x51000001L )
#define HSMBASE_SIGNAL_EXIT                        ( 0x51000002L )
#define HSMBASE_SIGNAL_INIT                        ( 0x51000003L )
#define HSMBASE_SIGNAL_ID                          ( 0x51000004L )
#define HSM_SIGNAL_A                               ( 0x51000005L )
#define HSM_SIGNAL_B                               ( 0x51000006L )
#define HSM_SIGNAL_C                               ( 0x51000007L )
#define HSM_SIGNAL_D                               ( 0x51000008L )
#define HSM_SIGNAL_E                               ( 0x51000009L )
#define HSM_SIGNAL_F                               ( 0x5100000AL )
#define HSM_SIGNAL_G                               ( 0x5100000BL )
#define HSM_SIGNAL_H                               ( 0x5100000CL )
#define HSM_SIGNAL_I                               ( 0x5100000DL )
#define HSM_SIGNAL_J                               ( 0x5100000EL )
#define HSM_SIGNAL_K                               ( 0x5100000FL )
#define HSM_SIGNAL_L                               ( 0x51000010L )
#define HSM_SIGNAL_M                               ( 0x51000011L )
#define HSM_SIGNAL_N                               ( 0x51000012L )
#define HSM_SIGNAL_O                               ( 0x51000013L )
#define HSM_SIGNAL_P                               ( 0x51000014L )
#define HSM_SIGNAL_Q                               ( 0x51000015L )
#define HSM_SIGNAL_R                               ( 0x51000016L )
#define HSM_SIGNAL_S                               ( 0x51000017L )
#define HSM_SIGNAL_T                               ( 0x51000018L )
#define HSM_SIGNAL_U                               ( 0x51000019L )
#define HSM_SIGNAL_N_1                             ( 0x51010012L )
#define HSM_SIGNAL_O_1                             ( 0x51010013L )
#define HSM_SIGNAL_P_1                             ( 0x51010014L )
#define HSM_SIGNAL_Q_1                             ( 0x51010015L )
#define HSM_SIGNAL_R_1                             ( 0x51010016L )
#define HSM_SIGNAL_S_1                             ( 0x51010017L )
#define HSM_SIGNAL_T_1                             ( 0x51010018L )
#define HSM_SIGNAL_U_1                             ( 0x51010019L )
#define HSM_SIGNAL_N_2                             ( 0x51020012L )
#define HSM_SIGNAL_O_2                             ( 0x51020013L )
#define HSM_SIGNAL_P_2                             ( 0x51020014L )
#define HSM_SIGNAL_Q_2                             ( 0x51020015L )
#define HSM_SIGNAL_R_2                             ( 0x51020016L )
#define HSM_SIGNAL_S_2                             ( 0x51020017L )
#define HSM_SIGNAL_T_2                             ( 0x51020018L )
#define HSM_SIGNAL_U_2                             ( 0x51020019L )
#define HSM_SIGNAL_N_3                             ( 0x51030012L )
#define HSM_SIGNAL_O_3                             ( 0x51030013L )
#define HSM_SIGNAL_P_3                             ( 0x51030014L )
#define HSM_SIGNAL_Q_3                             ( 0x51030015L )
#define HSM_SIGNAL_R_3                             ( 0x51030016L )
#define HSM_SIGNAL_S_3                             ( 0x51030017L )
#define HSM_SIGNAL_T_3                             ( 0x51030018L )
#define HSM_SIGNAL_U_3                             ( 0x51030019L )
#define HSM_SIGNAL_N_4                             ( 0x51040012L )
#define HSM_SIGNAL_O_4                             ( 0x51040013L )
#define HSM_SIGNAL_P_4                             ( 0x51040014L )
#define HSM_SIGNAL_Q_4                             ( 0x51040015L )
#define HSM_SIGNAL_R_4                             ( 0x51040016L )
#define HSM_SIGNAL_S_4                             ( 0x51040017L )
#define HSM_SIGNAL_T_4                             ( 0x51040018L )
#define HSM_SIGNAL_U_4                             ( 0x51040019L )
#define AGENCY_SIGNAL_TRIP                         ( 0x51040100L )
#define AGENCY_SIGNAL_CLR                          ( 0x51040101L )

#define EV_SIGNAL_MSG_KEEP                         ( 0x51040110L )
#define EV_SIGNAL_MSG_CLR                          ( 0x51040111L )


/*hsm state */
#define STATE_TOPSTATE                             ( 0x52000000L )
#define STATE_STARTUP                              ( 0x52000001L )
#define STATE_INITIALIZATION                       ( 0x52000002L )
#define STATE_SELF_TEST                            ( 0x52000003L )
#define STATE_CONFIGURATION                        ( 0x52000004L )
#define STATE_RESTART                              ( 0x52000005L )
#define STATE_STANDBY                              ( 0x52000006L )
#define STATE_SLEEP                                ( 0x52000007L )
#define STATE_FREEZE                               ( 0x52000008L )
#define STATE_IDLE                                 ( 0x52000009L )
#define STATE_OPERATE                              ( 0x5200000AL )
#define STATE_MANUAL                               ( 0x5200000BL )
#define STATE_AUTO                                 ( 0x5200000CL )
#define STATE_RECOVERABLE_FAULT                    ( 0x5200000DL )
#define STATE_OFFLINE                              ( 0x5200000EL )
#define STATE_ACTIVE_OPERATION                     ( 0x5200000FL )
#define STATE_TRANSITION                           ( 0x52000010L )
#define STATE_ONLINE                               ( 0x52000011L )
#define STATE_READY                                ( 0x52000012L )
#define STATE_MODULE1_TOPSTATE                     ( 0x52010000L )
#define STATE_MODULE1_IDLE                         ( 0x52010001L )
#define STATE_MODULE1_OPERATE                      ( 0x52010002L )
#define STATE_MODULE1_OFFLINE                      ( 0x52010003L )
#define STATE_MODULE1_FAULT                        ( 0x52010004L )
#define STATE_MODULE1_ACTIVE                       ( 0x52010005L )
#define STATE_MODULE1_TRANSITION                   ( 0x52010006L )
#define STATE_MODULE1_ONLINE                       ( 0x52010007L )
#define STATE_MODULE2_TOPSTATE                     ( 0x52020000L )
#define STATE_MODULE2_IDLE                         ( 0x52020001L )
#define STATE_MODULE2_OPERATE                      ( 0x52020002L )
#define STATE_MODULE2_OFFLINE                      ( 0x52020003L )
#define STATE_MODULE2_FAULT                        ( 0x52020004L )
#define STATE_MODULE2_ACTIVE                       ( 0x52020005L )
#define STATE_MODULE2_TRANSITION                   ( 0x52020006L )
#define STATE_MODULE2_ONLINE                       ( 0x52020007L )
#define STATE_MODULE3_TOPSTATE                     ( 0x52030000L )
#define STATE_MODULE3_IDLE                         ( 0x52030001L )
#define STATE_MODULE3_OPERATE                      ( 0x52030002L )
#define STATE_MODULE3_OFFLINE                      ( 0x52030003L )
#define STATE_MODULE3_FAULT                        ( 0x52030004L )
#define STATE_MODULE3_ACTIVE                       ( 0x52030005L )
#define STATE_MODULE3_TRANSITION                   ( 0x52030006L )
#define STATE_MODULE3_ONLINE                       ( 0x52030007L )
#define STATE_MODULE4_TOPSTATE                     ( 0x52040000L )
#define STATE_MODULE4_IDLE                         ( 0x52040001L )
#define STATE_MODULE4_OPERATE                      ( 0x52040002L )
#define STATE_MODULE4_OFFLINE                      ( 0x52040003L )
#define STATE_MODULE4_FAULT                        ( 0x52040004L )
#define STATE_MODULE4_ACTIVE                       ( 0x52040005L )
#define STATE_MODULE4_TRANSITION                   ( 0x52040006L )
#define STATE_MODULE4_ONLINE                       ( 0x52040007L )

/* commands */
#define CMD_GT_LIBMGR                              ( 0x60000001L )
#define CMD_GT_HELP                                ( 0x60000002L )
#define CMD_GT_RESET                               ( 0x60000003L )
#define CMD_GT_DISPMEM                             ( 0x60000004L )
#define CMD_GT_SETMEM                              ( 0x60000005L )
#define CMD_GT_PEEK                                ( 0x60000006L )
#define CMD_GT_POKE                                ( 0x60000007L )
#define CMD_GT_VER                                 ( 0x60000008L )
#define CMD_GT_TASK                                ( 0x60000009L )
#define CMD_GT_STACK                               ( 0x6000000AL )
#define CMD_GT_IDENT                               ( 0x6000000BL )
#define CMD_GT_SET                                 ( 0x6000000CL )
#define CMD_GT_GET                                 ( 0x6000000DL )
#define CMD_GT_VARS                                ( 0x6000000EL )
#define CMD_GT_AUTH_HELLO                          ( 0x6000000FL )
#define CMD_GT_RENDER                              ( 0x60000010L )
#define CMD_GT_SENDMAIL                            ( 0x60000011L )
#define CMD_GT_INV                                 ( 0x60000012L )
#define CMD_GT_RLY                                 ( 0x60000013L )
#define CMD_GT_DC_OFFSET_PWM                       ( 0x60000014L )
#define CMD_GT_SINE                                ( 0x60000015L )
#define CMD_GT_SYNC                                ( 0x60000016L )
#define CMD_GT_ADC                                 ( 0x60000017L )
#define CMD_GT_MEAS                                ( 0x60000018L )
#define CMD_GT_AGENCY                              ( 0x60000019L )
#define CMD_GT_AGENCY_FLT                          ( 0x6000001AL )
#define CMD_GT_CTRL_TASK_PRD                       ( 0x6000001BL )
#define CMD_GT_CTRL_TMR_PRD                        ( 0x6000001CL )
#define CMD_GT_TEST_AMP_EN                         ( 0x6000001DL )
#define CMD_GT_TEST_AMP_PWM                        ( 0x6000001EL )
#define CMD_GT_TEST_AMP_TEMP                       ( 0x6000001FL )
#define CMD_GT_TEST_AMP_POW                        ( 0x60000020L )
#define CMD_GT_TEST_AMP_PV                         ( 0x60000021L )
#define CMD_GT_TEST_AMP_TEMP_PAR                   ( 0x60000022L )
#define CMD_GT_TEST_AMP_POW_PAR                    ( 0x60000023L )
#define CMD_GT_TEST_AMP_PV_PAR                     ( 0x60000024L )
#define CMD_GT_AC_CAP_TMR                          ( 0x60000025L )
#define CMD_GT_EVENT                               ( 0x60000026L )
#define CMD_GT_DISCOVERY_PULSE                     ( 0x60000027L )
#define CMD_GT_RTC_DATE                            ( 0x60000028L )
#define CMD_GT_RTC_TIME                            ( 0x60000029L )
#define CMD_GT_RTC_GETDATETIME                     ( 0x6000002AL )
#define CMD_GT_SNTP_SYNC                           ( 0x6000002BL )
#define CMD_GT_FAULT                               ( 0x6000002CL )
#define CMD_GT_AGENCY_FACK                         ( 0x6000002DL )
#define CMD_GT_INV_STATE                           ( 0x6000002EL )
#define CMD_GT_INV_MODE                            ( 0x6000002FL )
#define CMD_GT_RCD_SELFTEST                        ( 0x60000030L )
#define CMD_GT_RCD_STATUS                          ( 0x60000031L )
#define CMD_GT_RCD_OFF                             ( 0x60000032L )
#define CMD_GT_RCD_ON                              ( 0x60000033L )
#define CMD_GT_RCD_CURRENT                         ( 0x60000034L )
#define CMD_GT_RCD_MODEL                           ( 0x60000035L )
#define CMD_GT_RCD_VERSION                         ( 0x60000036L )
#define CMD_GT_MB_TCPIP_TEST                       ( 0x60000037L )
#define CMD_GT_IPC_ACK                             ( 0x60000038L )
#define CMD_GT_IPC_NACK                            ( 0x60000039L )
#define CMD_GT_SETOK                               ( 0x6000003AL )
#define CMD_GT_SETFAIL                             ( 0x6000003BL )
#define CMD_GT_RUNOK                               ( 0x6000003CL )
#define CMD_GT_RUNFAIL                             ( 0x6000003DL )
#define CMD_GT_MINFO                               ( 0x6000003EL )
#define CMD_GT_RUN                                 ( 0x6000003FL )
#define CMD_GT_RCD_SET_FREQ_60                     ( 0x60000040L )
#define CMD_GT_RCD_GET_FREQ                        ( 0x60000041L )
#define CMD_GT_RCD_RAWCURRENT                      ( 0x60000042L )
#define CMD_GT_RCD_ADC0                            ( 0x60000043L )
#define CMD_GT_RCD_ADC1                            ( 0x60000044L )
#define CMD_GT_RCD_ADCDIF                          ( 0x60000045L )
#define CMD_GT_TEST_FAN_EN                         ( 0x60000046L )
#define CMD_GT_TEST_FAN_PWM                        ( 0x60000047L )
#define CMD_GT_IPCSET                              ( 0x60000048L )
#define CMD_GT_INV_BLOCK                           ( 0x60000049L )
#define CMD_GT_TEST_RMS_RESP                       ( 0x6000004AL )
#define CMD_GT_MPPT                                ( 0x6000004BL )
#define CMD_GT_MGET                                ( 0x6000004CL )
#define CMD_GT_MDAT                                ( 0x6000004DL )
#define CMD_GT_MCACHE                              ( 0x6000004EL )
#define CMD_GT_MBSERVER_PKT_TEST                   ( 0x6000004FL )
#define CMD_GT_MBSERVER_READ_REG                   ( 0x60000050L )
#define CMD_GT_MBSERVER_WRITE_REG                  ( 0x60000051L )
#define CMD_GT_PV_SCAN                             ( 0x60000052L )
#define CMD_GT_PV_SCAN_ABORT                       ( 0x60000053L )
#define CMD_GT_INV_OP_ENABLE                       ( 0x60000054L )
#define CMD_GT_AI_EN                               ( 0x60000055L )
#define CMD_GT_AI_PF                               ( 0x60000056L )
#define CMD_GT_AI_TASK_PRD                         ( 0x60000057L )
#define CMD_GT_AI_CHK_RATE                         ( 0x60000058L )
#define CMD_GT_AI_CHK_DUR                          ( 0x60000059L )
#define CMD_GT_CTRL_LP                             ( 0x6000005AL )
#define CMD_GT_ACI_TEST                            ( 0x6000005BL )
#define CMD_GT_RELAY_ALL                           ( 0x6000005CL )
#define CMD_GT_RELAY_TEST                          ( 0x6000005DL )
#define CMD_GT_RELAY_SINGLE                        ( 0x6000005EL )
#define CMD_GT_IPCGET                              ( 0x6000005FL )
#define CMD_GT_RSS_TEST                            ( 0x60000060L )
#define CMD_GT_MD5CHECK_RUN                        ( 0x60000061L )
#define CMD_ALL_MBOX_SEND                          ( 0x60000062L )
#define CMD_GT_SDCARD_TEST                         ( 0x60000063L )
#define CMD_GT_NOR_INFO                            ( 0x60000064L )
#define CMD_GT_NOR_ERASE                           ( 0x60000065L )
#define CMD_GT_NOR_CHIP_ERASE                      ( 0x60000066L )
#define CMD_GT_NOR_TEST                            ( 0x60000067L )
#define CMD_GT_APPEND                              ( 0x60000068L )
#define CMD_GT_ATTRIB                              ( 0x60000069L )
#define CMD_GT_CHKDSK                              ( 0x6000006AL )
#define CMD_GT_CMP                                 ( 0x6000006BL )
#define CMD_GT_COPY                                ( 0x6000006CL )
#define CMD_GT_DEL                                 ( 0x6000006DL )
#define CMD_GT_DIR                                 ( 0x6000006EL )
#define CMD_GT_FORMAT                              ( 0x6000006FL )
#define CMD_GT_FSINFO                              ( 0x60000070L )
#define CMD_GT_HEX                                 ( 0x60000071L )
#define CMD_GT_LOG                                 ( 0x60000072L )
#define CMD_GT_MKDIR                               ( 0x60000073L )
#define CMD_GT_MKINIT                              ( 0x60000074L )
#define CMD_GT_MOVE                                ( 0x60000075L )
#define CMD_GT_RENAME                              ( 0x60000076L )
#define CMD_GT_RMDIR                               ( 0x60000077L )
#define CMD_GT_TOUCH                               ( 0x60000078L )
#define CMD_GT_TREE                                ( 0x60000079L )
#define CMD_GT_TRUNCATE                            ( 0x6000007AL )
#define CMD_GT_TYPE                                ( 0x6000007BL )
#define CMD_GT_UNZIP                               ( 0x6000007CL )
#define CMD_GT_XCODE                               ( 0x6000007DL )
#define CMD_GT_TFTP                                ( 0x6000007EL )
#define CMD_GT_PING                                ( 0x6000007FL )
#define CMD_GT_STARTNET                            ( 0x60000080L )
#define CMD_GT_VMACDUMP                            ( 0x60000081L )
#define CMD_GT_PHYDUMP                             ( 0x60000082L )
#define CMD_SYSCFG_FACTORY                         ( 0x60000083L )
#define CMD_SYSCFG_COMMIT                          ( 0x60000084L )
#define CMD_GT_MD5                                 ( 0x60000085L )
#define CMD_GT_EEFS_TEST                           ( 0x60000086L )
#define CMD_GT_JOB                                 ( 0x60000087L )
#define CMD_GT_SCRIPTER_EXEC                       ( 0x60000088L )
#define CMD_GT_ECHO                                ( 0x60000089L )
#define CMD_GT_LUA_EXEC                            ( 0x6000008AL )
#define CMD_GT_OSCILLOGRAPHY                       ( 0x6000008BL )
#define CMD_GT_WDT_ON                              ( 0x6000008CL )
#define CMD_GT_WDT_DUMP                            ( 0x6000008DL )
#define CMD_GT_PV_DATA                             ( 0x6000008EL )
#define CMD_GT_AUTH_LOGON                          ( 0x6000008FL )
#define CMD_GT_AUTH_PASSWORD                       ( 0x60000090L )
#define CMD_GT_AUTH_BYE                            ( 0x60000091L )
#define CMD_GT_AUTH_FAILED                         ( 0x60000092L )
#define CMD_GT_DCDC_ENABLE                         ( 0x60000093L )
#define CMD_GT_POW_LIM_SET                         ( 0x60000094L )
#define CMD_GT_POW_SLOPE_SET                       ( 0x60000095L )
#define CMD_GT_PV_MANUAL_REF                       ( 0x60000096L )
#define CMD_GT_IOPIN                               ( 0x60000097L )
#define CMD_GT_MPPT_STATE                          ( 0x60000098L )
#define CMD_GT_TEST_AMP_PAR                        ( 0x60000099L )
#define CMD_GT_CALIB_SCALE                         ( 0x6000009AL )
#define CMD_GT_CALIB_OFFSET                        ( 0x6000009BL )
#define CMD_GT_CALIB                               ( 0x600000A0L )
#define CMD_GT_AGENCY_ACTIVE_FLT                   ( 0x600000A1L )
#define CMD_GT_AGENCY_CLR_FLT                      ( 0x600000A2L )
#define CMD_GT_AGENCY_CHK_EN                       ( 0x600000A3L )
#define CMD_GT_TCPIP_DHCP                          ( 0x600000A4L )
#define CMD_GT_WDT_INHIBIT                         ( 0x600000A5L )
#define CMD_GT_UCPROBE_CTX                         ( 0x600000A6L )
#define CMD_GT_REBOOT                              ( 0x600000A7L )
#define CMD_GT_WEB_SAVE                            ( 0x600000A8L )
#define CMD_GT_IPINFO                              ( 0x600000A9L )
#define CMD_GT_LIMITS                              ( 0x600000AAL )
#define CMD_GT_DELUSER                             ( 0x600000ABL )
#define CMD_GT_VARSTEST                            ( 0x600000ACL )
#define CMD_GT_AGENCY_CHK_T                        ( 0x600000ADL )
#define CMD_GT_AGENCY_CHK_L                        ( 0x600000AEL )
#define CMD_GT_DC_OFFSET_EN                        ( 0x600000AFL )
#define CMD_GT_RCD_SET_FREQ_50                     ( 0x600000B0L )
#define CMD_VARMATH_ADD                            ( 0x600000B1L )
#define CMD_VARMATH_SUB                            ( 0x600000B2L )
#define CMD_DUI_OPTIONS                            ( 0x600000B3L )
#define CMD_DUI_NEXTOPT                            ( 0x600000B2L )
#define CMD_DUI_PREVOPT                            ( 0x600000B4L )
#define CMD_DUI_TRIGGER                            ( 0x600000B5L )
#define CMD_DUI_TRIGGER_ENABLE                     ( 0x600000B6L )
#define CMD_DUI_TRIGGER_DISABLE                    ( 0x600000B7L )
#define CMD_DUI_TIMER                              ( 0x600000B8L )
#define CMD_FILESYS_LSDEV                          ( 0x600000B9L )
#define CMD_GT_PERMISSIONS_SET                     ( 0x600000BAL )
#define CMD_GT_PERMISSIONS_GET                     ( 0x600000BBL )
#define CMD_CTRIG_ADD                              ( 0x600000BCL )
#define CMD_CTRIG_ENABLE                           ( 0x600000BDL )
#define CMD_CTRIG_DISABLE                          ( 0x600000BEL )
#define CMD_CTRIG_LIST                             ( 0x600000BFL )
#define CMD_CTRIG_DEL                              ( 0x600000C0L )
#define CMD_SIGNAL                                 ( 0x600000C1L )
#define CMD_GT_BOOST                               ( 0x600000C2L )
#define CMD_GT_BOOST_PWM                           ( 0x600000C3L )
#define CMD_GT_RLY_A                               ( 0x600000C4L )
#define CMD_GT_RLY_B                               ( 0x600000C5L )
#define CMD_GT_RLY_C                               ( 0x600000C6L )
#define CMD_GT_RLY_N                               ( 0x600000C7L )
#define CMD_GT_RLY_ABC                             ( 0x600000C8L )
#define CMD_GT_BOOST_PWM_FREQ                      ( 0x600000C9L )
#define CMD_GT_INV_PWM_FREQ                        ( 0x600000CAL )
#define CMD_GT_INV_PWM                             ( 0x600000CBL )
#define CMD_GT_INV_PWM_AMP                         ( 0x600000CCL )   
#define CMD_GT_INV_PWM_D                           ( 0x600000CDL )   
#define CMD_GT_INV_PWM_Q                           ( 0x600000CEL ) 
#define CMD_NVCFG_COMMIT                           ( 0x600000CFL )   
#define CMD_NVCFG_GET                              ( 0x600000D0L ) 
#define CMD_MPPT_ACTIVE_OUT                        ( 0x600000D1L ) 
#define CMD_PROC_FLT_OUT                           ( 0x600000D2L ) 
#define CMD_GT_BOOST_PWM_DUTY                      ( 0x600000D3L ) 
#define CMD_GT_FAN_PWM_DUTY                        ( 0x600000D4L ) 
#define CMD_GT_INV_PWM_DUTY                        ( 0x600000D5L )
#define CMD_GT_RLY_AC                              ( 0x600000D6L )
#define CMD_DSP_RESET_COMM                         ( 0x600000D7L )
#define CMD_DSP_I_CAL                              ( 0x600000D8L )
#define CMD_GT_AC_CAP_ENABLE                       ( 0x600000D9L )
#define CMD_RCD_DEMAG_OUT                          ( 0x600000DAL ) 
#define CMD_MEASAC_GETRAWS                         ( 0x600000DBL )
#define CMD_MEASAC_GETINST                         ( 0x600000DCL )
#define CMD_MEASAC_GETFS                           ( 0x600000DDL )
#define CMD_COEFF_PLL                              ( 0x600000DEL )
#define CMD_GRIDFREQ_DFLT                          ( 0x600000DFL )
#define CMD_COEFF_ILOOP                            ( 0x600000E0L )
#define CMD_COEFF_DCLOOP                           ( 0x600000E1L )
#define CMD_COEFF_PVLOOP                           ( 0x600000E2L )
#define CMD_DCLINK_VREF                            ( 0x600000E3L )
#define CMD_GT_DPWS                                ( 0x600000E4L )
 

#define CMD_GT_CONDITION_TEST                      ( 0x801000ADL )
#define CMD_GT_LCD_SEND                            ( 0x801000AEL )
#define CMD_GT_MBSERVER_DUMP_MAP                   ( 0x801000AFL )
#define CMD_GT_PHYSCAN                             ( 0x801000B0L )
#define CMD_SYS_TEST                               ( 0x801000B1L )
#define CMD_GT_NAND_INFO                           ( 0x801000B2L )

#define CMD_GT_TESTMOD_TEST                        ( 0x801000B3L )
#define CMD_MBSERVER_DUMP_DEVICE_ID_MAP            ( 0x801000B4L )
#define CMD_RTC_TEST_RUN1                          ( 0x801000B5L )
#define CMD_PMDB_TEST1                             ( 0x801000B6L )
#define CMD_PMDB_TEST2                             ( 0x801000B7L )
#define CMD_PMDB_TEST3                             ( 0x801000B8L )
#define CMD_FONT_TEST1                             ( 0x801000B9L )
#define CMD_FONT_LIST                              ( 0x801000BAL )
#define CMD_FONT_PRINT                             ( 0x801000BBL )
#define CMD_PMDB_CLEAR                             ( 0x801000BCL )

/* variables */
#define VAR_ALL_FGA                                ( 0x80000000L )
#define VAR_GT_CLI_PRIO                            ( 0x80000001L )
#define VAR_GT_IP                                  ( 0x80000002L )
#define VAR_GT_NETMASK                             ( 0x80000003L )
#define VAR_GT_GATEWAY                             ( 0x80000004L )
#define VAR_GT_MAC                                 ( 0x80000005L )
#define VAR_GT_DBGLVL                              ( 0x80000006L )
#define VAR_GT_DHCP_ENABLED                        ( 0x80000007L )
#define VAR_GT_HOSTNAME                            ( 0x80000008L )
#define VAR_GT_HTTPS_PORT                          ( 0x80000009L )
#define VAR_GT_FTP_CTRL_PORT                       ( 0x8000000AL )
#define VAR_GT_FTP_DATA_PORT                       ( 0x8000000BL )
#define VAR_GT_HTTPS_RENDER_BUFFER                 ( 0x8000000CL )
#define VAR_GT_HTTPS_BUFFER_SIZE                   ( 0x8000000DL )
#define VAR_GT_RENDERBUFFER_NUM_BUFS               ( 0x8000000EL )
#define VAR_GT_MAILSERVERNAME                      ( 0x8000000FL )
#define VAR_GT_MAILSERVERPORT                      ( 0x80000010L )
#define VAR_GT_MAIL_DFLT_RECIPIENTS                ( 0x80000011L )
#define VAR_GT_MAIL_DFLT_TEMPLATE_FILE             ( 0x80000012L )
#define VAR_GT_MAIL_RENDER_BUFFER                  ( 0x80000013L )
#define VAR_GT_MAIL_BUFFER_SIZE                    ( 0x80000014L )
#define VAR_GT_SMTP_BUF_SIZE                       ( 0x80000015L )
#define VAR_GT_MAIL_SUBJECT                        ( 0x80000016L )
#define VAR_GT_SENDMAIL_SPOOL_FILE                 ( 0x80000017L )
#define VAR_GT_MAIL_SPOOL_CHECK_INTERVAL           ( 0x80000018L )
#define VAR_GT_MAIL_SPOOL_DIR                      ( 0x80000019L )
#define VAR_GT_NETREADY                            ( 0x8000001AL )
#define VAR_GT_RAMDISK_BYTES_PER_SECTOR            ( 0x8000001BL )
#define VAR_GT_RAMDISK0_NUM_SECTORS                ( 0x8000001CL )
#define VAR_GT_RAMDISK1_NUM_SECTORS                ( 0x8000001DL )
#define VAR_GT_RAMDISK0_VOLUME_LABEL               ( 0x8000001EL )
#define VAR_GT_RAMDISK1_VOLUME_LABEL               ( 0x8000001FL )
#define VAR_GT_SD_CARD_VOLUME_LABEL                ( 0x80000020L )
#define VAR_GT_SYSTEM_ROOT                         ( 0x80000021L )
#define VAR_GT_LANGUAGE                            ( 0x80000022L )
#define VAR_GT_HTTPS_ROOT                          ( 0x80000023L )
#define VAR_GT_EVTMGR_DFLT_EVENTS_FILE             ( 0x80000024L )
#define VAR_GT_DISCOVERY_PULSE_INTERVAL            ( 0x80000025L )
#define VAR_GT_DISCOVERY_POLLED                    ( 0x80000026L )
#define VAR_GT_DISCOVERY_TIMEOUT                   ( 0x80000027L )
#define VAR_GT_SNTP_INTERVAL                       ( 0x80000028L )
#define VAR_GT_SNTP_SERVER_ADDR                    ( 0x80000029L )
#define VAR_GT_TIMEZONE                            ( 0x8000002AL )
#define VAR_GT_LOCALTIME                           ( 0x8000002BL )
#define VAR_GT_UTC                                 ( 0x8000002CL )
#define VAR_GT_DATALOG_INTERVAL                    ( 0x8000002DL )
#define VAR_GT_DATALOG_EXTENSION                   ( 0x8000002EL )
#define VAR_GT_DATALOG_MAX_ITEMS                   ( 0x8000002FL )
#define VAR_GT_DATALOG_ENABLED                     ( 0x80000030L )
#define VAR_GT_DATALOG_DESCFILE                    ( 0x80000031L )
#define VAR_GT_DATALOG_BUFSIZE                     ( 0x80000032L )
#define VAR_GT_DATALOG_RENDER_BUFFER               ( 0x80000033L )
#define VAR_GT_DATALOG_FILE_CREATION_INTERVAL      ( 0x80000034L )
#define VAR_GT_DATALOG_TIMEBASE                    ( 0x80000035L )
#define VAR_GT_DATALOG_BASEDIR                     ( 0x80000036L )
#define VAR_GT_DATALOG_TEMPLATE_FILENAME           ( 0x80000037L )
#define VAR_GT_DATALOG_ACTIVE_FILENAME             ( 0x80000038L )
#define VAR_GT_EVTMGR_FILE_CREATION_INTERVAL       ( 0x80000039L )
#define VAR_GT_EVTMGR_TIMEBASE                     ( 0x8000003AL )
#define VAR_GT_EVTMGR_BASEDIR                      ( 0x8000003BL )
#define VAR_GT_EVTMGR_TEMPLATE_FILENAME            ( 0x8000003CL )
#define VAR_GT_EVTMGR_ACTIVE_FILENAME              ( 0x8000003DL )
#define VAR_GT_EVTMGR_RENDER_BUFFER                ( 0x8000003EL )
#define VAR_GT_FLTMGR_RENDER_BUFFER                ( 0x8000003FL )
#define VAR_GT_FLTMGR_DFLT_FAULTS_FILE             ( 0x80000040L )
#define VAR_GT_FLTMGR_FILE_CREATION_INTERVAL       ( 0x80000041L )
#define VAR_GT_FLTMGR_TIMEBASE                     ( 0x80000042L )
#define VAR_GT_FLTMGR_BASEDIR                      ( 0x80000043L )
#define VAR_GT_FLTMGR_LOG_HEADER_FILENAME          ( 0x80000044L )
#define VAR_GT_FLTMGR_ACTIVE_FILENAME              ( 0x80000045L )
#define VAR_GT_FLTMGR_ENABLE_MASK0                 ( 0x80000046L )
#define VAR_GT_FLTMGR_EMAIL_MASK0                  ( 0x80000047L )
#define VAR_GT_FLTMGR_FAULT_STATE0                 ( 0x80000048L )
#define VAR_GT_FLTMGR_QUEUE_SIZE                   ( 0x80000049L )
#define VAR_GT_FLTMGR_QUEUE_BUFFER_SIZE            ( 0x8000004AL )
#define VAR_GT_FLTMGR_RENDER_BUFFER_SIZE           ( 0x8000004BL )
#define VAR_GT_FLTMGR_WORK_FILE                    ( 0x8000004CL )
#define VAR_GT_FLTMGR_MAIL_HEADER                  ( 0x8000004DL )
#define VAR_GT_FLTMGR_MAIL_FOOTER                  ( 0x8000004EL )
#define VAR_GT_FLTMGR_TEMPLATE_DIRECTORY           ( 0x8000004FL )
#define VAR_GT_FLTMGR_MAIL_TIMEOUT                 ( 0x80000050L )
#define VAR_GT_FLTMGR_MAIL_RECIPIENTS              ( 0x80000051L )
#define VAR_GT_FLTMGR_MAIL_SUBJECT                 ( 0x80000052L )
#define VAR_GT_FLTMGR_MAIL_SPOOL_DIR               ( 0x80000053L )
#define VAR_GT_MBTCPIP_IS_LISTENING                ( 0x80000054L )
#define VAR_GT_MBTCPIP_PORT                        ( 0x80000055L )
#define VAR_GT_MBTCPIP_INACTIVITY_TO               ( 0x80000056L )
#define VAR_GT_NETLOGGER_PORT                      ( 0x80000057L )
#define VAR_GT_NETLOGGER_CACHESIZE                 ( 0x80000058L )
#define VAR_GT_NETLOGGER_NUMCACHE                  ( 0x80000059L )
#define VAR_GT_NETLOGGER_RENDER_BUFFER             ( 0x8000005AL )
#define VAR_GT_NETLOGGER_MAX_AGE                   ( 0x8000005BL )
#define VAR_GT_NETLOGGER_BUFFER_SIZE               ( 0x8000005CL )
#define VAR_GT_SYSTICK_INTERVAL                    ( 0x8000005DL )
#define VAR_GT_SYSTICK_MAX_CLIENTS                 ( 0x8000005EL )
#define VAR_GT_SYSTICK_PRIO                        ( 0x8000005FL )
#define VAR_GT_DSP_VARS                            ( 0x80000060L )
#define VAR_GT_DSPCFG_TIMEOUT                      ( 0x80000061L )
#define VAR_GT_DSPCFG_RETRIES                      ( 0x80000062L )
#define VAR_GT_DSPSTS_VARS                         ( 0x80000063L )
#define VAR_GT_DSPSTS_INTERVAL_SEC                 ( 0x80000064L )
#define VAR_GT_DSPSTS_MAX_VARS                     ( 0x80000065L )
#define VAR_GT_DSPSTS_BUFSIZE                      ( 0x80000066L )
#define VAR_GT_MGET_RENDER_BUFFER                  ( 0x80000067L )
#define VAR_GT_MGET_BUFFER_SIZE                    ( 0x80000068L )
#define VAR_GT_MGET_CACHESIZE                      ( 0x80000069L )
#define VAR_GT_MGET_NUMCACHE                       ( 0x8000006AL )
#define VAR_GT_IPC1_QUEUE_SIZE                     ( 0x8000006BL )
#define VAR_GT_IPC1_BUFFER_SIZE                    ( 0x8000006CL )
#define VAR_GT_IPC1_RETRIES                        ( 0x8000006DL )
#define VAR_GT_IPC1_TIMEOUT                        ( 0x8000006EL )
#define VAR_GT_IPC1_DEVICE                         ( 0x8000006FL )
#define VAR_GT_RUNNING                             ( 0x80000070L )
#define VAR_GT_UPTIME                              ( 0x80000071L )
#define VAR_GT_PERMISSIONS_CFG_PATH                ( 0x80000072L )
#define VAR_KEYDRV_PRIO                            ( 0x80000073L )
#define VAR_SYSINFO_SID                            ( 0x80000074L )
#define VAR_GT_DBGTIME_ENABLE                      ( 0x80000075L )
#define VAR_GT_DBGTIME_UTC                         ( 0x80000076L )
#define VAR_GT_LOCALE                              ( 0x80000077L )
#define VAR_SYSINFO_PRODUCT                        ( 0x80000078L )
#define VAR_SYSINFO_FGA                            ( 0x80000079L )
#define VAR_SYSINFO_HARDWARE_PLATFORM              ( 0x8000007AL )
#define VAR_SYSINFO_SERIAL_NUMBER                  ( 0x8000007BL )
#define VAR_GT_MGET_RENDER_WSTREAM                 ( 0x8000007CL )
#define VAR_GT_MBSERVER_TEST_U16                   ( 0x8000007DL )
#define VAR_GT_MBSERVER_TEST_U32                   ( 0x8000007EL )
#define VAR_GT_MBSERVER_TEST_STR                   ( 0x8000007FL )
#define VAR_GT_MBSERVER_ADDRESS                    ( 0x80000080L )
#define VAR_GT_MBSERVER_REG_MAP                    ( 0x80000081L )
#define VAR_GT_AGENCY_PRIO                         ( 0x80000082L )
#define VAR_GT_AGENCYREPORTER_PRIO                 ( 0x80000083L )
#define VAR_GT_RCD_MONIOTR_PRIO                    ( 0x80000084L )
#define VAR_GT_CLI0_PRIO                           ( 0x80000085L )
#define VAR_GT_CLI1_PRIO                           ( 0x80000086L )
#define VAR_GT_CLI2_PRIO                           ( 0x80000087L )
#define VAR_GT_IPC0_PRIO                           ( 0x80000088L )
#define VAR_GT_IPC1_PRIO                           ( 0x80000089L )
#define VAR_GT_IPC2_PRIO                           ( 0x8000008AL )
#define VAR_GT_TEST_TASK_PRIO                      ( 0x8000008BL )
#define VAR_GT_AGENCY_MUTEX_PRIO                   ( 0x8000008CL )
#define VAR_GT_CLI0_MUTEX_PRIO                     ( 0x8000008DL )
#define VAR_GT_CLI1_MUTEX_PRIO                     ( 0x8000008EL )
#define VAR_GT_CLI2_MUTEX_PRIO                     ( 0x8000008FL )
#define VAR_GT_IPC0_MUTEX_PRIO                     ( 0x80000091L )
#define VAR_GT_IPC1_MUTEX_PRIO                     ( 0x80000092L )
#define VAR_GT_IPC2_MUTEX_PRIO                     ( 0x80000093L )
#define VAR_GT_INIT_FILE                           ( 0x80000094L )
#define VAR_GT_RSS_POST_INTERVAL                   ( 0x80000095L )
#define VAR_GT_RSS_FILE                            ( 0x80000096L )
#define VAR_GT_MD5CHECK_MANIFEST_FILE_PATH         ( 0x80000097L )
#define VAR_GT_MD5CHECK_MANIFEST_CHK_FILE_PATH     ( 0x80000098L )
#define VAR_GT_FILEVARS                            ( 0x8000009AL )
#define VAR_GT_FILEVARS_NBUFS                      ( 0x8000009BL )
#define VAR_GT_FILEVARS_BUFSIZE                    ( 0x8000009CL )
#define VAR_GT_HTTPS_TOKEN_BUFFER_SIZE             ( 0x8000009DL )
#define VAR_GT_KTICK                               ( 0x8000009EL )
#define VAR_GT_UPLOAD_TARGET_DRIVE                 ( 0x8000009FL )
#define VAR_GT_USER_ROOT                           ( 0x800000A0L )
#define VAR_GT_DATA_ROOT                           ( 0x800000A1L )
#define VAR_SYSINFO_SOFTWARE_VERSION               ( 0x800000A2L )
#define VAR_GT_SCHEDULER_MAX_JOBS                  ( 0x800000A3L )
#define VAR_GT_SCHEDULER_BUF_SIZE                  ( 0x800000A4L )
#define VAR_GT_SCHEDULER_WORKBUF_SIZE              ( 0x800000A5L )
#define VAR_GT_SCHEDULER_INTERVAL_MS               ( 0x800000A6L )
#define VAR_GT_SCHEDULER_EXECBUF_SIZE              ( 0x800000A7L )
#define VAR_GT_SCHEDULER_OUTPUT_FILE               ( 0x800000A8L )
#define VAR_GT_SCRIPTER_BUF_SIZE                   ( 0x800000A9L )
#define VAR_GT_SCRIPTER_OUTPUT_FILE                ( 0x800000AAL )
#define VAR_GT_SCRIPTER_QUEUE_LENGTH               ( 0x800000ABL )
#define VAR_GT_SCRIPTER_MAX_COMMAND_LENGTH         ( 0x800000ACL )
#define VAR_GT_SCRIPTER_MAX_NAME_LENGTH            ( 0x800000ADL )
#define VAR_GT_LUA_STATE                           ( 0x800000AEL )
#define VAR_GT_LUA_SYS_LIB_OUTPUT_FILE             ( 0x800000AFL )
#define VAR_GT_LUA_QUEUE_LENGTH                    ( 0x800000B0L )
#define VAR_GT_LUA_MAX_NAME_LENGTH                 ( 0x800000B1L )
#define VAR_GT_LUA_SYS_LIB_MAX_NAME_LENGTH         ( 0x800000B2L )
#define VAR_GT_UTC_OFFSET_S_LABEL                  ( 0x800000B3L )
#define VAR_GT_FLTMGR_EXTENSION                    ( 0x800000B4L )
#define VAR_GT_FTP_USER_1                          ( 0x800000B5L )
#define VAR_GT_FTP_PASS_1                          ( 0x800000B6L )
#define VAR_GT_FTP_PATH_1                          ( 0x800000B7L )
#define VAR_GT_FTP_FLAGS_1                         ( 0x800000B8L )
#define VAR_GT_FTP_USER_2                          ( 0x800000B9L )
#define VAR_GT_FTP_PASS_2                          ( 0x800000BAL )
#define VAR_GT_FTP_PATH_2                          ( 0x800000BBL )
#define VAR_GT_FTP_FLAGS_2                         ( 0x800000BCL )
#define VAR_GT_USB_MSD_WRITE_PROTECT_VOLUME0       ( 0x800000BDL )
#define VAR_GT_USB_MSD_WRITE_PROTECT_VOLUME1       ( 0x800000BEL )
#define VAR_GT_USB_MSD_VOLUME0                     ( 0x800000BFL )
#define VAR_GT_USB_MSD_VOLUME1                     ( 0x800000C0L )
#define VAR_GT_UPLOAD_WORK_DIR                     ( 0x800000C1L )
#define VAR_GT_SCRIPTER_STARTUP_FILE               ( 0x800000C2L )
#define VAR_GT_LUA_STARTUP_FILE                    ( 0x800000C3L )
#define VAR_GT_LCD_BACKLIGHT                       ( 0x800000C4L )
#define VAR_GT_AUTH_SSNLIFE                        ( 0x800000C5L )
#define VAR_GT_HTTPSSL_PORT                        ( 0x800000C6L )
#define VAR_GT_WEB_ACCESS                          ( 0x800000C7L )
#define VAR_GT_OSCILLOGRAPHY_EXTENSION             ( 0x800000C8L )
#define VAR_GT_OSCILLOGRAPHY_BUFSIZE               ( 0x800000C9L )
#define VAR_GT_OSCILLOGRAPHY_FILE_CREATION_INTERVAL (0x800000CAL )
#define VAR_GT_OSCILLOGRAPHY_TIMEBASE              ( 0x800000CBL )
#define VAR_GT_OSCILLOGRAPHY_TEMPLATE_FILENAME     ( 0x800000CCL )
#define VAR_GT_OSCILLOGRAPHY_BASEDIR               ( 0x800000CDL )
#define VAR_GT_OSCILLOGRAPHY_ACTIVE_FILENAME       ( 0x800000CEL )
#define VAR_GT_IPC_DEBUG_LEVEL                     ( 0x800000CFL )
#define VAR_GT_AUTH_NAMEOFUSER                     ( 0x800000D0L )
#define VAR_GT_AUTH_NAMEOFADMIN                    ( 0x800000D1L )
#define VAR_GT_AUTH_PASSOFUSER                     ( 0x800000D2L )
#define VAR_GT_AUTH_PASSOFADMIN                    ( 0x800000D3L )
#define VAR_GT_PVSCAN_EXTENSION                    ( 0x800000D4L )
#define VAR_GT_PVSCAN_BUFSIZE                      ( 0x800000D5L )
#define VAR_GT_PVSCAN_FILE_CREATION_INTERVAL       ( 0x800000D6L )
#define VAR_GT_PVSCAN_TIMEBASE                     ( 0x800000D7L )
#define VAR_GT_PVSCAN_BASEDIR                      ( 0x800000D8L )
#define VAR_GT_PVSCAN_TEMPLATE_FILENAME            ( 0x800000D9L )
#define VAR_GT_PVSCAN_ACTIVE_FILENAME              ( 0x800000DAL )
#define VAR_GT_TEMP_ROOT                           ( 0x800000DBL )
#define VAR_GT_APS_ENABLED                         ( 0x800000DCL )
#define VAR_GT_APSMON_ON_DELAY                     ( 0x800000DDL )
#define VAR_GT_APSMON_OFF_DELAY                    ( 0x800000DEL )
#define VAR_GT_APSMON_INTERVAL                     ( 0x800000DFL )
#define VAR_GT_AUTH_ACCOUNT_FMT                    ( 0x800000E0L )
#define VAR_GT_DHCP_IP                             ( 0x800000E1L )
#define VAR_GT_DHCP_NET_MASK                       ( 0x800000E2L )
#define VAR_GT_DHCP_GATEWAY                        ( 0x800000E3L )
#define VAR_GT_GTTL_STATE                          ( 0x800000E4L )
#define VAR_GT_HTTPSSL_ENABLED                     ( 0x800000E5L )
#define VAR_EXEC                                   ( 0x800000E6L )
#define VAR_EXEC_BUFSIZE                           ( 0x800000E7L )
#define VAR_REBOOT_DELAY_SECS                      ( 0x800000E8L )
#define VAR_GT_TCPIP_TEST                          ( 0x800000E9L )
#define VAR_GT_SYSCFG_DBGLVL                       ( 0x800000EAL )
#define VAR_SYS_OK                                 ( 0x800000EBL )
#define VAR_GT_DSPCFG_RUN                          ( 0x800000ECL )
#define VAR_GT_DSPCFG_OK                           ( 0x800000EDL )
#define VAR_GT_DSPSTS_RUN                          ( 0x800000EEL )
#define VAR_GT_DSPCFG_DBGLVL                       ( 0x800000EFL )
#define VAR_GT_GTTL_DBGLVL                         ( 0x800000F0L )
#define VAR_GT_IPC1_MSG_LOST                       ( 0x800000F1L )
#define VAR_GT_INFOVARS_FILE                       ( 0x800000F2L )
#define VAR_GT_GTTL_INIT_TIMEOUT_SECS              ( 0x800000F3L )
#define VAR_GT_GTTL_APS_TIMEOUT_SECS               ( 0x800000F4L )
#define VAR_GT_GTTL_DSP_POWER_TIMEOUT_SECS         ( 0x800000F5L )
#define VAR_GT_GTTL_DSP_CONFIG_TIMEOUT_SECS        ( 0x800000F6L )
#define VAR_GT_GTTL_ERROR_TIMEOUT_SECS             ( 0x800000F7L )
#define VAR_GT_GTTL_NIGHTTIME_TIMEOUT_SECS         ( 0x800000F8L )
#define VAR_GT_FLTMGR_ENABLE_MASK1                 ( 0x800000F9L )
#define VAR_GT_FLTMGR_EMAIL_MASK1                  ( 0x800000FAL )
#define VAR_GT_FLTMGR_FAULT_STATE1                 ( 0x800000FBL )
#define VAR_GT_FLTMGR_FAULTCOUNT                   ( 0x800000FCL )
#define VAR_GT_AUTH_NAMEOFDESIGNER                 ( 0x800000FDL )
#define VAR_GT_AUTH_NAMEOFTESTER                   ( 0x800000FEL )
#define VAR_GT_AUTH_PASSOFDESIGNER                 ( 0x800000FFL )
#define VAR_GT_AUTH_PASSOFTESTER                   ( 0x80000100L )
#define VAR_GT_MAILER_ENABLE                       ( 0x80000101L )
#define VAR_GT_FLTMGR_MAILER                       ( 0x80000102L )
#define VAR_GT_DATALOG_ENABLE_MASK                 ( 0x80000103L )
#define VAR_GT_DATALOG_DURATION                    ( 0x80000104L )
#define VAR_GT_DATALOG_START                       ( 0x80000105L )
#define VAR_GT_DATALOG_STOP                        ( 0x80000106L )
#define VAR_GT_DATALOG_MAX_RECORDS                 ( 0x80000107L )
#define VAR_GT_PTY0_PORT                           ( 0x80000108L )
#define VAR_GT_PTY0_ENABLE                         ( 0x80000109L )
#define VAR_GT_FLTMGR_RELAY_MASK0                  ( 0x8000010AL )
#define VAR_GT_FLTMGR_RELAY_MASK1                  ( 0x8000010BL )
#define VAR_GT_FLTMGR_RELAY_ACTIVE                 ( 0x8000010CL )
#define VAR_GT_DSPSTS_STATIC_VARS                  ( 0x8000010DL )
#define VAR_SYS_UP                                 ( 0x8000010EL )
#define VAR_GT_PHYSCAN                             ( 0x8000010FL )
#define VAR_GT_PHYSCAN_INFO_DIR                    ( 0x80000110L )
#define VAR_SYS_TEST                               ( 0x80000111L )
#define VAR_GT_CONDITION_TEST_VAR16                ( 0x80000112L )
#define VAR_GT_CONDITION_TEST_VAR32                ( 0x80000113L )
#define VAR_GT_CONDITION_TEST_STR                  ( 0x80000114L )
#define VAR_GT_SNTP_ON                             ( 0x80000115L )
#define VAR_GT_CONDITION_MAX_WKNG_BUF_LEN          ( 0x80000116L )
#define VAR_GT_CONDITION_DEBUG                     ( 0x80000117L )
#define VAR_GT_CONDITION_MAX_RSLT_BUF_LEN          ( 0x80000118L )
#define VAR_GT_APSMON_UPTIME_SECS                  ( 0x80000119L )
#define VAR_GT_SYSTEM_VOLUME                       ( 0x8000011AL )
#define VAR_GT_MAILER_USERID                       ( 0x8000011BL )
#define VAR_GT_MAILER_PASSWORD                     ( 0x8000011CL )
#define VAR_GT_USBH0_VOLUME_LABEL                  ( 0x8000011DL )
#define VAR_GT_USBH1_VOLUME_LABEL                  ( 0x8000011EL )
#define VAR_GT_SENDMAIL_MAIL_FROM                  ( 0x8000011FL )
#define VAR_GT_FLTMGR_MAIL_FROM                    ( 0x80000120L )
#define VAR_GT_FLTMGR_ACTIVEFAULTS                 ( 0x80000121L )
#define VAR_GT_DNS                                 ( 0x80000122L )
#define VAR_GT_CLI0_DEBUG_LEVEL                    ( 0x80000123L )
#define VAR_GT_CLI1_DEBUG_LEVEL                    ( 0x80000124L )
#define VAR_GT_EVTMGR_QUEUE_SIZE                   ( 0x80000125L )
#define VAR_GT_EVTMGR_QUEUE_BUFFER_SIZE            ( 0x80000126L )
#define VAR_GT_USERVARS_FILE                       ( 0x80000127L )
#define VAR_GT_CLI2_DEBUG_LEVEL                    ( 0x80000128L )
#define VAR_GT_IPC2_QUEUE_SIZE                     ( 0x80000129L )
#define VAR_GT_IPC2_BUFFER_SIZE                    ( 0x8000012AL )
#define VAR_GT_IPC2_RETRIES                        ( 0x8000012BL )
#define VAR_GT_IPC2_TIMEOUT                        ( 0x8000012CL )
#define VAR_GT_IPC2_DEVICE                         ( 0x8000012DL )
#define VAR_GT_IPC2_MSG_LOST                       ( 0x8000012EL )
#define VAR_GT_OSCILLOGRAPHY_QUEUE_SIZE            ( 0x8000012FL )
#define VAR_DUI_BUF_SIZE                           ( 0x80000130L )
#define VAR_DUI_NUM_TRIGGERS                       ( 0x80000131L )
#define VAR_DUI_TRIGGER_LENGTH                     ( 0x80000132L )
#define VAR_DUI_TARGET_LENGTH                      ( 0x80000133L )
#define VAR_USB_DEV_STATE                          ( 0x80000134L )
#define VAR_UPLOADMGR_CONFIG                       ( 0x80000135L )
#define VAR_UPLOADMGR_DECODE_PROGRESS              ( 0x80000136L )
#define VAR_UPLOADMGR_EXTRACT_PROGRESS             ( 0x80000137L )
#define VAR_USBHOST_DBG_LVL                        ( 0x80000138L )
#define VAR_DUI_DBG_LVL                            ( 0x80000139L )
#define VAR_UPLOADMGR_DBG_LVL                      ( 0x8000013AL )
#define VAR_GT_HTTP_JSON_PATH                      ( 0x8000013BL )
#define VAR_GT_TESTMOD_VAR16                       ( 0x8000013CL )
#define VAR_GT_TESTMOD_VAR32                       ( 0x8000013DL )
#define VAR_GT_TESTMOD_STR                         ( 0x8000013EL )
#define VAR_LUA_MD5_REQUIRED                       ( 0x8000013FL )
#define VAR_CTRIG_TASK_PRIORITY                    ( 0x80000140L )
#define VAR_CTRIG_STACK_SIZE                       ( 0x80000141L )
#define VAR_CTRIG_NUM_TRIGGERS                     ( 0x80000142L )
#define VAR_CTRIG_OUTPUT_BUFFER_SIZE               ( 0x80000143L )
#define VAR_CTRIG_COMPARE_LENGTH                   ( 0x80000144L )
#define VAR_CTRIG_MAX_COMMAND_LENGTH               ( 0x80000145L )
#define VAR_CTRIG_DBG_LVL                          ( 0x80000146L )
#define VAR_LUA_TASK_PRIO                          ( 0x80000147L )
#define VAR_LUA_STACK_SIZE                         ( 0x80000148L )
#define VAR_LUA_DBGLVL                             ( 0x80000149L )
#define VAR_LUA_PATH                               ( 0x8000014AL )
#define VAR_I2C2_DBG_LVL                           ( 0x8000014BL )
#define VAR_I2C2_RX_BUF_SIZE                       ( 0x8000014CL )
#define VAR_I2C2_RX_TIMEOUT                        ( 0x8000014DL )
#define VAR_I2C2TEST_I2C2_TARGET                   ( 0x8000014EL )
#define VAR_USB_KBD_RXBUFSIZE                      ( 0x8000014FL )
#define VAR_USB_KBD_TIMEOUT                        ( 0x80000150L )
#define VAR_USB_KBD_DBG_LVL                        ( 0x80000151L )
#define VAR_KBD_CLI_ECHO                           ( 0x80000152L )
#define VAR_KBD_CLI_CONSOLE_OUTPUT                 ( 0x80000153L )
#define VAR_KBD_CLI_TASK_PRIO                      ( 0x80000154L )
#define VAR_KBD_CLI_PROMPT                         ( 0x80000155L )
#define VAR_GT_SCHEDULER_OUTPUT_DEVICE             ( 0x80000156L )
#define VAR_I2C2_STATS                             ( 0x80000157L )
#define VAR_I2C2_FREQ_KHZ                          ( 0x80000158L )
#define VAR_I2C2_RETRIES                           ( 0x80000159L )
#define VAR_I2C2TEST_I2C2_PRIO                     ( 0x8000015AL )
#define VAR_I2C2_MUNGE_RATE                        ( 0x8000015BL )
#define VAR_I2C2_TEST_MODE                         ( 0x8000015CL )
#define VAR_DBGSTREAM_ENABLE                       ( 0x8000015DL )
#define VAR_UDPDEV_MAX                             ( 0x8000015EL )
#define VAR_DBGSTREAM_DEVICE                       ( 0x8000015FL )
#define VAR_DUI_TASK_PRIO                          ( 0x80000160L )
#define VAR_XB_DEBUG_LEVEL                         ( 0x80000161L )
#define VAR_XB_TASK_PRIO                           ( 0x80000162L )
#define VAR_SYSINFO_SW_VERSION_NUMBER              ( 0x80000163L )
#define VAR_SYSINFO_SW_BUILD_NUMBER                ( 0x80000164L )
#define VAR_SYSINFO_HW_MODEL_NUMBER                ( 0x80000165L )
#define VAR_SYSINFO_HW_MODEL_REVISION              ( 0x80000166L )
#define VAR_IPC_MAX_PKT_SIZE                       ( 0x80000167L )
#define VAR_IPC_MAX_PKT_NUM                        ( 0x80000168L )
#define VAR_IPC_LOCAL_ADDR                         ( 0x80000169L )
#define VAR_IPC_DEST_ADDR                          ( 0x8000016AL )
#define VAR_IPC_DBGLVL                             ( 0x8000016BL )
#define VAR_IPCRX1_STACK_SIZE                      ( 0x8000016CL )
#define VAR_IPCRX1_PRIO                            ( 0x8000016DL )
#define VAR_IPCRX1_DEVICE                          ( 0x8000016EL )
#define VAR_IPCTEST_DEVICE                         ( 0x8000016FL )
#define VAR_IPCTEST_PRIO                           ( 0x80000170L )
#define VAR_KEYDRV_DEBOUNCE_MS                     ( 0x80000171L )
#define VAR_LEDDRV_PRIO                            ( 0x80000172L )
#define VAR_LEDDRV_TICK_MS                         ( 0x80000173L )
#define VAR_LEDDRV_ON_COUNT                        ( 0x80000174L )
#define VAR_LEDDRV_OFF_COUNT                       ( 0x80000175L )
#define VAR_LED_MODE                               ( 0x80000176L )
#define VAR_LED_FLASH_RATE                         ( 0x80000177L )
#define VAR_KEYDRV_TIMEOUT_TICKS                   ( 0x80000178L )
#define VAR_KEYDRV_DBGLVL                          ( 0x80000179L )
#define VAR_IPCVARS_PRIO                           ( 0x8000017AL )
#define VAR_IPCVARS_MAX_SHARED                     ( 0x8000017BL )
#define VAR_IPCVARS_TX_BUF_SIZE                    ( 0x8000017CL )
#define VAR_IPCVARS_MAX_STREAMS                    ( 0x8000017DL )
#define VAR_IPCVARS_DEVICE                         ( 0x8000017EL )
#define VAR_IPCVARS_DBGLVL                         ( 0x8000017FL )
#define VAR_IPCVARS_INTERVAL                       ( 0x80000180L )
#define VAR_IPCVARCFG_IPCVARCFG                    ( 0x80000181L )
#define VAR_IPCVARCFG_PRIO                         ( 0x80000182L )
#define VAR_IPCVARCFG_DBGLVL                       ( 0x80000183L )
#define VAR_IPCVARCFG_IPCVARS                      ( 0x80000184L )
#define VAR_IPCVARS_CACHE_SIZE                     ( 0x80000185L )
#define VAR_NVCFG_NUMITEMS                         ( 0x80000186L )
#define VAR_NVCFG_MEMUSED                          ( 0x80000187L )
#define VAR_NVCFG_DBGLVL                           ( 0x80000188L )
#define	VAR_DCDC_BOOST_ENABLE                      ( 0x80000189L )
#define VAR_MEAS_DBGLVL                            ( 0x8000018AL )
#define VAR_IPCTEST_EN                             ( 0x8000018BL )
#define VAR_DIGCTRL_BOOST1_EN                      ( 0x8000018CL )
#define VAR_DIGCTRL_BOOST2_EN                      ( 0x8000018DL )
#define VAR_DIGCTRL_BOOST3_EN                      ( 0x8000018EL )
#define VAR_DIGCTRL_BOOST4_EN                      ( 0x8000018FL )
#define VAR_DIGCTRL_DCLINK_EN                      ( 0x80000190L )
#define VAR_DIGCTRL_PVV_EN                         ( 0x80000191L )
#define VAR_DIGCTRL_DCLINK_VREF                    ( 0x80000192L )
#define VAR_MUNGETEST_EN                           ( 0x80000193L )
#define VAR_MODULE1_READY                          ( 0x80000194L )
#define VAR_MODULE2_READY                          ( 0x80000195L )
#define VAR_MODULE3_READY                          ( 0x80000196L )
#define VAR_MODULE4_READY                          ( 0x80000197L )
#define VAR_MODULE_CONFIG_1                        ( 0x80000198L )
#define VAR_MODULE_CONFIG_2                        ( 0x80000199L )
#define VAR_DIGCTRL_VPV1_EN                        ( 0x8000019AL )
#define VAR_DIGCTRL_VPV2_EN                        ( 0x8000019BL )
#define VAR_DIGCTRL_VPV3_EN                        ( 0x8000019CL )
#define VAR_DIGCTRL_VPV4_EN                        ( 0x8000019DL )
#define VAR_APS_STATUS                             ( 0x8000019EL )
#define VAR_OPERATION_MODE                         ( 0x8000019FL )
#define VAR_OPERATION_ENABLE                       ( 0x800001A0L )
#define VAR_DPWS                                   ( 0x800001A1L )
#define VAR_DPWS_DBGLVL                            ( 0x800001A2L )
#define VAR_NETIF_NBR                              ( 0x800001A3L )
#define VAR_DPWS_VENDOR_URL                        ( 0x800001A4L )
#define VAR_DPWS_MODEL_URL                         ( 0x800001A5L )
#define VAR_DPWS_STACKSIZE                         ( 0x800001A6L )
#define VAR_TIME_LOCAL_MDHMS                       ( 0x800001A7L )
#define VAR_KSYSLOG_ENABLE                         ( 0x800001A8L )
#define VAR_KSYSLOG_LEVEL                          ( 0x800001A9L )
#define VAR_KSYSLOG_DEVICE                         ( 0x800001AAL )

#define VAR_KERNEL_VERSION                         ( 0x81000000L )

#define VAR_GT_CTLBOOT_VERSION                     ( 0x85000000L )
#define VAR_GT_CTLBOOT_BUILD_MACHINE               ( 0x85000001L )
#define VAR_GT_CTLBOOT_BUILD_USER                  ( 0x85000002L )
#define VAR_GT_CTLBOOT_BUILD_DATE                  ( 0x85000003L )
#define VAR_GT_COMAPP_VERSION                      ( 0x85000004L )
#define VAR_GT_COMAPP_BUILD_MACHINE                ( 0x85000005L )
#define VAR_GT_COMAPP_BUILD_USER                   ( 0x85000006L )
#define VAR_GT_COMAPP_BUILD_DATE                   ( 0x85000007L )
#define VAR_GT_CTLAPP_VERSION                      ( 0x85000008L )
#define VAR_GT_CTLAPP_BUILD_MACHINE                ( 0x85000009L )
#define VAR_GT_CTLAPP_BUILD_USER                   ( 0x8500000AL )
#define VAR_GT_CTLAPP_BUILD_DATE                   ( 0x8500000BL )
#define VAR_GT_COMBOOT_VERSION                     ( 0x8500000CL )
#define VAR_GT_COMBOOT_BUILD_MACHINE               ( 0x8500000DL )
#define VAR_GT_COMBOOT_BUILD_USER                  ( 0x8500000EL )
#define VAR_GT_COMBOOT_BUILD_DATE                  ( 0x8500000FL )
#define VAR_GT_RCD_VERSION                         ( 0x85000010L )
#define VAR_GT_RCD_BUILD_MACHINE                   ( 0x85000011L )
#define VAR_GT_RCD_BUILD_USER                      ( 0x85000012L )
#define VAR_GT_RCD_BUILD_DATE                      ( 0x85000013L )
#define VAR_GT_FRIENDLYNAME                        ( 0x85000014L )
#define VAR_GT_MDL_POWER_RATING                    ( 0x85000015L )
#define VAR_GT_MDL_TEMP1_MIN                       ( 0x85000016L )
#define VAR_GT_MDL_TEMP1_MAX                       ( 0x85000017L )
#define VAR_GT_MDL_AC_CURRENT_MIN                  ( 0x85000018L )
#define VAR_GT_MDL_AC_CURRENT_MAX                  ( 0x85000019L )
#define VAR_GT_MDL_DC_CURRENT_MIN                  ( 0x8500001AL )
#define VAR_GT_MDL_DC_CURRENT_MAX                  ( 0x8500001BL )
#define VAR_GT_LCD_ADDRESS                         ( 0x8500001CL )
#define VAR_KBD_TASK_PRIO                          ( 0x8500001DL )
#define VAR_KBD_DBG_LVL                            ( 0x8500001EL )
#define VAR_SNTP_DBGLVL                            ( 0x8500001FL )
#define VAR_MBSERVER_DBGLVL                        ( 0x85000020L )
#define VAR_MBSERVER_DEV_ID_MAP                    ( 0x85000021L )
#define VAR_SYSINFO_VENDOR_NAME                    ( 0x85000022L )
#define VAR_SYSINFO_PRODUCT_CODE                   ( 0x85000023L )
#define VAR_SYSINFO_REVISION                       ( 0x85000024L )
#define VAR_SYSINFO_SOFTWARE_NAME                  ( 0x85000025L )
#define VAR_MBSERIAL_DBGLVL                        ( 0x85000026L )
#define VAR_UART2_BAUDRATE                         ( 0x85000027L )
#define VAR_MAILER_DBGLVL                          ( 0x85000028L )
#define VAR_DUI_DEVICE                             ( 0x85000029L )
#define VAR_KEYSERVER_DBGLVL                       ( 0x8500002AL )
#define VAR_KEYSERVER_DEVICE                       ( 0x8500002BL ) 
#define VAR_MBSERIAL_PRIO                          ( 0x8500002CL ) 
#define VAR_PMDB_BACKLIGHT                         ( 0x8500002DL )
#define VAR_FONT_DBGLVL                            ( 0x8500002EL )
#define VAR_SSP0_DBGLVL                            ( 0x8500002FL ) 

 
/* 0x85100000 - 0x85200000 reserved for application specific internal use */

/* file variables */
#define VAR_GT_EVENTS_FILE                         ( 0x80100000L )
#define VAR_GT_FAULTS_FILE                         ( 0x80100001L )
#define VAR_GT_LCD_TEXT_FILE                       ( 0x80100002L )
#define VAR_GT_WEB_STYLE_FILE                      ( 0x80100003L )
#define VAR_GT_GREETING_FILE                       ( 0x80100004L )
#define VAR_GT_WEB_TAB_HEADER                      ( 0x80100005L )
#define VAR_GT_WEB_TAB_FOOTER                      ( 0x80100006L )
#define VAR_GT_WEB_PAGE_HEADER                     ( 0x80100007L )
#define VAR_GT_WEB_TD_LOGO                         ( 0x80100008L )
#define VAR_GT_WEB_ADMIN_TAB_HEADER                ( 0x80100009L )
#define VAR_GT_USER_SETTINGS                       ( 0x8010000AL )
#define VAR_GT_WEB_PAGE_FOOTER                     ( 0x8010000BL )
#define VAR_GT_EMAIL_STYLE_FILE                    ( 0x8010000CL )
#define VAR_GT_INSTRUCTION_FILE                    ( 0x8010000DL )

/* energy logging */
#define VAR_GT_ENERGY_LOG_REC_PERIOD_SEC                ( 0x80100100L )
#define VAR_GT_ENERGY_LOG_TEMPLATE_FILENAME             ( 0x80100101L )
#define VAR_GT_ENERGY_LOG_BASE_DIR                      ( 0x80100102L )
#define VAR_GT_ENERGY_LOG_REND_BUF_SIZE                 ( 0x80100103L )
#define VAR_GT_ENERGY_LOG_SAMPLE_PERIOD_SEC             ( 0x80100104L )
#define VAR_GT_ENERGY_LOG_EXTENSION                     ( 0x80100105L )
#define VAR_GT_ENERGY_LOG_RENDER_BUF_DEV_NAME           ( 0x80100106L )
#define VAR_GT_ENERGY_LOG_FCI                           ( 0x80100107L )
#define VAR_GT_ENERGY_LOG_DATAHEADER                    ( 0x80100108L )
#define VAR_GT_ENERGY_LOG_LOW_LIFETIME_ENERGY_LOW_WSEC  ( 0x80100109L )
#define VAR_GT_ENERGY_LOG_HIGH_LIFETIME_ENERGY_100WH    ( 0x8010010AL )
#define VAR_GT_ENERGY_LOG_TIMESTAMP_FORMAT              ( 0x8010010BL )
#define VAR_GT_ENERGY_LOG_ENABLE_DEBUG_MSGS             ( 0x8010010CL )
#define VAR_GT_ENERGY_LOG_TIMEBASE                      ( 0x8010010DL )
#define VAR_GT_ENERGY_LOG_AGG_HOUR_CURRENT              ( 0x8010010EL )
#define VAR_GT_ENERGY_LOG_AGG_HOUR_PREVIOUS             ( 0x8010010FL )
#define VAR_GT_ENERGY_LOG_AGG_DAY_CURRENT               ( 0x80100110L )
#define VAR_GT_ENERGY_LOG_AGG_DAY_PREVIOUS              ( 0x80100111L )
#define VAR_GT_ENERGY_LOG_AGG_MONTH_CURRENT             ( 0x80100112L )
#define VAR_GT_ENERGY_LOG_AGG_MONTH_PREVIOUS            ( 0x80100113L )
#define VAR_GT_ENERGY_LOG_AGG_YEAR_CURRENT              ( 0x80100114L )
#define VAR_GT_ENERGY_LOG_AGG_YEAR_PREVIOUS             ( 0x80100115L )
#define VAR_GT_ENERGY_LOG_ENABLE_TEST_SPEEDUP           ( 0x80100116L )
#define VAR_GT_ENERGY_LOG_ENABLE_TEST_VALUE_EN          ( 0x80100117L )
#define VAR_GT_ENERGY_LOG_ENABLE_TEST_VALUE_RATE        ( 0x80100118L )
#define VAR_GT_ENERGY_LOG_ENABLE_TEST_VALUE             ( 0x80100119L )
#define VAR_GT_ENERGY_BACKUP_BASE_DIR                   ( 0x8010011AL )
#define VAR_GT_ENERGY_BACKUP_EXTENSION                  ( 0x8010011BL )
#define VAR_GT_ENERGY_BACKUP_TEMPLATE_FILENAME          ( 0x8010011CL )
#define VAR_GT_ENERGY_LOG_ENABLE_TEST_VALUE_CHG         ( 0x8010011DL )


#define VAR_GT_SWSYNC_LOST_GRID                    ( 0x9FF00000L )
#define VAR_GT_SWSYNC_TASK                         ( 0x9FF00001L )
#define VAR_GT_SWSYNC_INSYNC                       ( 0x9FF00002L )
#define VAR_GT_SWSYNC_ENABLE                       ( 0x9FF00003L )
#define VAR_GT_SWSYNC_PF_SET                       ( 0x9FF00004L )
#define VAR_GT_SWSYNC_PF_ACTIVE                    ( 0x9FF00005L )
#define VAR_GT_SWSYNC_INST_FREQ                    ( 0x9FF00006L )

#define VAR_GT_MEAS_TASK                           ( 0x9FF00007L )
#define VAR_GT_MEAS_AVG_FREQ                       ( 0x9FF00008L )
#define VAR_GT_MEAS_INST_TEMP                      ( 0x9FF00009L )
#define VAR_GT_MEAS_FS_TEMP                        ( 0x9FF0000AL )
#define VAR_GT_MEAS_PVPWR                          ( 0x9FF0000BL )
#define VAR_GT_MEAS_INST_IAC                       ( 0x9FF0000CL )
#define VAR_GT_MEAS_RMS_IAC                        ( 0x9FF0000DL )
#define VAR_GT_MEAS_INST_VAC                       ( 0x9FF0000EL )
#define VAR_GT_MEAS_RMS_VAC                        ( 0x9FF0000FL )
#define VAR_GT_MEAS_INST_VACRELAY                  ( 0x9FF00010L )
#define VAR_GT_MEAS_RMS_VACRELAY                   ( 0x9FF00011L )
#define VAR_GT_MEAS_INST_IACISOLATION              ( 0x9FF00012L )
#define VAR_GT_MEAS_RMS_IACISOLATION               ( 0x9FF00013L )
#define VAR_GT_MEAS_INST_IPV                       ( 0x9FF00014L )
#define VAR_GT_MEAS_FS_IPV                         ( 0x9FF00015L )
#define VAR_GT_MEAS_INST_VPV                       ( 0x9FF00016L )
#define VAR_GT_MEAS_FS_VPV                         ( 0x9FF00017L )
#define VAR_GT_MEAS_INST_IDCDC                     ( 0x9FF00018L )
#define VAR_GT_MEAS_FS_IDCDC                       ( 0x9FF00019L )
#define VAR_GT_MEAS_INST_VDCDC                     ( 0x9FF0001AL )
#define VAR_GT_MEAS_FS_VDCDC                       ( 0x9FF0001BL )
#define VAR_GT_MEAS_PACREALPWR                     ( 0x9FF0001CL )
#define VAR_GT_MEAS_PACAPPARENTPWR                 ( 0x9FF0001DL )
#define VAR_GT_MEAS_IACDCCOMP                      ( 0x9FF0001EL )
#define VAR_GT_MEAS_RATE_RMS                       ( 0x9FF0001FL )
#define VAR_GT_MEAS_RATE_DC                        ( 0x9FF00020L )
#define VAR_GT_MEAS_RATE_AC                        ( 0x9FF00021L )
#define VAR_GT_MEAS_RATE_TEMP                      ( 0x9FF00022L )

#define VAR_GT_FS_SCALE_VDCDC                      ( 0x9FF00023L )
#define VAR_GT_FS_SCALE_IDCDC                      ( 0x9FF00024L )
#define VAR_GT_FS_SCALE_IPV                        ( 0x9FF00025L )
#define VAR_GT_FS_SCALE_VPV                        ( 0x9FF00026L )
#define VAR_GT_FS_SCALE_TEMP                       ( 0x9FF00027L )
#define VAR_GT_FS_SCALE_VAC                        ( 0x9FF00028L )
#define VAR_GT_FS_SCALE_VACRELAY                   ( 0x9FF00029L )
#define VAR_GT_FS_SCALE_IAC                        ( 0x9FF0002AL )
#define VAR_GT_FS_SCALE_IACISOLATION               ( 0x9FF0002BL )
#define VAR_GT_FS_SCALE_IACDCCOMP                  ( 0x9FF0002CL )
#define VAR_GT_FS_SCALE_PWR_REAL                   ( 0x9FF0002DL )
#define VAR_GT_FS_SCALE_PWR_APPARENT               ( 0x9FF0002EL )
#define VAR_GT_FS_OFFSET_VDCDC                     ( 0x9FF0002FL )
#define VAR_GT_FS_OFFSET_IDCDC                     ( 0x9FF00030L )
#define VAR_GT_FS_OFFSET_VPV                       ( 0x9FF00031L )
#define VAR_GT_FS_OFFSET_IPV                       ( 0x9FF00032L )
#define VAR_GT_FS_OFFSET_TEMP                      ( 0x9FF00033L )
#define VAR_GT_FS_OFFSET_VAC                       ( 0x9FF00034L )
#define VAR_GT_FS_OFFSET_IAC                       ( 0x9FF00035L )
#define VAR_GT_FS_OFFSET_VACRELAY                  ( 0x9FF00036L )
#define VAR_GT_FS_OFFSET_IACISOLATION              ( 0x9FF00037L )
#define VAR_GT_FS_OFFSET_IACDCCOMP                 ( 0x9FF00038L )
#define VAR_GT_FS_OFFSET_PWR_REAL                  ( 0x9FF00039L )
#define VAR_GT_FS_OFFSET_PWR_APPARENT              ( 0x9FF0003AL )
#define VAR_GT_FS_OFFSET_VAC_L1                    ( 0x9FF0003BL )
#define VAR_GT_FS_OFFSET_VAC_L2                    ( 0x9FF0003CL )
#define VAR_GT_FS_SCALE_VAC_L1                     ( 0x9FF0003DL )
#define VAR_GT_FS_SCALE_VAC_L2                     ( 0x9FF0003EL )

#define VAR_GT_MPPT_TASK                           ( 0x9FF00040L )
#define VAR_GT_MPPT_DEFAULT_PERTURB_RATE           ( 0x9FF00041L )
#define VAR_GT_MPPT_STEPSIZE                       ( 0x9FF00042L )
#define VAR_GT_MPPT_VOLTAGE                        ( 0x9FF00043L )
#define VAR_GT_MPPT_VOC                            ( 0x9FF00044L )
#define VAR_GT_MPPT_MANUAL_SETPOINT                ( 0x9FF00045L )
#define VAR_GT_MPPT_MANUAL_VREF_MAX                ( 0x9FF00046L )
#define VAR_GT_MPPT_MANUAL_VREF_MIN                ( 0x9FF00047L )
#define VAR_GT_MPPT_VREF_MAX                       ( 0x9FF00048L )
#define VAR_GT_MPPT_VREF_OFF                       ( 0x9FF00049L )
#define VAR_GT_MPPT_POWER_LEVEL                    ( 0x9FF0004AL )
#define VAR_GT_MPPT_STATE                          ( 0x9FF0004BL )
#define VAR_GT_MPPT_ENABLE                         ( 0x9FF0004CL )
#define VAR_GT_PVSCAN_RATE                         ( 0x9FF0004DL )
#define VAR_GT_PVSCAN_STEPSIZE                     ( 0x9FF0004EL )
#define VAR_GT_MPPT_MODE                           ( 0x9FF0004FL )
#define VAR_GT_PVSCAN_ENABLE                       ( 0x9FF00050L )
#define VAR_GT_MPPT_STEPSIZE_MIN                   ( 0x9FF00051L )
#define VAR_GT_MPPT_STEPSIZE_MAX                   ( 0x9FF00052L )
#define VAR_GT_MPPT_VREF_MIN                       ( 0x9FF00053L )
#define VAR_MPPT_MODULE1_EN                        ( 0x9FF00054L )
#define VAR_MPPT_MODULE2_EN                        ( 0x9FF00055L )
#define VAR_MPPT_MODULE3_EN                        ( 0x9FF00056L )
#define VAR_MPPT_MODULE4_EN                        ( 0x9FF00057L )
#define VAR_GT_MPPT_VOLTAGE1                       ( 0x9FF00058L )
#define VAR_GT_MPPT_VOLTAGE2                       ( 0x9FF00059L )
#define VAR_GT_MPPT_VOLTAGE3                       ( 0x9FF0005AL )
#define VAR_GT_MPPT_VOLTAGE4                       ( 0x9FF0005BL )

#define VAR_GT_MEAS_INST_VAC_L1                    ( 0x9FF00060L )
#define VAR_GT_MEAS_RMS_VAC_L1                     ( 0x9FF00061L )
#define VAR_GT_MEAS_INST_VAC_L2                    ( 0x9FF00062L )
#define VAR_GT_MEAS_RMS_VAC_L2                     ( 0x9FF00063L )
#define VAR_GT_MEAS_INST_TEMP_2                    ( 0x9FF00064L )
#define VAR_GT_MEAS_FS_TEMP_2                      ( 0x9FF00065L )

#define VAR_GT_FS_SCALE_TEMP_2                     ( 0x9FF00080L )
#define VAR_GT_FS_OFFSET_TEMP_2                    ( 0x9FF00090L )


#define  VAR_GT_SW_ENABLE                          ( 0x9FF00107L )
#define  VAR_GT_CTRL_TASK                          ( 0x9FF00110L )
#define  VAR_GT_CTRL_TMR_FREQ                      ( 0x9FF00111L )
#define  VAR_GT_CTRL_TASK_PRD                      ( 0x9FF00112L )
#define  VAR_AMP_CTRL_RATE_V                       ( 0x9FF00113L )
#define  VAR_AMP_CTRL_RATE_P                       ( 0x9FF00114L )
#define  VAR_AMP_CTRL_RATE_T                       ( 0x9FF00115L )
#define  VAR_AMP_CTRL_PWM                          ( 0x9FF00116L )
#define  VAR_AMP_CTRL_EN                           ( 0x9FF00117L )
#define  VAR_AMP_CTRL_STATUS                       ( 0x9FF00118L )
#define  VAR_AMP_CTRL_VKP                          ( 0x9FF00119L )
#define  VAR_AMP_CTRL_VKI                          ( 0x9FF0011AL )
#define  VAR_AMP_CTRL_PKP                          ( 0x9FF0011BL )
#define  VAR_AMP_CTRL_PKI                          ( 0x9FF0011CL )
#define  VAR_AMP_CTRL_TKP                          ( 0x9FF0011DL )
#define  VAR_AMP_CTRL_TKI                          ( 0x9FF00120L )
#define  VAR_GT_PV_REFERNCE_V                      ( 0x9FF00121L )
#define  VAR_PV_VOLTAGE                            ( 0x9FF00122L )
#define  VAR_AC_OUTPUT_P                           ( 0x9FF00124L )
#define  VAR_HEATSINK_TEMPERATURE                  ( 0x9FF00125L )
#define  VAR_HEATSINK_TEMPERATURE_LIMIT            ( 0x9FF00126L )
#define  VAR_PAC_TASK_PRD                          ( 0x9FF00127L )
#define  VAR_PAC_LIM_SET                           ( 0x9FF00128L )
#define  VAR_PAC_LIM_ACTIVE                        ( 0x9FF00129L )
#define  VAR_GT_OCP_TRIP_CNT                       ( 0x9FF0012AL )
#define  VAR_AMP_SET_PWM                           ( 0x9FF0012BL )
#define  VAR_GT_INV_MODE                           ( 0x9FF0012CL )
#define  VAR_GT_INV_TASK                           ( 0x9FF0012DL )
#define  VAR_GT_INV_STATE                          ( 0x9FF0012EL )
#define  VAR_GT_INV_STATE_BLOCK                    ( 0x9FF0012FL )
#define  VAR_FAN_TASK_PRD                          ( 0x9FF00130L )
#define  VAR_FAN_START_LVL                         ( 0x9FF00131L )
#define  VAR_FAN_ENABLE_SET                        ( 0x9FF00132L )
#define  VAR_FAN_PWM_DUTY                          ( 0x9FF00133L )
#define  VAR_FAN_PWM_GAIN                          ( 0x9FF00134L )
#define  VAR_GT_GRAPHY_TASK                        ( 0x9FF00135L )
#define  VAR_GT_GRAPHY_STATE                       ( 0x9FF00136L )
#define  VAR_GT_GRAPHY_DATA1                       ( 0x9FF00137L )
#define  VAR_GT_GRAPHY_DATA2                       ( 0x9FF00138L )
#define  VAR_GT_GRAPHY_DATA3                       ( 0x9FF00139L )
#define  VAR_GT_GRAPHY_DATA4                       ( 0x9FF0013AL )
#define  VAR_GT_GRAPHY_DATA5                       ( 0x9FF0013BL )
#define  VAR_GT_GRAPHY_DATA6                       ( 0x9FF0013CL )
#define  VAR_GT_GRAPHY_DATA7                       ( 0x9FF0013DL )
#define  VAR_GT_GRAPHY_DATA8                       ( 0x9FF0013EL )
#define  VAR_GT_GRAPHY_DATA9                       ( 0x9FF0013FL )
#define  VAR_GT_GRAPHY_DATA10                      ( 0x9FF00140L )
#define  VAR_GT_GRAPHY_DATA11                      ( 0x9FF00141L )
#define  VAR_GT_GRAPHY_DATA12                      ( 0x9FF00142L )
#define  VAR_GT_GRAPHY_DATA13                      ( 0x9FF00143L )
#define  VAR_GT_GRAPHY_DATA14                      ( 0x9FF00144L )
#define  VAR_GT_GRAPHY_DATA15                      ( 0x9FF00145L )
#define  VAR_GT_GRAPHY_DATA16                      ( 0x9FF00146L )
#define  VAR_GT_GRAPHY_DATA17                      ( 0x9FF00147L )
#define  VAR_GT_GRAPHY_DATA18                      ( 0x9FF00148L )
#define  VAR_GT_GRAPHY_DATA19                      ( 0x9FF00149L )
#define  VAR_GT_GRAPHY_DATA20                      ( 0x9FF0014AL )
#define  VAR_GT_INV_OP_ENABLE                      ( 0x9FF0014BL )
#define  VAR_PAC_DERATE_RATE                       ( 0x9FF0014CL )
#define  VAR_GT_BIST_TASK                          ( 0x9FF0014DL )
#define  VAR_GT_BIST_TASK_PRD                      ( 0x9FF0014EL )
#define  VAR_GT_BIST_ENABLE_ITEM                   ( 0x9FF0014FL )
#define  VAR_GT_BIST_RESULT_ITEM                   ( 0x9FF00150L )
#define  VAR_GT_BIST_STACK_BEAT                    ( 0x9FF00151L )
#define  VAR_AI_TASK_PRD                           ( 0x9FF00152L )
#define  VAR_AI_ENABLE                             ( 0x9FF00153L )
#define  VAR_AI_RATE                               ( 0x9FF00154L )
#define  VAR_AI_DURATION                           ( 0x9FF00155L )
#define  VAR_AI_CHK_AMOUNT                         ( 0x9FF00156L )
#define  VAR_GT_SWSYNC_AIPF_SET                    ( 0x9FF00157L )
#define  VAR_GT_SWSYNC_SENSOR_DELAY                ( 0x9FF00158L )
#define  VAR_ACISNS_TASK_PRD                       ( 0x9FF00159L )
#define  VAR_RELAY_TASK_PRD                        ( 0x9FF0015AL )
#define  VAR_GT_INV_3PHASE_ACTION                  ( 0x9FF0015BL )
#define  VAR_GT_CLI0_TOTAL_MSG                     ( 0x9FF0015CL )
#define  VAR_GT_CLI0_GOOD_MSG                      ( 0x9FF0015DL )
#define  VAR_GT_CLI1_TOTAL_MSG                     ( 0x9FF0015EL )
#define  VAR_GT_CLI1_GOOD_MSG                      ( 0x9FF0015FL )
#define  VAR_GT_CLI2_TOTAL_MSG                     ( 0x9FF00160L )
#define  VAR_GT_CLI2_GOOD_MSG                      ( 0x9FF00161L )
#define  VAR_GT_IPC0_TOTAL_MSG                     ( 0x9FF00162L )
#define  VAR_GT_IPC0_GOOD_MSG                      ( 0x9FF00163L )
#define  VAR_GT_IPC1_TOTAL_MSG                     ( 0x9FF00164L )
#define  VAR_GT_IPC1_GOOD_MSG                      ( 0x9FF00165L )
#define  VAR_GT_IPC2_TOTAL_MSG                     ( 0x9FF00166L )
#define  VAR_GT_IPC2_GOOD_MSG                      ( 0x9FF00167L )
#define  VAR_ACISNS_MODEL                          ( 0x9FF00168L )
#define  VAR_GT_INV_3PHASE_CHK                     ( 0x9FF00169L )
#define  VAR_FAN_PWM_DUTY_SET                      ( 0x9FF0016AL )
#define  VAR_PAC_DERATE_3PHASE                     ( 0x9FF0016BL )
#define  VAR_AMP_CTRL_V_ERR_MAX                    ( 0x9FF0016CL )
#define  VAR_AMP_CTRL_V_ERR_MIN                    ( 0x9FF0016DL )
#define  VAR_AMP_CTRL_P_ERR_MAX                    ( 0x9FF0016EL )
#define  VAR_AMP_CTRL_P_ERR_MIN                    ( 0x9FF0016FL )
#define  VAR_AMP_CTRL_T_ERR_MAX                    ( 0x9FF00170L )
#define  VAR_AMP_CTRL_T_ERR_MIN                    ( 0x9FF00171L )
#define  VAR_AMP_CTRL_V_Y_MAX                      ( 0x9FF00172L )
#define  VAR_AMP_CTRL_V_Y_MIN                      ( 0x9FF00173L )
#define  VAR_AMP_CTRL_P_Y_MAX                      ( 0x9FF00174L )
#define  VAR_AMP_CTRL_P_Y_MIN                      ( 0x9FF00175L )
#define  VAR_AMP_CTRL_T_Y_MAX                      ( 0x9FF00176L )
#define  VAR_AMP_CTRL_T_Y_MIN                      ( 0x9FF00177L )
#define  VAR_GT_GRAPHY_RESTART                     ( 0x9FF00178L )
#define  VAR_GT_INV_SLEEP                          ( 0x9FF00179L )
#define  VAR_DCOFFSET_TASK_PRD                     ( 0x9FF0017AL )
#define  VAR_DCOFFSET_PWM_DUTY                     ( 0x9FF0017BL )
#define  VAR_DCOFFSET_ENABLE                       ( 0x9FF0017CL )
#define  VAR_DCOFFSET_CTRL_KP                      ( 0x9FF0017DL )
#define  VAR_DCOFFSET_CTRL_KI                      ( 0x9FF0017EL )
#define  VAR_DCOFFSET_CTRL_ERR_MAX                 ( 0x9FF0017FL )
#define  VAR_DCOFFSET_CTRL_ERR_MIN                 ( 0x9FF00180L )
#define  VAR_DCOFFSET_CTRL_Y_MAX                   ( 0x9FF00181L )
#define  VAR_DCOFFSET_CTRL_Y_MIN                   ( 0x9FF00182L )
#define  VAR_DCOFFSET_SET_DUTY                     ( 0x9FF00183L )
#define  VAR_ACISNS_SELFTEST                       ( 0x9FF00184L )
#define  VAR_GT_GRAPHY_NUM_BEFORE_TRIP             ( 0x9FF00185L )
#define  VAR_GT_GRAPHY_TRIP                        ( 0x9FF00186L )

/***************************************************************/
#define  VAR_S3P_MEAS_INST_TEMP_MOD_A               ( 0x9FF00187L )
#define  VAR_S3P_MEAS_FS_TEMP_MOD_A                 ( 0x9FF00188L )
#define  VAR_S3P_MEAS_INST_TEMP_MOD_B               ( 0x9FF00189L )
#define  VAR_S3P_MEAS_FS_TEMP_MOD_B                 ( 0x9FF0018AL )
#define  VAR_S3P_MEAS_INST_TEMP_MOD_C               ( 0x9FF0018BL )
#define  VAR_S3P_MEAS_FS_TEMP_MOD_C                 ( 0x9FF0018CL )
#define  VAR_S3P_MEAS_INST_TEMP_AMB                 ( 0x9FF0018DL )
#define  VAR_S3P_MEAS_FS_TEMP_AMB                   ( 0x9FF0018EL )

#define  VAR_S3P_MEAS_INST_IAC_PH_A                 ( 0x9FF0018FL )
#define  VAR_S3P_MEAS_RMS_IAC_PH_A                  ( 0x9FF00190L )
#define  VAR_S3P_MEAS_INST_IAC_PH_B                 ( 0x9FF00191L )
#define  VAR_S3P_MEAS_RMS_IAC_PH_B                  ( 0x9FF00192L )
#define  VAR_S3P_MEAS_INST_IAC_PH_C                 ( 0x9FF00193L )
#define  VAR_S3P_MEAS_RMS_IAC_PH_C                  ( 0x9FF00194L )

#define  VAR_S3P_MEAS_INST_IACDCCOMP_PH_A           ( 0x9FF00195L )
#define  VAR_S3P_MEAS_FS_IACDCCOMP_PH_A             ( 0x9FF00196L )
#define  VAR_S3P_MEAS_INST_IACDCCOMP_PH_B           ( 0x9FF00197L )
#define  VAR_S3P_MEAS_FS_IACDCCOMP_PH_B             ( 0x9FF00198L )
#define  VAR_S3P_MEAS_INST_IACDCCOMP_PH_C           ( 0x9FF00199L )
#define  VAR_S3P_MEAS_FS_IACDCCOMP_PH_C             ( 0x9FF0019AL )

#define  VAR_S3P_MEAS_INST_VAC_PH_A                 ( 0x9FF0019BL )
#define  VAR_S3P_MEAS_RMS_VAC_PH_A                  ( 0x9FF0019CL )
#define  VAR_S3P_MEAS_INST_VAC_PH_B                 ( 0x9FF0019DL )
#define  VAR_S3P_MEAS_RMS_VAC_PH_B                  ( 0x9FF0019EL )
#define  VAR_S3P_MEAS_INST_VAC_PH_C                 ( 0x9FF0019FL )
#define  VAR_S3P_MEAS_RMS_VAC_PH_C                  ( 0x9FF001A0L )
#define  VAR_S3P_MEAS_INST_VAC_NEU                  ( 0x9FF001A1L )
#define  VAR_S3P_MEAS_RMS_VAC_NEU                   ( 0x9FF001A2L )

#define  VAR_S3P_MEAS_INST_VAC_RLY_PH_A             ( 0x9FF001A3L )
#define  VAR_S3P_MEAS_RMS_VAC_RLY_PH_A              ( 0x9FF001A4L )
#define  VAR_S3P_MEAS_INST_VAC_RLY_PH_B             ( 0x9FF001A5L )
#define  VAR_S3P_MEAS_RMS_VAC_RLY_PH_B              ( 0x9FF001A6L )
#define  VAR_S3P_MEAS_INST_VAC_RLY_PH_C             ( 0x9FF001A7L )
#define  VAR_S3P_MEAS_RMS_VAC_RLY_PH_C              ( 0x9FF001A8L )

#define  VAR_S3P_MEAS_INST_VDC_TOP_AC               ( 0x9FF001A9L )
#define  VAR_S3P_MEAS_FS_VDC_TOP_AC                 ( 0x9FF001AAL )
#define  VAR_S3P_MEAS_INST_VDC_BOT_AC               ( 0x9FF001ABL )
#define  VAR_S3P_MEAS_FS_VDC_BOT_AC                 ( 0x9FF001ACL )

#define  VAR_S3P_MEAS_INST_REAL_TOTAL               ( 0x9FF001ADL )
#define  VAR_S3P_MEAS_FS_REAL_TOTAL                 ( 0x9FF001AEL )
#define  VAR_S3P_MEAS_INST_REACTIVE_TOTAL           ( 0x9FF001AFL )
#define  VAR_S3P_MEAS_FS_REACTIVE_TOTAL             ( 0x9FF001B0L )

#define  VAR_S3P_MEAS_INST_I_RCD                    ( 0x9FF001B1L )
#define  VAR_S3P_MEAS_RMS_I_RCD                     ( 0x9FF001B2L )

#define  VAR_S3P_MEAS_INST_TEMP_MOD12               ( 0x9FF001B4L )
#define  VAR_S3P_MEAS_FS_TEMP_MOD12                 ( 0x9FF001B5L )
#define  VAR_S3P_MEAS_INST_TEMP_MOD34               ( 0x9FF001B6L )
#define  VAR_S3P_MEAS_FS_TEMP_MOD34                 ( 0x9FF001B7L )

#define  VAR_S3P_MEAS_INST_I_PV1                    ( 0x9FF001B8L )
#define  VAR_S3P_MEAS_FS_I_PV1                      ( 0x9FF001B9L )
#define  VAR_S3P_MEAS_INST_V_PV1                    ( 0x9FF001BAL )
#define  VAR_S3P_MEAS_FS_V_PV1                      ( 0x9FF001BBL )
#define  VAR_S3P_MEAS_INST_I_PV2                    ( 0x9FF001BCL )
#define  VAR_S3P_MEAS_FS_I_PV2                      ( 0x9FF001BDL )
#define  VAR_S3P_MEAS_INST_V_PV2                    ( 0x9FF001BEL )
#define  VAR_S3P_MEAS_FS_V_PV2                      ( 0x9FF001BFL )
#define  VAR_S3P_MEAS_INST_I_PV3                    ( 0x9FF001C0L )
#define  VAR_S3P_MEAS_FS_I_PV3                      ( 0x9FF001C1L )
#define  VAR_S3P_MEAS_INST_V_PV3                    ( 0x9FF001C2L )
#define  VAR_S3P_MEAS_FS_V_PV3                      ( 0x9FF001C3L )
#define  VAR_S3P_MEAS_INST_I_PV4                    ( 0x9FF001C4L )
#define  VAR_S3P_MEAS_FS_I_PV4                      ( 0x9FF001C5L )
#define  VAR_S3P_MEAS_INST_V_PV4                    ( 0x9FF001C6L )
#define  VAR_S3P_MEAS_FS_V_PV4                      ( 0x9FF001C7L )

#define  VAR_S3P_MEAS_INST_VDC_TOP_DC               ( 0x9FF001C8L )
#define  VAR_S3P_MEAS_FS_VDC_TOP_DC                 ( 0x9FF001C9L )
#define  VAR_S3P_MEAS_INST_VDC_BOT_DC               ( 0x9FF001CAL )
#define  VAR_S3P_MEAS_FS_VDC_BOT_DC                 ( 0x9FF001CBL )

#define  VAR_S3P_MEAS_INST_VDC_LINK                 ( 0x9FF001CCL )
#define  VAR_S3P_MEAS_FS_VDC_LINK                   ( 0x9FF001CDL )
#define  VAR_S3P_MEAS_INST_V_FAN                    ( 0x9FF001CEL )
#define  VAR_S3P_MEAS_FS_V_FAN                      ( 0x9FF001CFL )

#define  VAR_S3P_MEAS_INST_V_APS                    ( 0x9FF001D0L )
#define  VAR_S3P_MEAS_FS_V_APS                      ( 0x9FF001D1L )

#define  VAR_S3P_MEAS_INST_PWR_PV1                  ( 0x9FF001D2L )
#define  VAR_S3P_MEAS_FS_PWR_PV1                    ( 0x9FF001D3L )
#define  VAR_S3P_MEAS_INST_PWR_PV2                  ( 0x9FF001D4L )
#define  VAR_S3P_MEAS_FS_PWR_PV2                    ( 0x9FF001D5L )
#define  VAR_S3P_MEAS_INST_PWR_PV3                  ( 0x9FF001D6L )
#define  VAR_S3P_MEAS_FS_PWR_PV3                    ( 0x9FF001D7L )
#define  VAR_S3P_MEAS_INST_PWR_PV4                  ( 0x9FF001D8L )
#define  VAR_S3P_MEAS_FS_PWR_PV4                    ( 0x9FF001D9L )

#define  VAR_S3P_MEAS_PWR_PV_TOTAL                  ( 0x9FF001DAL )

#define  VAR_S3P_MEAS_INST_V_REF                    ( 0x9FF001DBL )
#define  VAR_S3P_MEAS_FS_V_REF                      ( 0x9FF001DCL )

#define  VAR_S3P_MEAS_INST_IABC                     ( 0x9FF001DDL )
#define  VAR_S3P_MEAS_RMS_IABC                      ( 0x9FF001DEL )

#define  VAR_S3P_FS_SCALE_VPV1                      ( 0x9FF001E0L )
#define  VAR_S3P_FS_OFFSET_VPV1                     ( 0x9FF001E1L )
#define  VAR_S3P_FS_SCALE_VPV2                      ( 0x9FF001E2L )
#define  VAR_S3P_FS_OFFSET_VPV2                     ( 0x9FF001E3L )
#define  VAR_S3P_FS_SCALE_VPV3                      ( 0x9FF001E4L )
#define  VAR_S3P_FS_OFFSET_VPV3                     ( 0x9FF001E5L )
#define  VAR_S3P_FS_SCALE_VPV4                      ( 0x9FF001E6L )
#define  VAR_S3P_FS_OFFSET_VPV4                     ( 0x9FF001E7L )

#define  VAR_S3P_FS_SCALE_IPV1                      ( 0x9FF001E8L )
#define  VAR_S3P_FS_OFFSET_IPV1                     ( 0x9FF001E9L )
#define  VAR_S3P_FS_SCALE_IPV2                      ( 0x9FF001EAL )
#define  VAR_S3P_FS_OFFSET_IPV2                     ( 0x9FF001EBL )
#define  VAR_S3P_FS_SCALE_IPV3                      ( 0x9FF001ECL )
#define  VAR_S3P_FS_OFFSET_IPV3                     ( 0x9FF001EDL )
#define  VAR_S3P_FS_SCALE_IPV4                      ( 0x9FF001EEL )
#define  VAR_S3P_FS_OFFSET_IPV4                     ( 0x9FF001EFL )

#define  VAR_S3P_FS_SCALE_VDCTOP_DC                 ( 0x9FF001F0L )
#define  VAR_S3P_FS_OFFSET_VDCTOP_DC                ( 0x9FF001F1L )
#define  VAR_S3P_FS_SCALE_VDCBOT_DC                 ( 0x9FF001F2L )
#define  VAR_S3P_FS_OFFSET_VDCBOT_DC                ( 0x9FF001F3L )
#define  VAR_S3P_FS_SCALE_VDCLINK                   ( 0x9FF001F4L )
#define  VAR_S3P_FS_OFFSET_VDCLINK                  ( 0x9FF001F5L )

#define  VAR_S3P_FS_SCALE_TEMP_DC12                 ( 0x9FF001F6L )
#define  VAR_S3P_FS_OFFSET_TEMP_DC12                ( 0x9FF001F7L )
#define  VAR_S3P_FS_SCALE_TEMP_DC34                 ( 0x9FF001F8L )
#define  VAR_S3P_FS_OFFSET_TEMP_DC34                ( 0x9FF001F9L )

#define  VAR_S3P_FS_SCALE_VFAN                      ( 0x9FF001FAL )
#define  VAR_S3P_FS_OFFSET_VFAN                     ( 0x9FF001FBL )

#define  VAR_S3P_FS_SCALE_V_APS                     ( 0x9FF001FCL )
#define  VAR_S3P_FS_OFFSET_V_APS                    ( 0x9FF001FDL )

#define  VAR_S3P_FS_SCALE_I_RCD                     ( 0x9FF001FEL )
#define  VAR_S3P_FS_OFFSET_I_RCD                    ( 0x9FF001FFL )

#define  VAR_S3P_FS_SCALE_IAC_PH_A                  ( 0x9FF00200L )
#define  VAR_S3P_FS_OFFSET_IAC_PH_A                 ( 0x9FF00201L )
#define  VAR_S3P_FS_SCALE_IAC_PH_B                  ( 0x9FF00202L )
#define  VAR_S3P_FS_OFFSET_IAC_PH_B                 ( 0x9FF00203L )
#define  VAR_S3P_FS_SCALE_IAC_PH_C                  ( 0x9FF00204L )
#define  VAR_S3P_FS_OFFSET_IAC_PH_C                 ( 0x9FF00205L )

#define  VAR_S3P_FS_SCALE_VAC_PH_A                  ( 0x9FF00206L )
#define  VAR_S3P_FS_OFFSET_VAC_PH_A                 ( 0x9FF00207L )
#define  VAR_S3P_FS_SCALE_VAC_PH_B                  ( 0x9FF00208L )
#define  VAR_S3P_FS_OFFSET_VAC_PH_B                 ( 0x9FF00209L )
#define  VAR_S3P_FS_SCALE_VAC_PH_C                  ( 0x9FF0020AL )
#define  VAR_S3P_FS_OFFSET_VAC_PH_C                 ( 0x9FF0020BL )

#define  VAR_S3P_FS_SCALE_VAC_NEU                   ( 0x9FF0020CL )
#define  VAR_S3P_FS_OFFSET_VAC_NEU                  ( 0x9FF0020DL )

#define  VAR_S3P_FS_SCALE_VAC_RLY_PH_A              ( 0x9FF0020EL )
#define  VAR_S3P_FS_OFFSET_VAC_RLY_PH_A             ( 0x9FF0020FL )
#define  VAR_S3P_FS_SCALE_VAC_RLY_PH_B              ( 0x9FF00210L )
#define  VAR_S3P_FS_OFFSET_VAC_RLY_PH_B             ( 0x9FF00211L )
#define  VAR_S3P_FS_SCALE_VAC_RLY_PH_C              ( 0x9FF00212L )
#define  VAR_S3P_FS_OFFSET_VAC_RLY_PH_C             ( 0x9FF00213L )

#define  VAR_S3P_FS_SCALE_VDCTOP_AC                 ( 0x9FF00214L )
#define  VAR_S3P_FS_OFFSET_VDCTOP_AC                ( 0x9FF00215L )
#define  VAR_S3P_FS_SCALE_VDCBOT_AC                 ( 0x9FF00216L )
#define  VAR_S3P_FS_OFFSET_VDCBOT_AC                ( 0x9FF00217L )

#define  VAR_S3P_FS_SCALE_V_REF                     ( 0x9FF00218L )
#define  VAR_S3P_FS_OFFSET_V_REF                    ( 0x9FF00219L )

#define  VAR_S3P_FS_SCALE_TEMP_MOD_A                ( 0x9FF0021AL )
#define  VAR_S3P_FS_OFFSET_TEMP_MOD_A               ( 0x9FF0021BL )
#define  VAR_S3P_FS_SCALE_TEMP_MOD_B                ( 0x9FF0021CL )
#define  VAR_S3P_FS_OFFSET_TEMP_MOD_B               ( 0x9FF0021DL )
#define  VAR_S3P_FS_SCALE_TEMP_MOD_C                ( 0x9FF0021EL )
#define  VAR_S3P_FS_OFFSET_TEMP_MOD_C               ( 0x9FF0021FL )
#define  VAR_S3P_FS_SCALE_TEMP_AMB                  ( 0x9FF00220L )
#define  VAR_S3P_FS_OFFSET_TEMP_AMB                 ( 0x9FF00221L )

#define  VAR_S3P_FS_SCALE_IAC_DCCOMP_PH_A           ( 0x9FF00222L )
#define  VAR_S3P_FS_OFFSET_IAC_DCCOMP_PH_A          ( 0x9FF00223L )
#define  VAR_S3P_FS_SCALE_IAC_DCCOMP_PH_B           ( 0x9FF00224L )
#define  VAR_S3P_FS_OFFSET_IAC_DCCOMP_PH_B          ( 0x9FF00225L )
#define  VAR_S3P_FS_SCALE_IAC_DCCOMP_PH_C           ( 0x9FF00226L )
#define  VAR_S3P_FS_OFFSET_IAC_DCCOMP_PH_C          ( 0x9FF00227L )

#define  VAR_S3P_FS_SCALE_REAL_PWR                  ( 0x9FF00228L )
#define  VAR_S3P_FS_OFFSET_REAL_PWR                 ( 0x9FF00229L )
#define  VAR_S3P_FS_SCALE_REACTIVE_PWR              ( 0x9FF0022AL )
#define  VAR_S3P_FS_OFFSET_REACTIVE_PWR             ( 0x9FF0022BL )
#define  VAR_S3P_FS_SCALE_IABC                      ( 0x9FF0022EL )
#define  VAR_S3P_FS_OFFSET_IABC                     ( 0x9FF0022FL )
#define  VAR_DIGCTRL_AC_GRID_FREQ_DFLT              ( 0x9FF00230L )
#define  VAR_DIGCTRL_AC_ROTATION_DFLT               ( 0x9FF00231L )

#define  VAR_TEMP_SNS_BETA                          ( 0x9FF00234L )
#define  VAR_TEMP_SNS_RBASE                         ( 0x9FF00235L )
#define  VAR_TEMP_SNS_RREF                          ( 0x9FF00236L )
#define  VAR_TEMP_SNS_VFULL                         ( 0x9FF00237L )

#define  VAR_I2C_DBG_LVL                            ( 0x9FF00250L )
#define  VAR_I2C_TIMEOUT                            ( 0x9FF00251L )
#define  VAR_I2C_RX_TIMEOUT                         ( 0x9FF00252L )
#define  VAR_I2C_ADDRESS                            ( 0x9FF00253L )
#define  VAR_I2C_FREQ_KHZ                           ( 0x9FF00254L )
#define  VAR_I2C_RETRIES                            ( 0x9FF00255L )
#define  VAR_I2C_STATS                              ( 0x9FF00256L )


//power handling sysvar IDs
#define  VAR_PDC_LIM_SET                           ( 0x9FF00300L )
//#define  VAR_PDC_LIM_SET2                           ( 0x9FF00301L )
//#define  VAR_PDC_LIM_SET3                           ( 0x9FF00302L )
//#define  VAR_PDC_LIM_SET4                           ( 0x9FF00303L )
#define  VAR_PDC_LIM_ACTIVE1                        ( 0x9FF00304L )
#define  VAR_PDC_LIM_ACTIVE2                        ( 0x9FF00305L )
#define  VAR_PDC_LIM_ACTIVE3                        ( 0x9FF00306L )
#define  VAR_PDC_LIM_ACTIVE4                        ( 0x9FF00307L )
#define  VAR_PDC_DERATE_ST                          ( 0x9FF00308L )
#define  VAR_AMB_DERATE_F                           ( 0x9FF00309L )
#define  VAR_AMB_DERATE_0                           ( 0x9FF0030AL )
#define  VAR_HS_DERATE_F                            ( 0x9FF0030BL )
#define  VAR_HS_DERATE_0                            ( 0x9FF0030CL )
#define  VAR_PVVH_DERATE_F                          ( 0x9FF0030DL )
#define  VAR_PVVH_DERATE_0                          ( 0x9FF0030EL )
#define  VAR_PVVL_DERATE_F                          ( 0x9FF0030FL )
#define  VAR_PVVL_DERATE_20                         ( 0x9FF00310L )
#define  VAR_ACV_DERATE_F                           ( 0x9FF00311L )
#define  VAR_ACV_DERATE_0                           ( 0x9FF00312L )

#define  VAR_DIGCTRL_AC_TEST_DUTY                   ( 0x9FF00400L )
#define  VAR_DIGCTRL_AC_TEST_AMP                    ( 0x9FF00401L )
#define  VAR_DIGCTRL_DC_TEST_DUTY1                  ( 0x9FF00402L )
#define  VAR_DIGCTRL_DC_TEST_DUTY2                  ( 0x9FF00403L )
#define  VAR_DIGCTRL_DC_TEST_DUTY3                  ( 0x9FF00404L )
#define  VAR_DIGCTRL_DC_TEST_DUTY4                  ( 0x9FF00405L )
/***************************************************************/


#define  VAR_GT_MEMORY_MON_TASK                    ( 0x9FF00500L )
#define  VAR_GT_MEMORY_MON_TASK_PRD                ( 0x9FF00501L )
#define  VAR_GT_MEMORY_MON_STATUS                  ( 0x9FF00502L )

#define  VAR_GT_TEST_TEMP_VALUE                    ( 0x9FFF0000L )
#define  VAR_GT_TEST_TEMP_LIMIT                    ( 0x9FFF0001L )
#define  VAR_GT_TEST_POWER_VALUE                   ( 0x9FFF0002L )
#define  VAR_GT_TEST_PV_VALUE                      ( 0x9FFF0003L )
#define  VAR_GT_TEST_CTRL_PV_SET                   ( 0x9FFF0004L )

#define VAR_GT_LOCAL_TIME_S_LABEL                  ( 0x9FFF0005L )
#define VAR_GT_UTC_TIME_S_LABEL                    ( 0x9FFF0006L )
#define VAR_GT_LOCAL_TIME_STR_LABEL                ( 0x9FFF0007L )
#define VAR_GT_UTC_TIME_STR_LABEL                  ( 0x9FFF0008L )
#define VAR_GT_TIME_ZONE_S_LABEL                   ( 0x9FFF0009L )
#define VAR_GT_UTC_TIME_ISO_STR_LABEL              ( 0x9FFF000AL )
#define VAR_GT_LOCAL_TIME_ISO_STR_LABEL            ( 0x9FFF000BL )
#define VAR_GT_EEFS_VOLUME_LABEL                   ( 0x9FFF000CL )

#define VAR_AGY_OPTION_START                       ( 0x81000000L )
/*------------------------------------------------------------------------------------------------------------------
  (Refer to faults.h)
  Bit define to warnings and faults:
   0 : PV.OVMPPT                    -- PV higher than MPPT window   (warning)
   1 : PV.UNMPPT                    -- PV lower than MPPT window    (warning)
   2 : PV.VNGLOW                    -- Night time, PV voltage low   (def: no oscillography)
   3 : PV.VHIGH                     -- PV voltage high
   4 : APS.FAULT                    -- APS fault                    (def: no oscillography)
   5 : GRID.DELAY                   -- Grid reconnection delay      (def: no oscillography)
   6 : COMM.FAIL or RCD.FAIL        -- inter-communication failed
   7 : TEMP.OVER                    -- Over temperature fault
   8 : TEMP.SENSOR                  -- temperature sensor fault     (def: manual )
   9 : DC.INJECT                    -- DC inject fault
  10 : GRID.VHIGH                   -- Grid voltage high
  11 : GRID.VVHIGH                  -- Grid voltage very high
  12 : GRID.VLOW                    -- Grid voltage low
  13 : GRID.VVLOW                   -- Grid voltage very low
  14 : GRID.FHIGH                   -- Grid frequency high
  15 : GRID.FLOW                    -- Grid frequency low
  16 : GRID.FVLOW                   -- Grid frequency very low
  17 : OVP.FAULT                    -- OVP
  18 : OCP.FAULT                    -- OCP
  19 : ONLINE.GRND                  -- Online ground fault, issued from any of RC faults (alarms): ONLINE.ICONS or ONLINE.IINS1 or ONLINE.IINS2 or ONLINE.IINS3
  20 : MEM.FAIL                     -- Memory fault                                 (def: manual )
  21 : DCDC.VOVER or DCDC.VUNDER    -- DCDC fault                                   (def: manual )
  22 : ACI.SENSOR                   -- AC current sensor fault                      (def: manual )
  23 : RCD.SENSOR                   -- RCD sensor fault   (def: manual )
  24 : OPUT.RELAY                   -- Output relay fault                           (def: manual )
  25 : OFFLINE.GRND                 -- Offline ground fault (PV isolation fault )   (def: manual )
  26 : FAN.FAULT                    -- Fan fault

----------------------------------------------------------------------------------------------------------------*/
#define VAR_AGY_ENABLE_MASK_1                      ( 0x81000000L )
#define VAR_AGY_STOP_MASK_1                        ( 0x81000001L )
#define VAR_AGY_OSC_MASK_1                         ( 0x81000002L )
#define VAR_AGY_MANU_MASK_1                        ( 0x81000003L )
#define VAR_AGY_FAULT_REG_1                        ( 0x81000004L )
#define VAR_AGY_FLT_TO_EVENT_1                     ( 0x81000005L )
#define VAR_AGY_ENABLE_MASK_2                      ( 0x81000006L )
#define VAR_AGY_STOP_MASK_2                        ( 0x81000007L )
#define VAR_AGY_OSC_MASK_2                         ( 0x81000008L )
#define VAR_AGY_MANU_MASK_2                        ( 0x81000009L )
#define VAR_AGY_FAULT_REG_2                        ( 0x8100000AL )
#define VAR_AGY_FLT_TO_EVENT_2                     ( 0x8100000BL )
#define VAR_GT_INV_MANUAL_CHK_1                    ( 0x8100000CL )
#define VAR_GT_INV_MANUAL_CHK_2                    ( 0x8100000DL )

#define VAR_AGY_VAR_START_ID                       ( 0x81000020L )
#define VAR_AGY_OCP_TRIP_HI_THRESHOLD              ( 0x81000021L )
#define VAR_AGY_OCP_TRIP_LO_THRESHOLD              ( 0x81000022L )

#define VAR_AGY_CONF_START_ID                      ( 0x81000100L )
#define VAR_AGY_VRMS_NOMINAL                       ( 0x81000101L )
#define VAR_AGY_VRMS_NOMINAL_MIN                   ( 0x81000102L )
#define VAR_AGY_VRMS_NOMINAL_MAX                   ( 0x81000103L )
#define VAR_AGY_VRMS_HIGH                          ( 0x81000104L )
#define VAR_AGY_VRMS_LOW                           ( 0x81000105L )
#define VAR_AGY_VRMS_HIGH_RECON                    ( 0x81000106L )
#define VAR_AGY_FREQ_NOMINAL                       ( 0x81000108L )
#define VAR_AGY_FREQ_HIGH                          ( 0x81000109L )
#define VAR_AGY_FREQ_LOW                           ( 0x8100010AL )
#define VAR_AGY_FREQ_LOW_FAST                      ( 0x8100010BL )
#define VAR_AGY_INJECT_THRESHOLD                   ( 0x8100010CL )
#define VAR_AGY_FREQ_HYST                          ( 0x8100010DL )
#define VAR_AGY_VRMS_HYST                          ( 0x8100010EL )
#define VAR_AGY_OVP_INST_HYST                      ( 0x8100010FL )
#define VAR_AGY_IRMS_HIGH                          ( 0x81000110L )
#define VAR_AGY_IRMS_HYST                          ( 0x81000111L )
#define VAR_AGY_IINST_HIGH                         ( 0x81000112L )
#define VAR_AGY_IINST_HYST                         ( 0x81000113L )

#define VAR_FLT_GTFHIGH_TRIP                       ( 0x81000120L )
#define VAR_FLT_GTFHIGH_RECV                       ( 0x81000121L )
#define VAR_FLT_GTFHIGH_HBT                        ( 0x81000122L )
#define VAR_FLT_GTFLOW_TRIP                        ( 0x81000123L )
#define VAR_FLT_GTFLOW_RECV                        ( 0x81000124L )
#define VAR_FLT_GTFLOW_HBT                         ( 0x81000125L )
#define VAR_FLT_GTFVLOW_TRIP                       ( 0x81000126L )
#define VAR_FLT_GTFVLOW_RECV                       ( 0x81000127L )
#define VAR_FLT_GTFVLOW_HBT                        ( 0x81000128L )
#define VAR_FLT_GTVVHIGH_TRIP                      ( 0x81000129L )
#define VAR_FLT_GTVVHIGH_RECV                      ( 0x8100012AL )
#define VAR_FLT_GTVVHIGH_HBT                       ( 0x8100012BL )
#define VAR_FLT_GTVHIGH_TRIP                       ( 0x8100012CL )
#define VAR_FLT_GTVHIGH_RECV                       ( 0x8100012DL )
#define VAR_FLT_GTVHIGH_HBT                        ( 0x8100012EL )
#define VAR_FLT_GTVLOW_TRIP                        ( 0x8100012FL )
#define VAR_FLT_GTVLOW_RECV                        ( 0x81000130L )
#define VAR_FLT_GTVLOW_HBT                         ( 0x81000131L )
#define VAR_FLT_GTVVLOW_TRIP                       ( 0x81000132L )
#define VAR_FLT_GTVVLOW_RECV                       ( 0x81000133L )
#define VAR_FLT_GTVVLOW_HBT                        ( 0x81000134L )
#define VAR_FLT_DCINJECT_TRIP                      ( 0x81000135L )
#define VAR_FLT_DCINJECT_RECV                      ( 0x81000136L )
#define VAR_FLT_DCINJECT_HBT                       ( 0x81000137L )
#define VAR_FLT_GRID_RECON_TRIP                    ( 0x81000138L )
#define VAR_FLT_GRID_RECON_RECV                    ( 0x81000139L )
#define VAR_FLT_GRID_RECON_HBT                     ( 0x8100013AL )
#define VAR_I2C2_TX_BUF_SIZE                       ( 0x8100013BL )
#define VAR_I2C2_ADDRESS                           ( 0x8100013CL )

#define VAR_AGY_TEMP_HIGH                          ( 0x81000150L )
#define VAR_AGY_TEMP_LOW                           ( 0x81000151L )
#define VAR_AGY_I_OFFLN_GROUND                     ( 0x81000152L )
#define VAR_AGY_MPPT_BOUND_OFFSET                  ( 0x81000153L )
#define VAR_AGY_MPPT_UPPER_BOUND                   ( 0x81000154L )
#define VAR_AGY_MPPT_LOWER_BOUND                   ( 0x81000155L )
#define VAR_AGY_VOC_NOMINAL                        ( 0x81000156L )
#define VAR_AGY_VOC_UPPER_OFFSET                   ( 0x81000157L )
#define VAR_AGY_VOC_LOWER_OFFSET                   ( 0x81000158L )
#define VAR_AGY_PV_V_NIGHT_OFFSET                  ( 0x81000159L )
#define VAR_AGY_ACI_SENSOR_DISCREPANCY             ( 0x8100015AL )
#define VAR_AGY_RELAY_THRESHOLD                    ( 0x8100015BL )
#define VAR_AGY_TEMP_SENSOR_DIFF                   ( 0x8100015CL )
#define VAR_AGY_DCDC_V_LOW                         ( 0x8100015DL )
#define VAR_AGY_DCDC_V_HIGH                        ( 0x8100015EL )
#define VAR_AGY_OVP_INST_HIGH                      ( 0x8100015FL )
#define VAR_AGY_OVP_INST_OFFSET                    ( 0x81000160L )
#define VAR_AGY_ACI_SENSOR_NOMINAL                 ( 0x81000161L )
#define VAR_AGY_VRMS_DIFF                          ( 0x81000162L )

#define VAR_AGY_DC_TOP_V_HIGH                      ( 0x8100017CL )
#define VAR_AGY_DC_BTM_V_HIGH                      ( 0x8100017DL )


#define VAR_FLT_OVTEMP_TRIP                        ( 0x81000180L )
#define VAR_FLT_OVTEMP_RECV                        ( 0x81000181L )
#define VAR_FLT_OVTEMP_HBT                         ( 0x81000182L )

#define VAR_FLT_VNGLOW_TRIP                        ( 0x81000183L )
#define VAR_FLT_VNGLOW_RECV                        ( 0x81000184L )
#define VAR_FLT_VNGLOW_HBT                         ( 0x81000185L )

#define VAR_FLT_VHIGH_TRIP                         ( 0x81000186L )
#define VAR_FLT_VHIGH_RECV                         ( 0x81000187L )
#define VAR_FLT_VHIGH_HBT                          ( 0x81000188L )

#define VAR_FLT_OVMPPT_TRIP                        ( 0x8100018CL )
#define VAR_FLT_OVMPPT_RECV                        ( 0x8100018DL )
#define VAR_FLT_OVMPPT_HBT                         ( 0x8100018EL )

#define VAR_FLT_UDMPPT_TRIP                        ( 0x8100018FL )
#define VAR_FLT_UDMPPT_RECV                        ( 0x81000190L )
#define VAR_FLT_UDMPPT_HBT                         ( 0x81000191L )

#define VAR_FLT_OVP_TRIP                           ( 0x81000200L )
#define VAR_FLT_OVP_RECV                           ( 0x81000201L )
#define VAR_FLT_OVP_HBT                            ( 0x81000202L )

#define VAR_FLT_GTDELAY_TRIP                       ( 0x81000206L )
#define VAR_FLT_GTDELAY_RECV                       ( 0x81000207L )
#define VAR_FLT_GTDELAY_HBT                        ( 0x81000208L )

#define VAR_FLT_INTCOMM_TRIP                       ( 0x81000209L )
#define VAR_FLT_INTCOMM_RECV                       ( 0x8100020AL )
#define VAR_FLT_INTCOMM_HBT                        ( 0x8100020BL )

#define VAR_FLT_TMPSENSOR_TRIP                     ( 0x8100020CL )
#define VAR_FLT_TMPSENSOR_RECV                     ( 0x8100020DL )
#define VAR_FLT_TMPSENSOR_HBT                      ( 0x8100020EL )

#define VAR_FLT_OCP_TRIP                           ( 0x8100020FL )
#define VAR_FLT_OCP_RECV                           ( 0x81000210L )
#define VAR_FLT_OCP_HBT                            ( 0x81000211L )

#define VAR_FLT_ONLNGRND_TRIP                      ( 0x81000212L )
#define VAR_FLT_ONLNGRND_RECV                      ( 0x81000213L )
#define VAR_FLT_ONLNGRND_HBT                       ( 0x81000214L )

#define VAR_FLT_MEM_TRIP                           ( 0x81000215L )
#define VAR_FLT_MEM_RECV                           ( 0x81000216L )
#define VAR_FLT_MEM_HBT                            ( 0x81000217L )

#define VAR_FLT_DCDC_TRIP                          ( 0x81000218L )
#define VAR_FLT_DCDC_RECV                          ( 0x81000219L )
#define VAR_FLT_DCDC_HBT                           ( 0x8100021AL )

#define VAR_FLT_ACISENSOR_TRIP                     ( 0x8100021BL )
#define VAR_FLT_ACISENSOR_RECV                     ( 0x8100021CL )
#define VAR_FLT_ACISENSOR_HBT                      ( 0x8100021DL )

#define VAR_FLT_RCDSENSOR_TRIP                     ( 0x8100021EL )
#define VAR_FLT_RCDSENSOR_RECV                     ( 0x8100021FL )
#define VAR_FLT_RCDSENSOR_HBT                      ( 0x81000220L )

#define VAR_FLT_GTRELAY_TRIP                       ( 0x81000221L )
#define VAR_FLT_GTRELAY_RECV                       ( 0x81000222L )
#define VAR_FLT_GTRELAY_HBT                        ( 0x81000223L )

#define VAR_FLT_OFFLNGRND_TRIP                     ( 0x81000224L )
#define VAR_FLT_OFFLNGRND_RECV                     ( 0x81000225L )
#define VAR_FLT_OFFLNGRND_HBT                      ( 0x81000226L )

#define VAR_FLT_FAN_TRIP                           ( 0x81000227L )
#define VAR_FLT_FAN_RECV                           ( 0x81000228L )
#define VAR_FLT_FAN_HBT                            ( 0x81000229L )

#define VAR_FLT_GRID_RECONN_COUNTDOWN              ( 0x8100022AL )

#define VAR_FLT_OCRMS_TRIP                         ( 0x8100022BL )
#define VAR_FLT_OCRMS_RECV                         ( 0x8100022CL )
#define VAR_FLT_OCRMS_HBT                          ( 0x8100022DL )

#define VAR_FLT_GNDLEAKAGE_TRIP                    ( 0x8100022EL )
#define VAR_FLT_GNDLEAKAGE_RECV                    ( 0x8100022FL )
#define VAR_FLT_GNDLEAKAGE_HBT                     ( 0x81000230L )

#define VAR_FLT_DCINJECT2_TRIP                     ( 0x81000231L )
#define VAR_FLT_DCINJECT2_RECV                     ( 0x81000232L )

#define VAR_FLT_ACDSP_DCLINKV_HBT                  ( 0x81000233L )
#define VAR_FLT_ACDSP_DCLINK_OV_BTM_TRIP           ( 0x81000234L )
#define VAR_FLT_ACDSP_DCLINK_OV_BTM_RECV           ( 0x81000235L )
#define VAR_FLT_ACDSP_DCLINK_OV_TOP_TRIP           ( 0x81000236L )
#define VAR_FLT_ACDSP_DCLINK_OV_TOP_RECV           ( 0x81000237L )
#define VAR_FLT_ACDSP_DCLINK_IMBALANCE_TRIP        ( 0x81000238L )
#define VAR_FLT_ACDSP_DCLINK_IMBALANCE_RECV        ( 0x81000239L )
#define VAR_FLT_ACDSP_DCLINK_UV_TRIP               ( 0x8100023AL )
#define VAR_FLT_ACDSP_DCLINK_UV_RECV               ( 0x8100023BL )

#define VAR_FLT_ACDSP_DCLINKVSNS_TRIP              ( 0x8100023CL )
#define VAR_FLT_ACDSP_DCLINKVSNS_RECV              ( 0x8100023DL )
#define VAR_FLT_ACDSP_DCLINKVSNS_HBT               ( 0x8100023EL )

#define VAR_FLT_VREF_TRIP                          ( 0x8100023FL )
#define VAR_FLT_VREF_RECV                          ( 0x81000240L )
#define VAR_FLT_VREF_HBT                           ( 0x81000241L )

#define VAR_FLT_EFFICIENCY_LOSS_TRIP               ( 0x81000242L )
#define VAR_FLT_EFFICIENCY_LOSS_RECV               ( 0x81000243L )
#define VAR_FLT_EFFICIENCY_LOSS_HBT                ( 0x81000244L )

#define VAR_FLT_PHASE_ERR_TRIP                     ( 0x81000245L )
#define VAR_FLT_PHASE_ERR_RECV                     ( 0x81000246L )
#define VAR_FLT_PHASE_ERR_HBT                      ( 0x81000247L )

#define VAR_FLT_PLL_LOST_LOCK_TRIP                 ( 0x81000248L )
#define VAR_FLT_PLL_LOST_LOCK_RECV                 ( 0x81000249L )
#define VAR_FLT_PLL_LOST_LOCK_HBT                  ( 0x8100024AL )

#define VAR_FLT_PWM_FAIL_TRIP                      ( 0x8100024BL )
#define VAR_FLT_PWM_FAIL_RECV                      ( 0x8100024CL )
#define VAR_FLT_PWM_FAIL_HBT                       ( 0x8100024DL )

#define VAR_FLT_COCPU_FAIL_TRIP                    ( 0x8100024EL )
#define VAR_FLT_COCPU_FAIL_RECV                    ( 0x8100024FL )
#define VAR_FLT_COCPU_FAIL_HBT                     ( 0x81000250L )

#define VAR_FLT_OCINST_TRIP                        ( 0x81000251L )
#define VAR_FLT_OCINST_RECV                        ( 0x81000252L )
#define VAR_FLT_OCINST_HBT                         ( 0x81000253L )

/* RCD Monitoring      */
#define VAR_RCD_COMM_COUNTER                       ( 0x81000300L )
#define VAR_RCD_COMMAND                            ( 0x81000301L )
#define VAR_RCD_VERSION_MAJ                        ( 0x81000302L )
#define VAR_RCD_VERSION_MIN                        ( 0x81000303L )
#define VAR_RCD_VERSION_BN                         ( 0x81000304L )
#define VAR_RCD_STATUS                             ( 0x81000305L )
#define VAR_RCD_FREQUENCY                          ( 0x81000306L )
#define VAR_RCD_CURRENT                            ( 0x81000307L )
#define VAR_RCD_CRC                                ( 0x81000308L )
#define VAR_RCD_VOLTAGE                            ( 0x81000309L )
#define VAR_RCD_ADC0                               ( 0x8100030AL )
#define VAR_RCD_ADC1                               ( 0x8100030BL )
#define VAR_RCD_ADCDIF                             ( 0x8100030CL )

#define VAR_FLT_EOL_TRIP                           ( 0x81000400L )
#define VAR_FLT_EOL_RECV                           ( 0x81000401L )
#define VAR_FLT_EOL_HBT                            ( 0x81000402L )

#define VAR_FLT_3PHS_TRIP                          ( 0x81000403L )
#define VAR_FLT_3PHS_RECV                          ( 0x81000404L )
#define VAR_FLT_3PHS_HBT                           ( 0x81000405L )

#define VAR_FLT_ACV_TRIP                           ( 0x81000406L )
#define VAR_FLT_ACV_RECV                           ( 0x81000407L )
#define VAR_FLT_ACV_HBT                            ( 0x81000408L )

#define VAR_FLT_ACI_TRIP                           ( 0x81000409L )
#define VAR_FLT_ACI_RECV                           ( 0x8100040AL )
#define VAR_FLT_ACI_HBT                            ( 0x8100040BL )

//DCDC
#define VAR_AGY_DC_ENABLE_MASK_1                   ( 0x81001000L )
#define VAR_AGY_DC_ENABLE_MASK_2                   ( 0x81001001L )
#define VAR_AGY_DC_FAULT_REG_1                     ( 0x81001002L )
#define VAR_AGY_DC_FAULT_REG_2                     ( 0x81001003L )
#define VAR_AGY_DC_FLT_TO_WRN_1                    ( 0x81001004L )
#define VAR_AGY_DC_FLT_TO_WRN_2                    ( 0x81001005L )
#define VAR_AGY_DC_FLT_TO_EVENT_1                  ( 0x81001006L )
#define VAR_AGY_DC_FLT_TO_EVENT_2                  ( 0x81001007L )
#define VAR_AGY_DC_MANU_MASK_1                     ( 0x81001008L )
#define VAR_AGY_DC_MANU_MASK_2                     ( 0x81001009L )
#define VAR_AGY_DC_OSC_MASK_1                      ( 0x8100000AL )
#define VAR_AGY_DC_OSC_MASK_2                      ( 0x8100000BL )
#define VAR_AGY_DC_STOP_MASK_1                     ( 0x8100000CL )
#define VAR_AGY_DC_STOP_MASK_2                     ( 0x8100000DL )

//DCDCfault
#define VAR_FLT_PV_OV_TRIP                         ( 0x81001010L )
#define VAR_FLT_PV_OV_RECV                         ( 0x81001011L )
#define VAR_FLT_PV_OV_HBT                          ( 0x81001012L )

#define VAR_FLT_DC_OV_TRIP                         ( 0x81001013L )
#define VAR_FLT_DC_OV_RECV                         ( 0x81001014L )
#define VAR_FLT_DC_OV_HBT                          ( 0x81001015L )

#define VAR_FLT_DCBOT_OV_TRIP                      ( 0x81001016L )
#define VAR_FLT_DCBOT_OV_RECV                      ( 0x81001017L )
#define VAR_FLT_DCBOT_OV_HBT                       ( 0x81001018L )

#define VAR_FLT_DCTOP_OV_TRIP                      ( 0x81001019L )
#define VAR_FLT_DCTOP_OV_RECV                      ( 0x8100101AL )
#define VAR_FLT_DCTOP_OV_HBT                       ( 0x8100101BL )

#define VAR_FLT_DC_IMBALANCE_TRIP                  ( 0x8100101CL )
#define VAR_FLT_DC_IMBALANCE_RECV                  ( 0x8100101DL )
#define VAR_FLT_DC_IMBALANCE_HBT                   ( 0x8100101EL )

#define VAR_FLT_DC_UV_TRIP                         ( 0x8100101FL )
#define VAR_FLT_DC_UV_RECV                         ( 0x81001020L )
#define VAR_FLT_DC_UV_HBT                          ( 0x81001021L )

#define VAR_FLT_VDC_SENSOR_TRIP                    ( 0x81001022L )
#define VAR_FLT_VDC_SENSOR_RECV                    ( 0x81001023L )
#define VAR_FLT_VDC_SENSOR_HBT                     ( 0x81001024L )

#define VAR_FLT_PV_OC_RMS_TRIP                     ( 0x81001025L )
#define VAR_FLT_PV_OC_RMS_RECV                     ( 0x81001026L )
#define VAR_FLT_PV_OC_RMS_HBT                      ( 0x81001027L )

#define VAR_FLT_PV_OC_INST_TRIP                    ( 0x81001028L )
#define VAR_FLT_PV_OC_INST_RECV                    ( 0x81001029L )
#define VAR_FLT_PV_OC_INST_HBT                     ( 0x8100102AL )

#define VAR_FLT_DC_OT_TRIP                         ( 0x8100102BL )
#define VAR_FLT_DC_OT_RECV                         ( 0x8100102CL )
#define VAR_FLT_DC_OT_HBT                          ( 0x8100102DL )

#define VAR_FLT_APS_TRIP                           ( 0x8100102EL )
#define VAR_FLT_APS_RECV                           ( 0x8100102FL )
#define VAR_FLT_APS_HBT                            ( 0x81001030L )

#define VAR_FLT_RCMU_TRIP                          ( 0x81001031L )
#define VAR_FLT_RCMU_RECV                          ( 0x81001032L )
#define VAR_FLT_RCMU_HBT                           ( 0x81001033L )

#define VAR_EVT_PV_LOW_TRIP                        ( 0x81001040L )
#define VAR_EVT_PV_LOW_RECV                        ( 0x81001041L )
#define VAR_EVT_PV_LOW_HBT                         ( 0x81001042L )

#define VAR_WRN_DC_OT_TRIP                         ( 0x81001050L )
#define VAR_WRN_DC_OT_RECV                         ( 0x81001051L )
#define VAR_WRN_DC_OT_HBT                          ( 0x81001052L )

#define VAR_WRN_DCTEMP_SENSOR_TRIP                 ( 0x81001053L )
#define VAR_WRN_DCTEMP_SENSOR_RECV                 ( 0x81001054L )
#define VAR_WRN_DCTEMP_SENSOR_HBT                  ( 0x81001055L )

#define VAR_WRN_FAN_ALARM_TRIP                     ( 0x81001056L )
#define VAR_WRN_FAN_ALARM_RECV                     ( 0x81001057L )
#define VAR_WRN_FAN_ALARM_HBT                      ( 0x81001058L )

#define VAR_WRN_COOLING_LOSS_TRIP                  ( 0x81001059L )
#define VAR_WRN_COOLING_LOSS_RECV                  ( 0x8100105AL )
#define VAR_WRN_COOLING_LOSS_HBT                   ( 0x8100105BL )

#define VAR_WRN_PV_MISSING_TRIP                    ( 0x8100105CL )
#define VAR_WRN_PV_MISSING_RECV                    ( 0x8100105DL )
#define VAR_WRN_PV_MISSING_HBT                     ( 0x8100105EL )

#define VAR_AGY_PV_OV                              ( 0x81001070L )
#define VAR_AGY_VPV_LOW                            ( 0x81001071L )
#define VAR_AGY_PPV_LOW                            ( 0x81001072L )
#define VAR_AGY_DC_OV                              ( 0x81001073L )
#define VAR_AGY_DCTOP_OV                           ( 0x81001074L )
#define VAR_AGY_DCBOT_OV                           ( 0x81001075L )
#define VAR_AGY_DC_IMBALANCE                       ( 0x81001076L )
#define VAR_AGY_DC_UV                              ( 0x81001077L )
#define VAR_AGY_VDC_SENSOR                         ( 0x81001078L )
#define VAR_AGY_VDC_SENSOR_VPVREF                  ( 0x81001079L )
#define VAR_AGY_VDC_SENSOR_LOST                    ( 0x8100107AL )
#define VAR_AGY_VDC_SENSOR_IMB                     ( 0x8100107BL )
#define VAR_AGY_PV_OC_RMS                          ( 0x8100107CL )
#define VAR_AGY_PV_OC_INST                         ( 0x8100107DL )
#define VAR_AGY_DC_OT                              ( 0x8100107EL )
#define VAR_AGY_APS_LOW                            ( 0x8100107FL )
#define VAR_AGY_APS_LOW_VDCREF                     ( 0x81001080L )
#define VAR_AGY_IRCD                               ( 0x81001081L )
#define VAR_AGY_DC_OT_WRN                          ( 0x81001082L )
#define VAR_AGY_COOLING_LOST                       ( 0x81001083L )
#define VAR_AGY_PV_MISSING_LIMIT                   ( 0x81001084L )
#define VAR_AGY_PV_MISSING_REF                     ( 0x81001085L )

#define VAR_AGY_PV_OV_HYST                         ( 0x81001090L )
#define VAR_AGY_VPV_LOW_HYST                       ( 0x81001091L )
#define VAR_AGY_PPV_LOW_HYST                       ( 0x81001092L )
#define VAR_AGY_DC_OV_HYST                         ( 0x81001093L )
#define VAR_AGY_DCTOP_OV_HYST                      ( 0x81001094L )
#define VAR_AGY_DCBOT_OV_HYST                      ( 0x81001095L )
#define VAR_AGY_DC_IMBALANCE_HYST                  ( 0x81001096L )
#define VAR_AGY_DC_UV_HYST                         ( 0x81001097L )
#define VAR_AGY_PV_OC_RMS_HYST                     ( 0x81001098L )
#define VAR_AGY_PV_OC_INST_HYST                    ( 0x81001099L )
#define VAR_AGY_DC_OT_HYST                         ( 0x8100109AL )
#define VAR_AGY_APS_LOW_HYST                       ( 0x8100109BL )
#define VAR_AGY_COOLING_LOST_HYST                  ( 0x8100109CL )
#define VAR_AGY_PV_MISSING_HYST                    ( 0x8100109DL )


/* Device Definitions */
#define VAR_MDL_POWER_RATING                       ( 0x82000001L )
#define VAR_MDL_TEMP1_MIN                          ( 0x82000002L )
#define VAR_MDL_TEMP1_MAX                          ( 0x82000003L )
#define VAR_MDL_DC_CURRENT_MIN                     ( 0x82000004L )
#define VAR_MDL_DC_CURRENT_MAX                     ( 0x82000005L )
#define VAR_MDL_AC_CURRENT_MIN                     ( 0x82000006L )
#define VAR_MDL_AC_CURRENT_MAX                     ( 0x82000007L )

/* faults and warnings: range for GTTL - single phase */
#define FLT_ALL_START                              ( 0xF0000000L )
#define FLT_ALL_END                                ( 0xF00000FFL )

/* variables for testing the sysvars module */
#define VAR_GT_SYSVARS_TEST_VAR16U                 ( 0xFFFFFF00L )
#define VAR_GT_SYSVARS_TEST_VAR32U                 ( 0xFFFFFF01L )
#define VAR_GT_SYSVARS_TEST_VAR16S                 ( 0xFFFFFF02L )
#define VAR_GT_SYSVARS_TEST_VAR32S                 ( 0xFFFFFF03L )
#define VAR_GT_SYSVARS_TEST_ARY16                  ( 0xFFFFFF04L )
#define VAR_GT_SYSVARS_TEST_ARY32                  ( 0xFFFFFF05L )
#define VAR_GT_SYSVARS_TEST_STR                    ( 0xFFFFFF06L )
#define VAR_GT_SYSVARS_TEST_CRIT16U                ( 0xFFFFFF07L )
#define VAR_GT_SYSVARS_TEST_CRIT32U                ( 0xFFFFFF08L )
#define VAR_GT_SYSVARS_TEST_CRIT16S                ( 0xFFFFFF09L )
#define VAR_GT_SYSVARS_TEST_CRIT32S                ( 0xFFFFFF0AL )
#define VAR_GT_SYSVARS_TEST_CRIT32F                ( 0xFFFFFF0BL )

#define VAR_GT_SYSVARS_TEST_UNDEF                  ( 0xFFFFFFFFL )


#endif
