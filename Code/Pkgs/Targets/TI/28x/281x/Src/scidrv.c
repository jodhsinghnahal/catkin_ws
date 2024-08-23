/*===========================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by
copyright. The reproduction, in whole or in part, by anyone without the
written approval of Xantrex is prohibited.

FILE NAME:
    scidrv.c

PURPOSE:
   This file contains the driver code for the Serial Communications Interface
   for the F28x series of DSP from Texas Instruments. The driver utilizes
   interrupt driven transmission and reception of serial data with F28x's enhanced
   FIFO features.

   Usage:
   1.  Initialize the RTOS with the OSInit( ) instruction from the RTOS

   2.  Call the SCI constructor SCIDRV_fnCtor specifying baud rate, number
       of stop bits, number of data bits, and parity.

   3.  Ensure that the global interrupts are enabled.

   The constructor will set up the driver to operate with the interrupts. No
   other configuration is required.

FUNCTION(S):
Public:
    SCIDRV_fnCtor            - Initialize SCI port
    SCIDRV_fnDtor            - Quit using the serial port
    SCIDRV_fnEnTransmitter   - Enable the SCI transmitter
    SCIDRV_fnDisTransmitter  - Disable the SCI transmitter
    SCIDRV_fnEnReceiver      - Enable the SCI receiver
    SCIDRV_fnDisReceiver     - Disable the SCI receiver
    SCIDRV_fnSendBreak       - Send a break on the serial port
    SCIDRV_fnSendByte        - Send a byte to the serial port
    SCIDRV_fnReceiveByte     - Receive a serial byte and the status of the byte
    SCIDRV_fnSetTimeout      - Set the timeout for reception of a serial byte
    SCIDRV_fnGetTimeout      - Retrieve SCI timeout value
    SCIDRV_fnTxIsr  - Transmission ISR
    SCIDRV_fnRxIsr  - Receiving ISR

    Private:
        scidrv_fnProcessTx     - Transmission processing
        scidrv_fnStoreRxChar   - Store the received character
        scidrv_fnResetSci      - Reset the SCI module

NOTES:
   F2812 has two SCI ports. Only one port can be enabled at a time in this
   driver. The desired por can be defined via the micro definitions SCIA_ENABLE
   and SCIB_ENABLE.


CHANGE HISTORY:
$Log: scidrv.c $

    ***********************************************
    Revision: NovaPfrmB_JohnB/2
    User: JohnB     Date: 03/22/05  Time: 12:55AM
    Update ISR setup.  Each driver now writes to the Pie vector table, the
    address of the ISR is cares about.

    Updated Main to call the default ISR copying.

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/2
    User: HorstM     Date: 02/22/05  Time: 11:29PM
    renamed isr's; removed assignment of interrupt vectors to PIE vector table




    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/1
    User: HorstM     Date: 02/18/05  Time: 01:08AM
    fixed port enable flag (define), and corrected named constants in dtor


    ***********************************************
    Revision: NovaPfrmB_HorstM/3
    User: HorstM     Date: 01/18/05  Time: 11:43PM
    re-included sysclk.h; package re-instated


    ***********************************************
    Revision: NovaPfrmB_HorstM/2
    User: HorstM     Date: 01/13/05  Time: 05:31PM
    merged


    ***********************************************
    Revision: NovaPfrmB_JohnB/1
    User: JohnB     Date: 01/11/05  Time: 09:25PM
    Removed include of sysclk.h
    Revision: NovaPfrmB_HorstM/1
    User: HorstM     Date: 01/11/05  Time: 05:35PM
    corrected SCI port A pin names in constructor


    *****************  Version 1  *****************
    User: Hollyz       Date: 11/18/04   Time: 1:26p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Src
    Created

==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "scidrv.h"       // Header file for scidrv.c
#include "sciclient.h"    // Header file for sciclient.c
#include "device.h"       // Header file for target
#include "devicemacro.h"  // Header file for device macros
#include "xassert.h"      // Header file for assert
#include "SysClk.h"       // clock speed functions

/*==============================================================================
                            Type Definitions
==============================================================================*/
// None

/*==============================================================================
                                Enums
==============================================================================*/
/*==============================================================================
                               ENUMS
==============================================================================*/
typedef enum eSCI_DRIVER_INSTANCE
{
    eSCI_INVL_DRIVER = 0,
    eSCI_DRIVER_NOT_INSTANTIATED,
    eSCI_DRIVER_INSTANTIATED
} teSCI_DRIVER_INSTANCE;


/*==============================================================================
                              Defines
==============================================================================*/

// SCIB is the default enabled SCI port
#define SCIA_ENABLE           ( FALSE )
#define SCIB_ENABLE           ( FALSE )

// Register of spcific SCI port
#if ( SCIA_ENABLE == TRUE )
#define SciRegs      SciaRegs
#else
#define SciRegs      ScibRegs
#endif

// Masks for baud rate configuration
#define BAUD_HIGH             ( 8 )
#define BAUD_MSK              ( 0x00FF )

