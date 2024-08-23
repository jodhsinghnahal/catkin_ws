/*===========================================================================
Copyright © 2005 Xantrex International

This file is the property of Xantrex International and shall not be reproduced,
copied, or used as the basis for the manufacture or sale of equipment without
the express written permission of Xantrex International.

FILE NAME:  
    scidrv.c

PURPOSE:
   This file contains the driver code for the Serial Communications Interface
   for the F280x series of DSP from Texas Instruments. The driver utilizes
   interrupt driven transmission and reception of serial data with F280x's 
   enhanced FIFO features. 
   
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
    SCIDRV_fnRxIsr           - Receiving ISR

    Private:
         scidrv_fnResetSci      - Reset the SCI module
    
NOTES:   
   F280x has up to two SCI ports. Only one port can be enabled at a time in this 
   driver. The desired port can be defined via the macro definitions SCIA_ENABLE
   and SCIB_ENABLE.
    
CHANGE HISTORY:
$Log: SciDrv.c $

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/4
    User: JeffF     Date: 08/09/06  Time: 09:40PM
    removed duplicate calls

    ***********************************************
    Revision: SurveyorPfrmA_New_HueyD/1
    User: HueyD     Date: 06/21/06  Time: 06:06PM
    removed unused include

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/3
    User: JeffF     Date: 02/13/06  Time: 06:59PM
    Ran the copyright updater script to bring the copyright notice on all project
    files up to date.

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/2
    User: JeffF     Date: 12/08/05  Time: 08:05PM
    Added an error flag to handle SCI errors and reset the command array when
    an error is received. Issue came up with the SC-Comm tool and how it operates
    with in regards to the bootloader.

    ***********************************************
    Revision: SurveyorPfrmA_HenryC/2
    User: HenryC     Date: 07/16/05  Time: 07:39PM
    fprintf no longer uses FILE*, printf now support with direct fifo writes

    
    ***********************************************
    Revision: SurveyorPfrmA_HenryC/1
    User: HenryC     Date: 07/06/05  Time: 11:27PM
    
    Major Recoding of the SCI , no longer uses RTOS messaging , RXints loads CLI 
    command string directly, TX loads sci directly and properly uses fifos. TX 
    interrupts removed

==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"     // Requires an architecture defintion in the compiler
#include "LIB_stddefs.h"     // Standard defintions for all projects
#include "scidrv.h"          // Header file for scidrv.c
#include "sciclient.h"       // Header file for sciclient.c
#include "device.h"          // Header file for target
#include "devicemacro.h"     // Header file for device macros
#include "xassert.h"         // Header file for assert
#include "SysClk.h"          // clock speed functions
#include "stdio.h"
#include "Cli.h"

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
#define SCIA_ENABLE           ( TRUE )
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
#define FN_SEND_BREAK         ( FALSE )

/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                           Local/Private Constants
==============================================================================*/


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

//Set the driver instantiation to not instantiated
static uchar8 SciDriverInstance = eSCI_DRIVER_NOT_INSTANTIATED;

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
// Reset the SCI module
static void scidrv_fnResetSci( void );

#if( SCIA_ENABLE == TRUE )
    extern void ISR_fnSCIRXINTA( void );
#endif

#if( SCIB_ENABLE == TRUE )
    extern void ISR_fnSCIRXINTB( void );
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
                     
    Data bits - SCIDRV_teDATA_BITS DataBits
    - Data bit enums    SCIDRV_eDATA_BITS_7        7 data bits
                        SCIDRV_eDATA_BITS_8        8 data bits

    Stop bits - SCIDRV_teSTOP_BITS StopBits
    - Stop bit enums    SCIDRV_eSTOP_BITS_1        1 stop bit
                       
                       
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
   
    The function SCICLIENT_fnCtor must be called to set up system specific 
    services for the SCI driver
   
VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP and added 
      initialization for SCI FIFO registers.
Version: 1.01    Date: 28-Mar-2004    By: Jeff Fieldhouse       
    - Changed a few of the registers to reflect the differnces between the 
      281x and the 280x devices.
     
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
#if( SCIA_ENABLE == TRUE )
        SysCtrlRegs.PCLKCR0.bit.SCIAENCLK  = F280X_PCLKCR_CLK_ON;    // SCI-A 
        
        // Set the mux register to enable for SCI functionality on the pins
        GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;
        GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1; 
        
        // Load a pointer to the SCI ISR functions into the vector table
        PieVectTable.SCIRXINTA = &ISR_fnSCIRXINTA;
        
        // Enable the RX interrupt
        PieCtrlRegs.PIEIER9.bit.INTx1 = 1;    // PIE Group 9, INT1 for SCIARx
