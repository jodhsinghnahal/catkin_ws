/*******************************************************************
 *
 *    DESCRIPTION:	CUsbFt class which use FTD2XX.DLL to send USB msg out to 
 *					CAN bus and receive responses.
 *
 
Unsolicited Emergency Message from the PIC
----------------------------
[0] 0xFF
[1] 0x80
[2] ERRORCODE (0-255)


Config message to PIC
----------------------
[0] 0x00
[1] command | 0x80
[2..n] any data needed

Response to config message from PIC
------------------------
[0] 0x00
[1] command | 0x80
[2..n] response data

Outgoing CAN message
--------------------------
[0] ([1][RTR][EXT][unused 0..4])
[1] ID MSB
[2] ID
[3] ID
[4] ID LSB
[5] FUTURE USE (CANopen or DeviceNet) // ex. Wait for response? Etc..
[6] FUTURE USE (CANopen or DeviceNet)
[7] RESERVED FOR TX FLAGS 
[8] DATA LEN (0-8)
[9-16] DATA BYTES 0 to 8 (if needed) (NOT SENT IF RTR is SET)


Incoming CAN Message
------------------------------
[0] ([1][RTR][EXT][unused 0..4]) (WARNING: never send back 0xFF, its reserved for emergency)
[1] ID MSB
[2] ID
[3] ID
[4] ID LSB
[5] TIMESTAMP MSB
[6] TIMESTAMP 
[7] TIMESTAMP 
[8] TIMESTAMP LSB
[9] RESERVED FOR RX FLAGS
[10] DATA LEN (0-8)
[11-18] DATA BYTES 0 to 8 (if needed)





 *******************************************************************/

#include "StdAfx.h"
#include <iostream>
#include <fstream>
#include "UsbFt.h"
#include <process.h>
#include "TCHAR.h"
#include "reg.h"

// Hack to prevent crash on delete
#define  APOX_MAX_DONGLES        4
#define  USBFT_BUFF_LEN         64

#ifdef __cplusplus
extern "C" {
#endif

char USBFT_acBuffer[ APOX_MAX_DONGLES * USBFT_BUFF_LEN ];
char *USBFT_apcBufPtrs[ APOX_MAX_DONGLES ];

#ifdef __cplusplus
}
#endif   // _cplusplus

void CUsbFt::InternalInit()
{
	CString cStr;
	cStr.Format(_T("RxCharEvent%s"), m_cSerialNumber);
	m_hRxCharEvent = CreateEvent(NULL,FALSE,FALSE, cStr );
	cStr.Format(_T("USBReplyEvent%s"), m_cSerialNumber);
	m_hUSBReplyEvent = CreateEvent(NULL,FALSE,FALSE, cStr );
    m_ftHandle = NULL;
	m_listen = 0;
	m_hTxReplyEvent = NULL;
	m_xMode = MODE_RUN;
	m_pCANCallback = NULL;
	m_pEmergencyCallback = NULL;
	m_pCANCallbackPtr = NULL;
	m_pEmergencyCallbackPtr = NULL;
	m_RxMsgState = USB_IDLE;
	identifier_mode = false;
	InitializeCriticalSection( &m_CRITICAL_SECTION_DataQ );
	InitializeCriticalSection( &m_CRITICAL_SECTION_CANDataQ );
	InitializeCriticalSection( &m_CRITICAL_SECTION_WriteAccess );
	InitializeCriticalSection( &m_CRITICAL_SECTION_Access );
	InitializeCriticalSection( &m_CRITICAL_SECTION_FreeQ );
	m_RxMsgQ.reserve(5000);
	m_USBMsgQ.reserve(5000);
	m_CANMsgQ.reserve(5000);
	m_slRxDataFree = NULL;
	

}
RxData *CUsbFt::GetFreeRXData()
{
	RxData *tmp;
	EnterCriticalSection( &m_CRITICAL_SECTION_FreeQ );
	tmp = (RxData *)m_slRxDataFree;
	if (tmp) {
		m_slRxDataFree = tmp->fwd;
		LeaveCriticalSection( &m_CRITICAL_SECTION_FreeQ );
		return (RxData *)tmp;
	}
	else
	{
		LeaveCriticalSection( &m_CRITICAL_SECTION_FreeQ );
		AddRxMemory(50000);
		return GetFreeRXData();
	}

}
void CUsbFt::FreeRxData(RxData *ptr)
{
    if ( ptr != NULL )
    {
	EnterCriticalSection( &m_CRITICAL_SECTION_FreeQ );
	ptr->fwd = (RxData *)m_slRxDataFree;
	m_slRxDataFree = ptr;
	LeaveCriticalSection( &m_CRITICAL_SECTION_FreeQ );
    }
}
void CUsbFt::AddRxMemory(int num)
{
	 // this is the line with the memory leak
	//RxData *m_rxMemory = (RxData *)calloc(num, sizeof(RxData));	    
	m_rxMemory = (RxData *)calloc(num, sizeof(RxData));	    

	m_rxMemoryListQ.push_back((char *)m_rxMemory);
	RxData *tmpRxPtr = m_rxMemory;
	EnterCriticalSection( &m_CRITICAL_SECTION_FreeQ );
	for (int i=0; i < num; i++)
	{
		
		tmpRxPtr->fwd = m_slRxDataFree;
		m_slRxDataFree = tmpRxPtr;
		
		++tmpRxPtr;
	}
	LeaveCriticalSection( &m_CRITICAL_SECTION_FreeQ );
}
CUsbFt::CUsbFt(CString &sn)
{
	m_rxMemory = NULL;
	m_cSerialNumber = sn;
	InternalInit();
}
CUsbFt::CUsbFt()
{
	m_rxMemory = NULL;
	m_cSerialNumber = _T("TEMP");
	InternalInit();	
}

