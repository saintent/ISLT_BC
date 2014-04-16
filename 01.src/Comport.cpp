//================ File Desciption =========================================//
//=== File name : Comport.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "Comport.h"
#include "string.h"
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
//================ PRIVATE DEFINE ===========================================//
//
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//

Comport::Comport() {
	// TODO Auto-generated constructor stub

}

Comport::~Comport() {
	// TODO Auto-generated destructor stub
}

Status Comport::Init(IAAP* ptObj) {


	//Fifo init
	FIFO_Init(&phyObj.RxFifo, phyObj.RxBuff, 32);
	FIFO_Init(&phyObj.TxFifo, phyObj.TxBuff, 32);
	// Point to object class
	this->pIAAP = ptObj;

}

void Comport::SetAddress(uint8_t addr) {
	this->addr = addr;
}

Bool Comport::PendingProcess(void) {
	FIFO_ATTR_T* fifo;
	Bool result;
	fifo = &phyObj.RxFifo;
	if (FIFO_Count(fifo) != 0) {
		result = TRUE;
	}
	else {
		result = FALSE;
	}

	return result;
}

void Comport::Interactive(void) {
	uint8_t frmCount;
	uint8_t rspFrm[32];
	uint8_t rspLen;
	uint8_t payLoadLen;
	uint16_t fcsIn;
	uint16_t fcsCal;
	FRM_HEADER_T* primitive;
	FIFO_ATTR_T* fifo;
	fifo = &phyObj.RxFifo;
	// Read frame
	frmCount = 0;
	readFrame(fifo, frmBuffer, &frmLength, &frmCount);
	// Frame detect
	if (frmCount != 0) {
		// Process frame
		// Getting header info
		primitive = (FRM_HEADER_T*) frmBuffer;
		// Check address
		if (primitive->Dest != addr) {
			goto COMPORT_INTLR;
		}
		// Check CRC
		fcsIn = frmBuffer[frmLength-1];
		fcsIn |= ((uint16_t) frmBuffer[frmLength - 2] << 8) & 0xFF00;
		FCS::FcsRun( frmBuffer, frmLength - 2, &fcsCal);
		if (fcsCal != fcsIn) {
			goto COMPORT_INTLR;
		}
		// Correct frame
		// Call to process
		payLoadLen = primitive->Lenght - 5;
		pIAAP->ProcessPrimitve(&primitive->Cmd, payLoadLen, rspFrm, &rspLen);
		sentFrame(primitive->Source, primitive->Cmd, rspFrm, rspLen);
		COMPORT_INTLR :
		frmState = FRM_START;
		frmLength = 0;
	}

	return;
}

void Comport::PhyCallback(uint8_t data) {
	FIFO_ATTR_T* fifo;
	fifo = &phyObj.RxFifo;
	FIFO_PutChar(fifo, data);
}

void Comport::readFrame(FIFO_ATTR_T* pFIFO, uint8_t* pDecodeData,
		uint8_t* pDecodeDataSize, uint8_t* pFrameCount) {
	uint8_t u8Data;
	//Status sta;
	if (frmState == FRM_DONE) {
		//frmState = FRM_READ;
		pFrameCount[0] = 0;
		return;
	}
	while (FIFO_Count (pFIFO)) {
		FIFO_GetChar(pFIFO, &u8Data, TRUE);
		switch (frmState) {
		case FRM_DONE :
		default :
		case FRM_READ :
			switch (u8Data) {
			case END_FRM :
				// Getting end frame
				pFrameCount[0]++;
				frmState = FRM_DONE;
				return;
				break;
			default :
				// Getting frame data
				pDecodeData[pDecodeDataSize[0]] = u8Data;
				pDecodeDataSize[0]++;
				break;
			}
			break;
		case FRM_START :
			// Getting start flag
			if (u8Data == STR_FRM) {
				//pDecodeData[pDecodeDataSize[0]] = u8Data;
				//pDecodeDataSize[0]++;
				frmState = FRM_READ;
			}
			break;
		}
	}
}

void Comport::sentFrame(uint8_t dest, uint8_t cmd,
		uint8_t* rspFrm, uint8_t frmSize) {
	uint8_t frm[64];
	uint8_t len;
	uint8_t pos;
	uint16_t fcs;
	FIFO_ATTR_T* pfifo;
	pos = 0;
	len = 0;
	// Start Flag
	frm[pos++] = STR_FRM;
	// Jump to destination address
	pos++;
	frm[pos++] = dest;
	frm[pos++] = addr;
	frm[pos++] = cmd;
	memcpy(&frm[pos], rspFrm, frmSize);
	len = frmSize + 6;
	frm[1] = len;
	FCS::FcsRun(&frm[1], len, &fcs);
	pos += frmSize;
	frm[pos++] = (fcs >> 8) & 0xFF;
	frm[pos++] = (uint8_t) fcs;
	frm[pos++] = END_FRM;
	len = pos;
	pos = 0;
	pfifo = &phyObj.TxFifo;
	while(len) {
		FIFO_PutChar(pfifo, frm[pos]);
		pos++;
		len--;
	}
	UART_StartSent(phyObj.Port.Port);
	SentByte();
}

void Comport::SentByte(void) {
	uint8_t data;
	Uart_type port;
	FIFO_ATTR_T* pFIFO;
	pFIFO = &phyObj.TxFifo;
	port = phyObj.Port.Port;

	if (FIFO_Count(pFIFO) != 0) {
		FIFO_GetChar(pFIFO, &data, TRUE);
		UARTSendCh(port, data);
	}
	else {
		UART_StopSent(port);
	}
}


//================ END OF FILE ==============================================//

