/** \file LPCTimer.h
 *	\brief 
 */

#ifndef LPCTIMER_H_
#define LPCTIMER_H_

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
#include "stdint.h"
#include "lpc12xx_timer32.h"
// ---------- EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A
// ---------- PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
// N/A
// ---------- ENUMERATOR DEFINITION -------------------------------------------------------------- //
namespace LPCTimer {
	enum eTimerEvent {
		MatchCh0Int,
		MatchCh1Int,
		MatchCh2Int,
		MatchCh3Int
	};
}
// ---------- TYPEDEF DATA TYPE DEFINITION ------------------------------------------------------- //
// N/A
// ---------- STRUCT OR UNION DATA TYPE DEFINITION ----------------------------------------------- //
typedef void LPC_TIMERCallBackEvent_t(LPCTimer::eTimerEvent evnet);
// ---------- PUBLIC MACRO DEFINITION ------------------------------------------------------------ //
// N/A
// ---------- EXTERN FUNCTION -------------------------------------------------------------------- //
// N/A
// ---------- EXTERN VARIABLE -------------------------------------------------------------------- //
//
// ---------- CLASS DECLARATION ----------------------------------------------------------------- //
namespace LPCTimer {

/*
 *
 */
class LPCTimer {
public:
	LPCTimer();
	virtual ~LPCTimer();

	void Init(uint32_t period);

	void Start();
	void Stop();

	void SetMatch(uint32_t period);

	void SetEventCallBack(LPC_TIMERCallBackEvent_t* callback);

	LPC_TIMERCallBackEvent_t* onTimerEvent;
};

} /* namespace LPCTimer */

//extern LPCTimer::LPCTimer	lpcTimer;
// ---------- END OF CLASS DECLARATION ---------------------------------------------------------- //

#endif /* LPCTIMER_H_ */
