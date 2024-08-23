

#ifndef __ISCANDRV_H__
#define __ISCANDRV_H__


#define MAX_NR_OF_CARDS         16
#define CAN_ALL_MESSAGES        0xFFFF

/**********************************/
/* alle definierten CAN-Baudraten */
/**********************************/
#define CAN_OFFLINE    0
/* #define CAN_BAUD_5          not implemented */
#define CAN_BAUD_10    1
#define CAN_BAUD_20    2
#define CAN_BAUD_50    3
#define CAN_BAUD_100   4
#define CAN_BAUD_125   5
#define CAN_BAUD_250   6
#define CAN_BAUD_500   7
#define CAN_BAUD_800   8
#define CAN_BAUD_1000  9

#define MAX_BAUD       9


/******************************/
/* alle möglichen Fehlercodes */
/******************************/
#define CAN_NO_ERROR                 0	// success
#define CAN_OPEN_ERROR               1	// no access to device
#define CAN_DEVICE_NOT_FOUND         2	// device with ID not found
#define CAN_DRIVER_ERROR             3	// driver operation failed
#define CAN_INVALID_PARAMETER        4	// 
#define CAN_DEVICE_NOT_ONLINE        5	// operation allowed only in online state
#define CAN_DEVICE_TIMEOUT           6
#define CAN_TRANSMIT_BUFFER_BUSY     7	// device is transmitting a message
#define CAN_RECEIVE_BUFFER_EMPTY     8	// no message received
#define CAN_THREAD_NOT_STARTED       9	// Thread konnte nicht gestartet werden bzw. läuft nicht
#define CAN_THREAD_ALREADY_STARTED  10	// Thread läuft doch schon, erst beenden

#define CAN_BUFFER_OVERRUN          11

#define CAN_DEVICE_NOT_INITIALIZED  12
#define CAN_DEVICE_REMOVED          13  // Gerät abgezogen
#define CAN_DEVICE_ALREADY_IN_USE   15  // Gerät ist bereits von diesem oder einem anderen Programm in Benutzung

#define CAN_BUS_ERROR               16
#define CAN_BUS_OFF                 17
#define CAN_ERROR_PASSIVE           18
#define CAN_DATA_OVERRUN            19
#define CAN_ERROR_WARNING           20

#define CAN_BUS_RESET               22	// soft reset of SJA1000 after bus off condition

#define CAN_SEND_ERROR              30	// Busfehler beim Senden
#define CAN_SEND_NO_ACK             31	// no acknoledge, Node ist vermutlich allein am Bus
#define CAN_ERR_CRITICAL_BUS        32	// kritisches Bus-Timing oder Bus-Länge

#define CAN_THREAD_IS_BLOCKED       35	// callbackthread is blocked, stopping thread failed

#define CAN_DEVICE_NOT_LICENCED     40	// need a licence number under NT4
#define CAN_ACCESS_DENIED           41  // no acces rights for operation


/* verschiedene Gerätetypen, die für die Funktionen isCAN_GetDriverConfiguration, */
/* isCAN_GetDriverConfiguration und isCAN_AttachedDevices benötigt werden.        */
#define DEVTYPE_NO_DEV 0
#define DEVTYPE_USB    1
#define DEVTYPE_PCI    2
#define DEVTYPE_PCMCIA  3   // wird z.Z. nicht unterstützt
#define DEVTYPE_NETCUBE 4
#define NR_DEVTYPES     5

/* Flags und Konstanten, die für isCAN_AttachedDevices benötigt werden */
#define FIND_UNCONFIGURED 0x20
#define FIND_CONFIGURED   0x40
#define FIND_ALL_DEVICES  0x60
#define FIND_NEXT         0x80

/* Konstanten, die für isCAN_NotifyPnP benötigt werden */
#define NOTIFY_SET   1
#define NOTIFY_PULSE 2
#define NOTIFY_END   3   /* der Event-Mechanismus wird wieder abgeschaltet, hEvent muß NULL sein */


/* Kommandos für die Funktion isCAN_SetupDevice */
#define SETUPCMD_FILTER   1
#define SETUPCMD_BAUDCHG  2
#define SETUPCMD_TRANSMIT 3

/* Eigenschaften für die Funktion isCAN_SetupDevice */
#define SETUP_FILTER_DEFAULT           0  // default setting
#define SETUP_FILTER_IMMEDIATELY       4
#define SETUP_FILTER_EXACT             2
#define SETUP_FILTER_CLEAR_BUFFER      1
#define SETUP_FILTER_EXACT_WITH_CLEAR  3

