/*
 * MotorControl.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Prustya
 */

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
#include "stdint.h"
#include "LPCTimer.h"
// ---------- EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A
// ---------- PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
#define MOTION_IMPLEMENT_ISR
/*! \Brief Frequency of timer1 in [Hz].
 *
 * Modify this according to frequency used. Because of the prescaler setting,
 * the timer1 frequency is the clock frequency divided by 8.
 */
// Timer/Counter 1 running on 3,686MHz / 8 = 460,75kHz (2,17uS). (T1-FREQ 460750)
//#define T1_FREQ 2000000
// Timer is running on 500 kHz
#define T1_FREQ 500000

//! Number of (full)steps per round on stepper motor in use.
#define FSPR 200

#define SPR FSPR

// Maths constants. To simplify maths when calculating in speed_cntr_Move().
#define ALPHA (2*3.14159/SPR)                    // 2*pi/spr
#define A_T_x100 ((long)(ALPHA*T1_FREQ*100))     // (ALPHA / T1_FREQ)*100
#define T1_FREQ_148 ((int)((T1_FREQ*0.676)/100)) // divided by 100 and scaled by 0.676
#define A_SQ (long)(ALPHA*2*10000000000)         // ALPHA*2*10000000000
#define A_x20000 (int)(ALPHA*20000)              // ALPHA*20000
// ---------- ENUMERATOR DEFINITION -------------------------------------------------------------- //
namespace MOTOR {
enum Direction {
	Direction_CW, Direction_CCW
};

enum SpeedProfileState {
	SPS_Stop, SPS_Accel, SPS_Decel, SPS_Run
};
}
// ---------- TYPEDEF DATA TYPE DEFINITION ------------------------------------------------------- //
// N/A
// ---------- STRUCT OR UNION DATA TYPE DEFINITION ----------------------------------------------- //
// N/A
// ---------- PUBLIC MACRO DEFINITION ------------------------------------------------------------ //
#define KERNEL_TIMER	LPCTimer::LPCTimer
// ---------- EXTERN FUNCTION -------------------------------------------------------------------- //
// N/A
// ---------- EXTERN VARIABLE -------------------------------------------------------------------- //
// N/A
// ---------- CLASS DECLARATION ----------------------------------------------------------------- //
namespace MOTOR {
/*
 *
 */
class MotorControl {
public:
	MotorControl();
	virtual ~MotorControl();

	void Init();
	void begin();

	void MoveTo(uint32_t position);
	void Move(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);

	static void Callback(MotorControl* obj);

private:
	void Process(void);
	void StartTimer(void);
	void StopTimer();
	void SetDirection(Direction dir);

private:
	KERNEL_TIMER		timer;
	SpeedProfileState runState;
	Direction direction;
	uint32_t stepPosition;
	uint32_t stepDelay;
	uint32_t decelStart;
	int32_t decelVal;
	int32_t minDelay;
	int32_t accelCount;

	uint32_t lastAccelDelay;
	uint32_t stepCount;
	int32_t rest;

	uint32_t currentStep;
	uint32_t targetStep;
	uint32_t home;
	uint8_t isBusy;

};

}

#ifndef MOTION_IMPLEMENT_ISR
extern MOTOR::MotorControl motionControl;
#endif

// ---------- END OF CLASS DECLARATION ---------------------------------------------------------- //
#endif /* MOTORCONTROL_H_ */

