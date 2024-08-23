###############################################################################
# (c) 2024 Xantrex Canada ULC. All rights reserved.
# All trademarks are owned or licensed by Xantrex Canada ULC,
# its subsidiaries or affiliated companies.
#
# FILE NAME:  FswAlerts.py
#
# PURPOSE:
#     This file contains the lookup tables for faults and warnings supported
#     by the FSW family of inverter chargers.  The lookup tables allow
#     RV-C-specific SPNs and FMIs to be converted into device-native fault
#     and warning codes and strings
#
###############################################################################

###############################################################################
#                               Includes
###############################################################################

from RvcDefs import *

###############################################################################
#                             Constant Data
###############################################################################

# List of fault and warning codes for FSW family, from software/Agency/Inc/FaultWarnNum.h

FLTWRNN_AC_OUT_UV_SD =             1
FLTWRNN_AC_OUT_OV_SD =             2

#---------------------------------------------
#          ACBF collective fault
#---------------------------------------------
FLTWRNN_ACBF_AC1_L1_SD =           17
FLTWRNN_ACBF_GRP_BTM =             FLTWRNN_ACBF_AC1_L1_SD
FLTWRNN_ACBF_ACX_L1_SD =           22   # line 1 weld

#---------------------------------------------
#          AI collective fault
#---------------------------------------------
FLTWRNN_AI_QUAL_OF_SD =            23
FLTWRNN_AI_GRP_BTM =               FLTWRNN_AI_QUAL_OF_SD
FLTWRNN_AI_QUAL_UF_SD =            24
FLTWRNN_AI_OF_SD =                 25
FLTWRNN_AI_UF_SD =                 26
FLTWRNN_AI_OV_L1_FAST_SD =         27
FLTWRNN_AI_OV_L2_FAST_SD =         28
FLTWRNN_AI_QUAL_OV_SD =            29
FLTWRNN_AI_OV_L1L2_FAST_SD =       30
FLTWRNN_AI_OV_L1_SLOW_SD =         31
FLTWRNN_AI_OV_L2_SLOW_SD =         32
FLTWRNN_AI_OV_L1L2_SLOW_SD =       33
FLTWRNN_AI_UV_L1_SLOW_SD =         34
FLTWRNN_AI_UV_L2_SLOW_SD =         35
FLTWRNN_AI_UV_L1L2_SLOW_SD =       36
FLTWRNN_AI_UV_L1_FAST_SD =         37
FLTWRNN_AI_UV_L2_FAST_SD =         38
FLTWRNN_AI_QUAL_UV_SD =            39
FLTWRNN_AI_UV_L1L2_FAST_SD =       40
FLTWRNN_AI_GRP_TOP =               FLTWRNN_AI_UV_L1L2_FAST_SD

FLTWRNN_BATT_OT_SD =               44
FLTWRNN_CAP_OT_SD =                45

FLTWRNN_CONTROLLER_ERR_SD =        46

FLTWRNN_DC_UV_IMM_SD =             47
FLTWRNN_DC_UV_SD =                 48
FLTWRNN_DC_OV_SD =                 49

FLTWRNN_EE_SD =                    51
FLTWRNN_EE_CAL_FAIL =              52
FLTWRNN_EE_CONFIG_FAIL =           53
FLTWRNN_EE_DEFAULT_FAIL =          54
FLTWRNN_EE_LOG_FAIL =              55
FLTWRNN_EE_STRINGS_FAIL =          56

FLTWRNN_FET1_OT_SD =               57
FLTWRNN_FET2_OT_SD =               58

# Xanbus specific fault
FLTWRNN_GO_CONFIG_YOURSELF =       59

#---------------------------------------------
# Invalid fault / warning codes
#---------------------------------------------
FLTWRNN_INVALID_FLT_CODE =         60
FLTWRNN_INVALID_WRN_CODE =         61
FLTWRNN_INVALID_INTERRUPT =        62

