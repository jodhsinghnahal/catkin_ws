/*==============================================================================
Copyright © 2005 Xantrex Technology, Inc. All rights reserved.

This source file and the information contained in it are confidential and
proprietary to Xantrex Technology, Inc. The reproduction or disclosure, in
whole or in part, to anyone outside of Xantrex without the written approval of
a Xantrex officer under a Non-Disclosure Agreement, or to any employee of
Xantrex who has not previously obtained written authorization for access from
the individual responsible for the source code, will have a significant
detrimental effect on Xantrex and is expressly prohibited.

FILE NAME:
    ExtMemUTMain.c

PURPOSE:
    Simple unit test for the External Memory Interface.

FUNCTION(S):

    local:
        none

NOTES:

	none
	
CHANGE HISTORY:
$Log: $


==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/
#include "init.h"
#include "Device.h"

/*==============================================================================
                              Defines
==============================================================================*/
#define EXTMEM_MEMORY_SIZE     ( 65536 )	// size of the memory being tested
#define EXTMEM_TEST_SET_SIZE   ( 257 )		// size of the test data set
#define EXTMEM_LOCATION        ( 0x108000 ) // start address of the test memory
#define EXTMEM_TEST_DATA       ( 0x8000 )   // location of some test data

/*==============================================================================
                            Type Definitions
==============================================================================*/

/*==============================================================================
                                Enums
==============================================================================*/

/*=============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                           Public Variables
==============================================================================*/

/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/

/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/
void extmem_fnTest( void );

/*==============================================================================
                           Function Definitions
==============================================================================*/
main(void)
{
	// perform system initializations
    INIT_fnSysInit();

	// run the memory tests	
    while( TRUE )
    {
        // test all locations in the memory (write and read)
        extmem_fnTest();
    }
}

/******************************************************************************

FUNCTION NAME:
    extmem_fnTest

PURPOSE:
    Test the external memory

INPUTS:
    None

OUTPUTS:
    None

NOTES:


VERSION HISTORY:

Version: 1.00   Date:   Feb 18, 2005    By: Trevor monk
    - Created

******************************************************************************/
void extmem_fnTest( void )
{
	Uint16 *puiMem;
	Uint32 len;
	Uint16 values[EXTMEM_TEST_SET_SIZE];
	Uint16 idx;
	static Uint32 testnum = 1;
	static Uint32 numPassed = 0;
	static Uint32 numFailed = 0;
	static Uint16 offset = 0;
	Uint16 result;

	result = 1;

	// populate the test array with some code
	// the array length of 257 is chosen because it is a prime and it will not
	// fit exactly into the 64K memory space we are testing
	// each time we populate the test array we shuffle the test data by one word
	// so that it is different every time
	offset = ( ++offset % EXTMEM_TEST_SET_SIZE );
	
	puiMem = (Uint16 *)EXTMEM_TEST_DATA;
	for( idx=0; idx<EXTMEM_TEST_SET_SIZE; idx++ )
	{
		values[(idx + offset ) % EXTMEM_TEST_SET_SIZE] = *puiMem++;
	}
		
	// write to the entire memory
	idx = 0;
	puiMem = (Uint16 *)EXTMEM_LOCATION;
	for( len=0; len < EXTMEM_MEMORY_SIZE; len++ )
	{
		*puiMem++ = values[idx];
		if (idx++ >= EXTMEM_TEST_SET_SIZE)
		{
			idx = 0;
		}
	}
	
	// now read it all back and check for errors
	idx = 0;
	puiMem = (Uint16 *)EXTMEM_LOCATION;
	for( len=0; len < EXTMEM_MEMORY_SIZE; len++ )
	{
		// check that the value we wrote in the previous step is
		// what we expect it to be.
		if (values[idx] != *puiMem)
		{
			// signal a test error
			result = 0;
		}
		
		// move to next test location
		if (idx++ >= EXTMEM_TEST_SET_SIZE)
		{
			// wrap the test data back to the beginning
			idx = 0;
		}
			
		// move to the next memory location to test
		puiMem++;
	}
	
	// now clear the entire memory
	puiMem = (Uint16 *)EXTMEM_LOCATION;	
	for( len=0; len < EXTMEM_MEMORY_SIZE; len++ )
	{
		*puiMem++ = 0x00;
	}

	// generate statistics	
	if (result)
	{
		numPassed++;
	}
	else
	{
		numFailed++;
	}
	
_MEM_TEST_DONE:
	testnum++;
	return;
}



