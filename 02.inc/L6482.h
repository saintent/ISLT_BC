//================ File Description =========================================//
//=== File name : L6482.h
//===========================================================================//

#ifndef L6482_H_
#define L6482_H_

//================ Include Header ===========================================//
#include "stdint.h"
#include "l6482_type.h"
#include "lpc_types.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
//
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
//
//================ EXTERN FUNCTION ==========================================//
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
//================ EXTERN FUNCTION POINTER ==================================//
//
//================ EXTERN VARIABLE ==========================================//
//
//================ EXTERN QUEUE =============================================//
//
//================ CLASS DECLARATION ========================================//

class L6482 {
public:
	L6482();
	virtual ~L6482();

	uint32_t 	GetCurrentPosition(void);
	uint16_t	GetElecPosition(void);
	uint32_t	GetMarkPosition(void);
	uint32_t	GetSpeed(void);
	uint16_t	GetAcceleration(void);
	uint16_t	GetDeceleration(void);
	uint16_t	GetMaxSpeed(void);
	Status		SetMaxSpeed(uint16_t max);
	uint16_t	GetMinSpeed(void);
	Status		SetMinSpeed(uint16_t min);
	uint16_t	GetFullStepSpeed(void);
	uint8_t		GetHoldingRefVolt(void);
	Status		SetHoldingRefVolt(uint8_t volt);
	uint8_t		GetConstanceRefVolt(void);
	Status 		SetConstanceRefVolt(uint8_t volt);
	uint8_t		GetAccStartRefVolt(void);
	Status		SetAccStartRefVolt(uint8_t volt);
	uint8_t		GetDccStartRefVolt(void);
	Status  	SetDccStartRefVolt(uint8_t volt);
	uint8_t		GetFastDecaySettting(void);
	Status		SetFastDecaySetting(uint8_t volt);
	uint8_t		GetMinOnTime(void);
	Status		SetMinOnTime(uint8_t time);
	uint8_t		GetMinOffTime(void);
	Status		SetMinOffTime(uint8_t time);
	uint8_t		GetADCOut(void);
	uint8_t 	GetOCDThreshold(void);
	Status		SetOCDThreshold(uint8_t value);
	uint8_t		GetStepMode(void);
	Status		SetStepMode(uint8_t value);
	uint8_t		GetAlarmSetting(void);
	Status		SetAlarmSetting(uint8_t value);
	uint16_t	GetGateDriveConfig(uint8_t gate);
	Status		SetGateDriveConfig(uint8_t gate, uint16_t value);
	uint16_t	GetConfig(void);
	Status		SetConfig(uint16_t value);
	uint16_t	GetStatus(void);

	Status		Run(L6482_DIR_Typedef dir, uint32_t speed);
	Status		Move(L6482_DIR_Typedef dir, uint32_t step);
	Status 		GoTo(uint32_t abs_pos);
	Status		GoToDir(L6482_DIR_Typedef dir, uint32_t abs_pos);
	Status		GoHome(void);
	Status		GoMark(void);
	Status		ResetPos(void);
	Status		ResetDevice(void);
	Status		SoftStop(void);
	Status		HardStop(void);
	Status		SoftHiZ(void);
	Status		HardHiZ(void);


private :
	Status		setParamU8(L6482_REG_Typedef reg, uint8_t value);
	Status		setParamU16(L6482_REG_Typedef reg, uint8_t* value);
	Status		setParamU32(L6482_REG_Typedef reg, uint8_t* value);
	uint8_t		getParamU8(L6482_REG_Typedef reg);
	uint16_t	getParamU16(L6482_REG_Typedef reg);
	uint32_t	getParamU32(L6482_REG_Typedef reg);
	Status		sendOut(uint8_t cmd, uint8_t* data, uint8_t len);

};

//================ END OF FILE ==============================================//
#endif /* L6482_H_ */
