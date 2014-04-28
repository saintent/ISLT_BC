//================ File Description =========================================//
//=== File name : l6482.h
//===========================================================================//


#ifndef L6482_TYPE_H_
#define L6482_TYPE_H_


//================ Include Header ===========================================//
#include "stdint.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	L6482_ABS_POS = 0x01,
	L6482_EL_POS,
	L6482_MARK,
	L6482_SPEED,
	L6482_ACC,
	L6482_DEC,
	L6482_MAX_SPEED,
	L6482_MIN_SPEED,
	L6482_TVAL_HOLD,
	L6482_TVAL_RUN,
	L6482_TVAL_ACC,
	L6482_TVAL_DEC,
	L6482_T_FAST = 0x0E,
	L6482_TON_MIN,
	L6482_TOFF_MIN,
	L6482_ADC_OUT = 0x12,
	L6482_OCD_TH,
	L6482_FS_SPD = 0x15,
	L6482_STEP_MODE,
	L6482_ALARM_EN,
	L6482_GATECFG1,
	L6482_GATECFG2,
	L6482_CONFIG,
	L6482_STATUS
}L6482_REG_Typedef;

typedef enum {
	L6482_SetParam = 0x00,
	L6482_GetParam = 0x20,
	L6482_Run = 0x50,
	L6482_StepClock = 0x58,
	L6482_Move = 0x40,
	L6482_Goto = 0x60,
	L6482_GotoDIR = 0x68,
	L6482_GoUntil = 0x82,
	L6482_ReleaseSW = 0x92,
	L6482_GoHome = 0x70,
	L6482_GoMask = 0x78,
	L6482_ResetPos = 0xD7,
	L6482_ResetDevice = 0xC0,
	L6482_SoftStop = 0xB0,
	L6482_HardStop = 0xB8,
	L6482_SoftHiZ = 0xA0,
	L6482_HardHiZ = 0xA8,
	L6482_GetStatus = 0xD0
}L6482_CMD_Typedef;

typedef enum {
	MOVE_RW	= 0,	// Move Motor Reverse
	MOVE_FF			// Move Motor Forward
}L6482_DIR_Typedef;
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
typedef union {
	uint16_t	Word;
	struct {
		uint16_t	OSC_SEL:3;
		uint16_t	EXT_CLK:1;
		uint16_t	SW_MODE:1;
		uint16_t	EN_TQREG:1;
		uint16_t	rsv:1;
		uint16_t	OC_SD:1;
		uint16_t	UVLOVAL:1;
		uint16_t	VCCVAL:1;
		uint16_t	TSW:5;
		uint16_t	PRED_E:1;
	}Bit;
}L6482_CONFIG_Reg;

typedef union {
	uint16_t	Word;
	struct {
		uint16_t	HiZ:1;
		uint16_t	BUSY:1;
		uint16_t	SW_F:1;
		uint16_t 	SW_EVN:1;
		uint16_t	DIR:1;
		uint16_t 	MOT_STATUS:2;
		uint16_t	NOTPERF_CMD:2;
		uint16_t	STCK_MOD:1;
		uint16_t	UVLO:1;
		uint16_t	UVLV_ADC:1;
		uint16_t	TH_SD:2;
		uint16_t	OCD:1;
		uint16_t	rsv:2;
	}Bit;
}L6482_STATUS_Reg;

typedef struct {
	uint32_t	CurrentPos;
	uint32_t	MarkPos;
	uint32_t	CurrentSpeed;
	uint16_t	Acc;
	uint16_t	Dec;
	uint16_t	MaxSpeed;
	uint16_t	Minspeed;
	uint16_t	FullStepSpeed;
	uint16_t	ElecticalPos;
	uint8_t		HoldingRefVolt;
	uint8_t		ConstanctRefVolt;
	uint8_t		AccStartRefVolt;
	uint8_t		DecStartRefVolt;
	uint8_t		FasDecaySetting;
	uint8_t		MinOnTime;
	uint8_t		MinOffTime;
	uint8_t		Adc_Out;
	uint8_t		ODCThreshole;
	uint8_t 	StepMode;
	uint8_t		AlarmEnable;
	uint16_t	GateDriverConfig1;
	uint16_t	GateDriverConfig2;
	L6482_CONFIG_Reg	Config;
	L6482_STATUS_Reg	Status;
}L6482_REG;

//================ EXTERN FUNCTION ==========================================//
#ifdef __cplusplus
extern "C"
{
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
//================ END OF FILE ==============================================//
#endif /* L6482_TYPE_H_ */
