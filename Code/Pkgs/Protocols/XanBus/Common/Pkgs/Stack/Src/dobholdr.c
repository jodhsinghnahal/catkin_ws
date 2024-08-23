/*
***************************************************************************
**
** PROJECT: CAN Library
**
** $Workfile:: dobholdr.c     $       $Author:: Dalem              $
**
***************************************************************************
**
** PROJECT: CAN Library
**
**
** DESCRIPTION: NMEA 2000 Comm Library API - logic which holds active dobs
**
**
**            Copyright (c) 2000 Teleflex Marine/Industrial
**                       All Rights Reserved
**   Use, duplication, or disclosure of this material, in any
**   form, is forbidden without permission from Teleflex Marine
**
***************************************************************************/
#include <string.h>
#include "libenv.h"
#include "LibCore.h"
#include "pgn.h"
#include "xbudefs.h"
#include "xbgdefs.h"
#include "xbldefs.h"
#include "xassert.h"

/*=============================================================================
                              Definitions
==============================================================================*/

//! Number of Priority Tx DOB which can be active at one time
#define MAX_VIP_DOBS        ( 10 )

#define NO_NODE_LINK        ( 0xFF )

/*=============================================================================
                              Structures
==============================================================================*/

//! The structure used for the DOBs that are in the process of being transmitted
typedef struct DOBHOLDER_zTX_PEND_DOB
{
    PGNDOB *ptzDob;
    uchar8 ucPrev;
    uchar8 ucNext;
} DOBHOLDER_tzLL_DOB_NODE;


//! The structure for managing the DOBs pending transmision
//! Implements a double-linked list to allow out-of-order removal
//! Used by the PGNXMTR module; once a DOB is selected for transmission
//! it is removed from the FIFO and placed in this list.
//! The nodes are removed when all the DOB's frames are sent or when the
//! DOB times out.
typedef struct DOBHOLDER_zTX_LINK_LIST
{
    // The order of entries should not be changed: they get auto-initialized
    DOBHOLDER_tzLL_DOB_NODE *ptzNodes;
    uchar8 ucTotalNodes;    ///< Total nodes in the list
    uchar8 ucFirstUsed;     ///< index of the first node in the list
    uchar8 ucLastUsed;      ///< index of the last node in the list
    uchar8 ucUsedCount;     ///< current count of used nodes
    uchar8 ucFirstFree;     ///< index of the first free node in the list
    uchar8 ucLastFree;      ///< index of the last free node in the list
    uchar8 ucFreeCount;     ///< current count of free nodes
    uchar8 ucCritNest;      ///< critical section nesting counter
} DOBHOLDER_tzTX_LINK_LIST;


/*==============================================================================
                           Local/Private Variables
==============================================================================*/

/*!
 *  @details
 *
 *  The linked-list acts as both a warehouse for the DOBs posted by the
 *  application layer and as a delivery list for the DOBs that the PGNXTick()
 *  function started to send via the CAN bus.
 *
 *  The PGNXTick() processes messages from the linked list in the order in
 *  which they were placed. This will ensure messages are sent on the bus in
 *  the order desired by the application layer.The DOBs will be removed from
 *  the linked-list when all the DOBs' frames have been transmitted or on
 *  timeout. The DOBs can be removed from the linked-list in an out-of-order
 *  fashion, that is, a DOB with only one frame will be removed faster than
 *  one who has multiple frames. Only a linked-list allows such behaviour.
 *
 */

//! Regular DOBs linked-list
LOCAL DOBHOLDER_tzLL_DOB_NODE m_sRegularDOBHolder[ MAX_PGN_DOBS ];
DOBHOLDER_tzTX_LINK_LIST    PGNDOB_tzRegularLinkList =
                            {
                                &m_sRegularDOBHolder[ 0 ],  // ptzDobs
                                MAX_PGN_DOBS,               // ucMax
                                NO_NODE_LINK,               // ucFirstUsed
                                NO_NODE_LINK,               // ucLastUsed
                                0,                          // ucUsedCount
                                0,                          // ucFirstFree
                                MAX_PGN_DOBS-1,             // ucLastFree
                                MAX_PGN_DOBS                // ucFreeCount
                            };

