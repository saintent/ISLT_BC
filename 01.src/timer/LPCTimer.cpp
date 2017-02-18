/** \file LPCTimer.cpp
 *	\brief 
 */

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
//
#include "LPCTimer.h"
#include "lpc12xx_sysctrl.h"
#include "lpc12xx_timer32.h"
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
LPCTimer::LPCTimer*	_lpcTimer;
// ---------- PRIVATE METHOD (FUNCTION PROTOTYPE) ----------------------------------------------- //
// N/A
// ---------- PRIVATE DATA ---------------------------------------------------------------------- //
// N/A
// ---------- PRIVATE PROGRAMMING DEFINE -------------------------------------------------------- //
#define TIMER	LPC_CT32B0
#define TIME_IRQ	TIMER_32_0_IRQn
// ---------- PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A
// ---------- SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
namespace LPCTimer {
//=========== Public Method ======================================================================//

LPCTimer::LPCTimer() {
	// TODO Auto-generated constructor stub
	_lpcTimer = this;
}

LPCTimer::~LPCTimer() {
	// TODO Auto-generated destructor stub
}

void LPCTimer::Init(uint32_t period) {
	TIM32_InitTypeDef timer;
	TIM32_MATCHTypeDef match;
	TIM32_PWMTypeDef pwm;
	SYS_ConfigAHBCLK(SYS_AHBCLKCTRL_CT32B0, ENABLE);
	timer.PrescaleOption = TIM32_PRESCALE_TICKVAL;
	timer.PrescaleValue = 12;
	TIM32_Init(TIMER, TIM32_MODE_TIMER, &timer);

	match.MatchChannel = TIM32_MATCH_CHANNEL0;
	match.IntOnMatch = ENABLE;
	match.StopOnMatch = DISABLE;
	match.ResetOnMatch = ENABLE;
	match.ExtMatchOutputType = TIM32_EXTMATCH_NOTHING;
	match.MatchValue = period;
	TIM32_ConfigMatch(TIMER, &match);
	pwm.MatchChannel = TIM32_PWM_CH3;
	pwm.IntOnMatch = DISABLE;
	pwm.StopOnMatch = DISABLE;
	pwm.ResetOnMatch = DISABLE;
	pwm.ExtPWMOutput = ENABLE;
	pwm.MatchValue = period/2;
	TIM32_ConfigPWM(TIMER, &pwm);

	// Attach PIO21 TO CT32B0_MAT3
	LPC_IOCON->PIO0_21 &= ~0x07;
	LPC_IOCON->PIO0_21 |= 0x04;

	NVIC_EnableIRQ(TIME_IRQ);
}

void LPCTimer::Start() {
	TIM32_ResetCounter(LPC_CT32B0);
	TIM32_Cmd(TIMER, ENABLE);
}

void LPCTimer::Stop() {
	TIM32_Cmd(TIMER, DISABLE);
}

void LPCTimer::SetMatch(uint32_t period) {
	TIMER->MR0 = period;
	TIMER->MR3 = period/2;
}

void LPCTimer::SetEventCallBack(LPC_TIMERCallBackEvent_t* callback) {
	this->onTimerEvent = callback;
}
//=========== Private Method ======================================================================//

} /* namespace LPCTimer */

void TIMER32_0_IRQHandler(void) {
	uint32_t ir;
	LPCTimer::eTimerEvent event;
	ir = TIMER->IR;

	if (ir & TIM32_INT_MR0) {
		TIMER->IR |= TIM32_INT_MR0;
		event = LPCTimer::MatchCh0Int;
	}

	if (ir & TIM32_INT_MR3) {
		TIMER->IR |= TIM32_INT_MR3;
		event = LPCTimer::MatchCh3Int;
	}

	if (_lpcTimer->onTimerEvent != 0) {
		_lpcTimer->onTimerEvent(event);
	}

}
// ---------- END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