FLTWRNN_PWRBRD_SENSOR_FAIL =       63
#---------------------------------------------
#          ACOL collective fault
#---------------------------------------------
FLTWRNN_OL_PRIM_SD =                           64
FLTWRNN_SYS_CFG_SD =                           66
FLTWRNN_WD_RST_SD =                            67 # Watch dog reset
FLTWRNN_XFMR_OT_SD =                           68
FLTWRNN_XSIG_BAD_SD =                          69
FLTWRNN_SYS_DEV_ID_CFG_SD =                    70
FLTWRNN_SYS_MASTER_CFG_SD =                    71
FLTWRNN_SYS_AC_ASSOC_CFG_SD =                  72
FLTWRNN_XMFR_SENSOR_DISCONNECT_SD =            73
FLTWRNN_SYS_OTH_INV_FLT_SD =                   74
FLTWRNN_SYS_OTH_INV_XB_LOST_SD =               75
FLTWRNN_SYS_S_S_NO_MASTERS =                   76 # slave-slave no masters
FLTWRNN_SYS_PH2M_S_NO_MASTERS =                77 # Phase2master/slave no masters
FLTWRNN_BATT_TEMP_SENSOR_SHORT_FLT =           79
FLTWRNN_PWR_BOARD_OVER_TEMP_SD =               85

FLTWRNN_DEAD_CHARGE_NOT_AVAILABLE =            86
FLTWRNN_L2_ACBF =                              18

FLTWRNN_OTH_FREQ_DIFF_SD =                     87

FLTWRNN_MPPT_GROUND_FLT_SD =                   88


##############################################################################
# Warning IDs

# Warnings that have no fault counterparts will start at 99 and go down
FLTWRNN_AUTORSTR_EN_WRN =              99  # Autorestart is enabled
FLTWRNN_CANNOT_EQUALIZE_WRN =          96
FLTWRNN_EQUALIZE_ABORT_WRN =           95
FLTWRNN_XMFR_SENSOR_DISCONNECT_WRN =   94
FLTWRNN_BATT_TEMP_SENSOR_ERR_WRN =     93
FLTWRNN_CONN_TO_THREE_PHASE_WRN =      92


#-------------------------------------------------
# Warnings with fault counterparts will have the
# same number as their corresponding fault code
#-------------------------------------------------
FLTWRNN_BATT_TEMP_OT_WRN =             44
FLTWRNN_CAP_OT_WRN =                   45

FLTWRNN_INV_DC_UV_WRN =                48
FLTWRNN_DC_OV_WRN =                    49

FLTWRNN_FET1_OT_WRN =                  57
FLTWRNN_FET2_OT_WRN =                  58

FLTWRNN_OL_PRIM_WRN =                  64
FLTWRNN_XFMR_OT_WRN =                  68

FLTWRNN_SYS_AC_ASSOC_CFG_WRN =         72
FLTWRNN_OTH_FREQ_DIFF_WRN =            87

##############################################################################
# Fault/warning group defines

# AC1IN OV, UV, fault and warn; ACIN OF, UF, fault and warn
FLTWRNN_AC1IN_ALL =                    199
# AC2IN OV, UV, fault and warn; ACIN OF, UF, fault and warn
FLTWRNN_AC2IN_ALL =                    200
# ACOUT OV, UV, fault and warn
FLTWRNN_INVERT_ALL =                   201
# DC UV fault and warn
FLTWRNN_DCUV_ALL =                     202
# RMS OL fault and warn
FLTWRNN_OL_RMS_ALL =                   203
# APS OV, UV, fault
FLTWRNN_APS_ALL =                      204
# AI OF, UF, OV, UV, fault
FLTWRNN_AI_ALL =                       205
# Multi unit sync fault
FLTWRNN_MULTI_ALL =                    206
# Qualification transitional faults/warns
FTWRNN_AC_BAD_ALL =                    207

# External contactor faults/warns
FTWRNN_MULTI_UNIT_ALL =                208

FLTWRN_UNDEFINED =                     555