// Receive buffer register mask
#define RX_DATA_MSK           ( 0x00FF )

// enums are 16-bit ints, so can't pass values greater than 65535, therefore
// define baud rate multiplier to convert the enum into the actual baud rate
#define BAUD_CORR_FACTOR      ( 100L )

// Baud rate register formula constants
// Obtained from TI SCI manual
#define BRR_MUL               ( 8L )
#define BRR_SUB               ( 1 )

// Define baud rate divisor multiplier for break generation
#define BREAK_MULTIPLIER      ( 2 )

// Define for SCI null value
#define SCI_NULL              ( 0 )

// Define BREAK_PROCESSING as TRUE to incorporate break singal processing
// routine
#define BREAK_PROCESSING      ( TRUE )
#define POL                   ( 1 )

// No Delay between every transfer from FIFO transmit buffer to transmit
// shift register
#define SCIDRV_FFTX_DELAY     ( 0 )

// Function compile enabled
#define FN_ENABLE_TX          ( TRUE )
#define FN_DISABLE_TX         ( TRUE )
#define FN_ENABLE_RX          ( TRUE )
#define FN_DISABLE_RX         ( TRUE )
#define FN_SEND_BREAK         ( TRUE )

// Define for NOVA Communicator
#define EXTERNAL_COMMUNICATOR_EN  ( FALSE )

/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                           Local/Private Constants
==============================================================================*/


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

// Timeout, initiailze to timeout OFF condition
static uint16 RxTimeOut = 0;

//Set the driver instantiation to not instantiated
static uchar8 SciDriverInstance = eSCI_DRIVER_NOT_INSTANTIATED;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
// SCI transmission processing
static void scidrv_fnProcessTx( void );

// Reset the SCI module
static void scidrv_fnResetSci( void );

// Retreive received charater and status
static void scidrv_fnStoreRxChar( uchar8 RxData,
                                  uchar8 RxStatus );
#if( SCIA_ENABLE == TRUE )
extern void ISR_fnSCIRXINTA( void );
extern void ISR_fnSCITXINTA( void );
#endif

#if( SCIB_ENABLE == TRUE )
extern void ISR_fnSCIRXINTB( void );
extern void ISR_fnSCITXINTB( void );
#endif

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnCtor

PURPOSE:
    Configure and initialize the SCI

INPUTS:
    Baud rate - SCIDRV_teBAUD_RATE BaudRate
    - Baud rate enums   SCIDRV_eBAUD_2400            2400 baud
                        SCIDRV_eBAUD_4800            4800 baud
                        SCIDRV_eBAUD_9600            9600 baud
                        SCIDRV_eBAUD_19200          19200 baud
                        SCIDRV_eBAUD_38400          38400 baud
                        SCIDRV_eBAUD_57600          57600 baud
                        SCIDRV_eBAUD_115200        115200 baud

    Parity - SCIDRV_tePARITY Parity
    - Parity enums      SCIDRV_ePARITY_NONE        no parity
                        SCIDRV_ePARITY_EVEN        even parity
                        SCIDRV_ePARITY_ODD         odd parity

    Data bits - SCIDRV_teDATA_BITS DataBits
    - Data bit enums    SCIDRV_eDATA_BITS_1        1 data bit
                        SCIDRV_eDATA_BITS_2        2 data bits
                        SCIDRV_eDATA_BITS_3        3 data bits
                        SCIDRV_eDATA_BITS_4        4 data bits
                        SCIDRV_eDATA_BITS_5        5 data bits
                        SCIDRV_eDATA_BITS_6        6 data bits
                        SCIDRV_eDATA_BITS_7        7 data bits
                        SCIDRV_eDATA_BITS_8        8 data bits

    Stop bits - SCIDRV_teSTOP_BITS StopBits
    - Stop bit enums    SCIDRV_eSTOP_BITS_1        1 stop bit
                        SCIDRV_eSTOP_BITS_2        2 stop bits


    Example call 115200 baud, N, 8, 1
    SCIDRV_fnCtor( SCIDRV_eBAUD_115200,
                   SCIDRV_ePARITY_NONE,
                   SCIDRV_eDATA_BITS_8,
                   SCIDRV_eSTOP_BITS_1 );

OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)

NOTES:
    The constructor will enable the transmit and receive FIFO interrupts
    to avoid frequent interrupts.

    No other SCI routines can be called prior to calling the constructor.

    The function SCICLIENT_fnCtor must be called to set up system specific services
    for the SCI driver

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP and added initialization
      for SCI FIFO registers.

Version: 1.10    Date: March 21, 2005  By: John Bellini
    - Added initialization of ISR to the vector table.

