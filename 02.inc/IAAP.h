//================ File Description =========================================//
//=== File name : IAAP.h
//===========================================================================//

#ifndef IAAP_H_
#define IAAP_H_

//================ Include Header ===========================================//
#include "lpc_types.h"
#include "Heater.h"
#include "Valve.h"
#include "MotorControl.h"
#include "TempSensor.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	CMD_ASSOCIATION = 0x01,
	CMD_READ = 0x03,
	CMD_WRITE = 0x07,
	CMD_ACTION = 0x10
}CMD_TYPE_T;

typedef enum {
	ASS_LOGIN = 0x01,
	ASS_DIS = 0x02
}ASSOCIATION_TYPE_T;

typedef enum {
	REG_MOTOR = 0x00,
	REG_VALVE,
	REG_HEATER,
	REG_TEMPSENSOR
}IAAR_REGISTER_T;

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

class IAAP {
public:

public:
	IAAP();
	virtual ~IAAP();
	void Init(Heater* pHeater,
			Valve* pValve,
			MotorControl* pMotor,
			TempSensor* pTempSensor);
	void ProcessPrimitve(uint8_t* Data, uint8_t Size,
			uint8_t* Out, uint8_t* OutSize);
private :
	Heater* 		prHeater;
	Valve*			prValve;
	MotorControl* 	prMotor;
	TempSensor*		prTempSensor;
	uint8_t*		prOut;
	uint8_t*		prOutSize;
private :
	void associateCMDProcess(uint8_t* Data);
	void readCMDProcess(uint8_t* Data);
	void writeCMDProcess(uint8_t* Data);
	void actionCMDProcess(uint8_t* Data);
	void genResponse(uint8_t reg, Status result);
};

//================ END OF FILE ==============================================//
#endif /* IAAP_H_ */
