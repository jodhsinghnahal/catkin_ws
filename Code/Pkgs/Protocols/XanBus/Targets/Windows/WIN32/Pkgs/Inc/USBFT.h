/*******************************************************************
 *    DESCRIPTION:	CUsbFt class which use FTD2XX.DLL to 
 *    communicate to the USB-CAN board.
 *
 *******************************************************************/
#ifndef __USBFT__
#define __USBFT__

#include <windows.h>
#include "USBCAN.h"
#include <map>
#include <vector>

using namespace std;

#include "FTD2XX.h"	// FT245BM driver library
typedef struct {	
	bool rx_buff1_ext;
	bool rx_buff2_ext;

	DWORD RXM0;
	DWORD RXM1;
	DWORD RXF0;	
	DWORD RXF1;
	DWORD RXF2;
	DWORD RXF3;
	DWORD RXF4;
	DWORD RXF5;
} USB_CAN_FILTERS;



// The following are the codes used by the USB-CAN board
// to set and get information
enum { WHICH_CODE_IS_RUNNING= 0x00,
       RESET_CPU			= 0x01,
	   GET_TX_ERR_CNT		= 0x02,
	   GET_RX_ERR_CNT		= 0x03,
	   GET_CANSTAT			= 0x04,
	   GET_COMSTAT			= 0x05,
	   GET_MSGFILTER1       = 0x06,
	   GET_MSGFILTER2		= 0x07,
	   SET_MSGFILTER1       = 0x08,
	   SET_MSGFILTER2		= 0x09,

	    // All of these commands are new
	   IS_RCV_BUFFER_EMPTY  = 0x0A,
	   IS_TX_BUFFER_EMPTY	= 0x0B,
	   IS_TX_PENDING		= 0x0C,
	   GET_CANCON			= 0x0D,
	   GET_CIOCON			= 0x0E,
	   SET_SYNC_COUNT		= 0x10,
	   SET_SYNC_MSG			= 0x11,
	   TURN_SYNC_ON		    = 0x12,
	   TURN_SYNC_OFF        = 0x13,
	  
	   
	   SET_CONFIG_MODE      = 0x20,
	   SET_LOOPBACK_MODE    = 0x21,
	   SET_NORMAL_MODE		= 0x22,
	   SET_SLEEP_MODE		= 0x23,
	   SET_LISTEN_MODE	    = 0x24,
	   ABORT_ALL_TX			= 0x26,
	   SET_BAUD_1MEG		= 0x30,
	   SET_BAUD_500K		= 0x31,
	   SET_BAUD_250K		= 0x32,
	   SET_BAUD_125K		= 0x33,
	   GET_BAUD_REGS        = 0x34,
	   RESET_MICRO			= 0x42,
	   GET_HARDWARE_VERSION = 0x43,
	   GET_FIRMWARE_VERSION = 0x44
};




// these are bits in the header byte of RxData
#define WHO_BIT 0x80
#define RTR_BIT 0x40
#define EXT_BIT 0x20

typedef struct {	
	void *fwd;
	void *bak;
	// the following is embedded into the header byte;
	bool who; // this is set to 1 if message originated from the USB-CAN board
	bool rtr; // set to 1 if the RTR bit was set on the received message	
	bool ext; //0=11 bit identifier  1=29 bit identifier
	
	BYTE header; 	
	BYTE command; // The command type

	DWORD Id; // either the 11 or 29 bit identifier	
	DWORD timestamp;
	BYTE len;
	BYTE data[255];

	BYTE txflags;	
	BYTE rxflags;
}RxData;

typedef void (CALLBACK *TUsbCallback)(void *pThis, RxData *pRxData);







class USB_API CUsbFt
{
public:
	CUsbFt(CString &sn);
	CUsbFt();
	~CUsbFt();

	bool isMainCodeRunning();
	void InternalInit();
	// general

	/**	
	* This function is used to check to see if the read thread is running yet.
	*
	*
	* @return TRUE if the thread is initilialized, false otherwise
	*
	
	*/
	bool IsInitialized(){return (m_listen==1)?true:false; }

