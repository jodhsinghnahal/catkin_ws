/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex Technology Inc. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    sciclient.c

PURPOSE:
    Provides operating system specific functionality for the SCI driver. This
    file is specific for the uCOSII RTOS
   
FUNCTIONS:
    SCICLIENT_fnCtor            - Client specific constructor for uCOS-II
    SCICLIENT_fnDtor            - Client specific destructor for uCOS-II
    SCICLIENT_fnStoreTxData     - Client specific transmission data storage
    SCICLIENT_fnRetreiveTxData  - Client specific transmission data retreival
    SCICLIENT_fnStoreRxData     - Client specific reception data storage
    SCICLIENT_fnRetreiveRxData  - Client specific reception data reterival
  
NOTES:
    This file utilized the functionality of the uCOS II RTOS
    uCOS II must be started prior to calling these routines

    
CHANGE HISTORY:
$Log: sciclient.c $

    ***********************************************
    Revision: NovaPfrmB_TrevorM/2
    User: TrevorM     Date: 04/01/05  Time: 01:20AM
    Added retry and timeout mechanism to transmit function

    ***********************************************
    Revision: NovaPfrmB_TrevorM/1
    User: TrevorM     Date: 03/31/05  Time: 07:57PM
    merged

    ***********************************************
    Revision: SurveyorPfrmA_JeffF/1
    User: JeffF     Date: 03/30/05  Time: 06:07PM
    Added a return statement at the end of the Dtor function because it was
    missing.

    ***********************************************
    Revision: NovaPfrmB_Local_HorstM/1
    User: HorstM     Date: 02/18/05  Time: 01:09AM
    added CR/LF at end of file (compiler warning)
    

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/1
    User: BaldeeshK     Date: 01/29/05  Time: 12:44AM
    - Changed return type on constructor/destructor as
    part of code review changes Jan. 28/04
    
    
    *****************  Version 1  *****************
    User: Hollyz       Date: 11/18/04   Time: 1:26p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Src
    Created
    

==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "sciclient.h"        // for sci client
#include "scidrv.h"         // for sci driver
#include "config.h"         // for target configuration
#include "includes.h"        // for RTOS
#include "xassert.h"        // for assert
#include <errno.h>

/*==============================================================================
                            Type Definitions
==============================================================================*/

/*==============================================================================
                                Enums
==============================================================================*/

/*==============================================================================
                              Defines
==============================================================================*/
//Define the transmit and receive buffer sizes
#define TX_QUEUE_SIZE      ( 255 )
#define RX_QUEUE_SIZE      ( 255 )

//Define for SCI null value
#define SCI_NULL           ( 0 )
/*=============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

//Transmission queue pointer
static OS_EVENT *SciTx = ( OS_EVENT* ) 0;

//Transmission message queue
static void *SciTxMsg[ TX_QUEUE_SIZE ];

//Reception queue pointer
static OS_EVENT *SciRx = ( OS_EVENT* ) 0;

//Reception message queue
static void *SciRxMsg[ RX_QUEUE_SIZE ];

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME: 
   SCICLIENT_fnCtor
  
PURPOSE: 
   Constructor for SCICLIENT routines
   
INPUTS:
   None
             
OUTPUTS:
   None 

NOTES:

   The RTOS function OSInit( ) must be called prior to calling the constructor
   otherwise the semaphore will not be properly initialized.
   
   This function must be called before any other access to the SCI port

VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Created
     
*******************************************************************************/
tuiSTATUS SCICLIENT_fnCtor( void )
{
    //Create a transmission queue for the SCI
    SciTx = OSQCreate( SciTxMsg, 
                       TX_QUEUE_SIZE );
                          
    //Test if the queue was created
    if( SciTx == ( OS_EVENT * )0 )
    {   
        //Queue was not created so assert
        assert( 0 );
        return ( eSTATUS_ERR );

    }
       
    //Create a reception and status queue for the SCI
    SciRx = OSQCreate( SciRxMsg, 
                       RX_QUEUE_SIZE );
                          
    //Test if the queue was created
    if( SciRx == ( OS_EVENT * )0 )
    {
        //Queue was not created so assert
        assert( 0 );
        return ( eSTATUS_ERR );     
    }
    
    return ( eSTATUS_OK );
}

/*******************************************************************************

FUNCTION NAME: 
   SCICLIENT_fnDtor
  
PURPOSE: 
   Destructor for SCICLIENT routines
      
INPUTS:
   None
             
OUTPUTS:
   None
 
NOTES:

   
VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project.
     
*******************************************************************************/
#ifdef GLBL_DTOR_EN 