//! VIP DOBs linked-list
LOCAL DOBHOLDER_tzLL_DOB_NODE m_sVipDOBHolder[ MAX_VIP_DOBS ];
DOBHOLDER_tzTX_LINK_LIST    PGNDOB_tzVipLinkList =
                            {
                                &m_sVipDOBHolder[ 0 ],      // ptzDobs
                                MAX_VIP_DOBS,               // ucMax
                                NO_NODE_LINK,               // ucFirstUsed
                                NO_NODE_LINK,               // ucLastUsed
                                0,                          // ucUsedCount
                                0,                          // ucFirstFree
                                MAX_VIP_DOBS-1,             // ucLastFree
                                MAX_VIP_DOBS                // ucFreeCount
                            };

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

LOCAL PGNDOB *MatchFastPacketDob( uint32 ulPgn, UINT8 srcAddr, UINT8 seqNum );
LOCAL void dobhld_fnInitRegularLinkedList( void );
LOCAL void dobhld_fnInitVipLinkedList( void );
LOCAL uchar8 dobhld_fnAddDobToList( PGNDOB *ptzDob, uchar8 ucPriority );
LOCAL tucBOOL dobhld_fnRemoveDob( PGNDOB *pDob, uchar8 ucPriority );
LOCAL tucBOOL dobhld_fnRemoveNodeFromList( DOBHOLDER_tzLL_DOB_NODE *ptzNode, uchar8 ucPriority );

/*==============================================================================
                           Function Definitions
==============================================================================*/


/***************************************************************************//**

  DOBs Holder (Queue) constructor

*******************************************************************************/
void DOBHLDConstruct( void )
{
    dobhld_fnInitVipLinkedList();
    dobhld_fnInitRegularLinkedList();
}

/***************************************************************************//**

  DOBs Holder (Queue) destructor

*******************************************************************************/
void DOBHLDDestruct(void)
{
}


/***************************************************************************//**

  Tries to find if the current frame is associated with a DOB

  @param[in]  pInfo      pointer to PGN info
  @param[in]  pCanFrame  pointer to CAN frame
  @param[out] pDOB       pointer to pointer to DOB

  @retval TFXCR_NO_DATA -   not found but OK
  @retval TFXCR_NOT_FOUND - not found but error;
                            a DOB was supposed to be present but is not.
  @retval TFXCR_OK -        found

*******************************************************************************/
TFXCAN_RETURNS DOBHLDFindFromFrame( const PGN_tzPGN_INFO * pInfo,
                                    CANFRAME *pCanFrame,
                                    PGNDOB **pDOB )
{
    UINT8 srcAddr;
    UINT8 seqNum;

    // If this is a fastpacket PGN
    if( ( pInfo ) && ( pInfo->ucFastPacketSeqIdx != PGN_NOT_FASTPACKET ) )
    {
        // If packet number not zero
        if( pCanFrame->m_CanData.m_u8Data[0] & 0x1F )
        { // not first frame - should have a DOB
            srcAddr = pCanFrame->m_CanData.m_u8SA;
            seqNum = pCanFrame->m_CanData.m_u8Data[0] & 0x0E0;

            *pDOB = MatchFastPacketDob( pInfo->ulPgn, srcAddr, seqNum );

            return ( *pDOB == NULL ) ? TFXCR_NOT_FOUND : TFXCR_OK;
        }
    }

    // Only need look for fast packet and frames after first
    return TFXCR_NO_DATA;
}

/***************************************************************************//**

  Tries to add a DOB in the specified DOBs' queue

  @param[in] pDOB:       pointer to DOB to be added
  @param[in] ucPriority: specifies the DOB priority

  @retval TFXCR_OK -        DOB added
  @retval TFXCR_TABLE_FULL  could not add the DOB; the queue is full


*******************************************************************************/
TFXCAN_RETURNS DOBHLDAdd( PGNDOB *pDob )
{
    TFXCAN_RETURNS ucRetVal = TFXCR_OK;
    UINT8 ui8DobCount = 0;

    if( dobhld_fnAddDobToList( pDob, XB_REG_BUF ) == NO_NODE_LINK )
    {
        ucRetVal = TFXCR_TABLE_FULL;
    }

    ui8DobCount = PGNDOB_tzRegularLinkList.ucUsedCount + PGNDOB_tzVipLinkList.ucUsedCount;
    if( ui8DobCount > XBMSG_tzXbCommStats.ucMaxActiveDobs )
    {
        XBMSG_tzXbCommStats.ucMaxActiveDobs = ui8DobCount;
    }

    return ucRetVal;
}


