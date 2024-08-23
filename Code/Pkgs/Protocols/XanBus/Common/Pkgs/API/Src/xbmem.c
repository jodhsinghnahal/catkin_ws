/******************************************************************************

  (c) 2003 - 2020 Xantrex Canada ULC. All rights reserved.
  All trademarks are owned or licensed by Xantrex Canada ULC,
  its subsidiaries or affiliated companies.

****************************************************************************//**

  @addtogroup xbmem XBMEM
  @{

    @brief Xanbus Heap Memory Management

****************************************************************************//**

  @page xbmem_design XanBus Heap Memory Management

    Implements the XanBus heap management API.

  @par API Functions
    XBMEM_fnCtor        - Creator for XanBus Memory heap
    XBMEM_fnDtor        - Release the resource used by XanBus Memory heap
    XBMEM_fnMalloc      - Allocate a block of memory from the heap
    XBMEM_fnFree        - Free a block of memory back to the heap

  @par Local Functions
    xbmem_fnCheckHighWater - Update high water mark

  @note
    Memory protection macros default to the xbplatform functions without redefinition.
    Windows and Linux use the same xbplatform functions for the memory protection
    macros.

*******************************************************************************/


/*==============================================================================
                              Includes
==============================================================================*/
#include <string.h>
#include "xbldefs.h"        // Local Definitions
#include "xassert.h"        // Debugging

/*==============================================================================
                              Defines
==============================================================================*/

#define XBMEM_FREE 0
#define XBMEM_USED 1

#define XBMEM_HEAP_MIN      1024     // Lowest limit of heap size

//! Minimum memory block allocation size set to 4 to reduce heap fragmentation
#define XB_HEAP_MALLOC_SIZE_MIN_BLOCKS  ( 2u )

// Macros for turning off interrupts around critical section code

// The macros are NOT REENTRANT
#ifdef HCS12
#define  XBMEM_mINIT_CRITICAL()
#define  XBMEM_mENTER_CRITICAL()  __asm psha; __asm tpa; __asm sei; __asm staa xbmem_ucSr; __asm pula;
#define  XBMEM_mEXIT_CRITICAL()   __asm psha; __asm ldaa xbmem_ucSr; __asm psha; __asm pulc; __asm pula;
#define  XBMEM_mDELETE_CRITICAL()
#else
#ifdef __HCS12X__
#define  XBMEM_mINIT_CRITICAL()
#define  XBMEM_mENTER_CRITICAL()  __asm psha; __asm tpa; __asm sei; __asm staa xbmem_ucSr; __asm pula;
#define  XBMEM_mEXIT_CRITICAL()   __asm psha; __asm ldaa xbmem_ucSr; __asm psha; __asm pulc; __asm pula;
#define  XBMEM_mDELETE_CRITICAL()
#else
#ifdef __TMS320C28XX__
#define  XBMEM_mINIT_CRITICAL()
#define  XBMEM_mENTER_CRITICAL()    asm(" .global _xbmem_uiSt1    ");   \
                                    asm(" PUSH  ST1               ");   \
                                    asm(" MOV   AL, *-SP[1]       ");   \
                                    asm(" POP   ST1               ");   \
                                    asm(" MOVW  DP, #_xbmem_uiSt1 ");   \
                                    asm(" MOV   @_xbmem_uiSt1, AL ");   \
                                    asm(" DINT                    ");
#define  XBMEM_mEXIT_CRITICAL()     asm(" .global _xbmem_uiSt1    ");   \
                                    asm(" MOVW  DP, #_xbmem_uiSt1 ");   \
                                    asm(" TBIT  @_xbmem_uiSt1, #0 ");   \
                                    asm(" SBF   #2, TC            ");   \
                                    asm(" EINT                    ");
#define  XBMEM_mDELETE_CRITICAL()
#else
#ifdef __LM__
#include "device.h"
#define  XBMEM_mINIT_CRITICAL()
#define  XBMEM_mENTER_CRITICAL()  DEVICE_mGLOBAL_INT_DIS()
#define  XBMEM_mEXIT_CRITICAL()   DEVICE_mGLOBAL_INT_EN()
#define  XBMEM_mDELETE_CRITICAL()
#else
#ifdef __LPC2468__
#include "device.h"
#define  XBMEM_mINIT_CRITICAL()
#define  XBMEM_mENTER_CRITICAL()  DEVICE_mGLOBAL_INT_DIS()
#define  XBMEM_mEXIT_CRITICAL()   DEVICE_mGLOBAL_INT_EN()
#define  XBMEM_mDELETE_CRITICAL()
#endif // __LPC2468
#endif // __LM__
#endif // __TMS320C28XX__
#endif // HCS12X
#endif // HCS12

