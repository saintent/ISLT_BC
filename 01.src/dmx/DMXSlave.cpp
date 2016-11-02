/** \file DMXSlave.cpp
 *	\brief 
 */

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
//
#include "DMXSlave.h"
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
//=========== Public Method ======================================================================//
DMXSlave::DMXSlave(uint8_t startChannel, uint8_t channelSpan,
		dmxEvent_onFrameReceived* callback) {
	// TODO Auto-generated constructor stub
	this->frame.SetFrameSize(channelSpan);
	this->onFrameReceived = callback;
	this->startChannel = startChannel;
	this->currentRevcChannel = 0;
	this->state = dmxWaitForStart;
	this->channelSpan = channelSpan;
}

DMXSlave::~DMXSlave() {
	// TODO Auto-generated destructor stub
}
//=========== Private Method ======================================================================//

}
// ---------- END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