#ifndef NDEBUG
/***************************************************************************//**

  Returns the number of unused DOB queue entries

  @param[in] ucPriority: specifies the DOB priority

  @returns the number of free/unused DOB entries in the specified queue

*******************************************************************************/
uchar8 DOBHLDUnused( uchar8 ucPriority )
{
    DOBHOLDER_tzTX_LINK_LIST *ptzList;

    // Get local pointer to the specified Tx FIFO for convenience
    if( ucPriority == XB_REG_BUF )
    {
        ptzList = &PGNDOB_tzRegularLinkList;
    }
    else
    {
        ptzList = &PGNDOB_tzVipLinkList;
    }

    return ( ptzList->ucTotalNodes - ptzList->ucUsedCount );
}
#endif



/***************************************************************************//**

  Return the first/oldest DOB from the Linked-List .

  @returns PGNDOB * - pointer to the first/oldest DOB in the LL
                      NULL on failure

*******************************************************************************/
PGNDOB *DOBHLDFirst( void )
{
    // unsigned int uiIntSts;
    PGNDOB *ptzDob;

    // Enter critical section
    // Avoid interruptions while we manipulate the LL
    // uiIntSts = __disable_interrupts();

    ptzDob = NULL;
    if( PGNDOB_tzRegularLinkList.ucUsedCount > 0 )
    {
        // Get the first used DOB
        ptzDob = PGNDOB_tzRegularLinkList.ptzNodes[ PGNDOB_tzRegularLinkList.ucFirstUsed ].ptzDob;
    }

    // Exit critical section
    // __restore_interrupts( uiIntSts );

    return ptzDob;
}


/***************************************************************************//**

  Extract the next oldest DOB from the FIFO and place in in the linked list
  used for the DOBs in the process of being transmitted.

  @returns PGNDOB * - pointer to the oldest DOB in the FIFO
                      NULL on failure

*******************************************************************************/
PGNDOB *DOBHLDNext( PGNDOB *pLastDob )
{
    // unsigned int uiIntSts;
    PGNDOB *ptzDob;

    // Enter critical section
    // Avoid interruptions while we manipulate the LL
    // uiIntSts = __disable_interrupts();

    ptzDob = NULL;
    if( pLastDob->m_ucListIdx != NO_NODE_LINK )
    {
        // Pointer to this DOB's node in the LL
        DOBHOLDER_tzLL_DOB_NODE *ptzNode = &PGNDOB_tzRegularLinkList.ptzNodes[ pLastDob->m_ucListIdx ];
        // Get the DOB from the next node in the LL if available
        if( ptzNode->ucNext != NO_NODE_LINK )
        {
            ptzDob = PGNDOB_tzRegularLinkList.ptzNodes[ ptzNode->ucNext ].ptzDob;
        }
    }

    // Exit critical section
    // __restore_interrupts( uiIntSts );

    return ptzDob;
}


/***************************************************************************//**

  Removed the specified DOB from the linked list

  @param pDob - pointer to the DOB to be removed

*******************************************************************************/
void DOBHLDDelete( PGNDOB *pDob )
{
    dobhld_fnRemoveDob( pDob, XB_REG_BUF );
}