CUsbFt::~CUsbFt()
{
#ifdef SIM
	return;
#endif
	StopSyncMechanism();
	Sleep(100);
	if (m_hRxCharEvent)
	{
		m_listen = 0;
		SetEvent(m_hRxCharEvent);
		WaitForSingleObject(m_hReadThread, INFINITE);		
		CloseHandle(m_hRxCharEvent);
	}
	if (m_hUSBReplyEvent)
		CloseHandle(m_hUSBReplyEvent);
	if (m_ftHandle)
		FT_Close(m_ftHandle);

	// deallocate all of the rx buffer memory
	if (m_rxMemory != NULL)
		free( m_rxMemory );

	//m_rxMemoryListQ.erase(m_rxMemoryListQ.begin(), m_rxMemoryListQ.end());


	DeleteCriticalSection( &m_CRITICAL_SECTION_DataQ );
	DeleteCriticalSection( &m_CRITICAL_SECTION_CANDataQ );
	DeleteCriticalSection( &m_CRITICAL_SECTION_WriteAccess );
	DeleteCriticalSection( &m_CRITICAL_SECTION_Access );
	DeleteCriticalSection( &m_CRITICAL_SECTION_FreeQ );
}
BOOL CUsbFt::UpdateSerialNumberToName(CString mapping)
{
	CReg regUSB;
	return(regUSB.WriteString(HKEY_LOCAL_MACHINE, _T("Software\\DipChipElec\\USB-CANtastic\\USB SerialNumbers"), m_cSerialNumber, mapping));
}
CString CUsbFt::MapSerialNumberToName(char *serialNumber)
{
	CString cStrValue, cStrItem;
	cStrItem = serialNumber;
	CReg regUSB(HKEY_LOCAL_MACHINE, _T("Software\\DipChipElec\\USB-CANtastic\\USB SerialNumbers"));
	cStrValue = regUSB.GetString(cStrItem, cStrItem);
	return cStrValue;
}
DWORD CUsbFt::ListUSBCANBoards(DWORD *num, char* **sn)
{
	FT_STATUS ftStatus;
	DWORD numDevs=0, numBoards=0;
	unsigned int i;
	unsigned char which[ APOX_MAX_DONGLES ];
	ftStatus = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
	if (ftStatus == FT_OK)
	{
		if (numDevs > 0)
		{
		    memset( USBFT_acBuffer, 0, sizeof( USBFT_acBuffer ) );
		    memset( USBFT_apcBufPtrs, 0, sizeof( USBFT_apcBufPtrs ) );
		    for (i= 0; (i < numDevs) && (i < APOX_MAX_DONGLES); i++)
			USBFT_apcBufPtrs[ i ] = &USBFT_acBuffer[ i * USBFT_BUFF_LEN ];
			ftStatus = FT_ListDevices(USBFT_apcBufPtrs, &numDevs, FT_LIST_ALL|FT_OPEN_BY_DESCRIPTION);
			if (ftStatus == FT_OK)
			{
			    memset( which, 0, APOX_MAX_DONGLES );
			    for (i= 0; (i < numDevs) && (i < APOX_MAX_DONGLES); i++)
				{
					if (strncmp("USB-CAN", USBFT_apcBufPtrs[i], 7)==0)
					{
					    which[numBoards++]=i;
					}
				}
				if (numBoards > 0) {
					
				    for (i = 0; (i < numBoards) && (i < APOX_MAX_DONGLES); i++)
					{
						ftStatus = FT_ListDevices((void *)which[i], USBFT_apcBufPtrs[i], FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);
						
					}
				}
			}
			*sn = USBFT_apcBufPtrs;
			*num = numBoards;
			return FT_OK;
		}
		else
			return FT_DEVICE_NOT_FOUND;
	}
	else
		return ftStatus;

}
void CUsbFt::DefineUSBEmergencyCallback(TUsbCallback pCallback, void *pCallbackPtr)
{
	m_pEmergencyCallback = pCallback;
	m_pEmergencyCallbackPtr = pCallbackPtr;
}
void CUsbFt::DefineCANMsgCallback(TUsbCallback pCallback, void *pCallbackPtr)
{
	m_pCANCallback = pCallback;
	m_pCANCallbackPtr = pCallbackPtr;
}
DWORD CUsbFt::InitUSBDevice()
{
	FT_STATUS ftStatus, ftStatus2;
	AddRxMemory(50000);
#ifdef SIM
	return 0;
#endif
	if (m_ftHandle)	{
		m_listen = 0;
		SetEvent(m_hRxCharEvent);
		WaitForSingleObject(m_hReadThread, INFINITE);
		ftStatus = FT_ResetDevice( m_ftHandle );
		ftStatus = FT_Purge( m_ftHandle, FT_PURGE_RX | FT_PURGE_TX );
		FT_Close(m_ftHandle);
		m_ftHandle = 0;
	}
	
	
	ftStatus = FT_OpenEx((void *)m_cSerialNumber.GetBuffer(256), FT_OPEN_BY_SERIAL_NUMBER, &m_ftHandle);

	if ( FT_SUCCESS(ftStatus) )
	{
		// reset the USB device
		
		
		ftStatus = FT_ResetDevice( m_ftHandle );
		ftStatus = FT_Purge( m_ftHandle, FT_PURGE_RX | FT_PURGE_TX );
		ftStatus = FT_SetTimeouts(m_ftHandle, 500, 500);
		ftStatus = FT_SetUSBParameters ( m_ftHandle, 8192, 8192 );
		ftStatus2 = FT_SetLatencyTimer( m_ftHandle, 2 ); //setting this to 1 seems to be causing crashes. (Weird, possibly bad D2XX driver!)
        FT_SetResetPipeRetryCount(m_ftHandle, 200 );

		m_listen = 1;

		// start USB listener
		m_hReadThread =  (HANDLE)_beginthread(USBReadThread, 0, (void*)this );
		SetThreadPriority(m_hReadThread, THREAD_PRIORITY_TIME_CRITICAL);
		Sleep(10);
		
	}

	return (DWORD)ftStatus;
}


void CUsbFt::Set11BitMode( void )
{
 identifier_mode = false;
}

void CUsbFt::Set29BitMode( void )
{
 identifier_mode = true;
}


	 // This is a 11 standard message to stay backwards compatible with the old API
	 // Send tx flags = 0x00?
DWORD CUsbFt::SendRawCANMessage(bool rtr, BYTE message, BYTE nodeId, BYTE *data, int dataLen)
{
	return SendRawCANMessage((bool)rtr, (bool)false,(BYTE)0x00, (DWORD)nodeId, data, dataLen);
}

DWORD CUsbFt::SendRawCANMessage(bool rtr, BYTE txFlags, DWORD Id, BYTE *data, unsigned int dataLen)
{
	return SendRawCANMessage(rtr, identifier_mode, txFlags, Id, data, dataLen);
}

