/*=============================================================================
Copyright 2004 Xantrex Technology Inc.  All rights reserved.

This source file is proprietary to Xantrex Technology Inc. and protected by
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:
    init.c

PURPOSE:
    Initialization file for TI 28x DSP
     - INITIALIZES  DSP system registers, including the WATCHDOG timer
                    Initial IO port configuration
                    Unmask and disable interrupts
                    Initialize the Event Managers


    The main purpose of this file is to initilaize the DSP system registers
    to operate with the NOVA/SolarWave memory configuration and to bring the
    input and output pins to safe startup condition that will not damage the
    NOVA/SolarWave on startup.


FUNCTION(S):
    INIT_fnSysInit - public DSP initialization function
    INIT_fnInitIOConfig( void )
    INIT_fnEnableInterrupts( void ) - Public Interrupt Enable Function

    local:

    init_fnInitSystemControlRegs( void )
    init_fnInitPIECtrl( void )
    init_fnInitADC( void )
    init_fnInitEVA( void )
    init_fnInitEVB( void )

NOTES:
    The statements #ifndef CTRL_LOADER_INIT
                   #endif
    encasing all of the functions with the exception of INIT_fnInitIOConfig()
    are so that the loader can use only the INIT_fnInitIOConfig() function
    without the need to maintain a branch of this file

    Loader needs to define CTRL_LOADER_INIT


    Control configurations must be defined in the project


CHANGE HISTORY:
$History: init.c $
    
    *****************  Version 6  *****************
    User: Horstm       Date: 12/03/04   Time: 2:47p
    Updated in $/PD/Common/Targets/TI/c28x/HAL/Src
    changes for timer driver port
    
    *****************  Version 5  *****************
    User: Brentt       Date: 6/22/04    Time: 2:37p
    Updated in $/PD/Common/Targets/TI/c28x/HAL/Test/Init
    - changed DSP281x_Device.h to Device.h
    - added devicemacro.h to includes for support of usDelay
    - fixed some alignment issues from converting all tabs to spaces
    - moved most of the #defines into F28x.h
    - Peripheral clocks in init_fnInitSystemControlRegs are defaulted to
    off now for power saving

    *****************  Version 4  *****************
    User: Johnb        Date: 6/16/04    Time: 3:38p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/C28x/Packages/HAL/Src
    Changed CTRL_CONFIG_OPTION_0 to CTRL_CONFIG_OPTION_6 according to
    latest porting discoveries.

    *****************  Version 3  *****************
    User: Brentt       Date: 6/09/04    Time: 5:05p
    Updated in $/PD/Common/Targets/TI/c28x/HAL/Test/Init
    - Converted all tabs to 4 spaces - as per the coding standard
    - Changed ADC_US_DELAY_1 to 10000 (from 7000) to reflect the typical
    time required for powering up the band gap and reference instead of the
    optimal time

    *****************  Version 2  *****************
    User: Brentt       Date: 6/09/04    Time: 10:21a
    Updated in $/PD/Common/Targets/TI/c28x/HAL/Test/Init
    Changes as per buddy check results:
    - fixed a function naming problem init_fnInitGPIOConfig changed to
    INIT_fnInitIOConfig to remain consistent with other packages.
    - fixed some commenting throughout
    - Added a couple macros to do some bound checking on the PLL divisor

    *****************  Version 1  *****************
    User: Brentt       Date: 6/08/04    Time: 2:32p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Test/Init
    Created - First Check-in

=============================================================================*/


/*=============================================================================
                              Includes
=============================================================================*/
#include "Device.h"         // TI device headers
#include "devicemacro.h"    // Public Macros
#include "init.h"           // Public access
#include "config.h"         // Configuration for loader

/*==============================================================================
                              Defines
==============================================================================*/

//-----------------------------------------------------------------------------
// Defines for IO pin configuration
//-----------------------------------------------------------------------------

// Individual pins can be initialized to one of 4 states:
// 1.  Secondary function (General Purpose I/O) Input
// 2.  Secondary function (General Purpose I/O) Output low
// 3.  Secondary function (General Purpose I/O) Output high
// 4   Primary function (As defined by respective peripheral)

// Assing each pin as one of the 4 states. The program will configure the MUX
// control register, the data direction register, and in the case of an output
// the state of the pin (high or low)

//The following 5 defines must not be changed and are used for configuration
#define PIN_INPUT       ( 0x0000 )    // Bit pattern for pin as input
#define PIN_OUTPUT_LOW  ( 0x8000 )    // Bit pattern for pin as low output
#define PIN_OUTPUT_HIGH ( 0x8080 )    // Bit pattern for pin as high output
#define PIN_PRIMARY     ( 0x0001 )    // Bit pattern for pin as primary function
#define FUNC_MSK        ( 0x0001 )    // Mask used for configuration

//
// Location Explanation
//
// 0x#000 - Direction of PIN ( 8 - output; 0 - input )
// 0x0#00 - Unused
// 0x00#0 - Data Value of PIN ( 8 - high; 0 - low )
// 0x000# - Function of pin  ( 1 - primary; 0 - I/O )

//Assign the pin functions to Ports A through F
//Examples

//#define PORTA_PIN0_INIT  PIN_OUTPUT_LOW
// - sets PORTA pin0 as an output and sets the pin to a low value

//#define PORTA_PIN0_INIT  PIN_OUTPUT_HIGH
// - sets PORTA pin0 as an output and sets the pin to a high value

//#define PORTA_PIN0_INIT  PIN_INPUT
// - sets PORTA pin0 as an input

//#define PORTA_PIN0_INIT  PIN_PRIMARY
// - sets PORTA pin0 to the primary function

// Compilation is dependenpent on specifying a preprocessor definition
// for the control configuration
// Each control configuratin is specific to a target hardware

// If INIT_UNIT_TEST is not defined then CTRL_CONFIG options are used
// If INIT_UNIT_TEST is defined then CTRL_CONFIG_UNIT_TEST options are used
#ifndef INIT_UNIT_TEST

//*****************************************************************************
// Hardware dependent configurations

// Configuration for eZdsp
#if defined( CTRL_CONFIG_OPTION_6 )

//^^^^^^^^^^^^^^^^^^^^^^^^^
// eZdsp IO Configuration
//^^^^^^^^^^^^^^^^^^^^^^^^^

//Port A assignments
#define PORTA_PIN0_INIT       PIN_PRIMARY
#define PORTA_PIN1_INIT       PIN_PRIMARY
#define PORTA_PIN2_INIT       PIN_PRIMARY
#define PORTA_PIN3_INIT       PIN_INPUT
#define PORTA_PIN4_INIT       PIN_OUTPUT_HIGH
#define PORTA_PIN5_INIT       PIN_OUTPUT_LOW
#define PORTA_PIN6_INIT       PIN_INPUT
#define PORTA_PIN7_INIT       PIN_INPUT
#define PORTA_PIN8_INIT       PIN_INPUT
#define PORTA_PIN9_INIT       PIN_PRIMARY
#define PORTA_PIN10_INIT      PIN_PRIMARY
#define PORTA_PIN11_INIT      PIN_PRIMARY
#define PORTA_PIN12_INIT      PIN_PRIMARY
#define PORTA_PIN13_INIT      PIN_INPUT
#define PORTA_PIN14_INIT      PIN_OUTPUT_HIGH
#define PORTA_PIN15_INIT      PIN_OUTPUT_LOW

