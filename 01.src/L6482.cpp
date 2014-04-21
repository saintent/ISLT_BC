//================ File Desciption =========================================//
//=== File name : L6482.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "stdint.h"
#include "L6482.h"
#include "l6482_type.h"
#include "lpc_types.h"
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

L6482::L6482() {
	// TODO Auto-generated constructor stub

}

L6482::~L6482() {
	// TODO Auto-generated destructor stub
}

uint32_t L6482::GetCurrentPosition(void) {
	return getParamU32(L6482_ABS_POS);
}
uint16_t L6482::GetElecPosition(void) {
	return getParamU16(L6482_EL_POS);
}
uint32_t L6482::GetMarkPosition(void) {
	return getParamU32(L6482_MARK);
}
uint32_t L6482::GetSpeed(void) {
	return getParamU32(L6482_SPEED);
}
uint16_t L6482::GetAcceleration(void) {
	return getParamU16(L6482_ACC);
}
uint16_t L6482::GetDeceleration(void) {
	return getParamU16(L6482_DEC);
}
uint16_t L6482::GetMaxSpeed(void) {
	return getParamU16(L6482_MAX_SPEED);
}
Status L6482::SetMaxSpeed(uint16_t max) {
	uint8_t data[2];
	data[1] = (uint8_t)(max >> 8);
	data[0] = (uint8_t)max;
	return setParamU16(L6482_MAX_SPEED, data);
}
uint16_t L6482::GetMinSpeed(void) {
	return getParamU16(L6482_ABS_POS);
}
Status L6482::SetMinSpeed(uint16_t min) {
	uint8_t data[2];
	data[1] = (uint8_t)(min >> 8);
	data[0] = (uint8_t)min;
	return setParamU16(L6482_MIN_SPEED, data);
}
uint16_t L6482::GetFullStepSpeed(void) {
	return getParamU32(L6482_FS_SPD);
}
uint8_t L6482::GetHoldingRefVolt(void) {
	return getParamU8(L6482_TVAL_HOLD);
}
Status L6482::SetHoldingRefVolt(uint8_t volt) {
	return setParamU8(L6482_TVAL_HOLD, volt);
}
uint8_t L6482::GetConstanceRefVolt(void) {
	return getParamU8(L6482_TVAL_RUN);
}
Status L6482::SetConstanceRefVolt(uint8_t volt) {
	return setParamU8(L6482_TVAL_RUN, volt);
}
uint8_t L6482::GetAccStartRefVolt(void) {
	return getParamU8(L6482_TVAL_ACC);
}
Status L6482::SetAccStartRefVolt(uint8_t volt) {
	return setParamU8(L6482_TVAL_ACC, volt);
}
uint8_t L6482::GetDccStartRefVolt(void) {
	return getParamU8(L6482_TVAL_DEC);
}
Status L6482::SetDccStartRefVolt(uint8_t volt) {
	return setParamU8(L6482_TVAL_DEC, volt);
}
uint8_t L6482::GetFastDecaySettting(void) {
	return getParamU8(L6482_T_FAST);
}
Status L6482::SetFastDecaySetting(uint8_t volt) {
	return setParamU8(L6482_T_FAST, volt);
}
uint8_t L6482::GetMinOnTime(void) {
	return getParamU8(L6482_TON_MIN);
}
Status L6482::SetMinOnTime(uint8_t time) {
	return setParamU8(L6482_TON_MIN, time);
}
uint8_t L6482::GetMinOffTime(void) {
	return getParamU8(L6482_TOFF_MIN);
}
Status L6482::SetMinOffTime(uint8_t time) {
	return setParamU8(L6482_TOFF_MIN, time);
}
uint8_t L6482::GetADCOut(void) {
	return getParamU8(L6482_ADC_OUT);
}
uint8_t L6482::GetOCDThreshold(void) {
	return getParamU8(L6482_OCD_TH);
}
Status L6482::SetOCDThreshold(uint8_t value) {
	return setParamU8(L6482_OCD_TH, value);
}
uint8_t L6482::GetStepMode(void) {
	return getParamU8(L6482_STEP_MODE);
}
Status L6482::SetStepMode(uint8_t value) {
	return setParamU8(L6482_STEP_MODE, value);
}
uint8_t L6482::GetAlarmSetting(void) {
	return getParamU8(L6482_ALARM_EN);
}
Status L6482::SetAlarmSetting(uint8_t value) {
	return setParamU8(L6482_ALARM_EN, value);
}
uint16_t L6482::GetGateDriveConfig(uint8_t gate) {
	uint16_t out;
	if (gate == 1) {
		out = getParamU16(L6482_GATECFG1);
	}
	else if (gate == 2) {
		out = getParamU16(L6482_GATECFG2);
	}
	else {
		out = 0;
	}
	return out;
}
Status L6482::SetGateDriveConfig(uint8_t gate, uint16_t value) {
	uint8_t data[2];
	Status result;
	if (gate == 1) {
		result = setParamU16(L6482_GATECFG1, data);
	}
	else if (gate == 2) {
		result = setParamU16(L6482_GATECFG2, data);
	}
	else {
		result = FALSE;
	}
	return result;
}
uint16_t L6482::GetConfig(void) {
	return getParamU16(L6482_CONFIG);
}
Status L6482::SetConfig(uint16_t value) {
	return setParamU16(L6482_CONFIG, value);
}
uint16_t L6482::GetStatus(void) {
	return getParamU16(L6482_STATUS);
}