*******************************************************************************/
tuiSTATUS SCIDRV_fnCtor( SCIDRV_teBAUD_RATE BaudRate,
                         SCIDRV_tePARITY    Parity,
                         SCIDRV_teDATA_BITS DataBits,
                         SCIDRV_teSTOP_BITS StopBits )
{
    uint32 LspClkSpeed;  // Clock speed
    uint16 BaudReg;      // Baud rate register value

    // only run the Ctor if the SCI has not been instantiated
    if( SciDriverInstance == eSCI_DRIVER_NOT_INSTANTIATED )
    {
        // Enable access to protected registers
        DEVICE_mACCESS_EN();

        // Enable the SCI clk source before writing to any SCI registers
        // Enable the SCI module by setting the pins to their primary function
        // Set the interrupt vector for RXINT and TXINT
        // Enable Rx/Tx interrupts in PIE group 9 for desired SCI channel
        //
        #if ( SCIA_ENABLE == TRUE )
        SysCtrlRegs.PCLKCR.bit.SCIAENCLK  = F28X_PCLKCR_CLK_ON;    // SCI-A

        GpioMuxRegs.GPFMUX.bit.SCITXDB_GPIOG4 = 1;
        GpioMuxRegs.GPFMUX.bit.SCIRXDB_GPIOG5 = 1;

        PieVectTable.RXAINT = &ISR_fnSCIRXINTA;
        PieVectTable.TXAINT = &ISR_fnSCITXINTA;

        PieCtrlRegs.PIEIER9.bit.INTx1 = 1;           // PIE Group 9, INT1 for SCIARx
        PieCtrlRegs.PIEIER9.bit.INTx2 = 1;           // PIE Group 9, INT2 for SCIATx

        #elif( SCIB_ENABLE == TRUE )
        SysCtrlRegs.PCLKCR.bit.SCIBENCLK  = F28X_PCLKCR_CLK_ON;   // SCI-B

        GpioMuxRegs.GPGMUX.bit.SCITXDB_GPIOG4 = 1;
        GpioMuxRegs.GPGMUX.bit.SCIRXDB_GPIOG5 = 1;

        PieVectTable.RXBINT = &ISR_fnSCIRXINTB;
        PieVectTable.TXBINT = &ISR_fnSCITXINTB;

        PieCtrlRegs.PIEIER9.bit.INTx3 = 1;           // PIE Group 9, INT3 for SCIBRx
        PieCtrlRegs.PIEIER9.bit.INTx4 = 1;           // PIE Group 9, INT4 for SCIBTx
        #endif

        // Disable register access
        DEVICE_mACCESS_DIS();

        // Disable the SCI hardware prior to initialization of SCI control varaibles
        //
        // Disable SCI receiver
        SciRegs.SCICTL1.bit.RXENA = F28X_SCICTL1_RX_DIS;

        // Disable SCI transmitter
        SciRegs.SCICTL1.bit.TXENA = F28X_SCICTL1_TX_DIS;

        // Disable the RxErr interrupt
        SciRegs.SCICTL1.bit.RXERRINTENA = F28X_SCICTL1_RX_ERR_INT_DIS;

        // Disable the RxBk interrupt
        SciRegs.SCICTL2.bit.RXBKINTENA = F28X_SCICTL2_RXBK_INT_DIS;

        // Disable the Tx interrupt
        SciRegs.SCICTL2.bit.TXINTENA = F28X_SCICTL2_TXRDY_INT_DIS;

        // Reset the SCI module
        scidrv_fnResetSci();


        // Calculate and set the baud rate registers
        //
        // Get the frequency that the LSPCLK is running at
        LspClkSpeed = SYSCLK_fnGetLoSpeedPeriphClk();

        // Baud rate register formula from TI SCI manual
        BaudReg = ( uint16 )( LspClkSpeed / \
                  ( ( uint32 )BaudRate * BAUD_CORR_FACTOR * BRR_MUL ) ) - BRR_SUB;

        // Set the baud rate registers
        SciRegs.SCIHBAUD = ( BaudReg >> BAUD_HIGH ) & BAUD_MSK;
        SciRegs.SCILBAUD = BaudReg & BAUD_MSK;

        // Set the Parity
        //
        switch( Parity )
        {
            case SCIDRV_ePARITY_NONE:
                // Disable parity generation
                SciRegs.SCICCR.bit.PARITYENA = \
                    F28X_SCICCR_PARITY_ENA_DIS;
                break;

            case SCIDRV_ePARITY_ODD:
                // Enable parity generation
                SciRegs.SCICCR.bit.PARITYENA = \
                    F28X_SCICCR_PARITY_ENA_EN;

                // Set odd parity
                SciRegs.SCICCR.bit.PARITY = \
                    F28X_SCICCR_PARITY_ODD;
                break;

            case SCIDRV_ePARITY_EVEN:
                // Enable parity generation
                SciRegs.SCICCR.bit.PARITYENA = \
                    F28X_SCICCR_PARITY_ENA_EN;

                // Set even parity
                SciRegs.SCICCR.bit.PARITY = \
                    F28X_SCICCR_PARITY_EVEN;
                break;

            default:
                // Invalid parity
                assert( 0 );
                break;
        }

        // Set the number of stop bits
        //
        switch( StopBits )
        {
            case SCIDRV_eSTOP_BITS_1:
                // Set 1 stop bit
                SciRegs.SCICCR.bit.STOPBITS = F28X_SCICCR_STOP_BITS_ONE;
                break;

            case SCIDRV_eSTOP_BITS_2:
                // Set 2 stop bits
                SciRegs.SCICCR.bit.STOPBITS = F28X_SCICCR_STOP_BITS_TWO;
                break;

            default:
                // Invalid number of stop bits
                assert( 0 );
                break;
        }

        // Set the number of data bits
        //
        switch( DataBits )
        {
            case SCIDRV_eDATA_BITS_7:
                // Set for 7 data bits
                SciRegs.SCICCR.bit.SCICHAR = F28X_SCICCR_CHARLEN_7;
                break;

            case SCIDRV_eDATA_BITS_8:
                // Set for 8 data bits
                SciRegs.SCICCR.bit.SCICHAR = F28X_SCICCR_CHARLEN_8;
                break;

            default:
                // Invalid number of data bits has been sent to the function
                assert( 0 );
                break;
        }

        // Set up the SCI registers
        //
        // Set idle line communications mode
        SciRegs.SCICCR.bit.ADDRIDLE_MODE = F28X_SCICCR_MULTIPRO_MODE_IDLELINE;

        // Disable transmitter wake mode
        SciRegs.SCICTL1.bit.TXWAKE = F28X_SCICTL1_TX_WAKE_DIS;

        // Disable receiver sleep mode
        SciRegs.SCICTL1.bit.SLEEP = F28X_SCICTL1_SLEEP_DIS;

        // Set the emulation suspend mode to complete sequence before stopping
        SciRegs.SCIPRI.all |= F28X_SCIPRI_SUSMODE_SOFT << F28X_SCIPRI_SUSMODE_BITPOS;

        // Enable the receive/break interrupt
        SciRegs.SCICTL2.bit.RXBKINTENA = F28X_SCICTL2_RXBK_INT_EN;

        // Enable the Rx error interrupt
        SciRegs.SCICTL1.bit.RXERRINTENA = F28X_SCICTL1_RX_ERR_INT_EN;

        // Set up the SCI FIFO registers
        //
        // Resume SCI FIFO transimitter or receiver
        SciRegs.SCIFFTX.bit.SCIRST = F28X_SCIFFTX_TXRX_CHAN_RESUME;

        // Enable FIFI transmit and receive
        SciRegs.SCIFFTX.bit.SCIFFENA = F28X_SCIFFTX_TXRX_FIFO_EN;

        // Reset the Tx FIFO pointer and hold in reset
        SciRegs.SCIFFTX.bit.TXFIFOXRESET = F28X_SCIFFTX_TX_FIFO_PTR_RESET;

        // Trigger Tx interrupt when transmit FIFO is empty
        SciRegs.SCIFFTX.bit.TXFFILIL = F28X_SCIFFRX_RX_FIFO_INTLVL_0;

        // Enable Tx FIFO interrupt
        SciRegs.SCIFFTX.bit.TXFFIENA = F28X_SCIFFTX_TX_FIFO_INT_EN;

        // Re-enable transmit FIFO operation
        SciRegs.SCIFFTX.bit.TXFIFOXRESET = F28X_SCIFFTX_TX_FIFO_PTR_REEN;

        // Reset the Rx FIFO pointer and hold in reset
        SciRegs.SCIFFRX.bit.RXFIFORESET = F28X_SCIFFRX_RX_FIFO_PTR_RESET;

        // Trigger Rx interrupt when transmit FIFO is empty
        SciRegs.SCIFFRX.bit.RXFFIL = F28X_SCIFFRX_RX_FIFO_INTLVL_1;

        // Enable Rx FIFO interrupt
        SciRegs.SCIFFRX.bit.RXFFIENA = F28X_SCIFFRX_RX_FIFO_INT_EN;

        // Re-enable receive FIFO operation
        SciRegs.SCIFFRX.bit.RXFIFORESET = F28X_SCIFFRX_RX_FIFO_PTR_REEN;

        // No delay between every transfer from FIFO transmit buffer to
        // transmit shift register
        SciRegs.SCIFFCT.bit.FFTXDLY = SCIDRV_FFTX_DELAY;

        // Setup client services for the SCI
        //
        SCICLIENT_fnCtor();

        // Enable INT9 required for SCI
        //
        DEVICE_mINT9_EN();

        // Receiver is always enabled
        SCIDRV_fnEnReceiver();

        //Set driver as instantiated
        SciDriverInstance = eSCI_DRIVER_INSTANTIATED;
    }
    else
    {
        //Set status to error if driver already instantiated
        return( eSTATUS_ERR );
    }// End if( SciDriverInstance == eSCI_DRIVER_NOT_INSTANTIATED )

    // Return status to the calling routine
    return( eSTATUS_OK );

} // End SCIDRV_fnCtor