# Fault and warning strings from english.c
# See Agency.c and stringids.h for matching between fault/warning codes and these strings
# This should be a dictionary matching FLTWRN_* code to string
FAULT_WARNING_STRINGS = {
    FLTWRNN_AC_OUT_OV_SD : "AC Output Over Voltage",
    FLTWRNN_AC_OUT_UV_SD : "AC Output Under Voltage",
    FLTWRNN_ACBF_AC1_L1_SD : "Relay(s) Welded",
    FLTWRNN_ACBF_ACX_L1_SD : "Relay(s) Welded",
    FLTWRNN_L2_ACBF : "Relay(s) Welded",
    FLTWRNN_BATT_OT_SD : "Battery Over Temperature",
    FLTWRNN_CAP_OT_SD : "Capacitor Over Temperature",
    FLTWRNN_CONTROLLER_ERR_SD : "Controller Error",
    FLTWRNN_DC_OV_SD : "DC Over Voltage",
    FLTWRNN_DC_UV_SD : "DC Under Voltage",
    FLTWRNN_DC_UV_IMM_SD : "DC Under Voltage",
    FLTWRNN_EE_SD : "NVM Error",
    FLTWRNN_EE_CAL_FAIL : "NVM Error",
    FLTWRNN_EE_LOG_FAIL : "NVM Error",
    FLTWRNN_EE_STRINGS_FAIL : "NVM Error",
    FLTWRNN_EE_CONFIG_FAIL : "NVM Error",
    FLTWRNN_EE_DEFAULT_FAIL : "NVM Error",
    FLTWRNN_FET1_OT_SD : "FET1 Over Temperature",
    FLTWRNN_FET2_OT_SD : "FET2 Over Temperature",
    FLTWRNN_GO_CONFIG_YOURSELF : "GOCFG Process Failed",
    FLTWRNN_INVALID_INTERRUPT : "Invalid Interrupt",
    FLTWRNN_INVALID_FLT_CODE : "Invalid Fault",
    FLTWRNN_INVALID_WRN_CODE : "Invalid Warning",
    FLTWRNN_PWRBRD_SENSOR_FAIL : "Power Board Sensor Failed",
    FLTWRNN_OL_PRIM_SD : "AC Overload",
    FLTWRNN_SYS_CFG_SD : "System Configuration Fault",
    FLTWRNN_WD_RST_SD : "Watchdog Error",
    FLTWRNN_XFMR_OT_SD : "Transformer Over Temperature",
    FLTWRNN_XSIG_BAD_SD : "External Sync Failed",
    FLTWRNN_SYS_DEV_ID_CFG_SD : "System Device ID Configuration Fault",
    FLTWRNN_SYS_MASTER_CFG_SD : "System Master Configuration Fault",
    FLTWRNN_SYS_AC_ASSOC_CFG_SD : "System AC Association Configuration Fault",
    FLTWRNN_XMFR_SENSOR_DISCONNECT_SD : "Transformer Temperature Sensor Disconnected",
    FLTWRNN_SYS_OTH_INV_FLT_SD : "System Other Unit Fault",
    FLTWRNN_SYS_OTH_INV_XB_LOST_SD : "System Other Unit Lost",
    FLTWRNN_SYS_S_S_NO_MASTERS : "System Slave No Master Fault", # slave-slave no masters
    FLTWRNN_SYS_PH2M_S_NO_MASTERS : "System No Phase1 Master Fault",
    FLTWRNN_BATT_TEMP_SENSOR_SHORT_FLT  : "Battery Temperature Sensor Short",
    FLTWRNN_PWR_BOARD_OVER_TEMP_SD : "Power Board Over Temperature",
    FLTWRNN_DEAD_CHARGE_NOT_AVAILABLE : "Dead Battery Charge Not Available",
    FLTWRNN_OTH_FREQ_DIFF_SD : "Other Unit Frequency Different Fault",
    FLTWRNN_MPPT_GROUND_FLT_SD : "MPPT Ground Fault",

    FLTWRNN_AUTORSTR_EN_WRN : "Autorestart Is Enabled",
    FLTWRNN_CANNOT_EQUALIZE_WRN : "Can not Equalize",
    FLTWRNN_EQUALIZE_ABORT_WRN : "Equalize Abort",
    FLTWRNN_XMFR_SENSOR_DISCONNECT_WRN : "Transformer Temperature Sensor Disconnected",
    FLTWRNN_BATT_TEMP_SENSOR_ERR_WRN : "Battery Temperature Sensor Error",
    FLTWRNN_CONN_TO_THREE_PHASE_WRN : "Connected to Two of Three Phase",
    FLTWRNN_BATT_TEMP_OT_WRN : "Battery Over Temperature",
    FLTWRNN_CAP_OT_WRN : "Capacitor Over Temperature",
    FLTWRNN_INV_DC_UV_WRN : "DC Under Voltage",
    FLTWRNN_DC_OV_WRN : "DC Over Voltage",
    FLTWRNN_FET1_OT_WRN : "FET1 Over Temperature",
    FLTWRNN_FET2_OT_WRN : "FET2 Over Temperature",
    FLTWRNN_OL_PRIM_WRN : "AC Overload",
    FLTWRNN_XFMR_OT_WRN : "Transformer Over Temperature",
    FLTWRNN_SYS_AC_ASSOC_CFG_WRN : "System AC Association Configuration",
    FLTWRNN_OTH_FREQ_DIFF_WRN : "Other Unit Frequency Different",
}



