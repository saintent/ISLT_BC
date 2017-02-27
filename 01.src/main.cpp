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
#include "comport.h"
#include "relay.h"
#include "TempSensor.h"
#include "TTable.h"
#include "IAAP.h"
//#include "tb6600.h"
#include "lpc12xx_uart.h"
#include "DMXSlave.h"
#include "DMXChannelHandler.h"
#include "LPCTimer.h"
#include "motor/MotorControl.h"

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
} TickTime;
struct {
	uint8_t us10Tick;
	uint8_t ms1Tick;
	uint8_t ms10Tick;
	uint8_t ms100Tick;
	uint8_t secTick;
	uint8_t rsv[2];
} ObjectTick;

void DMXFrameReceivedComplete(uint16_t ch);

// Object in program
Heater pH1;
Heater pH2;
Heater pH3;
//Comport			pRS485;
Comport pZB;
MOTOR::MotorControl pMotor;
TempSensor pTempSensor;
IAAP pIAAP;
tb6600 pTB6600;
dmx::DMXSlave dmxSlave(1, 8, DMXFrameReceivedComplete);
dmx::DMXChannelHandler MotorCh(1, &pMotor);
dmx::DMXChannelHandler HeaterCh1(2, &pH1);
dmx::DMXChannelHandler HeaterCh2(3, &pH2);
dmx::DMXChannelHandler HeaterCh3(4, &pH3);