/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnDtor

PURPOSE:
    Destroy the instance of the SCI.

INPUTS:
    none

OUTPUTS:
    tuiSTATUS - Status code (either OK or an error code)

NOTES:
    This routine disables the SCI module and assocaited interrupts. If a
    semaphore is pending the SCI module will be disabled and an error condition
    will be indicated.

    No SCI routines can be executed after the destructor has been called.

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP.

Version: 1.10    Date: FEB-17-2005    By: Horst Maurer
    - fixed named constants from F28X.h

*******************************************************************************/
#ifdef GLBL_DTOR_EN

tuiSTATUS SCIDRV_fnDtor( void )
{

    if( SciDriverInstance == eSCI_DRIVER_INSTANTIATED )
    {

        // Disable the SCI module by setting the pins to their secondary function
#if ( SCIA_ENABLE == TRUE )
        GpioMuxRegs.GPFMUX.bit.SCITXDB_GPIOG4 = 0;
        GpioMuxRegs.GPFMUX.bit.SCIRXDB_GPIOG5 = 0;
#else
        GpioMuxRegs.GPGMUX.bit.SCITXDB_GPIOG4 = 0;
        GpioMuxRegs.GPGMUX.bit.SCIRXDB_GPIOG5 = 0;
#endif
        // Disable SCI receiver
        SciRegs.SCICTL1.bit.RXENA = F28X_SCICTL1_RX_DIS;

        // Disable the trasnmitter
        SciRegs.SCICTL1.bit.TXENA = F28X_SCICTL1_TX_DIS;

        // Disable the RxBk interrupt
        SciRegs.SCICTL2.bit.RXBKINTENA = F28X_SCICTL2_RXBK_INT_DIS;

        // Disable the RxErr interrupt
        SciRegs.SCICTL1.bit.RXERRINTENA = F28X_SCICTL1_RX_ERR_INT_DIS;

        // Disable Rx FIFO interrupt
        SciRegs.SCIFFRX.bit.RXFFIENA = F28X_SCIFFRX_RX_FIFO_INT_DIS;

        // Disable Tx FIFO interrupt
        SciRegs.SCIFFTX.bit.TXFFIENA = F28X_SCIFFTX_TX_FIFO_INT_DIS;

        // Reset the SCI module
        scidrv_fnResetSci();

        // Destroy client services for the SCI
        SCICLIENT_fnDtor();

        //Set the driver to not instantiated
        SciDriverInstance = eSCI_DRIVER_NOT_INSTANTIATED;

        //Set status to OK if drive successfully uninstantiated
        return( eSTATUS_OK );

    }
    else
    {

        //Set status to error if driver not already instantiated
       return( eSTATUS_ERR );

    }
}

