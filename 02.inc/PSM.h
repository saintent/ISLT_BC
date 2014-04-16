//================ File Description =========================================//
//=== File name : PSM.h
//===========================================================================//


#ifndef PSM_H_
#define PSM_H_


//================ Include Header ===========================================//
#include "lpc_types.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
/**
 * @brief Zigbee Command
 */
typedef enum ZB_CMD_tag {
	ZB_CMD_RESET = 0
}ZB_CMD;
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
/**
 * @brief Zigbee state
 */
typedef struct ZB_tag {
	Bool isUp;			//< Set true is module is ready
	Bool haveBuffer;
	Bool haveConfirm;
	Bool haveLink;

	ZB_CMD command;

	UNS_16 PANID;
	UNS_16 DeviceID;

	UNS_8 QOS;
}ZB_PSM, *P_ZB_PSM;
/**
 * @brief Program state machine
 */
typedef struct PSM_tag {
	//Pointer to zigbee state
	P_ZB_PSM	pZigbee;
};
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
#endif /* PSM_H_ */
