//================ File Desciption =========================================//
//=== File name : IAAP.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "IAAP.h"
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

IAAP::IAAP() {
	// TODO Auto-generated constructor stub

}

IAAP::~IAAP() {
	// TODO Auto-generated destructor stub
}

void IAAP::Init(Heater* pHeater, Valve* pValve, MotorControl* pMotor,
		TempSensor* pTempSensor) {
	this->prHeater = pHeater;
	this->prValve = pValve;
	this->prMotor = pMotor;
	this->prTempSensor = pTempSensor;
}

void IAAP::ProcessPrimitve(uint8_t* Data, uint8_t Size,
		uint8_t* Out, uint8_t* OutSize) {
	uint8_t cmd;
	prOut = Out;
	prOutSize = OutSize;
	cmd = Data[0];
	switch (cmd) {
	case CMD_ASSOCIATION :
		associateCMDProcess(&Data[1]);
		break;
	case CMD_READ :
		readCMDProcess(&Data[1]);
		break;
	case CMD_WRITE :
		writeCMDProcess(&Data[1]);
		break;
	case CMD_ACTION :
		actionCMDProcess(&Data[1]);
	default :
		break;
	}
}

void IAAP::associateCMDProcess(uint8_t* Data) {
	ASSOCIATION_TYPE_T association;
	association = (ASSOCIATION_TYPE_T) Data[0];
	switch (association) {
	case ASS_LOGIN:
		break;
	case ASS_DIS:
		break;
	default:
		break;
	}
}

void IAAP::readCMDProcess(uint8_t* Data) {
	uint8_t attr;
	uint8_t* dOut;
	uint8_t dOutSize;
	uint8_t* pdOutSize;
	uint8_t u8Data;
	uint16_t u16Data;
	uint32_t u32Data;
	IAAR_REGISTER_T reg;
	reg = (IAAR_REGISTER_T) Data[0];
	attr = Data[1];
	dOut = this->prOut;
	pdOutSize = this->prOutSize;
	dOut[0] = reg;
	dOut[1] = attr;
	switch (reg) {
	case REG_MOTOR:
		switch (attr) {
		case MOTOR_ATTR_MPS :
			//u16Data = prMotor->GetMPS();
			dOut[3] = (uint8_t) (u16Data >> 8);
			dOut[4] = (uint8_t) u16Data;
			dOutSize = 2;
			break;
		case MOTOR_ATTR_LV :
			prMotor->GetMoveLevel(&dOut[3], &dOutSize);
			break;
		case MOTOR_ATTR_SPD :
			u16Data = prMotor->GetSpeed();
			dOut[3] = (uint8_t) (u16Data >> 8);
			dOut[4] = (uint8_t) u16Data;
			dOutSize = 2;
			break;
		default :
			dOutSize = 0;
			break;
		}
		dOut[2] = dOutSize;
		pdOutSize[0] = dOutSize + 3;
		break;
	case REG_VALVE:
		switch (attr) {
		case VALVE_ATTR_DIR :
			dOut[3] = prValve->GetStatus();
			dOutSize = 1;
			break;
		default:
			dOutSize = 0;
			break;
		}
		dOut[2] = dOutSize;
		pdOutSize[0] = dOutSize + 3;
		break;
	case REG_HEATER:
		switch (attr) {
		case HEATER_ATTR_STATUS :
			dOut[3] = prHeater->Relay_GetSta();
			dOutSize = 1;
			break;
		default :
			dOutSize = 0;
			break;
		}
		dOut[2] = dOutSize;
		pdOutSize[0] = dOutSize + 3;
		break;
	case REG_TEMPSENSOR:
		switch (attr) {
		case TEMP_ATTR_TEMP:
			dOut[3] = prTempSensor->GetTemp();
			dOutSize = 1;
			break;
		default:
			dOutSize = 0;
			break;
		}
		dOut[2] = dOutSize;
		pdOutSize[0] = dOutSize + 3;
		break;
	default:
		break;
	}
}