	/**	
	* This function is used to send a CAN message out to the CAN bus
	*
	*
	*
	* @param rtr True if you want the RTR bit set when you send the message
	* @param message This is the upper 4 bits of the 11 bit CAN identifier (COBID)
	* @param nodeId This is the lower 7 bits of the 11 bit CAN identifier (COBID)
	* @param data This is an array of 0 to 8 data bytes you want to send in the CAN message
	* @param dataLen This is how many bytes you are going to send (0 to 8)
	*
	*
	* @return USB_DATA_TOO_LONG if dataLen is > 8
	* @return USB_OK if the message was succesfully send
	* @return USB_CANNOT_TRANSMIT if the USB driver will not accept any more data. Usually this means you lost comms with the board
	* @return USB_ERR_NO_USB if the USB driver did not connect
	*	
	*/
	DWORD SendRawCANMessage(bool rtr, BYTE txFlags, DWORD Id, BYTE *data, unsigned int dataLen);
	DWORD SendRawCANMessage(bool rtr, BYTE message, BYTE nodeId, BYTE *data, int dataLen);
	DWORD SendRawCANMessage(bool rtr, bool ext, BYTE txFlags, DWORD Id, BYTE *data, unsigned int dataLen);
	//DWORD SendRawCANMessage(bool rtr, int Id, BYTE *data, int dataLen);
	//DWORD SendRawCANMessage(bool rtr, BYTE message, BYTE nodeId, BYTE *data, int dataLen);


