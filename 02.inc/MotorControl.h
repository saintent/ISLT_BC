/*
 * MotorControl.h
 *
 *  Created on: Oct 11, 2013
 *      Author: MSI
 */

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_

#include "stdint.h"
#include "lpc_types.h"

#define MOTOR_PORT		LPC_GPIO0
#define GATE_AP			23
#define GATE_AN			22
#define GATE_BP			21
#define GATE_BN			20
#define CT_TH			4
#define CT_MAX			40

#define MAX_STEP		500
#define MIN_STEP		0

typedef enum {
	MOVE_STOP = 0,
	MOVE_FF,		// Move Motor Forward
	MOVE_RW			// Move Motor Reverse
}MOVE_DIR_T;

typedef enum {
	LEVEL_0 = 0,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3
}MOVE_LEVEL_T;

typedef enum {
	STEP_1 = 0,
	STEP_2,
	STEP_3,
	STEP_4
}MOTOR_SETP_T;

typedef enum {
	MOTOR_ATTR_MPS = 0,
	MOTOR_ATTR_LV,
	MOTOR_ATTR_SPD
}MOTOR_ATTR_TYPE_T;

typedef enum {
	MOTOR_ACT_MTS = 0,
	MOTOR_ACT_MTL,
	MOTOR_ACT_SETLV
}MOTOR_ACT_TYPE_T;

typedef enum {
	MOTOR_STATE_IDLE = 0,
	MOTOR_STATE_MOVE_FF,
	MOTOR_STATE_MOVE_FW
}MOTOR_STATE_TYPT_T;

class MotorControl {
private :
	/*
	 * @brief Start Flag
	 */
	uint8_t		startMove;
	/*
	 * @brief Set Level of High
	 */
	uint16_t	level[4];
	/*
	 * @brief Move distance per step
	 */
	uint16_t	movePerStep;
	/*
	 * @brief Motor speed (RPM)
	 */
	uint16_t	speed;
	/*
	 * @brief Number of step to move
	 */
	int16_t	moveToStep;
	/*
	 * @brief Current step
	 */
	int16_t	currentStep;
	/*
	 * @brief Current core state
	 */
	MOTOR_SETP_T stepState;
	/*
	 * @brief Move direction
	 */
	MOVE_DIR_T	direction;
	/*
	 * @brief Counter period
	 */
	uint8_t counter;
	/*
	 * @brief Motor process state ref MOTOR_STATE_TYPT_T
	 */
	MOTOR_STATE_TYPT_T processState;
	/*
	 * @brief Pending Next State
	 */
	MOTOR_STATE_TYPT_T pendingNextState;
public :
	MotorControl();
	virtual ~MotorControl();

	void Init(void);
	void Reset(void);
	void Tick(void);
	Status MoveToLevel(MOVE_LEVEL_T level);
	Status MoveToStep(MOVE_DIR_T dir, uint32_t step);
	void SetLevel(MOVE_LEVEL_T level, uint16_t val);
	void SetMPS(uint16_t val);
	uint16_t GetMPS(void);
	void GetMoveLevel(uint8_t* dOut, uint8_t* dSize);
	uint16_t GetSpeed(void);
	void ForceMove(MOVE_DIR_T dir);
private :
	void moveStep(MOVE_DIR_T dir, uint8_t en);
	uint8_t processMove(void);
	void calcStep(uint16_t start, uint16_t end);
};

#endif /* MOTORCONTROL_H_ */