//Port B assignments
#define PORTB_PIN0_INIT       PIN_INPUT
#define PORTB_PIN1_INIT       PIN_INPUT
#define PORTB_PIN2_INIT       PIN_INPUT
#define PORTB_PIN3_INIT       PIN_INPUT
#define PORTB_PIN4_INIT       PIN_INPUT
#define PORTB_PIN5_INIT       PIN_INPUT
#define PORTB_PIN6_INIT       PIN_INPUT
#define PORTB_PIN7_INIT       PIN_INPUT
#define PORTB_PIN8_INIT       PIN_INPUT
#define PORTB_PIN9_INIT       PIN_INPUT
#define PORTB_PIN10_INIT      PIN_INPUT
#define PORTB_PIN11_INIT      PIN_INPUT
#define PORTB_PIN12_INIT      PIN_INPUT
#define PORTB_PIN13_INIT      PIN_INPUT
#define PORTB_PIN14_INIT      PIN_INPUT
#define PORTB_PIN15_INIT      PIN_INPUT

//Port D assignments
#define PORTD_PIN0_INIT       PIN_INPUT
#define PORTD_PIN1_INIT       PIN_INPUT
#define PORTD_PIN5_INIT       PIN_INPUT
#define PORTD_PIN6_INIT       PIN_INPUT

//Port E assignments
#define PORTE_PIN0_INIT       PIN_INPUT
#define PORTE_PIN1_INIT       PIN_INPUT
#define PORTE_PIN2_INIT       PIN_INPUT

//Port F assignments
#define PORTF_PIN0_INIT       PIN_INPUT
#define PORTF_PIN1_INIT       PIN_INPUT
#define PORTF_PIN2_INIT       PIN_INPUT
#define PORTF_PIN3_INIT       PIN_INPUT
#define PORTF_PIN4_INIT       PIN_INPUT
#define PORTF_PIN5_INIT       PIN_INPUT
#define PORTF_PIN6_INIT       PIN_INPUT
#define PORTF_PIN7_INIT       PIN_INPUT
#define PORTF_PIN8_INIT       PIN_INPUT
#define PORTF_PIN9_INIT       PIN_INPUT
#define PORTF_PIN10_INIT      PIN_INPUT
#define PORTF_PIN11_INIT      PIN_INPUT
#define PORTF_PIN12_INIT      PIN_INPUT
#define PORTF_PIN13_INIT      PIN_INPUT
#define PORTF_PIN14_INIT      PIN_INPUT

// Port G assignments
#define PORTG_PIN4_INIT       PIN_INPUT
#define PORTG_PIN5_INIT       PIN_INPUT

#else

#error    No Valid Control Configuration Defined

#endif
// End of defines for IO pin configuration (eZdsp)
//-----------------------------------------------------------------------------

#else // INIT_UNIT_TEST defined

//*****************************************************************************
//Unit test configurations

#if defined( CTRL_CONFIG_UNIT_TEST_0 )

//Port A assignments
#define PORTA_PIN0_INIT      PIN_OUTPUT_HIGH
#define PORTA_PIN1_INIT      PIN_OUTPUT_LOW
#define PORTA_PIN2_INIT      PIN_OUTPUT_HIGH
#define PORTA_PIN3_INIT      PIN_OUTPUT_LOW
#define PORTA_PIN4_INIT      PIN_OUTPUT_HIGH
#define PORTA_PIN5_INIT      PIN_OUTPUT_LOW
#define PORTA_PIN6_INIT      PIN_OUTPUT_HIGH
#define PORTA_PIN7_INIT      PIN_OUTPUT_LOW
#define PORTA_PIN8_INIT      PIN_OUTPUT_HIGH
#define PORTA_PIN9_INIT      PIN_OUTPUT_LOW
#define PORTA_PIN10_INIT     PIN_OUTPUT_HIGH
#define PORTA_PIN11_INIT     PIN_OUTPUT_LOW
#define PORTA_PIN12_INIT     PIN_OUTPUT_HIGH
#define PORTA_PIN13_INIT     PIN_OUTPUT_LOW
#define PORTA_PIN14_INIT     PIN_OUTPUT_HIGH
#define PORTA_PIN15_INIT     PIN_OUTPUT_LOW

//Port B assignments
#define PORTB_PIN0_INIT      PIN_OUTPUT_HIGH
#define PORTB_PIN1_INIT      PIN_OUTPUT_LOW
#define PORTB_PIN2_INIT      PIN_OUTPUT_HIGH
#define PORTB_PIN3_INIT      PIN_OUTPUT_LOW
#define PORTB_PIN4_INIT      PIN_OUTPUT_HIGH
#define PORTB_PIN5_INIT      PIN_OUTPUT_LOW
#define PORTB_PIN6_INIT      PIN_OUTPUT_HIGH
#define PORTB_PIN7_INIT      PIN_OUTPUT_LOW
#define PORTB_PIN8_INIT      PIN_OUTPUT_HIGH
#define PORTB_PIN9_INIT      PIN_OUTPUT_LOW
#define PORTB_PIN10_INIT     PIN_OUTPUT_HIGH
#define PORTB_PIN11_INIT     PIN_OUTPUT_LOW
#define PORTB_PIN12_INIT     PIN_OUTPUT_HIGH
#define PORTB_PIN13_INIT     PIN_OUTPUT_LOW
#define PORTB_PIN14_INIT     PIN_OUTPUT_HIGH
#define PORTB_PIN15_INIT     PIN_OUTPUT_LOW

//Port D assignments
#define PORTD_PIN0_INIT      PIN_OUTPUT_HIGH
#define PORTD_PIN1_INIT      PIN_OUTPUT_LOW
#define PORTD_PIN5_INIT      PIN_OUTPUT_HIGH
#define PORTD_PIN6_INIT      PIN_OUTPUT_LOW

//Port E assignments
#define PORTE_PIN0_INIT      PIN_OUTPUT_HIGH
#define PORTE_PIN1_INIT      PIN_OUTPUT_LOW
#define PORTE_PIN2_INIT      PIN_OUTPUT_HIGH

//Port F assignments
#define PORTF_PIN0_INIT      PIN_OUTPUT_HIGH
#define PORTF_PIN1_INIT      PIN_OUTPUT_LOW
#define PORTF_PIN2_INIT      PIN_OUTPUT_HIGH
#define PORTF_PIN3_INIT      PIN_OUTPUT_LOW
#define PORTF_PIN4_INIT      PIN_OUTPUT_HIGH
#define PORTF_PIN5_INIT      PIN_OUTPUT_LOW
#define PORTF_PIN6_INIT      PIN_OUTPUT_HIGH
#define PORTF_PIN7_INIT      PIN_OUTPUT_LOW
#define PORTF_PIN8_INIT      PIN_OUTPUT_HIGH
#define PORTF_PIN9_INIT      PIN_OUTPUT_LOW
#define PORTF_PIN10_INIT     PIN_OUTPUT_HIGH
#define PORTF_PIN11_INIT     PIN_OUTPUT_LOW
#define PORTF_PIN12_INIT     PIN_OUTPUT_HIGH
#define PORTF_PIN13_INIT     PIN_OUTPUT_LOW
#define PORTF_PIN14_INIT     PIN_OUTPUT_HIGH

// Port G assignments
#define PORTG_PIN4_INIT      PIN_OUTPUT_HIGH
#define PORTG_PIN5_INIT      PIN_OUTPUT_LOW

#elif defined( CTRL_CONFIG_UNIT_TEST_1 )