// The default for operating systems like Windows and Linux
#ifndef  XBMEM_mINIT_CRITICAL
void XBPLATFORM_fnInitCritical( void );
#define  XBMEM_mINIT_CRITICAL()   XBPLATFORM_fnInitCritical()
#endif

#ifndef  XBMEM_mENTER_CRITICAL
void XBPLATFORM_fnEnterCritical( void );
#define  XBMEM_mENTER_CRITICAL()  XBPLATFORM_fnEnterCritical()
#endif

#ifndef  XBMEM_mEXIT_CRITICAL
void XBPLATFORM_fnLeaveCritical( void );
#define  XBMEM_mEXIT_CRITICAL()   XBPLATFORM_fnLeaveCritical()
#endif

#ifndef  XBMEM_mDELETE_CRITICAL
void XBPLATFORM_fnDeleteCritical( void );
#define  XBMEM_mDELETE_CRITICAL() XBPLATFORM_fnDeleteCritical()
#endif


/*==============================================================================
                            Local Prototypes
==============================================================================*/

static void xbmem_fnCheckHighWater( void );

/*==============================================================================
                            Type Definitions
==============================================================================*/

typedef struct XBMEM_zHEADER *XBMEM_tpzHEADER;

typedef struct XBMEM_zHEADER
{
  uchar8 ucInUse;
  XBMEM_tpzHEADER tpzNext;
  uint16    siSize;
} XBMEM_tzHEADER;

#define XBMEM_HEADER_SIZE ( sizeof( XBMEM_tzHEADER ) )

/*==============================================================================
                              Public Variables
==============================================================================*/

// Heap start and size, which are initialized by XBMEM_fnCtor()
static uint16 XBMEM_uiHeapSize = 0;
static void *XBMEM_pvHeapStart = NULL;
static uchar8 XBMEM_ucPriorityDobEnable = XBMEM_VIP_BUF_DISABLE;

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

static XBMEM_tpzHEADER xbmem_tpzHeapEnd  = NULL; // pointer
static XBMEM_tpzHEADER xbmem_tpzFreePtr  = NULL; // pointer to free list
static uint16 xbmem_uiUsedBlocks;

#ifndef NDEBUG
static uint16 xbmem_uiMallocCount=0;
static uint16 xbmem_uiFreeCount =0;
#endif

// This is the saved status register for the critical macros.
// This value cannot be saved on the stack because the macro pushes onto
// the stack and changes the compilers sense of stack position
// The macros are not reentrant!!!
#ifdef HCS12
static uchar8 xbmem_ucSr;
#else
#ifdef __HCS12X__
static uchar8 xbmem_ucSr;
#else
#ifdef __TMS320C28XX__
uint16 xbmem_uiSt1;
#endif // __TMS320C28XX__
#endif // HCS12X
#endif // HCS12

/*==============================================================================
                           Function Definitions
==============================================================================*/

/*******************************************************************************

FUNCTION NAME:
    XBMEM_fnCtor        - Creator for XanBus Memory heap

PURPOSE:
    This function is use to setup the XanBus memory heap. This function
    should be called only once at startup. Calling this more than once
    could result in loss of data.

INPUTS:
    'pvHeapStart' points to the start of the XanBus heap
    'uiHeapSize' is the number of bytes in the XanBus heap

OUTPUTS:
    TRUE if successful,
    FALSE if some step of initialization failed.

NOTES:

VERSION HISTORY:

Version: 1.00  Date: Jun 03, 2004  By: Dalem
    - Code moved out of XBINIT_fnInit
Version: 1.01  Date: Jun 23, 2004  By: Dalem
    - Added critical section code
    - Added asserts and removed XBINIT_fnSaveError call
Version 1.02  Date: 06/29/05  By: Hollyz
    - Added _mINIT_CRITICAL macro to initialize the critical section
Version 1.03  Date: 06/13/06  By: Hollyz
    - Initialize the comm statistic for .uiHeapFreeMin

*******************************************************************************/

