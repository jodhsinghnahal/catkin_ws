###############################################################################
# (c) 2020 Xantrex Canada ULC. All rights reserved.
# All trademarks are owned or licensed by Xantrex Canada ULC,
# its subsidiaries or affiliated companies.
#
# FILE NAME:  FxccAlerts.py
#
# PURPOSE:
#     This file contains the lookup tables for faults and warnings supported
#     by the FXC family of inverter chargers.  The lookup tables allow
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

# Fault/warning flags from enum defined in fxcc's fxcc/Inc/inv_data.h

INVDATA_eDATA_ID = ["PROTOCOL",
                    "SERNO",
                    "FW_VER_1",
                    "GRID_VOLT_RATING",
                    "GRID_CUR_RATING",
                    "AC_OUT_VOLT_RATING",
                    "AC_OUT_FREQ_RATING",
                    "AC_OUT_CUR_RATING",
                    "AC_OUT_APP_PWR_RATING",
                    "AC_OUT_ACT_PWR_RATING",
                    "BATT_VOLT_RATING",
                    "BATT_RECH_VOLT_THRESH",
                    "BATT_UNDER_VOLT_THRESH",
                    "BATT_ABS_VOLT",
                    "BATT_FLOAT_VOLT",
                    "BATT_TYPE",
                    "MAX_AC_CHG_CUR",
                    "MAX_CHG_CUR",
                    "TRANSFER_MODE",
                    "OUT_SRC_PRIO",
                    "CHG_SRC_PRIO",
                    "PARALLEL_MAX",
                    "MACH_TYPE",
                    "TOPOLOGY",
                    "OUTPUT_MODE",
                    "BATT_DISCH_VOLT",
                    "PV_OK_COND",
                    "PV_PWR_BAL",
                    "BUZZER_ON_FLAG",
                    "OVLD_BYPASS_FLAG",
                    "PWR_SAVING_FLAG",
                    "LCD_ESC_TO_DFLT_FLAG",
                    "OVLD_RESTART_FLAG",
                    "OVER_TEMP_RESTART_FLAG",
                    "BACKLIGHT_ON_FLAG",
                    "ALARM_ON_FLAG",
                    "FLT_CODE_RECORD_FLAG",
                    "GRID_VOLT",
                    "GRID_FREQ",
                    "AC_OUT_VOLT",
                    "AC_OUT_FREQ",
                    "AC_OUT_APP_PWR",
                    "AC_OUT_ACT_PWR",
                    "OUT_LOAD_PER",
                    "BUS_VOLT",
                    "BATT_VOLT",
                    "BATT_CHG_CUR",
                    "BATT_CAP",
                    "INV_HS_TEMP",
                    "AC_IN_CUR",
                    "BATT_VOLT_SCC",
                    "BATT_DISCH_CUR",
                    "ADD_SBU_PRIO_VER",
                    "CFG_CHANGED",
                    "SCC_FW_UPDATED",
                    "LOAD_ON",
                    "BATT_VOLT_STEADY",
                    "CHG_STATUS",
                    "BATT_VOLT_FOR_FANS",
                    "EEPROM_VER",
                    "PV_CHG_PWR",
                    "CHG_FLOAT_MODE",
                    "IGN_SIG_CHECK",
                    "BEEP_SILENCE",
                    "LBCO_SHUTDOWN_EN",
                    "LBCO_BUZZ_PERIODIC",
                    "LBCO_BUZZ_ONCE",
                    "DEV_MODE",
                    "INV_FLT_FLAG",
                    "BUS_OVER_FLAG",
                    "BUS_UNDER_FLAG",
                    "BUS_SOFT_FAIL_FLAG",
                    "LINE_FAIL_FLAG",
                    "OVP_SHORT_FLAG",
                    "INV_VOLT_LOW_FLAG",
                    "INV_VOLT_HIGH_FLAG",
                    "OVER_TEMP_FLAG",
                    "FAN_LOCKED_FLAG",
                    "BATT_VOLT_HIGH_FLAG",
                    "BATT_LOW_ALARM_FLAG",
                    "BATT_LOW_SHUTDOWN_FLAG",
                    "OVERLOAD_FLAG",
                    "EEPROM_FLT_FLAG",
                    "INV_OVER_CUR_FLAG",
                    "INV_SOFT_FAIL_FLAG",
                    "OP_WAVE_FAIL_FLAG",
                    "OP_DC_OVER_VOLT_FLAG",
                    "BATT_OPEN_FLAG",
                    "CUR_SENSOR_FAIL_FLAG",
                    "BATT_SHORT_FLAG",
                    "PWR_LIMIT_WARN_FLAG",
                    "NTC_NO_CONNECT_FLAG",
                    "MODEL_NAME",
                    "PWR_SAVE_HRS",
                    "LBCO_DELAY_SEC",
                    "IGN_SETTING",
                    "LBCO_RECOVERY_VOLT",
                    "POWER_LIMIT",
                    "POWER_LIMIT_TIME",
                    "UTIL_LOW_LOSS_VOLT",
                    "BATT_TEMP_ASSUMPTION",
                    "CHG_STAGES",
                    "CHG_IGN_CONTROL",
                    "EQLZ_CHG_FLOODED",
                    "BREAKER_CUR",
                    "AC_OUT_VOLT_RATING_DFLT",
                    "AC_OUT_FREQ_RATING_DFLT",
                    "AC_MAX_CHG_CUR_DFLT",
                    "BATT_UNDER_VOLT_THRESH_DFLT",
                    "CHG_FLOAT_VOLT_DFLT",
                    "CHG_BULK_VOLT_DFLT",
                    "MAX_CHG_CUR_DFLT",
                    "TRANSFER_MODE_DFLT",
                    "BATT_TYPE_DFLT",
                    "BUZZER_EN_DFLT",
                    "PWR_SAVING_DFLT",
                    "INV_SD_RECOVERY_DFLT",
                    "PWR_SAVING_TIMER_DFLT",
                    "LBCO_TIMER_DFLT",
                    "IGN_CTRL_DFLT",
                    "LBCO_RECOV_VOLT_DFLT",
                    "UNIT_PWR_LIMIT",
                    "PWR_LIMIT_TIMER_DFLT",
                    "AC_LOW_LOSS_VOLT_DFLT",
                    "BATT_TEMP_DFLT",
                    "CHG_IGN_CTRL_DFLT",
                    "EQLZ_EN_DFLT",
                    "BRKR_SIZE_DFLT"]

