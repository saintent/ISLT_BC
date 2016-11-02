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
#include "tb6600.h"
#include "lpc12xx_uart.h"
#include "DMXSlave.h"

// TODO: insert other include files here
//extern void SysTick_Handler (void);
//extern void ADC_IRQHandler (void);
// TODO: insert other definitions and declarations here
struct {
	uint8_t Tick1ms;
	uint8_t Tick10ms;
	uint8_t Tick100ms;
	uint8_t Tick1sec;
	uint8_t rsv;
}TickTime;
struct {
	uint8_t us10Tick;
	uint8_t ms1Tick;
	uint8_t ms10Tick;
	uint8_t ms100Tick;
	uint8_t secTick;
	uint8_t rsv[2];
}ObjectTick;

void DMXFrameReceivedComplete(uint16_t ch);

// Object in program
button			pBt;
Heater 			pH1;
Heater			pH2;
Heater 			pH3;
Valve			pValve;
Valve			pValve2;
//Comport			pRS485;
Comport 		pZB;
//MotorControl 	pMotor;
TempSensor 		pTempSensor;
IAAP			pIAAP;
tb6600			pTB6600;
dmx::DMXSlave	dmxSlave(1, 8, DMXFrameReceivedComplete);

uint8_t 		isButtonPress;

void DMXFrameReceivedComplete(uint16_t ch) {

}

void DMXDataRecv_CallBack(Uart_type port, uint8_t data) {
	dmx::DMXSlave::OnRecviceFrame(&dmxSlave, uart::eUartRecvData, data);
}


void DMXBreakInt_CallBack(Uart_type port) {
	dmx::DMXSlave::OnRecviceFrame(&dmxSlave, uart::eUartBreakInt, 0);
}

void phyZBDataIn_Callback(Uart_type port, uint8_t data) {
	pZB.PhyCallback(data);
}

void phyZBSendCmp_Callback(Uart_type port) {
	pZB.SentByte();
}

void button_CallBack(BT_TYPE_T type, BT_STATE_TYPE_T state) {
	Bool isBusy;
	//isBusy = pMotor.IsBusy();
	switch (type) {
	case BT_UP :
		if (state == BT_STATE_PUSH) {
			//pMotor.MoveToStep()
			//pMotor.ForceMove(MOVE_FF);
			isButtonPress = TRUE;
			pTB6600.SetDir(TB6600_CW);
			pTB6600.SetManualMode(ENABLE);
			pTB6600.ForceActive(ENABLE);

		}
		if (state == BT_STATE_PRESS) {
			//pMotor.ForceMove(MOVE_STOP);
			isButtonPress = FALSE;
			pTB6600.SetManualMode(DISABLE);
			pTB6600.ForceActive(DISABLE);
/*			if(isBusy == FALSE) {
				pMotor.MoveToStep(MOVE_FF, 3200*5);
			}*/
		}
		break;
	case BT_DOWN :
		if (state == BT_STATE_PUSH) {
			//pMotor.MoveToStep()
			//pMotor.ForceMove(MOVE_RW);
			pTB6600.SetDir(TB6600_CCW);
			pTB6600.SetManualMode(ENABLE);
			pTB6600.ForceActive(ENABLE);
			isButtonPress = TRUE;
		}
		else if (state == BT_STATE_PRESS) {
			//pMotor.ForceMove(MOVE_STOP);
			isButtonPress = FALSE;
			pTB6600.SetManualMode(DISABLE);
			pTB6600.ForceActive(DISABLE);
/*			if(isBusy == FALSE) {
				pMotor.MoveToStep(MOVE_RW, 3200*5);
			}*/
		}
		break;
	case BT_RIGHT :
		if (state == BT_STATE_PRESS) {
			//pValve.Valve_Control(VALVE_RIGHT);
			//pHeater.Relay_Control(RELAY_OFF);
			if(pH1.Relay_GetSta() == RELAY_ON) {
				pH1.Relay_Control(RELAY_OFF);
			}
			else {
				pH1.Relay_Control(RELAY_ON);
			}
		}
		break;
	case BT_LEFT :
		if (state == BT_STATE_PRESS) {
			//pValve.Valve_Control(VALVE_LEFT);
			//pHeater.Relay_Control(RELAY_ON);
			if(pH2.Relay_GetSta() == RELAY_ON) {
				pH2.Relay_Control(RELAY_OFF);
				pH3.Relay_Control(RELAY_OFF);
			}
			else {
				pH2.Relay_Control(RELAY_ON);
				pH3.Relay_Control(RELAY_ON);
			}
		}
		break;
	default :
		break;
	}
}