	/**
	* The following two functions set either 29 or 11 bit mode
	* when transmitting and receiving CAN messages
	*/
	void Set11BitMode( void );
	void Set29BitMode( void );
	/**	
	* This function is used to read the external EEPROM attached to the FT245BM chip
	* this information is burnt in at the factory.
	*
	*
	* @param mfg fills in a manufacturer named string
	* @param mfgid fills in a manufacturer id string
	* @param descr fills in the description of the product
	* @param serialnum fills in the unique serial number for each product
	*
	*
	* @return same as return from FTDI's library FT_EE_Read
	*	
	*/
	DWORD ReadFT245_Data( char *mfg, char *mfgid, char *descr, char *serialnum  );
// USB-CAN communications
	/**	
	* This function returns the available devices.
	* 
	*
	*
	* @param numDevs - Pointer to data that returns the number of USB-CANtastic devices found
	* @param sn - Pointer to an array of pointers that conatins a list of the serial numbers found
	*	
	*
	* @return FT_DEVICE_NOT_FOUND if the board is not found
	* @return FT_OK if the board was found
	*	
	*/
	DWORD	ListUSBCANBoards(DWORD *numDevs, char ***sn);
	// USB-CAN communications
	/**	
	* This function first sets up all of FTDI's library parameters, and then starts a
	* read thread to read all of the incoming messages. 
	*
	*
	* @param 
	*	
	*
	* @return FT_DEVICE_NOT_FOUND if the board is not found
	* @return FT_OK if the board was found
	*	
	*/
	DWORD	InitUSBDevice();
	/**	
	* This function is used to send a packet of data to the USB bus
	* This function will BYTE STUFF the packet of data, and then send it to the USB-CANtastic
	* 
	*
	* @param data is the raw array of data bytes you want to send to the USB-CANtastic
	*
	* @param size is the number of bytes in your packet.
	*
	*
	* @return USB_OK If the packet was succesfully added to the USB transmit buffer.
	* @return USB_CANNOT_TRANSMIT The USB transmit buffer is full, and cannot add any more data
	* @return USB_ERR_NO_USB The USB driver was not succesfully opened, or has disconnected		
	*/
	DWORD	USBSend(BYTE *data, int size);
	/**	
	* This is the actual thread that runs in the background. This thread is event driven
	* and will not lock up the CPU polling for data. When the Windows USB state machine
	* receives data. The FTDI driver will send us an event. This state machine will then
	* grab each byte of data, and start the BYTE DE-STUFFING process. If/when we receive
	* a complete un-stuffed packet. This function will put the message packet into a linked list
	* and call the ProcessRxMsg function to handle the data
	*
	*
	* @param pArg is a pointer to the class that started the thread
	*
	* @param
	*
	*
	* @return
	*
	
	*/
	static void USBReadThread(void *pArg);
	/**	
	* USBProcessByte does the actual BYTE de-stuffing of the incoming stream
	* of USB data. This function is called for every byte of incoming data
	*  
	*
	* @param inBytes this is the actual byte of data that the USB port just received
	*
	* @param RxMsgLen This is a pointer to the length of the message that is begin formed via the byte destuffing
	*
	* @param RxMsgData This is the byte de-stuffed message, that is in the process of being de-stuffed
	*  
	*
	*
	* @return USB_OK If their are no problems in the Byte destuffing state machine	
	* @return USB_EXPECTING_DLE	 Happens when a DLE character was expected, but a different character was received
	* @return USB_EXPECTING_STX	Happens when a STX (Start of Transmission) character was expected, but a different character was received
	* @return USB_BAD_CHECKSUM	This occurs if the checksum is incorrect
	* @return USB_EXPECTING_ETX	Happens when a ETX (End of transmission) character was expected, but a different character was received
	* @return USB_BAD_STATE	This should never happen unless someone called this function before the last
	*                       de-stuffed message was properly handled, and the state machine variables were properly set.
	*	
	*/
	DWORD	USBProcessByte(BYTE inByte, int *RxMsgLen, BYTE *RxMsgData);
	/**	
	* When an incoming message has completed the BYTE-destuffing process.
	* This function will be called. It will grab the message off of the recevied queue
	* (put there by the ReadThread, and it will either call a callback function
	* or set an Event, depending on the current mode of the software.	
	*
	*
	*
	* @return
	*
	
	*/
	DWORD	ProcessRxMsg();
	/**	
	* This function is used to grab a RxData message, and take it off of the m_RxMsgQ (received message queue)
	*
	*
	*
	* @param pData is a pointer to the RxData type that will be returned (if available)
	*
	*
	*
	* @return USB_OK if their was data on the queue
	* @return 1 if the queue was empty
	*
	
	*/
	DWORD	ReadRxMsg(RxData *pData);
	/**	
	* This function is thread safe, and used to read a messages from the CAN received
	* message Queue.
	*
	*
	*
	* @param pData This is the RxData pointer for the returned message (if available )
	*	
	*
	* @return USB_OK If their was a message in the queue
	* @return 1 if their was no more data on the queue
	*
	
	*/
	DWORD	ReadCANMsg(RxData *pData);
	/**	
	* This function is used to define the Emergency Message Callback functions
	* when the USB board want to report an error. Do not have the callback functions
	* change a dialog box item. The callback will occur in the seperate
	* ReadThread
	*
	*
	*
	* @param pCallback This is the function pointer to your emergency callback handler
	*
	* @param pCallbackPtr This is a "This" pointer to the instance of your class
	*	
	*/
	void DefineUSBEmergencyCallback(TUsbCallback pCallback, void *pCallbackPtr);
	/**	
	* This function is used to define the Normal CAN Message Callback functions
	* when the USB-CAntastic board sends us a complete CAN message.
	* The ReadThread will call the callback functions you have passed in.
	* Warning: Do not have the callback functions
	* change a dialog box item. The callback will occur in the seperate
	* ReadThread. (Its best to have the callback function generate a thread safe Windows Message)
	*
	*
	* @param pCallback This is the function pointer to your CAN message callback handler
	*
	* @param pCallbackPtr This is a "This" pointer to the instance of your class
	*	
	*/
	void DefineCANMsgCallback(TUsbCallback pCallback, void *pCallbackPtr);
	/**	
	* Sets the USB-CAntastic's CAN mode to configuration mode.
	* Configuration mode allows you to change settings such as filters and BAUD rates
	*
	*
	*
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period
	
	*/
	DWORD SetConfigMode(void);
	/**	
	* Sets the USB-CAntastic's CAN mode to Normal mode.
	* This is the mode you want to be in if you are sending and receiving data to/from the CAN bus
	*
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period
	*/
	DWORD SetNormalMode(void);
	/**	
	* Sets the USB-CAntastic's CAN mode to Listen Only mode.
	* Listen Only mode is used when you want to Snoop on the bus
	* But don't want to append ACK's or transmit any can messages
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period
		*/
	DWORD SetListenOnlyMode(void);
	/**	
	* Sets the USB-CAntastic's CAN mode to Disabled mode.
	* Disabled mode takes you off-bus, and you will not receive or tranmit any CAN data
	*
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period
	*
	* @return
	*
	
	*/
	DWORD SetDisabledMode(void);
	/**	
	* Sets the USB-CAntastic's CAN mode to loopback mode.
	* Loopback Mode is a fun mode! It allows you to receive any mesages you transmit.
	* And take you off-bus, so no externally send messages are seen. 
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*/
	DWORD SetLoopBackMode(void);

	
	