#else
        SysCtrlRegs.PCLKCR0.bit.SCIBENCLK  = F280X_PCLKCR_CLK_ON;   // SCI-B 
        
        // Set the mux register to enable for SCI functionality on the pins        
        GpioCtrlRegs.GPAMUX1.bit.GPIO9  = 2;
        GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;
        
        // Load a pointer to the SCI ISR functions into the vector table        
        PieVectTable.SCIRXINTB = &ISR_fnSCIRXINTB;
        PieVectTable.SCITXINTB = &ISR_fnSCITXINTB;
        
        // Enable the RX interrupt        
        PieCtrlRegs.PIEIER9.bit.INTx3 = 1;    // PIE Group 9, INT3 for SCIBRx
#endif
           
        // Disable register access
        DEVICE_mACCESS_DIS();  
        
        // Disable the SCI hardware prior to initialization of SCI control varaibles
        // 
        // Disable SCI receiver
        SciRegs.SCICTL1.bit.RXENA = F280X_SCICTL1_RX_DIS;
        
        // Disable SCI transmitter
        SciRegs.SCICTL1.bit.TXENA = F280X_SCICTL1_TX_DIS;
        
        // Disable the RxErr interrupt
        SciRegs.SCICTL1.bit.RXERRINTENA = F280X_SCICTL1_RX_ERR_INT_DIS;
        
        // Disable the RxBk interrupt
        SciRegs.SCICTL2.bit.RXBKINTENA = F280X_SCICTL2_RXBK_INT_DIS;
        
        // Disable the Tx interrupt
        SciRegs.SCICTL2.bit.TXINTENA = F280X_SCICTL2_TXRDY_INT_DIS;
        
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
        SciRegs.SCICCR.bit.PARITYENA = F280X_SCICCR_PARITY_ENA_DIS;  
    
        // Set the number of stop bits
        SciRegs.SCICCR.bit.STOPBITS = F280X_SCICCR_STOP_BITS_ONE;
       
        // Set the number of data bits
		SciRegs.SCICCR.bit.SCICHAR = F280X_SCICCR_CHARLEN_8;
       
        // Set up the SCI registers
        // 
        // Set idle line communications mode
        SciRegs.SCICCR.bit.ADDRIDLE_MODE = F280X_SCICCR_MULTIPRO_MODE_IDLELINE;
            
        // Disable transmitter wake mode
        SciRegs.SCICTL1.bit.TXWAKE = F280X_SCICTL1_TX_WAKE_DIS;
        
        // Disable receiver sleep mode
        SciRegs.SCICTL1.bit.SLEEP = F280X_SCICTL1_SLEEP_DIS;
        
        // Set the emulation suspend mode to complete sequence before stopping
        SciRegs.SCIPRI.all |= F280X_SCIPRI_SUSMODE_SOFT << F280X_SCIPRI_SUSMODE_BITPOS;
        
        // Enable the receive/break interrupt
        SciRegs.SCICTL2.bit.RXBKINTENA = F280X_SCICTL2_RXBK_INT_EN;
        
        // Enable the Rx error interrupt
        SciRegs.SCICTL1.bit.RXERRINTENA = F280X_SCICTL1_RX_ERR_INT_EN;
        
        // Set up the SCI FIFO registers
        //
        // Resume SCI FIFO transimitter or receiver
        SciRegs.SCIFFTX.bit.SCIRST = F280X_SCIFFTX_TXRX_CHAN_RESUME;
        
        // Enable FIFO transmit and receive
        SciRegs.SCIFFTX.bit.SCIFFENA = F280X_SCIFFTX_TXRX_FIFO_EN;
        
        // Reset the Tx FIFO pointer and hold in reset
        SciRegs.SCIFFTX.bit.TXFIFOXRESET = F280X_SCIFFTX_TX_FIFO_PTR_RESET;
        
        // Trigger Tx interrupt when transmit FIFO is empty 
        // SciRegs.SCIFFTX.bit.TXFFIL = F280X_SCIFFRX_RX_FIFO_INTLVL_0; 

        // DISABLE Tx FIFO interrupt
        SciRegs.SCIFFTX.bit.TXFFIENA = F280X_SCIFFTX_TX_FIFO_INT_DIS;
        
        // Re-enable transmit FIFO operation
        SciRegs.SCIFFTX.bit.TXFIFOXRESET = F280X_SCIFFTX_TX_FIFO_PTR_REEN;
        
        // Reset the Rx FIFO pointer and hold in reset
        SciRegs.SCIFFRX.bit.RXFIFORESET = F280X_SCIFFRX_RX_FIFO_PTR_RESET;
        
        // Trigger Rx interrupt when receive FIFO has 1 word 
        SciRegs.SCIFFRX.bit.RXFFIL = F280X_SCIFFRX_RX_FIFO_INTLVL_1; 

        // Enable Rx FIFO interrupt
        SciRegs.SCIFFRX.bit.RXFFIENA = F280X_SCIFFRX_RX_FIFO_INT_EN;

        // Re-enable receive FIFO operation
        SciRegs.SCIFFRX.bit.RXFIFORESET = F280X_SCIFFRX_RX_FIFO_PTR_REEN;
        
        // No delay between every transfer from FIFO transmit buffer to 
        // transmit shift register   
        SciRegs.SCIFFCT.bit.FFTXDLY = SCIDRV_FFTX_DELAY;
        
        // Enable INT9 required for SCI
        //
        DEVICE_mINT9_EN();  
            
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
        // Enable access to protected registers
        DEVICE_mACCESS_EN();             
        
