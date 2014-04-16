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

TIM32_MATCHTypeDef matchVal;
TIM32_PWMTypeDef pwmCfg;

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
			pMotor.ForceMove(MOVE_FF);
		}
		if (state == BT_STATE_PRESS) {
			pMotor.ForceMove(MOVE_STOP);
		}
		break;
	case BT_DOWN :
		if (state == BT_STATE_PUSH) {
			//pMotor.MoveToStep()
			pMotor.ForceMove(MOVE_RW);
		}
		else if (state == BT_STATE_PRESS) {
			pMotor.ForceMove(MOVE_STOP);
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
	UARTRegDataCb(RS485_PORT, phyRS485DataIn_Callback);
	//UARTRegDataCb(ZB_PORT, phyZBDataIn_Callback);
	UARTRegSendCmp(RS485_PORT, phyRS485SendCmp_Callback);
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

void PWM_INIT(void) {
	TIM32_InitTypeDef tim32;

/*	IOCON_PIO_CFG_Type icon;

	IOCON_StructInit (&icon);
	icon.type = IOCON_CT32_B0_MAT2_LOC1;
	icon.invert = IOCON_PIO_INV_INVERTED;
	 IOCON_SetFunc (&icon);*/

	TIM32_StructInit(TIM32_MODE_TIMER, &tim32);
	tim32.PrescaleOption = TIM32_PRESCALE_USVAL;
	tim32.PrescaleValue = 1;	// 1 usec

	pwmCfg.MatchChannel = TIM32_MATCH_CHANNEL0;
	pwmCfg.IntOnMatch = TRUE;
	pwmCfg.ResetOnMatch = TRUE;
	pwmCfg.StopOnMatch = FALSE;
	pwmCfg.ExtPWMOutput = DISABLE;
	pwmCfg.MatchValue = 100; //--> 10 kHz

	TIM32_Init(LPC_CT32B0, TIM32_MODE_TIMER, &tim32);
	TIM32_ConfigPWM(LPC_CT32B0, &pwmCfg);

	pwmCfg.MatchChannel = TIM32_MATCH_CHANNEL2;
	pwmCfg.IntOnMatch = TRUE;
	pwmCfg.ResetOnMatch = FALSE;
	pwmCfg.StopOnMatch = FALSE;
	pwmCfg.ExtPWMOutput = DISABLE;
	pwmCfg.MatchValue = 25; //--> 50%
	TIM32_ConfigPWM(LPC_CT32B0, &pwmCfg);

	SYS_ConfigAHBCLK(SYS_AHBCLKCTRL_CT32B0, ENABLE);
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
	TIM32_Cmd(LPC_CT32B0, ENABLE);

}


uint8_t motorState;
//uint8_t pwmCount;
uint8_t apc;
uint8_t anc;
uint8_t bpc;
uint8_t bnc;
uint8_t cstr;

void TestMotor(void) {

	switch (motorState) {
	case 0 :

		GPIO_SetHighLevel(MOTOR_PORT, GATE_AP, ENABLE);	// A+ 100
		GPIO_SetLowLevel(MOTOR_PORT, GATE_AN, ENABLE);	// A- off
		GPIO_SetHighLevel(MOTOR_PORT, GATE_BP, ENABLE);	// B+ 100
		GPIO_SetLowLevel(MOTOR_PORT, GATE_BN, ENABLE);	// B- off
		motorState = 1;
		cstr |= 1;
		break;
	case 1 :
		GPIO_SetHighLevel(MOTOR_PORT, GATE_AP, ENABLE);	// A+ 100
		GPIO_SetLowLevel(MOTOR_PORT, GATE_AN, ENABLE);	// A- off
		GPIO_SetLowLevel(MOTOR_PORT, GATE_BP, ENABLE);	// B+ off
		GPIO_SetHighLevel(MOTOR_PORT, GATE_BN, ENABLE);	// B- 100
		motorState = 2;
		cstr |= 2;
		break;
	case 2 :
		GPIO_SetLowLevel(MOTOR_PORT, GATE_AP, ENABLE);	// A+ off
		GPIO_SetHighLevel(MOTOR_PORT, GATE_AN, ENABLE);	// A- 100
		GPIO_SetLowLevel(MOTOR_PORT, GATE_BP, ENABLE);	// B+ off
		GPIO_SetHighLevel(MOTOR_PORT, GATE_BN, ENABLE);	// B- 100
		motorState = 3;
		cstr |= 4;
		break;
	case 3 :
		GPIO_SetLowLevel(MOTOR_PORT, GATE_AP, ENABLE);	// A+ off
		GPIO_SetHighLevel(MOTOR_PORT, GATE_AN, ENABLE);	// A- 100
		GPIO_SetHighLevel(MOTOR_PORT, GATE_BP, ENABLE);	// B+ 100
		GPIO_SetLowLevel(MOTOR_PORT, GATE_BN, ENABLE);	// B- off
		motorState = 0;
		cstr |= 8;
		break;
	}
}
#define DUtyTh		20
void TestPWMTick(void) {
	if (cstr & 1) {
		if (++apc >= DUtyTh) {
			GPIO_SetLowLevel(MOTOR_PORT, GATE_AP, ENABLE);	// A+ 100
			apc = 0;
			cstr &=  ~(uint8_t)0x01;
		}
	}
	if (cstr & 2) {
		if (++bnc >= DUtyTh) {
			GPIO_SetLowLevel(MOTOR_PORT, GATE_BN, ENABLE);	// A+ 100
			bnc = 0;
			cstr &=  ~(uint8_t)0x02;
		}
	}
	if (cstr & 4) {
		if (++anc >= DUtyTh) {
			GPIO_SetLowLevel(MOTOR_PORT, GATE_AN, ENABLE);	// A+ 100
			anc = 0;
			cstr &=  ~(uint8_t)0x04;
		}
	}
	if (cstr & 8) {
		if (++bpc >= DUtyTh) {
			GPIO_SetLowLevel(MOTOR_PORT, GATE_BP, ENABLE);	// A+ 100
			bpc = 0;
			cstr &=  ~(uint8_t)0x08;
		}
	}
}
#define DutyVal		4
void PWMPPeriod_Rountine(void) {
	if (apc >= DutyVal && apc < 20) {
		GPIO_SetHighLevel(MOTOR_PORT, GATE_AP, ENABLE);	// A+
	}
	if (anc >= DutyVal && anc < 20) {
		GPIO_SetHighLevel(MOTOR_PORT, GATE_AN, ENABLE);	// A-
	}
	if (bpc >= DutyVal && bpc < 20) {
		GPIO_SetHighLevel(MOTOR_PORT, GATE_BP, ENABLE);	// B+
	}
	if (bnc >= DutyVal && bnc < 20) {
		GPIO_SetHighLevel(MOTOR_PORT, GATE_BN, ENABLE);	// B-
	}
}

void PWMMatch_Rountine(void) {
	if (apc >= DutyVal && apc < 20) {
		GPIO_SetLowLevel(MOTOR_PORT, GATE_AP, ENABLE);	// A+
	}
	if (anc >= DutyVal && anc < 20) {
		GPIO_SetLowLevel(MOTOR_PORT, GATE_AN, ENABLE);	// A-
	}
	if (bpc >= DutyVal && bpc < 20) {
		GPIO_SetLowLevel(MOTOR_PORT, GATE_BP, ENABLE);	// B+
	}
	if (bnc >= DutyVal && bnc < 20) {
		GPIO_SetLowLevel(MOTOR_PORT, GATE_BN, ENABLE);	// B-
	}
}

int main(void) {
	//RELAY_PORT rPort;
	uint8_t mt, start, set, state, count;
	mt = 0;
	start = 0;
	set = 0;
	state = 0;
	count = 0;
	motorState = 0;
	apc = 0;
	anc = 0;
	bpc = 0;
	bnc = 0;
	matchVal.MatchChannel = TIM32_MATCH_CHANNEL2;
	matchVal.IntOnMatch = FALSE;
	matchVal.ResetOnMatch = FALSE;
	matchVal.StopOnMatch = FALSE;
	//matchVal.MatchValue = 18;
    // TODO: insert code here
	SystemCoreClockUpdate();
	// Set SysTick as 1 ms
	SysTick_Config( SystemCoreClock / (1000 - 1));
	// Initialized parameter

    // Initialized object
	Init();
/*	LPC_GPIO0->CLR |= (1 << GATE_AP)
					| (1 << GATE_AN)
					| (1 << GATE_BP)
					| (1 << GATE_BN);*/
	PWM_INIT();
	pTempSensor.Read();
	//MOTOR_PORT->CLR = (1 << GATE_AN);

    // Enter an infinite loop, just incrementing a counter
    while(1) {
        if (ObjectTick.ms1Tick) {
        	pMotor.Tick();
        	//TestPWMTick();
        	pBt.Tick();
        	ObjectTick.ms1Tick = FALSE;
        }
        if (ObjectTick.ms10Tick) {
        	//TestMotor();
/*        	if (!start) {
            	matchVal.MatchValue = 100;
            	TIM32_ConfigMatch(LPC_CT32B0, &matchVal);
            	start = 1;
            	count = 0;
        	}
        	else {
        		if (!set && (++count >= 2)) {
        			matchVal.MatchValue = 10;
        			TIM32_ConfigMatch(LPC_CT32B0, &matchVal);
        			set = 1;
        		}
        	}*/
        	ObjectTick.ms10Tick = FALSE;
        }

        if (ObjectTick.ms100Tick) {
        	if (state) {
				start = 0;
				set = 0;
				pwmCfg.ExtPWMOutput = ENABLE;
				TIM32_ConfigPWM(LPC_CT32B0, &pwmCfg);
				state = 0;
        	}
        	else {
        		pwmCfg.ExtPWMOutput = DISABLE;
        		TIM32_ConfigPWM(LPC_CT32B0, &pwmCfg);
				state = 1;
        	}

        	ObjectTick.ms100Tick = FALSE;
        }

        if (ObjectTick.secTick) {
        	/*if (++DutyTh >= 20) {
        		DutyTh = 10;
        	}*/
        	//pMotor.moveStep(MOVE_FF);
        	/*if (mt) {
        		MOTOR_PORT->CLR = (1 << GATE_BP);
        		MOTOR_PORT->SET = (1 << GATE_BN);
        		mt = 0;
        	}
        	else {
				MOTOR_PORT ->CLR = (1 << GATE_BN);
				MOTOR_PORT ->SET = (1 << GATE_BP);
				mt = 1;
        	}*/
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

void TIMER32_0_IRQHandler(void) {
    uint32_t i, int_status_reg;

    int_status_reg = TIM32_GetIntStatusReg (LPC_CT32B0);


    /*clear the interrupt flag of match channel 0  */
    if (int_status_reg & TIM32_INT_MR0) {
    	//PWMPPeriod_Rountine();
    	TIM32_ClearIntPendingBit(LPC_CT32B0,TIM32_INT_MR0);
    }
    if (int_status_reg & TIM32_INT_MR2) {
    	PWMMatch_Rountine();
    	TIM32_ClearIntPendingBit(LPC_CT32B0,TIM32_INT_MR2);
    }
}

