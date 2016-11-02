/** \file DMXSlave.h
 *	\brief 
 */

#ifndef DMXSLAVE_H_
#define DMXSLAVE_H_

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
#include "stdlib.h"
#include "DMXBase.h"
// ---------- EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A
// ---------- PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
// N/A
// ---------- ENUMERATOR DEFINITION -------------------------------------------------------------- //
// N/A
// ---------- TYPEDEF DATA TYPE DEFINITION ------------------------------------------------------- //
// N/A
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
class DMXSlave : public DMXBase {
public:
	DMXSlave() { };
	DMXSlave(uint8_t startChannel, uint8_t channelSpan, dmxEvent_onFrameReceived* callback = 0);
	virtual ~DMXSlave();
};

}
// ---------- END OF CLASS DECLARATION ---------------------------------------------------------- //

#endif /* DMXSLAVE_H_ */
