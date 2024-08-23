
#define USB_API

#pragma warning( disable: 4251 )
#pragma warning( disable: 4786 )


/*************************************************************************
** Definitions for USB-UCB message data
*************************************************************************/
#define USB_DATA_SIZE	8

/*************************************************************************
** Definitions for USB message start and end char
*************************************************************************/
#define USB_DLE 0x10
#define USB_STX 0x02
#define USB_ETX 0x03

/*************************************************************************
** Definitions for function status
*************************************************************************/
#define USB_ERR_NO_USB			101
#define USB_ERR_FT				102
#define USB_ERR_NO_HEXFILE		103
#define	USB_START_DOWNLOAD	0x53	//'S'
#define USB_END_DOWNLOAD	0x45	//'E'