# Create a variable INVDATA_eID_<blah> to have the enum value
for i, varstr in enumerate(INVDATA_eDATA_ID):
    exec('INVDATA_eID_%s = %d' % (varstr, i))

# Fault/warning strings defined in fxcc's fxcc/Src/inv_data.c invdata_atzVarDefs
# but consistent with enum names


# Fault and warning code lookup table from fxcc's can_nwk/Src/rvc.c

#! Mapping of faults and warnings flags to Service Point Numbers (SPNs)
#!  Used in the Diagnostic Message
rvc_atzFltSpnLookup = [
  (INVDATA_eID_BUS_OVER_FLAG,           RVC_SPN_MSB_DC_BUS,             RVC_USE_INV_CHG_INST,               RVC_SPN_LSB_VOLTAGE,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE             ),
  (INVDATA_eID_BUS_UNDER_FLAG,          RVC_SPN_MSB_DC_BUS,             RVC_USE_INV_CHG_INST,               RVC_SPN_LSB_VOLTAGE,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_DATUM_BELOW_NORMAL_OPERATING_RANGE             ),
  (INVDATA_eID_BUS_SOFT_FAIL_FLAG,      RVC_SPN_MSB_DC_BUS,             RVC_USE_INV_CHG_INST,               RVC_SPN_LSB_VOLTAGE,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE                       ),
  (INVDATA_eID_LINE_FAIL_FLAG,          RVC_SPN_MSB_RMS_VOLTAGE,        RVC_mAC_IN_L1_SPN_ISB(1),           RVC_SPN_LSB_RMS_VOLTAGE,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE                       ),
  (INVDATA_eID_OVP_SHORT_FLAG,          RVC_SPN_MSB_RMS_CURRENT,        RVC_mAC_OUT_L1_SPN_ISB(1),          RVC_SPN_LSB_RMS_CURRENT,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_CURRENT_ABOVE_NORMAL                           ),
  (INVDATA_eID_INV_VOLT_LOW_FLAG,       RVC_SPN_MSB_RMS_VOLTAGE,        RVC_mAC_OUT_L1_SPN_ISB(1),          RVC_SPN_LSB_RMS_VOLTAGE,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_DATUM_BELOW_NORMAL_OPERATING_RANGE             ),
  (INVDATA_eID_INV_VOLT_HIGH_FLAG,      RVC_SPN_MSB_RMS_VOLTAGE,        RVC_mAC_OUT_L1_SPN_ISB(1),          RVC_SPN_LSB_RMS_VOLTAGE,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE             ),
  (INVDATA_eID_OVER_TEMP_FLAG,          RVC_SPN_MSB_GLOBAL,             RVC_mSPN_ISB(RVC_SPN_NODE_TEMP),    RVC_mSPN_LSB(RVC_SPN_NODE_TEMP),
    RVC_eSAEJ1939FMI_DATUM_VALID_BUT_ABOVE_NORMAL_MODERATELY_SEVERE,    RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE             ),
  (INVDATA_eID_FAN_LOCKED_FLAG,         RVC_SPN_MSB_COOLING_FAN,        RVC_USE_INV_CHG_INST,               RVC_SPN_LSB_COOLING_FAN,
    RVC_eSAEJ1939FMI_MECHANICAL_SYSTEM_NOT_RESPONDING,                  RVC_eSAEJ1939FMI_ABNORMAL_FREQUENCY_PULSE_WIDTH_PERIOD          ),
  (INVDATA_eID_BATT_VOLT_HIGH_FLAG,     RVC_SPN_MSB_DC_VOLTAGE,         RVC_USE_INV_CHG_INST,               RVC_SPN_LSB_DC_VOLTAGE,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE             ),
  (INVDATA_eID_BATT_LOW_ALARM_FLAG,     RVC_SPN_MSB_DC_VOLTAGE,         RVC_USE_INV_CHG_INST,               RVC_SPN_LSB_DC_VOLTAGE,
    RVC_eSAEJ1939FMI_DATUM_VALID_BUT_BELOW_NORMAL_LEAST_SEVERE,         RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,              ),
  (INVDATA_eID_BATT_LOW_SHUTDOWN_FLAG,  RVC_SPN_MSB_DC_VOLTAGE,         RVC_USE_INV_CHG_INST,               RVC_SPN_LSB_DC_VOLTAGE,
    RVC_eSAEJ1939FMI_DATUM_VALID_BUT_BELOW_NORMAL_MODERATELY_SEVERE,    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,              ),
  (INVDATA_eID_OVERLOAD_FLAG,           RVC_SPN_MSB_REAL_PWR,           RVC_mAC_OUT_L1_SPN_ISB(1),          RVC_SPN_LSB_REAL_PWR,
    RVC_eSAEJ1939FMI_DATUM_VALID_BUT_ABOVE_NORMAL_MODERATELY_SEVERE,    RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE             ),
  (INVDATA_eID_EEPROM_FLT_FLAG,         RVC_SPN_MSB_GLOBAL,             RVC_mSPN_ISB(RVC_SPN_NODE_CFG),     RVC_mSPN_LSB(RVC_SPN_NODE_CFG),
    RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE,                          RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,              ),
  (INVDATA_eID_INV_OVER_CUR_FLAG,       RVC_SPN_MSB_RMS_CURRENT,        RVC_mAC_OUT_L1_SPN_ISB(1),          RVC_SPN_LSB_RMS_CURRENT,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE             ),
  (INVDATA_eID_INV_SOFT_FAIL_FLAG,      RVC_SPN_MSB_RMS_CURRENT,        RVC_mAC_OUT_L1_SPN_ISB(1),          RVC_SPN_LSB_RMS_CURRENT,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE                       ),
  (INVDATA_eID_OP_WAVE_FAIL_FLAG,       RVC_SPN_MSB_HARM_DIST,          RVC_mAC_OUT_L1_SPN_ISB(1),          RVC_SPN_LSB_HARM_DIST,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE                       ),
  (INVDATA_eID_OP_DC_OVER_VOLT_FLAG,    RVC_SPN_MSB_RMS_VOLTAGE,        RVC_mAC_OUT_L1_SPN_ISB(1),          RVC_SPN_LSB_RMS_VOLTAGE,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE                       ),
  (INVDATA_eID_BATT_OPEN_FLAG,          RVC_SPN_MSB_DC_VOLTAGE,         RVC_USE_INV_CHG_INST,               RVC_SPN_LSB_DC_VOLTAGE,
    RVC_eSAEJ1939FMI_SHORT_CIRCUIT_HIGH_VOLTAGE,                        RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,              ),
  (INVDATA_eID_CUR_SENSOR_FAIL_FLAG,    RVC_SPN_MSB_DC_CURRENT,         RVC_USE_INV_CHG_INST,               RVC_SPN_LSB_DC_CURRENT,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_FAILURE_NOT_IDENTIFIABLE                       ),
  (INVDATA_eID_BATT_SHORT_FLAG,         RVC_SPN_MSB_DC_VOLTAGE,         RVC_USE_INV_CHG_INST,               RVC_SPN_LSB_DC_VOLTAGE,
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_SHORT_CIRCUIT_LOW_VOLTAGE                      ),
  (INVDATA_eID_PWR_LIMIT_WARN_FLAG,     RVC_SPN_MSB_REAL_PWR,           RVC_mAC_OUT_L1_SPN_ISB(1),          RVC_SPN_LSB_REAL_PWR,
    RVC_eSAEJ1939FMI_DATUM_VALID_BUT_ABOVE_NORMAL_LEAST_SEVERE,         RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,              ),
# (INVDATA_eID_BTS_TEMPERATURE_FLAG,    RVC_SPN_MSB_BATT_TEMP,         RVC_USE_INV_CHG_INST,               RVC_SPN_LSB_BATT_TEMP,
#   RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_DATUM_ABOVE_NORMAL_OPERATING_RANGE            ),
  (INVDATA_eID_NTC_NO_CONNECT_FLAG,     RVC_SPN_MSB_GLOBAL,             RVC_mSPN_ISB(RVC_SPN_NODE_TEMP),    RVC_mSPN_LSB(RVC_SPN_NODE_TEMP),
    RVC_eSAEJ1939FMI_NO_FAILURE_INFORMATION_AVAILABLE,                  RVC_eSAEJ1939FMI_SHORT_CIRCUIT_HIGH_VOLTAGE                     ),
]

