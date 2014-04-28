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
#include "l6482.h"
#include "l6482_type.h"


#define MOTOR_PORT		LPC_GPIO0

#define L6482_STCK		20
#define L6482_FLAG		21
#define L6482_RESET		22
#define L6482_BUSY		23

#define CT_TH			4
#define CT_MAX			40

#define MAX_STEP		500
#define MIN_STEP		0


typedef enum {
	LEVEL_0 = 0,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3
}MOVE_LEVEL_T;


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
	MOTOR_CURRENT_2A = 0,
	MOTOR_CURRENT_3A,
	MOTOR_CURRENT_4A,
	MOTOR_CURRENT_5A,
	MOTOR_CURRENT_6A
}MOTOR_CURRENT_LIMIT_T;

class MotorControl {
private :
	/*
	 * @brief Set Level of High
	 */
	uint32_t	level[4];

	MOTOR_CURRENT_LIMIT_T currentLimit;

	L6482_REG l6482_Register;

	L6482	device;
public :
	MotorControl();
	virtual ~MotorControl();

	void Init(void);
	void Reset(void);
	void Tick(void);
	Status MoveToLevel(MOVE_LEVEL_T level);
	Status MoveToStep(L6482_DIR_Typedef dir, uint32_t step);
	void SetLevel(MOVE_LEVEL_T level, uint16_t val);
	void SetMPS(uint16_t val);
	uint16_t GetMPS(void);
	void GetMoveLevel(uint8_t* dOut, uint8_t* dSize);
	uint16_t GetSpeed(void);
	Bool IsBusy(void);
	//void ForceMove(MOVE_DIR_T dir);
private :
	//void moveStep(MOVE_DIR_T dir, uint8_t en);
	//uint8_t processMove(void);
	uint8_t	TvalTable[5];
	void motorDebug(char* str,uint8_t str_len);
	void motorPrintStatus(void);
};

#endif /* MOTORCONTROL_H_ */