void Init(void) {
	Heater* hArry[3];
	IOCON_PIO_CFG_Type iocon;
	IOCON_StructInit(&iocon);
	iocon.type = IOCON_PIO_0_31;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_1_0;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_1_1;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_1_2;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_1_3;
	IOCON_SetFunc(&iocon);
	RELAY_PORT hPort1 = { LPC_GPIO0, GPIO_PIN_31 };
	RELAY_PORT hPort2 = { LPC_GPIO1, GPIO_PIN_0 };
	RELAY_PORT hPort3 = { LPC_GPIO1, GPIO_PIN_1 };
	RELAY_PORT rVP1 = { LPC_GPIO1, GPIO_PIN_2 };
	RELAY_PORT lVP1 = { LPC_GPIO1, GPIO_PIN_3 };
	RELAY_PORT rVP = { LPC_GPIO1, GPIO_PIN_4 };
	RELAY_PORT lVP = { LPC_GPIO1, GPIO_PIN_5 };
	COM_PORT_T zbPort = { UART1, UART_LOC_0, 0, 38400 };
	//COM_PORT_T rsPort = { UART0, UART_LOC_0, 0, 38400 };


	//SYS_ResetPeripheral(SYS_PRESETCTRL_UART1_RST, DISABLE);

	UARTInit(RS485_PORT, 250000, RS485_LOC);
	UART_RS485Init();
	/*UART_CFG_Type ucfg;
	UART_Init((LPC_UART_TypeDef *)LPC_UART1);
	UART_GetConfig((LPC_UART_TypeDef *)LPC_UART1, &ucfg);
	ucfg.baudrate = 115200;
	UART_SetConfig((LPC_UART_TypeDef *)LPC_UART1, &ucfg);
	SetupUART_Location(ZB_PORT, ZB_LOC);
	UART_ConfigInts((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, ENABLE);*/
	//SYS_ResetPeripheral(SYS_PRESETCTRL_UART1_RST, ENABLE);
	UARTInit(ZB_PORT, 38400, ZB_LOC);

	// Set Callback response
	UARTRegDataCb(RS485_PORT, DMXDataRecv_CallBack);
	UARTRegBreakIntCallback(RS485_PORT, DMXBreakInt_CallBack);
	UARTRegDataCb(ZB_PORT, phyZBDataIn_Callback);
	//UARTRegSendCmp(RS485_PORT, phyRS485SendCmp_Callback);
	UARTRegSendCmp(ZB_PORT, phyZBSendCmp_Callback);


	// Heater Initialized hardware
	pH1.Init(hPort1);
	pH2.Init(hPort2);
	pH3.Init(hPort3);

	//pH1.Relay_Control(RELAY_ON);
	//pH2.Relay_Control(RELAY_ON);
	//pH3.Relay_Control(RELAY_ON);

	hArry[0] = &pH1;
	hArry[1] = &pH2;
	hArry[2] = &pH3;
	//pHeater.Relay_Control(RELAY_ON);
	// Valve initialized hardware
	pValve.Init(lVP, rVP);
	//pValve.Valve_Control(VALVE_LEFT);
	pValve.Valve_Disable();
	pValve2.Init(lVP1,rVP1);
	pValve2.Valve_Disable();
	// Motor initialized hardware
	//pMotor.Init();
	pTB6600.Init();
	// Temperature initialized hardware
	pTempSensor.Init(tTable, 101);
	//
	pIAAP.Init(hArry[0], &pValve, &pTB6600, &pTempSensor);
	//pRS485.Init(&pIAAP, &rsPort);
	//pRS485.SetAddress(0x01);
	pZB.Init(&pIAAP, &zbPort);

	pBt.Init();
	pBt.RegisterCallBack(button_CallBack);




}

int main(void) {
	//RELAY_PORT rPort;
    // TODO: insert code here
	SystemCoreClockUpdate();
	// Set SysTick as 10 us
	SysTick_Config( SystemCoreClock / (10000 - 1));
	// Initialized parameter

    // Initialized object
	Init();
	//

    // Enter an infinite loop, just incrementing a counter
    while(1) {
    	if (ObjectTick.us10Tick) {
//#ifndef BMODEL
        	pTB6600.Tick();
//#endif
        	ObjectTick.us10Tick = FALSE;
    	}

        if (ObjectTick.ms1Tick) {
        	pBt.Tick();
/*
#ifdef BMODEL
        	pTB6600.Tick();
#endif
*/
        	ObjectTick.ms1Tick = FALSE;
        }
        if (ObjectTick.ms10Tick) {

        	ObjectTick.ms10Tick = FALSE;
        }

        if (ObjectTick.ms100Tick) {

        	ObjectTick.ms100Tick = FALSE;
        }

        if (ObjectTick.secTick) {
        	pTempSensor.Read();
        	//UARTSendCh(RS485_PORT, pTempSensor.GetTemp());
        	//UARTSendCh(ZB_PORT, pTempSensor.GetTemp());
        	ObjectTick.secTick = FALSE;
        }

        if (pZB.PendingProcess()) {
        	pZB.Interactive();
        }
    }
    return 0 ;
}

void SysTick_Handler(void) {

	ObjectTick.us10Tick = TRUE;
	if (++TickTime.Tick1ms >= 10) {
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
		TickTime.Tick1ms = 0;
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
		pTempSensor.calTemp(adcVal);
		TempSensor::ReadCallback(&pTempSensor, adcVal);
		//UARTSendCh(RS485_PORT, pTempSensor.GetTemp());
	}
}


