//================ File Description =========================================//
//=== File name : ssp.h
//===========================================================================//


#ifndef SSP_H_
#define SSP_H_


//================ Include Header ===========================================//
//
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
extern "C"
{
#endif

void SSPInit(void);
uint8_t SSPSend(uint8_t data);
void SSPRecvBlockFIFO (uint8_t *pBuffer, uint32_t BufferLength);

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
#endif /* SSP_H_ */
