//================ File Description =========================================//
//=== File name : tb6600.h
//===========================================================================//

#ifndef TB6600_H_
#define TB6600_H_

//================ Include Header ===========================================//
#include "string.h"
#include "stdint.h"
#include "lpc_types.h"
#include "lpc12xx_gpio.h"
#include "lpc_uart.h"

//================ PULBIC DEFINE ============================================//
#define M1_PIN		31
#define M2_PIN		0
#define M3_PIN		1
#define MRST_PIN	23
#define MCLK_PIN	21
#define MDIR_PIN	22
#define MEN_PIN		20
#define MTQ_PIN		3
#define MLA_PIN		4

#define MAX_STEP	4000
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	TB6600_TQ = 0,
	TB6600_LA,
	TB6600_M,
	TB6600_EN,
	TB6600_PSC
}TB6600SetFn;

typedef enum {
	TB6600_CW = 0,
	TB6600_CCW
}TB6600Dir;


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

class tb6600 {
public:
	tb6600();
	virtual ~tb6600();

	void Init(void);
	uint8_t Set(TB6600SetFn fn, uint8_t value);
	uint8_t Get(TB6600SetFn fn);
	uint32_t GetCurrentPosition(void);
	FunctionalState EnableMotor(FunctionalState state);
	FunctionalState ResetMotor(FunctionalState state);
	Status SetDir(TB6600Dir dir);
	Status Move(uint32_t step, TB6600Dir dir);
	void SetManualMode(FunctionalState state);
	void ForceActive(FunctionalState state);
	void Tick(void);
private :
	FunctionalState tq;
	FunctionalState la;
	FunctionalState en;
	uint8_t mfn;
	uint32_t currentStep;
	uint8_t prescale;
	uint8_t currentClock;
	uint32_t targetStep;
	TB6600Dir dir;
	uint8_t inProcess;
	uint8_t manualMode;
};

//================ END OF FILE ==============================================//
#endif /* TB6600_H_ */