	/**	
	* This function will abort any messages that are currently in the CAN transmit buffers.
	* 
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*	
	*/
	DWORD AbortAllCanTransmissions();
	/**	
	* This function requests from the USB-CANtastic board, which code is running
	* either bootcode, or main code
	*
	* @param bMainRunning true=(Main Code is running)  false=(BootCode is running)
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_BAD_STATE if the USB-CANtastic did not respond properly
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	
	*/
	DWORD    AskWhichCodeIsRunning(bool *bMainRunning);
	/**	
	* This function will request the current TXERRCNT register from the PIC's internal CAN registers
	* please see the 18F458's datasheet for the meaning of this register.
	* 
	* @param m_iTxErrCnt will be an 8 bit value representing the register
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*	
	*/
	DWORD	RequestTxErrCnt(unsigned short *m_iTxErrCnt);
	/**	
	* This function will request the current RXERRCNT register from the PIC's internal CAN registers
	* please see the 18F458's datasheet for the meaning of this register.
	* 
	* @param m_iRxErrCnt will be an 8 bit value representing the register
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*	
	
	*/
	DWORD	RequestRxErrCnt(unsigned short *m_iTxErrCnt);
	/**	
	* This function will request the current CANSTAT register from the PIC's internal CAN registers
	* please see the 18F458's datasheet for the meaning of this register.
	* 
	* @param m_iCANSTAT will be an 8 bit value representing the register
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*	
	*/
	DWORD RequestCanMode(unsigned short *m_iCANSTAT);
	/**	
	* This function will request the current COMSTAT register from the PIC's internal CAN registers
	* please see the 18F458's datasheet for the meaning of this register.
	* 
	* @param m_iCOMSTAT will be an 8 bit value representing the register
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*
	*/
	DWORD RequestCanStatus(unsigned short *m_iCOMSTAT);
	/**	
	* This function will request the current Filter registers from the PIC's internal CAN registers
	* please see the 18F458's datasheet for the meaning of this registers.
	* 
	* @param pUSBCANFilter will consist of 16 bit values representing the filter register
	* The following data structure gets filled in
	*  typedef struct {	
	* int RXM0;
	* int RXM1;
	* int RXF0;	
	* int RXF1;
	* int RXF2;
	* int RXF3;
	* int RXF4;
	* int RXF5;
	*} USB_CAN_FILTERS;       
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*
	*/
	DWORD RequestFilterUpdate(USB_CAN_FILTERS *pUSBCANFilter);
	/**	
	* This function will set the current Filter registers in the PIC's internal CAN registers
	* please see the 18F458's datasheet for the meaning of this registers.
	* 
	* @param pUSBCANFilter will consist of 16 bit values representing the filter register
	* The following data structure gets filled in and sent to the PIC
	*  typedef struct {	
	* int RXM0;
	* int RXM1;
	* int RXF0;	
	* int RXF1;
	* int RXF2;
	* int RXF3;
	* int RXF4;
	* int RXF5;
	*} USB_CAN_FILTERS;       
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*
	*/
	DWORD WriteFilterData(USB_CAN_FILTERS *pUSBCANFilter);
	/**	
	* This function will return the Hardware Version of the USB-CANtastic board
	* format is typcially "H1.0"
	*
	*
	* @param pHW a pointer to a char buffer
	*
	* @param size the size of the string you are expecting (typically 5)
	*
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*	
	*/
	DWORD RequestHardwareVersion(char *pHW, int size);
	/**	
	* This function will return the Firmware Version of the USB-CANtastic board's firmware
	* format is typcially "F1.3"
	*
	*
	* @param pHW a pointer to a char buffer
	*
	* @param size the size of the string you are expecting (typically 5)
	*
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*	
	*/
	DWORD RequestFirmwareVersion(char *pFW, int size);
	/**	
	* This function is used to set the BAUD rate of the CAN controller
	*
	*
	* @param i should be SET_BAUD_1MEG, SET_BAUD_500K, SET_BAUD_250K, or SET_BAUD_125K
	*
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*	
	*/
	DWORD SetUsbCanBaudRate( int i ); 	
	/**	
	* This function will request the current BRGCON1,BRGCON2, and BRGCON3 registers from the PIC's internal CAN hardware
	* please see the 18F458's datasheet for the meaning of these registers.
	* 
	* @param BRGCON1 will be an 8 bit value representing the BRGCON1 register
	* @param BRGCON2 will be an 8 bit value representing the BRGCON2 register
	* @param BRGCON3 will be an 8 bit value representing the BRGCON3 register
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*
	*/
	DWORD GetBaudRegisters(int *BRGCON1, int *BRCON2, int *BRCON3);
	