Status L6482::Run(L6482_DIR_Typedef dir, uint32_t speed) {
	uint8_t cmd;
	uint8_t data[3];
	// Assigned command
	cmd = (uint8_t)(L6482_Run);
	cmd |= (uint8_t)(dir);
	data[2] = (uint8_t)((speed >> 16) & 0x0F);
	data[1] = (uint8_t)(speed >> 8);
	data[0] = (uint8_t)(speed);
	// Send Data out
	return sendOut(cmd, data, 3);

}
Status L6482::Move(L6482_DIR_Typedef dir, uint32_t step) {
	uint8_t cmd;
	uint8_t data[3];
	// Assigned command
	cmd = (uint8_t)(L6482_Move);
	cmd |= (uint8_t)(dir);
	data[2] = (uint8_t)((step >> 16) & 0x3F);
	data[1] = (uint8_t)(step >> 8);
	data[0] = (uint8_t)(step);
	// Send Data out
	return sendOut(cmd, data, 3);
}
Status L6482::GoTo(uint32_t abs_pos) {
	uint8_t cmd;
	uint8_t data[3];
	// Assigned command
	cmd = (uint8_t)(L6482_Goto);
	data[2] = (uint8_t)((abs_pos >> 16) & 0x3F);
	data[1] = (uint8_t)(abs_pos >> 8);
	data[0] = (uint8_t)(abs_pos);
	// Send Data out
	return sendOut(cmd, data, 3);
}
Status L6482::GoToDir(L6482_DIR_Typedef dir, uint32_t abs_pos) {
	uint8_t cmd;
	uint8_t data[3];
	// Assigned command
	cmd = (uint8_t)(L6482_GotoDIR);
	cmd |= (uint8_t)(dir);
	data[2] = (uint8_t)((abs_pos >> 16) & 0x3F);
	data[1] = (uint8_t)(abs_pos >> 8);
	data[0] = (uint8_t)(abs_pos);
	// Send Data out
	return sendOut(cmd, data, 3);
}
Status L6482::GoHome(void) {
	uint8_t cmd;
	// Assigned command
	cmd = (uint8_t) (L6482_Goto);
	// Send Data out
	return sendOut(cmd, data, 1);
}
Status L6482::GoMark(void) {
	uint8_t cmd;
	// Assigned command
	cmd = (uint8_t) (L6482_GoMask);
	// Send Data out
	return sendOut(cmd, data, 1);
}
Status L6482::ResetPos(void) {
	uint8_t cmd;
	// Assigned command
	cmd = (uint8_t) (L6482_ResetPos);
	// Send Data out
	return sendOut(cmd, data, 1);
}
Status L6482::ResetDevice(void) {
	uint8_t cmd;
	// Assigned command
	cmd = (uint8_t) (L6482_ResetDevice);
	// Send Data out
	return sendOut(cmd, data, 1);
}
Status L6482::SoftStop(void) {
	uint8_t cmd;
	// Assigned command
	cmd = (uint8_t) (L6482_SoftStop);
	// Send Data out
	return sendOut(cmd, data, 1);
}
Status L6482::HardStop(void) {
	uint8_t cmd;
	// Assigned command
	cmd = (uint8_t) (L6482_HardStop);
	// Send Data out
	return sendOut(cmd, data, 1);
}
Status L6482::SoftHiZ(void) {
	uint8_t cmd;
	// Assigned command
	cmd = (uint8_t) (L6482_SoftHiZ);
	// Send Data out
	return sendOut(cmd, data, 1);
}
Status L6482::HardHiZ(void) {
	uint8_t cmd;
	// Assigned command
	cmd = (uint8_t) (L6482_HardHiZ);
	// Send Data out
	return sendOut(cmd, data, 1);
}

