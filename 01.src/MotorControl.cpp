/*
 * MotorControl.cpp
 *
 *  Created on: Oct 11, 2013
 *      Author: MSI
 */

#include "stdlib.h"
#include "string.h"
#include "MotorControl.h"
#include "lpc12xx_libcfg.h"
#include "lpc12xx_ssp.h"
#include "ssp.h"

MotorControl::MotorControl() {
	// TODO Auto-generated constructor stub
	//this->Init();
}

MotorControl::~MotorControl() {
	// TODO Auto-generated destructor stub
}

void MotorControl::Init(void) {
	//uint8_t configReg[2];
	//uint8_t getCommand;
	//getCommand = 0x3A;
	// Initiate IO as Out put
	IOCON_PIO_CFG_Type icon;
	IOCON_StructInit(&icon);
	icon.type = IOCON_PIO_0_20;
	IOCON_SetFunc(&icon);
	icon.type = IOCON_PIO_0_21;
	IOCON_SetFunc(&icon);
	icon.type = IOCON_PIO_0_22;
	IOCON_SetFunc(&icon);
	icon.type = IOCON_PIO_0_23;
	IOCON_SetFunc(&icon);
	GPIO_SetDir(MOTOR_PORT, L6842_RESET, 1);
	GPIO_SetDir(MOTOR_PORT, L6842_STCK, 1);
	//GPIO_SetDir(MOTOR_PORT, GATE_BP, 1);
	//GPIO_SetDir(MOTOR_PORT, GATE_BN, 1);
	GPIO_SetHighLevel(MOTOR_PORT, L6842_RESET, 1);
	SSPInit();

	//SSPSendPolling(&getCommand, 1, configReg, 2);

	//this->l6482Config.Word = (uint16_t)configReg[0] << 8;
	//this->l6482Config.Word |= (uint16_t)configReg[1];
	this->startMove = FALSE;
	//memset(this->level, 0, 4);
	this->level[0] = 0;
	this->level[1] = 50;
	this->level[2] = 100;
	this->level[3] = 150;

	this->speed = 60;
	this->moveToStep = 0;
	this->currentStep = 0;
	this->stepState = STEP_1;
	this->counter = 0;
	this->processState = MOTOR_STATE_IDLE;
	this->pendingNextState = MOTOR_STATE_IDLE;
	this->direction = MOVE_STOP;


}

void MotorControl::Reset(void) {
	this->Init();
}

void MotorControl::Tick(void) {
	uint8_t cmpStep;
	cmpStep = 0;
	switch (this->processState) {
	case MOTOR_STATE_IDLE :
		// Enter State
		// Initiate state
		if (!this->startMove) {
			this->startMove = TRUE;
			this->counter = 0;
			if (this->direction == MOVE_STOP) {
				this->direction = MOVE_FF;
			}
		}
		// Process
		cmpStep = this->processMove();

		// Exit State
		if (cmpStep) {
			this->startMove = FALSE;
			if (this->pendingNextState != this->processState) {
				this->processState = this->pendingNextState;
			}
			else {
				if (this->direction == MOVE_FF) {
					this->direction = MOVE_RW;
				} else {
					this->direction = MOVE_FF;
				}
			}
		}
		break;
	case MOTOR_STATE_MOVE_FF :
		// Enter State
		// Initiate state
		if (!this->startMove) {
			this->startMove = TRUE;
			this->direction = MOVE_FF;
			this->counter = 0;
		}
		// Process
		// Current Step must be less form MAX_STEP
		if (this->currentStep < MAX_STEP) {
			cmpStep = this->processMove();
		}
		// Go to Idle
		else {
			cmpStep = TRUE;
		}
		// Exit State
		if (cmpStep) {
			this->startMove = FALSE;
			// Increment Step
			this->currentStep++;
			// Finish Job
			if (this->currentStep == this->moveToStep) {
				this->pendingNextState = MOTOR_STATE_IDLE;
			}
			// Check max step
			else if (this->currentStep >= MAX_STEP) {
				this->pendingNextState = MOTOR_STATE_IDLE;
			}
			// Change State
			if (this->pendingNextState != this->processState) {
				this->processState = this->pendingNextState;
			}
		}
		break;
	case MOTOR_STATE_MOVE_FW :
		// Enter State
		// Initiate state
		if (!this->startMove) {
			this->startMove = TRUE;
			this->direction = MOVE_RW;
			this->counter = 0;
		}
		// Process
		cmpStep = this->processMove();
		// Exit State
		if (cmpStep) {
			this->startMove = FALSE;
			// Decrement Step
			this->currentStep--;
			// Finish Job
			if (this->currentStep == this->moveToStep) {
				this->pendingNextState = MOTOR_STATE_IDLE;
			}
			// Check max step
			else if (this->currentStep >= MIN_STEP) {
				this->pendingNextState = MOTOR_STATE_IDLE;
			}
			// Change State
			if (this->pendingNextState != this->processState) {
				this->processState = this->pendingNextState;
			}
		}
		break;
	}
}