	/**	
	* This function will request if their are any receive buffer are empty in the CAN hardware	
	* 
	* @param rcvempty true=their is an empty receive buffer false=no receiver buffers are available
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*
	*/
	DWORD IsReceiveBufferEmpty( bool *rcvempty );
	/**	
	* This function will request if their are any transmit buffers that are empty in the CAN hardware	
	* 
	* @param txempty true=their is at least one empty transmit buffer false=all transmit buffers are full
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*
	*/
	DWORD IsTransmitBufferEmpty( bool *txempty );
//	   IS_TX_BUFFER_EMPTY	= 0x0B,
	/**	
	* This function will request if their are any transmits pending in the CAN hardware	
	* 
	* @param txpending true=their is at least one transmit pending false=all transmit buffers are empty
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*
	*/
	DWORD IsTransmitPending( bool *txpending );
//	   IS_TX_PENDING		= 0x0C,
	/**	
	* This function will request the current CANCON register from the PIC's internal CAN registers
	* please see the 18F458's datasheet for the meaning of this register.
	* 
	* @param cancon will be an 8 bit value representing the register
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*
	*/
	DWORD RequestCANCON( unsigned short *cancon );
//	   GET_CANCON			= 0x0D,
	/**	
	* This function will request the current CIOCON register from the PIC's internal CAN registers
	* please see the 18F458's datasheet for the meaning of this register.
	* 
	* @param ciocon will be an 8 bit value representing the register
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*
	*/
	DWORD RequestCIOCON( unsigned short *ciocon );
//	   GET_CIOCON			= 0x0E,
	/**	
	* This function will setup the SYNC time in 40millisecond increments
	* However, SYNC's must be enabled by the StartSyncMechanism() to function.
	* Note: 0 is 40milliseconds, 1 = 80 milliseconds, 2= 120msec etc..
	* This function actually sets up a timer interrupt on the PIC, for very accurate
	* SYNC message transmissions.
	* 
	* @param time a register representing the time between sync messages (0 to 255)
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*
	*/
	DWORD SetupSyncCount( int time ); // this is a 8 bit * 40ms resolution
//	   SET_SYNC_COUNT		= 0x10,
	/**	
	* This function is used to setup the message that is transmitted every SYNC clock edge
	*
	*
	* @param canid This is the canid, of who gets the next message (The lower 7 bits of 11 bit CAN identifier)
	*
	* @param msgtype This is the msgtype (The upper 4 bits of the 11 bit CAN identifier)
	*
	* @param datalen This is the number of bytes in the data field (0 to 8)
	*
	* @param data This is the actual bytes of data to be transmitted each time
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	*	
	*/
	DWORD SetupSyncMechanism(DWORD msgtype, BYTE datalen, BYTE *data);
//	   SET_SYNC_MSG			= 0x11,
	/**	
	* This function is used to tell the USB-CANtastic, to start sending the sync messages
	* setup using the  SetupSyncMechanism() and SetupSyncCount() functions
	*	
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period	
	
	*	
	*/
	DWORD StartSyncMechanism(void);
	/**	
	* This function is used to tell the USB-CANtastic, to STOP sending the sync messages
	* setup using the  SetupSyncMechanism() and SetupSyncCount() functions
	*	
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period		
	*	
	*/
	DWORD StopSyncMechanism(void);	
//	   TURN_SYNC_ON_OFF		= 0x12,