# Look up table for converting SPN/FMI from DiagMsg to the FSW internal fault/warning code
# based on lookup table in software/XbNet/Pgn/Src/rvcreq.c

atzFltLookup = [
  ( FLTWRNN_AC_OUT_UV_SD,               RVC_SPN_MSB_RMS_VOLTAGE,   RVC_mAC_OUT_L1_SPN_ISB( 1 ),          RVC_SPN_LSB_RMS_VOLTAGE,              RVC_eSAEJ1939FMI_DATUM_BELOW_NORMAL_OPERATING_RANGE ),
  ( FLTWRNN_AC_OUT_OV_SD,               RVC_SPN_MSB_RMS_VOLTAGE,   RVC_mAC_OUT_L1_SPN_ISB( 1 ),          RVC_SPN_LSB_RMS_VOLTAGE,              RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE ),
  ( FLTWRNN_ACBF_AC1_L1_SD,             RVC_SPN_MSB_AC_BACKFEED,   RVC_mAC_IN_L1_SPN_ISB( 1 ),           RVC_SPN_LSB_AC_BACKFEED,              RVC_eSAEJ1939FMI_MECHANICAL_SYSTEM_NOT_RESPONDING ),
  ( FLTWRNN_ACBF_ACX_L1_SD,             RVC_SPN_MSB_AC_BACKFEED,   RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_AC_BACKFEED,              RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_BATT_OT_SD,                 RVC_SPN_MSB_BATT_TEMP,     RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_BATT_TEMP,                RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE ),
  ( FLTWRNN_CAP_OT_SD,                  RVC_SPN_MSB_CAP_TEMP,      RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_CAP_TEMP,                 RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE ),
  ( FLTWRNN_CONTROLLER_ERR_SD,          RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_MICRO),     RVC_mSPN_LSB(RVC_SPN_NODE_MICRO),     RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_DC_UV_IMM_SD,               RVC_SPN_MSB_DC_VOLTAGE,    RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_DC_VOLTAGE,               RVC_eSAEJ1939FMI_DATUM_BELOW_NORMAL_OPERATING_RANGE ),
  ( FLTWRNN_DC_UV_SD,                   RVC_SPN_MSB_DC_VOLTAGE,    RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_DC_VOLTAGE,               RVC_eSAEJ1939FMI_DATUM_BELOW_NORMAL_OPERATING_RANGE ),
  ( FLTWRNN_DC_OV_SD,                   RVC_SPN_MSB_DC_VOLTAGE,    RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_DC_VOLTAGE,               RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE ),

  ( FLTWRNN_EE_SD,                      RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_MICRO),     RVC_mSPN_LSB(RVC_SPN_NODE_MICRO),     RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_EE_CAL_FAIL,                RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_CFG),       RVC_mSPN_LSB(RVC_SPN_NODE_CFG),       RVC_eSAEJ1939FMI_OUT_OF_CALIBRATION ),
  ( FLTWRNN_EE_CONFIG_FAIL,             RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_CFG),       RVC_mSPN_LSB(RVC_SPN_NODE_CFG),       RVC_eSAEJ1939FMI_DATUM_ERRATIC_INTERMITTENT_INVALID ),
  ( FLTWRNN_EE_DEFAULT_FAIL,            RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_FW),        RVC_mSPN_LSB(RVC_SPN_NODE_FW),        RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_EE_LOG_FAIL,                RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_RAM),       RVC_mSPN_LSB(RVC_SPN_NODE_RAM),       RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_EE_STRINGS_FAIL,            RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_RAM),       RVC_mSPN_LSB(RVC_SPN_NODE_RAM),       RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),

  ( FLTWRNN_FET1_OT_SD,                 RVC_SPN_MSB_FET1_TEMP,     RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_FET1_TEMP,                RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE ),
  ( FLTWRNN_FET2_OT_SD,                 RVC_SPN_MSB_FET2_TEMP,     RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_FET2_TEMP,                RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE ),
  ( FLTWRNN_INVALID_FLT_CODE,           RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_FW),        RVC_mSPN_LSB(RVC_SPN_NODE_FW),        RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_INVALID_WRN_CODE,           RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_FW),        RVC_mSPN_LSB(RVC_SPN_NODE_FW),        RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_INVALID_INTERRUPT,          RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_FW),        RVC_mSPN_LSB(RVC_SPN_NODE_FW),        RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_PWRBRD_SENSOR_FAIL,         RVC_SPN_MSB_CTRL_BRD_TEMP, RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_CTRL_BRD_TEMP,            RVC_eSAEJ1939FMI_DATUM_ERRATIC_INTERMITTENT_INVALID ),
  ( FLTWRNN_OL_PRIM_SD,                 RVC_SPN_MSB_RMS_CURRENT,   RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_RMS_CURRENT,              RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE ),
  ( FLTWRNN_SYS_CFG_SD,                 RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_CFG),       RVC_mSPN_LSB(RVC_SPN_NODE_CFG),       RVC_eSAEJ1939FMI_DATUM_ERRATIC_INTERMITTENT_INVALID ),
  ( FLTWRNN_WD_RST_SD,                  RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_WD),        RVC_mSPN_LSB(RVC_SPN_NODE_WD),        RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_XFMR_OT_SD,                 RVC_SPN_MSB_XFMR_TEMP,     RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_XFMR_TEMP,                RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE ),
  ( FLTWRNN_XSIG_BAD_SD,                RVC_SPN_MSB_STACK_SYNC,    RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_STACK_SYNC,               RVC_eSAEJ1939FMI_ABNORMAL_FREQUENCY_PULSE_WIDTH_PERIOD ),
  ( FLTWRNN_SYS_DEV_ID_CFG_SD,          RVC_SPN_MSB_GLOBAL,        RVC_mSPN_ISB(RVC_SPN_NODE_SERIAL_ID), RVC_mSPN_LSB(RVC_SPN_NODE_SERIAL_ID), RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_SYS_MASTER_CFG_SD,          RVC_SPN_MSB_STACK_CFG,     RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_STACK_CFG,                RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_SYS_AC_ASSOC_CFG_SD,        RVC_SPN_MSB_STACK_CFG,     RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_STACK_CFG,                RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_XMFR_SENSOR_DISCONNECT_SD,  RVC_SPN_MSB_XFMR_TEMP,     RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_XFMR_TEMP,                RVC_eSAEJ1939FMI_DATUM_ERRATIC_INTERMITTENT_INVALID ),
  ( FLTWRNN_SYS_OTH_INV_FLT_SD,         RVC_SPN_MSB_STACK_COMM,    RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_STACK_COMM,               RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_SYS_OTH_INV_XB_LOST_SD,     RVC_SPN_MSB_STACK_COMM,    RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_STACK_COMM,               RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_SYS_S_S_NO_MASTERS,         RVC_SPN_MSB_STACK_CFG,     RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_STACK_CFG,                RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_SYS_PH2M_S_NO_MASTERS,      RVC_SPN_MSB_STACK_CFG,     RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_STACK_CFG,                RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_BATT_TEMP_SENSOR_SHORT_FLT, RVC_SPN_MSB_BATT_TEMP,     RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_BATT_TEMP,                RVC_eSAEJ1939FMI_DATUM_ERRATIC_INTERMITTENT_INVALID ),
  ( FLTWRNN_PWR_BOARD_OVER_TEMP_SD,     RVC_SPN_MSB_CTRL_BRD_TEMP, RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_CTRL_BRD_TEMP,            RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE ),
  ( FLTWRNN_L2_ACBF,                    RVC_SPN_MSB_AC_BACKFEED,   RVC_mAC_IN_L2_SPN_ISB( 1 ),           RVC_SPN_LSB_AC_BACKFEED,              RVC_eSAEJ1939FMI_MECHANICAL_SYSTEM_NOT_RESPONDING ),
  ( FLTWRNN_OTH_FREQ_DIFF_SD,           RVC_SPN_MSB_STACK_CFG,     RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_STACK_CFG,                RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_MPPT_GROUND_FLT_SD,         RVC_SPN_MSB_GND_FLT,       RVC_USE_INV_CHG_INST,                 RVC_SPN_LSB_GND_FLT,                  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRN_UNDEFINED,                   RVC_SPN_MSB_GLOBAL,        0,                                    0,                                    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE )
]