//Port A assignments
#define PORTA_PIN0_INIT      PIN_OUTPUT_LOW
#define PORTA_PIN1_INIT      PIN_OUTPUT_HIGH
#define PORTA_PIN2_INIT      PIN_OUTPUT_LOW
#define PORTA_PIN3_INIT      PIN_OUTPUT_HIGH
#define PORTA_PIN4_INIT      PIN_OUTPUT_LOW
#define PORTA_PIN5_INIT      PIN_OUTPUT_HIGH
#define PORTA_PIN6_INIT      PIN_OUTPUT_LOW
#define PORTA_PIN7_INIT      PIN_OUTPUT_HIGH
#define PORTA_PIN8_INIT      PIN_OUTPUT_LOW
#define PORTA_PIN9_INIT      PIN_OUTPUT_HIGH
#define PORTA_PIN10_INIT     PIN_OUTPUT_LOW
#define PORTA_PIN11_INIT     PIN_OUTPUT_HIGH
#define PORTA_PIN12_INIT     PIN_OUTPUT_LOW
#define PORTA_PIN13_INIT     PIN_OUTPUT_HIGH
#define PORTA_PIN14_INIT     PIN_OUTPUT_LOW
#define PORTA_PIN15_INIT     PIN_OUTPUT_HIGH

//Port B assignments
#define PORTB_PIN0_INIT      PIN_OUTPUT_LOW
#define PORTB_PIN1_INIT      PIN_OUTPUT_HIGH
#define PORTB_PIN2_INIT      PIN_OUTPUT_LOW
#define PORTB_PIN3_INIT      PIN_OUTPUT_HIGH
#define PORTB_PIN4_INIT      PIN_OUTPUT_LOW
#define PORTB_PIN5_INIT      PIN_OUTPUT_HIGH
#define PORTB_PIN6_INIT      PIN_OUTPUT_LOW
#define PORTB_PIN7_INIT      PIN_OUTPUT_HIGH
#define PORTB_PIN8_INIT      PIN_OUTPUT_LOW
#define PORTB_PIN9_INIT      PIN_OUTPUT_HIGH
#define PORTB_PIN10_INIT     PIN_OUTPUT_LOW
#define PORTB_PIN11_INIT     PIN_OUTPUT_HIGH
#define PORTB_PIN12_INIT     PIN_OUTPUT_LOW
#define PORTB_PIN13_INIT     PIN_OUTPUT_HIGH
#define PORTB_PIN14_INIT     PIN_OUTPUT_LOW
#define PORTB_PIN15_INIT     PIN_OUTPUT_HIGH

//Port D assignments
#define PORTD_PIN0_INIT      PIN_OUTPUT_LOW
#define PORTD_PIN1_INIT      PIN_OUTPUT_HIGH
#define PORTD_PIN5_INIT      PIN_OUTPUT_LOW
#define PORTD_PIN6_INIT      PIN_OUTPUT_HIGH

//Port E assignments
#define PORTE_PIN0_INIT      PIN_OUTPUT_LOW
#define PORTE_PIN1_INIT      PIN_OUTPUT_HIGH
#define PORTE_PIN2_INIT      PIN_OUTPUT_LOW

//Port F assignments
#define PORTF_PIN0_INIT      PIN_OUTPUT_LOW
#define PORTF_PIN1_INIT      PIN_OUTPUT_HIGH
#define PORTF_PIN2_INIT      PIN_OUTPUT_LOW
#define PORTF_PIN3_INIT      PIN_OUTPUT_HIGH
#define PORTF_PIN4_INIT      PIN_OUTPUT_LOW
#define PORTF_PIN5_INIT      PIN_OUTPUT_HIGH
#define PORTF_PIN6_INIT      PIN_OUTPUT_LOW
#define PORTF_PIN7_INIT      PIN_OUTPUT_HIGH
#define PORTF_PIN8_INIT      PIN_OUTPUT_LOW
#define PORTF_PIN9_INIT      PIN_OUTPUT_HIGH
#define PORTF_PIN10_INIT     PIN_OUTPUT_LOW
#define PORTF_PIN11_INIT     PIN_OUTPUT_HIGH
#define PORTF_PIN12_INIT     PIN_OUTPUT_LOW
#define PORTF_PIN13_INIT     PIN_OUTPUT_HIGH
#define PORTF_PIN14_INIT     PIN_OUTPUT_LOW

// Port G assignments
#define PORTG_PIN4_INIT      PIN_OUTPUT_LOW
#define PORTG_PIN5_INIT      PIN_OUTPUT_HIGH

#elif defined( CTRL_CONFIG_UNIT_TEST_2 )

//Port A assignments
#define PORTA_PIN0_INIT      PIN_INPUT
#define PORTA_PIN1_INIT      PIN_INPUT
#define PORTA_PIN2_INIT      PIN_INPUT
#define PORTA_PIN3_INIT      PIN_INPUT
#define PORTA_PIN4_INIT      PIN_INPUT
#define PORTA_PIN5_INIT      PIN_INPUT
#define PORTA_PIN6_INIT      PIN_INPUT
#define PORTA_PIN7_INIT      PIN_INPUT
#define PORTA_PIN8_INIT      PIN_INPUT
#define PORTA_PIN9_INIT      PIN_INPUT
#define PORTA_PIN10_INIT     PIN_INPUT
#define PORTA_PIN11_INIT     PIN_INPUT
#define PORTA_PIN12_INIT     PIN_INPUT
#define PORTA_PIN13_INIT     PIN_INPUT
#define PORTA_PIN14_INIT     PIN_INPUT
#define PORTA_PIN15_INIT     PIN_INPUT

//Port B assignments
#define PORTB_PIN0_INIT      PIN_INPUT
#define PORTB_PIN1_INIT      PIN_INPUT
#define PORTB_PIN2_INIT      PIN_INPUT
#define PORTB_PIN3_INIT      PIN_INPUT
#define PORTB_PIN4_INIT      PIN_INPUT
#define PORTB_PIN5_INIT      PIN_INPUT
#define PORTB_PIN6_INIT      PIN_INPUT
#define PORTB_PIN7_INIT      PIN_INPUT
#define PORTB_PIN8_INIT      PIN_INPUT
#define PORTB_PIN9_INIT      PIN_INPUT
#define PORTB_PIN10_INIT     PIN_INPUT
#define PORTB_PIN11_INIT     PIN_INPUT
#define PORTB_PIN12_INIT     PIN_INPUT
#define PORTB_PIN13_INIT     PIN_INPUT
#define PORTB_PIN14_INIT     PIN_INPUT
#define PORTB_PIN15_INIT     PIN_INPUT

//Port D assignments
#define PORTD_PIN0_INIT      PIN_INPUT
#define PORTD_PIN1_INIT      PIN_INPUT
#define PORTD_PIN5_INIT      PIN_INPUT
#define PORTD_PIN6_INIT      PIN_INPUT

//Port E assignments
#define PORTE_PIN0_INIT      PIN_INPUT
#define PORTE_PIN1_INIT      PIN_INPUT
#define PORTE_PIN2_INIT      PIN_INPUT

//Port F assignments
#define PORTF_PIN0_INIT      PIN_INPUT
#define PORTF_PIN1_INIT      PIN_INPUT
#define PORTF_PIN2_INIT      PIN_INPUT
#define PORTF_PIN3_INIT      PIN_INPUT
#define PORTF_PIN4_INIT      PIN_INPUT
#define PORTF_PIN5_INIT      PIN_INPUT
#define PORTF_PIN6_INIT      PIN_INPUT
#define PORTF_PIN7_INIT      PIN_INPUT
#define PORTF_PIN8_INIT      PIN_INPUT
#define PORTF_PIN9_INIT      PIN_INPUT
#define PORTF_PIN10_INIT     PIN_INPUT
#define PORTF_PIN11_INIT     PIN_INPUT
#define PORTF_PIN12_INIT     PIN_INPUT
#define PORTF_PIN13_INIT     PIN_INPUT
#define PORTF_PIN14_INIT     PIN_INPUT

// Port G assignments
#define PORTG_PIN4_INIT      PIN_INPUT
#define PORTG_PIN5_INIT      PIN_INPUT

