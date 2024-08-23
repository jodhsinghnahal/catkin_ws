/*=============================================================================
Copyright © 2006 Xantrex International

This file is the property of Xantrex International and shall not be reproduced, 
copied, or used as the basis for the manufacture or sale of equipment without 
the express written permission of Xantrex International.

FILE NAME:  iicdrv.c

PURPOSE:
    Device driver for the I2C Controller on the TIDSP F280X. 
    Documentation from TI is:
    
    "TMS320x280x Inter-Integrated Circuit (I2C) Module Reference Guide (SPRU721A)
        November 2004 - Revised October 2005"

FUNCTION(S):
    IICDRV_fnCtor		    - Initialize IIC Controller
    IICDRV_fnDtor           - Destructor for IIC Controller
    IICDRV_fnEnInt          - Enable IIC interrupts
	IICDRV_fnReadData		- Read data  from IIC slave
	IICDRV_fnWriteData		- Write data into IIC slave
	IICDRV_fnWriteReadData	- Write and then read data from the IIC slave
    IICDRV_ISR              - Interrupt Service Routine for IIC
    IICDRV_fnEnInt          - Enable IIC interrupts
    IICDRV_fnAcquireLock    - Acquire exclusive access to the I2C module
    IICDRV_fnReleaseLock    - Release exclusive access to the I2C module

    local:
         iicdrv_fnISR		 - Main body of code for IIC Interrupt
         iicdrv_fnCheckBusy  - Ensures IIC module is free
         iicdrv_fnIOComplete - Signal that IO has completed
         iicdrv_fnSignalStartIO - Indicate that IO operation is starting
         iicdrv_fnWaitForIOComplete  - Wait for an IO operation to complete

NOTES:
    This driver only supports a single-master system. It supports a multi-slave
    system.
        
    The application calling these functions
    must supply the buffer and the length of the data to be read or written and 
    also the address of the slave device.

    INITIALIZATION SEQUENCE:
        IICDRV_fnCtor()
        IICDRV_fnEnInt()

    MUTUAL EXCLUSION SEMAPHORE
    ==========================
    This driver only allows access to one task at a time as the module
    must complete an operation before it can start a new one. This driver
    provides semaphore services to applications to ensure exclusive access.

    This driver provides a flag (IICDRV_ACQ_REL_LOCK_ENABLE) to include the 
    services of the mutual exclusion semaphore if more than one application
    will be using this driver. Setting this flag to TRUE will make use
    of the semaphore.

    To read or write data:
        IICDRV_fnAcquireLock() 
        IICDRV_fnWriteData() OR IICDRV_fnReadData() OR IICDRV_fnWriteReadData()
        IICDRV_fnReleaseLock() 

    When any of the driver Read/Write functions are called, the application
    is stalled until the operation completes. This is accomplish with the
    use of a semaphore to signal the application when the operation completes.
    This function is invisible to the application and is part of the 
    driver.
        
CHANGE HISTORY:
$Log: iicdrv.c $

=============================================================================*/

/*=============================================================================
                                 Include files
=============================================================================*/

#include <errno.h>          // driver level error codes
#include <limits.h>         // for CHAR_BIT
#include <stdlib.h>         // for malloc
#include <string.h>         // for memset
#include "iicdrv.h"         // IIC driver defines
#include "includes.h"       // RTOS
#include "Device.h"         // TI device headers
#include "devicemacro.h"    // macros to enable PIE
#include "sysclk.h"         // for clock speed calcs
#include "xassert.h"        // allow the use of assert( n )


/*==============================================================================
                              Function Enable Switches
==============================================================================*/

// include code to acquire and release the LOCK for the IIC
#ifndef IICDRV_ACQ_REL_LOCK_ENABLE
#define IICDRV_ACQ_REL_LOCK_ENABLE      ( TRUE )
#endif


/*=============================================================================
                                local prototypes
=============================================================================*/

// function to ensure iic module is free
static tuiSTATUS iicdrv_fnCheckBusy( void );

// signal the start of IO
static tuiSTATUS iicdrv_fnSignalStartIO( void );

// wait for IO to complete
static tuiSTATUS iicdrv_fnWaitForIOComplete( void );

// signal that IO is complete
static void iicdrv_fnIOComplete( void );

/*==============================================================================
                        External Function Protoypes
==============================================================================*/
extern void ISR_fnI2C1AINT( void );

/*=============================================================================
                              Internal Definitions
=============================================================================*/

#define IICDRV_MAX_TIMEOUT          ( 0xFFFF )    // maximum timeout value

#define IICDRV_MODULE_CLOCK         ( 10000000 )  // 7-12MHz for DSP

