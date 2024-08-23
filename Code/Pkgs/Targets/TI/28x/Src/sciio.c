/*==============================================================================
Copyright 2005 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex International and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written 
approval of Xantrex is prohibited.

FILE NAME:  sciio.c

PURPOSE:
    To provide a file input/output structure using the Serial Communication
    Interface (SCIDRV).

FUNCTION(S):
    SCIIO_fnCtor  - Initialize the IO over the SCI device
            
    Local:
        sciio_fnOpen   - callback for handling file opening
        sciio_fnClose  - callback for handling file closing
        sciio_fnRead   - callback for handling device reading
        sciio_fnWrite  - callback for handling device writing
        sciio_fnLseek  - callback for handling device seeking
        sciio_fnUnlink - callback for handling file deleting
        sciio_fnRename - callback for handling file renaming
 
NOTES:
    This module registers a new IO driver with the stdio subsystem via the
    add_device function.  This module also implements a number of standard
    input/output callback functions which read/write or otherwise access
    the lower level SCI driver.  This abstracts the SCIDRV functionality,
    and lets the application layer perform device input/output using the
    stdio functions.

    Xantrex type naming conventions are relaxed to more easily integrate with 
    the stdio library.

    For architecture details, refer to TMS320C28x Optimizing C,C++ Compiler 
    User's Guide (SPRU514)
                                                                           
CHANGE HISTORY :
$Log: sciio.c $

    ***********************************************
    Revision: NovaPfrmB_TrevorM/5
    User: TrevorM     Date: 05/10/05  Time: 02:34PM
    path modifications

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/2
    User: JeffF     Date: 04/19/05  Time: 09:36PM
    Changed return values for some of the functions to use pre defined tuiSTATUS
    values.
    
             
==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/
#include <stdio.h>         // For device list management
#include <file.h>
#include <string.h>
                           
#include "LIB_stdtype.h"   // Standard type definitions
#include "LIB_stddefs.h"   // Standard libaray definitions
#include "xassert.h"       // assert implementation
#include "scidrv.h"        // for SCI driver
#include "sciio.h"         // sciio function declarations

/*==============================================================================
                              Defines
==============================================================================*/

// SCI stuff
#define SCI_TIMEOUT_1000MS   ( 100 )   // 1 character per second
#define SCI_TIMEOUT_200MS    ( 20 )    // 5 character per second
#define SCI_NO_TIMEOUT       ( 0 )

/*==============================================================================
                               Macros
==============================================================================*/
                           
/*==============================================================================
                                Enums
==============================================================================*/

/*=============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
int sciio_fnOpen( const char *path, unsigned flags, int mode );
int sciio_fnClose( int fd );
int sciio_fnRead( int fd, char *buffer, unsigned count );
int sciio_fnWrite( int fd, const char *buffer, unsigned count );
off_t sciio_fnLseek( int fd, off_t offset, int origin );
int sciio_fnUnlink( const char *path );
int sciio_fnRename( const char *old_name, const char *new_name );

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Function Definitions
==============================================================================*/
/*******************************************************************************

FUNCTION NAME: 
    SCIIO_fnCtor
  
PURPOSE: 
    Set up the SCI device for use with the stdio file functions

INPUTS: 
    None

OUTPUTS:
    returns a FILE handle to the SCI IO device.

NOTES:

VERSION HISTORY:

Version: 1.00    Date: 30-Mar-2005    By: Trevor Monk
    - created
    
*******************************************************************************/
tuiSTATUS SCIIO_fnCtor( SCIDRV_teBAUD_RATE BaudRate,
                        SCIDRV_tePARITY    Parity,
                        SCIDRV_teDATA_BITS DataBits,
                        SCIDRV_teSTOP_BITS StopBits, 
                        FILE **ppInput,
                        FILE **ppOutput)
{
    uint16 TimeOut;         // timeout for receiving a character    
    FILE *fp;
    
    // add a device to the stdio device list
    if( add_device("sci", 
        _MSA,
        sciio_fnOpen,
        sciio_fnClose,
        sciio_fnRead,
        sciio_fnWrite,
        sciio_fnLseek,
        sciio_fnUnlink,
        sciio_fnRename) != 0)
    {
        return( eSTATUS_ERR );
    }

    // Initialize the SCI port
    if( SCIDRV_fnCtor( BaudRate,
                       Parity,
                       DataBits,
                       StopBits
                      ) != eSTATUS_OK )
    {
        // Unable to set up the SCI properly. Must be due to improper
        // arguments sent to SCIDRV_fnCtor, or a problem with memory allocation
        // from the RTOS.
        
        // Stop the system if running in debug mode
        assert( FALSE );                
        return( eSTATUS_ERR );   
    }

    // Set the SCI driver timout
    TimeOut = SCI_NO_TIMEOUT;
    SCIDRV_fnSetTimeout( TimeOut );

    // Enable the SCI transmitter and receiver
    SCIDRV_fnEnTransmitter();
    SCIDRV_fnEnReceiver();
    
    // open the reader
    fp = fopen("sci:reader", "r");
    *ppInput = fp;    
    if (fp == NULL)
    {
        assert( FALSE );
        return( eSTATUS_ERR );
    }
    
    // disable all buffering since the SCIClient does it anyway        
    setbuf( fp, NULL );
    
    // open the writer
    fp = fopen("sci:writer", "w");
    *ppOutput = fp;
    if (fp == NULL)
    {
        assert( FALSE );
        return( eSTATUS_ERR );
    }

    // disable all buffering since the SCIClient does it anyway    
    setbuf( fp, NULL );
    
    // indicate success
    return( eSTATUS_OK );
}