#else // #if defined( CTRL_CONFIG_UNIT_TEST_0 )

#error    No Unit Test Configuration Chosen

#endif // #if defined( CTRL_CONFIG_UNIT_TEST_0 )
//End of unit test configurations
//*****************************************************************************

#endif // ifndef( INIT_UNIT_TEST )


#ifndef INIT_UNIT_TEST

//*****************************************************************************
//Operating configuration

//Function enables
//IO configuration function enable
#define INIT_IO_CFG_EN      ( TRUE )

// PIE Control Registers Initialization
#define INIT_PIE_EN         ( TRUE )

//Analog converter initialization function enable
#define INIT_ADC_EN         ( TRUE )

//Event manager A initiaiization enable
#define INIT_EVA_EN         ( TRUE )

//Event manager B initiaiization enable
#define INIT_EVB_EN         ( TRUE )

// Enable Interrupts
#define INIT_INT_EN         ( FALSE )

//End of operating configuration
//*****************************************************************************

#else // ifndef INIT_UNIT_TEST

//*****************************************************************************
// Unit test configuration

// Function enables
// IO configuration function enable
#define INIT_IO_CFG_EN      ( TRUE )

// PIE Control Registers Initialization
#define INIT_PIE_EN         ( TRUE )

// Analog converter initialization function enable
#define INIT_ADC_EN         ( TRUE )

// Event manager A initiaiization enable
#define INIT_EVA_EN         ( TRUE )

// Event manager B initiaiization enable
#define INIT_EVB_EN         ( TRUE )

// Enable Interrupts
#define INIT_INT_EN         ( FALSE )

//End of unit test configuration
//*****************************************************************************

#endif // ifndef INIT_UNIT_TEST

// Multiply the Maximum speed desired by 2 and then divide by the crystal speed
// to get the desire divider for the PLL.
#define mCALC_PLL_DIV( MAX_SPD, XTAL_SPD )  ( ( MAX_SPD << 1 ) / XTAL_SPD )
// The actual PLL divider value to write to the register
#define PLLCR_PLL_DIV                       mCALC_PLL_DIV( CTRL_CPU_MAXSPEED, \
                                                           CTRL_CONFIG_CRYSTAL )
// macro to GET the PLL divider is within valid limits
#define mGET_PLL_DIV                        ( ( PLLCR_PLL_DIV <= 6 ) ?      \
                                            ( ( PLLCR_PLL_DIV >= 0 ) ?      \
                                                PLLCR_PLL_DIV : 0 ) : 0 )

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
                           Local/Private Constants
==============================================================================*/


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
void init_fnInitSystemControlRegs( void );
void INIT_fnInitIOConfig( void );
void init_fnInitEVA( void );
void init_fnInitEVB( void );
void init_fnInitADC( void );
void init_fnInitPIECtrl( void );
void INIT_fnEnableInterrupts( void );

/*==============================================================================
                           Function Definitions
==============================================================================*/

/******************************************************************************

FUNCTION NAME:
    INIT_fnSysInit

PURPOSE:
    Initialization of C28x hardware

INPUTS:
    None

OUTPUTS:
    None

NOTES:


VERSION HISTORY:

Version: 1.00   Date:   June 7, 2004    By: Brent Tokarchuk
    - Created

******************************************************************************/
#ifndef CTRL_LOADER_INIT  //Loader specific conditional compile

void INIT_fnSysInit( void )
{
    // Configure DSP sys control registers
    init_fnInitSystemControlRegs();

    // Configure the IO pins
#if ( INIT_IO_CFG_EN == TRUE )
    INIT_fnInitIOConfig();
#endif

    // Initialize PIE Controller
#if ( INIT_PIE_EN == TRUE )
    init_fnInitPIECtrl();
#endif

    // Initialize analog converter
#if ( INIT_ADC_EN == TRUE )
   init_fnInitADC();
#endif

    // Initialize event manager A
#if ( INIT_EVA_EN == TRUE )
    init_fnInitEVA();
#endif

    // Initialize event manager B
#if ( INIT_EVB_EN == TRUE )
    init_fnInitEVB();
#endif

    // Enable Interrupts
#if ( INIT_INT_EN == TRUE )
    INIT_fnEnableInterrupts();
#endif

} // INIT_fnSysInit

#endif  //#ifndef CTRL_LOADER_INIT

/******************************************************************************

FUNCTION NAME:
    init_fnInitSystemControlRegs

PURPOSE:
    This function sets up the system control registers. These registers
    include all listed on pg. 3-3 of TMS320F28x DSP System Control
    Interrupt Reference Guide Rev A:

    HISPCP        - High-Speed Peripheral Clock Prescalar Register for HSPCLK
    LOSPCP        - Low-Speed Prescalar Register for HSPCLK clock
    PCLKCR        - Peripheral Clock Control Register
    LPMCR0        - Low Power Mode Control Register 0
    LPMCR1        - Low Power Mode Control Register 1
    PLLCR        - PLL Control Register
    SCSR        - System Control and Status Register
    WDCNTR        - WatchDog Counter Register
    WDKEY        - WatchDog Reset Key Register
    WDCR        - WatchDog Control Register

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version: 1.00  Date: May 18, 2004  By: Brent Tokarchuk
    - Created

Version: 1.01  Date: June 18, 2004 By: Brent Tokarchuk
    -   Updated #define names to reflect moving #defines into external file
    -   Changed the default state of the peripheral clocks to OFF for power
        saving.

******************************************************************************/
void init_fnInitSystemControlRegs( void )
{

    uint16 i;    // loop counter

    // enable write access to protected registers
    EALLOW;

    // Configure the System Control and Status (SCSR) Register
    //
    // must configure using ORed bit files since the WDOVERRIDE bit can be toggled
    // by a Read-Modify-Write sequence since if the system read out a 1 from that bit
    // and wrote it back, the process of writing a 1 back into that bit can clear it
    // and thus prevent access to the WDDIS register inadvertently.
    SysCtrlRegs.SCSR = F28X_mSCSR_INIT;

    // Configure the Peripheral Clock Control (PCLKCR) Register
    // 
    // All of the clocks are disabled for power saving reasons, therefore it
    // is the responsibility of the individual drivers to enable the
    // clocls that they need to use.
    SysCtrlRegs.PCLKCR.bit.ECANENCLK  = F28X_PCLKCR_CLK_OFF;    // eCAN clock is disabled
    SysCtrlRegs.PCLKCR.bit.MCBSPENCLK = F28X_PCLKCR_CLK_OFF;    // McBSP clock is disabled
    SysCtrlRegs.PCLKCR.bit.SCIBENCLK  = F28X_PCLKCR_CLK_OFF;    // SCI-B clock is disabled
    SysCtrlRegs.PCLKCR.bit.SCIAENCLK  = F28X_PCLKCR_CLK_OFF;    // SCI-A clock is disabled
    SysCtrlRegs.PCLKCR.bit.SPIENCLK   = F28X_PCLKCR_CLK_OFF;    // SPI clock is disabled
    SysCtrlRegs.PCLKCR.bit.ADCENCLK   = F28X_PCLKCR_CLK_OFF;    // ADC clock is disabled
    SysCtrlRegs.PCLKCR.bit.EVBENCLK   = F28X_PCLKCR_CLK_OFF;    // EV-B clock is disabled
    SysCtrlRegs.PCLKCR.bit.EVAENCLK   = F28X_PCLKCR_CLK_OFF;    // EV-A clock is disabled

    // Configure Watchdog Control (WDCR) Register
    //
    // must configure using ORed bit fields since no structure exists for this register
    SysCtrlRegs.WDCR = F28X_mWDCR_INIT;

    // Configure the High-speed peripheral clock
    SysCtrlRegs.HISPCP.bit.HSPCLK = F28X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_1;
    // Configure the Low-speed peripheral clock
    SysCtrlRegs.LOSPCP.bit.LSPCLK = F28X_XXSPCP_XXPCLK_SYSCLKOUT_DIV_1;

    // Configure the PLL system clock divider
    SysCtrlRegs.PLLCR.bit.DIV = mGET_PLL_DIV;

    // Wait for PLL to lock
    //
    // This value chosen from an example used by TI
    for( i = 0; i < 5000; i++ )
    {
    }

    // set up the Low Power Mode for the device.  This setting is only valid when
    // the IDLE instruction is executed.
    SysCtrlRegs.LPMCR0.bit.LPM = F28X_LPMCR0_LPM_LPM_IDLE;
    // Ensure the interrupts that will wake the device from standby have all been cleared
    SysCtrlRegs.LPMCR1.all = F28X_LPMCR1_NO_STANDBY_INTS;

    // disable write access to the protected registers.
    EDIS;

} // init_fnInitSystemControlRegs( void )

