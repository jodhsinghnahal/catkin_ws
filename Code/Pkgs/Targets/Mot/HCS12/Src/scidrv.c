/*=============================================================================
Copyright © 2002 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and 
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in 
whole or in part, to anyone outside of Xantrex without the written approval of 
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of 
Xantrex who has not previously obtained written authorization for access from 
the individual responsible for the source code, will have a significant 
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:  scidrv.c

PURPOSE:
    This file has the functions that control the first SCI port on the
    Motorola MC9S12DP256 microcontroller.

FUNCTION(S):
    SCIDRV_fnCtor           - Initialize SCI port 0
    SCIDRV_fnDtor           - Quit using the serial port
    SCIDRV_fnEnTransmitter  - Enable the SCI transmitter
    SCIDRV_fnDisTransmitter - Disable the SCI transmitter
    SCIDRV_fnEnReceiver     - Enable the SCI receiver
    SCIDRV_fnDisReceiver    - Disable the SCI receiver
    SCIDRV_fnSendByte       - Send a byte to the serial port
    SCIDRV_fnSetTimeout     - Set the timeout for reception of a serial byte
    SCIDRV_fnReceiveByte    - Receive a serial byte and the status of the byte
    SCIDRV_fnSendBreak      - Send a break on the serial port 
    SCI0_ISR                - Interrupt Service Routine for SCI port 0

    local:
        scidrv_fnSciTx          - Transmit a queued byte on the SCI
        scidrv_fnSciRx          - Put a received byte into the receive queue
        scidrv_fnISR            - Main body of code for SCI Interrupt

NOTES:
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use CodeWright or ignore 
    **the text.
    
    This driver uses the Queue services of the uC/OS-II Real Time OS

HISTORY:
$History: scidrv.c $
    
    *****************  Version 22  *****************
    User: Fredp        Date: 23/01/04   Time: 4:21p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/HCS12/Packages/HAL/Src
    Added code to do timing check for ipcchain task and ISR
    
    *****************  Version 21  *****************
    User: Ronm         Date: 1/16/04    Time: 11:50a
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Baseline/HCS12/Packages/HAL/Src
    - Increased rx buffer (to accomodate larger msgs), decreased tx buffer.
    
    *****************  Version 20  *****************
    User: Hairuozo     Date: 1/06/04    Time: 11:38a
    Updated in $/PD/Network_Extensions/Code/Management_Device/AGS/Alpha/HAL/SCI
    following the changes made on device.h,
    change mDISABLE_INTR() to DEVICE_mGLOBAL_INT_DIS() and
                 mENABLE_INTR() to DEVICE_mGLOBAL_INT_EN() 
    
    *****************  Version 19  *****************
    User: Ronm         Date: 12/03/03   Time: 6:59p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/RV_Series/RS2000_1520B/HCS12/HAL/Src
    - decreased queue sizes in order to conserve RAM. They will need to be
    increased for the full Nova/XB build.
    
    *****************  Version 18  *****************
    User: Ronm         Date: 12/03/03   Time: 10:24a
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/RV_Series/RS2000_1520B/HCS12/HAL/Src
    - reduced queue size since max msg (with a segment) will never be this
    big.
    
    *****************  Version 17  *****************
    User: Toml         Date: 10/02/03   Time: 1:19p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/InvChgEx/HAL/Src
    Updated to latest version of scidrv.h
    
    *****************  Version 16  *****************
    User: Ronm         Date: 8/29/03    Time: 4:16p
    Updated in $/PD/Inverter_Charger/Nova_Series/Code/Common/Coprocessor/HAL/Src
    - Increased buffer sizes to accomodate larger extended message sizes.
    
    *****************  Version 15  *****************
    User: Baldeeshk    Date: 25/07/03   Time: 9:59a
    Updated in $/PD/Network_Extensions/Code/Common/HAL/Src
    Changed macros that disable/enable interrupts to use the ones defined
    in device.h.  Removed plldrv.h include, uses bus clock from config.h.
    
    *****************  Version 14  *****************
    User: Peterd       Date: 7/24/03    Time: 3:23p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/UIEx/HAL/Src
    Changed #ifdef MCU_xxxx selection to #include "config.h"
    
    *****************  Version 13  *****************
    User: Toml         Date: 6/27/03    Time: 3:32p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DG256B/Test/UIEx/HAL/Src
    New API
    
    *****************  Version 12  *****************
    User: Toml         Date: 5/12/03    Time: 3:50p
    Updated in $/PD/Common/Protocols/XanBus/Code/Motorola/S12DP256B/Test/UIEx/HAL/Src
    Reduced chances of interrupt stack overflow on exit from ISRs.
    Eliminated compiler warnings.
    
    *****************  Version 11  *****************
    User: Zoranm       Date: 1/05/03    Time: 5:33p
    Updated in $/PD/Network_Extensions/Code/UI/SCP/HAL/Src
    Added support for 6812DG256B
    
    *****************  Version 10  *****************
    User: Baldeeshk    Date: 10/04/03   Time: 2:19p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Modified calculation of baud rate registers.  It now gets the bus clock
    from the pll driver and calculates what should be in the baud rate
    register accordingly.
    
    *****************  Version 9  *****************
    User: Toml         Date: 2/17/03    Time: 6:31p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Updated for new <6812xxxxxB.h> header files
    
    *****************  Version 8  *****************
    User: Toml         Date: 2/03/03    Time: 2:04p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Reformatted file and function headers
  
  *****************  Version 7  *****************
  User: Baldeeshk    Date: 31/01/03   Time: 5:24p
  Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
  Implemented SCI_fnSendBreak function.
    
    *****************  Version 6  *****************
    User: Baldeeshk    Date: 28/01/03   Time: 11:57a
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Changed variable declaration in destructor to the beginning of the
    function.
    
    *****************  Version 5  *****************
    User: Toml         Date: 1/27/03    Time: 4:37p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Prevented queue overflow on transmit.
    
    *****************  Version 4  *****************
    User: Karli        Date: 1/24/03    Time: 5:06p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Move local function prototypes for sci_fnSciTx, sci_fnSciRx, and
    SCI0_ISR into file and added stub for SCI_fnSendBreak. 
    
    *****************  Version 3  *****************
    User: Toml         Date: 1/23/03    Time: 7:45p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Changed interrupt routine to handle interrupt stack switching.
    
    *****************  Version 2  *****************
    User: Toml         Date: 1/21/03    Time: 12:54p
    Updated in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Updated RTOS to uC/OS version 2.61
    
    *****************  Version 1  *****************
    User: Toml         Date: 1/20/03    Time: 6:11p
    Created in $/PD/Common/Targets/Motorola/HCS12/HAL/Src
    Initial version

=============================================================================*/