#if( SCIA_ENABLE == TRUE ) 
        // Disable the SCI module by setting the pins to their GPIO function
        GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;
        GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;
#else
        GpioCtrlRegs.GPAMUX1.bit.GPIO9  = 0;
        GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;
#endif
        
        // Disable register access
        DEVICE_mACCESS_DIS();
                
        // Disable SCI receiver
        SCIDRV_fnDisReceiver();

        // Disable the trasnmitter
        SCIDRV_fnDisTransmitter();
         
        // Disable the RxBk interrupt
        SciRegs.SCICTL2.bit.RXBKINTENA = F280X_SCICTL2_RXBK_INT_DIS;

        // Disable the RxErr interrupt
        SciRegs.SCICTL1.bit.RXERRINTENA = F280X_SCICTL1_RX_ERR_INT_DIS;
        
        // Disable Rx FIFO interrupt
        SciRegs.SCIFFRX.bit.RXFFIENA = F280X_SCIFFRX_RX_FIFO_INT_DIS;

        // Disable Tx FIFO interrupt
        SciRegs.SCIFFTX.bit.TXFFIENA = F280X_SCIFFTX_TX_FIFO_INT_DIS;
               
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

#endif 


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
   
  
     
VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project, modified for the F2812 DSP and implemented recieve 
      FIFO interrupt.
Version: 1.01	 Date: 12-JUL-2005    By: Henry Cutler
	- removed break processing and incomming chars are sent directly to CLI string
Version: 1.02    Date: 08-Dec-2005    By: Jeff Fieldhouse
    - Added call with error code when an SCI error is received. This is used
      to flush the command buffer so that it doesn't screw up following commands		  
     
*******************************************************************************/
void SCIDRV_fnRxIsr( void )
{   
    union SCIRXBUF_REG  unRxData;    // Received character and FIFO flags
    union SCIRXST_REG   unRxStatus;  // Receiver status

    // Obtain the status of the receiver
    unRxStatus.all = SciRegs.SCIRXST.all;
    
    // Obtain the received character and recieve FIFO error flags
    unRxData.all = SciRegs.SCIRXBUF.all;

    // Test all possible error conditions , if any are seen reset scio
    if( ( unRxStatus.bit.BRKDT == F280X_SCIRXST_BRKDT_FLAG_IS_SET ) ||
        ( unRxStatus.bit.RXERROR == F280X_SCIRXST_RX_ERROR_FLAG_IS_SET ) ||
        ( unRxStatus.bit.FE == F280X_SCIRXST_FRAME_ERR_FLAG_IS_SET ) ||
		( unRxData.bit.SCIFFPE == F280X_SCIRXBUF_SCIFFPE_IS_SET ) ||
		( unRxData.bit.SCIFFFE == F280X_SCIRXBUF_SCIFFFE_IS_SET ) )
	{        
    	// Reset the SCI
        scidrv_fnResetSci(); 

        // Send an error command to the CLI
        CLI_fnStoreRxData( CLI_BYTE_ERROR );
    }

   	// The interrupt was generated by SCI FIFO receive 
   	// and byte has no errors
    else
    {
    	// Store Data directly to the CLI command string
		CLI_fnStoreRxData( ( uchar8 )unRxData.bit.RXDT );
    }
    
    // Clear Overflow flag
    SciRegs.SCIFFRX.bit.RXFFOVRCLR = F280X_SCIFFRX_CLEAR_RX_FIFO_OVF_FLAG; 
    
    // Clear Interrupt flag
    SciRegs.SCIFFRX.bit.RXFFINTCLR = F280X_SCIFFRX_CLEAR_RX_FIFO_INT_FLAG;
    
    // Issue PIE ACK    
    PieCtrlRegs.PIEACK.all |= IER_INT9_ACK;    
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
    SciRegs.SCICTL1.bit.SWRESET = F280X_SCICTL1_SW_RESET_ASSERT;
    
    // reenable the SCI module
    SciRegs.SCICTL1.bit.SWRESET = F280X_SCICTL1_SW_RESET_DEASSERT; 
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
    SciRegs.SCICTL1.bit.TXENA = F280X_SCICTL1_TX_DIS;
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
    SciRegs.SCICTL1.bit.RXENA = F280X_SCICTL1_RX_DIS;
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
    SciRegs.SCICTL1.bit.TXENA = F280X_SCICTL1_TX_EN;
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
    SciRegs.SCICTL1.bit.RXENA = F280X_SCICTL1_RX_EN;
} 

#endif