// Currently the only slave device on this bus is TI's bq27200 Battery Gas Gauge
// The clock requirement is for:
//      min TwH = 4 us
//      min TwL = 4.7 us
// For simplicity, we will set both durations to be equal.
// JTTEST should set this to 5 if circuitry can accommodate. Right now
// the rise time is too high.
#define IICDRV_CLOCK_HALF_PERIOD    ( 20 ) // 5us
/*=============================================================================
	                              Macros
=============================================================================*/

// Reset Module
#define IICDRV_mMODULE_RESET() \
            ( I2caRegs.I2CMDR.all = \
                   (  F280X_mCFGMSK( I2CMDR, BC,      EQU_8     ) \
                    | F280X_mCFGMSK( I2CMDR, FDF,     DIS       ) \
                    | F280X_mCFGMSK( I2CMDR, STB,     DIS       ) \
                    | F280X_mCFGMSK( I2CMDR, IRS,     DIS       ) \
                    | F280X_mCFGMSK( I2CMDR, DLB,     DIS       ) \
                    | F280X_mCFGMSK( I2CMDR, RM,      DIS       ) \
                    | F280X_mCFGMSK( I2CMDR, XA,      SEVEN     ) \
                    | F280X_mCFGMSK( I2CMDR, TRX,     RX        ) \
                    | F280X_mCFGMSK( I2CMDR, MST,     SLAVE     ) \
                    | F280X_mCFGMSK( I2CMDR, STP,     DIS       ) \
                    | F280X_mCFGMSK( I2CMDR, STT,     DIS       ) \
                    | F280X_mCFGMSK( I2CMDR, FREE,    STOP      ) \
                    | F280X_mCFGMSK( I2CMDR, NACKMOD, DIS       ) ) )


#define IICDRV_mMODULE_EN() \
                    ( I2caRegs.I2CMDR.bit.IRS = F280X_I2CMDR_IRS_ENA )


// Generate stop condition
#define IICDRV_mGEN_STOP_COND() \
                    ( I2caRegs.I2CMDR.bit.STP = F280X_I2CMDR_STP_ENA )

// Set up non-repeat transmit with STOP (RM = 0, STT = 1, STP = 1, TRX = 1)
#define IICDRV_mTX_STP() \
                    ( I2caRegs.I2CMDR.all |= \
                                ( F280X_mCFGMSK( I2CMDR, STP, ENA ) \
                                | F280X_mCFGMSK( I2CMDR, STT, ENA ) \
                                | F280X_mCFGMSK( I2CMDR, MST, MSTR ) \
                                | F280X_mCFGMSK( I2CMDR, TRX, TX  ) ) )
                                
//                                | F280X_mCFGMSK( I2CMDR, MST, MSTR ) \ 

// Set up non-repeat transmit without STOP (RM = 0, STT = 1, STP = 0, TRX = 1)
#define IICDRV_mTX() \
                     { I2caRegs.I2CMDR.bit.STP = F280X_I2CMDR_STP_DIS; \
                       I2caRegs.I2CMDR.all |= \
                                ( F280X_mCFGMSK( I2CMDR, STT, ENA ) \
                                | F280X_mCFGMSK( I2CMDR, MST, MSTR ) \
                                | F280X_mCFGMSK( I2CMDR, TRX, TX  ) ); \
                     }

                                

// Set up non-repeat receive (RM = 0, STT = 1, STP = 1, TRX = 0)
#define IICDRV_mRX() \
                    { I2caRegs.I2CMDR.bit.TRX = F280X_I2CMDR_TRX_RX; \
                      I2caRegs.I2CMDR.all |= \
                                ( F280X_mCFGMSK( I2CMDR, STP, ENA ) \
                                | F280X_mCFGMSK( I2CMDR, MST, MSTR ) \
                                | F280X_mCFGMSK( I2CMDR, STT, ENA )); \
                    }

// Set up receive mode                                    
#define IICDRV_mSETUPRX() \
                    ( I2caRegs.I2CMDR.all &= F280X_mCFGMSK( I2CMDR, TRX, RX ))

// Disable all interrupts in module
#define IICDRV_mMODULE_INT_DIS() \
                    ( I2caRegs.I2CIER.all = 0 )

// Enable/Disable Receive Ready interrupt
#define IICDRV_mRX_INT_EN() \
                    ( I2caRegs.I2CIER.all |= ( F280X_mCFGMSK( I2CIER, RRDY, ENA) \
                                           | F280X_mCFGMSK( I2CIER, NACK, ENA   ) \
                                           | F280X_mCFGMSK( I2CIER, SCD,  ENA )))
#define IICDRV_mRX_INT_DIS() \
                    ( I2caRegs.I2CIER.all &= ~( F280X_mCFGMSK( I2CIER, RRDY, ENA) \
                                           | F280X_mCFGMSK( I2CIER, NACK, ENA   ) \
                                           | F280X_mCFGMSK( I2CIER, SCD, ENA )))
    
