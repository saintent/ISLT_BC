/** \file DMXFrame.cpp
 *	\brief 
 */

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
#include "stdlib.h"
#include "DMXFrame.h"
// ---------- EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A
// ---------- PROGRAMMING DEFINITION INCLUDE ----------------------------------------------------- //
// N/A
// ---------- EXTERN OBJECT ---------------------------------------------------------------------- //
// N/A
// ---------- PUBLIC INTERFACE METHOD ------------------------------------------------------------ //
// N/A
// ---------- PUBLIC METHOD (FUNCTION PROTOTYPE) ------------------------------------------------- //
// N/A
// ---------- PUBLIC DATA ----------------------------------------------------------------------- //
// N/A
// ---------- PRIVATE METHOD (FUNCTION PROTOTYPE) ----------------------------------------------- //
// N/A
// ---------- PRIVATE DATA ---------------------------------------------------------------------- //
// N/A
// ---------- PRIVATE PROGRAMMING DEFINE -------------------------------------------------------- //
// N/A
// ---------- PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A
// ---------- SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
namespace dmx {

DMXFrame::DMXFrame(uint16_t frameSize) {
	// TODO Auto-generated constructor stub
	this->frameSize = frameSize;
	//this->pDataFrame = (uint8_t*) malloc(frameSize);
	this->currentCh = 0;
}

DMXFrame::DMXFrame(DMXFrame &buffer) {
	this->frameSize = buffer.frameSize;
	//this->pDataFrame = buffer.pDataFrame;
	this->currentCh = 0;
}

DMXFrame::~DMXFrame() {
	// TODO Auto-generated destructor stub
	//free((void*) this->pDataFrame);
}

uint8_t DMXFrame::GetChanelData(uint16_t index) {
	uint8_t returnValue = 0;

	if (index < this->frameSize) {
		returnValue = this->pDataFrame[index];
	}

	return returnValue;
}

uint8_t* DMXFrame::GetFrameData(void) {
	return this->pDataFrame;
}

uint8_t DMXFrame::StartWriteFrame(void) {

	this->currentCh = 0;

	return 1;
}

uint8_t DMXFrame::WriteDataToFrame(uint8_t data) {
	uint8_t result = 1;
	if (this->currentCh < this->frameSize) {
		this->pDataFrame[this->currentCh] = data;
		this->currentCh++;
		result = 0;
	}

	return result;

}

uint8_t DMXFrame::SetFrameSize(uint16_t size) {
	this->frameSize = size;
	/*free((void*) this->pDataFrame);

	this->pDataFrame = new uint8_t[size];//(uint8_t*) malloc(size);
	 */
	return 1;
}

uint8_t &DMXFrame::operator [](uint16_t index) {

	return this->pDataFrame[index];
}

}

//=========== Public Method ======================================================================//

//=========== Private Method ======================================================================//

// ---------- END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
