//================ File Description =========================================//
//=== File name : tb6600.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "tb6600.h"
#include "string.h"
#include "stdint.h"
#include "lpc_types.h"
#include "lpc12xx_gpio.h"
#include "lpc_uart.h"
#include "lpc12xx_iocon.h"
//================ PUBLIC METHOD ============================================//
// extern function from another file
//
//================ PRIVATE METHOD ===========================================//
// non extern function
//
//================ PUBLIC DATA ==============================================//
// extern data
//
//================ PRIVATE DATA =============================================//
// non extern data
//
//================ PRIVATE DEFINE ===========================================//
//
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//

tb6600::tb6600() {
	// TODO Auto-generated constructor stub

}

tb6600::~tb6600() {
	// TODO Auto-generated destructor stub
}

void tb6600::Init(void) {
	IOCON_PIO_CFG_Type iocon;
	IOCON_StructInit(&iocon);
	/*iocon.type = IOCON_PIO_0_31;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_1_0;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_1_1;
	IOCON_SetFunc(&iocon);*/
	iocon.type = IOCON_PIO_0_20;
	iocon.od = IOCON_PIO_OD_ENABLE;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_0_21;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_0_22;
	IOCON_SetFunc(&iocon);
	GPIO_SetDir(LPC_GPIO0, MCLK_PIN, 1);
	GPIO_SetDir(LPC_GPIO0, MDIR_PIN, 1);
	GPIO_SetDir(LPC_GPIO0, MRST_PIN, 1);
	GPIO_SetDir(LPC_GPIO0, MEN_PIN, 1);
	/*GPIO_SetDir(LPC_GPIO0, M1_PIN, 1);
	GPIO_SetDir(LPC_GPIO1, M2_PIN, 1);
	GPIO_SetDir(LPC_GPIO1, M3_PIN, 1);
	GPIO_SetDir(LPC_GPIO1, MLA_PIN, 1);
	GPIO_SetDir(LPC_GPIO1, MTQ_PIN, 1);*/

	this->ResetMotor(DISABLE);
	//GPIO_SetHighLevel(LPC_GPIO0, MRST_PIN, 1);

	this->EnableMotor(ENABLE);
	//GPIO_SetHighLevel(LPC_GPIO0, MEN_PIN, 1);

	//this->Set(TB6600_M, 0x6);
	/*GPIO_SetHighLevel(LPC_GPIO0, M1_PIN, 1);
	GPIO_SetHighLevel(LPC_GPIO1, M2_PIN, 1);
	GPIO_SetLowLevel(LPC_GPIO1, M3_PIN, 1);*/

	//this->Set(TB6600_TQ, DISABLE);
	//GPIO_SetLowLevel(LPC_GPIO1, MTQ_PIN, 1);
	//this->Set(TB6600_LA, ENABLE);
	//GPIO_SetHighLevel(LPC_GPIO1, MLA_PIN, 1);

	// Init High state of CLK
	GPIO_SetHighLevel(LPC_GPIO0, 21, 1);

	this->currentClock = 0;
	this->prescale = 5;
	this->currentStep = 0;
	this->targetStep = 0;
	this->dir = TB6600_CW;
	this->inProcess = FALSE;
	this->manualMode = FALSE;
}

uint8_t tb6600::Set(TB6600SetFn fn, uint8_t value) {
	uint8_t result;
	result = SUCCESS;
	switch (fn) {
		case TB6600_M:
			if (value < 7) {
				this->mfn = value;
				// For M3
				if (value & 0x01) {
					GPIO_SetHighLevel(LPC_GPIO1,M3_PIN, 1);
				}
				else {
					GPIO_SetLowLevel(LPC_GPIO1, M3_PIN, 1);
				}
				// For M2
				if (value & 0x02) {
					GPIO_SetHighLevel(LPC_GPIO1, M2_PIN, 1);
				}
				else {
					GPIO_SetLowLevel(LPC_GPIO1, M2_PIN, 1);
				}
				// For M1
				if (value & 0x04) {
					GPIO_SetHighLevel(LPC_GPIO0, M1_PIN, 1);
				}
				else {
					GPIO_SetLowLevel(LPC_GPIO0, M1_PIN, 1);
				}
			}
			else {
				result = ERROR;
			}

			break;
		case TB6600_TQ :
			if (value == ENABLE) {
				GPIO_SetHighLevel(LPC_GPIO1,MTQ_PIN, 1);
				this->tq = ENABLE;
			}
			else {
				GPIO_SetLowLevel(LPC_GPIO1, MTQ_PIN, 1);
				this->tq = DISABLE;
			}
			break;
		case TB6600_LA :
			if (value == ENABLE) {
				GPIO_SetHighLevel(LPC_GPIO1, MLA_PIN, 1);
				this->la = ENABLE;
			}
			else {
				GPIO_SetLowLevel(LPC_GPIO1, MLA_PIN, 1);
				this->la = DISABLE;
			}
			break;
		case TB6600_PSC :
			this->prescale = value;
			break;
		default:
			break;
	}
}