/*=============================================================================
                                 Include files
=============================================================================*/

#include "config.h"
#include "INCLUDES.H"
#include "scidrv.h"

// define SCIDRV_ESMASTER_TASK_TIME_CHECK to show the IPCchan task time
//
// you MUST enable the ESMASTER_TASK_TIME_CHECK flag in the esmaster.c file to get good result
//
//#define SCIDRV_ESMASTER_TASK_TIME_CHECK
//

//
// define SCIDRV_IRQ_TIME_CHECK to show the  
//
// #define SCIDRV_IRQ_TIME_CHECK
//
#ifdef  SCIDRV_IRQ_TIME_CHECK
#include "debug.h"
#endif  // SCIDRV_IRQ_TIME_CHECK

/*=============================================================================
                                local prototypes
=============================================================================*/

#pragma CODE_SEG __NEAR_SEG INTERRUPT_ROUTINES

// Transmit a queued byte on the SCI
void scidrv_fnSciTx( void );

// Put a received byte into the receive queue
void scidrv_fnSciRx( uchar8 SciStatus, uchar8 SciData );

// SCI Interrupt Service Routine
void SCI0_ISR( void );

#pragma CODE_SEG DEFAULT

/*=============================================================================
                              Internal Definitions
=============================================================================*/

#define SCISR1_TDRE 0x80
#define SCISR1_TC   0x40
#define SCISR1_RDRF 0x20
#define SCISR1_IDLE 0x10
#define SCISR1_OR   0x08
#define SCISR1_NF   0x04
#define SCISR1_FE   0x02
#define SCISR1_PF   0x01

