//================ File Desciption =========================================//
//=== File name : TempSensor.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "TempSensor.h"
#include "lpc12xx_adc.h"
//================ PUBLIC METHOD ============================================//
// extern function from another file
//
//================ PRIVATE METHOD ===========================================//
// non extern function
//
//================ PUBLIC DATA ==============================================//
// extern data
//
//================ PRIVATE DATA =============================================//
// non extern data
//
//================ PRIVATE DEFINE ===========================================//
//
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//

TempSensor::TempSensor() {
	// TODO Auto-generated constructor stub

}

TempSensor::~TempSensor() {
	// TODO Auto-generated destructor stub
}


void TempSensor::Init(uint16_t* ptTable, uint8_t tTableSize) {
	this->tempTable = ptTable;
	this->taempTableSize = tTableSize;
	// Config pin function
	IOCON_PIO_CFG_Type adio;
	IOCON_StructInit(&adio);
	adio.type = IOCON_ADC_AD0;
	adio.pinmode = IOCON_PIO_MODE_INACTIVE;
	adio.ad = IOCON_PIO_AD_ANALOG;
	// ADC channel select function
	IOCON_SetFunc(&adio);
	// ADC run as 1 MHz
	ADC_Init(1000000UL);
	// ADC Channel 0
	ADC_ChannelCmd(ADC_CHANNEL_0, ENABLE);

	ADC_IntConfig(ADC_CHANNEL_0, ENABLE);
	// Enable ADC interrupt
	NVIC_EnableIRQ(ADC_IRQn);
}

Adc_Status TempSensor::Read(void) {
	if(!this->onRead) {
		ADC_StartCmd(ADC_START_NOW);
		this->onRead = ADC_ONREAD;
	}
	return this->onRead;
}

uint16_t TempSensor::GetValue(void) {
	return this->value;
}

uint8_t TempSensor::GetTemp(void) {
	return this->tempValue;
}

void TempSensor::ReadCallback(TempSensor* obj, uint16_t val) {
	obj->value = (uint16_t) val;
	obj->calTemp();
	obj->onRead = ADC_FREE;
}

void TempSensor::calTemp(void) {
	uint8_t gotTemp;
	uint8_t tIndex;
	gotTemp = 1;
	tIndex = 0;
	while (gotTemp) {
		if (this->value < tempTable[tIndex]) {
			tempValue = tIndex;
			gotTemp = 0;
		}
		tIndex++;
	}
}
//================ END OF FILE ==============================================//

