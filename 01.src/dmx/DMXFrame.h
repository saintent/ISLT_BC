/** \file DMXFrame.h
 *	\brief 
 */

#ifndef DMXFRAME_H_
#define DMXFRAME_H_

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
#include "stdint.h"
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
class DMXFrame {
public:

	DMXFrame(uint16_t frameSize = 16);
	DMXFrame(DMXFrame &buffer);
	virtual ~DMXFrame(void);

	uint8_t GetChanelData(uint16_t index);
	uint8_t* GetFrameData(void);

	uint8_t StartWriteFrame(void);
	uint8_t WriteDataToFrame(uint8_t data);

	uint8_t SetFrameSize(uint16_t size);

	uint8_t &operator[](uint16_t index);

private:
	uint8_t* pDataFrame;
	uint16_t frameSize;
	uint16_t currentCh;

};

}

// ---------- END OF CLASS DECLARATION ---------------------------------------------------------- //

#endif /* DMXFRAME_H_ */