#endif // #ifndef GLBL_DTOR_EN


/*******************************************************************************

FUNCTION NAME:
    scidrv_fnProcessTx

PURPOSE:
    SCI transmission processing

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    This function is called by SCI transmit interrupt routine CIDRV_fnSciTxInterrupt.
    It will dump the characters to the transmit buffer, up to 16 bytes at a time. If
    no more data is in the transmission queue, it will turn the transmitter off.

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the 2812 DSP and implemented
      transmit FIFO

*******************************************************************************/
static void scidrv_fnProcessTx( void )
{
    tucBOOL DataReady;   // Status of data
    uchar8 SerialData;   // Next character to sned
    uint16 i;

    // F28x's transmit buffer is supplemented with a 16 level FIFO
    for ( i = 0; i < F28X_SCIFFTX_TX_FIFO_INTLVL_16; i++ )
     {
        //Obtain the next character to send to the SCI
        SerialData = SCICLIENT_fnRetreiveTxData( &DataReady );

        if( DataReady == TRUE )
        {
            // If data was returned then send it out the port
            SciRegs.SCITXBUF = SerialData;
        }
        else
        {
            // Disable the transmitter and quit if no data is ready to send
            SCIDRV_fnDisTransmitter();
            break;
        }
     }
}


/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnTxIsr

PURPOSE:
    SCI transmission interrupt routine

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    When SCI FIFO is enabled, the TXINT of the standard SCI will be disabled
    and this interrupt will service as SCI transmit FIFO interrupt.

    The SCI transmit FIFO interrupt is enabled when the SCI is initialized, and
    the transmitter is enabled via calling function SCIDRV_fnSendByte while valid
    data is stored in the transmission queue. This will trigger the interrupt to
    happen whenever the FIFO status bits are less than or equel to the FIFO level
    bits.

    The SCI transmisson FIFO interrupt level is set to 0 to avoid frequent interruts.

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP and implemented
      transmit FIFO interrupt.

*******************************************************************************/
void SCIDRV_fnTxIsr( void )
{
    scidrv_fnProcessTx();                     // Transmission processing

    SciRegs.SCIFFTX.bit.TXINTCLR=1;           // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all |= IER_INT9_ACK;   // Issue PIE ACK
}