// Enable/Disable Transmit Ready and SCD interrupts
#define IICDRV_mTX_INT_EN()  \
                    ( I2caRegs.I2CIER.all |=  ( F280X_mCFGMSK( I2CIER, XRDY, ENA) \
                                           | F280X_mCFGMSK( I2CIER, NACK, ENA   ) \
                                           | F280X_mCFGMSK( I2CIER, SCD, ENA )))
#define IICDRV_mTX_INT_DIS() \
                    ( I2caRegs.I2CIER.all &= ~(  F280X_mCFGMSK( I2CIER, XRDY, ENA) \
                                           | F280X_mCFGMSK( I2CIER, NACK, ENA   ) \
                                           | F280X_mCFGMSK( I2CIER, SCD, ENA )))
    
// Enable/Disable Register Access Ready interrupt
#define IICDRV_mACCESS_INT_EN() \
                    ( I2caRegs.I2CIER.bit.ARDY = F280X_I2CIER_ARDY_ENA )
                    
#define IICDRV_mACCESS_INT_DIS() \
                    ( I2caRegs.I2CIER.bit.ARDY = F280X_I2CIER_ARDY_DIS )
 
// PIE interrupt acknowledge
#define IICDRV_mPIE_ACK()\
                    ( PieCtrlRegs.PIEACK.bit.ACK8 = 1 )

// Read Interrupt source register
#define IICDRV_mINTSRC() \
                    ( I2caRegs.I2CISRC.bit.INTCODE )

// Is bus busy?
#define IICDRV_mBUSY() \
                    ( F280X_I2CSTR_BB_BUSY == I2caRegs.I2CSTR.bit.BB )

// Reset the module to try to recover when an error is detected        
#define IICDRV_mMODULE_RECOVER() \
        {   IICDRV_mMODULE_INT_DIS(); \
            IICDRV_mMODULE_RESET(); \
            IICDRV_mMODULE_EN(); \
        }

// Set slave address. Slave address must be shifted right by 1 since
// bits 0-6 are used.
// Most device specs give slave address 0-7 with bit 0 being stuffed with 0 (W)
// or 1(R)
#define IICDRV_mSET_SLAVEADDR( Address ) \
                    ( I2caRegs.I2CSAR = ( Address >> 1 ))

/*=============================================================================
                                   Local Data
=============================================================================*/
// Use the RTOS semaphore to signal completion of IO operation
OS_EVENT *ptzSignalSem;

#if ( IICDRV_ACQ_REL_LOCK_ENABLE == TRUE )
// Use the RTOS semaphore to restrict access to the IIC to one user at a time
OS_EVENT *ptzMutexSem;
#endif

static volatile tucBOOL tucLineBusy;  // busy flag for default wait/signal functions

static tucBOOL tucIOStatus;        // Status of IO request

static uchar8 *pucTxBufferStart;   // pointer to current position in tx buffer
static uchar8 *pucTxBufferEnd;     // pointer to the end of the tx buffer

static uchar8 *pucRxBufferStart;   // pointer to current position in rx buffer
static uchar8 *pucRxBufferEnd;     // pointer to the end of the rx buffer

/*==============================================================================
                           Function Definitions
==============================================================================*/