Status MotorControl::MoveToLevel(MOVE_LEVEL_T position) {
	Status st;
	uint16_t lv;
	uint32_t step;
	st = ERROR;
	// Calculate number of step
	lv = this->level[position];
	// If move forward
	if (this->currentStep < (int16_t)(lv)) {
		step = lv - this->currentStep;
		this->pendingNextState = MOTOR_STATE_MOVE_FF;
	}
	// If move reverse
	else if (this->currentStep > (int16_t)(lv)) {
		step = this->currentStep - lv;
		this->pendingNextState = MOTOR_STATE_MOVE_FW;
	} else {
		// Error
		step = 0;
	}

	if (step != 0) {
		moveToStep = step;
		st = SUCCESS;
	}

	return (st);
}

Status MotorControl::MoveToStep(MOVE_DIR_T dir, uint32_t step) {
	Status st;
	st = ERROR;
	direction = dir;
	switch (dir) {
	case MOVE_FF:
		this->moveToStep = this->currentStep + step;
		if (this->moveToStep > MAX_STEP) {
			this->moveToStep = MAX_STEP;
		}
		this->pendingNextState = MOTOR_STATE_MOVE_FF;
		break;
	case MOVE_RW:
		this->moveToStep = this->currentStep - step;
		if (this->moveToStep <= MIN_STEP) {
			this->moveToStep = MIN_STEP;
		}
		this->pendingNextState = MOTOR_STATE_MOVE_FW;
		break;
	default:
		break;
	}
	st = SUCCESS;

	return (st);
}

void MotorControl::SetLevel(MOVE_LEVEL_T level, uint16_t val) {
	this->level[level] = val;
}

void MotorControl::SetMPS(uint16_t val) {
	this->movePerStep = val;
}

uint16_t MotorControl::GetMPS(void) {
	return this->movePerStep;
}

void MotorControl::GetMoveLevel(uint8_t* dOut, uint8_t* dSize) {
	memcpy(dOut, level, sizeof(level));
	dSize[0] = sizeof(level);
}

uint16_t MotorControl::GetSpeed(void) {
	return this->speed;
}

void MotorControl::ForceMove(MOVE_DIR_T dir) {
	if (dir == MOVE_STOP) {
		this->pendingNextState = MOTOR_STATE_IDLE;
	}
	else if (dir == MOVE_FF) {
		this->pendingNextState = MOTOR_STATE_MOVE_FF;
	}
	else {
		this->pendingNextState = MOTOR_STATE_MOVE_FW;
	}
}

void MotorControl::moveStep(MOVE_DIR_T dir, uint8_t en) {
	switch (this->stepState) {
	case STEP_1:
		/* Wavefrom
		 *     Here
		 * A+ |----|----|____|____|
		 * A- |____|____|----|----|
		 * B+ |----|____|____|----|
		 * B- |____|----|----|____|  */
		// Enter State

		// Process State

		// Move to next State
		if (!en) {
			if (dir == MOVE_FF) {
				this->stepState = STEP_2;
			} else {
				this->stepState = STEP_4;
			}
		}
		break;
	case STEP_2:
		/* Wavefrom
		 *          Here
		 * A+ |----|----|____|____|
		 * A- |____|____|----|----|
		 * B+ |----|____|____|----|
		 * B- |____|----|----|____|  */
		// Enter State

		// Process State

		// Move to next State
		if (!en) {
			if (dir == MOVE_FF) {
				this->stepState = STEP_3;
			} else {
				this->stepState = STEP_1;
			}
		}
		break;
	case STEP_3:
		/* Wavefrom
		 *               Here
		 * A+ |----|----|____|____|
		 * A- |____|____|----|----|
		 * B+ |----|____|____|----|
		 * B- |____|----|----|____|  */
		// Enter State

		// Process State

		// Move to next State
		if (!en) {
			if (dir == MOVE_FF) {
				this->stepState = STEP_4;
			} else {
				this->stepState = STEP_2;
			}
		}
		break;
	case STEP_4:
		/* Wavefrom
		 *                    Here
		 * A+ |----|----|____|____|
		 * A- |____|____|----|----|
		 * B+ |----|____|____|----|
		 * B- |____|----|----|____|  */
		// Enter State

		// Process State

		// Move to next State
		if (!en) {
			if (dir == MOVE_FF) {
				this->stepState = STEP_1;
			} else {
				this->stepState = STEP_3;
			}
		}
		break;
	default:
		break;
	}
}

uint8_t MotorControl::processMove(void) {
	uint8_t cmp;
	cmp = 0;
	switch (this->counter) {
	case 0:
		moveStep(this->direction, 1);
		break;
	case 3:
		moveStep(this->direction, 0);
		break;
	case 9:
		//moveStep(this->direction, 0);
		moveStep(this->direction, 1);
		break;
	case 13:
		moveStep(this->direction, 0);
		break;
	case 19:
		//moveStep(this->direction, 0);
		moveStep(this->direction, 1);
		break;
	case 23:
		moveStep(this->direction, 0);
		break;
	case 29:
		//moveStep(this->direction, 0);
		moveStep(this->direction, 1);
		break;
	case 33:
		moveStep(this->direction, 0);
		break;
	case 39:
		//moveStep(this->direction, 1);
		cmp = 1;
		break;
	default:
		break;
	}
	this->counter++;
	return cmp;
}


