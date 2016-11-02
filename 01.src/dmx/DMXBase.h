/** \file DMXBase.h
 *	\brief 
 */

#ifndef DMXBASE_H_
#define DMXBASE_H_

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
#include "stdint.h"
#include "DMXFrame.h"
#include "uartEvent.h"
using namespace uart;
// ---------- EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A
// ---------- PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
// N/A
// ---------- ENUMERATOR DEFINITION -------------------------------------------------------------- //
namespace dmx {
	enum eDmxState {
        dmxWaitForStart,
        dmxWaitStartAddress,
		dmxWaitforHandlerChannel,
        dmxRecvData,
        dmxFrameReady,
	};
}
// ---------- TYPEDEF DATA TYPE DEFINITION ------------------------------------------------------- //
typedef void dmxEvent_onFrameReceived(uint16_t channel);
// ---------- STRUCT OR UNION DATA TYPE DEFINITION ----------------------------------------------- //
// N/A
// ---------- PUBLIC MACRO DEFINITION ------------------------------------------------------------ //
// N/A
// ---------- EXTERN FUNCTION -------------------------------------------------------------------- //
// N/A
// ---------- EXTERN VARIABLE -------------------------------------------------------------------- //
// N/A
// ---------- CLASS DECLARATION ----------------------------------------------------------------- //
namespace dmx {
/*
 *
 */
class DMXBase {
public:
	DMXBase() { };
	DMXBase(uint8_t startChannel, uint8_t channelSpan, dmxEvent_onFrameReceived* callback = 0);
	virtual ~DMXBase(void);

	uint8_t GetChannelValue(uint16_t channel);

	static void OnRecviceFrame(void* obj, eUartEvent event, uint8_t data);
private:
	void OnReceiveProcess(eUartEvent event, uint8_t data);

protected:
	DMXFrame					frame;
	dmxEvent_onFrameReceived* 	onFrameReceived;
	uint16_t 					startChannel;
	uint16_t 					currentRevcChannel;
	eDmxState					state;
	uint8_t 					channelSpan;

};

}

// ---------- END OF CLASS DECLARATION ---------------------------------------------------------- //

#endif /* DMXBASE_H_ */
