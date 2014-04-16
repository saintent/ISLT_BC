//================ File Description =========================================//
//=== File name : TempSensor.h
//===========================================================================//

#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

//================ Include Header ===========================================//
#include "lpc12xx.h"
#include "lpc12xx_adc.h"
#include "lpc12xx_iocon.h"
//================ PULBIC DEFINE ============================================//
#define ADC_ADINT		0x00010000
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	ADC_FREE = 0,
	ADC_ONREAD
}Adc_Status;

typedef enum {
	TEMP_ATTR_TEMP = 0,
}TEMP_ATTR_TYPE_T;
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

class TempSensor {
public:
	TempSensor();
	virtual ~TempSensor();
	static void ReadCallback(TempSensor* obj, uint16_t val);
	void Init(uint16_t* ptTable, uint8_t tTableSize);
	Adc_Status Read();
	uint16_t GetValue(void);
	uint8_t GetTemp(void);
private :
	void calTemp(void);
private :
	uint16_t* tempTable;
	uint8_t	 taempTableSize;
	Adc_Status onRead;
	uint16_t value;
	uint8_t	tempValue;

};

//================ END OF FILE ==============================================//
#endif /* TEMPSENSOR_H_ */