tuiSTATUS SCICLIENT_fnDtor( void )
{

    INT8U Error;    // Error indicator, INT8U from OS

    //Delete transmit and receive queues
//Include queue deletion if enabled in RTOS
#if( OS_Q_DEL_EN > 0 )

    // Delete the transmission queue
    SciTx = OSQDel( SciTx, OS_DEL_ALWAYS, &Error );
    
    //Test if the queue was deleted
    if( SciTx != ( OS_EVENT * ) 0 )
    {
        //Error occured when deleting queue
        assert( 0 );
        return ( eSTATUS_ERR );
    }
   
    // Delete the transmission queue
    SciRx = OSQDel( SciRx, OS_DEL_ALWAYS, &Error );
 
    //Test if the queue was deleted   
    if (SciRx != ( OS_EVENT * ) 0 )
    {
        //Error occured when deleting queue
        assert( 0 );
        return ( eSTATUS_ERR );
    }

#endif //if( OS_Q_DEL_EN == TRUE )
       
    return( eSTATUS_OK );
}

#endif //ifdef GLBL_DTOR_EN 
 

/*******************************************************************************

FUNCTION NAME: 
   SCICLIENT_fnStoreTxData
  
PURPOSE: 
   Provides client services to store data in transmission queue
   
   This implementation uses the uCOS II queue management services
   
   SCICLIENT_fnCtor must have been called in order for this
   service to function
   
INPUTS:
   uchar8 TxData - Transmitted data to be stored in the buffer
             
OUTPUTS:
   tucBOOL Status 
   TRUE - data was stored
   FALSE - data was not stored

NOTES:
   This implementation require the uCOS II RTOS to have been intialized  
   and the fucntion SCICLIENT_fnCtor to have been called prior to use of this
   routine
   
   In order to save RAM memory instead of using the TX queue as a pointer to a
   memory partition, the data to be transmitted out the SCI is stored in the
   actual queue. The TxData must be cast to void* in order to be placed in
   the queue by OSQPost. The routine SCICLIENT_fnRetreiveTxData retrieves the
   data using OSQAccept as a void* which is then cast to a uchar8. This method
   works as we are only interested in the lower 8 bits of the returned pointer
   which is the data to be transmitted. SCI driver unit test confirms that this
   functions as expected.

   
VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project.

*******************************************************************************/
tucBOOL SCICLIENT_fnStoreTxData( uchar8 TxData )
{

    INT8U Error;    // Error indicator, INT8U from OS
    uint16 numtries;
    
    // maximum number of attempts to queue a character for transmission
    numtries = 3;  

    while( numtries > 0 )
    {
        //Place the character to be transmitted in the queue
        Error =  OSQPost( SciTx, 
                        ( void* )TxData );                  
       
        //Test if the character was placed in the queue
        if( Error == OS_NO_ERR )
        {
            //Data placed in transmission buffer
            return( TRUE );
        }
        else if( Error == OS_Q_FULL )
        {
           //Data not placed in transmission buffer as queue is full
           // Data not placed in receive buffer as queue is full
           // let's wait a bit and see if it empties
           // An alternative strategy is to use a semaphore to 
           // track the queue full empty status.
           // for example, the SCIClient could pend on the semaphore
           // when the queue is full, and post the semaphore when
           // the queue is half full.
           OSTimeDly(1);
        }
        else
        {
           //Assert on all other OSQPOST error conditions
           assert( 0 );
        }

        numtries--;
    }
        
    //Return if data not stored
    return( FALSE );
   
} 

/*******************************************************************************

FUNCTION NAME: 
   SCICLIENT_fnRetreiveTxData
  
PURPOSE: 
   Retrieves the next character from the buffer as filled by 
   SCICLIENT_fnStoreTxData
   
INPUTS:
   tucBOOL* DataReady - pointer to data ready status
   TRUE - valid data  ready for transmission
   FALSE = no data ready for transmission
       
             
OUTPUTS:
   uchar8 Data - returned data from queue
 

NOTES:
   This implementation require the uCOS II RTOS to have been intialized  
   and the fucntion SCICLIENT_fnCtor to have been called prior to use of this
   routine
   
   In order to save RAM memory instead of using the TX queue as a pointer to a
   memory partition, the data to be transmitted out the SCI is stored in the
   actual queue. The TxData must be cast to void* in order to be placed in
   the queue by OSQPost. The routine SCICLIENT_fnRetreiveTxData retrieves the
   data using OSQAccept as a void* which is then cast to a uchar8. This method
   works as we are only interested in the lower 8 bits of the returned pointer
   which is the data to be transmitted. SCI driver unit test confirms that this
   functions as expected.  

   
VERSION HISTORY:
Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Copied from NOVA project.

*******************************************************************************/
uchar8 SCICLIENT_fnRetreiveTxData( tucBOOL* DataReady )
{
    void *Msg;        
    INT8U Error;    // Error indicator, INT8U from OS
       
    //Get the next character from the queue to transmit
    Msg = OSQAccept( SciTx, 
                     &Error );

    //If no error returned from OSQAccept
    if( Error == OS_NO_ERR )
    {
       //Set data ready to true
       *DataReady = TRUE;
       
       //Return the character, cast to uchar8 to retreive byte of data
       return( ( uchar8 )Msg );

    }
    else if( Error == OS_Q_EMPTY ) //Queue empty
    {  
    
       //Set data ready to FALSE
        *DataReady = FALSE;    
          
    }
    else
    {
    
       //Set data ready to FALSE
        *DataReady = FALSE;
            
       //Assert on all other OSQAccept error conditions
       assert( 0 );
    }
    
    //Return the SCI_NULL
    return( SCI_NULL );     

}