atzWrnLookup = [
  ( FLTWRNN_CANNOT_EQUALIZE_WRN,        RVC_SPN_MSB_BATT_EQLZ,  RVC_USE_INV_CHG_INST, RVC_SPN_LSB_BATT_EQLZ,  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_EQUALIZE_ABORT_WRN,         RVC_SPN_MSB_BATT_EQLZ,  RVC_USE_INV_CHG_INST, RVC_SPN_LSB_BATT_EQLZ,  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_XMFR_SENSOR_DISCONNECT_WRN, RVC_SPN_MSB_XFMR_TEMP,  RVC_USE_INV_CHG_INST, RVC_SPN_LSB_XFMR_TEMP,  RVC_eSAEJ1939FMI_DATUM_ERRATIC_INTERMITTENT_INVALID ),
  ( FLTWRNN_BATT_TEMP_SENSOR_ERR_WRN,   RVC_SPN_MSB_BATT_TEMP,  RVC_USE_INV_CHG_INST, RVC_SPN_LSB_BATT_TEMP,  RVC_eSAEJ1939FMI_DATUM_ERRATIC_INTERMITTENT_INVALID ),
  ( FLTWRNN_CONN_TO_THREE_PHASE_WRN,    RVC_SPN_MSB_PHASE_STS,  RVC_USE_INV_CHG_INST, RVC_SPN_LSB_PHASE_STS,  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_BATT_TEMP_OT_WRN,           RVC_SPN_MSB_BATT_TEMP,  RVC_USE_INV_CHG_INST, RVC_SPN_LSB_BATT_TEMP,  RVC_eSAEJ1939FMI_DATUM_VALID_BUT_ABOVE_NORMAL_LEAST_SEVERE ),
  ( FLTWRNN_INV_DC_UV_WRN,              RVC_SPN_MSB_DC_VOLTAGE, RVC_USE_INV_CHG_INST, RVC_SPN_LSB_DC_VOLTAGE, RVC_eSAEJ1939FMI_DATUM_VALID_BUT_BELOW_NORMAL_LEAST_SEVERE ),
  ( FLTWRNN_DC_OV_WRN,                  RVC_SPN_MSB_DC_VOLTAGE, RVC_USE_INV_CHG_INST, RVC_SPN_LSB_DC_VOLTAGE, RVC_eSAEJ1939FMI_DATUM_VALID_BUT_ABOVE_NORMAL_LEAST_SEVERE ),
  ( FLTWRNN_FET1_OT_WRN,                RVC_SPN_MSB_FET1_TEMP,  RVC_USE_INV_CHG_INST, RVC_SPN_LSB_FET1_TEMP,  RVC_eSAEJ1939FMI_DATUM_VALID_BUT_ABOVE_NORMAL_LEAST_SEVERE ),
  ( FLTWRNN_FET2_OT_WRN,                RVC_SPN_MSB_FET2_TEMP,  RVC_USE_INV_CHG_INST, RVC_SPN_LSB_FET2_TEMP,  RVC_eSAEJ1939FMI_DATUM_VALID_BUT_ABOVE_NORMAL_LEAST_SEVERE ),
  ( FLTWRNN_XFMR_OT_WRN,                RVC_SPN_MSB_XFMR_TEMP,  RVC_USE_INV_CHG_INST, RVC_SPN_LSB_XFMR_TEMP,  RVC_eSAEJ1939FMI_DATUM_VALID_BUT_ABOVE_NORMAL_LEAST_SEVERE ),
  ( FLTWRNN_SYS_AC_ASSOC_CFG_WRN,       RVC_SPN_MSB_STACK_CFG,  RVC_USE_INV_CHG_INST, RVC_SPN_LSB_STACK_CFG,  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRNN_OTH_FREQ_DIFF_WRN,          RVC_SPN_MSB_STACK_CFG,  RVC_USE_INV_CHG_INST, RVC_SPN_LSB_STACK_CFG,  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE ),
  ( FLTWRN_UNDEFINED,                   RVC_SPN_MSB_GLOBAL,     0,                    0,                      RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE )
]


