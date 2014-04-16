//================ File Description =========================================//
//=== File name : Relay.h
//===========================================================================//

#ifndef RELAY_H_
#define RELAY_H_

//================ Include Header ===========================================//
#include "lpc_types.h"
#include "lpc12xx_gpio.h"
#include "gpiodef.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum Relay_st {
	RELAY_OFF = 0,
	RELAY_ON
}RELAY_ST;
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
typedef struct Relay_port {
	LPC_GPIO_TypeDef * 	loc;
	GPIO_PIN			pin;
}RELAY_PORT;

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

class Relay {
public:
	Relay(void);
	Relay(RELAY_PORT port);
	virtual ~Relay();
	void Init(RELAY_PORT port);
	RELAY_ST Relay_Control(RELAY_ST State);
	RELAY_ST Relay_GetSta(void);

protected :
	RELAY_PORT 	port;
	RELAY_ST	status;
};

//================ END OF FILE ==============================================//
#endif /* RELAY_H_ */
