//================ File Description =========================================//
//=== File name : Comport.h
//===========================================================================//

#ifndef COMPORT_H_
#define COMPORT_H_

//================ Include Header ===========================================//
#include "lpc_types.h"
#include "lpc12xx_libcfg.h"
#include "lpc_uart.h"
#include "lib_fifo.h"
#include "fcs.h"
#include "IAAP.h"
//================ PULBIC DEFINE ============================================//
#define STR_FRM			0xFE
#define END_FRM			0xFC
#define RS485_PORT		UART0
#define RS485_LOC		UART_LOC_0
#define ZB_PORT			UART1
#define ZB_LOC			UART_LOC_0
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	PHY_RS485 = 0,
	PHY_ZB,
	PHY_MAX
}PHY_OBJ_T;

typedef enum {
	FRM_START = 0,
	FRM_READ,
	FRM_DONE,
	FRM_GET_ADDR1,
	FRM_GET_ADDR2
}FRM_STATE_T;
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//

//================ TYPEDEF STRUCT/UNION =====================================//
typedef struct {
	Uart_type 	Port;
	UART_LOC_T	Loc;
	uint16_t	rsv;
	uint32_t	Speed;
}COM_PORT_T;

typedef struct {
	COM_PORT_T	Port;				// 8 btye
	uint8_t		TxBuff[32];			// 32 byte
	uint8_t		RxBuff[32];			// 32 byte
	FIFO_ATTR_T	TxFifo;				// 16 byte
	FIFO_ATTR_T	RxFifo;				// 16 byte
	uint8_t		rsv[6];				// Reserve 20 byte
				// Over all			80 byte
}PHY_OBJECT_T;

typedef struct {
	uint8_t		Lenght;
	uint16_t	Dest;
	uint16_t	Source;
	uint8_t		Cmd;
	uint8_t		PrimitiveEntry;
}FRM_HEADER_T;
//================ EXTERN FUNCTION ==========================================//
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
//================ EXTERN FUNCTION POINTER ==================================//
//
//================ EXTERN VARIABLE ==========================================//
//
//================ EXTERN QUEUE =============================================//
//
//================ CLASS DECLARATION ========================================//

class Comport {
private :
	PHY_OBJECT_T	phyObj;
	uint16_t		addr;
	uint8_t			frmBuffer[32];
	uint8_t			frmLength;
	FRM_STATE_T		frmState;
	uint8_t 		byteCount;
	IAAP*			pIAAP;
public:
	Comport();
	virtual ~Comport();
	Status	Init(IAAP* ptObj, COM_PORT_T *pPort);
	Bool	PendingProcess(void);
	void	Interactive(void);
	void	PhyCallback(uint8_t data);
	void 	SentByte(void);
	void 	SetAddress(uint8_t addr);
private :
	//uint16_t	calCRC;
	//Status		readFrame;
	void	readFrame(FIFO_ATTR_T* pFIFO, uint8_t* pDecodeData,
			uint8_t* pDecodeDataSize, uint8_t* pFrameCount);
	void 	sentFrame(uint16_t dest, uint8_t cmd,
			uint8_t* rspFrm, uint8_t frmSize);
};

//================ END OF FILE ==============================================//
#endif /* COMPORT_H_ */
