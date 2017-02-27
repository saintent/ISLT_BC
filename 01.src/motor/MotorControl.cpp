/*
 * MotorControl.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Prustya
 */

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
#include "MotorControl.h"
#include "LPCTimer.h"
#include "lpc12xx_gpio.h"
#include "lpc12xx_iocon.h"
#ifdef ARDUINO >= 100
#include <Arduino.h>
#endif
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
static unsigned long sqrt(unsigned long x);
void TimerEvent_Callback(LPCTimer::eTimerEvent event);
//unsigned int min(unsigned int x, unsigned int y);
// ---------- PRIVATE DATA ---------------------------------------------------------------------- //
#ifndef MOTION_IMPLEMENT_ISR
MOTOR::MotorControl motionControl;
#endif
MOTOR::MotorControl* _motionControl;
// ---------- PRIVATE PROGRAMMING DEFINE -------------------------------------------------------- //
#define MCLK_PIN	21
#define MDIR_PIN	22
#define MEN_PIN		20

// ---------- PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A
// ---------- SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
//=========== Public Method ======================================================================//
namespace MOTOR {
MotorControl::MotorControl() {
	_motionControl = this;
	this->Init();
}

MotorControl::~MotorControl() {
	// TODO Auto-generated destructor stub
}

void MotorControl::begin() {
	this->Init();

}

void MotorControl::Init() {
	this->accelCount = 0;
	this->decelStart = 0;
	this->decelVal = 0;
	this->minDelay = 0;
	this->runState = SPS_Stop;
	this->stepDelay = 0;
	this->stepPosition = 0;

	timer.Init(1000);
	timer.SetEventCallBack(TimerEvent_Callback);

	IOCON_PIO_CFG_Type iocon;
	IOCON_StructInit(&iocon);
	iocon.type = IOCON_PIO_0_20;
	iocon.od = IOCON_PIO_OD_ENABLE;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_0_22;
	IOCON_SetFunc(&iocon);
	GPIO_SetDir(LPC_GPIO0, MDIR_PIN, 1);
	GPIO_SetDir(LPC_GPIO0, MEN_PIN, 1);
}
void MotorControl::MoveTo(uint32_t position) {
	// Get ditance
	int32_t distance;
	if (!this->isBusy) {
		StopTimer();
		this->isBusy = TRUE;
		distance = position - this->currentStep;
		this->targetStep = distance + (int32_t) this->currentStep;
		this->Move(distance, 800, 500, 5000);

	}
}

void MotorControl::Move(int32_t step, uint32_t accel, uint32_t decel,
		uint32_t speed) {
	//! Number of steps before we hit max speed.
	unsigned int max_s_lim;
	//! Number of steps before we must start deceleration (if accel does not hit max speed).
	unsigned int accel_lim;

	// Set direction from sign on step value.
	if (step < 0) {
		this->direction = Direction_CCW;
		step = -step;
		this->SetDirection(Direction_CCW);
	} else {
		this->direction = Direction_CW;
		this->SetDirection(Direction_CW);
	}

	// If moving only 1 step.
	if (step == 1) {
		// Move one step...
		this->accelCount = -1;
		// ...in DECEL state.
		this->runState = SPS_Decel;
		// Just a short delay so main() can act on 'running'.
		this->stepDelay = 1000;
		//status.running = TRUE;
	}
	// Only move if number of steps to move is not zero.
	else if (step != 0) {
		// Refer to documentation for detailed information about these calculations.

		// Set max speed limit, by calc min_delay to use in timer.
		// min_delay = (alpha / tt)/ w
		this->minDelay = A_T_x100 / speed;

		// Set accelration by calc the first (c0) step delay .
		// step_delay = 1/tt * sqrt(2*alpha/accel)
		// step_delay = ( tfreq*0.676/100 )*100 * sqrt( (2*alpha*10000000000) / (accel*100) )/10000
		this->stepDelay = (T1_FREQ_148 * sqrt(A_SQ / accel)) / 100;

		// Find out after how many steps does the speed hit the max speed limit.
		// max_s_lim = speed^2 / (2*alpha*accel)
		max_s_lim = (long) speed * speed
				/ (long) (((long) A_x20000 * accel) / 100);
		// If we hit max speed limit before 0,5 step it will round to 0.
		// But in practice we need to move atleast 1 step to get any speed at all.
		if (max_s_lim == 0) {
			max_s_lim = 1;
		}

		// Find out after how many steps we must start deceleration.
		// n1 = (n1+n2)decel / (accel + decel)
		accel_lim = ((long) step * decel) / (accel + decel);
		// We must accelrate at least 1 step before we can start deceleration.
		if (accel_lim == 0) {
			accel_lim = 1;
		}

		// Use the limit we hit first to calc decel.
		if (accel_lim <= max_s_lim) {
			this->decelVal = accel_lim - step;
		} else {
			this->decelVal = -((long) max_s_lim * accel) / decel;
		}
		// We must decelrate at least 1 step to stop.
		if (this->decelVal == 0) {
			this->decelVal = -1;
		}

		// Find step to start decleration.
		this->decelStart = step + this->decelVal;

		// If the maximum speed is so low that we dont need to go via accelration state.
		if ((int32_t) this->stepDelay <= this->minDelay) {
			this->stepDelay = this->minDelay;
			this->runState = SPS_Run;
		} else {
			this->runState = SPS_Accel;
		}

		// Reset counter.
		this->accelCount = 0;
		timer.SetMatch(this->stepDelay);
		StartTimer();

	} else {
		this->isBusy = FALSE;
	}
}

void MotorControl::Callback(MotorControl* obj) {
	obj->Process();
}

} /* end of namespace MOTOR */
//=========== Private Method ======================================================================//
namespace MOTOR {

void MotorControl::Process() {
	// Holds next delay period.
	int32_t new_step_delay;
	// Remember the last step delay used when accelrating.
	//static int last_accel_delay;
	// Counting steps when moving.
	//static unsigned int step_count = 0;
	// Keep track of remainder from new_step-delay calculation to incrase accurancy
	//static unsigned int rest = 0;

	timer.SetMatch(this->stepDelay);

	if (this->runState != SPS_Stop) {
		if (this->direction == Direction_CCW) {
			this->currentStep--;
		} else {
			this->currentStep++;
		}
	}

	switch (this->runState) {
	case SPS_Stop:
		this->stepCount = 0;
		//step_count = 0;
		this->rest = 0;

		// Stop Timer/Counter 1.
		this->isBusy = FALSE;
		StopTimer();
		break;

	case SPS_Accel:

		//sm_driver_StepCounter(srd.dir);

		this->stepCount++;
		this->accelCount++;
		new_step_delay = this->stepDelay
				- (((2 * (long) this->stepDelay) + rest)
						/ (4 * this->accelCount + 1));
		rest = ((2 * (long) this->stepDelay) + rest)
				% (4 * this->accelCount + 1);
		// Chech if we should start decelration.
		if (this->stepCount >= this->decelStart) {
			this->accelCount = this->decelVal;
			this->runState = SPS_Decel;
		}
		// Chech if we hitted max speed.
		else if (new_step_delay <= this->minDelay) {
			this->lastAccelDelay = new_step_delay;
			new_step_delay = this->minDelay;
			this->rest = 0;
			this->runState = SPS_Run;
		}
		break;

	case SPS_Run:

		//sm_driver_StepCounter(srd.dir);

		this->stepCount++;
		new_step_delay = this->minDelay;
		// Chech if we should start decelration.
		if (this->stepCount >= this->decelStart) {
			this->accelCount = this->decelVal;
			// Start decelration with same delay as accel ended with.
			new_step_delay = this->lastAccelDelay;
			this->runState = SPS_Decel;
			//rest = 0;

		}
		break;

	case SPS_Decel:

		//sm_driver_StepCounter(srd.dir);

		this->stepCount++;
		this->accelCount++;
		new_step_delay = this->stepDelay
				- (((2 * (long) this->stepDelay) + rest)
						/ (4 * this->accelCount + 1));
		rest = ((2 * (long) this->stepDelay) + rest)
				% (4 * this->accelCount + 1);
		// Check if we at last step
		if (this->accelCount >= 0) {
			this->runState = SPS_Stop;
		}
		break;
	}

	this->stepDelay = new_step_delay;

}

void MotorControl::StartTimer() {
	timer.Start();
}

void MotorControl::StopTimer() {
	timer.Stop();
}

void MotorControl::SetDirection(Direction dir) {
	if (dir == Direction_CW) {
		GPIO_SetHighLevel(LPC_GPIO0, MDIR_PIN, 1);
	} else {
		GPIO_SetLowLevel(LPC_GPIO0, MDIR_PIN, 1);
	}
}

}
/* end of namespace MOTOR */

/*! \brief Square root routine.
 *
 * sqrt routine 'grupe', from comp.sys.ibm.pc.programmer
 * Subject: Summary: SQRT(int) algorithm (with profiling)
 *    From: warwick@cs.uq.oz.au (Warwick Allison)
 *    Date: Tue Oct 8 09:16:35 1991
 *
 *  \param x  Value to find square root of.
 *  \return  Square root of x.
 */
static unsigned long sqrt(unsigned long x) {
	register unsigned long xr;  // result register
	register unsigned long q2;  // scan-bit register
	register unsigned char f;   // flag (one bit)

	xr = 0;                     // clear result
	q2 = 0x40000000L;           // higest possible result bit
	do {
		if ((xr + q2) <= x) {
			x -= xr + q2;
			f = 1;                  // set flag
		} else {
			f = 0;                  // clear flag
		}
		xr >>= 1;
		if (f) {
			xr += q2;               // test flag
		}
	} while (q2 >>= 2);          // shift twice
	if (xr < x) {
		return xr + 1;             // add for rounding
	} else {
		return xr;
	}
}

void TimerEvent_Callback(LPCTimer::eTimerEvent event) {
	MOTOR::MotorControl::Callback(_motionControl);
}
// ---------- END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
