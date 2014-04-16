//================ File Description =========================================//
//=== File name : lib_fifo.c
//===========================================================================//



//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "lib_fifo.h"
//================ PUBLIC METHOD ============================================//
// extern function from another file
//
//================ PRIVATE METHOD ===========================================//
// non extern function
//
//================ PUBLIC DATA ==============================================//
// extern data
//
//================ PRIVATE DATA =============================================//
// non extern data
//
//================ PRIVATE DEFINE ===========================================//
//
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//
Status FIFO_Init(FIFO_ATTR_T* pFifo, void* pBuffer,uint32_t size) {
	pFifo->read = 0;
	pFifo->write = 0;
	pFifo->size = size;
	pFifo->buf.pBuffer = pBuffer;
	return (SUCCESS);
}

uint32_t calcFifoCount(FIFO_ATTR_T* pFifo) {
	uint32_t count;
	uint32_t rd;
	uint32_t wr;
	uint32_t sz;

	rd = pFifo->read;
	wr = pFifo->write;
	sz = pFifo->size;

	// calculate
	count = wr + ((wr < rd) ? sz : 0) - rd;

	return (count);
}

uint32_t FIFO_Count(FIFO_ATTR_T* pFifo) {
	uint32_t count;
	// TODO : Enter critical

	count = calcFifoCount(pFifo);

	// TODO : Exit critical

	return (count);
}

uint32_t FIFO_CheckAvaliable(FIFO_ATTR_T* pFifo) {
	uint32_t aval;
	uint32_t sz;

	sz = pFifo->size;
	aval = (sz - 1) - FIFO_Count(pFifo);

	return (aval);
}

Status FIFO_Flush(FIFO_ATTR_T* pFifo) {
	// TODO : Enter Critical

	pFifo->read = pFifo->write;

	// TODO : Exit critical
	return (SUCCESS);
}

Status FIFO_PutChar(FIFO_ATTR_T* pFifo, uint8_t data) {
	uint8_t* pBuf;
	uint32_t wr;
	uint32_t sz;

	// TODO : Enter critical

	pBuf = pFifo->buf.pU8Buffer;
	sz = pFifo->size;

	wr = pFifo->write;

	pBuf[wr] = data;

	wr = (wr + 1) % sz;

	pFifo->write = wr;

	// TODO : Exit critical

	return (SUCCESS);
}

Status FIFO_GetChar(FIFO_ATTR_T* pFifo, uint8_t* out, uint8_t remove) {
	uint8_t* pBuf;
	uint32_t sz;
	uint32_t rd;

	// TODO : Enter critical

	pBuf = pFifo->buf.pU8Buffer;
	sz = pFifo->size;

	rd = pFifo->read;

	out[0] = pBuf[rd];

	if(remove == TRUE) {
		rd = (rd + 1) % sz;

		pFifo->read = rd;
	}

	// TODO : Exit critical

	return (SUCCESS);
}


//================ END OF FILE ==============================================//