#define SETUP_BAUDCHG_DEFAULT  0
#define SETUP_BAUDCHG_CLEAR    1

#define SETUP_TRANSMIT_SYNC    0  // default setting
#define SETUP_TRANSMIT_ASYNC   1

/**************************/
/* die CAN-Message selbst */
/**************************/
typedef struct
{
	WORD MessageID;
	BOOLEAN RemoteReq;
	BYTE DataLen;
	BYTE Data[8];
} CAN_Message_type;


#pragma pack(push, 1)
typedef struct
{
	BYTE DeviceType;     /* no device = 0, USB = 1, PCI = 2, PCMCIA = 3 ... */
	DWORD DeviceID;
	BOOLEAN bAttached;   /* dieses Gerät ist angeschlossen */
	BOOLEAN bInUse;      /* das Gerät wird gerade benutzt */
} DeviceInfo_type;
#pragma pack(pop)


/**********************************/
/* und die Funktionsdeklarationen */
/**********************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Funktionen in der Dll */
BYTE __cdecl isCAN_DeviceInit(BYTE DeviceNr, BYTE baud);
BYTE __cdecl isCAN_SetBaud(BYTE DeviceNr, BYTE baud);
BYTE __cdecl isCAN_SetBaudRegister(BYTE DeviceNr, BYTE regBTR0, BYTE regBTR1);
BYTE __cdecl isCAN_SetMessageID(BYTE DeviceNr, WORD MessageID);
BYTE __cdecl isCAN_TransmitMessage(BYTE DeviceNr, CAN_Message_type* pMessage);
BYTE __cdecl isCAN_ReceiveMessage(BYTE DeviceNr, CAN_Message_type* pMessage);
BYTE __cdecl isCAN_CloseDevice(BYTE DeviceNr);
BYTE __cdecl isCAN_SetupDevice(BYTE DeviceNr, BYTE setupcmd, DWORD property, void* pData);
BYTE __cdecl isCAN_GetDriverConfiguration(BYTE DeviceNr, DeviceInfo_type* pInfo);
BYTE __cdecl isCAN_SetDriverConfiguration(BYTE DeviceNr, DeviceInfo_type* pInfo, void* pData);
BYTE __cdecl isCAN_AttachedDevices(BYTE flag, DeviceInfo_type* pInfo);
BYTE __cdecl isCAN_NotifyPnP(HANDLE hEvent, BYTE flag);

/* diesselben Funktionen mit einer anderen Aufrufkonvention z.B. für Visual Basic */
BYTE __stdcall isCANvb_DeviceInit(BYTE DeviceNr, BYTE baud);
BYTE __stdcall isCANvb_SetBaud(BYTE DeviceNr, BYTE baud);
BYTE __stdcall isCANvb_SetBaudRegister(BYTE DeviceNr, BYTE regBTR0, BYTE regBTR1);
BYTE __stdcall isCANvb_SetMessageID(BYTE DeviceNr, WORD MessageID);
BYTE __stdcall isCANvb_TransmitMessage(BYTE DeviceNr, CAN_Message_type* pMessage);
BYTE __stdcall isCANvb_ReceiveMessage(BYTE DeviceNr, CAN_Message_type* pMessage);
BYTE __stdcall isCANvb_CloseDevice(BYTE DeviceNr);
BYTE __stdcall isCANvb_SetupDevice(BYTE DeviceNr, BYTE setupcmd, DWORD property, void* pData);
BYTE __stdcall isCANvb_GetDriverConfiguration(BYTE DeviceNr, DeviceInfo_type* pInfo);
BYTE __stdcall isCANvb_SetDriverConfiguration(BYTE DeviceNr, DeviceInfo_type* pInfo, void* pData);
BYTE __stdcall isCANvb_AttachedDevices(BYTE flag, DeviceInfo_type* pInfo);
BYTE __stdcall isCANvb_NotifyPnP(HANDLE hEvent, BYTE flag);

/* Prototyp für Rückrufe */
void __cdecl MessageHandler(BYTE DeviceNr, void* pContext);
typedef void (__cdecl *pMessageHandler)(BYTE, void*);

BYTE __cdecl isCAN_StartMessageHandler(BYTE DeviceNr,
									   pMessageHandler CallbackFunc,
									   void* pContext);
BYTE __cdecl isCAN_StopMessageHandler(BYTE DeviceNr);