/******************************************************************************

FUNCTION NAME:
    IICDRV_fnCtor

PURPOSE:
    This function initializes the I2C module

INPUTS:

OUTPUTS:
    eSTATUS_OK if successful.

NOTES:
    This function must be called before calling any other functions in this
    module

VERSION HISTORY:

Version 1.00  Date: 20-Jun-2006  By: Josephine Tsen
       - Created
       		  
******************************************************************************/
tuiSTATUS IICDRV_fnCtor( void )
{
    uint16 count;
    uint16 d;

    // spin off any bits pending from the slave devices, this has no impact on
    // the normal run-time, but will make the system more robust for debugging
    // Generate at least enough clock cycles for the longest application data
    // While SCLA is a GPIO line, toggle it so that it looks like a valid clock
    // for at least 3 bytes
    // set up pin 33 (GPIO/SCLA pin) to be an I/O pin
    DEVICE_mACCESS_EN();
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;
    
    // configure the direction of pin to be an output
    GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;
    
    // configure the default for the pin to be low
    GpioDataRegs.GPBDAT.bit.GPIO33 = 1;
    DEVICE_mACCESS_DIS();
    
	// Clock out all data from previous access to slave in case
    // slave is stuck in previous access.
    for( count = 0; count < (4*8); count++ )
    {
        // delay for 10 us
        DEVICE_mDELAY_US( 10 );

        // Toggle GPIO 33 (SCLA)
        GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1;
    }

    // Enable the I2C module clocks
    DEVICE_mACCESS_EN( );    
    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = F280X_PCLKCR0_I2CAENCLK_ENA;
    DEVICE_mACCESS_DIS( );   

    // Initialize interrupt vectors
    DEVICE_mACCESS_EN();
    PieVectTable.I2CINT1A = &ISR_fnI2C1AINT;  
    DEVICE_mACCESS_DIS();
    
    // Put the I2C module into reset so that registers can be configured 
    IICDRV_mMODULE_RESET();
    
    // Set up module clock
    I2caRegs.I2CPSC.bit.IPSC = ( SYSCLK_fnGetSysClk() / IICDRV_MODULE_CLOCK ) - 1;

    // Set up master clock output on the SCL line
    //
    // Page 29 of reference guide for I2C module states that:
    // 
    // Clock High-Time duration ( TwH) = Tmod x (ICCH + d)
    // Clock Low-Time duration  ( TwL) = Tmod x (ICCL + d)
    //
    // Therefore ICCH = (TwH / Tmod) - d
    //  
    // Tmod = 1 / ( IIC_MODULE_CLOCK)
    //
    // ICCH = (TwH * IIC_MODULE_CLOCK) - d
    // ICCL = (TwL * IIC_MODULE_CLOCK) - d
    //
    // For simplicity, we will set both durations to be equal.
    //
    // ICCH = ICCL = ( IIC_CLOCK_HALF_PERIOD * IIC_MODULE_CLOCK ) - d
    //
    // the value of d depends on IPSC:
    //
    //      IPSC    d
    //      ---------
    //      0       7
    //      1       6
    //      >1      5
    //
    // 
    if( 0 == I2caRegs.I2CPSC.bit.IPSC )
    {
        d = 7;
    }
    else if( 1 == I2caRegs.I2CPSC.bit.IPSC )
    {
        d = 6;
    }
    else
    {
        d = 5;
    }

    I2caRegs.I2CCLKH = ( IICDRV_CLOCK_HALF_PERIOD * (IICDRV_MODULE_CLOCK / 1000000 )) - d;
    I2caRegs.I2CCLKL = I2caRegs.I2CCLKH;

    // Disable Transmit and Receive FIFOs
    I2caRegs.I2CFFTX.all = 
                        ( F280X_mCFGMSK( I2CFFTX, I2CFFEN,  DIS )
                         |F280X_mCFGMSK( I2CFFTX, TXFFRST,  RES )
                         |F280X_mCFGMSK( I2CFFTX, TXFFIENA, DIS ) );

    I2caRegs.I2CFFRX.all = 
                        ( F280X_mCFGMSK( I2CFFRX, RXFFRST,  RES )
                         |F280X_mCFGMSK( I2CFFRX, RXFFIENA, DIS ) );

    // Module's own slave address
    I2caRegs.I2COAR = 0; 

    // Disable all interrupts
    I2caRegs.I2CIER.all = 
                        ( F280X_mCFGMSK( I2CIER, AL,   DIS )
                         |F280X_mCFGMSK( I2CIER, NACK, DIS )
                         |F280X_mCFGMSK( I2CIER, ARDY, DIS )
                         |F280X_mCFGMSK( I2CIER, RRDY, DIS )
                         |F280X_mCFGMSK( I2CIER, XRDY, DIS )
                         |F280X_mCFGMSK( I2CIER, SCD,  DIS )
                         |F280X_mCFGMSK( I2CIER, AAS,  DIS ) ); 

    // Clear Interrupt source register by reading it
    d = IICDRV_mINTSRC() ;

    // Take module out of reset
    IICDRV_mMODULE_EN();
    
    // Configure SCLA and SDAA pins.
    DEVICE_mACCESS_EN();
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;
    DEVICE_mACCESS_DIS();

    // Generate STOP condition to ensure the I2C module clears the Busy bit
    //IICDRV_mGEN_STOP_COND();
    
    // Ensure the Busy Bit is cleared
    if( eSTATUS_OK != iicdrv_fnCheckBusy() )
    {
        return( eSTATUS_ERR );
    }
    
    tucLineBusy = TRUE;                     // set the default wait status to wait
    tucIOStatus = eSTATUS_OK;               // set default IO Status
    pucTxBufferStart = pucTxBufferEnd = NULL;  
    pucRxBufferStart = pucRxBufferEnd = NULL;  

    // create a binary semaphore
    ptzSignalSem = OSSemCreate( 1 );

    // Error creating semaphore
    if( ptzSignalSem == NULL )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }

#if ( IICDRV_ACQ_REL_LOCK_ENABLE == TRUE )
    // create a binary semaphore
    ptzMutexSem = OSSemCreate( 1 );

    // Error creating semaphore
    if( ptzMutexSem == NULL )
    {
        assert( 0 );
        return( eSTATUS_ERR );
    }
#endif
    
    return ( eSTATUS_OK );
}