/*******************************************************************************

FUNCTION NAME:
    scidrv_fnStoreRxChar

PURPOSE:
    Store the status and data of the received characters.

INPUTS:
    uchar8 RxData    - the received character
    uchar8 RxStataus - Status of the received character
                    SCIDRV_eOK           - character received with no errors
                    SCIDRV_eBREAK        - break condition detected
                    SCIDRV_eERROR_FRAME  - character received with frame error
                    SCIDRV_eERROR_PO     - character received with parity or overrun error
OUTPUTS:
    None

NOTES:

    This module is called by SCI reveive interrupt routine.

    To determine the reception of a break character the following conditions will
    be used
    - a character has been received that generates a frame error condition
    - the value in the recieve register when a frame error occurs is 0

    When these two conditions are vaild then instead of an error condition, a
    break detect will be indicated

    After any error condition the SCI module must be reset to clear the error
    condition in order for the SCI to continue to receive characters. There is no
    other method to clear an error condition.

    This module only returns a signal break condition if a long break signal is
    sent to the SCI. When detecting using the frame error with a NULL condition,
    the TI SCI generates an error condition that requires a reset of the SCI. The
    tail of the break signal is recognized as a start bit and an extra character is
    generated. This character is ignored by this module.

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004   By: Holly Zhou
    - Copied from NOVA project, modified for the 2812 DSP and implemented SCI FIFO
      receive feature.

*******************************************************************************/
static void scidrv_fnStoreRxChar( uchar8 RxData,
                                  uchar8 RxStatus )
{
    static uchar8 SequentialBreaks = FALSE;

    switch( RxStatus )
    {
        // break condition
        case SCIDRV_eBREAK:

            SequentialBreaks = TRUE;

            // Store data and status
            SCICLIENT_fnStoreRxData( RxData,
                                     (uchar8)SCIDRV_eBREAK );
            break;

        // frame error condition
        case SCIDRV_eERROR_FRAME:

            //Test if the character received is null. This will be a break
            //condtion

            if ( RxData == SCI_NULL )
            {
                //Frame error with received character NULL
                //To reach this section of code send a break signal


                //If the received break signal is sequential after
                //reception of the last break
                if( SequentialBreaks == FALSE )
                {
                    //Indicate that a break signal has arrived
                    SequentialBreaks = TRUE;
                    // Store data and status
                    SCICLIENT_fnStoreRxData( RxData,
                                             (uchar8)SCIDRV_eBREAK );
                }
             }
             else
             {
                //Frame error with received character not NULL
                //To reach this section of code set the sending parity to
                //even and transmit a capital P character

                // Store data and status
                SCICLIENT_fnStoreRxData( RxData,
                                         (uchar8)SCIDRV_eERROR );

             } //End if ( ucChar == SCI_NULL )
             break;

        // Parity or Overrun error condition
        case SCIDRV_eERROR_PO:
            //Parity or overrun error generated

            // Store data and status
            SCICLIENT_fnStoreRxData( RxData,
                                     (uchar8)SCIDRV_eERROR );
            break;

        // No error occurs
        case SCIDRV_eOK:
             if( SequentialBreaks == FALSE )
             {

                SCICLIENT_fnStoreRxData( RxData,
                                         (uchar8)SCIDRV_eOK );
             }
             else
             {
                SequentialBreaks = FALSE;
             }
             break;

        // shall not get here
        default:
            assert( 0 );
            break;
    }
}

/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnRxIsr

PURPOSE:
    SCI RX interrupt

    Determines that status of a received character and stores the status and
    data in buffers

INPUTS:
    None

OUTPUTS:
    None

NOTES:
    When SCI receive FIFO interrupt is enabled, RXINT is the common interrupt for
    SCI FIFO receive, receive error, receive break and receive FIFO overflow conditions.

    The interrupt receives a character and determines the status of the received
    character. If a break condition is detected as indicated by the recption of a
    NULL and a frame error then the receiver stores only one break detect if
    multiple breaks are received sequentially. This results from the following
    condition.

    The break signal is created by holding the receiver line low for longer than
    the duration of a character. The break genreation will be done by dropping
    the sending baud rate to one-half of the receivers rate and sending a NULL. A
    break detect is detected when a frame error is generated and the received
    character is a NULL.


    When a character is received the value of the character and the status of the
    character are stored in separate queues. The head pointers are then set to
    the next position in the queue.

    The queues are read by
    SCIDRV_teSTATUS scidrv_fnReceiveByte( uchar8 *ucRxData )

    The length of the queue should be at least equal to the length of the longest
    message that will be sent to the SCI prior to being read.

    When a character has been received the routine will POST that a character is
    ready for reception.

    uC/OS-II RTOS must be operating for routine to be used


VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP and implemented recieve
      FIFO interrupt.

