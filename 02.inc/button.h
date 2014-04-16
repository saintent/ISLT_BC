//================ File Description =========================================//
//=== File name : button.h
//===========================================================================//

#ifndef BUTTON_H_
#define BUTTON_H_

//================ Include Header ===========================================//
#include "stdint.h"
#include "lpc_types.h"
//================ PULBIC DEFINE ============================================//
#define BT_PORT		LPC_GPIO0
#define BT_U_LOC		3
#define BT_D_LOC		4
#define BT_L_LOC		5
#define BT_R_LOC		6
#define BT_SENTIVITY	50
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	BT_UP = 0,
	BT_DOWN,
	BT_LEFT,
	BT_RIGHT,
	BT_ALL
}BT_TYPE_T;

typedef enum {
	BT_STATE_IDLE = 0,
	BT_STATE_PUSH,
	BT_STATE_PRESS
	//BT_STATE_RELESE,

}BT_STATE_TYPE_T;
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
typedef void ButtonCallBack_t(BT_TYPE_T bt, BT_STATE_TYPE_T state);
//================ TYPEDEF STRUCT/UNION =====================================//
typedef struct {
	BT_STATE_TYPE_T state;
	uint8_t			decisionTime;
	uint8_t			rsv[2];
}BT_OBJ_TYPE_T;
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

class button {
public:
	button();
	virtual ~button();

	void Init(void);
	void Tick(void);
	void RegisterCallBack(ButtonCallBack_t *fn);


private :
	ButtonCallBack_t *cbFn;
	BT_OBJ_TYPE_T btState[BT_ALL];
	void checkStatus(BT_TYPE_T bt, uint8_t status);
};

//================ END OF FILE ==============================================//
#endif /* BUTTON_H_ */