###############################################################################
#                           Function Definitions
###############################################################################

# Look up a fault code, given the components of an SPN an FMI code
def fnGetFaultCode(spn_msb, spn_isb, spn_lsb, fmi):
    for alert_code, test_msb, test_isb, test_lsb, wrn_fmi, flt_fmi in rvc_atzFltSpnLookup:
        if ((test_msb == spn_msb)
            and ((test_isb == spn_isb) or (test_isb == RVC_USE_INV_CHG_INST))
            and (test_lsb == spn_lsb)
            and (flt_fmi == fmi)):
            # Found a match!
            return alert_code

    # No match found
    return None

# Look up a warning code, given the components of an SPN an FMI code
def fnGetWarnCode(spn_msb, spn_isb, spn_lsb, fmi):
    for alert_code, test_msb, test_isb, test_lsb, wrn_fmi, flt_fmi in rvc_atzFltSpnLookup:
        if ((test_msb == spn_msb)
            and ((test_isb == spn_isb) or (test_isb == RVC_USE_INV_CHG_INST))
            and (test_lsb == spn_lsb)
            and (wrn_fmi == fmi)):
            # Found a match!
            return alert_code

    # No match found
    return None

# Get the string that describes a fault or warning
def fnGetAlertString(alert_code):
    return INVDATA_eDATA_ID[alert_code]

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
            print("No fxcc fault code found for %s" % msg)
        else:
            alert_string = fnGetAlertString(alert_code)

    elif msg.fnGetValue('OpStsYel') == 'On':
        alert_type = 'warnings'
        alert_code = fnGetWarnCode(msg.fnGetRawValue('SpnMsb', 0),
                                   msg.fnGetRawValue('SpnIsb', 0),
                                   msg.fnGetRawValue('SpnLsb', 0),
                                   msg.fnGetRawValue('Fmi', 0))
        if alert_code is None:
            print("No fxcc warning code found for %s" % msg)
        else:
            alert_string = fnGetAlertString(alert_code)

    return alert_type, alert_code, alert_string