/******************************************************************************

FUNCTION NAME:
    INIT_fnInitIOConfig

PURPOSE:
    Initialization of C28x GPIO pins.  This function sets up the I/O
    based on which configuration is chosen from the #define statements
    above.  Each pin on the c28x can be set to be either a General
    Purpose I/O pin or the it can be configured to behave as required
    by the internal peripheral it is connected to.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    The input function of the I/O and the input path to the peripheral
    are always enabled, it is the output path of the peripheral that is
    MUXed with the output of the I/O.  Therefore when a pin is configured
    as an I/O pin, the corresponding peripheral function MUST be disabled.
    Otherwise, interrupts may be inadvertently triggered.


VERSION HISTORY:

Version: 1.00  Date: May-25-2004    By: Brent Tokarchuk
    - Created

Version: 1.01  Date: June 18, 2004  By: Brent Tokarchuk
    -   Updated #define names to reflect moving #defines into external file

******************************************************************************/
#ifndef CTRL_LOADER_INIT //Loader specific conditional compile
#if( INIT_IO_CFG_EN == TRUE )

void INIT_fnInitIOConfig( void )
{

    // enable write access to protected registers
    EALLOW;

    //
    // Setup PORT A
    //
    // Select primary or secondary function in MUX Control Register
    GpioMuxRegs.GPAMUX.bit.PWM1_GPIOA0       = PORTA_PIN0_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.PWM2_GPIOA1       = PORTA_PIN1_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.PWM3_GPIOA2       = PORTA_PIN2_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.PWM4_GPIOA3       = PORTA_PIN3_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.PWM5_GPIOA4       = PORTA_PIN4_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.PWM6_GPIOA5       = PORTA_PIN5_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.T1PWM_GPIOA6      = PORTA_PIN6_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.T2PWM_GPIOA7      = PORTA_PIN7_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8     = PORTA_PIN8_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9     = PORTA_PIN9_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10   = PORTA_PIN10_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.TDIRA_GPIOA11     = PORTA_PIN11_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.TCLKINA_GPIOA12   = PORTA_PIN12_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.C1TRIP_GPIOA13    = PORTA_PIN13_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.C2TRIP_GPIOA14    = PORTA_PIN14_INIT & FUNC_MSK;
    GpioMuxRegs.GPAMUX.bit.C3TRIP_GPIOA15    = PORTA_PIN15_INIT & FUNC_MSK;

    // Select data direction of pin (input/output)
    GpioMuxRegs.GPADIR.bit.GPIOA0            = (PORTA_PIN0_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA1            = (PORTA_PIN1_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA2            = (PORTA_PIN2_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA3            = (PORTA_PIN3_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA4            = (PORTA_PIN4_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA5            = (PORTA_PIN5_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA6            = (PORTA_PIN6_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA7            = (PORTA_PIN7_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA8            = (PORTA_PIN8_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA9            = (PORTA_PIN9_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA10           = (PORTA_PIN10_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA11           = (PORTA_PIN11_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA12           = (PORTA_PIN12_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA13           = (PORTA_PIN13_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA14           = (PORTA_PIN14_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPADIR.bit.GPIOA15           = (PORTA_PIN15_INIT >> XT_BITPOS15) & FUNC_MSK;

    // Select the value of the pin (1/0)
    GpioDataRegs.GPADAT.bit.GPIOA0           = (PORTA_PIN0_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA1           = (PORTA_PIN1_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA2           = (PORTA_PIN2_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA3           = (PORTA_PIN3_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA4           = (PORTA_PIN4_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA5           = (PORTA_PIN5_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA6           = (PORTA_PIN6_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA7           = (PORTA_PIN7_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA8           = (PORTA_PIN8_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA9           = (PORTA_PIN9_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA10          = (PORTA_PIN10_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA11          = (PORTA_PIN11_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA12          = (PORTA_PIN12_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA13          = (PORTA_PIN13_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA14          = (PORTA_PIN14_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPADAT.bit.GPIOA15          = (PORTA_PIN15_INIT >> XT_BITPOS7) & FUNC_MSK;

    //
    // Setup Port B
    //
    // Select primary or secondary function MUX Control Register
    GpioMuxRegs.GPBMUX.bit.PWM7_GPIOB0       = PORTB_PIN0_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.PWM8_GPIOB1       = PORTB_PIN1_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.PWM9_GPIOB2       = PORTB_PIN2_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.PWM10_GPIOB3      = PORTB_PIN3_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.PWM11_GPIOB4      = PORTB_PIN4_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.PWM12_GPIOB5      = PORTB_PIN5_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.T3PWM_GPIOB6      = PORTB_PIN6_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.T4PWM_GPIOB7      = PORTB_PIN7_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.CAP4Q1_GPIOB8     = PORTB_PIN8_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.CAP5Q2_GPIOB9     = PORTB_PIN9_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.CAP6QI2_GPIOB10   = PORTB_PIN10_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.TDIRB_GPIOB11     = PORTB_PIN11_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.TCLKINB_GPIOB12   = PORTB_PIN12_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.C4TRIP_GPIOB13    = PORTB_PIN13_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.C5TRIP_GPIOB14    = PORTB_PIN14_INIT & FUNC_MSK;
    GpioMuxRegs.GPBMUX.bit.C6TRIP_GPIOB15    = PORTB_PIN15_INIT & FUNC_MSK;

    // Select data direction of pin (input/output)
    GpioMuxRegs.GPBDIR.bit.GPIOB0            = (PORTB_PIN0_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB1            = (PORTB_PIN1_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB2            = (PORTB_PIN2_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB3            = (PORTB_PIN3_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB4            = (PORTB_PIN4_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB5            = (PORTB_PIN5_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB6            = (PORTB_PIN6_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB7            = (PORTB_PIN7_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB8            = (PORTB_PIN8_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB9            = (PORTB_PIN9_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB10           = (PORTB_PIN10_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB11           = (PORTB_PIN11_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB12           = (PORTB_PIN12_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB13           = (PORTB_PIN13_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB14           = (PORTB_PIN14_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPBDIR.bit.GPIOB15           = (PORTB_PIN15_INIT >> XT_BITPOS15) & FUNC_MSK;

    // Select the value of the pin (1/0)
    GpioDataRegs.GPBDAT.bit.GPIOB0           = (PORTB_PIN0_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB1           = (PORTB_PIN1_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB2           = (PORTB_PIN2_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB3           = (PORTB_PIN3_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB4           = (PORTB_PIN4_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB5           = (PORTB_PIN5_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB6           = (PORTB_PIN6_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB7           = (PORTB_PIN7_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB8           = (PORTB_PIN8_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB9           = (PORTB_PIN9_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB10          = (PORTB_PIN10_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB11          = (PORTB_PIN11_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB12          = (PORTB_PIN12_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB13          = (PORTB_PIN13_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB14          = (PORTB_PIN14_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPBDAT.bit.GPIOB15          = (PORTB_PIN15_INIT >> XT_BITPOS7) & FUNC_MSK;

    //
    // Setup PORT D
    //
    // Select primary or secondary function MUX Control Register
    GpioMuxRegs.GPDMUX.bit.T1CTRIP_PDPA_GPIOD0 = PORTD_PIN0_INIT & FUNC_MSK;
    GpioMuxRegs.GPDMUX.bit.T2CTRIP_SOCA_GPIOD1 = PORTD_PIN1_INIT & FUNC_MSK;
    GpioMuxRegs.GPDMUX.bit.T3CTRIP_PDPB_GPIOD5 = PORTD_PIN5_INIT & FUNC_MSK;
    GpioMuxRegs.GPDMUX.bit.T4CTRIP_SOCB_GPIOD6 = PORTD_PIN6_INIT & FUNC_MSK;

    // Select data direction of pin (input/output)
    GpioMuxRegs.GPDDIR.bit.GPIOD0            = (PORTD_PIN0_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPDDIR.bit.GPIOD1            = (PORTD_PIN1_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPDDIR.bit.GPIOD5            = (PORTD_PIN5_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPDDIR.bit.GPIOD6            = (PORTD_PIN6_INIT >> XT_BITPOS15) & FUNC_MSK;

    // Select the value of the pin (1/0)
    GpioDataRegs.GPDDAT.bit.GPIOD0           = (PORTD_PIN0_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPDDAT.bit.GPIOD1           = (PORTD_PIN1_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPDDAT.bit.GPIOD5           = (PORTD_PIN5_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPDDAT.bit.GPIOD6           = (PORTD_PIN6_INIT >> XT_BITPOS7) & FUNC_MSK;

    //
    // Setup PORT E
    //
    // Select primary or secondary function MUX Control Register
    GpioMuxRegs.GPEMUX.bit.XINT1_XBIO_GPIOE0   = PORTE_PIN0_INIT & FUNC_MSK;
    GpioMuxRegs.GPEMUX.bit.XINT2_ADCSOC_GPIOE1 = PORTE_PIN1_INIT & FUNC_MSK;
    GpioMuxRegs.GPEMUX.bit.XNMI_XINT13_GPIOE2  = PORTE_PIN2_INIT & FUNC_MSK;

    // Select data direction of pin (input/output)
    GpioMuxRegs.GPEDIR.bit.GPIOE0            = (PORTE_PIN0_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPEDIR.bit.GPIOE1            = (PORTE_PIN1_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPEDIR.bit.GPIOE2            = (PORTE_PIN2_INIT >> XT_BITPOS15) & FUNC_MSK;

    // Select the value of the pin (1/0)
    GpioDataRegs.GPEDAT.bit.GPIOE0           = (PORTE_PIN0_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPEDAT.bit.GPIOE1           = (PORTE_PIN1_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPEDAT.bit.GPIOE2           = (PORTE_PIN2_INIT >> XT_BITPOS7) & FUNC_MSK;

    //
    // Setup PORT F
    //
    // Select primary or secondary function MUX Control Register
    GpioMuxRegs.GPFMUX.bit.SPISIMOA_GPIOF0   = PORTF_PIN0_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.SPISOMIA_GPIOF1   = PORTF_PIN1_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.SPICLKA_GPIOF2    = PORTF_PIN2_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.SPISTEA_GPIOF3    = PORTF_PIN3_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.SCITXDA_GPIOF4    = PORTF_PIN4_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.SCIRXDA_GPIOF5    = PORTF_PIN5_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.CANTXA_GPIOF6     = PORTF_PIN6_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.CANRXA_GPIOF7     = PORTF_PIN7_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.MCLKXA_GPIOF8     = PORTF_PIN8_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.MCLKRA_GPIOF9     = PORTF_PIN9_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.MFSXA_GPIOF10     = PORTF_PIN10_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.MFSRA_GPIOF11     = PORTF_PIN11_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.MDXA_GPIOF12      = PORTF_PIN12_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.MDRA_GPIOF13      = PORTF_PIN13_INIT & FUNC_MSK;
    GpioMuxRegs.GPFMUX.bit.XF_GPIOF14        = PORTF_PIN14_INIT & FUNC_MSK;

    // Select data direction of pin (input/output)
    GpioMuxRegs.GPFDIR.bit.GPIOF0            = (PORTF_PIN0_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF1            = (PORTF_PIN1_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF2            = (PORTF_PIN2_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF3            = (PORTF_PIN3_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF4            = (PORTF_PIN4_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF5            = (PORTF_PIN5_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF6            = (PORTF_PIN6_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF7            = (PORTF_PIN7_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF8            = (PORTF_PIN8_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF9            = (PORTF_PIN9_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF10           = (PORTF_PIN10_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF11           = (PORTF_PIN11_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF12           = (PORTF_PIN12_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF13           = (PORTF_PIN13_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPFDIR.bit.GPIOF14           = (PORTF_PIN14_INIT >> XT_BITPOS15) & FUNC_MSK;

    // Select the value of the pin (1/0)
    GpioDataRegs.GPFDAT.bit.GPIOF0           = (PORTF_PIN0_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF1           = (PORTF_PIN1_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF2           = (PORTF_PIN2_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF3           = (PORTF_PIN3_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF4           = (PORTF_PIN4_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF5           = (PORTF_PIN5_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF6           = (PORTF_PIN6_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF7           = (PORTF_PIN7_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF8           = (PORTF_PIN8_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF9           = (PORTF_PIN9_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF10          = (PORTF_PIN10_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF11          = (PORTF_PIN11_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF12          = (PORTF_PIN12_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF13          = (PORTF_PIN13_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPFDAT.bit.GPIOF14          = (PORTF_PIN14_INIT >> XT_BITPOS7) & FUNC_MSK;

    //
    // Setup PORT G
    //
    // Select primary or secondary function MUX Control Register
    GpioMuxRegs.GPGMUX.bit.SCITXDB_GPIOG4    = PORTG_PIN4_INIT & FUNC_MSK;
    GpioMuxRegs.GPGMUX.bit.SCIRXDB_GPIOG5    = PORTG_PIN5_INIT & FUNC_MSK;

    // Select data direction of pin (input/output)
    GpioMuxRegs.GPGDIR.bit.GPIOG4            = (PORTG_PIN4_INIT >> XT_BITPOS15) & FUNC_MSK;
    GpioMuxRegs.GPGDIR.bit.GPIOG5            = (PORTG_PIN5_INIT >> XT_BITPOS15) & FUNC_MSK;

    // Select the value of the pin (1/0)
    GpioDataRegs.GPGDAT.bit.GPIOG4           = (PORTG_PIN4_INIT >> XT_BITPOS7) & FUNC_MSK;
    GpioDataRegs.GPGDAT.bit.GPIOG5           = (PORTG_PIN5_INIT >> XT_BITPOS7) & FUNC_MSK;

    // disable write access to the protected registers.
    EDIS;

} // INIT_fnInitIOConfig

#endif//End #if( INIT_IO_CFG_EN == TRUE )
#endif //#ifndef CTRL_LOADER_INIT

/*****************************************************************************\

FUNCTION NAME:
    init_fnInitEVA

PURPOSE:
    Event Manager A Initialization for the TMS320LF28x DSP.  The main goal
    of this routine is to intialize any register in the event manager with
    "control" in its name to a know quiet state. A list of these registers
    can be found on page 1-12 of the TMS320F28x DSP Event Manager (EV)
    Reference Guide Rev A.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    Some of the registers do not need to have all bits explicitly set. For
    example, the TxCON registers control the operation of individual timers
    and if their operation is disabled, then that is enough for the purposes
    of the init package since it is assumed that other drivers that wish to
    use the timers will set up the registers properly upon creation.

VERSION HISTORY:

Version: 1.00  Date: May 20, 2004  By: Brent Tokarchuk
    - Created

Version: 1.01  Date: June 18, 2004 By: Brent Tokarchuk
    -   Updated #define names to reflect moving #defines into external file

\*****************************************************************************/
#ifndef CTRL_LOADER_INIT //Loader specific conditional compile
#if( INIT_EVA_EN == TRUE )

void init_fnInitEVA( void )
{
    // Setup Event Manager A

    // General purpose timer control register A.
    EvaRegs.GPTCONA.bit.T2CTRIPE = F28X_GPTCONX_TXCTRIPE_DIS;
    EvaRegs.GPTCONA.bit.T1CTRIPE = F28X_GPTCONX_TXCTRIPE_DIS;
    EvaRegs.GPTCONA.bit.T2TOADC  = F28X_GPTCONX_TXTOADC_DIS;
    EvaRegs.GPTCONA.bit.T1TOADC  = F28X_GPTCONX_TXTOADC_DIS;
    EvaRegs.GPTCONA.bit.TCMPOE   = F28X_GPTCONX_TCMPOE_HI_Z;
    EvaRegs.GPTCONA.bit.T2CMPOE  = F28X_GPTCONX_TCMPOE_HI_Z;
    EvaRegs.GPTCONA.bit.T1CMPOE  = F28X_GPTCONX_TCMPOE_HI_Z;
    EvaRegs.GPTCONA.bit.T2PIN    = F28X_GPTCONX_TCMPOE_FORCE_LOW;
    EvaRegs.GPTCONA.bit.T1PIN    = F28X_GPTCONX_TCMPOE_FORCE_LOW;

    // Timer 1 Control Register - (looking for the other bitfields? see NOTES:)
    EvaRegs.T1CON.bit.TENABLE = F28X_TXCON_TENABLE_FALSE;

    // Timer 2 Control Register - (looking for the other bitfields? see NOTES:)
    EvaRegs.T2CON.bit.TENABLE = F28X_TXCON_TENABLE_FALSE;

    // Compare Control Register A
    //
    // Disabling compare operation makes all shadowed registers
    // (CMPRx, ACTRx) transparent.  This means that with this functionality
    // disabled, making changes in those "shadowed" registers has no impact
    // whatsoever on the system and we therefore do not need to bother
    // initializing them.
    EvaRegs.COMCONA.bit.CENABLE = F28X_COMCONX_CENABLE_FALSE;

    // Dead-Band timer Control Register A - (looking for the other bitfields? see NOTES:)
    //
    // Disable the dead band timers for timer 3, 2 and 1.  Disabling these
    // affects PWM pins 1,2,3,4,5 and 6
    EvaRegs.DBTCONA.bit.EDBT1 = F28X_DBTCONX_DBT_DIS;
    EvaRegs.DBTCONA.bit.EDBT2 = F28X_DBTCONX_DBT_DIS;
    EvaRegs.DBTCONA.bit.EDBT3 = F28X_DBTCONX_DBT_DIS;

    // Capture Unit Registers - (looking for the other bitfields? see NOTES:)
    EvaRegs.CAPCONA.bit.CAP12EN = F28X_CAPCONX_CAP_1_2_DIS;
    EvaRegs.CAPCONA.bit.CAP3EN  = F28X_CAPCONX_CAP_3_DIS;

    // Extension Control Register
    EvaRegs.EXTCONA.bit.EVSOCE = F28X_EXTCONA_ADC_SOC_DIS;
    EvaRegs.EXTCONA.bit.INDCOE = F28X_EXTCONA_CMP_IND_DIS;

} // init_fnInitEVA

#endif//End #if( INIT_EVA_EN == TRUE )
#endif //#ifndef CTRL_LOADER_INIT
/*****************************************************************************\

FUNCTION NAME:
    init_fnInitEVB

PURPOSE:
    Event Manager B Initialization for the TMS320LF28x DSP.  The main goal
    of this routine is to intialize any register in the event manager with
    "control" in its name to a know quiet state. A list of these registers
    can be found on page 1-12 of the TMS320F28x DSP Event Manager (EV)
    Reference Guide Rev A.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    Some of the registers do not need to have all bits explicitly set. For
    example, the TxCON registers control the operation of individual timers
    and if their operation is disabled, then that is enough for the purposes
    of the init package since it is assumed that other drivers that wish to
    use the timers will set up the registers properly upon creation.

    In addition, in some places only one bit is initialized.  This is usually
    because the one bit totally disables all functionality of that "module"
    and thus configuring the rest of the bits is a waste of time.

VERSION HISTORY:

Version: 1.00  Date: May 20, 2004  By: Brent Tokarchuk
    - Created

Version: 1.01  Date: June 18, 2004 By: Brent Tokarchuk
    -   Updated #define names to reflect moving #defines into external file

\*****************************************************************************/
#ifndef CTRL_LOADER_INIT //Loader specific conditional compile
#if( INIT_EVB_EN == TRUE )

void init_fnInitEVB( void )
{

    //Setup Event Manager B

    // General purpose timer control register B.
    EvbRegs.GPTCONB.bit.T4CTRIPE = F28X_GPTCONX_TXCTRIPE_DIS;
    EvbRegs.GPTCONB.bit.T3CTRIPE = F28X_GPTCONX_TXCTRIPE_DIS;
    EvbRegs.GPTCONB.bit.T4TOADC  = F28X_GPTCONX_TXTOADC_DIS;
    EvbRegs.GPTCONB.bit.T3TOADC  = F28X_GPTCONX_TXTOADC_DIS;
    EvbRegs.GPTCONB.bit.TCMPOE   = F28X_GPTCONX_TCMPOE_HI_Z;
    EvbRegs.GPTCONB.bit.T4CMPOE  = F28X_GPTCONX_TCMPOE_HI_Z;
    EvbRegs.GPTCONB.bit.T3CMPOE  = F28X_GPTCONX_TCMPOE_HI_Z;
    EvbRegs.GPTCONB.bit.T4PIN    = F28X_GPTCONX_TCMPOE_FORCE_LOW;
    EvbRegs.GPTCONB.bit.T3PIN    = F28X_GPTCONX_TCMPOE_FORCE_LOW;

    // Timer 3 Control Register - (looking for the other bitfields? see NOTES:)
    EvbRegs.T3CON.bit.TENABLE = F28X_TXCON_TENABLE_FALSE;

    // Timer 4 Control Register - (looking for the other bitfields? see NOTES:)
    EvbRegs.T4CON.bit.TENABLE = F28X_TXCON_TENABLE_FALSE;

    // Compare Control Register
    //
    // Disabling compare operation makes all shadowed registers
    // (CMPRx, ACTRx) transparent.  This means that with this functionality
    // disabled, making changes in those "shadowed" registers has no impact
    // whatsoever on the system and we therefore do not need to bother
    // initializing them.
    EvbRegs.COMCONB.bit.CENABLE = F28X_COMCONX_CENABLE_FALSE;

    // Dead-Band timer Control Register
    //
    // Disable the dead band timers for timer 3, 2 and 1.  Disabling these
    // affects PWM pins 7,8,9,10,11 and 12
    EvbRegs.DBTCONB.bit.EDBT1 = F28X_DBTCONX_DBT_DIS;
    EvbRegs.DBTCONB.bit.EDBT2 = F28X_DBTCONX_DBT_DIS;
    EvbRegs.DBTCONB.bit.EDBT3 = F28X_DBTCONX_DBT_DIS;

    // Capture Unit Registers - (looking for the other bitfields? see NOTES:)
    EvbRegs.CAPCONB.bit.CAP45EN = F28X_CAPCONX_CAP_1_2_DIS;
    EvbRegs.CAPCONB.bit.CAP6EN  = F28X_CAPCONX_CAP_3_DIS;

    // Extension Control Register - (looking for the other bitfields? see NOTES:)
    EvbRegs.EXTCONB.bit.EVSOCE = F28X_EXTCONA_ADC_SOC_DIS;
    EvbRegs.EXTCONB.bit.INDCOE = F28X_EXTCONA_CMP_IND_DIS;

} // init_fnInitEVB

#endif // INIT_EVB_EN == TRUE
#endif //#ifndef CTRL_LOADER_INIT

/*****************************************************************************\

FUNCTION NAME:
    init_fnInitPIECtrl

PURPOSE:
    Peripheral Interrupt Expansion initialization for the C28x DSP

INPUTS:
    None

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version: 1.0  Date: May 20, 2004  By: Brent Tokarchuk
    - Created

Version: 1.01 Date: June 18, 2004 By: Brent Tokarchuk
    -   Updated #define names to reflect moving #defines into external file

\*****************************************************************************/
#ifndef CTRL_LOADER_INIT // Loader specific conditional compile
#if( INIT_PIE_EN == TRUE )

void init_fnInitPIECtrl( void )
{

    // enable write access to protected registers
    EALLOW;

    // Disable PIE:
    PieCtrlRegs.PIECRTL.bit.ENPIE = F28X_PIECTRL_ENPIE_FALSE;

    // Disable all Peripheral Interrupts by clearing all PIEIER registers:
    PieCtrlRegs.PIEIER1.all  = F28X_PIEIERX_INT_DIS;
    PieCtrlRegs.PIEIER2.all  = F28X_PIEIERX_INT_DIS;
    PieCtrlRegs.PIEIER3.all  = F28X_PIEIERX_INT_DIS;
    PieCtrlRegs.PIEIER4.all  = F28X_PIEIERX_INT_DIS;
    PieCtrlRegs.PIEIER5.all  = F28X_PIEIERX_INT_DIS;
    PieCtrlRegs.PIEIER6.all  = F28X_PIEIERX_INT_DIS;
    PieCtrlRegs.PIEIER7.all  = F28X_PIEIERX_INT_DIS;
    PieCtrlRegs.PIEIER8.all  = F28X_PIEIERX_INT_DIS;
    PieCtrlRegs.PIEIER9.all  = F28X_PIEIERX_INT_DIS;
    PieCtrlRegs.PIEIER10.all = F28X_PIEIERX_INT_DIS;
    PieCtrlRegs.PIEIER11.all = F28X_PIEIERX_INT_DIS;
    PieCtrlRegs.PIEIER12.all = F28X_PIEIERX_INT_DIS;

    // Clear all flagged interrupts by clearing PIEIFR registers:
    PieCtrlRegs.PIEIFR1.all  = F28X_PIEIFRX_INT_FLG_CLR;
    PieCtrlRegs.PIEIFR2.all  = F28X_PIEIFRX_INT_FLG_CLR;
    PieCtrlRegs.PIEIFR3.all  = F28X_PIEIFRX_INT_FLG_CLR;
    PieCtrlRegs.PIEIFR4.all  = F28X_PIEIFRX_INT_FLG_CLR;
    PieCtrlRegs.PIEIFR5.all  = F28X_PIEIFRX_INT_FLG_CLR;
    PieCtrlRegs.PIEIFR6.all  = F28X_PIEIFRX_INT_FLG_CLR;
    PieCtrlRegs.PIEIFR7.all  = F28X_PIEIFRX_INT_FLG_CLR;
    PieCtrlRegs.PIEIFR8.all  = F28X_PIEIFRX_INT_FLG_CLR;
    PieCtrlRegs.PIEIFR9.all  = F28X_PIEIFRX_INT_FLG_CLR;
    PieCtrlRegs.PIEIFR10.all = F28X_PIEIFRX_INT_FLG_CLR;
    PieCtrlRegs.PIEIFR11.all = F28X_PIEIFRX_INT_FLG_CLR;
    PieCtrlRegs.PIEIFR12.all = F28X_PIEIFRX_INT_FLG_CLR;

    // PIE all cleared, move on to CPU registers
    IFR = F28X_IFR_CLR_ALL_FLG;
    IER = F28X_IER_INT_ALL_DIS;

#if( ( CTRL_CONFIG_CPU == CTRL_CPU_TI_TMX320F2812A ) )
    // Disable External Interrupts on devices that support it
    XIntruptRegs.XINT1CR.bit.ENABLE = F28X_XINTXCR_INT_DIS;
    XIntruptRegs.XINT2CR.bit.ENABLE = F28X_XINTXCR_INT_DIS;
    XIntruptRegs.XNMICR.bit.ENABLE  = F28X_XINTXCR_INT_DIS;
#endif  // #if 2812

    // disable write access to the protected registers.
    EDIS;

} // fnInitPIECtrl

#endif // End #if( INIT_PIE_EN == TRUE )
#endif // #ifndef CTRL_LOADER_INIT

/*****************************************************************************\

FUNCTION NAME:
    init_fnInitADC

PURPOSE:
    Analog Converter initialization for the TMS320LF28x DSP

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    The ADC resets into a good quiet state so all we want to do is power it
    up so that it is ready for use when the driver calls it.

    A strict power up sequence must be followed ( pg 1-25: TMS320F28x DSP
    Analog-to-Digital Converter (ADC) Reference Guide Rev A.)

VERSION HISTORY:

Version 1.00    Date: May 27, 2004     By: Brent Tokarchuk
    -   Created

Version: 1.01   Date: June 18, 2004    By: Brent Tokarchuk
    -   Updated #define names to reflect moving #defines into external file
    -   Moved the initialization from here to the ADC driver Ctor since
        the Ctor resets the ADC anyways and wipes out anything we did here.

\*****************************************************************************/
#ifndef CTRL_LOADER_INIT //Loader specific conditional compile
#if( INIT_ADC_EN == TRUE )

void init_fnInitADC( void )
{
    // reset the ADC into it's default off state.
    AdcRegs.ADCTRL1.bit.RESET = F28X_ADCTRL1_RESET_ALL;

} // init_fnInitADC

#endif // INIT_ADC == TRUE
#endif //#ifndef CTRL_LOADER_INIT

/*****************************************************************************\

FUNCTION NAME:
    INIT_fnEnableInterrupts

PURPOSE:
    Enable the interrupts on the C28x chip and set the appropriate registers.

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    None

VERSION HISTORY:

Version 1.00    Date: May 23, 2004     By: Brent Tokarchuk
    -   Created

Version: 1.01   Date: June 18, 2004    By: Brent Tokarchuk
    -   Updated #define names to reflect moving #defines into external file

\*****************************************************************************/
#ifndef CTRL_LOADER_INIT //Loader specific conditional compile
#if( INIT_INT_EN == TRUE )

void INIT_fnEnableInterrupts( void )
{

    // Enable the PIE
    PieCtrlRegs.PIECRTL.bit.ENPIE = F28X_PIECTRL_ENPIE_TRUE;

    // Enables PIE to drive a pulse into the CPU
    PieCtrlRegs.PIEACK.all = 0xFFFF;

    // Enable Interrupts at the CPU level
    EINT;

} // INIT_fnEnableInterrupts

#endif // INIT_INT_EN == TRUE
#endif //#ifndef CTRL_LOADER_INIT