//*************************************************************************
//
//*************************************************************************
LOCAL PGNDOB *MatchFastPacketDob( uint32 ulPgn, UINT8 srcAddr, UINT8 seqNum )
{
    // unsigned int uiIntSts;
    DOBHOLDER_tzTX_LINK_LIST *ptzList;
    PGNDOB *pDOB;
    uchar8 l;

    // Enter critical section
    // Avoid interruptions while we traverse the LL
    // uiIntSts = __disable_interrupts();

    pDOB = NULL;
    // Check both lists starting with the VIP list
    for( l=0; l<2; l++ )
    {
        DOBHOLDER_tzLL_DOB_NODE *ptzNode;
        uchar8 ucNext;
        uchar8 i = 0;

        if( l == 0 )
        {
            ptzList = &PGNDOB_tzVipLinkList;
        }
        else
        {
            ptzList = &PGNDOB_tzRegularLinkList;
        }

        if( ptzList->ucUsedCount > 0 )
        {
            // Start the search with the first used node
            ptzNode = &ptzList->ptzNodes[ ptzList->ucFirstUsed ];
            // Save the index to the next node
            ucNext = ptzNode->ucNext;
            for( ; i < ptzList->ucUsedCount; i++ )
            {
                // Assign next DOB and check for NULL
                // Check the next element in the linked list
                pDOB = ptzNode->ptzDob;

                if( ( pDOB != NULL ) && ( pDOB->m_pPgnInfo != NULL ) )
                {
                    if( ( pDOB->m_pPgnInfo->ulPgn == ulPgn )
                     && ( pDOB->m_SourceAddr == srcAddr )
                     && ( ( pDOB->m_u8FastPacketSequenceNo & 0x0E0 ) == seqNum )
                     && ( ( pDOB->m_ucFlags & DOB_PACKET_TYPE_MASK ) == DOB_IS_RECV_FASTPACKET ) )
                    {
                        break;
                    }
                }
                // Reset the pDOB to ensure correct return result in case the DOB is not found
                pDOB = NULL;
                // Not found - get the index to the next node
                ucNext = ptzNode->ucNext;
                // Get the next node
                ptzNode = &ptzList->ptzNodes[ ucNext ];
            }
            // Was the DOB found?
            if( pDOB != NULL )
            {
                break;
            }
        }
    }

    // Exit critical section
    // __restore_interrupts( uiIntSts );

    return pDOB;
}

/***************************************************************************//**

  Tries to add a DOB in the VIP DOBs queue

  @param[in] pDOB:          pointer to DOB to be added

  @retval TFXCR_OK -        DOB added
  @retval TFXCR_TABLE_FULL  could not add the DOB; the queue is full

*******************************************************************************/
TFXCAN_RETURNS addToVipDob( PGNDOB *pDob )
{
    TFXCAN_RETURNS ucRetVal = TFXCR_OK;
    UINT8 ui8DobCount = 0;

    if( dobhld_fnAddDobToList( pDob, XB_VIP_BUF ) == NO_NODE_LINK )
    {
        ucRetVal = TFXCR_TABLE_FULL;
    }

    ui8DobCount = PGNDOB_tzRegularLinkList.ucUsedCount + PGNDOB_tzVipLinkList.ucUsedCount;
    if( ui8DobCount > XBMSG_tzXbCommStats.ucMaxActiveDobs )
    {
        XBMSG_tzXbCommStats.ucMaxActiveDobs = ui8DobCount;
    }

    return ucRetVal;
}

/***************************************************************************//**

  Get the first/oldest VIP DOB from the VIP linked list used for the DOBs
  in the process of being transmitted.

  @returns PGNDOB * - pointer to the oldest VIP DOB in the Linked-List
                      NULL on failure

*******************************************************************************/
PGNDOB *searchFirstVipDOB( void )
{
    // unsigned int uiIntSts;
    PGNDOB *ptzDob;

    // Enter critical section
    // Avoid interruptions while we manipulate the LL
    // uiIntSts = __disable_interrupts();

    ptzDob = NULL;
    if( PGNDOB_tzVipLinkList.ucUsedCount > 0 )
    {
        // Get the first used DOB
        ptzDob = PGNDOB_tzVipLinkList.ptzNodes[ PGNDOB_tzVipLinkList.ucFirstUsed ].ptzDob;
    }

    // Exit critical section
    // __restore_interrupts( uiIntSts );

    return ptzDob;
}


/***************************************************************************//**

  Get the next oldest VIP DOB from the linked list of DOBs in the process
  of being transmitted.

  @returns PGNDOB * - pointer to the oldest VIP DOB in the VIP linked-list;
                      NULL on failure

*******************************************************************************/
PGNDOB *searchNextVipDob( PGNDOB *pLastDob )
{
    // unsigned int uiIntSts;
    PGNDOB *ptzDob;

    // Enter critical section
    // Avoid interruptions while we manipulate the LL
    // uiIntSts = __disable_interrupts();

    ptzDob = NULL;
    if( pLastDob->m_ucListIdx != NO_NODE_LINK )
    {
        // Pointer to this DOB's node in the LL
        DOBHOLDER_tzLL_DOB_NODE *ptzNode = &PGNDOB_tzVipLinkList.ptzNodes[ pLastDob->m_ucListIdx ];
        // Get the DOB from the next node in the LL if available
        if( ptzNode->ucNext != NO_NODE_LINK )
        {
            ptzDob = PGNDOB_tzVipLinkList.ptzNodes[ ptzNode->ucNext ].ptzDob;
        }
    }

    // Exit critical section
    // __restore_interrupts( uiIntSts );

    return ptzDob;
}