/*******************************************************************************

FUNCTION NAME: 
   SCICLIENT_fnStoreRxData
  
PURPOSE: 
   Provides client services to store data in the reception queue
   
INPUTS:
   uchar8 RxData - Received data to be stored in the buffer
   uchar8 RxStatus - Status to be stored in the buffer
                      
OUTPUTS:
 
   tucBOOL Status 
   TRUE  - data was stored
   FALSE - data was not stored

NOTES:
   This implementation require the uCOS II RTOS to have been intiazlized  
   and the fucntion SCICLIENT_fnCtor to have been called prior to use of this
   routine

   
VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Created.
     
*******************************************************************************/
tucBOOL SCICLIENT_fnStoreRxData( uchar8 RxData, 
                                 uchar8 RxStatus )
{
    INT32U Message;   // Combined status/data word
    INT16U *pMessage; //Pointer to structure in managed memory
    INT8U Error;      // Error indicator, INT8U from OS
    
    // Create the combined status/data word
    pMessage = ( INT16U *)&Message;   
       
    //Store status and data in the memory block
    *pMessage++ = RxData;
    *pMessage   = RxStatus;

    //Place the character in the receive queue
    Error = OSQPost( SciRx, 
                     ( void* )Message );                  
   
    //Test if the character was placed in the queue
    if( Error == OS_NO_ERR )
    {
        //Data placed in receive buffer
        return( TRUE );
    }
    else if( Error == OS_Q_FULL )
    {
       //Data not placed in receive buffer as queue is full
        return( FALSE );
    }
    else
    {
       //Assert on all other OSQPOST error conditions
       assert( 0 );
    }
        
    //Return if data not stored
    return( FALSE );     
}

/*******************************************************************************

FUNCTION NAME: 
   SCICLIENT_fnRetreiveRxData
  
PURPOSE: 
   Retrieves the next character from the buffer as filled by 
   SCICLIENT_fnStoreRxData
   
   
INPUTS:
   uchar8* RxData - Pointer to received data buffer
   uchar8* RxStatus - Pointer to received status buffer
             
OUTPUTS:
    tucBOOL - Status 
    TRUE  - valid data returned or timeout condition has occured
    FALSE - OS Error detected
 

NOTES:
   This implementation require the uCOS II RTOS to have been intiazlized  
   and the fucntion SCICLIENT_fnCtor to have been called prior to use of this
   routine

   
VERSION HISTORY:

Version: 1.00    Date: 17-Nov-2004    By: Holly Zhou
    - Created

*******************************************************************************/
tucBOOL SCICLIENT_fnRetreiveRxData( uchar8* RxData,
                                    uchar8* RxStatus )
{
    INT8U PendError;     // Error indicator, INT8U from OS
    INT32U Message;         // Received message as status/data
    INT16U *pMessage ;   // Pointer to the message
        
    //Test the queue to determine if a character is available
    //Pointer points to allocated memory
    Message = ( INT32U )OSQPend( SciRx, 
                                 SCIDRV_fnGetTimeout(), 
                                 &PendError);
       
    //If timeout has occured then indicate timeout condition
    if( PendError == OS_TIMEOUT )
    {
        *RxData = SCI_NULL;              //Set the value of data to 0
        *RxStatus = SCIDRV_eTIMEOUT;    //Set the status to timeout

        //No return here on purpose. The return condition of "TRUE" is valid
        //since this timeout could be part of a break.

    }
    //If no error condition has occured
    else if( PendError == OS_NO_ERR )
    {
       
        // Retrieve the status and data from the 'pointer'
        pMessage = ( INT16U *)&Message;
       
        //Recover the stored data
        *RxData = ( INT8U )*pMessage++;
       
        //Recover the stored status
        *RxStatus = ( INT8U )*pMessage;
    }
    //OS Error
    else
    {   

        //Assert on all other OSQPend errors
       assert(0);
       
       return( FALSE );

    } //if( PendError == OS_TIMEOUT )
   
    return( TRUE );

}