#define SCICR1_LOOPS 0x80
#define SCICR1_SCISWAI 0x40
#define SCICR1_RSRC  0x20
#define SCICR1_M     0x10
#define SCICR1_WAKE  0x08
#define SCICR1_ILT   0x04
#define SCICR1_PE    0x02
#define SCICR1_PT    0x01

#define SCICR2_TIE   0x80
#define SCICR2_TCIE  0x40
#define SCICR2_RIE   0x20
#define SCICR2_ILIE  0x10
#define SCICR2_TE    0x08
#define SCICR2_RE    0x04
#define SCICR2_RWU   0x02
#define SCICR2_SBK   0x01

#define SCIDRV_RX_QUEUE_SIZE 255
#define SCIDRV_TX_QUEUE_SIZE 100
#define SCIDRV_RX_TIMEOUT    0
#define SCIDRV_ROUND_FACTOR  10

// Baud rate values
#define SCIDRV_BAUD_2400         2400
#define SCIDRV_BAUD_4800         4800
#define SCIDRV_BAUD_9600         9600
#define SCIDRV_BAUD_19200       19200
#define SCIDRV_BAUD_38400       38400
#define SCIDRV_BAUD_57600       57600
#define SCIDRV_BAUD_115200     115200

//Masks to configure baud rate
#define BAUD_HIGH         8 
#define BAUD_MSK          0x00FF
#define BREAK_MULTIPLIER  2
#define NULL_BYTE         0x00

/*=============================================================================
                                   Local Data
=============================================================================*/

static OS_EVENT *scidrv_pzRxQueue;               // Points to recieve queue
static OS_EVENT *scidrv_pzTxQueue;               // Points to transmit queue
static void *scidrv_apRxQ[ SCIDRV_RX_QUEUE_SIZE ];  // Recieve queue pointers
static void *scidrv_apTxQ[ SCIDRV_TX_QUEUE_SIZE ];  // Transmit queue pointers
static uint16 scidrv_uiRxTimeout;

/******************************************************************************

FUNCTION NAME:
    SCIDRV_fnCtor

PURPOSE:
    This function initializes the SCI port

INPUTS:
    'eSciBaudRate' is an enum of the desired baud rate
    'eSciParity' is an enum of the desired parity setting
    'eSciDataBits' is an enum of the desired number of data bits (7 or 8 only)
    'eSciStopBits' is desired number of Stop bits (1 only)

OUTPUTS:
    eSTATUS_OK if all goes well.

NOTES:
    This function must be called before any other access to the SCI port

VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header
    
Version 1.10    Date: Jan 23,2004   By: Fred Pang
    - Added code to do ISR timing check

******************************************************************************/