DWORD CUsbFt::SendRawCANMessage(bool rtr, bool ext, BYTE txFlags, DWORD Id, BYTE *data, unsigned int dataLen)
{
	BYTE sendData[20];
	int size = 0;


/*
Outgoing CAN message
--------------------------
[0] ([1][RTR][EXT][unused 0..4])
[1] ID MSB
[2] ID
[3] ID
[4] ID LSB
[5] FUTURE USE (CANopen or DeviceNet) // ex. Wait for response? Etc..
[6] FUTURE USE (CANopen or DeviceNet)
[7] RESERVED FOR TX FLAGS 
[8] DATA LEN (0-8)
[9-16] 
*/
	sendData[size++] = 0x80 | (rtr?0x40:0x00) | (ext?0x20:0x00);
	sendData[size++] = (BYTE)(Id>>24) & 0x1f;
	sendData[size++] = (BYTE)(Id>>16) & 0xff;
	sendData[size++] = (BYTE)(Id>>8)  & 0xff;
	sendData[size++] = (BYTE)(Id)     & 0xff;



			
	sendData[size++] = 0x00; // future use
	sendData[size++] = 0x00; // future use

	sendData[size++] = txFlags;

	sendData[size++] = dataLen;

	if (dataLen > 8) return (USB_DATA_TOO_LONG);

	for (unsigned int i=0; i < dataLen; i++)
	{
		sendData[size++] = data[i];
	}

	return ( USBSend( sendData, size) );	
}



DWORD CUsbFt::ReadRxMsg(RxData *pRxData)
{
	if ( !m_RxMsgQ.empty() )
	{
		RxData *Rxdata = m_RxMsgQ.front();
		m_RxMsgQ.erase (m_RxMsgQ.begin());				
		*pRxData = *Rxdata;		
		FreeRxData(Rxdata);
		return USB_OK;
	}
	else
		return 1;
}

DWORD CUsbFt::ReadCANMsg(RxData *pRxData)
{
	
	DWORD status;
	EnterCriticalSection( &m_CRITICAL_SECTION_CANDataQ );

	if ( !m_CANMsgQ.empty() )
	{
		RxData *Rxdata = m_CANMsgQ.front();
		m_CANMsgQ.erase(m_CANMsgQ.begin());				
		*pRxData = *Rxdata;		
		FreeRxData(Rxdata);
		status = USB_OK;
	}
	else
		status = 1;
	LeaveCriticalSection( &m_CRITICAL_SECTION_CANDataQ );
	return status;
		
}


/* This function will grab the next waiting message off of the received message list*/
DWORD CUsbFt::ProcessRxMsg()
{
	RxData rxData;
	while ( ReadRxMsg( &rxData) == USB_OK ) // this line deletes data from the Queue
	{
		if ( rxData.who && m_xMode == MODE_RUN)
		{
			// This data did not come from the CAN bus
			// but is a response to a command sent by the user to the board.
			// or an unsolicted emergency message.
			// So the question is, what should I do with the response???
			// I would like to get the data back to the user who sent it
			// otherwise, how will they user know who sent the data.
			HandleUSBCANMessage(&rxData);

		}
		else if ( m_xMode == MODE_RUN )
		{
			RxData *pNewRxData;
			pNewRxData = GetFreeRXData();
			*pNewRxData = rxData;
			
			EnterCriticalSection( &m_CRITICAL_SECTION_CANDataQ );
			m_CANMsgQ.push_back(pNewRxData);
			LeaveCriticalSection( &m_CRITICAL_SECTION_CANDataQ );
			if (m_pCANCallback)
				m_pCANCallback(m_pCANCallbackPtr, &rxData);			
		}
		else if ( m_xMode == MODE_DOWNLOAD_USB )
		{
			if ( rxData.len == 1 && rxData.data[0] == '*' )
				SetEvent(m_hTxReplyEvent);
			else
			{
////  100 USB_EXPECTING_DLE (This code was expecting a DLE)
////  101 USB_EXPECTING_STX (This code was expecting a STX)
////  102 USB_BAD_CHECKSUM  (This code's checksum did not match messages)
////  103 USB_EXPECTING_ETX (This code was expecting a ETX)
////  104 USB_BAD_STATE     (Unknown state in the receive state machine)
////  105 USB_MSG_TOO_BIG   (Too many bytes received)
////  106 BAD_FLASH_WRITE   (A Write to Flash Failed )
////  107 ADDR_TOO_BIG      (Address in or above bootcode)
				int ii;
				ii = rxData.data[0];
				cout<<"Bootloader returned an error %d"<<ii<<endl;				
			}
		}
	}

	// after all message have been processed, loop through all nodes and if its SDO dataQ
	//	is not empty, set event to signal the response from the node.


	return USB_OK;
}
DWORD CUsbFt::WaitForUSBResponse(BYTE message, BYTE *pData)
{
	DWORD status=-1;
#ifdef SIM
	return 0;
#endif
	EnterCriticalSection( &m_CRITICAL_SECTION_DataQ );
		// should check for proper message - should be my response
	if ( !m_USBMsgQ.empty() )
	{
		while ((!m_USBMsgQ.empty()) && (status != USB_OK))
		{
			RxData *QData = m_USBMsgQ.front();
			m_USBMsgQ.erase(m_USBMsgQ.begin());
			if (( QData != NULL )
			    && (QData->command == message))
			{
				memcpy(pData, QData->data, QData->len); //sizeof(QData->data));
				FreeRxData(QData);
				status = USB_OK;
			}
			else
			{
			 // What do we do here! throw the message away?
				status = USB_OK;
			}

		}
		LeaveCriticalSection( &m_CRITICAL_SECTION_DataQ );
	}
	else
	{
		LeaveCriticalSection( &m_CRITICAL_SECTION_DataQ );
	}
	
	if (status != USB_OK)
	{
		int rc = WaitForSingleObject( m_hUSBReplyEvent, 1000);

		if (rc != WAIT_OBJECT_0)
		{
			status = USB_TIMEOUT;
		}
		else
		{
			EnterCriticalSection( &m_CRITICAL_SECTION_DataQ );
			// should check for proper message - should be my response
			if ( !m_USBMsgQ.empty() )
			{
				RxData *QData = m_USBMsgQ.front();
				m_USBMsgQ.erase(m_USBMsgQ.begin());
				if (( QData != NULL ) && ( QData->command == message ))
				{
					memcpy(pData, QData->data, QData->len);
					status = 0;
				}
				FreeRxData(QData);
				
			}			
			else
			{
				status = 1; // What is this error
			}

			LeaveCriticalSection( &m_CRITICAL_SECTION_DataQ );		
		}
	}
	
	return status;
}
DWORD CUsbFt::USBSendAndReceive(BYTE *sData, int dataSize, BYTE *pData)
{
	DWORD status;
	int tries=0;
	int done=0;
	EnterCriticalSection( &m_CRITICAL_SECTION_Access );
	do {
		status = USBSend(sData, dataSize);
		if (status != USB_OK)
		{
			LeaveCriticalSection( &m_CRITICAL_SECTION_Access );
			return status;
		}
		else
		{
			do
			{
				status= WaitForUSBResponse(sData[1] & 0x7f, pData);
			} while ((status != USB_TIMEOUT) && (status != USB_OK));
			if (status == USB_OK)
				done = 1;

		}
	} while (++tries < 3 && !done);
	LeaveCriticalSection( &m_CRITICAL_SECTION_Access );
	return status;

}