/*******************************************************************************

FUNCTION NAME: 
    sciio_fnOpen
  
PURPOSE: 
    callback function for handling file opening

INPUTS: 
    path - pointer to the file path string
    flags - file open flags (see stdio.h)
    mode - file open mode (see stdio.h)
    
OUTPUTS:
    returns 0

NOTES:

    This function is not used, it is simply a stub

    File Descriptors
    
    1 = writer
    2 = reader
        
VERSION HISTORY:

Version: 1.00    Date: 30-Mar-2005    By: Trevor Monk
    - created
    
*******************************************************************************/
int sciio_fnOpen( const char *path, unsigned flags, int mode )
{
    if( flags & O_WRONLY )
        return 1;

    return 2;
}

/*******************************************************************************

FUNCTION NAME: 
    sciio_fnClose
  
PURPOSE: 
    callback function for handling file closing

INPUTS: 
    fd - file descriptor (see stdio.h)
    
OUTPUTS:
    returns 0
    
NOTES:

    This function is not used, it is simply a stub
    
VERSION HISTORY:

Version: 1.00    Date: 30-Mar-2005    By: Trevor Monk
    - created
    
*******************************************************************************/
int sciio_fnClose( int fd )
{
    // perform no action, just indicate success
    return 0;
}


/*******************************************************************************

FUNCTION NAME: 
    sciio_fnRead
  
PURPOSE: 
    callback function for handling file reading via the SCI

INPUTS: 
    fd - file descriptor (see stdio.h)
    buffer - pointer to buffer to store the retrieved characters
    count - number of bytes to read from the device
        
OUTPUTS:
    returns the number of characters read
    returns 0 if a timeout occurred before the read was complete

NOTES:
    
VERSION HISTORY:

Version: 1.00    Date: 30-Mar-2005    By: Trevor Monk
    - created
    
*******************************************************************************/
int sciio_fnRead( int fd, char *buffer, unsigned count )
{
    uint16 idx = 0;
    SCIDRV_teSTATUS status;
        
    while( idx < count )
    {
        status = SCIDRV_fnReceiveByte( ( uchar8* )&buffer[ idx ] );
        if( status != SCIDRV_eOK )
        {
            return 0;
        }
        
        idx++;
    }
    
    // return the number of characters actually read
    return (int)idx;
}


/*******************************************************************************

FUNCTION NAME: 
    sciio_fnWrite
  
PURPOSE: 
    callback function for handling file reading via the SCI

INPUTS: 
    fd - file descriptor (see stdio.h)
    buffer - pointer to buffer containing the characters to write
    count - number of bytes to write to the device
        
OUTPUTS:
    returns the number of characters written

NOTES:
    
VERSION HISTORY:

Version: 1.00    Date: 30-Mar-2005    By: Trevor Monk
    - created
    
*******************************************************************************/
int sciio_fnWrite( int fd, const char *buffer, unsigned count )
{
    uint16 idx = 0;
        
    while( idx < count )
    {
        SCIDRV_fnSendByte( buffer[ idx ] );
        idx++;
    }

    // return the number of characters written
    return count;
}

/*******************************************************************************

FUNCTION NAME: 
    sciio_fnLseek
  
PURPOSE: 
    callback function for handling file seeking

INPUTS: 
    fd - file descriptor (see stdio.h)
    offset - seek offset relative to origin
    origin - start of seek
        
OUTPUTS:
    returns EOF since this function is currently unsupported on the SCI

NOTES:
    
VERSION HISTORY:

Version: 1.00    Date: 30-Mar-2005    By: Trevor Monk
    - created
    
*******************************************************************************/
off_t sciio_fnLseek( int fd, off_t offset, int origin )
{
    // indicate failure
    return 0;
}

/*******************************************************************************

FUNCTION NAME: 
    sciio_fnUnlink
  
PURPOSE: 
    callback function for handling file deleting

INPUTS: 
    path - path to file to be deleted
        
OUTPUTS:
    returns -1 since this function is currently unsupported on the SCI

NOTES:
    
VERSION HISTORY:

Version: 1.00    Date: 30-Mar-2005    By: Trevor Monk
    - created
    
*******************************************************************************/
int sciio_fnUnlink( const char *path )
{
    // indicate failure
    return 0;
}

/*******************************************************************************

FUNCTION NAME: 
    sciio_fnRename
  
PURPOSE: 
    callback function for handling file renaming

INPUTS: 
    old_name - old file name
    new_name - new name
        
OUTPUTS:
    returns 1 (failure) since this function is currently unsupported on the SCI

NOTES:
    
VERSION HISTORY:

Version: 1.00    Date: 30-Mar-2005    By: Trevor Monk
    - created
    
*******************************************************************************/
int sciio_fnRename( const char *old_name, const char *new_name )
{
    // indicate failure
    return 0;
}
