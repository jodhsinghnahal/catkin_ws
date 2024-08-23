#include "stdafx.h"

#include "USBFT.H"

#include "apox.h"
#include "candrv.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  CID_PRIORITY_BITOFFSET  26
#define  CID_RESERVED_BITOFFSET  25
#define  CID_DATAPAGE_BITOFFSET  24
#define  CID_FORMAT_BITOFFSET    16
#define  CID_SPECIFIC_BITOFFSET  8
#define  CID_SRCADDR_BITOFFSET   0

static char ** pucSnBuf;
static CUsbFt * pcUsbFt = NULL;

unsigned long APOX_fnListUSBCANBoards( void )
{
    CUsbFt cUsbFt;
    unsigned long ulNumDevs;
    
    if ( cUsbFt.ListUSBCANBoards( &ulNumDevs, &pucSnBuf ) == FT_OK )
    {
        return ulNumDevs;
    }
    
    return 0;
}

unsigned int APOX_fnInstantiate( unsigned char ucBitRate )
{
	CString cstrUSB;
	USB_CAN_FILTERS tzUsbCanFilters;
	
	cstrUSB.Format( _T("%s"), *pucSnBuf ); 
    
    if ( pcUsbFt == NULL )
    {
        pcUsbFt = new CUsbFt( cstrUSB );
    }
    
    if ( pcUsbFt != NULL )
    {
        if ( FT_SUCCESS( pcUsbFt->InitUSBDevice() ) )
        {
	    Sleep(500); // Allow things to settle down
            pcUsbFt->SwitchToMainCode();
	    Sleep(250); // wait for micro to reset
            
            pcUsbFt->Set29BitMode();
            if( ucBitRate == CANDRV_eBIT_RATE_125KBPS )
			{	
                pcUsbFt->SetUsbCanBaudRate( pcUsbFt->CAN_BAUD_125 );
			}
			else if( ucBitRate == CANDRV_eBIT_RATE_250KBPS )
			{	
                pcUsbFt->SetUsbCanBaudRate( pcUsbFt->CAN_BAUD_250 );
			}
			else if( ucBitRate == CANDRV_eBIT_RATE_500KBPS )
			{	
                pcUsbFt->SetUsbCanBaudRate( pcUsbFt->CAN_BAUD_500 );
			}
			else if( ucBitRate == CANDRV_eBIT_RATE_1MBPS )
			{	
                pcUsbFt->SetUsbCanBaudRate( pcUsbFt->CAN_BAUD_1000 );
			}
			else 
			{
                printf( "Bit Rate not supported\r\n" );
				// Bit rate not supported
				return (FALSE);
			}
            
            tzUsbCanFilters.rx_buff1_ext = TRUE;
            tzUsbCanFilters.rx_buff2_ext = TRUE;
            tzUsbCanFilters.RXF0 = 0xFFFFFFFF;
            tzUsbCanFilters.RXF1 = 0xFFFFFFFF;
            tzUsbCanFilters.RXF2 = 0xFFFFFFFF;
            tzUsbCanFilters.RXF3 = 0xFFFFFFFF;
            tzUsbCanFilters.RXF4 = 0xFFFFFFFF;
            tzUsbCanFilters.RXF5 = 0xFFFFFFFF;
            tzUsbCanFilters.RXM0 = 0x80000000;
            tzUsbCanFilters.RXM1 = 0x80000000;
            
            pcUsbFt->WriteFilterData( &tzUsbCanFilters );
            pcUsbFt->SetNormalMode();
            
            return TRUE;
        }
    }
    
    return FALSE;
}

void APOX_fnClose( void )
{
    if ( pcUsbFt != NULL )
    {
        pcUsbFt->AbortAllCanTransmissions();

	// DNunes - adding call to delete pcUsbFt
	delete pcUsbFt;
	pcUsbFt = NULL;
    }
}

unsigned int APOX_fnStatus( void )
{
    unsigned short usComStatReg;
    
    if ( pcUsbFt == NULL )
    {
        return APOX_STS_BUS_OFF;
    }
    
    if ( pcUsbFt->RequestCanStatus( &usComStatReg ) == pcUsbFt->USB_OK )
    {
        if ( ( usComStatReg & 0x20 ) != 0 )
        {
            return APOX_STS_BUS_OFF;
        }
        
        if ( ( usComStatReg & 0x10 ) != 0 )
        {
            return APOX_STS_ERR_PASSIVE;
        }
        
        return APOX_STS_ERR_ACTIVE;
    }
    
    return APOX_STS_BUS_OFF;
}

unsigned int APOX_fnReadCANMsg( unsigned char * pucDataPage,
                                unsigned char * pucPriority,
                                unsigned char * pucPktSpecific,
                                unsigned char * pucPktFmt,
                                unsigned char * pucSrcAddr,
                                unsigned char * pucData,
                                unsigned char * pucDataLen )
{
    RxData tzRxData;
    
    if ( pcUsbFt == NULL )
    {
        return FALSE;
    }
    
    if ( pcUsbFt->ReadCANMsg( &tzRxData ) == pcUsbFt->USB_OK )
    {
        *pucDataPage = ( unsigned char )( ( tzRxData.Id >> CID_DATAPAGE_BITOFFSET ) & 0x01 );
        
        *pucPriority = ( unsigned char )( tzRxData.Id >> CID_PRIORITY_BITOFFSET );
        
        *pucPktSpecific = ( unsigned char )( tzRxData.Id >> CID_SPECIFIC_BITOFFSET );
        
        *pucPktFmt = ( unsigned char )( tzRxData.Id >> CID_FORMAT_BITOFFSET );
        
        *pucSrcAddr = ( unsigned char )( tzRxData.Id >> CID_SRCADDR_BITOFFSET );
        
        *pucDataLen = ( unsigned char )( tzRxData.len % 9 );
        
        memcpy( pucData, tzRxData.data, *pucDataLen );
        
        return TRUE;
    }

    return FALSE;
}

unsigned long APOX_fnSendRawCANMessage( unsigned char ucDataPage,
                                        unsigned char ucPriority,
                                        unsigned char ucPktSpecific,
                                        unsigned char ucPktFmt,
                                        unsigned char ucSrcAddr,
                                        unsigned char * pucData,
                                        unsigned char ucDataLen )
{
    unsigned long ulId = 0;
    
    if ( pcUsbFt == NULL )
    {
        return FALSE;
    }
    
    // Insert Priority
    ulId |= ( unsigned long )( ucPriority & 0x7 ) << CID_PRIORITY_BITOFFSET;
    
    // Insert DataPage
    ulId |= ( unsigned long )( ucDataPage & 0x1 ) << CID_DATAPAGE_BITOFFSET;
    
    // Insert Pdu Format
    ulId |= ( unsigned long )( ucPktFmt & 0xff ) << CID_FORMAT_BITOFFSET;
    
    // Insert Pdu Specific
    ulId |= ( unsigned long )( ucPktSpecific & 0xff ) << CID_SPECIFIC_BITOFFSET; 
    
    // Insert Source Address
    ulId |= ( unsigned long )( ucSrcAddr & 0xff ) << CID_SRCADDR_BITOFFSET;
    
    if ( pcUsbFt->SendRawCANMessage( FALSE, TRUE, 0, ulId, pucData, ucDataLen ) == pcUsbFt->USB_OK )
    {
        return TRUE;
    }
    
    return FALSE;
}                                                

#ifdef __cplusplus
}
#endif   // _cplusplus