/******************************************************************************

FUNCTION NAME:
    IIC_fnDtor

PURPOSE:
    This function acts as a destructor for the IIC module

INPUTS:
    none
    
OUTPUTS:
    none

NOTES:
    

VERSION HISTORY:

Version 1.00  Date: 20-Jun-2006  By: Josephine Tsen
       - Created
******************************************************************************/
#ifdef GLBL_DTOR_EN
void IICDRV_fnDtor( void )
{

    // disable module
	IICDRV_mMODULE_RESET(); 

#if( OS_SEM_DEL_EN > 0 )    
    uchar8  RetCode;

    // delete the semaphore even if there are tasks waiting for it
    OSSemDel( ptzSignalSem,
              OS_DEL_ALWAYS,
              &RetCode );
#if( IICDRV_ACQ_REL_LOCK_ENABLE == TRUE )
    // delete the semaphore even if there are tasks waiting for it
    OSSemDel( ptzMutexSem,
              OS_DEL_ALWAYS,
              &RetCode );

#endif

#endif
	

}
#endif

/*******************************************************************************

FUNCTION NAME:
    IICRV_fnEnInt

PURPOSE:
    Enable the IIC interrupts (the PIE interrupts for IIC module)

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: 22-Jun-2006  By: Josephine Tsen
    - Created.

*******************************************************************************/
void IICDRV_fnEnInt( void )
{

    // INTx1 should be correct. Reference in Sys control doc Table 6-4 is diffferent
    // from Table 6-5. I2C doc is the same as Table 6-5s
    PieCtrlRegs.PIEIER8.bit.INTx1 = 1; 
            
    // Acknowledge PIE Interrupt
    IICDRV_mPIE_ACK();

    // Enable PIE interrupt
    DEVICE_mINT8_EN();

} 

/******************************************************************************

FUNCTION NAME:
    IICDRV_fnReadData

PURPOSE:
    This function reads data from the IIC bus.

INPUTS:
    IICDRV_tzData tzReadData - encasulates the calling address,
                             the data length, and the buffer
                             for which to put the data in

OUTPUTS:
    eSTATUS_OK if successful

NOTES:
    This waits for the module to become free, and then generates a start
    condition.  It transmits the calling address, and then waits until the rest 
    of the data has been read.  The wait function can be user defined by setting
    the SetNotify function, or it can use a default polling method.  The polling
    method may waste CPU cycles if it is a high priority task.  
    
    
Version 1.00  Date: 20-Jun-2006  By: Josephine Tsen
       - Created
		  
******************************************************************************/

tuiSTATUS IICDRV_fnReadData( IICDRV_tzDATA *ptzReadData )
{					  
	tuiSTATUS Err;       // error checker

    assert( ptzReadData );
    assert( ptzReadData->pucDataBuffer );
                      
    // Check if bus is busy
	if( eSTATUS_OK != iicdrv_fnCheckBusy())
	{
	    return ( eSTATUS_ERR );
	}

    // Get IO signal semaphore
	if( eSTATUS_OK != iicdrv_fnSignalStartIO())
	{
	    return( eSTATUS_ERR );
	}
	
    // set the start of the buffer
    pucRxBufferStart = ptzReadData->pucDataBuffer;

    // set the end so we know when to stop
    pucRxBufferEnd = ptzReadData->pucDataBuffer + 
                     ptzReadData->ucDataLength;
	
    // Set up slave address
    IICDRV_mSET_SLAVEADDR( ptzReadData->ucSlaveAddress );

    // Set up number of bytes to read
    I2caRegs.I2CCNT = ptzReadData->ucDataLength;

    // Enable receive interrupt
    IICDRV_mRX_INT_EN();

    // Set up receive (RM = 0, STT = 1, STP = 1, TRX = 0)
    IICDRV_mRX();
 
	// wait until all bytes have shifted out
	Err = iicdrv_fnWaitForIOComplete();

    // Disable receive interrupt
    IICDRV_mRX_INT_DIS();
    
	return( Err );
	

}

/******************************************************************************

FUNCTION NAME:
    IICDRV_fnWriteData

PURPOSE:
    This function writes data byte to the IIC bus.

    Occasionally a write request to a device may fail if the device is 
    busy with the previous writes. Therefore, if a back-to-back write
    request to the same device fails, insert a short delay and try again.
    Look at the data sheet for the specific device to determine its 
    maximum write time.

INPUTS:
    IICDRV_tzDATA tzWriteData - encasulates the calling address,
                             the data length, and the buffer
                             for which to put the data in
    
OUTPUTS:
    eSTATUS_OK if successful

NOTES:
    none
    
VERSION HISTORY:

Version 1.00  Date: 20-Jun-2006  By: Josephine Tsen
       - Created
		  
******************************************************************************/

