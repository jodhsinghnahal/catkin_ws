###############################################################################
# (c) 2020 Xantrex Canada ULC. All rights reserved.
# All trademarks are owned or licensed by Xantrex Canada ULC,
# its subsidiaries or affiliated companies.
#
# FILE NAME:  RvcDefs.py
#
# PURPOSE:
#     This file contains the equivalent of #defines found in the RVC
#     library's rvcudefs.h and rvcgdefs.h, which are used in both FexF4Alerts.py
#     and FxccAlerts.py.
#
###############################################################################

###############################################################################
#                               Includes
###############################################################################

###############################################################################
#                             Constant Data
###############################################################################

# FMI enum from rvcudefs.h
RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE =   0
RVC_eSAEJ1939FMI_DATUM_BELOW_NORMAL_OPERATING_RANGE =   1
RVC_eSAEJ1939FMI_DATUM_ERRATIC_INTERMITTENT_INVALID =   2
RVC_eSAEJ1939FMI_SHORT_CIRCUIT_HIGH_VOLTAGE =   3
RVC_eSAEJ1939FMI_SHORT_CIRCUIT_LOW_VOLTAGE =   4
RVC_eSAEJ1939FMI_CURRENT_BELOW_NORMAL =   5
RVC_eSAEJ1939FMI_CURRENT_ABOVE_NORMAL =   6
RVC_eSAEJ1939FMI_MECHANICAL_SYSTEM_NOT_RESPONDING =   7
RVC_eSAEJ1939FMI_ABNORMAL_FREQUENCY_PULSE_WIDTH_PERIOD =   8
RVC_eSAEJ1939FMI_ABNORMAL_UPDATE_RATE =   9
RVC_eSAEJ1939FMI_ABNORMAL_RATEOF_CHANGE =  10
RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE =  11
RVC_eSAEJ1939FMI_BAD_INTELLIGENT_DEVICE_COMPONENT =  12
RVC_eSAEJ1939FMI_OUT_OF_CALIBRATION =  13
RVC_eSAEJ1939FMI_NONE_OF_THE_ABOVE  =  14
RVC_eSAEJ1939FMI_DATUM_VALID_BUT_ABOVE_NORMAL_LEAST_SEVERE =  15
RVC_eSAEJ1939FMI_DATUM_VALID_BUT_ABOVE_NORMAL_MODERATELY_SEVERE =  16
RVC_eSAEJ1939FMI_DATUM_VALID_BUT_BELOW_NORMAL_LEAST_SEVERE =  17
RVC_eSAEJ1939FMI_DATUM_VALID_BUT_BELOW_NORMAL_MODERATELY_SEVERE =  18
RVC_eSAEJ1939FMI_RECEIVED_INVALID_NETWORK_DATUM =  19
RVC_eSAEJ1939FMI_POLARITY_REVERSED  =  20
RVC_eSAEJ1939FMI_NETWORKED_DEVICE_NOT_RESPONDING =  21
RVC_eSAEJ1939FMI_RESERVED1          =  22
RVC_eSAEJ1939FMI_RESERVED2          =  23
RVC_eSAEJ1939FMI_RESERVED3          =  24
RVC_eSAEJ1939FMI_RESERVED4          =  25
RVC_eSAEJ1939FMI_RESERVED5          =  26
RVC_eSAEJ1939FMI_RESERVED6          =  27
RVC_eSAEJ1939FMI_RESERVED7          =  28
RVC_eSAEJ1939FMI_RESERVED8          =  29
RVC_eSAEJ1939FMI_RESERVED9          =  30
RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE =  31

# SPN component parts from rvcgdefs.h

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#   RV-C  Service Point Number (SPN) Definitions for the DM_RV Message      #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