###############################################################################
#                           Function Definitions
###############################################################################

# Look up a fault code, given the components of an SPN an FMI code
def fnGetFaultCode(spn_msb, spn_isb, spn_lsb, fmi):
    for fault_code, test_msb, test_isb, test_lsb, test_fmi in atzFltLookup:
        if ((test_msb == spn_msb)
            and ((test_isb == spn_isb) or (test_isb == RVC_USE_INV_CHG_INST))
            and (test_lsb == spn_lsb)
            and (test_fmi == fmi)):
            # Found a match!
            return fault_code

    # No match found
    return None

# Look up a warning code, given the components of an SPN an FMI code
def fnGetWarnCode(spn_msb, spn_isb, spn_lsb, fmi):
    for warn_code, test_msb, test_isb, test_lsb, test_fmi in atzWrnLookup:
        if ((test_msb == spn_msb)
            and ((test_isb == spn_isb) or (test_isb == RVC_USE_INV_CHG_INST))
            and (test_lsb == spn_lsb)
            and (test_fmi == fmi)):
            # Found a match!
            return warn_code

    # No match found
    return None

# Get the string that describes a fault or warning
def fnGetAlertString(alert_code):
    return FAULT_WARNING_STRINGS.get(alert_code, 'Unknown {}'.format(alert_code))

