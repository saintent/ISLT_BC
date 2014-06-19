/*
 * MotorControl.cpp
 *
 *  Created on: Oct 11, 2013
 *      Author: MSI
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MotorControl.h"
#include "lpc12xx_libcfg.h"
#include "lpc12xx_ssp.h"
#include "ssp.h"
#include "l6482.h"
#include "l6482_type.h"
#include "lpc_uart.h"
#include "lib_fifo.h"

MotorControl::MotorControl() {
	// TODO Auto-generated constructor stub
	//this->Init();
}

MotorControl::~MotorControl() {
	// TODO Auto-generated destructor stub
}

void MotorControl::Init(void) {
	char str[16];
	this->TvalTable[MOTOR_CURRENT_2A] = 7;
	this->TvalTable[MOTOR_CURRENT_3A] = 10;
	this->TvalTable[MOTOR_CURRENT_4A] = 13;
	this->TvalTable[MOTOR_CURRENT_5A] = 16;
	this->TvalTable[MOTOR_CURRENT_6A] = 30;
	this->currentLimit = MOTOR_CURRENT_2A;
	// Initiate IO as Out put
	IOCON_PIO_CFG_Type icon;
	IOCON_StructInit(&icon);
	icon.type = IOCON_PIO_0_20;
	IOCON_SetFunc(&icon);
	icon.type = IOCON_PIO_0_21;
	IOCON_SetFunc(&icon);
	icon.type = IOCON_PIO_0_22;
	IOCON_SetFunc(&icon);
	icon.type = IOCON_PIO_0_23;
	IOCON_SetFunc(&icon);
	// Set as output
	GPIO_SetDir(RESET_PORT, L6482_RESET, 1);
	GPIO_SetDir(STCK_PORT, L6482_STCK, 1);
	// Set as input
	GPIO_SetDir(FLAG_PORT, L6482_FLAG, 0);
	GPIO_SetDir(BUSY_PORT, L6482_BUSY, 0);
	// Hold reset state for l6482
	GPIO_SetHighLevel(RESET_PORT, L6482_RESET, 1);

	SSPInit();
	//sprintf(str, "L6842 Init\n");
	//motorDebug(str, 11);
	//device.HardHiZ();
	//device.HardStop();
	device.ResetDevice();
	device.SoftStop();
	this->l6482_Register.Status.Word = device.GetStatus();
	// Get all register in l6482
	this->l6482_Register.CurrentPos = device.GetCurrentPosition();
	this->l6482_Register.ElecticalPos = device.GetElecPosition();
	this->l6482_Register.MarkPos = device.GetMarkPosition();
	this->l6482_Register.CurrentSpeed = device.GetSpeed();
	// Set acc 68
	device.SetAcceleration(10);
	this->l6482_Register.Acc = device.GetAcceleration();
	// Set dec
	device.SetDeceleration(10);
	this->l6482_Register.Dec = device.GetDeceleration();
	// Set Max speed
	device.SetMaxSpeed(5);
	this->l6482_Register.MaxSpeed = device.GetMaxSpeed();
	// Set Min speed
	device.SetMinSpeed(5);
	this->l6482_Register.Minspeed = device.GetMinSpeed();
	// Set Full Step speed
	device.SetFullStepSpeed(5);
	this->l6482_Register.FullStepSpeed = device.GetFullStepSpeed();
	// Set holding current
	//device.SetHoldingRefVolt(this->TvalTable[this->currentLimit]);
	this->l6482_Register.HoldingRefVolt = device.GetHoldingRefVolt();
	// Set run current
	//device.SetConstanceRefVolt(this->TvalTable[this->currentLimit]);
	this->l6482_Register.ConstanctRefVolt = device.GetConstanceRefVolt();
	// Set Acc current
	//device.SetAccStartRefVolt(this->TvalTable[this->currentLimit]);
	this->l6482_Register.AccStartRefVolt = device.GetAccStartRefVolt();
	// Set dec current
	//device.SetDeceleration(this->TvalTable[this->currentLimit]);
	this->l6482_Register.DecStartRefVolt = device.GetDeceleration();
	// Max fast decay = 10 µs, Max fast decay at step change = 16 µs
	device.SetFastDecaySetting((uint8_t)(4 << 4) | (7));
	this->l6482_Register.FasDecaySetting = device.GetFastDecaySettting();
	// Minimum ON time = 4 µs.
	device.SetMinOnTime(1);
	this->l6482_Register.MinOnTime = device.GetMinOnTime();
	// Minimum OFF time = 21 µs.
	device.SetMinOffTime(41);
	this->l6482_Register.MinOffTime = device.GetMinOffTime();
	this->l6482_Register.Adc_Out = device.GetADCOut();
	// Set over current
	device.SetOCDThreshold(15);
	this->l6482_Register.ODCThreshole = device.GetOCDThreshold();
	device.SetStepMode(0x07);
	this->l6482_Register.StepMode = device.GetStepMode();
	device.SetAlarmSetting(0x01);
	this->l6482_Register.AlarmEnable = device.GetAlarmSetting();
	//  Gate current 64 mA, controlled current time 625ns
	device.SetGateDriveConfig(1, (uint16_t)((3 << 5) | 0x17));
	this->l6482_Register.GateDriverConfig1 = device.GetGateDriveConfig(1);
	//   Dead time : 375 ns, TBLANK : 750ns
	device.SetGateDriveConfig(2, (uint16_t)(( 7 << 5) | 7));
	this->l6482_Register.GateDriverConfig2 = device.GetGateDriveConfig(2);
	//device.SetConfig()
	this->l6482_Register.Config.Word = device.GetConfig();
	//this->l6482_Register.Config.Bit.SW_MODE = 1;
	// VCC value = 15 V.
	this->l6482_Register.Config.Bit.VCCVAL = 1;
	// UVLO threshold = 11 V (10 V on boot).
	this->l6482_Register.Config.Bit.UVLOVAL = 1;
	// Target switching time = 48 µs.
	this->l6482_Register.Config.Bit.TSW = 20;
	// Predictive current control enabled.
	this->l6482_Register.Config.Bit.PRED_E = 1;
	device.SetConfig(this->l6482_Register.Config.Word);
	this->l6482_Register.Config.Word = device.GetConfig();

	//motorPrintStatus();
	GET_MOTOR_STA();
	//device.StepMode(MOVE_FF);
	//GET_MOTOR_STA();
	device.Run(MOVE_FF, 3200);
	//GET_MOTOR_STA();
	//device.Move(MOVE_FF, 400);
	//memset(this->level, 0, 4);
	this->level[0] = 0;
	this->level[1] = 200;
	this->level[2] = 400;
	this->level[3] = 600;

	this->countTime = 0;

	FIFO_Init(&this->debugFIFO, this->textDebug, 32);

	//device.HardStop();
	//device.Run(MOVE_FF, 13);
	GET_MOTOR_STA();

}

void MotorControl::Reset(void) {
	this->Init();
}

void MotorControl::Tick(void) {
	Bool isAlarm;
	Bool busy;
	char str[8];
	// Alarm check
	isAlarm = CheckAlarm();
	// Alarm Event
	/*if (isAlarm == TRUE) {
		// Getting alarm
		l6482_Register.Status.Word = device.GetStatus();
		//motorPrintStatus();
	}*/

	/*if (GPIO_GetPinValue(STCK_PORT, L6482_STCK)) {
		GPIO_SetLowLevel(STCK_PORT, L6482_STCK, 1);
	}
	else {
		GPIO_SetHighLevel(STCK_PORT, L6482_STCK, 1);
	}*/

	this->l6482_Register.CurrentPos = device.GetCurrentPosition();

	busy = IsBusy();
	if (busy == TRUE) {
		sprintf(str, "Busy\r\n");
	}
	else {
		sprintf(str, "Free\r\n");
	}
	motorDebug(str, 6);


}

