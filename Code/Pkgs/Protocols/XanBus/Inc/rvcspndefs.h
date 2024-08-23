/*******************************************************************************

  (c) 2020 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @file rvcspndefs.h

  @brief RV-C SPN Protocol Definitions

  RV-C Service Point Number Protocol definitions used in the DM_RV message.

*******************************************************************************/

#ifndef RVC_SPN_DEFS_H
#define RVC_SPN_DEFS_H

/*=============================================================================
                              Includes
=============================================================================*/

/*=============================================================================
                            Definitions
=============================================================================*/

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//                            RV-C DM_RV Message                             //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

#define RVC_OCCURRENCE_COUNT_NOT_AVAILABLE  ( 0x7F )
#define RVC_NO_DSA_EXTENSION_DEFINED        ( 0xFF )
#define RVC_SPN_NOT_AVAILABLE               ( 0x7FFFFUL )
#define RVC_BANK_SELECT_NOT_AVAILABLE       ( 0x0F )


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   RV-C  Service Point Number (SPN) Definitions for the DM_RV Message      //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

//! Global SPNs from Table 7.3 of RV-C standard (ISB and LSB combined)
#define RVC_SPN_UNKNOWN             (  0 )
#define RVC_SPN_NODE_MICRO          (  1 )
#define RVC_SPN_NODE_ADC            (  2 )
#define RVC_SPN_NODE_CFG            (  3 )
#define RVC_SPN_NODE_RAM            (  4 )
#define RVC_SPN_NODE_PS             (  5 )
#define RVC_SPN_NODE_TEMP           (  6 )
#define RVC_SPN_NODE_CLOCK          (  7 )
#define RVC_SPN_NODE_DATE_TIME      (  8 )
#define RVC_SPN_NODE_RVC_CONN       (  9 )
#define RVC_SPN_NODE_SERIAL_ID      ( 10 )
#define RVC_SPN_NODE_WD             ( 11 )
#define RVC_SPN_NODE_FW             ( 12 )
#define RVC_SPN_NODE_SUBNET_COMM    ( 13 )

//! MSB portion for global SPNs listed above
#define RVC_SPN_MSB_GLOBAL          ( 0 )

//! Macro for just the ISB part of a global SPN
#define RVC_mSPN_ISB( x )           ( ( x >> 3 ) & 0xff )

//! Macro for just the LSB part of a global SPN
#define RVC_mSPN_LSB( x )           ( x & 0x0007 )

//! SPN components for inverters and chargers as listed in Table 6.20.27a in RV-C Standard
//! (ISB component indicates the instance)
#define RVC_SPN_MSB_DC_VOLTAGE          ( 1 )
#define RVC_SPN_LSB_DC_VOLTAGE          ( 0 )
#define RVC_SPN_MSB_DC_CURRENT          ( 1 )
#define RVC_SPN_LSB_DC_CURRENT          ( 1 )
#define RVC_SPN_MSB_BATT_TEMP           ( 1 )
#define RVC_SPN_LSB_BATT_TEMP           ( 2 )
#define RVC_SPN_MSB_DC_SRC_SOC          ( 1 )
#define RVC_SPN_LSB_DC_SRC_SOC          ( 3 )
#define RVC_SPN_MSB_DC_SRC_SOH          ( 1 )
#define RVC_SPN_LSB_DC_SRC_SOH          ( 4 )
#define RVC_SPN_MSB_DC_SRC_CAP          ( 1 )
#define RVC_SPN_LSB_DC_SRC_CAP          ( 5 )
#define RVC_SPN_MSB_AC_RIPPLE           ( 1 )
#define RVC_SPN_LSB_AC_RIPPLE           ( 6 )
#define RVC_SPN_MSB_AC_BACKFEED         ( 1 )
#define RVC_SPN_LSB_AC_BACKFEED         ( 7 )
#define RVC_SPN_MSB_FET1_TEMP           ( 2 )
#define RVC_SPN_LSB_FET1_TEMP           ( 0 )
#define RVC_SPN_MSB_FET2_TEMP           ( 2 )
#define RVC_SPN_LSB_FET2_TEMP           ( 1 )
#define RVC_SPN_MSB_CTRL_BRD_TEMP       ( 2 )
#define RVC_SPN_LSB_CTRL_BRD_TEMP       ( 2 )
#define RVC_SPN_MSB_COOLING_FAN         ( 2 )
#define RVC_SPN_LSB_COOLING_FAN         ( 3 )
#define RVC_SPN_MSB_CAP_TEMP            ( 3 )
#define RVC_SPN_LSB_CAP_TEMP            ( 0 )
#define RVC_SPN_MSB_XFMR_TEMP           ( 3 )
#define RVC_SPN_LSB_XFMR_TEMP           ( 1 )
#define RVC_SPN_MSB_AMB_TEMP            ( 3 )
#define RVC_SPN_LSB_AMB_TEMP            ( 2 )
#define RVC_SPN_MSB_BATT_CHG_TO         ( 3 )
#define RVC_SPN_LSB_BATT_CHG_TO         ( 3 )
#define RVC_SPN_MSB_BATT_EQLZ           ( 3 )
#define RVC_SPN_LSB_BATT_EQLZ           ( 4 )
#define RVC_SPN_MSB_DC_BRDG             ( 3 )
#define RVC_SPN_LSB_DC_BRDG             ( 5 )
#define RVC_SPN_MSB_XFR_RLY             ( 3 )
#define RVC_SPN_LSB_XFR_RLY             ( 6 )
#define RVC_SPN_MSB_STACK_CFG           ( 3 )
#define RVC_SPN_LSB_STACK_CFG           ( 7 )
#define RVC_SPN_MSB_STACK_COMM          ( 4 )
#define RVC_SPN_LSB_STACK_COMM          ( 0 )
#define RVC_SPN_MSB_STACK_SYNC          ( 4 )
#define RVC_SPN_LSB_STACK_SYNC          ( 1 )