	// special code downloading functions
	/**	
	* This function is used to send a instruction code to the USB-CANtastic board	
	* and is the basis for alot of the single byte commands.
	*
	* @param code is the actual byte command used by the USB-CANtastic board
	*
	*
	* @return USB_OK if the USB-CANtastic properly responded to request
	* @return USB_TIMEOUT if the USB-CANtastic did not respond within a timeout period			
	*/
	DWORD	SendCommand( BYTE code); //, bool MainCodeRunning=false );
	/**	
	* This function resets the microcontroller. 
	*
	*/
	void	SwitchToBootCode();
	/**	
	* This function is used to switch from bootcode to main code.
	* The bootcode will call the appropriate function, and switch to main code
	* by using the jump vector, burnt in during the bootloading of the
	* main firmware
	*	
	*/
	void	SwitchToMainCode();
	/**	
	* This function is used to read in a complete .HEX file
	* for later download to either the USB-CANtastic, or an external can node
	* 
	*
	*
	* @param fileName The file and pathname of the .HEX file
	*
	* @param xdata This is a large array that will hold the entire .HEX file
	*
	*
	* @return USB_ERR_NO_HEXFILE if the .hex file was not found
	* @return USB_OK if the file was read without a problem
	*	
	*/
	DWORD	ReadHexFile(char *fileName, BYTE *xdata);
	/**	
	* This function is used to reset the microcontroller on the USB-CANtastic
	* P.S. if main code was running, then bootcode will start running.
	* bootcode always runs after a powerup, or a reset*
	*
	*	
	*/
	DWORD	ResetMicrocontroller();

	/**	
	* This function puts the read thread into MODE_DOWNLOAD_USB
	* and switchs the USB-CANtastic into bootcode.
	*/
	void	GetReadyToDownloadUSB();
	
	/**	
	* This function puts the read thread back into MODE_RUN
	* and is used after you have completed downloading the latest firmware
	* to the USB-CANtastic.
	*	
	*/
	void	EndDownload(  );
	/**	
	* This function is used to download the latest USB-CANtastic firmware to the USB-CANtastic
	*
	* @param filename is the name and path of the .HEX file you want to download
	*
	*
	* @return USB_OK if the download was succesful
	*	
	*/
	DWORD	StartDownloadUSB(char * filename);
	/**	
	* This function is used to map a USB serial number into a user name.
	*
	* @param serialNumber The serialNumber to map
	*
	*
	* @return CString The found mapping or the serialNumber if no mapping found
	*	
	*/
	CString MapSerialNumberToName(char *serialNumber);
	/**	
	* This function is used to update a mapping from a USB serial number into a user name.
	*
	* @param mapping The new user name to associate with this USB serial number
	*
	*
	* @return BOOL true if successful
	*	
	*/
	BOOL UpdateSerialNumberToName(CString mapping);
	CString &SerialNumber() {return m_cSerialNumber;}