/***************************************************************************//**

  Removed the specified VIP DOB from the VIP linked list.

  @param pDob - pointer to the DOB to be removed/deleted

*******************************************************************************/
void deleteFromVipDob( PGNDOB *pDob )
{
    dobhld_fnRemoveDob( pDob, XB_VIP_BUF );
}


/***************************************************************************//**

  Initialize the VIP DOBs linked-list; places all nodes in the free list

  @retval TRUE  success
  @retval FALSE failure.

*******************************************************************************/
LOCAL void dobhld_fnInitVipLinkedList( void )
{
    DOBHOLDER_tzLL_DOB_NODE *ptzNode;
    uchar8 i;

    // Zero all the nodes
    memset( (void*)m_sVipDOBHolder, 0 , sizeof( m_sVipDOBHolder ) );

    // The ucFirstFree remain zero; no need to re-init
    // The first node does not have a prev link
    m_sVipDOBHolder[ 0 ].ucPrev = NO_NODE_LINK;
    // The next link for the first is the index 1.
    m_sVipDOBHolder[ 0 ].ucNext = 1;
    // Setup the link chain for the intermediate nodes
    for( i=1; i<MAX_VIP_DOBS-1; i++ )
    {
        ptzNode = &m_sVipDOBHolder[ i ];
        ptzNode->ucPrev = i - 1;
        ptzNode->ucNext = i + 1;
    }
    // Last node
    m_sVipDOBHolder[ MAX_VIP_DOBS - 1 ].ucPrev = MAX_VIP_DOBS - 2;
    m_sVipDOBHolder[ MAX_VIP_DOBS - 1 ].ucNext = NO_NODE_LINK;

    // The free count is the same as the total
    PGNDOB_tzVipLinkList.ucFreeCount = MAX_VIP_DOBS;
    PGNDOB_tzVipLinkList.ucLastFree = MAX_VIP_DOBS - 1;
}


/***************************************************************************//**

  Initialize the DOBs linked-list; places all nodes in the free list

  @retval TRUE  success
  @retval FALSE failure.

*******************************************************************************/
LOCAL void dobhld_fnInitRegularLinkedList( void )
{
    DOBHOLDER_tzLL_DOB_NODE *ptzNode;
    uchar8 i;

    // Zero all the nodes
    memset( (void*)m_sRegularDOBHolder, 0 , sizeof(m_sRegularDOBHolder) );

    // The ucFirstFree remain zero; no need to re-init
    // The first node does not have a prev link
    m_sRegularDOBHolder[ 0 ].ucPrev = NO_NODE_LINK;
    // The next link for the first is the index 1.
    m_sRegularDOBHolder[ 0 ].ucNext = 1;
    // Setup the link chain for the intermediate nodes
    for( i=1; i<MAX_PGN_DOBS-1; i++ )
    {
        ptzNode = &m_sRegularDOBHolder[ i ];
        ptzNode->ucPrev = i - 1;
        ptzNode->ucNext = i + 1;
    }
    // Last node
    m_sRegularDOBHolder[ MAX_PGN_DOBS - 1 ].ucPrev = MAX_PGN_DOBS - 2;
    m_sRegularDOBHolder[ MAX_PGN_DOBS - 1 ].ucNext = NO_NODE_LINK;

    // The free count is the same as the total
    PGNDOB_tzRegularLinkList.ucFreeCount = MAX_PGN_DOBS;
    PGNDOB_tzRegularLinkList.ucLastFree = MAX_PGN_DOBS - 1;
}


