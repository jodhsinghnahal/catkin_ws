

#ifndef __ISCANEXT_H__
#define __ISCANEXT_H__



#define CAN_ALL_MESSAGES_EX        0xFFFFFFFF

/*******************************/
/* ein zusätzlicher Fehlercode */
/*******************************/
#define CAN_RECEIVED_EFF_MESSAGE    24	// Message im Extended Frame Format empfangen
										// call isCAN_ReceiveMessageEx

/**************************************************************/
/* die CAN-Message für die Nutzung des Extended Frame Formats */
/**************************************************************/
typedef struct
{
	DWORD MessageID;
	BOOLEAN bExtended;
	BOOLEAN RemoteReq;
	BYTE DataLen;
	BYTE Data[8];
} CAN_MessageEx_type;


/**********************************/
/* und die Funktionsdeklarationen */
/**********************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Funktionen zur Nutzung des Extended Frame Format */
BYTE __cdecl isCAN_DeviceInitEx(BYTE DeviceNr, BYTE baud);
BYTE __cdecl isCAN_TransmitMessageEx(BYTE DeviceNr, CAN_MessageEx_type* pMessage);
BYTE __cdecl isCAN_ReceiveMessageEx(BYTE DeviceNr, CAN_MessageEx_type* pMessage);
BYTE __cdecl isCAN_SetMessageExID(BYTE DeviceNr, DWORD MessageID);

/* diesselben Funktionen mit einer anderen Aufrufkonvention z.B. für Visual Basic */
BYTE __stdcall isCANvb_DeviceInitEx(BYTE DeviceNr, BYTE baud);
BYTE __stdcall isCANvb_TransmitMessageEx(BYTE DeviceNr, CAN_MessageEx_type* pMessage);
BYTE __stdcall isCANvb_ReceiveMessageEx(BYTE DeviceNr, CAN_MessageEx_type* pMessage);
BYTE __stdcall isCANvb_SetMessageExID(BYTE DeviceNr, DWORD MessageID);

#ifdef __cplusplus
}
#endif


/* Funktionszeiger zum dynamischen Laden der Dll */
typedef BYTE (__cdecl *pCAN_DeviceInitEx)(BYTE DeviceNr, BYTE baud);
typedef BYTE (__cdecl *pCAN_TransmitMessageEx)(BYTE DeviceNr, CAN_MessageEx_type* pMessage);
typedef BYTE (__cdecl *pCAN_ReceiveMessageEx)(BYTE DeviceNr, CAN_MessageEx_type* pMessage);
typedef BYTE (__cdecl *pCAN_SetMessageExID)(BYTE DeviceNr, DWORD MessageID);


/* Makros für die Zuweisung der Funktionszeiger */
#define FUNCPTR_DEVICEINITEX(hDll) (pCAN_DeviceInitEx)GetProcAddress(hDll, "isCAN_DeviceInitEx")
#define FUNCPTR_TRANSMITEX(hDll)   (pCAN_TransmitMessageEx)GetProcAddress(hDll, "isCAN_TransmitMessageEx")
#define FUNCPTR_RECEIVEEX(hDll)    (pCAN_ReceiveMessageEx)GetProcAddress(hDll, "isCAN_ReceiveMessageEx")
#define FUNCPTR_SETMESSAGEEX(hDll) (pCAN_SetMessageExID)GetProcAddress(hDll, "isCAN_SetMessageExID")

#endif // __ISCANEXT_H__
