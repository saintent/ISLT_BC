/** \file DMXBase.cpp
 *	\brief 
 */

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
//
#include "DMXBase.h"
using namespace uart;
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

//=========== Public Method ======================================================================//
namespace dmx {

DMXBase::DMXBase(uint8_t startChannel, uint8_t channelSpan,
		dmxEvent_onFrameReceived* callback) {
	// TODO Auto-generated constructor stub
	this->frame.SetFrameSize(channelSpan);
	this->onFrameReceived = callback;
	this->startChannel = startChannel;
	this->currentRevcChannel = 0;
	this->state = dmxWaitForStart;
	this->channelSpan = channelSpan;
}

DMXBase::~DMXBase() {
	// TODO Auto-generated destructor stub
}

uint8_t DMXBase::GetChannelValue(uint16_t channel) {

	return this->frame.GetChanelData(channel - 1);
}

void DMXBase::OnRecviceFrame(void* obj, eUartEvent event, uint8_t data) {
	((DMXBase*)obj)->OnReceiveProcess(event, data);
}

void DMXBase::OnReceiveProcess(eUartEvent event, uint8_t data) {

	if (event == eUartBreakInt) {
		this->state = dmxWaitStartAddress;
	}
	else {
		switch (this->state) {
		case dmxWaitStartAddress:
			// Ignore data for this frame
			if (data == 0x00) {
				// correct routine
				// Goto received state
				this->state = dmxWaitforHandlerChannel;
				this->currentRevcChannel = 0;
			} else {
				// Some thing error goto state wait for start
				this->state = dmxWaitForStart;
			}
			break;
		case dmxWaitforHandlerChannel:
			if (++this->currentRevcChannel >= this->startChannel) {
				// received channel data is reach to handler channel
				this->state = dmxRecvData;
				// insert 1st data
				this->frame.StartWriteFrame();
				this->frame.WriteDataToFrame(data);
			}
			break;
		case dmxRecvData:
			this->frame.WriteDataToFrame(data);

			if (++this->currentRevcChannel >= (this->channelSpan + this->startChannel)) {

				if (this->onFrameReceived != 0) {
					this->onFrameReceived(this->currentRevcChannel);
				}

				this->state = dmxFrameReady;
			}
			break;
		case dmxFrameReady:
		default :
			// Do nothing
			// wait until get next frame
			break;

		}
	}
}

//=========== Private Method ======================================================================//


}
// ---------- END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
