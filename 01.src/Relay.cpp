//================ File Desciption =========================================//
//=== File name : Relay.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "Relay.h"
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

Relay::Relay(void) {
	this->status = RELAY_OFF;
}

Relay::Relay(RELAY_PORT port) {
	// TODO Auto-generated constructor stub
	this->port = port;
	this->status = RELAY_OFF;
	Init(this->port);
}

Relay::~Relay() {
	// TODO Auto-generated destructor stub
}

void Relay::Init(RELAY_PORT port) {
	this->port = port;
	GPIO_SetDir(this->port.loc, this->port.pin, GPIO_OUTPUT);
	GPIO_SetLowLevel(this->port.loc, this->port.pin, ENABLE);
}

RELAY_ST Relay::Relay_Control(RELAY_ST State) {
	if (State == RELAY_OFF) {
		GPIO_SetLowLevel(port.loc, port.pin, ENABLE);
	}
	else {
		GPIO_SetHighLevel(port.loc, port.pin, ENABLE);
	}
	this->status = State;
	return this->status;
}

RELAY_ST Relay::Relay_GetSta(void) {
	return this->status;
}



//================ END OF FILE ==============================================//