# Get the fault or warning embedded in a DiagMsg1 PGN
def fnReadDiagMsg1(msg):
    # By default, no active faults or warnings indicated
    alert_code = None
    alert_type = None
    alert_string = ''

    # Is this a fault, a warning or an indication of no errors?
    if msg.fnGetValue('OpStsRed') == 'On':
        # Interpret as a fault
        alert_type = 'faults'
        alert_code = fnGetFaultCode(msg.fnGetRawValue('SpnMsb', 0),
                                    msg.fnGetRawValue('SpnIsb', 0),
                                    msg.fnGetRawValue('SpnLsb', 0),
                                    msg.fnGetRawValue('Fmi', 0))
        if alert_code is None:
            print("No fault code found for FSW %s" % msg)
        else:
            alert_string = fnGetAlertString(alert_code)

    elif msg.fnGetValue('OpStsYel') == 'On':
        alert_type = 'warnings'
        alert_code = fnGetWarnCode(msg.fnGetRawValue('SpnMsb', 0),
                                   msg.fnGetRawValue('SpnIsb', 0),
                                   msg.fnGetRawValue('SpnLsb', 0),
                                   msg.fnGetRawValue('Fmi', 0))
        if alert_code is None:
            print("No warning code found for FSW %s" % msg)
        else:
            alert_string = fnGetAlertString(alert_code)

    return alert_type, alert_code, alert_string
