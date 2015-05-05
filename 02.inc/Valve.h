//================ File Description =========================================//
//=== File name : Valve.h
//===========================================================================//

#ifndef VALVE_H_
#define VALVE_H_

//================ Include Header ===========================================//
#include "lpc_types.h"
#include "Relay.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum Valve_dir {
	VALVE_LEFT = 0,
	VALVE_RIGHT,
	VALVE_ALL
}VALVE_DIR;

typedef enum {
	VALVE_ATTR_DIR = 0,
}VALVE_ATTR_TYPE_T;

typedef enum {
	VALVE_ACT_CTR = 0,
}VALVE_ACT_TYPE_T;
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

class Valve {
public:
	Valve(void);
	Valve(RELAY_PORT left, RELAY_PORT right);
	virtual ~Valve();

	void Init(RELAY_PORT left, RELAY_PORT right);
	VALVE_DIR Valve_Control(VALVE_DIR dir);
	VALVE_DIR GetStatus(void);
	//void Valve_Enable();
	void Valve_Disable();
private :
	Relay 		vLeft;
	Relay 		vRight;
	VALVE_DIR	vDir;
};

//================ END OF FILE ==============================================//
#endif /* VALVE_H_ */