	// USB-CAN board variables
	enum USB_ERRORS { USB_OK=0,
					USB_NO_ERROR=99,   // 0x63
                  USB_EXPECTING_DLE=100, // 0x64
                  USB_EXPECTING_STX=101,
                  USB_BAD_CHECKSUM=102,
                  USB_EXPECTING_ETX=103,
                  USB_BAD_STATE=104,
                  USB_MSG_TOO_BIG=105,
                  USB_MSG_TIMEOUT=106,  // 0x6A
                  USB_RX_BAD_CAN_MSG=107, //0x6b
                  USB_CAN_TX_STUCK=108,
                  CAN_RCV_OVERFLOW=109,
                  CAN_RCV_WARNING=110,
                  CAN_TX_WARNING=111,
                  CAN_RX_BUS_PASSIVE=112,
                  CAN_TX_BUS_PASSIVE=113,
                  CAN_BUS_OFF=114,
				  CAN_IN_CONFIG_MODE=115,
				  USB_TIMEOUT=116,
				  USB_DATA_TOO_LONG=117,
				  USB_CANNOT_TRANSMIT=118,
				  USB_ERROR_NO_USB=119
                 };
	//enum USB_ERRORS { USB_OK=0, USB_EXPECTING_DLE=100, USB_EXPECTING_STX, USB_BAD_CHECKSUM, USB_EXPECTING_ETX, USB_BAD_STATE, USB_TIMEOUT, USB_DATA_TOO_LONG };
	enum USB_STATE_STATES { USB_IDLE=0, USB_FRAME_START, USB_FRAME, USB_FRAME_NLE, USB_MESSAGE_COMPLETE };
	enum USB_MODE { MODE_RUN=0, MODE_DOWNLOAD_USB, MODE_DOWNLOAD_CAN };
	enum QUEUE_NUM { UCB_QUEUE_PDO=0, UCB_QUEUE_EMERGENCY, UCB_QUEUE_EVENT, UCB_QUEUE_SYNC };
	enum CAN_BUAD_RATE {CAN_BAUD_1000, CAN_BAUD_500, CAN_BAUD_250, CAN_BAUD_125};
	enum CAN_DOWNLOAD_ERRORS { CAN_INVALID_BOOT_COMMAND=1, CAN_MESSAGE_OUT_OF_SYNC, CAN_INVALID_LENGTH, BAD_FLASH_ADDRESS, BAD_FLASH_WRITE };
	#define CAN_DOWNLOAD_ACK  (13)


protected:
	RxData *GetFreeRXData();
	void FreeRxData(RxData *ptr);
	void AddRxMemory(int num);
	DWORD   SendUsbCanCmd( BYTE code, BYTE *data );
	DWORD WaitForUSBResponse(BYTE message, BYTE *pData);
	void HandleUSBCANMessage(RxData *pRxData);
	HANDLE m_hReadThread;
	int m_bTerminateRead;
	FT_HANDLE m_ftHandle;	// handle for USB device
	HANDLE m_hRxCharEvent;  // char arrived event
	HANDLE m_hTxReplyEvent;	// got replay during downloading
	HANDLE m_hUSBReplyEvent;
	CRITICAL_SECTION m_CRITICAL_SECTION_DataQ;
	CRITICAL_SECTION m_CRITICAL_SECTION_CANDataQ;
	CRITICAL_SECTION m_CRITICAL_SECTION_WriteAccess;
	CRITICAL_SECTION m_CRITICAL_SECTION_Access;
	CRITICAL_SECTION m_CRITICAL_SECTION_FreeQ;
	int	m_listen;
	int m_xMode;

	// false=11 bit mode    true=29bit mode
	bool identifier_mode;
		
	DWORD USBSendAndReceive(BYTE *sData, int dataSize, BYTE *pData);
	vector<RxData*> m_RxMsgQ;
	vector<RxData*> m_USBMsgQ;
	vector<RxData*> m_CANMsgQ;
	void *m_slRxDataFree;
	vector<char*> m_rxMemoryListQ;

	RxData *m_rxMemory;
	
	int  m_RxMsgState;
	CString m_cSerialNumber;
	
	TUsbCallback	m_pCANCallback;
	void *m_pCANCallbackPtr;
	TUsbCallback	m_pEmergencyCallback;
	void *m_pEmergencyCallbackPtr;
	
};

#endif