//! Inverter instance replacement marker
#define RVC_USE_INV_CHG_INST            ( PGN_UC_NO_DATA )

//! Macro for determining the AC Input L1 instance
#define RVC_mAC_IN_L1_SPN_ISB( x )          ( 0x00 + ( x & 0x0F ) )

//! Macro for determining the AC Input L2 instance
#define RVC_mAC_IN_L2_SPN_ISB( x )          ( 0x10 + ( x & 0x0F ) )

//! Macro for determining the AC Input L3 instance
//! @attention Not a standard definition
#define RVC_mAC_IN_L3_SPN_ISB( x )          ( 0x20 + ( x & 0x0F ) )

//! Macro for determining the AC Input L1L2 instance
//! @attention Not a standard definition
#define RVC_mAC_IN_L1L2_SPN_ISB( x )        ( 0x30 + ( x & 0x0F ) )

//! Macro for determining the AC Input L1 instance
#define RVC_mAC_OUT_L1_SPN_ISB( x )         ( 0x40 + ( x & 0x0F ) )

//! Macro for determining the AC Input L2 instance
#define RVC_mAC_OUT_L2_SPN_ISB( x )         ( 0x50 + ( x & 0x0F ) )

//! SPN components for inverters and chargers as listed in Table 6.20.27b in RV-C Standard
//! (ISB component indicates the instance)
#define RVC_SPN_MSB_RMS_VOLTAGE         ( 0x81 )
#define RVC_SPN_LSB_RMS_VOLTAGE         (    0 )
#define RVC_SPN_MSB_RMS_CURRENT         ( 0x81 )
#define RVC_SPN_LSB_RMS_CURRENT         (    1 )
#define RVC_SPN_MSB_FREQ                ( 0x81 )
#define RVC_SPN_LSB_FREQ                (    2 )
#define RVC_SPN_MSB_OPEN_GND            ( 0x81 )
#define RVC_SPN_LSB_OPEN_GND            (    3 )
#define RVC_SPN_MSB_OPEN_NEUT           ( 0x81 )
#define RVC_SPN_LSB_OPEN_NEUT           (    4 )
#define RVC_SPN_MSB_REV_POL             ( 0x81 )
#define RVC_SPN_LSB_REV_POL             (    5 )
#define RVC_SPN_MSB_GND_FLT             ( 0x81 )
#define RVC_SPN_LSB_GND_FLT             (    6 )
#define RVC_SPN_MSB_PEAK_VOLTAGE        ( 0x81 )
#define RVC_SPN_LSB_PEAK_VOLTAGE        (    7 )
#define RVC_SPN_MSB_PEAK_CURRENT        ( 0x82 )
#define RVC_SPN_LSB_PEAK_CURRENT        (    0 )
#define RVC_SPN_MSB_GND_CURRENT         ( 0x82 )
#define RVC_SPN_LSB_GND_CURRENT         (    1 )
#define RVC_SPN_MSB_REAL_PWR            ( 0x82 )
#define RVC_SPN_LSB_REAL_PWR            (    2 )
#define RVC_SPN_MSB_REACT_PWR           ( 0x82 )
#define RVC_SPN_LSB_REACT_PWR           (    3 )
#define RVC_SPN_MSB_HARM_DIST           ( 0x82 )
#define RVC_SPN_LSB_HARM_DIST           (    4 )
#define RVC_SPN_MSB_PHASE_STS           ( 0x82 )
#define RVC_SPN_LSB_PHASE_STS           (    5 )