/***************************************************************************//**

  Add a DOB to the linked-list.

  @param pDob - pointer to the DOB to be added

  @returns uchar8 - the index to the node in the linked-list
                    or 0xFF for failure.

*******************************************************************************/
LOCAL uchar8 dobhld_fnAddDobToList( PGNDOB *ptzDob, uchar8 ucPriority )
{
    // unsigned int uiIntSts;
    uchar8 ucRetVal;
    DOBHOLDER_tzTX_LINK_LIST *ptzList;

    // Enter critical section
    // Avoid interruptions while we manipulate the LL
    // uiIntSts = __disable_interrupts();

    // Get local pointer to the desired Tx LL for convenience
    if( ucPriority == XB_REG_BUF )
    {
        ptzList = &PGNDOB_tzRegularLinkList;
    }
    else
    {
        ptzList = &PGNDOB_tzVipLinkList;
    }

    ucRetVal = NO_NODE_LINK;

    // Is there room in the list?
    if( ( ptzList->ucUsedCount < ptzList->ucTotalNodes ) &&
        ( ptzList->ucFreeCount > 0 ) )
    {
        // Pointer to the current last used node - if there is one
        DOBHOLDER_tzLL_DOB_NODE *pLastUsedNode;
        uchar8 ucFirstFree = ptzList->ucFirstFree;
        // The new used node will be the first free node
        DOBHOLDER_tzLL_DOB_NODE *pNewUsedNode = &ptzList->ptzNodes[ ptzList->ucFirstFree ];

        if( ptzList->ucUsedCount > 0 )
        {
            // Get the last used node for linking the new node to it
            pLastUsedNode = &ptzList->ptzNodes[ ptzList->ucLastUsed ];
            // Create the link between the existing last used and the first free node
            pNewUsedNode->ucPrev = ptzList->ucLastUsed;
            pLastUsedNode->ucNext = ucFirstFree;
        }
        else
        {
            // Adding the first used node
            ptzList->ucFirstUsed = ucFirstFree;
            pNewUsedNode->ucPrev = NO_NODE_LINK;
            pLastUsedNode = pNewUsedNode;
        }

        // More than one free node?
        if( ptzList->ucFreeCount > 1 )
        {
            // The prev of the new first free node has no link
            ptzList->ptzNodes[ pNewUsedNode->ucNext ].ucPrev = NO_NODE_LINK;
        }
        // else there will be no more free nodes to link after the add

        // Change the index to the first free node
        ptzList->ucFirstFree = pNewUsedNode->ucNext;
        // Change the index to the last used node
        ptzList->ucLastUsed = ucFirstFree;
        // Mark the new used node as being the last used; it has no Next link
        pNewUsedNode->ucNext = NO_NODE_LINK;
        // Store the pointer to the DOB in this new used node
        pNewUsedNode->ptzDob = ptzDob;
        // Store the LL node index in the DOB
        ptzDob->m_ucListIdx = ucFirstFree;
        // Increase the used count
        ptzList->ucUsedCount++;
        // Decrease the free count
        ptzList->ucFreeCount--;

        if( ptzList->ucFreeCount == 0 )
        {
            ptzList->ucFirstFree = NO_NODE_LINK;
            ptzList->ucLastFree = NO_NODE_LINK;
        }

        // Debug code - check that after removal the used count plus the free count
        // equal with total count
        assert( ( ptzList->ucFreeCount + ptzList->ucUsedCount ) == ptzList->ucTotalNodes );

        ucRetVal = ucFirstFree;
    }

    // Exit critical section
    // __restore_interrupts( uiIntSts );

    return ucRetVal;
}


