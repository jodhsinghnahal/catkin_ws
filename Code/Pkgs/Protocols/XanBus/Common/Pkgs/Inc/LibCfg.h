
/***************************************************************************
**
** PROJECT: CAN Library
**
** $Workfile:: LibCfg.h       $       $Author:: Dalem              $
**
***************************************************************************
**
** PROJECT: CAN Library
**
**
**
** DESCRIPTION: TFX CAN Library configuration information
**
**
**
**
**
**            Copyright (c) 2000 Teleflex Marine/Industrial
**                       All Rights Reserved
**   Use, duplication, or disclosure of this material, in any
**   form, is forbidden without permission from Teleflex Marine
**
CHANGE HISTORY:

    ***********************************************
    Revision: XanBus_HollyZ/2
    User: HollyZ     Date: 05/25/06  Time: 07:12PM
    Revert Xb stack to V1.2.7

    ***********************************************
    Revision: XanBus_HenryW/1
    User: HenryW     Date: 08/19/05  Time: 08:07PM
    - cleaned up history list.

    ***********************************************
    Revision: XanBus_HollyZ/1
    User: HollyZ     Date: 06/03/05  Time: 10:20PM
    Removed port micros.  CAN port usage is assigned by the application.

    ***********************************************
    Revision: GateWayPfrmANMEA_DaleM/1
    User: DaleM     Date: 02/10/05  Time: 07:13PM
    Cleaned up old history list

***************************************************************************/

#ifndef TFXLIBCFG_H
#define TFXLIBCFG_H

#define MAX_PGN_DOBS                40  // number of PGN which can be active at one time

#define MAX_FRAMES_OUT_PER_TICK     20  // number of frames transmitted on a single output cycle
#define MAX_FRAMES_IN_PER_TICK      40  // number of frames received on a single input cycle

// number of CAN ports which can be supported by SHIM layer
#define MAX_PORTS                   ( 2 )

#endif
