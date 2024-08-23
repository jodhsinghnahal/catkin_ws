#ifdef __cplusplus
extern "C" {
#endif

enum
{
    APOX_STS_ERR_ACTIVE,
    APOX_STS_ERR_PASSIVE,
    APOX_STS_BUS_OFF,
    APOX_STS_UNKNOWN
};

extern unsigned long APOX_fnListUSBCANBoards( void );
extern unsigned int APOX_fnInstantiate( unsigned char ucBitRate );
extern void APOX_fnClose( void );
extern unsigned int APOX_fnStatus( void );
extern unsigned int APOX_fnReadCANMsg(  unsigned char * pucDataPage,
                                        unsigned char * pucPriority,
                                        unsigned char * pucPktSpecific,
                                        unsigned char * pucPktFmt,
                                        unsigned char * pucSrcAddr,
                                        unsigned char * pucData,
                                        unsigned char * pucDataLen );
                                        
extern unsigned long APOX_fnSendRawCANMessage(  unsigned char ucDataPage,
                                                unsigned char ucPriority,
                                                unsigned char ucPktSpecific,
                                                unsigned char ucPktFmt,
                                                unsigned char ucSrcAddr,
                                                unsigned char * pucData,
                                                unsigned char ucDataLen );                                        

#ifdef __cplusplus
}
#endif   // _cplusplus