tuiSTATUS SCIDRV_fnCtor( SCIDRV_teBAUD_RATE eSciBaudRate,
                         SCIDRV_tePARITY eSciParity,
                         SCIDRV_teDATA_BITS eSciDataBits,
                         SCIDRV_teSTOP_BITS eSciStopBits )
{
    uint32 ulBusClk;
    uint32 ulBaudRate;
    uint16 uiBaudReg;

    
    ulBusClk = ( uint32 )CONFIG_BUSCLK;
    
    // Set the baud rate
    switch ( eSciBaudRate )
    {

        case SCIDRV_eBAUD_115200:
            ulBaudRate = SCIDRV_BAUD_115200;
            break;
        
        case SCIDRV_eBAUD_57600:
            ulBaudRate = SCIDRV_BAUD_57600;
            break;
            
        case SCIDRV_eBAUD_38400:
            ulBaudRate = SCIDRV_BAUD_38400;
            break;

        case SCIDRV_eBAUD_19200:
            ulBaudRate = SCIDRV_BAUD_19200;
            break;

        case SCIDRV_eBAUD_9600:
            ulBaudRate = SCIDRV_BAUD_9600;
            break;

        case SCIDRV_eBAUD_4800:
            ulBaudRate = SCIDRV_BAUD_4800;
            break;

        case SCIDRV_eBAUD_2400:
            ulBaudRate = SCIDRV_BAUD_2400;
            break;

        default:
            // Baud rate not supported
            return eSTATUS_ERR;
    }
    
   //Calculate value for baud rate registers
   //Formula from Motorola SCI manual
   
   // get a rounded result for the baud register value
   if( ( ( ( ulBusClk * SCIDRV_ROUND_FACTOR ) / ( ulBaudRate * 16 ) )
                    % SCIDRV_ROUND_FACTOR ) >= SCIDRV_ROUND_FACTOR / 2 )
   {
       uiBaudReg = ( uint16 )( ulBusClk / ( ulBaudRate * 16 ) ) + 1;
   }
   else
   {
       uiBaudReg = ( uint16 )( ulBusClk / ( ulBaudRate * 16 ) );
   }
               
   //Set the baud rate registers
   SCI0BDH = ( uiBaudReg >> BAUD_HIGH ) & BAUD_MSK;
   SCI0BDL = uiBaudReg & BAUD_MSK;    

    // Set the parity
    switch ( eSciParity )
    {
        case SCIDRV_ePARITY_NONE:
            SCI0CR1_PE = 0;
            break;

        case SCIDRV_ePARITY_EVEN:
            SCI0CR1_PE = 1;
            SCI0CR1_PT = 0;
            break;

        case SCIDRV_ePARITY_ODD:
            SCI0CR1_PE = 1;
            SCI0CR1_PT = 1;
            break;

        default:
            // Parity not supported
            return eSTATUS_ERR;
    }

    // Set the data bits, can be only 8 or 9, including parity
    if ((( eSciParity == SCIDRV_ePARITY_NONE )
            && ( eSciDataBits == SCIDRV_eDATA_BITS_8 ))
        || (( eSciParity != SCIDRV_ePARITY_NONE )
            && ( eSciDataBits == SCIDRV_eDATA_BITS_7 )))
    {
        // 8 bits total
        SCI0CR1_M = 0;
    }
    else if (( eSciParity != SCIDRV_ePARITY_NONE )
            && ( eSciDataBits == SCIDRV_eDATA_BITS_8 ))
    {
        // 9 bits total
        SCI0CR1_M = 1;
    }
    else
    {
        // No other combination is supported
        return eSTATUS_ERR;
    }

    // Set the stop bits
    if ( eSciStopBits != SCIDRV_eSTOP_BITS_1 )
    {
        // Only 1 stop bit supported by device
        return eSTATUS_ERR;
    }

    // Make sure we're not using loopback mode
    SCI0CR1_LOOPS = 0;

    // Leave the port in the disabled state
    SCI0CR2 = 0x00;

    // Default to no timeout on the receiver
    scidrv_uiRxTimeout = 0;

    // Create the receive queue
    scidrv_pzRxQueue = OSQCreate( &scidrv_apRxQ[ 0 ], SCIDRV_RX_QUEUE_SIZE );
    if ( scidrv_pzRxQueue == NULL )
    {
        return eSTATUS_ERR;
    }

    // Create the transmit queue
    scidrv_pzTxQueue = OSQCreate( &scidrv_apTxQ[ 0 ], SCIDRV_TX_QUEUE_SIZE );
    if ( scidrv_pzTxQueue == NULL )
    {
        return eSTATUS_ERR;
    }
    
#ifdef  SCIDRV_IRQ_TIME_CHECK
    // initialize all ports
    DEBUG_mINIT_ALLPINS();
#endif  // SCIDRV_IRQ_TIME_CHECK

    // If we get this far then everything must be super
    return eSTATUS_OK;
}

/******************************************************************************

FUNCTION NAME:
    SCIDRV_fnDtor

PURPOSE:
    This function disables the SCI port.
    If the OS supports queue deletion, then the queues used by
    this driver are released.

INPUTS:
    None

OUTPUTS:
    eSTATUS_OK if all goes well

NOTES:
    Do not call any other SCI functions after this function.

VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header

******************************************************************************/

