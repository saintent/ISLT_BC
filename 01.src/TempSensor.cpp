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

	tempSlot = 0;
}

Adc_Status TempSensor::Read(void) {
	//uint32_t regVal;
	uint32_t raw;
	//FlagStatus flagStatus;
	if(!this->onRead) {
		ADC_StartCmd(ADC_START_NOW);
		this->onRead = ADC_ONREAD;
	}
	return this->onRead;
	/*ADC_StartCmd(ADC_START_NOW);

	//flagStatus = ADC_GetStatus(ADC_CHANNEL_0, ADC_FLAG_DONE);
	while (ADC_GetStatus(ADC_CHANNEL_0, ADC_FLAG_DONE)) {
		//flagStatus = ADC_GetStatus(ADC_CHANNEL_0, ADC_FLAG_DONE);
		//regVal = LPC_ADC->DR0;
		if (regVal & ADC_FLAG_DONE) {
			raw = ADC_GetData(ADC_CHANNEL_0);
			break;
		}
	}
	raw = ADC_GetData(ADC_CHANNEL_0);
	this->value = raw;
	calTemp(raw);*/
	//return ADC_FREE;
}

uint16_t TempSensor::GetValue(void) {
	return this->value;
}

uint8_t TempSensor::GetTemp(void) {
	return this->tempValue;
}

void TempSensor::ReadCallback(TempSensor* obj, uint16_t val) {
	obj->value = (uint16_t) val;
	obj->calTemp(val);
	obj->onRead = ADC_FREE;
}

void TempSensor::calTemp(uint16_t val) {
	uint8_t gotTemp;
	uint8_t tIndex;
	uint16_t tsum;
	gotTemp = 1;
	tIndex = 0;
	value = val;
	onRead = ADC_FREE;
	while (gotTemp) {
		if (this->value < tempTable[tIndex]) {
			tempValueMove[tempSlot] = tIndex;
			if(++tempSlot >= 32) {
				tempSlot = 0;
			}
			gotTemp = 0;
		}
		tIndex++;
	}
	calAverage();
	/*tsum = tempValueMove[0] + tempValueMove[1] + tempValueMove[2]
			+ tempValueMove[3] + tempValueMove[4] + tempValueMove[5]
			+ tempValueMove[6] + tempValueMove[7] + tempValueMove[8]
			+ tempValueMove[9];
	tempValue = tsum/10;*/
}

void TempSensor::calAverage(void) {
	uint16_t sum;
	uint8_t i;
	i = 0;
	sum = 0;
	for ( i = 0; i < 32; i++) {
		sum += tempValueMove[i];
	}
	tempValue = sum/32;
}
//================ END OF FILE ==============================================//

