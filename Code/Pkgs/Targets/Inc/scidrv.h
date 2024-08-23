/*===========================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by
copyright. The reproduction, in whole or in part, by anyone without the
written approval of Xantrex is prohibited.

FILE NAME:
    scidrv.h

PURPOSE:
    Provide public access to a Serial Communications Interface driver

FUNCTION(S):
    SCIDRV_fnCtor            - Initialize SCI port 0
    SCIDRV_fnDtor            - Quit using the serial port
    SCIDRV_fnEnTransmitter   - Enable the SCI transmitter
    SCIDRV_fnDisTransmitter  - Disable the SCI transmitter
    SCIDRV_fnEnReceiver      - Enable the SCI receiver
    SCIDRV_fnDisReceiver     - Disable the SCI receiver
    SCIDRV_fnSendBreak       - send a break on the serial port
    SCIDRV_fnSendByte        - Send a byte to the serial port
    SCIDRV_fnReceiveByte     - Receive a serial byte and the status of the byte
    SCIDRV_fnSetTimeout      - Set the timeout for reception of a serial byte
    SCIDRV_fnGetTimeout      - Retrieve SCI timeout value
    SCIDRV_fnTxIsr  - Transmission ISR
    SCIDRV_fnRxIsr  - Receiving ISR

NOTES:

USAGE:
    1.  The constructor must be called to initialize the serial port
        Example call 115200 baud, N, 8, 1
        SCIDRV_fnCtor( SCIDRV_eBAUD_115200,
                       SCIDRV_ePARITY_NONE,
                       SCIDRV_eDATA_BITS_8,
                       SCIDRV_eSTOP_BITS_1
                      );

    2.  To set the receiver timeout use the function SCIDRV_fnSetTimeout.
        A value of 0 sent by this function will disable the receiver timeout.

    3.  To send a byte out the serial port use the function
        SCIDRV_fnSendByte.

    4.  To read a byte from the serial port use the function
        SCIDRV_fnReceiveByte.

CHANGE HISTORY:
$Log: scidrv.h $

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/1
    User: HorstM     Date: 02/22/05  Time: 11:23PM
    renamed isr's

    

    ***********************************************
    Revision: NovaPfrmB_JohnB/1
    User: JohnB     Date: 02/03/05  Time: 10:34PM
    Removed "extern" SCICLIENT functions as per code review.
    
    

    *****************  Version 1  *****************
    User: Hollyz       Date: 11/18/04   Time: 1:18p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Inc
    Created

==============================================================================*/
#ifndef SCIDRV_INCL
#define SCIDRV_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    // Requres and architecture defintion in the compiler
#include "LIB_stddefs.h"    // Standard defintions for all projects
#include "HALErrBase.h"

/*==============================================================================
                            Type Definitions
==============================================================================*/
// None

/*==============================================================================
                                Enums
==============================================================================*/
// errno codes based on errbase.h
// Note:  errno is an ANSI standard and can be
// found in "The C Programming Language" by K & R for reference.
typedef enum SCIDRV_eERRNO
{
    SCIDRV_eERRNO_CTOR = ERR_SCI_DRV, // cannot construct the SCI driver
    SCIDRV_eERRNO_DTOR,
    SCIDRV_eERRNO_NO_INSTANCE,        // the SCI driver was not instantiated
    SCIDRV_eERRNO_BUFF_OVERRUN        // buffer overrun

} SCIDRV_teERRNO;

// Receiver return status
typedef enum SCIDRV_eSTATUS
{
    SCIDRV_eOK = 0,                   // the character is OK
    SCIDRV_eBREAK,                    // break condition
    SCIDRV_eERROR,                    // frame, parity or overrun error
    SCIDRV_eERROR_FRAME,              // frame error
    SCIDRV_eERROR_PO,                 // parity or overrun error
    SCIDRV_eTIMEOUT,                  // receive timeout
    SCIDRV_eINVALID                   // none of the above
} SCIDRV_teSTATUS;

// enum for the baud rate selection
typedef enum SCIDRV_eBAUD_RATE
{
    SCIDRV_eBAUD_INVL   = 0,
    SCIDRV_eBAUD_2400   = 24,
    SCIDRV_eBAUD_4800   = 48,
    SCIDRV_eBAUD_9600   = 96,
    SCIDRV_eBAUD_19200  = 192,
    SCIDRV_eBAUD_38400  = 384,
    SCIDRV_eBAUD_57600  = 576,
    SCIDRV_eBAUD_115200 = 1152

} SCIDRV_teBAUD_RATE;

// enum for the number of parity bits
typedef enum SCIDRV_ePARITY
{
    SCIDRV_ePARITY_INVL = 0,
    SCIDRV_ePARITY_NONE,
    SCIDRV_ePARITY_EVEN,
    SCIDRV_ePARITY_ODD

} SCIDRV_tePARITY;

// enum for the number of stop bits
typedef enum SCIDRV_eSTOP_BITS
{
    SCIDRV_eSTOP_BITS_INVL = 0,
    SCIDRV_eSTOP_BITS_1,
    SCIDRV_eSTOP_BITS_2

} SCIDRV_teSTOP_BITS;

// enum for the number of data bits
typedef enum SCIDRV_eDATA_BITS
{
    SCIDRV_eDATA_BITS_INVL = 0,
    SCIDRV_eDATA_BITS_1,
    SCIDRV_eDATA_BITS_2,
    SCIDRV_eDATA_BITS_3,
    SCIDRV_eDATA_BITS_4,
    SCIDRV_eDATA_BITS_5,
    SCIDRV_eDATA_BITS_6,
    SCIDRV_eDATA_BITS_7,
    SCIDRV_eDATA_BITS_8

} SCIDRV_teDATA_BITS;


/*==============================================================================
                              Defines
==============================================================================*/
// None

/*=============================================================================
                              Structures
==============================================================================*/
// None

/*==============================================================================
                          External/Public Constants
==============================================================================*/
// None

/*==============================================================================
                          External/Public Variables
==============================================================================*/
// None

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
// SCI constructor
// NOTE: No other SCI functions can be called unless the constructor is called
tuiSTATUS SCIDRV_fnCtor( SCIDRV_teBAUD_RATE BaudRate,
                         SCIDRV_tePARITY    Parity,
                         SCIDRV_teDATA_BITS DataBits,
                         SCIDRV_teSTOP_BITS StopBits
                        );

// SCI destructor
// No other SCI functions can be called after the destructor is called
tuiSTATUS SCIDRV_fnDtor( void );  // SCI desructor

// Enable the SCI transmitter
void SCIDRV_fnEnTransmitter( void );

// Disable the SCI transmitter
void SCIDRV_fnDisTransmitter( void );

// Enable the SCI receiver
void SCIDRV_fnEnReceiver( void );

// Disable the SCI receiver
void SCIDRV_fnDisReceiver( void );

// Send a byte to the serial port
void SCIDRV_fnSendByte( uchar8 SerialData );

// Set the timeout for reception of a serial byte
void SCIDRV_fnSetTimeout( uint16 Timeout );

// Get the value of the SCI driver timeout
uint16 SCIDRV_fnGetTimeout( void );

// Receive a serial byte and the status of the byte
SCIDRV_teSTATUS SCIDRV_fnReceiveByte( uchar8* RxData );

// SCI transmission interrupt serivce routine
void SCIDRV_fnTxIsr( void );

// SCI reception interrupt service routine
void SCIDRV_fnRxIsr( void );

// transmit a break on the serial port
void SCIDRV_fnSendBreak( void );

#endif  // SCIDRV_INCL