tuiSTATUS SCIDRV_fnDtor( void )
{
    tuiSTATUS RetVal;    // Value returned by this function
#if OS_Q_DEL_EN > 0
    uchar8 QDelErr;    // Error code for Queue deletion
#endif

    // Disable all functions of the serial port
    SCI0CR2 = 0x00;
    RetVal = eSTATUS_OK;

    // Release the queues if that is supported by the OS
#if OS_Q_DEL_EN > 0

    OSQDel( scidrv_pzRxQueue, OS_DEL_NO_PEND, &QDelErr );
    if ( QDelErr != OS_NO_ERR )
    {
        RetVal = eSTATUS_ERR;
    }

    OSQDel( scidrv_pzTxQueue, OS_DEL_NO_PEND, &QDelErr );
    if ( QDelErr != OS_NO_ERR )
    {
        RetVal = eSTATUS_ERR;
    }
#endif

    return RetVal;
}


/******************************************************************************

FUNCTION NAME:
    SCIDRV_fnEnTransmitter

PURPOSE:
    This function enables the SCI's transmitter.

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header

******************************************************************************/

void SCIDRV_fnEnTransmitter( void )
{
    // Set the transmit enable bit
    SCI0CR2_TE = 1;
}



/******************************************************************************

FUNCTION NAME:
    SCIDRV_fnDisTransmitter

PURPOSE:
    This function disables the SCI's transmitter and associated interrupts

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header

******************************************************************************/

void SCIDRV_fnDisTransmitter( void )
{
    // Clear the transmit enable bit and transmit interrupts
    SCI0CR2_TE = 0;
    SCI0CR2_TCIE = 0;
    SCI0CR2_SCTIE = 0;
}


/******************************************************************************

FUNCTION NAME:
    SCIDRV_fnEnReceiver

PURPOSE:
    This function enables the SCI's receiver and the receive interrupt.

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header

******************************************************************************/

void SCIDRV_fnEnReceiver( void )
{
    // Enable the reciever
    SCI0CR2_RE = 1;

    // Enable the receive interrupt
    SCI0CR2_RIE = 1;
}


/******************************************************************************

FUNCTION NAME:
    SCIDRV_fnDisReceiver

PURPOSE:
    This function disables the receiver and the receive interrupt

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header

******************************************************************************/

void SCIDRV_fnDisReceiver( void )
{
    // Disable the reciever
    SCI0CR2_RE = 0;

    // Disable the receive interrupt
    SCI0CR2_RIE = 0;
}


/******************************************************************************

FUNCTION NAME:
    SCIDRV_fnSendByte

PURPOSE:
    This function sends a byte to the SCI port.  Since transmission can
    take some time, the character is put into a queue which is handled by
    the transmit interrupt service function.

INPUTS:
    'ucSerialData' is The byte to send through the SCI port

OUTPUTS:
    Nothing

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header

Version 1.10    Date: Jan 23,2004   By: Fred Pang
    - Added code to do task timing check for ipcchain task

******************************************************************************/

void SCIDRV_fnSendByte( uchar8 ucSerialData )
{
    uint16 Message;
    uchar8 *pMessage;
    volatile uchar8 SCISR1;
    uchar8 PostStatus;

    /*
        We'll send the byte in the second byte of the
        message which the OS thinks is a pointer.  The first byte
        will be a 1, which will make even a zero byte distinguishable
        from a NULL pointer which indicates an empty queue.
    */

    pMessage = (uchar8 *)&Message;
    *pMessage++ = 0x01;          // A non-zero value which is otherwise ignored
    *pMessage = ucSerialData;

    // Put the message into the queue
    do
    {
    	PostStatus = OSQPost( scidrv_pzTxQueue, (void *)Message );
    	
    	
		//  Give the Queue a chance to empty if full
    	if ( PostStatus == OS_Q_FULL )
    	{
#ifndef SCIDRV_ESMASTER_TASK_TIME_CHECK
    		OSTimeDly( 1 );
#endif  // ~SCIDRV_ESMASTER_TASK_TIME_CHECK
    	}
    } while ( PostStatus == OS_Q_FULL );

    // Is the start of a new series of bytes?
    if ( SCI0CR2_SCTIE == 0 )
    {
        // Transmit interrupts disabled, let's get things rolling

        // Byte won't go out unless the SCISR1 register is read first
        SCISR1 = SCI0SR1;

        // Now send the queued byte to the SCI data register
        scidrv_fnSciTx();
    }
}