Status MotorControl::MoveToLevel(MOVE_LEVEL_T position) {
	Status st;
	uint16_t lv;
	uint32_t step;
	st = ERROR;
	// Calculate number of step
	lv = this->level[position];
	// If move forward

	// If move reverse


	return (st);
}

Status MotorControl::MoveToStep(L6482_DIR_Typedef dir, uint32_t step) {
	Status st;
	st = ERROR;
	//direction = dir;
	if (step > 0x03FFFFF) {
		return st;
	}
	device.HardStop();
	this->l6482_Register.Status.Word = device.GetStatus();
	switch (dir) {
	case MOVE_FF:
		device.Move(MOVE_FF, step);
		break;
	case MOVE_RW:
		device.Move(MOVE_RW, step);
		break;
	default:
		break;
	}
	st = SUCCESS;
	this->l6482_Register.Status.Word = device.GetStatus();
	return (st);
}

void MotorControl::SetLevel(MOVE_LEVEL_T level, uint16_t val) {
	this->level[level] = val;
}

void MotorControl::GetMoveLevel(uint8_t* dOut, uint8_t* dSize) {
	memcpy(dOut, level, sizeof(level));
	dSize[0] = sizeof(level);
}

uint16_t MotorControl::GetSpeed(void) {
	return (uint16_t)this->l6482_Register.CurrentSpeed;
}