DWORD CUsbFt::USBSend( BYTE *data, int dataSize)
{
	BYTE	buffer[300];
	DWORD	retLen;
	DWORD	size = 0;
	BYTE	chksum = 0;
	DWORD	status;
#ifdef SIM
	return 0;
#endif
	// BYTE STUFFING
	//----------------------------------------------
	// message format
	// DLE STX data CSUM DLE ETX
	// but if data contains a DLE
	// then insert another DLE before it (BYTE Stuffing)
	// this function is not concerned with the data format
	// other than the fact that it adds a checksum to the end

	// start the transmission
	EnterCriticalSection( &m_CRITICAL_SECTION_WriteAccess );
	buffer[size++] = USB_DLE;
	buffer[size++] = USB_STX;  // start transmission

	// BYTE Stuff the data and calc checksum
	for (int i=0; i<dataSize; i++)
	{
		chksum ^= data[i];

		if (data[i] == USB_DLE)
		{
			buffer[size++] = USB_DLE;
		}
		buffer[size++] = data[i];
	}

	// BYTE STUFF checksum if necessary
	if (chksum == USB_DLE )
	{
		buffer[size++] = USB_DLE;
	}
	// Send the check sum
	buffer[size++] = chksum;

	// terminate the transmission
	buffer[size++] = USB_DLE;
	buffer[size++] = USB_ETX;  // end transmission

	if( m_ftHandle )
	{
		if ( (status = FT_Write(m_ftHandle, buffer, size, &retLen)) == FT_OK )
			status = USB_OK;
		else
			status = USB_CANNOT_TRANSMIT;
	}
	else
		status = USB_ERR_NO_USB;
	LeaveCriticalSection( &m_CRITICAL_SECTION_WriteAccess );
	return status;
}



/* The following function is a thread that runs continously trying to read USB data 
It will wait for a received character event from the FTDI library,
After it receives the data, there are two modes
MODE_DOWNLOAD_USB will just put the data into a data queue
MODE_RUN will BYTE DE_STUFF the message
  and when a complete message is received It packs the data into an RxData format
  and stuffs it into the received m_RxMsgQ Queue
	
  Then it will call ProcessRxMsg to handle the message

*/

void CUsbFt::USBReadThread( void *pArg)
{
	CUsbFt* pThis = (CUsbFt*)pArg;

	FT_STATUS ftStatus;
	DWORD	EventMask = FT_EVENT_RXCHAR ;

	if ( pArg == NULL )
	{
	    return;
	}

	ftStatus = FT_SetEventNotification(pThis->m_ftHandle, EventMask, pThis->m_hRxCharEvent);
	int max=0;
	//DWORD EventDWord;
	DWORD RxBytes; //, TxBytes;
	DWORD ByteReceived;
	DWORD ProcessStatus;
	BYTE RxBuffer[65536];
	BYTE RxMsgData[32768];
	int  RxMsgLen;

	while (pThis->m_listen)
	{
		ftStatus = FT_GetQueueStatus( pThis->m_ftHandle, &RxBytes );
		if (ftStatus != 0)
				TRACE("%d\r\n", max);
		if ( RxBytes == 0 )
		{
			WaitForSingleObject(pThis->m_hRxCharEvent,INFINITE);
			ftStatus = FT_GetQueueStatus( pThis->m_ftHandle, &RxBytes );
			if (ftStatus != 0)
				TRACE("%d\r\n", max);
			
		}
		if ( RxBytes > 0)
		{
			// read USB data
			ftStatus = FT_Read(pThis->m_ftHandle, RxBuffer, RxBytes, &ByteReceived);
			if ( ByteReceived > 0 && ftStatus == FT_OK)
			{
				if ((int)ByteReceived > max)
				{
					max = ByteReceived;
					TRACE("%d\r\n", max);
				}
				if ( pThis->m_xMode == MODE_DOWNLOAD_USB )
				{
					RxData *data = pThis->GetFreeRXData();
					if ( data != NULL )
					{
					    data->Id   = 0;
					    data->rtr  = (bool)false;					
					    data->who      = true;
					    data->len = (BYTE)ByteReceived;
					    data->rxflags = 1;
					    for (DWORD pp=0; pp<ByteReceived; pp++)
						data->data[pp] = RxBuffer[pp];
					}

					// put the data into the data queue
					pThis->m_RxMsgQ.push_back(data);
				}
				else	// MODE_RUN and MODE_DOWNLOAD_CAN
				{
					
					for (DWORD i=0; i<ByteReceived; i++)
					{
						ProcessStatus = pThis->USBProcessByte( RxBuffer[i], &RxMsgLen, RxMsgData);

						if (ProcessStatus != USB_OK )
						{
						   // If we are in here, then we got a bad message
						   // maybe we should push the message onto the emergency stack??                             
							
						}
						else if (pThis->m_RxMsgState == USB_MESSAGE_COMPLETE)
						{
							RxData *data = pThis->GetFreeRXData();
							
							// clear all of the fields in the new RxData
							memset(data,0,sizeof(RxData));

							/*
							For messages returned from CAN bus, the format will be:

							0:[who|rtr|idMode|(unused)]
							1:[ID MSB]
							2:[ID3]
							3:[ID2]
							4:[ID LSB]
							5:[TIMESTAMP MSB]
							6:[TIMESTAMP LSB]
							7:[RX_FLAGS]
							8:[LENGTH]
							9-16:[DATA 0-8 bytes]
							*/

							// For messages returned from the PIC, the format will be
							/*Unsolicited Emergency Message from the PIC
							 ----------------------------
							 [0] 0xFF
							 [1] 0x80
							 [2] ERRORCODE (0-255)
							 
							 
							 Config message to PIC
							 ----------------------
							 [0] 0x00
							 [1] command | 0x80
							 [2..n] any data needed
							 
							 Response to config message from PIC
							 ------------------------
							 [0] 0x00
							 [1] command | 0x80
							 [2..n] response data							 
							*/


							// fill the RxData structure with USB data		
							if ((RxMsgData[0] == 0x00) || (RxMsgData[0] == 0xFF))
                                data->who = true;
							else
								data->who = false;
							

							if (data->who==false) // this node is from the CAN bus.
							{
								data->rtr       = (RxMsgData[0] & 0x40)?true:false;
								data->ext	    = (RxMsgData[0] & 0x20)?true:false;
								data->Id        = (((DWORD)RxMsgData[4]<<24) & 0x1f000000) |
												  (((DWORD)RxMsgData[3]<<16) & 0x00ff0000) |
												  (((DWORD)RxMsgData[2]<<8 ) & 0x0000ff00) |
												  (((DWORD)RxMsgData[1]    ) & 0x000000ff) ;
									              										
								data->timestamp = (((DWORD)RxMsgData[8]<<24 ) & 0xff000000) |
												  (((DWORD)RxMsgData[7]<<16 ) & 0x00ff0000) |
												  (((DWORD)RxMsgData[6]<<8 )  & 0x0000ff00) |
												  (((DWORD)RxMsgData[5]    )  & 0x000000ff) ;								

								data->rxflags     = RxMsgData[9];
								data->len		  = RxMsgData[10];
																	
								for (int h=0; h<data->len; h++)
									data->data[h] = RxMsgData[h+11];
							}
							else // this data is from the USB-CAN board
							{		
								data->Id      = RxMsgData[0];
								data->command = RxMsgData[1] & 0x7F;									
								data->len = RxMsgLen-2; //(minus two for the first two bytes)
								for (int jj=0; jj<data->len; jj++)
									data->data[jj] = RxMsgData[jj+2];
							}
														
							// put the data into the data queue
							pThis->m_RxMsgQ.push_back(data);
							pThis->m_RxMsgState = USB_IDLE; // reset state for next msg
						}
					}
				}
			}

			// then call the ProcessRxMsg function to process the data
			pThis->ProcessRxMsg();
		}
	}
}