# Global SPNs from Table 7.3 of RV-C standard (ISB and LSB combined)
RVC_SPN_UNKNOWN =              0
RVC_SPN_NODE_MICRO =           1
RVC_SPN_NODE_ADC =             2
RVC_SPN_NODE_CFG =             3
RVC_SPN_NODE_RAM =             4
RVC_SPN_NODE_PS =              5
RVC_SPN_NODE_TEMP =            6
RVC_SPN_NODE_CLOCK =           7
RVC_SPN_NODE_DATE_TIME =       8
RVC_SPN_NODE_RVC_CONN =        9
RVC_SPN_NODE_SERIAL_ID =       10
RVC_SPN_NODE_WD =              11
RVC_SPN_NODE_FW =              12
RVC_SPN_NODE_SUBNET_COMM =     13

#! Inverter instance replacement marker
RVC_USE_INV_CHG_INST =         0xff

# MSB portion for global SPNs listed above
RVC_SPN_MSB_GLOBAL =           0

# Macro for just the ISB part of a global SPN
def RVC_mSPN_ISB( inst ):
    return ( inst >> 3 ) & 0xff

# Macro for just the LSB part of a global SPN
def RVC_mSPN_LSB( inst ):
    return inst & 0x0007

#! Macro for determining the AC Input L1 instance
def RVC_mAC_IN_L1_SPN_ISB( inst ):
    return 0x00 + ( inst & 0x0F )

#! Macro for determining the AC Input L2 instance
def RVC_mAC_IN_L2_SPN_ISB( inst ):
    return 0x10 + ( inst & 0x0F )

#! Macro for determining the AC Input L3 instance
#! @attention Not a standard definition
def RVC_mAC_IN_L3_SPN_ISB( inst ):
    return 0x20 + ( inst & 0x0F )

#! Macro for determining the AC Input L1L2 instance
#! @attention Not a standard definition
def RVC_mAC_IN_L1L2_SPN_ISB( inst ):
    return 0x30 + ( inst & 0x0F )

#! Macro for determining the AC Input L1 instance
def RVC_mAC_OUT_L1_SPN_ISB( inst ):
    return 0x40 + ( inst & 0x0F )

#! Macro for determining the AC Input L2 instance
def RVC_mAC_OUT_L2_SPN_ISB( inst ):
    return 0x50 + ( inst & 0x0F )

# SPN components for inverters and chargers as listed in Table 6.20.27a in RV-C Standard
# (ISB component indicates the instance)
RVC_SPN_MSB_DC_VOLTAGE =           1
RVC_SPN_LSB_DC_VOLTAGE =           0
RVC_SPN_MSB_DC_CURRENT =           1
RVC_SPN_LSB_DC_CURRENT =           1
RVC_SPN_MSB_BATT_TEMP =            1
RVC_SPN_LSB_BATT_TEMP =            2
RVC_SPN_MSB_DC_SRC_SOC =           1
RVC_SPN_LSB_DC_SRC_SOC =           3
RVC_SPN_MSB_DC_SRC_SOH =           1
RVC_SPN_LSB_DC_SRC_SOH =           4
RVC_SPN_MSB_DC_SRC_CAP =           1
RVC_SPN_LSB_DC_SRC_CAP =           5
RVC_SPN_MSB_AC_RIPPLE =            1
RVC_SPN_LSB_AC_RIPPLE =            6
RVC_SPN_MSB_AC_BACKFEED =          1
RVC_SPN_LSB_AC_BACKFEED =          7
RVC_SPN_MSB_FET1_TEMP =            2
RVC_SPN_LSB_FET1_TEMP =            0
RVC_SPN_MSB_FET2_TEMP =            2
RVC_SPN_LSB_FET2_TEMP =            1
RVC_SPN_MSB_CTRL_BRD_TEMP =        2
RVC_SPN_LSB_CTRL_BRD_TEMP =        2
RVC_SPN_MSB_COOLING_FAN =          2
RVC_SPN_LSB_COOLING_FAN =          3
RVC_SPN_MSB_CAP_TEMP =             3
RVC_SPN_LSB_CAP_TEMP =             0
RVC_SPN_MSB_XFMR_TEMP =            3
RVC_SPN_LSB_XFMR_TEMP =            1
RVC_SPN_MSB_AMB_TEMP =             3
RVC_SPN_LSB_AMB_TEMP =             2
RVC_SPN_MSB_BATT_CHG_TO =          3
RVC_SPN_LSB_BATT_CHG_TO =          3
RVC_SPN_MSB_BATT_EQLZ =            3
RVC_SPN_LSB_BATT_EQLZ =            4
RVC_SPN_MSB_DC_BRDG =              3
RVC_SPN_LSB_DC_BRDG =              5
RVC_SPN_MSB_XFR_RLY =              3
RVC_SPN_LSB_XFR_RLY =              6
RVC_SPN_MSB_STACK_CFG =            3
RVC_SPN_LSB_STACK_CFG =            7
RVC_SPN_MSB_STACK_COMM =           4
RVC_SPN_LSB_STACK_COMM =           0
RVC_SPN_MSB_STACK_SYNC =           4
RVC_SPN_LSB_STACK_SYNC =           1