tucBOOL XBMEM_fnCtor( void *pvHeapStart,
                      uint16 uiHeapSize )
{
    printf("jsdf\n");
    assert( pvHeapStart != NULL );
    assert( uiHeapSize > XBMEM_HEAP_MIN );

    // Validate parameters
    if (( pvHeapStart == NULL )
        || ( uiHeapSize < XBMEM_HEAP_MIN ))
    {
        // Failure
        return( FALSE );
    }

    if( XBMEM_pvHeapStart == NULL )
    {
        // first time heap creation,
        // initialize the critical section one time only
        XBMEM_mINIT_CRITICAL();
    }

    XBMEM_mENTER_CRITICAL();

    // clear the memory contents on the heap
    ( void )memset( pvHeapStart, 0, uiHeapSize );

    // Save the heap start and size
    XBMEM_pvHeapStart = pvHeapStart;
    XBMEM_uiHeapSize = uiHeapSize;

    // Initialize the comm statistic for .uiHeapFreeMin
    // this is moved to XBMSG_fnCtor()
    //XBMSG_tzXbCommStats.uiHeapFreeMin = XBMEM_uiHeapSize;

    //--- Setup heap ---
    // Zero the stats
    xbmem_uiUsedBlocks = 0;

    // The free pointer points to the top of the heap
    xbmem_tpzFreePtr          = (XBMEM_tpzHEADER)XBMEM_pvHeapStart;
    xbmem_tpzFreePtr->siSize  = XBMEM_uiHeapSize / XBMEM_HEADER_SIZE;

    xbmem_tpzFreePtr->tpzNext = NULL;
    xbmem_tpzFreePtr->ucInUse = XBMEM_FREE;

    // The heap end points to the last block on the botom
    xbmem_tpzHeapEnd          = xbmem_tpzFreePtr + xbmem_tpzFreePtr->siSize - 1;

    XBMEM_mEXIT_CRITICAL();

    // Success
    return ( TRUE );
}

/*******************************************************************************

FUNCTION NAME:
    XBMEM_fnDtor        - Release XanBus Memory heap

PURPOSE:
    This function is use to release the resource used by Xanbus Memory heap.

INPUTS:
    none

OUTPUTS:
    none

NOTES:
    This function is only used by Windows application now.

VERSION HISTORY:

Version: 1.00  Date: 06-22-05   By: Hollyz
    - Created

*******************************************************************************/
void XBMEM_fnDtor( void )
{
    XBMEM_mENTER_CRITICAL();

    XBMEM_uiHeapSize = 0;
    XBMEM_pvHeapStart = NULL;
    xbmem_tpzHeapEnd  = NULL;
    xbmem_tpzFreePtr  = NULL;
    xbmem_uiUsedBlocks = 0;

    XBMEM_mEXIT_CRITICAL();

    // release the resource used by critical section object
	XBMEM_mDELETE_CRITICAL();
}


/******************************************************************************

FUNCTION NAME:
    XBMEM_fnMalloc

PURPOSE:
    This is XanBus' on version of malloc(), it returns a pointer of
    contiguous memory of the specified size in bytes.

INPUTS:
    'NumBytes' is the number of bytes to get from the XanBus heap

OUTPUTS:
    A pointer to the memory block if successful
    NULL if a free block could not be found

NOTES:

VERSION HISTORY:

Version 1.00  Date: 12/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: Jun 23, 2004  By: Dalem
    - Added critical section code
    - Added asserts and removed XBINIT_fnSaveError call

******************************************************************************/

