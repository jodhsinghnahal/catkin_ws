// Interface to CAN device driver.

#ifndef DEVCAN_H
#define DEVCAN_H

#ifndef CAN_MSG_DATA_MAX
#define CAN_MSG_DATA_MAX   ( 8 )    //Max number of data bytes in a CAN message
#endif

#include <stdint.h>

#define DEVCAN_CMD_CODE    ( 1 )
#define DEVCAN_DEVCTL_GET_BUS_STATE    __DIOF(_DCMD_MISC, DEVCAN_CMD_CODE + 0, uint32_t)
#define DEVCAN_DEVCTL_GET_SW_OVERFLOWS __DIOF(_DCMD_MISC, DEVCAN_CMD_CODE + 1, uint32_t)
#define DEVCAN_DEVCTL_GET_HW_OVERFLOWS __DIOF(_DCMD_MISC, DEVCAN_CMD_CODE + 2, uint32_t)
#define DEVCAN_DEVCTL_DEBUG            __DION(_DCMD_MISC, DEVCAN_CMD_CODE + 3)

union DEVCAN_uFRAME_DATA
{
    uint8_t  aucBytes[ 8 ]; // CAN message data
    uint32_t aulWords[ 2 ];
};

typedef struct DEVCAN_zFRAME
{
    union DEVCAN_uFRAME_DATA uData;
    uint32_t ulLen;                     // Actual CAN message data length
    uint32_t ulMid;                     // CAN message identifier
} DEVCAN_tzFRAME;


#endif // DEVCAN_H
