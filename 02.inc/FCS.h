//================ File Description =========================================//
//=== File name : FCS.h
//===========================================================================//

#ifndef FCS_H_
#define FCS_H_

//================ Include Header ===========================================//
#include "stdint.h"
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

class FCS {
public:
	FCS();
	virtual ~FCS();
	static uint8_t	FcsCheck(uint8_t* u8In, uint8_t lenght);
	static void	FcsRun(uint8_t* u8In, uint8_t lenght, uint16_t* fcsOut);
private :
	static uint16_t fcstab[256];
};
//================ END OF FILE ==============================================//
#endif /* FCS_H_ */