/*------------------------------------------------------------------------- 
Process BYTE does the actual BYTE De-stuffing of the incoming data from the
USB port 
---------------------------------------------------------------------------*/
DWORD CUsbFt::USBProcessByte( BYTE inByte, int *RxMsgLen, BYTE *RxMsgData )
{   
	static unsigned char chksum;
	DWORD status = USB_OK;

	switch (m_RxMsgState)
	{
	case USB_IDLE:
		if (inByte == USB_DLE)
		{
			m_RxMsgState = USB_FRAME_START;
			chksum = 0;
			*RxMsgLen = 0;
		}
		else
		{
			status = USB_EXPECTING_DLE;
			m_RxMsgState = USB_IDLE;
		}
		break;
	case USB_FRAME_START:
		if (inByte == USB_STX)
		{
			m_RxMsgState = USB_FRAME;
		}
		else
		{
			status = USB_EXPECTING_STX;
			m_RxMsgState = USB_IDLE;
		}
		break;
	case USB_FRAME:
		if (inByte == USB_DLE)
		{
			m_RxMsgState = USB_FRAME_NLE;
		}
		else
		{
			RxMsgData[(*RxMsgLen)++] = inByte;
			chksum ^= inByte;
		}
		break;
	case USB_FRAME_NLE:
		if (inByte == USB_ETX)
		{
			if (!chksum) // checksum should be zero if message was good
			{
				m_RxMsgState = USB_MESSAGE_COMPLETE;
				(*RxMsgLen)--; // we are always 1 ahead
			}
			else
			{
 				status = USB_BAD_CHECKSUM;
				m_RxMsgState = USB_IDLE;
			}
		}
		else if (inByte == USB_STX)
		{
			status = USB_EXPECTING_ETX;    // Not expecting a NLE-STX combination
			m_RxMsgState = USB_IDLE;
		}
		else
		{
			RxMsgData[(*RxMsgLen)++] = inByte;
			chksum ^= inByte;
			m_RxMsgState = USB_FRAME;
		}
		break;
	case  USB_MESSAGE_COMPLETE:
	default:
		status = USB_BAD_STATE; // should never get here
		m_RxMsgState = USB_IDLE;
		break;
	}  // end switch
	
	return status;
}


DWORD CUsbFt::SendCommand( BYTE code  )
{
	BYTE txBuf[5];   
	int size = 0;
	DWORD status;
	
	txBuf[size++] = 0x00;
	txBuf[size++] = code | 0x80;
	status = USBSend( txBuf, size );
	return status;
}

void CUsbFt::SwitchToBootCode()
{	
	ResetMicrocontroller(); // this will start the bootcode by resetting the microcontroller
}


void CUsbFt::SwitchToMainCode()
{
	bool maincoderunning;
	
	if (AskWhichCodeIsRunning(&maincoderunning) == USB_OK)
	{
		if (!maincoderunning) {
			SendCommand('R'); //, false);
			Sleep(450);
		}
	}	
}


void CUsbFt::GetReadyToDownloadUSB()
{
	CString cStr;
	cStr.Format(_T("TxReplyEvent%s"), m_cSerialNumber);
	m_hTxReplyEvent = CreateEvent(NULL,FALSE,FALSE, cStr );
	m_xMode = MODE_DOWNLOAD_USB;

	// switch USB-CAN processor to BootCode state
	SwitchToBootCode();
}


void CUsbFt::EndDownload()
{
   int lastmode;

    lastmode = m_xMode;

	if (m_hTxReplyEvent)
	{
		CloseHandle(m_hTxReplyEvent);
		m_hTxReplyEvent = NULL;
	}
	m_xMode = MODE_RUN;

	// switch USB-CAN processor to MainCode state
	if (lastmode == MODE_DOWNLOAD_USB)
	{
		SwitchToMainCode();
	}
}

