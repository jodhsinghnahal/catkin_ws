/*==============================================================================
Copyright © 2006 Xantrex International

This file is the property of Xantrex International and shall not be reproduced, 
copied, or used as the basis for the manufacture or sale of equipment without 
the express written permission of Xantrex International.

FILE NAME:  
    iicdrv.h
                        
PURPOSE:
    Provide public access to the IIC (I2C) driver

FUNCTION(S):

NOTES:

HISTORY:
$History: iicdrv.h $

==============================================================================*/
#ifndef IICDRV_INCL
#define IICDRV_INCL

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"	//Requres and architecture defintion in the compiler
#include "LIB_stddefs.h"    //Standard defintions for all projects
#include "HALErrBase.h"

 
/*=============================================================================
                              Structures
==============================================================================*/
// Structure that contains information when doing a simple
// read or write to an IIC slave
typedef struct IICDRV_zDATA
{
    uchar8 ucSlaveAddress;
    uchar8 ucDataLength;
    uchar8 *pucDataBuffer;
} IICDRV_tzDATA;

// Structure that contains information when doing a write, followed by
// a read from an IIC slave
typedef struct IICDRV_zTXRXDATA
{
    uchar8 ucSlaveAddress;
    uchar8 ucTxDataLength;
    uchar8 *pucTxDataBuffer;
    uchar8 ucRxDataLength;
    uchar8 *pucRxDataBuffer;
} IICDRV_tzTXRXDATA;

/*=============================================================================
                              Typedefs
==============================================================================*/

// typdef for function pointer for user defined wait and signal functions
typedef void      (*tpfnSIGNALCALLBK)( void );
typedef tuiSTATUS (*tpfnWAITCALLBK)( void );


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
// IIC constructor
tuiSTATUS IICDRV_fnCtor( void );

// IIC destructor
void IICDRV_fnDtor( void );

// Write data buffer into IIC slave
tuiSTATUS IICDRV_fnWriteData( IICDRV_tzDATA *ptzWriteData ); 

// Read data buffer from IIC slave
tuiSTATUS IICDRV_fnReadData( IICDRV_tzDATA *ptzReadData );  

// Write data to IIC slave and then read data from it without releasing bus
// in between.
tuiSTATUS IICDRV_fnWriteReadData( IICDRV_tzTXRXDATA *ptzWriteReadData );

// Register callback functions
tuiSTATUS IICDRV_fnSetCallbacks( tpfnSIGNALCALLBK fnNotifyFunction, // function that notifies
                            tpfnWAITCALLBK fnWaitFunction ); // function that waits

// Acquire exclusive access to I2C                             
tuiSTATUS IICDRV_fnAcquireLock( sint16 siWaitTicks );

// Release exclusive access to I2C
void IICDRV_fnReleaseLock( void );


#endif  // IICDRV_INCL