void *XBMEM_fnMalloc( uint16 NumBytes )
{
    uint16 NumUnits;
    XBMEM_tpzHEADER p;
    XBMEM_tpzHEADER pl;

    // Ask for nothing, get nothing
    if ( NumBytes == 0 )
    {
        return NULL;
    }

    // Is there any free memory left in the heap?
    if (xbmem_tpzFreePtr == NULL)
    {
        // free list is empty; no more memory
        XBMSG_tzXbCommStats.uiMallocFail++;
        return (NULL);
    }

#ifndef NDEBUG
    xbmem_uiMallocCount++;
#endif

    // Calculate the number of memory blocks required
    NumUnits = ( ( NumBytes + XBMEM_HEADER_SIZE - 1 ) / XBMEM_HEADER_SIZE ) + 1;
    // Ensure number of units is a multiple of XB_HEAP_MALLOC_SIZE_MIN_BLOCKS
    NumUnits = (uint16)(((uint16)(NumUnits >> XB_HEAP_MALLOC_SIZE_MIN_BLOCKS) + 1) << XB_HEAP_MALLOC_SIZE_MIN_BLOCKS);
    // This can be initialized outside critical code
    pl = NULL;

    // Turn off interrupts
    XBMEM_mENTER_CRITICAL();

    // There is some free memory but is there a block that is
    // the right size?  Scan from the beginning of the free list
    p  = xbmem_tpzFreePtr;
    while ( p != NULL )
    {
        // Is this block big enough?
        if ( p->siSize >= NumUnits )
        {
            // Is the block bigger than what we really need?
            if ( p->siSize > NumUnits )
            {
                // block is bigger, so break it
                p[ NumUnits ].tpzNext = p->tpzNext;           // link to next free block
                p[ NumUnits ].siSize  = p->siSize - NumUnits; // size of new free block
                p[ NumUnits ].ucInUse = XBMEM_FREE;
                p->siSize = NumUnits;                    // new size of block
                p->tpzNext = p + NumUnits;               // ptr to next free block
            }

            // Are we still at the first block?
            if ( pl == NULL )
            {
                // we use the first block
                xbmem_tpzFreePtr = p->tpzNext;
            }
            else
            {
                pl->tpzNext = p->tpzNext;
            }

            XBMEM_mEXIT_CRITICAL();

            // This block from the free list had better be really free!
            assert( p->ucInUse == XBMEM_FREE );

            // Unlink the block
            p->tpzNext = NULL;

            // and mark it as used
            p->ucInUse = XBMEM_USED;

            // Track the number of used blocks
            xbmem_uiUsedBlocks += NumUnits;
            xbmem_fnCheckHighWater();

            // Return a pointer to the data block just after the header
            return ( ++p );
        }

        // next block
        pl = p;
        p  = p->tpzNext;
    }

    XBMEM_mEXIT_CRITICAL();

    // All free blocks are too small
    XBMSG_tzXbCommStats.uiMallocFail++;
    return ( NULL );
}


/******************************************************************************

FUNCTION NAME:
    XBMEM_fnFree

PURPOSE:
    This is XanBus' on version of free(), it returns a block of memory
    that had been malloc'd from the XanBus heap

INPUTS:
    'pvMemBlock' points to the memory block to return to the heap

OUTPUTS:
    Nothing

NOTES:

VERSION HISTORY:

Version 1.00  Date: 12/12/03  By: Tom Lightfoot
    - Creation
Version: 1.01  Date: Jun 23, 2004  By: Dalem
    - Added critical section code
    - Added asserts and removed XBINIT_fnSaveError call

******************************************************************************/