DWORD CUsbFt::ReadHexFile(char *fileName, BYTE *xdata)
{
	bool hex_eof = false;	
	DWORD data_byte;
	DWORD byte_length;
	DWORD address;
	DWORD extendedaddress = 0 ;
	DWORD ex_addr;
	char *sptr;
	char str[100];
	char temp[100];
	

	// read the hax file and put the data into xdata[]
	ifstream hexFile;
	hexFile.open(fileName, ios::in);
	if ( !hexFile.is_open() )
	{
		return USB_ERR_NO_HEXFILE;
	}
	else
	{
		while (!(hexFile.eof()) && !hex_eof)   
		{
			hexFile.getline(str, 100, '\n');	

			// Get the record length
			temp[0] = str[1];
			temp[1] = str[2];
			temp[2] = '\0';
			sscanf(temp,"%x",&byte_length);
    
			// Get the address
		    temp[0] = str[3];
		  	temp[1] = str[4];
			temp[2] = str[5];
			temp[3] = str[6];
			temp[4] = '\0';
			sscanf(temp,"%x",&address);

			if (str[7]=='0' && str[8]=='0') // this string contains normal data
			{
				// scan through string, and write the data byte to flash		 		
				for (DWORD pos=0; pos<byte_length; pos++)
				{				          	    
    				sptr = &str[2*pos+9];
					   
					//generate the data field
					temp[0] = *sptr++;
					temp[1] = *sptr;
					temp[2] = '\0';
					sscanf(temp,"%x",&data_byte);					
					ex_addr = extendedaddress;
					ex_addr <<= 16;
					ex_addr |= address; 
					if (ex_addr < (0x7FFF-64))
  					   xdata[ex_addr] = (BYTE)data_byte;					
					address++;
				}		
			}	
			else if (str[7]=='0' && str[8]=='4') // this string contains extended addressing mode
			{	  
				// Get the extended address
				temp[0] = str[9];
  				temp[1] = str[10];
				temp[2] = str[11];
  				temp[3] = str[12];
				temp[4] = '\0';
				sscanf(temp,"%x",&extendedaddress);
			}
			else  if (str[7]=='0' && str[8]=='1') // this string contains end of file
			{
				// "END OF HEX FILE FOUND"
				hex_eof = true;
			}
		}
		
		return USB_OK;
	}
}

DWORD CUsbFt::StartDownloadUSB( char *filename )
{
	bool liveone;
	int i,j,size;

	DWORD wordAddress;
	unsigned char str[100];
	BYTE xdata[65536];
	DWORD status;

	// read the hax file
	for (i=0; i<65536; i++)
	   xdata[i] = 0xff;
	//status = ReadHexFile("USB-CAN.HEX", xdata);
	status = ReadHexFile(filename, xdata);
	
	if (status != USB_OK)
		return status;

	// start a download
	GetReadyToDownloadUSB();
	SendCommand( USB_START_DOWNLOAD );
	int numpackages = 0;

	// The format that we are sending to the WARP board
	// is 'address(2bytes),up to 32words(64bytes) for a total of 66 bytes
	for (i=0; i<65536; i+=64)
	{
		liveone = false;
		size = 0;
		wordAddress = i;
 		str[size++]=(char)(wordAddress & 0x000000ff);
		str[size++]=(char)((wordAddress>>8) & 0x000000ff);
	
	    for (j=0; j<64; j++)
		{		
			if ( xdata[i+j] != 0xff)
				liveone = true; // this means at least part of the address range has data to write
			str[size++] = xdata[i+j];		 
		}

		if (liveone)
		{
			numpackages++;
			USBSend(str,size);

			int tryNumber = 3;

			while ( tryNumber )
			{
				int rc = WaitForSingleObject( m_hTxReplyEvent, 1000);

				if (rc != WAIT_OBJECT_0)
				{
					USBSend(str, size);	   
					tryNumber--;
				}
				else
					break;
			}
			
			if (tryNumber == 0)
			{
				status = USB_ERR_FT;
				SendCommand( USB_END_DOWNLOAD );
				EndDownload();
				return status;
			}
		}
	}

	// End the download
	SendCommand( USB_END_DOWNLOAD );
	EndDownload();

	return status;
}






void CUsbFt::HandleUSBCANMessage(RxData *pRxData)
{
	RxData *pNewRxData;

	if ( pRxData == NULL )
	{
	    return;
	}

	if (pRxData->Id != 0) 
	{
		if (m_pEmergencyCallback)
			m_pEmergencyCallback(m_pEmergencyCallbackPtr, pRxData);
		return;
	}
	pNewRxData = GetFreeRXData();
	*pNewRxData = *pRxData;
	EnterCriticalSection( &m_CRITICAL_SECTION_DataQ );
	m_USBMsgQ.push_back(pNewRxData);
	LeaveCriticalSection( &m_CRITICAL_SECTION_DataQ );
	SetEvent(m_hUSBReplyEvent);
}


// This function requests the TXERRCNT register
DWORD CUsbFt::RequestTxErrCnt(unsigned short *m_iTxErrCnt)
{
	DWORD status;
	BYTE data[255];
	status = SendUsbCanCmd(GET_TX_ERR_CNT, data);
	if (status == USB_OK)
		if (m_iTxErrCnt)
			*m_iTxErrCnt = data[0];
	return status;
}


// This function requests the RXERRCNT register
DWORD CUsbFt::RequestRxErrCnt(unsigned short *m_iRxErrCnt)
{
	DWORD status;
	BYTE data[255];
	status = SendUsbCanCmd(GET_RX_ERR_CNT, data);
	if (status == USB_OK)
		if (m_iRxErrCnt) *m_iRxErrCnt = data[0];
	return status;
}


// this function requests the CANSTAT register
DWORD CUsbFt::RequestCanMode(unsigned short *m_iCANSTAT)
{
	DWORD status;
	BYTE data[255];
	status = SendUsbCanCmd(GET_CANSTAT, data);
	if (status == USB_OK)
		if (m_iCANSTAT) *m_iCANSTAT = data[0];
	return status;
}