/******************************************************************************

FUNCTION NAME:
    SCIDRV_fnSetTimeout

PURPOSE:
    This function sets the timeout period for receiving a byte on the SCI
    port through a call to SCIDRV_fnReceiveByte()

INPUTS:
    'uiTimeout' is the timeout period in RTI ticks

OUTPUTS:
    Nothing

NOTES:
    The default timeout is zero, which means that the recieve function
    will wait forever until it recieves a character.

VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header

******************************************************************************/

void SCIDRV_fnSetTimeout( uint16 uiTimeout )
{
    scidrv_uiRxTimeout = uiTimeout;
}


/******************************************************************************

FUNCTION NAME:
    SCIDRV_fnReceiveByte

PURPOSE:
    This function receives a byte from the SCI port and returns
    the status of the byte.

INPUTS:
    'ucRxData' is a pointer to where the received byte is to go

OUTPUTS:
    SCIDRV_eOK if all goes well
    SCIDRV_eTIMEOUT if no character was received before the timeout period
    SCIDRV_eBREAK if a break character was received
    SCIDRV_eERROR if there was a noise, parity, framing, overrun or OS error.

NOTES:
    This function will cause the calling task to block until a character
    is received

VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header

******************************************************************************/

SCIDRV_teSTATUS SCIDRV_fnReceiveByte( uchar8 *ucRxData )
{
    uint16 Message;    // Received message as status/data
    uchar8 *pMessage;  // Pointer to the message
    uchar8 OSErr;      // Error code from OS function call


    // Receive the data from the queue, which the OS thinks is a pointer
    Message = (uint16)OSQPend( scidrv_pzRxQueue, scidrv_uiRxTimeout, &OSErr );
    
    // Check for timeout condition
    if ( OSErr == OS_TIMEOUT )
    {
        *ucRxData = 0x00;
        return SCIDRV_eTIMEOUT;
    }

    // Check for other OS errors
    if ( OSErr != OS_NO_ERR )
    {
        *ucRxData = 0x00;
        return SCIDRV_eERROR;
    }

    // Retrieve the status and data from the 'pointer'
    pMessage = (uchar8 *)&Message;

    // Copy the data byte which is the second byte of the message
    *ucRxData = *( pMessage + 1 );

    // Return the status byte which is the first byte of the message
    return *pMessage;
}

/******************************************************************************

FUNCTION NAME:
    SCIDRV_fnSendBreak

PURPOSE:
    This is the interrupt service routine for SCI port 0.
    The interrupt can be due to several different reasons and
    this routine has to determine the cause for the
    interrupt and call the appropriate receive or transmit function.

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version: 1.00  Date: Jan 31 2003  By: Baldeesh Khaira
    - Implemented function

Version 1.01  Date: 02/03/03  By: code_fix.py
    - Added version history to function header

******************************************************************************/

void SCIDRV_fnSendBreak( void )
{
   uint16 uiBaudRate;
   uint16 uiBreakBaudRate;
   
   //Recover the baud rate from the baud select registers
   uiBaudRate = ( ( uint16 )( SCI0BDH ) << BAUD_HIGH )
              | ( uint16 )( SCI0BDL );
              
   //Create the baud rate for sending out the break signal
   //Break signal is a NULL transmitted at 1/2 the baud rate
   //Multiple current baud rate divisor by 2
   uiBreakBaudRate = uiBaudRate * BREAK_MULTIPLIER;
   
   //Wait for any serial transmissions to complete before changing the baud rate
   //otherwise data in the process of being transmitted will be corrupted

   while( (!(SCI0SR1_TC )) || ( SCI0CR2_SCTIE ))
   {
   }

   //Drop the baud rate
   SCI0BDH = ( uiBreakBaudRate >> BAUD_HIGH ) & BAUD_MSK;
   SCI0BDL = uiBreakBaudRate & BAUD_MSK;
	
   //Send a NULL to the SCI
   SCI0DRL = NULL_BYTE;

   while( (!(SCI0SR1_TC )) || ( SCI0CR2_SCTIE ))
   {
   }

	//Restore the origional baud rate
   SCI0BDH = ( uiBaudRate >> BAUD_HIGH ) & BAUD_MSK;
   SCI0BDL = uiBaudRate & BAUD_MSK; 
}