*******************************************************************************/
void SCIDRV_fnRxIsr( void )
{
    uchar8 i;
    union SCIRXBUF_REG  unRxData;    // Received character and FIFO flags
    union SCIRXST_REG   unRxStatus;  // Receiver status

    // Obtain the status of the receiver
    unRxStatus.all = SciRegs.SCIRXST.all;

    // Obtain the received character and recieve FIFO error flags
    unRxData.all = SciRegs.SCIRXBUF.all;

    // Test if the interrupt was generated by a break condition
    if( unRxStatus.bit.BRKDT == F28X_SCIRXST_BRKDT_FLAG_IS_SET)
    {
        // Reset the SCI after a break condition
        scidrv_fnResetSci();

        // Store data and status
        scidrv_fnStoreRxChar( ( uchar8 )unRxData.bit.RXDT,
                              SCIDRV_eBREAK );
    }
    // Test if the interrupt was generated by a error condition
    else if( unRxStatus.bit.RXERROR == F28X_SCIRXST_RX_ERROR_FLAG_IS_SET )
    {
        // Reset the SCI after an error condition
        scidrv_fnResetSci();

        // Frame error
        if( unRxStatus.bit.FE == F28X_SCIRXST_FRAME_ERR_FLAG_IS_SET )
        {
            scidrv_fnStoreRxChar( ( uchar8 )unRxData.bit.RXDT,
                                  SCIDRV_eERROR_FRAME );
        }
        // Parity or Overrun error
        else
        {
            scidrv_fnStoreRxChar( ( uchar8 )unRxData.bit.RXDT,
                                  SCIDRV_eERROR_PO );
        }
    }
    // The interrupt was generated by SCI FIFO receive
    else
    {
        //SCIFFE and SCIFFPE are asociated with the character on the top of the FIFO
        //Test if a parity error occured
        if(  unRxData.bit.SCIFFPE == F28X_SCIRXBUF_SCIFFPE_IS_SET )
        {
            scidrv_fnStoreRxChar( ( uchar8 )unRxData.bit.RXDT,
                                  SCIDRV_eERROR_PO );
        }
        //Test if a frame error occured
        else if( unRxData.bit.SCIFFFE == F28X_SCIRXBUF_SCIFFFE_IS_SET )
        {
            scidrv_fnStoreRxChar( ( uchar8 )unRxData.bit.RXDT,
                                  SCIDRV_eERROR_FRAME );
        }
        else
        {
            // Store the character which is on the top of the  FIFO
            scidrv_fnStoreRxChar( ( uchar8 )unRxData.bit.RXDT,
                                  SCIDRV_eOK );

            // Get and store the rest characters in FIFO
            for ( i = 1; i < SciRegs.SCIFFRX.bit.RXFIFST; i++ )
            {
                scidrv_fnStoreRxChar( ( uchar8 )SciRegs.SCIRXBUF.bit.RXDT,
                                      SCIDRV_eOK );
            }
        }

        // TBD: receive FIFO overrun error
        //if( SciRegs.SCIFFRX.bit.RXFFOVF == F28X_SCIFFRX_RX_FIFO_OVF_FLAG_IS_SET )
        //{
        //}
    }

    // Clear Overflow flag
    SciRegs.SCIFFRX.bit.RXFFOVRCLR = F28X_SCIFFRX_CLEAR_RX_FIFO_OVF_FLAG;

    // Clear Interrupt flag
    SciRegs.SCIFFRX.bit.RXFFINTCLR = F28X_SCIFFRX_CLEAR_RX_FIFO_INT_FLAG;

    // Issue PIE ACK
    PieCtrlRegs.PIEACK.all |= IER_INT9_ACK;
}

/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnReceiveByte

PURPOSE:
    Receive a byte from the SCI

INPUTS:
    RxData - Pointer to where to store the character

OUTPUTS:
    returns - Status of the reception

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP

*******************************************************************************/
SCIDRV_teSTATUS SCIDRV_fnReceiveByte( uchar8 *RxData )
{
    uchar8 Data;
    uchar8 Status;

    // Recover the received data
    SCICLIENT_fnRetreiveRxData( &Data, &Status );

    // Set the data value
    *RxData = Data;

    // Return the status
    return( ( SCIDRV_teSTATUS )Status );
}


/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnSetTimeout

PURPOSE:
    Set timeout for reception of character

INPUTS:
    Value of the timeout

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP

*******************************************************************************/
void SCIDRV_fnSetTimeout( uint16 Timeout )
{
    // Set timeout value
    RxTimeOut = Timeout;
}


/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnGetTimeout

PURPOSE:
    Get timeout for reception of a character

INPUTS:
    None

OUTPUTS:
    Vale of SCI timeout

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP


*******************************************************************************/
uint16 SCIDRV_fnGetTimeout( void )
{
    // Obtain the timeout value
    return( RxTimeOut );
}


/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnSendByte

PURPOSE:
    Send a single character to the SCI data transmission queue AND enable the
    transmiter

NOTES:
    The function operates as follows:

    1.  The data to be transmited is stored by the SCICLIENT application.

    2.  Enable the transmitter to trigger the transmit interrupt happen.

    SCICLIENT_fnStoreTxData must be created for the specific application.
    This routine provides client specific implementation of the data storage
    mechanism independent of the system.

    SCICLIENT_fnStoreTxData returns TRUE if the data was stored
    SCICLIENT_fnStoreTxData returns FALSE if the data was not stored

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Created

*******************************************************************************/
void SCIDRV_fnSendByte( uchar8 SerialData )
{
    // Test if data was placed in storage
   if( SCICLIENT_fnStoreTxData( SerialData ) == TRUE )
   {
        if( SciRegs.SCICTL1.bit.TXENA == F28X_SCICTL1_TX_DIS )
        {
            // Enable transmit if Tx data is ready
             SCIDRV_fnEnTransmitter();
         }
   }
}