void DMXFrameReceivedComplete(uint16_t ch) {
	MotorCh.SetValue(dmxSlave.GetChannelValue(1));
	HeaterCh1.SetValue(dmxSlave.GetChannelValue(2));
	HeaterCh2.SetValue(dmxSlave.GetChannelValue(3));
	HeaterCh3.SetValue(dmxSlave.GetChannelValue(4));
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

void PinMap(void) {
	IOCON_PIO_CFG_Type iocon;
	IOCON_StructInit(&iocon);
	// Heater 3
	iocon.type = IOCON_PIO_0_31;
	IOCON_SetFunc(&iocon);
	// Heater 2
	iocon.type = IOCON_PIO_1_0;
	IOCON_SetFunc(&iocon);
	// Heater 1
	iocon.type = IOCON_PIO_1_1;
	IOCON_SetFunc(&iocon);

	// DMX Channel select pin
	iocon.od = IOCON_PIO_OD_DISABLE;
	iocon.pmode = IOCON_PIO_PMODE_ENABLE;
	iocon.pinmode = IOCON_PIO_MODE_PULLUP;
	iocon.type = IOCON_PIO_0_3;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_0_4;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_0_5;
	IOCON_SetFunc(&iocon);
	iocon.type = IOCON_PIO_0_6;
	IOCON_SetFunc(&iocon);
	GPIO_SetDir(LPC_GPIO0, 3, 0);
	GPIO_SetDir(LPC_GPIO0, 4, 0);
	GPIO_SetDir(LPC_GPIO0, 5, 0);
	GPIO_SetDir(LPC_GPIO0, 6, 0);
}

void DMXSetup(void) {
	uint32_t dport;

	dport = ~(LPC_GPIO0 ->PIN >> 3) & 0x0F;

	switch (dport) {
	case 0:
		dmxSlave.SetStartChannel(1);
		break;
	case 1:
		dmxSlave.SetStartChannel(49);
		break;
	case 2:
		dmxSlave.SetStartChannel(71);
		break;
	case 4:
		dmxSlave.SetStartChannel(91);
		break;
	case 8:
		dmxSlave.SetStartChannel(121);
		break;
	}
}

void Init(void) {
	Heater* hArry[3];
	PinMap();
	RELAY_PORT hPort1 = { LPC_GPIO0, GPIO_PIN_31 };
	RELAY_PORT hPort2 = { LPC_GPIO1, GPIO_PIN_0 };
	RELAY_PORT hPort3 = { LPC_GPIO1, GPIO_PIN_1 };
	COM_PORT_T zbPort = { UART1, UART_LOC_0, 0, 38400 };

	UARTInit(RS485_PORT, 250000, RS485_LOC);
	UART_RS485Init();
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

	hArry[0] = &pH1;
	hArry[1] = &pH2;
	hArry[2] = &pH3;
	// Motor initialized hardware
	pMotor.Init();
	// Temperature initialized hardware
	pTempSensor.Init(tTable, 101);
	//
	pIAAP.Init(hArry[0], 0, 0, &pTempSensor);
	pZB.Init(&pIAAP, &zbPort);

	//dmxSlave.frame.SetFrameSize(8);
	DMXSetup();
}

void DMSProcessTick(void) {
	static dmx::DMXChannelHandler* dmxHeater[3] = { &HeaterCh1, &HeaterCh2,
			&HeaterCh3 };
	uint8_t dmxMVal;
	//if (MotorCh.IsUpdate()) {

		dmxMVal = MotorCh.GetValue();
		if (dmxMVal >= 200) {
			((MOTOR::MotorControl*) MotorCh.context)->MoveTo(1200);
		} else if (dmxMVal < 200 && dmxMVal >= 150) {
			((MOTOR::MotorControl*) MotorCh.context)->MoveTo(2400);
		} else if (dmxMVal < 150 && dmxMVal >= 70) {
			((MOTOR::MotorControl*) MotorCh.context)->MoveTo(3200);
		} else {
			((MOTOR::MotorControl*) MotorCh.context)->MoveTo(4000);
		}
	//}
	for (uint8_t i = 0; i < 3; i++) {
		if (dmxHeater[i]->IsUpdate()) {
			if (dmxHeater[i]->GetValue() > 200) {
				((Heater*) (dmxHeater[i]->context))->Relay_Control(RELAY_ON);
			} else {
				((Heater*) (dmxHeater[i]->context))->Relay_Control(RELAY_OFF);
			}
		}
	}

	LPC_UART0->FDR |= UART_CFG_DMAMODE_ENABLE;

}

int main(void) {
	static uint8_t c500ms = 0;

	//RELAY_PORT rPort;
	// TODO: insert code here
	SystemCoreClockUpdate();
	// Set SysTick as 10 us
	SysTick_Config(SystemCoreClock / (10000 - 1));
	// Initialized parameter

	// Initialized object
	Init();
	//
	pMotor.MoveTo(4000);
	// Enter an infinite loop, just incrementing a counter
	while (1) {
		if (ObjectTick.us10Tick) {
			ObjectTick.us10Tick = FALSE;
		}

		if (ObjectTick.ms1Tick) {
			ObjectTick.ms1Tick = FALSE;
		}
		if (ObjectTick.ms10Tick) {
			ObjectTick.ms10Tick = FALSE;
		}

		if (ObjectTick.ms100Tick) {

			if (++c500ms >= 5) {
				DMSProcessTick();
				c500ms = 0;
			}
			ObjectTick.ms100Tick = FALSE;
		}

		if (ObjectTick.secTick) {
			pTempSensor.Read();
			ObjectTick.secTick = FALSE;
		}

		if (pZB.PendingProcess()) {
			pZB.Interactive();
		}
	}
	return 0;
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
				ObjectTick.ms100Tick = TRUE;
				if (++TickTime.Tick1sec >= 10) {
					// 1 sec Routing
					ObjectTick.secTick = TRUE;
					TickTime.Tick1sec = 0;
				}
				TickTime.Tick100ms = 0;
			}
			TickTime.Tick10ms = 0;
		}
		TickTime.Tick1ms = 0;
	}
}

void ADC_IRQHandler(void) {
	uint32_t reg;
	uint16_t adcVal;
	reg = LPC_ADC ->GDR;		//Get Status
	reg = LPC_ADC ->STAT;
	// Interrupt flag
	if (reg & ADC_ADINT) {
		// Channel 0
		adcVal = ADC_GetData(ADC_CHANNEL_0);
		pTempSensor.calTemp(adcVal);
		TempSensor::ReadCallback(&pTempSensor, adcVal);
	}
}

