/** \file MotionControl.h
 *	\brief 
 */

#ifndef MOTIONCONTROL_H_
#define MOTIONCONTROL_H_

// ---------- SYSTEM INCLUDE --------------------------------------------------------------------- //
#include "stdint.h"
#include "MotorControl.h"
//#include "Display7Segment.h"
// ---------- EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A
// ---------- PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
// N/A
// ---------- ENUMERATOR DEFINITION -------------------------------------------------------------- //
namespace Motion {
	enum ControlState {
		DCMode,
		SolMode,
		IncMode,
		AbsMode,
		LoopMode,
		SetupDC,
		SetupSol,
		SetupInc,
		SetupAbs,
		SetupLoop
	};

	enum SetupParameter {
		SP_SpeedFW,
		SP_SpeedFR,
		SP_Position,
		SP_Speed,
		SP_Output,
		SP_Delay,
	};

	enum OutputMode {
		NotUse,
		ON,
		OFF,
		Pulse
	};
}
// ---------- TYPEDEF DATA TYPE DEFINITION ------------------------------------------------------- //
// N/A
// ---------- STRUCT OR UNION DATA TYPE DEFINITION ----------------------------------------------- //
/*typedef struct {
	uint32_t	speed;
	uint32_t 	acc;
};*/
// ---------- PUBLIC MACRO DEFINITION ------------------------------------------------------------ //
// N/A
// ---------- EXTERN FUNCTION -------------------------------------------------------------------- //
// N/A
// ---------- EXTERN VARIABLE -------------------------------------------------------------------- //
// N/A
// ---------- CLASS DECLARATION ----------------------------------------------------------------- //
namespace Motion {

/*
 *
 */
class MotionControl {
public:
	MotionControl();
	virtual ~MotionControl();

private:
	//Display7Segment*		display;
	MOTOR::MotorControl* 	motor;
	ControlState			controlState;

};

} /* namespace Motion */
// ---------- END OF CLASS DECLARATION ---------------------------------------------------------- //

#endif /* MOTIONCONTROL_H_ */