tuiSTATUS IICDRV_fnWriteData( IICDRV_tzDATA *ptzWriteData )
{					  
    tuiSTATUS Err;     // error status

    assert( ptzWriteData );
    assert( ptzWriteData->pucDataBuffer );
    
    // Check if bus is busy
	Err = iicdrv_fnCheckBusy();

	// if timed out
	if( Err != eSTATUS_OK )
	{
	    return ( eSTATUS_ERR );
	}

    // Get IO signal semaphore
	if( eSTATUS_OK != iicdrv_fnSignalStartIO())
	{
	    return( eSTATUS_ERR );
	}
		
    // set the start of the tx buffer
    pucTxBufferStart = ptzWriteData->pucDataBuffer;

    // set the end so we know when to stop
    pucTxBufferEnd = ptzWriteData->pucDataBuffer + 
                     ptzWriteData->ucDataLength;
                                        
	
    // Set up slave address
    IICDRV_mSET_SLAVEADDR( ptzWriteData->ucSlaveAddress );

    // Set up number of bytes to write
    I2caRegs.I2CCNT = ptzWriteData->ucDataLength;

    // Write first byte to transmit data register
    I2caRegs.I2CDXR = *pucTxBufferStart++;
    
    // Enable Transmit and NACK interrupt
    IICDRV_mTX_INT_EN();
    
    // Set up non-repeat transmit (RM = 0, STT = 1, STP = 1, TRX = 1)
    IICDRV_mTX_STP();
    
	// wait until all bytes have shifted out
    Err = iicdrv_fnWaitForIOComplete();
    
    // Disable TX and NACK interrupts
    IICDRV_mTX_INT_DIS();

    return( Err );
}

/******************************************************************************

FUNCTION NAME:
    IICDRV_fnWriteReadData

PURPOSE:
    This function writes data byte to the IIC bus and then reads from it 
    without releasing the bus in between.

INPUTS:
    tzData - encasulates the calling address,
                             the data length, and the buffer
                             for which to put the data in
    
OUTPUTS:
    eSTATUS_OK if successful

NOTES:
    The function enables the IIC, and the IIC interrupt, and generates a
    start condition.  It then writes the calling address, letting the device
    know which address space to write data to.  It then waits until the ISR
    shifts the rest of the bytes out of the module.  The wait function can be 
    user defined by setting the SetNotify function, or it can use a default 
    polling method.  The polling method may waste CPU cycles if it is a high
    priority task
    
VERSION HISTORY:

Version 1.00  Date: 20-Jun-2006  By: Josephine Tsen
       - Created
		  
******************************************************************************/

tuiSTATUS IICDRV_fnWriteReadData( IICDRV_tzTXRXDATA *ptzWriteReadData )
{					  
    tuiSTATUS Err;     // error status

    assert( ptzWriteReadData );
    assert( ptzWriteReadData->pucTxDataBuffer );
    assert( ptzWriteReadData->pucRxDataBuffer );
    
    // Check if bus is busy
	Err = iicdrv_fnCheckBusy();

	// if timed out
	if( Err != eSTATUS_OK )
	{
	    return ( eSTATUS_ERR );
	}

    // Get IO signal semaphore
	if( eSTATUS_OK != iicdrv_fnSignalStartIO())
	{
	    return( eSTATUS_ERR );
	}
		
    // set the start of the buffer
    pucTxBufferStart = ptzWriteReadData->pucTxDataBuffer;

    // set the end so we know when to stop
    pucTxBufferEnd = ptzWriteReadData->pucTxDataBuffer + 
                     ptzWriteReadData->ucTxDataLength;
                   
    // set the start of the rx buffer
    pucRxBufferStart = ptzWriteReadData->pucRxDataBuffer;

    // set the end so we know when to stop
    pucRxBufferEnd = ptzWriteReadData->pucRxDataBuffer + 
                     ptzWriteReadData->ucRxDataLength;
                     	
    // Set up slave address
    IICDRV_mSET_SLAVEADDR( ptzWriteReadData->ucSlaveAddress );

    // Set up number of bytes to write
    I2caRegs.I2CCNT = ptzWriteReadData->ucTxDataLength;

    // Write first byte to transmit data register
    I2caRegs.I2CDXR = *pucTxBufferStart++;
    
    // Enable Transmit and NACK interrupt
    IICDRV_mTX_INT_EN();
    IICDRV_mACCESS_INT_EN();
    
    // Set up non-repeat transmit (RM = 0, STT = 1, STP = 0, TRX = 1)
    IICDRV_mTX();
    
	// wait until all bytes have shifted out
    Err = iicdrv_fnWaitForIOComplete();
    
    // Disable Transmit and Receive interrupts (Receive was enabled by ISR)
    IICDRV_mTX_INT_DIS();
    IICDRV_mRX_INT_DIS();
    IICDRV_mACCESS_INT_DIS();

    return( Err );
	
}