#pragma CODE_SEG __NEAR_SEG INTERRUPT_ROUTINES

/******************************************************************************

FUNCTION NAME:
    scidrv_fnSciTx

PURPOSE:
    This function receives a byte from the transmit
    queue and sends it to the SCI port.  If the queue is empty then
    the transmit interrupt is disabled.

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:
    This function is called by SCIDRV_fnSendByte at the beginning
    of a transmission, and by the SCI0_ISR whenever the SCI
    is free to send another byte.

VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header
Version 1.01  Date: 05/12/03  By: Tom Lightfoot
    - Reduced likelihood of interrupt stack overflow on exit from ISR

******************************************************************************/

void scidrv_fnSciTx( void )
{
    uint16 Message;    // Message in transmit queue, which the OS thinks is
                       // a pointer.
    uchar8 OSError;    // Repository for error codes

    /*
        In order to save space, we'll use the pointers
        in the transmit queue for data instead of pointers to data somewhere
        else.  The last byte of the two-byte pointer will hold the byte
        to send through the serial port.
    */


    // Read the next byte from the queue, or nothing if the queue is empty
    Message = (uint16)OSQAccept( scidrv_pzTxQueue, &OSError );

    // Turn off interrupts for the remainder of this ISR
    DEVICE_mGLOBAL_INT_DIS( );

    // Is there another byte?
    if ( Message )
    {
        // There is! Send that next byte
        SCI0DRL = (uchar8)Message;
        // Note that writing to data register will clear the interrupt

        // Make sure that there will be an interrupt when the data register
        // is clear again
        SCI0CR2_SCTIE = 1;
    }
    else
    {
        // No more data to send so disable any more TDRE interrupts
        SCI0CR2_SCTIE = 0;
    }
}


/******************************************************************************

FUNCTION NAME:
    scidrv_fnSciRx

PURPOSE:
    This function is called by the interrupt service routine when it
    discovers that the interrupt is due to a received character,
    break or error.  The received character and its status are put on the
    receive queue to be processed by the SCIDRV_fnReceiveByte function.

INPUTS:
    'SciStatus' is The bitmask from the SCI Status Register
    'SciData' is the character in the SCI data register

OUTPUTS:
    Nothing

NOTES:
    This function is called by the SCI0_ISR

VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header
Version 1.01  Date: 05/12/03  By: Tom Lightfoot
    - Reduced likelihood of interrupt stack overflow on exit from ISR

******************************************************************************/

void scidrv_fnSciRx( uchar8 SciStatus, uchar8 SciData )
{
    uchar8 eStatus;    // Status enum to send to queue
    uint16 Message;    // Combined status/data word
    uchar8 *pMessage;  // Pointer to the bytes of the status/data word

    /*
        In order to save space, we'll use the pointers in the queue
        for data rather than just a pointer to data somewhere else.
        Data pointers are two-bytes wide and the first byte will encode
        the status of the data byte and the second byte will be the data
        byte itself.
    */

    // Decode the status of the received bit
    // Is this a break character?
    if (( SciStatus & SCISR1_FE ) && ( SciData == 0x00 ))
    {
        eStatus = SCIDRV_eBREAK;
    }
    else if ( SciStatus & ( SCISR1_OR + SCISR1_NF + SCISR1_FE + SCISR1_PF ) )
    {
        eStatus = SCIDRV_eERROR;
    }
    else
    {
        eStatus = SCIDRV_eOK;
    }

    // Create the combined status/data word
    pMessage = (uchar8 *)&Message;
    *pMessage++ = eStatus;
    *pMessage = SciData;

    // Write the status and data to the receive queue
    (void)OSQPost( scidrv_pzRxQueue, (void *)Message );
    
    // Turn off interrupts for the remainder of this ISR
    DEVICE_mGLOBAL_INT_DIS();

    /*
        At this point the queue may be full but there's not much
        we can do about it.  We can't make an ISR wait until
        a task reads another byte out of the queue.
    */
}