Status L6482::setParamU8(L6482_REG_Typedef reg, uint8_t value) {
	uint8_t dataReturn;
	uint8_t cmd;
	// Assigned command
	cmd = (uint8_t)(L6482_SetParam);
	cmd |= reg;
	// Send Data out
	dataReturn = SSPSend(cmd);
	dataReturn = SSPSend(value);

	return SUCCESS;
}
Status L6482::setParamU16(L6482_REG_Typedef reg, uint8_t* value) {
	uint8_t dataReturn;
	uint8_t cmd;
	// Assigned command
	cmd = (uint8_t)(L6482_SetParam);
	cmd |= reg;
	// Send Data out
	dataReturn = SSPSend(cmd);
	dataReturn = SSPSend(value[0]);
	dataReturn = SSPSend(value[1]);

	return SUCCESS;
}
Status L6482::setParamU32(L6482_REG_Typedef reg, uint8_t* value) {
	uint8_t dataReturn;
	uint8_t cmd;
	// Assigned command
	cmd = (uint8_t)(L6482_SetParam);
	cmd |= reg;
	// Send Data out
	dataReturn = SSPSend(cmd);
	dataReturn = SSPSend(value[0]);
	dataReturn = SSPSend(value[1]);
	dataReturn = SSPSend(value[2]);

	return SUCCESS;
}

uint8_t L6482::getParamU8(L6482_REG_Typedef reg) {
	uint8_t dataReturn;
	uint8_t cmd;
	dataReturn = 0;

	// Assigned command
	cmd = (uint8_t)(L6482_GetParam);
	cmd |= reg;
	// Send get command
	dataReturn = SSPSend(cmd);
	// Read data out
	dataReturn = SSPSend(0x00);

	return (dataReturn);
}
uint16_t L6482::getParamU16(L6482_REG_Typedef reg) {
	uint16_t dataReturn;
	uint8_t dataHigh;
	uint8_t dataLow;

	// Assigned command
	cmd = (uint8_t)(L6482_GetParam);
	cmd |= reg;
	// Send get command
	dataReturn = SSPSend(cmd);
	// Read data out
	dataHigh = SSPSend(0x00);
	dataLow = SSPSend(0x00);

	dataReturn = (uint16_t)(dataHigh << 8) | (uint16_t)(dataLow);

	return (dataReturn);
}
uint32_t L6482::getParamU32(L6482_REG_Typedef reg) {
	uint32_t dataReturn;
	uint8_t dataByte2;
	uint8_t dataByte1;
	uint8_t	dataByte0;

	// Assigned command
	cmd = (uint8_t)(L6482_GetParam);
	cmd |= reg;
	// Send get command
	dataReturn = SSPSend(cmd);
	// Read data out
	dataByte2 = SSPSend(0x00);
	dataByte1 = SSPSend(0x00);
	dataByte0 = SSPSend(0x00);

	dataReturn = (uint32_t)(dataByte2 << 16)
			| (uint32_t)(dataByte1 << 8)
			| (uint32_t)(dataByte0);

	return (dataReturn);
}

Status L6482::sendOut(uint8_t cmd, uint8_t* data, uint8_t len) {
	dataReturn = SSPSend(cmd);
	while (len) {
		dataReturn = SSPSend(data[--len]);
	}
	return SUCCESS;
}

//================ END OF FILE ==============================================//