Bool MotorControl::IsBusy(void) {
	uint32_t busyFlag;
	busyFlag = GPIO_GetPinValue(BUSY_PORT, L6482_BUSY);
	// Not busy
	if (busyFlag) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

Bool MotorControl::CheckAlarm(void) {
	uint32_t flag;
	flag = GPIO_GetPinValue(FLAG_PORT, L6482_FLAG);
	// No error occur
	if (flag) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}

void MotorControl::motorDebug(char* str,uint8_t str_len) {
	uint8_t index;
	index = 0;
	while (index < str_len) {
		FIFO_PutChar(&this->debugFIFO, str[index]);
		index++;
	}
	UART_StartSent(UART0);
}

void MotorControl::motorPrintStatus(void) {
	char str[32];
	L6482_STATUS_Reg * sta;
	this->l6482_Register.Status.Word = device.GetStatus();
	sta = &this->l6482_Register.Status;
	// High impedance status
	if(sta->Bit.HiZ == 1) {
		sprintf(str, "\nHiZ: 1\t");
	}
	else { sprintf(str, "\nHiZ: 0\t"); }
	motorDebug(str, 8);

	// Over current status
	if(sta->Bit.OCD == 1) {
		sprintf(str, "OCD: 1\t");
	}
	else { sprintf(str, "OCD: 0\t"); }
	motorDebug(str, 7);

	// Thermal Status
	if(sta->Bit.TH_SD == 0) {
		sprintf(str, "TH_SD: No\t");
	}
	else if(sta->Bit.TH_SD == 1) {
		sprintf(str, "TH_SD: Wr\t");
	}
	else if(sta->Bit.TH_SD == 2) {
		sprintf(str, "TH_SD: BS\t");
	}
	else {
		sprintf(str, "TH_SD: DS\t");
	}
	motorDebug(str, 10);

	// Move Dir
	if(sta->Bit.DIR == 1) {
		sprintf(str, "\r\nMDIR: FF\t");
	}
	else {
		sprintf(str, "\r\nMDIR: RW\t");
	}
	motorDebug(str, 11);

	// Motor Status
	if(sta->Bit.MOT_STATUS == 0) {
		sprintf(str, "MSTA: ST\r\n");
	}
	else if(sta->Bit.MOT_STATUS == 1) {
		sprintf(str, "MSTA: AC\r\n");
	}
	else if(sta->Bit.MOT_STATUS == 2) {
		sprintf(str, "MSTA: DC\r\n");
	}
	else {
		sprintf(str, "MSTA: CS\r\n");
	}
	//str[9] = '\n';
	motorDebug(str, 10);
}

void MotorControl::SendByte(void) {
	uint8_t data;
	Uart_type port;
	FIFO_ATTR_T* pFIFO;
	pFIFO = &this->debugFIFO;
	port = UART0;

	if (FIFO_Count(pFIFO) != 0) {
		FIFO_GetChar(pFIFO, &data, TRUE);
		UARTSendCh(port, data);
	}
	else {
		UART_StopSent(port);
	}
}
