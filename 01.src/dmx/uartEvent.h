/** \file uartEvent.h
*	\brief 
*/

#ifndef UARTEVENT_H_
#define UARTEVENT_H_

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
//
// ---------- EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A
// ---------- PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
// N/A
// ---------- ENUMERATOR DEFINITION -------------------------------------------------------------- //
namespace uart {
	enum eUartEvent {
		eUartRecvData,
		eUartOverrun,
		eUartParityError,
		eUartFramingError,
		eUartBreakInt,
		eUartTransmitCmp,
	};
}
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


// ---------- END OF CLASS DECLARATION ---------------------------------------------------------- //

#endif /* UARTEVENT_H_ */