void IAAP::writeCMDProcess(uint8_t* Data) {

	IAAR_REGISTER_T reg;
	reg = (IAAR_REGISTER_T) Data[0];
	switch (reg) {
	case REG_MOTOR:
		break;
	case REG_VALVE:
		break;
	case REG_HEATER:
		break;
	case REG_TEMPSENSOR:
		break;
	default:
		break;
	}
}

void IAAP::actionCMDProcess(uint8_t* Data) {
	uint8_t u8Data;
	uint16_t u16Data;
	uint32_t u32Data;
	IAAR_REGISTER_T reg;
	MOTOR_ACT_TYPE_T mActType;
	VALVE_ACT_TYPE_T vActType;
	HEATER_ACT_TYPE_T hActType;
	//MOVE_DIR_T mMoveDir;
	VALVE_DIR vDir;
	RELAY_ST hStatus;
	reg = (IAAR_REGISTER_T) Data[0];
	switch (reg) {
	case REG_MOTOR:
		mActType = (MOTOR_ACT_TYPE_T) Data[1];
		switch (mActType) {
		case MOTOR_ACT_MTS :
			//mMoveDir = (MOVE_DIR_T) Data[2];
			u32Data = (uint32_t)Data[3] << 24
					| (uint32_t)Data[4] << 16
					| (uint32_t)Data[5] << 8
					| (uint32_t)Data[6];
			//prMotor->MoveToStep(mMoveDir, u32Data);
			this->genResponse(reg, SUCCESS);
			break;
		case MOTOR_ACT_MTL :
			u8Data = Data[2];
			if (u8Data <= LEVEL_3) {
				prMotor->MoveToLevel((MOVE_LEVEL_T) u8Data);
				this->genResponse(reg, SUCCESS);
			}
			else {
				this->genResponse(reg, ERROR);
			}
			break;
		case MOTOR_ACT_SETLV :
			u8Data = Data[2];
			u16Data = (uint16_t) Data[3] << 8
					| (uint16_t) Data[4];
			if (u8Data <= LEVEL_3) {
				prMotor->SetLevel((MOVE_LEVEL_T) u8Data, u16Data);
				this->genResponse(reg, SUCCESS);
			}
			else {
				this->genResponse(reg, ERROR);
			}
			break;
		default :
			// Exception command
			this->genResponse(reg, ERROR);
			break;
		}
		break;
	case REG_VALVE:
		vActType = (VALVE_ACT_TYPE_T) Data[1];
		switch (vActType) {
		case VALVE_ACT_CTR :
			vDir = (VALVE_DIR) Data[2];
			if ((vDir == VALVE_LEFT) || (vDir == VALVE_RIGHT)) {
				prValve->Valve_Control(vDir);
				this->genResponse(reg, SUCCESS);
			}
			else {
				this->genResponse(reg, ERROR);
			}
			break;
		default :
			// Exception command
			this->genResponse(reg, ERROR);
			break;
		}
		break;
	case REG_HEATER:
		hActType = (HEATER_ACT_TYPE_T) Data[1];
		switch (hActType) {
		case HEATER_ACT_CTR :
			hStatus = (RELAY_ST) Data[2];
			if ((hStatus == RELAY_OFF) || (hStatus == RELAY_ON)) {
				prHeater->Relay_Control(hStatus);
				this->genResponse(reg, SUCCESS);
			}
			else {
				this->genResponse(reg, ERROR);
			}
			break;
		default :
			// Exception command
			this->genResponse(reg, ERROR);
			break;
		}
		break;
	case REG_TEMPSENSOR:
		// Exception command
		this->genResponse(reg, ERROR);
		break;
	default:
		break;
	}
}

void IAAP::genResponse(uint8_t reg, Status result) {
	prOut[0] = reg;
	prOut[1] = result;
	prOutSize[0] = 2;
}
//================ END OF FILE ==============================================//