/*******************************************************************************

FUNCTION NAME:
    scidrv_fnResetSci

PURPOSE:
    Reset the SCI

INPUTS:
    None

NOTES:
    To reset the SCI write a 0 the the SW RESET bit of SCICTL1. To re-enable the
    SCI module write a 1 to the bit. This routine must be called after the
    detection of a break condition or a receive error.

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP

*******************************************************************************/
static void scidrv_fnResetSci( void )
{
    // reset the SCI module
    SciRegs.SCICTL1.bit.SWRESET = F28X_SCICTL1_SW_RESET_ASSERT;

    // reenable the SCI module
    SciRegs.SCICTL1.bit.SWRESET = F28X_SCICTL1_SW_RESET_DEASSERT;
}


/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnDisTransmitter

PURPOSE:
    Disable the SCI transmitter

INPUTS:
    none

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP

*******************************************************************************/
#if( FN_DISABLE_TX == TRUE )  // Function compile enable

void SCIDRV_fnDisTransmitter( void )
{
    // Disable the transmitter
    SciRegs.SCICTL1.bit.TXENA = F28X_SCICTL1_TX_DIS;
}

#endif


/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnDisReceiver

PURPOSE:
    Disable the SCI receiver module

INPUTS:
    none

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP

*******************************************************************************/
#if( FN_DISABLE_RX == TRUE ) // Function compile enable

void SCIDRV_fnDisReceiver( void )
{
    // Disable receiver
    SciRegs.SCICTL1.bit.RXENA = F28X_SCICTL1_RX_DIS;
}

#endif


/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnEnTransmitter

PURPOSE:
    Enable the SCI transmitter

INPUTS:
    none

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP

*******************************************************************************/
#if( FN_ENABLE_TX == TRUE ) // Function compile enable

void SCIDRV_fnEnTransmitter( void )
{
    // Enable the transmitter
    SciRegs.SCICTL1.bit.TXENA = F28X_SCICTL1_TX_EN;
}

#endif

/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnEnReceiver

PURPOSE:
    Enable the SCI receiver module

INPUTS:
    none

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP

*******************************************************************************/
#if( FN_ENABLE_RX == TRUE ) // Function compile enable

void SCIDRV_fnEnReceiver( void )
{
    // Enable the receiver
    SciRegs.SCICTL1.bit.RXENA = F28X_SCICTL1_RX_EN;
}

#endif

/*******************************************************************************

FUNCTION NAME:
    SCIDRV_fnSendBreak

PURPOSE:
    Send a break signal to the SCI

INPUTS:
    none

OUTPUTS:
    None

NOTES:
    The generation of the break signal waits until both the transmit buffer and
    the transmit shift register are empty. This indicates that no data is in the
    transmit queue.

    A break is defined by the DSP as being a low on the RX line for ten bit
    periods following a missing stop bit. As such, the break signal is generated
    by dropping the baud rate in half and sending out a NULL.

    The routine must wait until the NULL has been sent out where it then can
    reset the baud rate back to it's original rate.


VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP

*******************************************************************************/
#if( FN_SEND_BREAK == TRUE )   // Function compile enable

void SCIDRV_fnSendBreak( void )
{
    uint16 BaudRate;       // Current baud rate storage
    uint16 BreakBaudRate;  // Break signal baud rate

    // Recover the baud rate from the baud select registers
    BaudRate = ( ( uint16 )( SciRegs.SCIHBAUD ) << BAUD_HIGH )
               | ( uint16 )( SciRegs.SCILBAUD );

    // Create the baud rate for sending out the break signal
    // Break signal is a NULL transmitted at 1/2 the baud rate
    // Multiple current baud rate divisor by 2
    BreakBaudRate = BaudRate * BREAK_MULTIPLIER;

    // Wait for any serial transmissions to complete before changing the baud rate
    // otherwise data in the process of being transmitted will be corrupted
    while( SciRegs.SCICTL2.bit.TXEMPTY == \
           F28X_SCICTL2_TX_EMPTY_FLAG_IS_CLR )
    {
    }

    // Drop the baud rate
    SciRegs.SCIHBAUD = ( BreakBaudRate >> BAUD_HIGH ) & BAUD_MSK;
    SciRegs.SCILBAUD = BreakBaudRate & BAUD_MSK;

    // Send a NULL to the SCI
    SciRegs.SCITXBUF = SCI_NULL;

    // Initialize the break timeout count

    // Wait for the break transmissions to complete before changing the baud rate
    while( SciRegs.SCICTL2.bit.TXEMPTY == \
           F28X_SCICTL2_TX_EMPTY_FLAG_IS_CLR )
    {
    }

    // Restore the origional baud rate
    SciRegs.SCIHBAUD = ( BaudRate >> BAUD_HIGH ) & BAUD_MSK;
    SciRegs.SCILBAUD = BaudRate & BAUD_MSK;

} // End SCI_fnSendBreak

#endif
