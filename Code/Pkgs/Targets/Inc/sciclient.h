/*==============================================================================
Copyright 2004 Xantrex International.  All rights reserved.

This source file is proprietary to Xantrex Technology Inc. and protected by 
copyright. The reproduction, in whole or in part, by anyone without the written
approval of Xantrex is prohibited.

FILE NAME:  
    sciclient.h
                        
PURPOSE:
    Provide public access to operating system specific SCI functions

FUNCTIONS:
    SCICLIENT_fnCtor            - Client specific constructor for uCOS-II
    SCICLIENT_fnDtor            - Client specific destructor for uCOS-II
    SCICLIENT_fnStoreTxData     - Client specific transmission data storage
    SCICLIENT_fnRetreiveTxData  - Client specific transmission data retreival
    SCICLIENT_fnStoreRxData     - Client specific reception data storage
    SCICLIENT_fnRetreiveRxData  - Client specific reception data reterival

NOTES:

CHANGE HISTORY:
$Log: sciclient.h $

    ***********************************************
    Revision: NovaPfrmB_BaldeeshK/1
    User: BaldeeshK     Date: 01/29/05  Time: 12:44AM
    - Changed return type on constructor/destructor as
    part of code review changes Jan. 28/04
    
    
    *****************  Version 1  *****************
    User: Hollyz       Date: 11/18/04   Time: 1:29p
    Created in $/PD/Common/Targets/TI/c28x/HAL/Inc
    Created


==============================================================================*/
#ifndef SCICLIENT_INCL
#define SCICLIENT_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"	//Requres and architecture defintion in the compiler
#include "LIB_stddefs.h"    //Standard defintions for all projects


/*==============================================================================
                            Type Definitions
==============================================================================*/

/*==============================================================================
                                Enums
==============================================================================*/

/*==============================================================================
                              Defines
==============================================================================*/

/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                          External/Public Constants
==============================================================================*/


/*==============================================================================
                          External/Public Variables
==============================================================================*/
 

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
//SCI client constructor
tuiSTATUS SCICLIENT_fnCtor( void );

//SCI client desstructor
tuiSTATUS SCICLIENT_fnDtor( void );

//Store data to be transmitted
tucBOOL SCICLIENT_fnStoreTxData( uchar8 TxData );

//Retrieve next transmitted data
uchar8 SCICLIENT_fnRetreiveTxData( tucBOOL* DataReady );

//Store received data
tucBOOL SCICLIENT_fnStoreRxData( uchar8 RxData,
                                 uchar8 RxStatus );

//Retreive next received data
tucBOOL SCICLIENT_fnRetreiveRxData( uchar8* RxData, 
                                    uchar8* RxStatus );

#endif  // SCICLIENT_INCL