// this function requests the COMSTAT register
DWORD CUsbFt::RequestCanStatus(unsigned short *m_iCOMSTAT)
{
	DWORD status;
	BYTE data[255];
	status = SendUsbCanCmd(GET_COMSTAT, data);
	if (status == USB_OK)
		if (m_iCOMSTAT) *m_iCOMSTAT = data[0];
	return status;
}


// This function aborts all transmissions that are already in the CAN transmit buffer
DWORD CUsbFt::AbortAllCanTransmissions()
{
	BYTE data[255];
	return(SendUsbCanCmd(ABORT_ALL_TX, data));	
}


DWORD CUsbFt::SetConfigMode(void)
{
	BYTE data[255];
	return(SendUsbCanCmd(SET_CONFIG_MODE, data));	
}

DWORD CUsbFt::SetNormalMode(void)
{
	BYTE data[255];
	return(SendUsbCanCmd(SET_NORMAL_MODE, data));	
}

DWORD CUsbFt::SetListenOnlyMode(void)
{
	BYTE data[255];
	return(SendUsbCanCmd(SET_LISTEN_MODE, data));	
}

// SLEEP MODE??
DWORD CUsbFt::SetDisabledMode(void)
{
	BYTE data[255];
	return(SendUsbCanCmd(SET_SLEEP_MODE, data));
}

DWORD CUsbFt::SetLoopBackMode(void)
{
	BYTE data[255];
	return(SendUsbCanCmd(SET_LOOPBACK_MODE, data));
}

// this function querys the USB-CAN board, to find out
// if the board is running the bootcode or the main code
DWORD CUsbFt::AskWhichCodeIsRunning(bool *iMainCode)
{
	DWORD status;
	BYTE data[255];
	status = SendUsbCanCmd(WHICH_CODE_IS_RUNNING, data);
	if (status == USB_OK) {
		if (data[0] == 0xcc)
			*iMainCode = true;
		else if (data[0] == 0x55)
			*iMainCode = false;
		else
			status = USB_BAD_STATE;
	}

	return status;
		
}

DWORD CUsbFt::ResetMicrocontroller()
{
	return SendUsbCanCmd(RESET_MICRO, 0);
}


DWORD CUsbFt::RequestHardwareVersion(char *pHW, int size)
{
	BYTE data[255];
	DWORD status;
	status = SendUsbCanCmd(GET_HARDWARE_VERSION, data);
	if (status == USB_OK)
	{
		for (int i=0;i<size;i++)
			pHW[i] = data[i];
		if (size >= 5)	
			pHW[4] = '\0';
	}
	return status;
}


DWORD CUsbFt::RequestFirmwareVersion(char *pHW, int size)
{
	BYTE data[255];
	DWORD status;
	status = SendUsbCanCmd(GET_FIRMWARE_VERSION, data);	
	if (status == USB_OK)
	{
		for (int i=0;i<size;i++)
			pHW[i] = data[i];
		if (size >= 5)	
			pHW[4] = '\0';
	}
	return status;
}




DWORD CUsbFt::SetUsbCanBaudRate( int rate )
{
	BYTE data[255];
	BYTE command;
	switch (rate)
	{
	case 0:
		command = SET_BAUD_1MEG;		
		break;
	case 1:
		command = SET_BAUD_500K;		
		break;
	case 2:
		command = SET_BAUD_250K;		
		break;
	case 3:
		command = SET_BAUD_125K;		
		break;
	}
	return(SendUsbCanCmd(command, data));
}

DWORD CUsbFt::GetBaudRegisters(int *BRGCON1, int *BRCON2, int *BRCON3)
{
	BYTE data[255];
	DWORD status;
	status = SendUsbCanCmd(GET_BAUD_REGS, data);
	if (status == USB_OK)
	{
		*BRGCON1 = data[0];
		*BRCON2 = data[1];
		*BRCON3 = data[2];
	}
	return status;
}
DWORD CUsbFt::RequestFilterUpdate(USB_CAN_FILTERS *pUSBCANFilter)
{
	 // We need to keep the number of data bytes <= 8bytes
	 // so the filters are stored in two locations

	BYTE data[255];
	DWORD status;

	if ( pUSBCANFilter == NULL )
	{
	    return USB_OK;
	}

	status = SetConfigMode();
	if (status != USB_OK)
		return status;
	
	status = SendUsbCanCmd(GET_MSGFILTER1, data);		
	if (status == USB_OK)
	{
		pUSBCANFilter->RXM0 = data[3]<<24 | data[2]<<16 | data[1]<<8 | data[0]; 
		pUSBCANFilter->RXM1 = data[7]<<24 | data[6]<<16 | data[5]<<8 | data[4]; 
		pUSBCANFilter->RXF0 = data[11]<<24 | data[10]<<16 | data[9]<<8 | data[8]; 
		pUSBCANFilter->RXF1 = data[15]<<24 | data[14]<<16 | data[13]<<8 | data[12]; 
		
		status = SendUsbCanCmd(GET_MSGFILTER2, data);
		if (status == USB_OK)
		{
			pUSBCANFilter->RXF2 = data[3]<<24 | data[2]<<16 | data[1]<<8 | data[0]; 
			pUSBCANFilter->RXF3 = data[7]<<24 | data[6]<<16 | data[5]<<8 | data[4]; 
			pUSBCANFilter->RXF4 = data[11]<<24 | data[10]<<16 | data[9]<<8 | data[8]; 
			pUSBCANFilter->RXF5 = data[15]<<24 | data[14]<<16 | data[13]<<8 | data[12]; 			
		}
	}
	return status;			
}


