//================ File Desciption =========================================//
//=== File name : button.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "button.h"
#include "string.h"
#include "lpc12xx_libcfg.h"
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

button::button() {
	// TODO Auto-generated constructor stub

}

button::~button() {
	// TODO Auto-generated destructor stub
}

void button::Init(void) {
	// Initiate IO as Out put
	IOCON_PIO_CFG_Type icon;
	IOCON_StructInit(&icon);
	icon.od = IOCON_PIO_OD_ENABLE;
	icon.type = IOCON_PIO_0_3;
	IOCON_SetFunc(&icon);
	icon.type = IOCON_PIO_0_4;
	IOCON_SetFunc(&icon);
	icon.type = IOCON_PIO_0_5;
	IOCON_SetFunc(&icon);
	icon.type = IOCON_PIO_0_6;
	IOCON_SetFunc(&icon);
	// Set GPIO as input
	GPIO_SetDir(BT_PORT, BT_U_LOC, 0);
	GPIO_SetDir(BT_PORT, BT_D_LOC, 0);
	GPIO_SetDir(BT_PORT, BT_L_LOC, 0);
	GPIO_SetDir(BT_PORT, BT_L_LOC, 0);
	this->cbFn = 0;
	this->btState[0].state = BT_STATE_IDLE;
	this->btState[1].state = BT_STATE_IDLE;
	this->btState[2].state = BT_STATE_IDLE;
	this->btState[3].state = BT_STATE_IDLE;
}

void button::Tick(void) {
	this->checkStatus(BT_UP, (uint8_t)GPIO_GetPinValue(BT_PORT, BT_U_LOC));
	this->checkStatus(BT_DOWN, (uint8_t)GPIO_GetPinValue(BT_PORT, BT_D_LOC));
	this->checkStatus(BT_RIGHT, (uint8_t)GPIO_GetPinValue(BT_PORT, BT_R_LOC));
	this->checkStatus(BT_LEFT, (uint8_t)GPIO_GetPinValue(BT_PORT, BT_L_LOC));
}

void button::RegisterCallBack(ButtonCallBack_t *fn) {
	this->cbFn = fn;
}

void button::checkStatus(BT_TYPE_T type, uint8_t status) {
	BT_OBJ_TYPE_T* bt;
	bt = &this->btState[type];
	switch (bt->state) {
	case BT_STATE_IDLE :
	default :
		if (status == 1) {
			if (++bt->decisionTime >= BT_SENTIVITY) {
				bt->state = BT_STATE_PUSH;
				bt->decisionTime = 0;
			}
		}
		else {
			bt->decisionTime = 0;
		}
		break;
	case BT_STATE_PUSH :
		if (status == 0) {
			if (++bt->decisionTime >= BT_SENTIVITY) {
				bt->state = BT_STATE_PRESS;
				bt->decisionTime = 0;
			}
		}
		else {
			if (this->cbFn != 0) {
				cbFn(type, BT_STATE_PUSH);
			}
			bt->decisionTime = 0;
		}
		break;
	case BT_STATE_PRESS :
		if (this->cbFn != 0) {
			cbFn(type, BT_STATE_PRESS);
		}
		bt->state = BT_STATE_IDLE;
		break;
	}
}

//================ END OF FILE ==============================================//