# SPN components for inverters and chargers as listed in Table 6.20.27b in RV-C Standard
# (ISB component indicates the instance)
RVC_SPN_MSB_RMS_VOLTAGE =          0x81
RVC_SPN_LSB_RMS_VOLTAGE =          0
RVC_SPN_MSB_RMS_CURRENT =          0x81
RVC_SPN_LSB_RMS_CURRENT =          1
RVC_SPN_MSB_FREQ =                 0x81
RVC_SPN_LSB_FREQ =                 2
RVC_SPN_MSB_OPEN_GND =             0x81
RVC_SPN_LSB_OPEN_GND =             3
RVC_SPN_MSB_OPEN_NEUT =            0x81
RVC_SPN_LSB_OPEN_NEUT =            4
RVC_SPN_MSB_REV_POL =              0x81
RVC_SPN_LSB_REV_POL =              5
RVC_SPN_MSB_GND_FLT =              0x81
RVC_SPN_LSB_GND_FLT =              6
RVC_SPN_MSB_PEAK_VOLTAGE =         0x81
RVC_SPN_LSB_PEAK_VOLTAGE =         7
RVC_SPN_MSB_PEAK_CURRENT =         0x82
RVC_SPN_LSB_PEAK_CURRENT =         0
RVC_SPN_MSB_GND_CURRENT =          0x82
RVC_SPN_LSB_GND_CURRENT =          1
RVC_SPN_MSB_REAL_PWR =             0x82
RVC_SPN_LSB_REAL_PWR =             2
RVC_SPN_MSB_REACT_PWR =            0x82
RVC_SPN_LSB_REACT_PWR =            3
RVC_SPN_MSB_HARM_DIST =            0x82
RVC_SPN_LSB_HARM_DIST =            4
RVC_SPN_MSB_PHASE_STS =            0x82
RVC_SPN_LSB_PHASE_STS =            5

# SPN components for inverters and chargers as proposed by Xantrex for Anti-Islanding
# These are under review for 30 days starting July 20, 2020.
RVC_SPN_MSB_ANTI_ISLAND_FREQ =   0x83
RVC_SPN_LSB_ANTI_ISLAND_FREQ =   0
RVC_SPN_MSB_ANTI_ISLAND_VOLT =   0x83
RVC_SPN_LSB_ANTI_ISLAND_VOLT =   1

# SPN components for inverters and chargers as listed in Table 6.20.27c in RV-C Standard
# (ISB component indicates the instance)
RVC_SPN_MSB_APS  =            0x84
RVC_SPN_MSB_OPS  =            0x85
RVC_SPN_MSB_DC_BUS =          0x86
RVC_SPN_LSB_VOLTAGE =         0
RVC_SPN_LSB_CURRENT =         1
RVC_SPN_LSB_TEMP =            2