//! SPN components for inverters and chargers as proposed by Xantrex for Anti-Islanding
//! These are under review for 30 days starting July 20, 2020.
#define RVC_SPN_MSB_ANTI_ISLAND_FREQ    ( 0x83 )
#define RVC_SPN_LSB_ANTI_ISLAND_FREQ    (    0 )
#define RVC_SPN_MSB_ANTI_ISLAND_VOLT    ( 0x83 )
#define RVC_SPN_LSB_ANTI_ISLAND_VOLT    (    1 )

//! SPN components for inverters and chargers as listed in Table 6.20.27c in RV-C Standard
//! (ISB component indicates the instance)
#define RVC_SPN_MSB_APS                 ( 0x84 )
#define RVC_SPN_MSB_OPS                 ( 0x85 )
#define RVC_SPN_MSB_DC_BUS              ( 0x86 )
#define RVC_SPN_LSB_VOLTAGE             (    0 )
#define RVC_SPN_LSB_CURRENT             (    1 )
#define RVC_SPN_LSB_TEMP                (    2 )

//! SPN components for solar charge controllers as listed in Table 6.45.24a in RV-C Standard
//! (ISB component indicates the instance)
#define RVC_SPN_MSB_SCC_DC_VOLTAGE      ( 1 )
#define RVC_SPN_LSB_SCC_DC_VOLTAGE      ( 0 )
#define RVC_SPN_MSB_SCC_DC_CURRENT      ( 1 )
#define RVC_SPN_LSB_SCC_DC_CURRENT      ( 1 )
#define RVC_SPN_MSB_SCC_BATT_TEMP       ( 1 )
#define RVC_SPN_LSB_SCC_BATT_TEMP       ( 2 )
#define RVC_SPN_MSB_SCC_DC_SRC_SOC      ( 1 )
#define RVC_SPN_LSB_SCC_DC_SRC_SOC      ( 3 )
#define RVC_SPN_MSB_SCC_DC_SRC_SOH      ( 1 )
#define RVC_SPN_LSB_SCC_DC_SRC_SOH      ( 4 )
#define RVC_SPN_MSB_SCC_DC_SRC_CAP      ( 1 )
#define RVC_SPN_LSB_SCC_DC_SRC_CAP      ( 5 )
#define RVC_SPN_MSB_SCC_AC_RIPPLE       ( 1 )
#define RVC_SPN_LSB_SCC_AC_RIPPLE       ( 6 )
#define RVC_SPN_MSB_SCC_REV_POL         ( 1 )
#define RVC_SPN_LSB_SCC_REV_POL         ( 7 )
#define RVC_SPN_MSB_SCC_AMB_TEMP        ( 2 )
#define RVC_SPN_LSB_SCC_AMB_TEMP        ( 0 )
#define RVC_SPN_MSB_SCC_BATT_EQLZ       ( 2 )
#define RVC_SPN_LSB_SCC_BATT_EQLZ       ( 1 )
#define RVC_SPN_MSB_SCC_ARRAY_VOLTAGE   ( 2 )
#define RVC_SPN_LSB_SCC_ARRAY_VOLTAGE   ( 2 )
#define RVC_SPN_MSB_SCC_ARRAY_CURRENT   ( 2 )
#define RVC_SPN_LSB_SCC_ARRAY_CURRENT   ( 3 )

//! SPN components for generator starters as listed in Table 6.35.9 in RV-C Standard
//! (ISB component indicates the instance)

#define RVC_SPN_MSB_AGS_INVALID_CFG     ( 1 )
#define RVC_SPN_LSB_AGS_INVALID_CFG     ( 0 )
#define RVC_SPN_MSB_AGS_DATA_NOT_AVAIL  ( 1 )
#define RVC_SPN_LSB_AGS_DATA_NOT_AVAIL  ( 1 )

#endif // RVC_SPN_DEFS_H


//*****************************************************************************
// End of file.
//*****************************************************************************