/***************************************************************************//**

  Remove a Tx Pending DOB node from the linked-list

  @param pDob - pointer to the DOB to be removed/deleted

  @retval TRUE  for success
  @retval FALSE for failure.

  @details
    There are 3 different cases:
        1. the node to be removed is the first used
        2. the node to be removed is in the middle of the list
        3. the node to be removed is the last used

  @note
    Does not need to be protected aganinst interruptions as the caller already
    has critical section protection.

*******************************************************************************/
LOCAL tucBOOL dobhld_fnRemoveNodeFromList( DOBHOLDER_tzLL_DOB_NODE *ptzNode,
                                           uchar8 ucPriority )
{
    DOBHOLDER_tzTX_LINK_LIST *ptzList;
    DOBHOLDER_tzLL_DOB_NODE *pLastFreeNode;
    uchar8 ucThisNodeIdx;
    tucBOOL tucStatus = TRUE;

    // Get local pointer to the desired Tx LL for convenience
    if( ucPriority == XB_REG_BUF )
    {
        ptzList = &PGNDOB_tzRegularLinkList;
    }
    else
    {
        ptzList = &PGNDOB_tzVipLinkList;
    }

    if( ptzList->ucUsedCount == 0 )
    {
        tucStatus = FALSE;
    }
    else if( ptzList->ucUsedCount == 1 )
    {
        // There is only one node left which is both first and last

        // Debug code
        if( ptzList->ucLastUsed != ptzList->ucFirstUsed )
        {
            tucStatus = FALSE;
        }
        // This is the first used node
        ucThisNodeIdx = ptzList->ucFirstUsed;
        // This node will have no next link
        ptzNode->ucNext = NO_NODE_LINK;
        // No more used nodes
        ptzList->ucLastUsed = NO_NODE_LINK;
        ptzList->ucFirstUsed = NO_NODE_LINK;
    }
    else if( ptzNode->ucPrev == NO_NODE_LINK )
    {
        // This is the first used of many nodes, there is no prev node
        // This node's next
        DOBHOLDER_tzLL_DOB_NODE *pNextUsedNode = &ptzList->ptzNodes[ ptzNode->ucNext ];

        // Make sure it is the first used node
        if( ptzNode != &ptzList->ptzNodes[ ptzList->ucFirstUsed ] )
        {
            tucStatus = FALSE;
        }

        // Debug code: pNextUsedNode->ucPrev should be equal with ucFirstUsed
        if( pNextUsedNode->ucPrev != ptzList->ucFirstUsed )
        {
            tucStatus = FALSE;
        }

        // Save the index to this node
        ucThisNodeIdx = ptzList->ucFirstUsed;
        // The new first used is this node's next node
        ptzList->ucFirstUsed = ptzNode->ucNext;
        // The prev of the next node will now have no link
        pNextUsedNode->ucPrev = NO_NODE_LINK;
        // This node will have no next link
        ptzNode->ucNext = NO_NODE_LINK;
    }
    else if( ptzNode->ucNext == NO_NODE_LINK )
    {
        // This is the last used of many nodes; there is no next node
        // This node's prev
        DOBHOLDER_tzLL_DOB_NODE *pPrevUsedNode = &ptzList->ptzNodes[ ptzNode->ucPrev ];

        // Make sure it is the last used node
        if( ptzNode != &ptzList->ptzNodes[ ptzList->ucLastUsed ] )
        {
            tucStatus = FALSE;
        }
        // Debug code: pPrevUsedNode->ucNext should be equal with ptzList->ucLastUsed
        if( pPrevUsedNode->ucNext != ptzList->ucLastUsed )
        {
            tucStatus = FALSE;
        }

        // Save the index to this node
        ucThisNodeIdx = ptzList->ucLastUsed;
        // Point the last used to the prev of this node
        ptzList->ucLastUsed = ptzNode->ucPrev;
        // The prev node's next will have no link
        pPrevUsedNode->ucNext = NO_NODE_LINK;
        // This node's next remains without link
    }
    else
    {
        // The node to remove is in the middle
        // Need to link this node's previous node with its next node
        DOBHOLDER_tzLL_DOB_NODE *pPrevUsedNode = &ptzList->ptzNodes[ ptzNode->ucPrev ];
        DOBHOLDER_tzLL_DOB_NODE *pNextUsedNode = &ptzList->ptzNodes[ ptzNode->ucNext ];

        // Debug code: pPrevUsedNode->ucNext should be equal with pNextUsedNode->ucPrev
        if( pPrevUsedNode->ucNext != pNextUsedNode->ucPrev )
        {
            tucStatus = FALSE;
        }

        // Save the index to this node
        ucThisNodeIdx = pPrevUsedNode->ucNext;
        // Link this nodes prev and next nodes
        pPrevUsedNode->ucNext = ptzNode->ucNext;
        pNextUsedNode->ucPrev = ptzNode->ucPrev;
    }

    if( tucStatus == TRUE )
    {
        if( ptzList->ucFreeCount > 0 )
        {
            // There are already free nodes
            pLastFreeNode = &ptzList->ptzNodes[ ptzList->ucLastFree ];
            // Add to free list
            pLastFreeNode->ucNext = ucThisNodeIdx;
            // This node's prev is the last free
            ptzNode->ucPrev = ptzList->ucLastFree;
        }
        else
        {
            // This will be the first free node in the list

            // This node becomes the first and the last free
            pLastFreeNode = ptzNode;
            // The first free will be this node index
            ptzList->ucFirstFree = ucThisNodeIdx;
            // SInce this node becames the first free there is no prev
            ptzNode->ucPrev = NO_NODE_LINK;
        }

        // Decrement the used count
        ptzList->ucUsedCount--;
        ptzNode->ucNext = NO_NODE_LINK;
        ptzList->ucLastFree = ucThisNodeIdx;
        // Increment the free count
        ptzList->ucFreeCount++;
    }

    // Debug code - check that after removal the used count plus the free count equal with max count
    if( ( ptzList->ucFreeCount + ptzList->ucUsedCount ) !=
        ptzList->ucTotalNodes )
    {
        tucStatus = FALSE;
    }

    return tucStatus;
}