DWORD CUsbFt::WriteFilterData(USB_CAN_FILTERS *pUSBCANFilter)
{
	BYTE txdata[100];
	BYTE iData[255];
    int size = 0;
	DWORD status;

	if ( pUSBCANFilter == NULL )
	{
	    return USB_OK;
	}

	status = SetConfigMode(); // first make sure we are in config mode
	if (status != USB_OK)
		return status;

	 // now write the filter data	
	txdata[size++] = 0x00;
	txdata[size++] = (SET_MSGFILTER1) | 0x80; // who = USB_CAN command = 0x02
    txdata[size++] = (BYTE)(pUSBCANFilter->RXM0>>24); 
	txdata[size++] = (BYTE)(pUSBCANFilter->RXM0>>16);
	txdata[size++] = (BYTE)(pUSBCANFilter->RXM0>>8); 
	txdata[size++] = (BYTE)pUSBCANFilter->RXM0;

	txdata[size++] = (BYTE)(pUSBCANFilter->RXM1>>24); 
	txdata[size++] = (BYTE)(pUSBCANFilter->RXM1>>16);
	txdata[size++] = (BYTE)(pUSBCANFilter->RXM1>>8);
	txdata[size++] = (BYTE)pUSBCANFilter->RXM1;
	
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF0>>24); 
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF0>>16);
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF0>>8);	
	txdata[size++] = (BYTE)pUSBCANFilter->RXF0;	
	
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF1>>24); 
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF1>>16);
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF1>>8);
	txdata[size++] = (BYTE)pUSBCANFilter->RXF1;
	
	status = USBSendAndReceive( txdata, size, iData);
	if (status != USB_OK)
		return status;

    size = 0;	
	txdata[size++] = 0x00;
	txdata[size++] = (SET_MSGFILTER2) | 0x80; // who = USB_CAN command = 0x02
	
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF2>>24); 
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF2>>16);
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF2>>8);
	txdata[size++] = (BYTE)pUSBCANFilter->RXF2;

	txdata[size++] = (BYTE)(pUSBCANFilter->RXF3>>24); 
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF3>>16);
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF3>>8);
	txdata[size++] = (BYTE)pUSBCANFilter->RXF3;
	
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF4>>24); 
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF4>>16);
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF4>>8);
	txdata[size++] = (BYTE)pUSBCANFilter->RXF4;
	
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF5>>24); 
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF5>>16);
	txdata[size++] = (BYTE)(pUSBCANFilter->RXF5>>8);
	txdata[size++] = (BYTE)pUSBCANFilter->RXF5;
    
	status = USBSendAndReceive( txdata, size, iData);
	return status;

	 // tell the UI to update the screen	
}


DWORD CUsbFt::SendUsbCanCmd( BYTE code, BYTE *data )
{
	BYTE txdata[8];
	
	txdata[0]= 0;
	txdata[1] = code | 0x80; // who = USB_CAN command = 0x02
	if (data)
		return USBSendAndReceive(txdata, 2, data);
	else
		return USBSend( txdata, 2);
}
DWORD CUsbFt::IsReceiveBufferEmpty( bool *rcvempty )
{
//	   IS_RCV_BUFFER_EMPTY  = 0x0A,
	DWORD status;
	BYTE data[255];
	status = SendUsbCanCmd(IS_RCV_BUFFER_EMPTY, data);
	if (status == USB_OK)
		if (rcvempty) *rcvempty = (data[0])?true:false;
	return status;
}

DWORD CUsbFt::IsTransmitBufferEmpty( bool *txempty )
{
//	   IS_TX_BUFFER_EMPTY	= 0x0B,	
	DWORD status;
	BYTE data[255];
	status = SendUsbCanCmd(IS_TX_BUFFER_EMPTY, data);
	if (status == USB_OK)
		if (txempty) *txempty = (data[0])?true:false;
	return status;
}

DWORD CUsbFt::IsTransmitPending( bool *txpending )
{
//	   IS_TX_PENDING		= 0x0C,
	DWORD status;
	BYTE data[255];
	status = SendUsbCanCmd(IS_TX_PENDING, data);
	if (status == USB_OK)
		if (txpending) *txpending = (data[0])?true:false;
	return status;
	
}

DWORD CUsbFt::RequestCANCON( unsigned short *cancon )
{
//	   GET_CANCON			= 0x0D,
	DWORD status;
	BYTE data[255];
	status = SendUsbCanCmd(GET_CANCON, data);
	if (status == USB_OK)
		if (cancon) *cancon = data[0];
	return status;
	
}

DWORD CUsbFt::RequestCIOCON( unsigned short *ciocon )
{
//	   GET_CIOCON			= 0x0E,
	DWORD status;
	BYTE data[255];
	status = SendUsbCanCmd(GET_CIOCON, data);
	if (status == USB_OK)
		if (ciocon) *ciocon = data[0];
	return status;
}

DWORD CUsbFt::SetupSyncCount( int time )
{
	BYTE txdata[20];
	BYTE rxdata[255];
    int size = 0;
	DWORD status;
	
	txdata[size++] = 0x00;
	txdata[size++] = 0x80 | SET_SYNC_COUNT;
	txdata[size++] = (BYTE)((time)&0xff);
		
	status = USBSendAndReceive( txdata, size, rxdata);	
	return status;
}

DWORD CUsbFt::SetupSyncMechanism(DWORD Id, BYTE datalen, BYTE *data)
{
	BYTE txdata[20];
	BYTE rxdata[255];
	int size = 0;
	DWORD status;
		
	txdata[size++] = 0x00;
	txdata[size++] = 0x80 | SET_SYNC_MSG;
	txdata[size++] = (BYTE)(Id>>24);  // ID MSB
	txdata[size++] = (BYTE)(Id>>16);
	txdata[size++] = (BYTE)(Id>>8);
	txdata[size++] = (BYTE)(Id);      // ID LSB
	txdata[size++] = datalen;	
	txdata[size++] = data[0];
	txdata[size++] = data[1];
    txdata[size++] = data[2];
	txdata[size++] = data[3];
	txdata[size++] = data[4];
	txdata[size++] = data[5];
	txdata[size++] = data[6];
	txdata[size++] = data[7];	
	status = USBSendAndReceive( txdata, size, rxdata);
	return status;
}

DWORD CUsbFt::StartSyncMechanism(void)
{	
	BYTE data[255];
	return(SendUsbCanCmd(TURN_SYNC_ON, data));
}

  // start the sync mechanism on the USB-CAN board running
DWORD CUsbFt::StopSyncMechanism(void)
{
	BYTE data[255];
	return(SendUsbCanCmd(TURN_SYNC_OFF, data));	
}


DWORD CUsbFt::ReadFT245_Data( char *mfg, char *mfgid, char *descr, char *serialnum  )
{
DWORD status;
FT_PROGRAM_DATA ftData;

ftData.Signature1 = 0x00000000;
ftData.Signature2 = 0xffffffff;
ftData.Manufacturer = mfg; 
ftData.ManufacturerId = mfgid; 

ftData.SerialNumber = serialnum; 
ftData.Description = descr; 


status = FT_EE_Read(m_ftHandle,&ftData); 
//status = FT_ReadEE(m_ftHandle, 0, &ftData );

return status;

}