/*******************************************************************************

FUNCTION NAME: IICDRV_fnAcquireLock

PURPOSE: Acquire exclusive access to the I2C hardware

INPUTS:
    siWaitTicks : maximum number of ticks to wait for the IIC hardware
                  to become available. 

                  If the number of ticks is negative, this function will
                  try to acquire the lock without waiting.
                  
                  If the number of ticks is positive, this function will wait
                  this number of clock ticks for the lock.

                  If the number of ticks is zero
                  this function will wait forever for the lock.
                  
OUTPUTS:
    tuiSTATUS - eSTATUS_ERR if the lock could not be acquired
              - eSTATUS_OK if access to the SPI hardware has been granted

NOTES:
    none

VERSION HISTORY:

Version: 1.00  Date: 04-Jul-2006  By: Josephine Tsen
    - Created.

*******************************************************************************/
tuiSTATUS IICDRV_fnAcquireLock( sint16 siWaitTicks )
{
#if ( IICDRV_ACQ_REL_LOCK_ENABLE == TRUE )
    uchar8 RetCode;

    // Caller does not want to wait for lock
    if( siWaitTicks < 0 )
    {
        // try to get the semaphore without blocking
        if( OSSemAccept( ptzMutexSem ) == 0 )
            return( eSTATUS_ERR );
        else
            return( eSTATUS_OK );
    }

    else
    {
        // Wait for lock
        OSSemPend( ptzMutexSem,
               siWaitTicks,
               &RetCode );

        // If this func doesn't get the semaphore within uiWaitTicks it means that
        // the SPI hardware is still busy with another transaction
        if( RetCode != OS_NO_ERR )
        {
            assert( 0 );
            return( eSTATUS_ERR );
        }
    }
#endif 
    
    return( eSTATUS_OK );
} 


/*******************************************************************************

FUNCTION NAME: IICDRV_fnReleaseLock

PURPOSE: Release exclusive access to the I2C hardware

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    none

VERSION HISTORY:

Version: 1.00  Date: 04-Jul-2006  By: Josephine Tsen
    - Created.

*******************************************************************************/
void IICDRV_fnReleaseLock( void )
{
#if ( IICDRV_ACQ_REL_LOCK_ENABLE == TRUE )
    // post the semaphore so we know that the SPI hardware is available
    OSSemPost( ptzMutexSem );
#endif 
} 

/******************************************************************************

FUNCTION NAME:
    iicdrv_fnCheckBusy

PURPOSE:
    This checks to see if the iic module is busy, and times out if it does
    not become free soon.

INPUTS:
    none

OUTPUTS:
    eSTATUS_OK if successful

NOTES:
    
VERSION HISTORY:

Version 1.00  Date: 20-Jun-2006  By: Josephine Tsen
       - Created
		  
******************************************************************************/
static tuiSTATUS iicdrv_fnCheckBusy( void )
{
	uint16 Timeout = 0;  // count the number of loops
	
    // keep looping until bus becomes free, or we timeout
	while( ( IICDRV_mBUSY() ) && ( Timeout < IICDRV_MAX_TIMEOUT ) )
	{
	    Timeout++;
	}
	
	// if it reached timeout value
	if( Timeout >= IICDRV_MAX_TIMEOUT )
	{
	    // Try to recover module
        IICDRV_mMODULE_RECOVER();
        
	    return ( eSTATUS_ERR );  // bus isn't becoming free, return error
	}
	else
	{
	    return ( eSTATUS_OK );  // bus is free, return is okay
	}
}