/******************************************************************************

FUNCTION NAME:
    scidrv_fnISR

PURPOSE:
    This function constitutes the main interrupt routine for
    the SCI interrupt.  It determines the specific cause of the
    interrupt and calls the appropriate function (recieve or transmit).

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:
    This is a separate function from SCI0_ISR because local variables on
    the stack would interfere with the stack calculations
    made in OSIntEnterExt

VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header

Version 1.10    Date: Jan 23,2004   By: Fred Pang
    - Added code to do ISR timing check

******************************************************************************/

void scidrv_fnISR( void )
{
    uchar8 SciStatus;    // A holder for the status because we should only
                         // read the register once.
    uchar8 SciData;      // Data byte read from the SCI port

    // Read the SCI status byte
    SciStatus = SCI0SR1;

    // Find out what caused the interrupt
    if ( SciStatus & ( SCISR1_RDRF + SCISR1_OR ))
    {
        // Receive or overrun interrupt

        // Read the data register to clear this bit.
        SciData = SCI0DRL;

#ifndef SCIDRV_IRQ_TIME_CHECK
        // Now interrupts can be enabled
        DEVICE_mGLOBAL_INT_EN();
#endif  // SCIDRV_IRQ_TIME_CHECK

        // Handle the status and data byte with the special function
        scidrv_fnSciRx( SciStatus, SciData );
    }
    // Is the transmit register empty with the interrupt enabled?
    else if (( SciStatus & SCISR1_TDRE ) && SCI0CR2_SCTIE )
    {
        // Prevent re-triggering of the interrupt
        SCI0CR2_SCTIE = 0;

#ifndef SCIDRV_IRQ_TIME_CHECK
        // Re-enable interrupts in order to use OS services
        DEVICE_mGLOBAL_INT_EN();
#endif  // #ifndef SCIDRV_IRQ_TIME_CHECK

        // Transmit
        scidrv_fnSciTx();
    }
    else if (( SciStatus & SCISR1_IDLE ) && SCI0CR2_ILIE )
    {
        // Idle line interrupt, shouldn't be enabled but read the
        // data register to clear this bit
        SciData = SCI0DRL;
    }
}

#pragma TRAP_PROC
/******************************************************************************

FUNCTION NAME:
    SCI0_ISR

PURPOSE:
    This is the interrupt service routine for SCI port 0.
    The interrupt can be due to several different reasons and
    this routine has to determine the cause for the
    interrupt and call the appropriate receive or transmit function.

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:
    This driver must cooperate with the uC/OS-II RTOS so some extra
    processing is required at the beginning and end of the ISR to
    deal with interrupt nesting and the saving of the stack pointer.

VERSION HISTORY:

Version 1.00  Date: 02/03/03  By: code_fix.py
    - Added version history to function header

Version 1.10    Date: Jan 23,2004   By: Fred Pang
    - Added code to do ISR timing check

******************************************************************************/

void SCI0_ISR( void )
{
    // Indicate to the operating system that we're now in an interrupt
    OSIntEnterExt();

#ifdef  SCIDRV_IRQ_TIME_CHECK
    DEVICE_mGLOBAL_INT_DIS();    
    DEBUG_mPIN0_SET();    
#endif  // SCIDRV_IRQ_TIME_CHECK
    
    // Run the main ISR code (it has local variables so it must be
    // in a separate function so that the OS stack calculations don't
    // go awry.
    scidrv_fnISR();
    
#ifdef  SCIDRV_IRQ_TIME_CHECK
    DEBUG_mPIN0_CLR();
    DEVICE_mGLOBAL_INT_EN();
#endif  // SCIDRV_IRQ_TIME_CHECK

    // Exit the interrupt service routine
    OSIntExitExt();
}

#pragma CODE_SEG DEFAULT