#ifdef __cplusplus
}
#endif

/* Funktionszeiger zum dynamischen Laden der Dll */
typedef BYTE (__cdecl *pCAN_DeviceInit)(BYTE DeviceNr, BYTE baud);
typedef BYTE (__cdecl *pCAN_SetBaud)(BYTE DeviceNr, BYTE baud);
typedef BYTE (__cdecl *pCAN_SetBaudRegister)(BYTE DeviceNr, BYTE regBTR0, BYTE regBTR1);
typedef BYTE (__cdecl *pCAN_SetMessageID)(BYTE DeviceNr, WORD MessageID);
typedef BYTE (__cdecl *pCAN_TransmitMessage)(BYTE DeviceNr, CAN_Message_type* pMessage);
typedef BYTE (__cdecl *pCAN_ReceiveMessage)(BYTE DeviceNr, CAN_Message_type* pMessage);
typedef BYTE (__cdecl *pCAN_CloseDevice)(BYTE DeviceNr);
typedef BYTE (__cdecl *pCAN_SetupDevice)(BYTE DeviceNr, BYTE setupcmd, DWORD property, void* pData);
typedef BYTE (__cdecl *pCAN_GetDriverConfiguration)(BYTE DeviceNr, DeviceInfo_type* pInfo);
typedef BYTE (__cdecl *pCAN_SetDriverConfiguration)(BYTE DeviceNr, DeviceInfo_type* pInfo, void* pData);
typedef BYTE (__cdecl *pCAN_AttachedDevices)(BYTE flag, DeviceInfo_type* pInfo);
typedef BYTE (__cdecl *pCAN_NotifyPnP)(HANDLE hEvent, BYTE flag);
typedef BYTE (__cdecl *pCAN_StartMessageHandler)(BYTE DeviceNr,
												 pMessageHandler CallbackFunc,
												 void* pContext);
typedef BYTE (__cdecl *pCAN_StopMessageHandler)(BYTE DeviceNr);


/* Makros für die Zuweisung der Funktionszeiger */
#define FUNCPTR_DEVICEINIT(hDll) (pCAN_DeviceInit)GetProcAddress(hDll, _T("isCAN_DeviceInit"))
#define FUNCPTR_SETBAUD(hDll)    (pCAN_SetBaud)GetProcAddress(hDll, _T("isCAN_SetBaud"))
#define FUNCPTR_SETBAUDREG(hDll) (pCAN_SetBaudRegister)GetProcAddress(hDll, _T("isCAN_SetBaudRegister"))
#define FUNCPTR_SETMESSAGE(hDll) (pCAN_SetMessageID)GetProcAddress(hDll, _T("isCAN_SetMessageID"))
#define FUNCPTR_TRANSMIT(hDll)   (pCAN_TransmitMessage)GetProcAddress(hDll, _T("isCAN_TransmitMessage"))
#define FUNCPTR_RECEIVE(hDll)    (pCAN_ReceiveMessage)GetProcAddress(hDll, _T("isCAN_ReceiveMessage"))
#define FUNCPTR_CLOSE(hDll)      (pCAN_CloseDevice)GetProcAddress(hDll, _T("isCAN_CloseDevice"))
#define FUNCPTR_SETUPDEV(hDll)   (pCAN_SetupDevice)GetProcAddress(hDll, _T("isCAN_SetupDevice"))
#define FUNCPTR_GETDRVCFG(hDll)  (pCAN_GetDriverConfiguration)GetProcAddress(hDll, _T("isCAN_GetDriverConfiguration"))
#define FUNCPTR_SETDRVCFG(hDll)  (pCAN_SetDriverConfiguration)GetProcAddress(hDll, _T("isCAN_SetDriverConfiguration"))
#define FUNCPTR_ATTACHED(hDll)   (pCAN_AttachedDevices)GetProcAddress(hDll, _T("isCAN_AttachedDevices"))
#define FUNCPTR_NOTIFYPNP(hDll)  (pCAN_NotifyPnP)GetProcAddress(hDll, _T("isCAN_NotifyPnP"))

#define FUNCPTR_STARTCALLBACK(hDll) (pCAN_StartMessageHandler)GetProcAddress(hDll, _T("isCAN_StartMessageHandler"))
#define FUNCPTR_STOPCALLBACK(hDll)  (pCAN_StopMessageHandler)GetProcAddress(hDll, _T("isCAN_StopMessageHandler"))

#endif // __ISCANDRV_H__