/******************************************************************************

FUNCTION NAME:
    IICDRV_fnIsr1A

PURPOSE:
    This interrupt routine for the I2C 1A interrupts. This function is 
    called by the I2C1AINT ISR that is implemented in assembler.
INPUTS:
    None

OUTPUTS:
    None

NOTES:

VERSION HISTORY:

Version 1.00  Date: 20-Jun-2006  By: Josephine Tsen
       - Created
		  
******************************************************************************/
void IICDRV_fnIsr1A( void )
{
    uint16 IntSrc;

    // Get Interrupt source
    IntSrc = IICDRV_mINTSRC();
        
    // if we are transmitting or receiving
	if( F280X_mCFGMSK( I2CISRC, INTCODE, XRDY ) == IntSrc )
	{
        // if we aren't at the end of the data to transmit
        if( pucTxBufferStart != pucTxBufferEnd )
        {
            // send next byte out
            I2caRegs.I2CDXR = *pucTxBufferStart++;
	    }	    
	}
	else if( F280X_mCFGMSK( I2CISRC, INTCODE, RRDY ) == IntSrc )
	{   
        // read value from device port
	    *pucRxBufferStart++ = I2caRegs.I2CDRR;
	    
    }  
    else if( F280X_mCFGMSK( I2CISRC, INTCODE, SCD ) == IntSrc )
    {
		iicdrv_fnIOComplete();
	}
    else if( F280X_mCFGMSK( I2CISRC, INTCODE, NACK ) == IntSrc )
    {

        // Recover from error
        IICDRV_mMODULE_RECOVER();
                
        // NACK was received
        tucIOStatus = eSTATUS_ERR;
        
		//iicdrv_fnNotify(); // notify application
		iicdrv_fnIOComplete();
	}
    else if( F280X_mCFGMSK( I2CISRC, INTCODE, ARDY ) == IntSrc )
    {	        
	    // Is composite Write followed by read in progress?
	    if( pucRxBufferStart != pucRxBufferEnd )
	    {
	        // Read is now required 
	           
            // Set up number of bytes to read
            I2caRegs.I2CCNT = pucRxBufferEnd - pucRxBufferStart;
            
            // Disable transmit interrupt
            IICDRV_mTX_INT_DIS();

            // Enable receive interrupt
            IICDRV_mRX_INT_EN();

            // Set up receive with STOP(RM = 0, STT = 1, STP = 1, TRX = 0)
            IICDRV_mRX();
	            
	    }
	    else    
	    {
	        // No composite Write followed by read
	        // Write is complete
	         
            // we are done for this burst, notify application to proceed
            iicdrv_fnIOComplete();
        }
        
    }
	else
	{
        // unexpected interrupt
        assert( FALSE );
          
        // Recover from error
        IICDRV_mMODULE_RECOVER();
        
    }

    // Acknowledge PIE Interrupt
    IICDRV_mPIE_ACK();
    	
}

/*******************************************************************************

FUNCTION NAME: iicdrv_fnSignalStartIO

PURPOSE: Signal the beginning of an IO operation. 

INPUTS:
    none

OUTPUTS:
    tuiSTATUS

NOTES:
    none

VERSION HISTORY:

Version: 1.00  Date: 04-Jul-2006  By: Josephine Tsen
    - Created. 
    
*******************************************************************************/
static tuiSTATUS iicdrv_fnSignalStartIO( void )
{
    // Check if the IIC semaphore is available
    // This resource should be available at this point or else it is
    // an error condition
    if( OSSemAccept( ptzSignalSem ) > 0 )
    {
        // Resource is available
        return( eSTATUS_OK );
    }
    else
    {
        return( eSTATUS_ERR );
    }
}


/*******************************************************************************

FUNCTION NAME: iicdrv_fnWaitForIOComplete


PURPOSE: Provide a means for the IIC driver to wait for the IO to complete.


INPUTS:
    none

OUTPUTS:
     tuiSTATUS - eSTATUS_ERR if timeout or other error occurred
               - eSTATUS_OK  if IO completed in time

NOTES:
    None.

VERSION HISTORY:

Version: 1.00  Date: 04-Jul-2006  By: Josephine Tsen
    - Created.

*******************************************************************************/
static tuiSTATUS iicdrv_fnWaitForIOComplete( void )
{
    uchar8 RetCode;
#define IICDRV_TIMEOUT_TICKS         ( OS_TICKS_PER_SEC / 5 )

    // Wait for the ISR to signal completion
    // If this func doesn't get the semaphore within IICDRV_TIMEOUT_TICKS number of
    // ticks, it means that the ISR was not called in time.
    OSSemPend( ptzSignalSem,
               IICDRV_TIMEOUT_TICKS,
               &RetCode );

    if( RetCode != OS_NO_ERR )
    {
        // Try to recover module
        IICDRV_mMODULE_RECOVER();

        // reset IO Status
        tucIOStatus = eSTATUS_OK;   
        
        // post the semaphore so it is available for the next operation
        OSSemPost( ptzSignalSem );

        return ( eSTATUS_ERR );
    }
    else
    {
        // Error with IO
        if( eSTATUS_OK != tucIOStatus )
        {
            // Try to recover module
            IICDRV_mMODULE_RECOVER();

            // reset IO status
            tucIOStatus = eSTATUS_OK;  
            
            // post the semaphore so it is available for the next operation
            OSSemPost( ptzSignalSem );

            return ( eSTATUS_ERR );
        }
        else
        {
            // reset IO status
            tucIOStatus = eSTATUS_OK;  
 
            // post the semaphore so it is available for the next operation
            OSSemPost( ptzSignalSem );

            return( eSTATUS_OK );
        }

    }
} 


/*******************************************************************************

FUNCTION NAME: iicdrv_fnIOComplete


PURPOSE: Signal that IO operation has been completed.


INPUTS:
    none

OUTPUTS:
    none

NOTES:
    none

VERSION HISTORY:

Version: 1.00  Date: 04-Jul-2006  By: Josephine Tsen
    - Created.

*******************************************************************************/
static void iicdrv_fnIOComplete( void )
{
    // post the semaphore so we know that the IO has completed
    OSSemPost( ptzSignalSem );

} 