/***************************************************************************//**

  Find and remove the specified DOB from the linked list

  @param pDob - pointer to the DOB to be removed

  @details
    Only the DOBs in the linked list which are in the process of being
    transmitted are to be erased; the DOB was either transmitted in full
    or is removed due to timeout.
    The DOB is only removed from the linked-list.

*******************************************************************************/
LOCAL tucBOOL dobhld_fnRemoveDob( PGNDOB *pDob, uchar8 ucPriority )
{
    // unsigned int uiIntSts;
    uchar8 i;
    uchar8 ucNextNode;
    tucBOOL tucStatus;
    DOBHOLDER_tzTX_LINK_LIST *ptzList;
    DOBHOLDER_tzLL_DOB_NODE *pNode;

    // Enter critical section
    // Avoid interruptions while we manipulate the LL
    // uiIntSts = __disable_interrupts();

    tucStatus = TRUE;
    if( pDob->m_ucListIdx != NO_NODE_LINK )
    {
        if( ucPriority == XB_VIP_BUF )
        {
            ptzList = &PGNDOB_tzVipLinkList;
        }
        else
        {
            ptzList = &PGNDOB_tzRegularLinkList;
        }
        pNode = &ptzList->ptzNodes[ pDob->m_ucListIdx ];
        // Check that this is the specified DOB
        if( pNode->ptzDob == pDob )
        {
            // Mark the DOB pointer as unused (NULL)
            pNode->ptzDob = NULL;
            // Remove this node from the used list and add it to the free list
            if( dobhld_fnRemoveNodeFromList( pNode, ucPriority ) != TRUE )
            {
                tucStatus = FALSE;
            }
        }
        else
        {
            assert( pNode->ptzDob == pDob );
        }
    }
    else
    {
        if( pDob->m_ucFlags & DOB_IS_VIP )
        {
            ucPriority = XB_VIP_BUF;
            ptzList = &PGNDOB_tzVipLinkList;
        }
        else
        {
            ucPriority = XB_REG_BUF;
            ptzList = &PGNDOB_tzRegularLinkList;
        }

        pNode = &ptzList->ptzNodes[ ptzList->ucFirstUsed ];

        for( i=0; i < ptzList->ucUsedCount; i++ )
        {
            if( pNode->ptzDob == pDob )
            {
                // Mark the DOB pointer as unused (NULL)
                pNode->ptzDob = NULL;
                // Remove this node from the used list and add it to the free list
                if( dobhld_fnRemoveNodeFromList( pNode, ucPriority ) != TRUE )
                {
                    tucStatus = FALSE;
                }
                // Done - exit the loop
                break;
            }
            else
            {
                ucNextNode = pNode->ucNext;
                if( ucNextNode != NO_NODE_LINK )
                {
                    // Check the next node in the linked-list
                    pNode = &ptzList->ptzNodes[ ucNextNode ];
                }
                else
                {
                    tucStatus = FALSE;
                }
            }
        }
    }

    // Exit critical section
    // __restore_interrupts( uiIntSts );

    return tucStatus;
}


//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
