
#ifndef DEVICE_INCL
#define DEVICE_INCL

/*==============================================================================
                              Includes
==============================================================================*/

/*==============================================================================
                              Defines
==============================================================================*/

/*==============================================================================
                              Macros
==============================================================================*/

// enable interrupts
#define DEVICE_mGLOBAL_INT_EN( )

// disable interrupts
#define DEVICE_mGLOBAL_INT_DIS( )


#define DEVICE_mWD_EN( Rate )

#define DEVICE_mWD_DIS( )

// reset the watchdog timer
#define DEVICE_mWD_RST()
                        
#define DEVICE_mSHUTDOWN()

// trigger a Watchdog Reset
#define DEVICE_mWD_TRIGGER()

// reset the Device 
#define DEVICE_mCPU_RST()

#endif  // DEVICE_INCL
