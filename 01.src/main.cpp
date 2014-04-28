/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC12xx.h"
#endif

#include <cr_section_macros.h>
#include "button.h"
#include "heater.h"
#include "valve.h"
#include "comport.h"
#include "relay.h"
#include "MotorControl.h"
#include "TempSensor.h"
#include "TTable.h"
#include "IAAP.h"
#include "lpc12xx_timer32.h"
#include "ssp.h"

// TODO: insert other include files here
//extern void SysTick_Handler (void);
//extern void ADC_IRQHandler (void);
// TODO: insert other definitions and declarations here
struct {
	uint8_t Tick10ms;
	uint8_t Tick100ms;
	uint8_t Tick1sec;
	uint8_t rsv;
}TickTime;
struct {
	uint8_t ms1Tick;
	uint8_t ms10Tick;
	uint8_t ms100Tick;
	uint8_t secTick;
	uint8_t rsv[2];
}ObjectTick;

// Object in program
button			pBt;
Heater 			pHeater;
Valve			pValve;
Comport			pRS485;
Comport 		pZB;
MotorControl 	pMotor;
TempSensor 		pTempSensor;
IAAP			pIAAP;

void phyRS485DataIn_Callback(Uart_type port, uint8_t data) {
	pRS485.PhyCallback(data);
}

void phyZBDataIn_Callback(Uart_type port, uint8_t data) {
	pZB.PhyCallback(data);
}

void phyRS485SendCmp_Callback(Uart_type port) {
	pRS485.SentByte();
}

void phyZBSendCmp_Callback(Uart_type port) {
	pZB.SentByte();
}

void button_CallBack(BT_TYPE_T type, BT_STATE_TYPE_T state) {
	switch (type) {
	case BT_UP :
		if (state == BT_STATE_PUSH) {
			//pMotor.MoveToStep()
			//pMotor.ForceMove(MOVE_FF);
		}
		if (state == BT_STATE_PRESS) {
			//pMotor.ForceMove(MOVE_STOP);
		}
		break;
	case BT_DOWN :
		if (state == BT_STATE_PUSH) {
			//pMotor.MoveToStep()
			//pMotor.ForceMove(MOVE_RW);
		}
		else if (state == BT_STATE_PRESS) {
			//pMotor.ForceMove(MOVE_STOP);
		}
		break;
	case BT_RIGHT :
		if (state == BT_STATE_PRESS) {
			pValve.Valve_Control(VALVE_RIGHT);
		}
		break;
	case BT_LEFT :
		if (state == BT_STATE_PRESS) {
			pValve.Valve_Control(VALVE_LEFT);
		}
		break;
	default :
		break;
	}
}

void Init(void) {
	RELAY_PORT hPort = { LPC_GPIO0, GPIO_PIN_19 };
	RELAY_PORT rVP = { LPC_GPIO1, GPIO_PIN_5 };
	RELAY_PORT lVP = { LPC_GPIO1, GPIO_PIN_6 };

	UARTInit(RS485_PORT, 9600, RS485_LOC);
	UART_RS485Init();
	//UARTInit(ZB_PORT, 115200, ZB_LOC);
	// Set Callback response
	//UARTRegDataCb(RS485_PORT, phyRS485DataIn_Callback);
	//UARTRegDataCb(ZB_PORT, phyZBDataIn_Callback);
	//UARTRegSendCmp(RS485_PORT, phyRS485SendCmp_Callback);
	//UARTRegSendCmp(ZB_PORT, phyZBSendCmp_Callback);


	// Heater Initialized hardware
	pHeater.Init(hPort);
	pHeater.Relay_Control(RELAY_ON);
	// Valve initialized hardware
	pValve.Init(lVP, rVP);
	pValve.Valve_Control(VALVE_LEFT);
	// Motor initialized hardware
	pMotor.Init();
	// Temperature initialized hardware
	pTempSensor.Init(tTable, 101);
	//
	pIAAP.Init(&pHeater, &pValve, &pMotor, &pTempSensor);
	pRS485.Init(&pIAAP);
	pRS485.SetAddress(0x01);

	pBt.Init();
	pBt.RegisterCallBack(button_CallBack);


}

int main(void) {
	//RELAY_PORT rPort;
    // TODO: insert code here
	SystemCoreClockUpdate();
	// Set SysTick as 1 ms
	SysTick_Config( SystemCoreClock / (1000 - 1));
	// Initialized parameter

    // Initialized object
	Init();
	//pTempSensor.Read();

    // Enter an infinite loop, just incrementing a counter
    while(1) {
        if (ObjectTick.ms1Tick) {
        	//pMotor.Tick();
        	pBt.Tick();
        	ObjectTick.ms1Tick = FALSE;
        }
        if (ObjectTick.ms10Tick) {

        	ObjectTick.ms10Tick = FALSE;
        }

        if (ObjectTick.ms100Tick) {

        	ObjectTick.ms100Tick = FALSE;
        }

        if (ObjectTick.secTick) {

        	ObjectTick.secTick = FALSE;
        }

        if (pRS485.PendingProcess()) {
        	pRS485.Interactive();
        }
    }
    return 0 ;
}

void SysTick_Handler(void) {
	// 1 ms Routine
	ObjectTick.ms1Tick = TRUE;
	if (++TickTime.Tick10ms >= 10) {
		// 10 ms Routine
		ObjectTick.ms10Tick = TRUE;
		if (++TickTime.Tick100ms >= 10) {
			// 100 ms Routing
			//ObjectTick.ms100Tick = TRUE;
			if (++TickTime.Tick1sec >= 10) {
				// 1 sec Routing
				ObjectTick.secTick = TRUE;
				//UARTSendCh(RS485_PORT, 'U');
				TickTime.Tick1sec = 0;
			}
			TickTime.Tick100ms = 0;
		}
		TickTime.Tick10ms = 0;
	}
}

void ADC_IRQHandler (void) {
	uint32_t reg;
	uint16_t adcVal;
	reg = LPC_ADC->GDR;		//Get Status
	reg = LPC_ADC->STAT;
	// Interrupt flag
	if(reg & ADC_ADINT) {
		// Channel 0
		adcVal = ADC_GetData(ADC_CHANNEL_0);
		TempSensor::ReadCallback(&pTempSensor, adcVal);
	}
}


