//================ File Description =========================================//
//=== File name : Heater.h
//===========================================================================//

#ifndef HEATER_H_
#define HEATER_H_

//================ Include Header ===========================================//
#include "Relay.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	HEATER_ATTR_STATUS = 0
}HEATER_ATTR_TYPE_T;

typedef enum {
	HEATER_ACT_CTR_0 = 0,
	HEATER_ACT_CTR_1,
	HEATER_ACT_CTR_2
}HEATER_ACT_TYPE_T;
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

class Heater: public Relay {
public:
	Heater(void);
	Heater(RELAY_PORT port);
	virtual ~Heater();
};

//================ END OF FILE ==============================================//
#endif /* HEATER_H_ */