uint8_t tb6600::Get(TB6600SetFn fn) {
	uint8_t result;
	switch (fn) {
	case TB6600_M:
		result = this->mfn;
		break;
	case TB6600_TQ:
		result = this->tq;
		break;
	case TB6600_LA:
		result = this->la;
		break;
	case TB6600_EN :
		result = this->en;
		break;
	case TB6600_PSC :
		result = this->prescale;
	default:
		break;
	}
	return result;
}

uint32_t tb6600::GetCurrentPosition(void) {
	return this->currentStep;
}

FunctionalState tb6600::EnableMotor(FunctionalState state) {
	if (state == ENABLE) {
		GPIO_SetHighLevel(LPC_GPIO0, MEN_PIN, 1);
		this->en = ENABLE;
	}
	else {
		GPIO_SetLowLevel(LPC_GPIO0, MEN_PIN, 1);
		this->en = DISABLE;
	}
	return state;
}

FunctionalState tb6600::ResetMotor(FunctionalState state) {
	if (state == DISABLE) {
		GPIO_SetHighLevel(LPC_GPIO0, MRST_PIN, 1);
		this->en = ENABLE;
	}
	else {
		GPIO_SetLowLevel(LPC_GPIO0, MRST_PIN, 1);
		this->en = DISABLE;
	}
	return state;
}

Status tb6600::SetDir(TB6600Dir dir) {
	Status st;
	st = ERROR;
	if (!inProcess) {
		if (dir == TB6600_CW) {
			GPIO_SetHighLevel(LPC_GPIO0, MDIR_PIN, 1);
		} else {
			GPIO_SetLowLevel(LPC_GPIO0, MDIR_PIN, 1);
		}
		this->dir = dir;
		st = SUCCESS;
	}
	return st;
}

Status tb6600::Move(uint32_t step, TB6600Dir dir) {
	Status busy;
	busy = ERROR;
	if (!inProcess) {
		EnableMotor(ENABLE);
		ResetMotor(DISABLE);
		this->SetDir(dir);
		inProcess = TRUE;
		if (dir == TB6600_CW) {
			// must be implement max step
			targetStep = currentStep + step;
			if (targetStep > 3800) {
				targetStep = 3800;
			}
		}
		else {
			if (currentStep < step) {
				targetStep = 0;
			}
			else {
				targetStep = currentStep - step;
			}
		}
	}
	else {
		busy = SUCCESS;
	}
	return busy;
}

void tb6600::SetManualMode(FunctionalState state) {
	manualMode = state;
}

void tb6600::ForceActive(FunctionalState state) {
	if (state == ENABLE) {
		inProcess = TRUE;
	}
	else {
		inProcess = FALSE;
	}
}

void tb6600::Tick(void) {
	// if use pre-scale
	if (inProcess) {
		if (++currentClock >= prescale) {
			LPC_GPIO0 ->NOT = 1 << 21;
			if (GPIO_GetPinValue(LPC_GPIO0, 21) == TRUE) {
				// Getting 1 clk
				if (dir == TB6600_CW) {
					currentStep++;
					if (currentStep > 3800) {
						currentStep = 3800;
					}
				}
				else {
					if (currentStep > 0) {
						currentStep--;
					}
				}
				if (!manualMode) {
					if (currentStep == targetStep) {
						// complete
						inProcess = FALSE;
					}
				}
				// Min step
				if (currentStep == 0) {
					inProcess = FALSE;
				}
				// Max step

			}
			currentClock = 0;
		}
	}
	if (currentStep == 0) {
		EnableMotor(DISABLE);
		ResetMotor(ENABLE);
	}
}

//================ END OF FILE ==============================================//

