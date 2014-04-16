//================ File Description =========================================//
//=== File name : lib_fifo.h
//===========================================================================//


#ifndef LIB_FIFO_H_
#define LIB_FIFO_H_


//================ Include Header ===========================================//
#include "stdint.h"
#include "lpc_types.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
typedef union FIFO_type {
	void* 		pBuffer;
	uint8_t*	pU8Buffer;
	void**		pPBuffer;
}FIFO_TYPE_t;

typedef struct FIFO_attr {
	uint32_t			size;
	volatile uint32_t	read;
	volatile uint32_t	write;
	FIFO_TYPE_t			buf;
}FIFO_ATTR_T;


//================ ENUMERATOR DEFINITION ====================================//
//
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
//
//================ EXTERN FUNCTION ==========================================//
#ifdef __cplusplus
extern "C"
{
#endif

Status FIFO_Init(FIFO_ATTR_T* pFifo, void* pBuffer,uint32_t size);
uint32_t FIFO_Count(FIFO_ATTR_T* pFifo);
uint32_t FIFO_CheckAvaliable(FIFO_ATTR_T* pFifo);
Status FIFO_Flush(FIFO_ATTR_T* pFifo);
Status FIFO_PutChar(FIFO_ATTR_T* pFifo, uint8_t data);
Status FIFO_GetChar(FIFO_ATTR_T* pFifo, uint8_t* out, uint8_t remove);

#ifdef __cplusplus
}
#endif
//================ EXTERN FUNCTION POINTER ==================================//
//
//================ EXTERN VARIABLE ==========================================//
//
//================ EXTERN QUEUE =============================================//
//
//================ END OF FILE ==============================================//
#endif /* LIB_FIFO_H_ */