void XBMEM_fnFree( void *pvMemBlock )
{
    XBMEM_tpzHEADER p;
    XBMEM_tpzHEADER pNext;
    XBMEM_tpzHEADER pPrev;
    XBMEM_tpzHEADER pEnd;
    XBMEM_tpzHEADER pPrevEnd;

    // Ignore attempted return of null blocks
    if ( pvMemBlock == NULL )
    {
        return;
    }

#ifndef NDEBUG
    xbmem_uiFreeCount++;
#endif

    // Cast the block pointer as a header pointer
    p = (XBMEM_tpzHEADER)pvMemBlock;

    // And slide back to where the header is
    --p;

    // Make sure the block is marked as being in use
    assert( p->ucInUse != XBMEM_FREE );

    // Make sure the pointer points to memory INSIDE the heap
    assert( p >= (XBMEM_tpzHEADER) XBMEM_pvHeapStart );
    assert( p <= xbmem_tpzHeapEnd );

    // Make sure the pointer points to memory INSIDE the heap
    if ( ( p < (XBMEM_tpzHEADER)XBMEM_pvHeapStart )
      || ( p > xbmem_tpzHeapEnd ) )
    {
        return;
    }

    // Update the memory use stats
    xbmem_uiUsedBlocks -= p->siSize;

    // Mark the block as being unused
    p->ucInUse = XBMEM_FREE;
    p->tpzNext = NULL;

    XBMEM_mENTER_CRITICAL();

    // Handle return to an empty free list
    if ( xbmem_tpzFreePtr == NULL )
    {
        // freelist is empty
        xbmem_tpzFreePtr = p;

        XBMEM_mEXIT_CRITICAL();
        return;
    }

    // Start looking for where to return the returned memory block.
    // We want to re-link it in address order
    pNext = xbmem_tpzFreePtr;
    pPrev = NULL;
    while (( pNext != NULL ) && ( pNext < p ))
    {
        pPrev = pNext;
        pNext = pNext->tpzNext;
    }

    pEnd = p + p->siSize;
    if (pNext == pEnd )
    {
        // concatenate p and pNext
        p->siSize += pNext->siSize;
        p->tpzNext = pNext->tpzNext;
    }
    else if ( pNext == p )
    {
        // block to be released was already in free list
        assert( pNext == p );
    }
    else if (( pEnd > pNext ) && ( pNext != NULL ))
    {
        // block to be released was completely contained in element in free list
        assert( ( pEnd > pNext ) && ( pNext != NULL ) );
    }
    else
    {
        // Link the block into the tail of the list
        p->tpzNext = pNext;
    }

    if ( pPrev == NULL )
    {
        // p must be the first free block in the list
        xbmem_tpzFreePtr = p;
        XBMEM_mEXIT_CRITICAL();
        return;
    }

    pPrevEnd = pPrev + pPrev->siSize;
    if ( p == pPrevEnd )
    {
        // concatenate pPrev and p
        pPrev->siSize += p->siSize;
        pPrev->tpzNext = p->tpzNext;
        XBMEM_mEXIT_CRITICAL();
        return;
    }

    if ( pPrevEnd > p )
    {
        // start of block to be released was contained in element in free list
        assert( pPrevEnd > p );
        XBMEM_mEXIT_CRITICAL();
        return;
    }

    // Link the head of the list to the returned block
    pPrev->tpzNext = p;
    XBMEM_mEXIT_CRITICAL();
}

/******************************************************************************

FUNCTION NAME:
    xbmem_fnCheckHighWater

PURPOSE:
    This function updates the high water mark of the number of bytes
    allocated on the heap, including the header overhead.

INPUTS:
    None

OUTPUTS:
    Nothing

NOTES:
    The high water mark updated is the number that is transmitted in
    the XanBus Comm Stats PGN.

VERSION HISTORY:

Version 1.00  Date: 12/15/03  By: Tom Lightfoot
    - Creation
Version 1.01  Date: 04/12/06  By: Holly Zhou
    - Report .uiHeapFreeMin instead of .uiMallocHighWater

******************************************************************************/

static void xbmem_fnCheckHighWater( void )
{
    uint16 BytesLeft;    // Number of bytes left on Heap

    // Calculate the number of bytes free on XB heap
    BytesLeft = XBMEM_uiHeapSize - sizeof( XBMEM_tzHEADER ) * xbmem_uiUsedBlocks;

    // Is this less than the previous Heap free minimum ?
    if ( BytesLeft < XBMSG_tzXbCommStats.uiHeapFreeMin )
    {
        // Update with the new minimum available bytes on Heap
        XBMSG_tzXbCommStats.uiHeapFreeMin = BytesLeft;
    }
}


uint16 XBMEM_fnGetFreeBytes( void )
{
  uint16 BytesLeft;    // Number of bytes left on Heap

  XBMEM_mENTER_CRITICAL();

  // Calculate the number of bytes free on XB heap
  BytesLeft = XBMEM_uiHeapSize - sizeof( XBMEM_tzHEADER ) * xbmem_uiUsedBlocks;

  XBMEM_mEXIT_CRITICAL();

  return BytesLeft;

}

#ifndef NDEBUG
void XBMEM_fnGetCount( uint16 *pMalloc, uint16 *pFree )
{
     *pMalloc = xbmem_uiMallocCount;
     *pFree = xbmem_uiFreeCount;
}
#endif

void XBMEM_fnSetPriorityDobStatus( uchar8 status )
{
     XBMEM_ucPriorityDobEnable = status;
}

uchar8 XBMEM_fnGetPriorityDobStatus( void )
{
     return XBMEM_ucPriorityDobEnable;
}


//*****************************************************************************
// End of file.
//! @}
//*****************************************************************************
