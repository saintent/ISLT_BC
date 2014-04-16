//================ File Desciption =========================================//
//=== File name : Valve.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "Valve.h"
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

Valve::Valve(void) {

}

Valve::Valve(RELAY_PORT left, RELAY_PORT right) {
	// TODO Auto-generated constructor stub
	Relay rTemp(left);
	Relay lTemp(right);
	this->vLeft = lTemp;
	this->vRight = rTemp;
	this->vDir = VALVE_ALL;
}

Valve::~Valve() {
	// TODO Auto-generated destructor stub
}

void Valve::Init(RELAY_PORT left, RELAY_PORT right) {
	Relay rTemp(left);
	Relay lTemp(right);
	this->vLeft = lTemp;
	this->vRight = rTemp;
	this->vDir = VALVE_ALL;
}

VALVE_DIR Valve::Valve_Control(VALVE_DIR dir) {
	if (dir == VALVE_ALL) {
		vLeft.Relay_Control(RELAY_ON);
		vRight.Relay_Control(RELAY_ON);
	}
	else if (dir == VALVE_LEFT) {
		vLeft.Relay_Control(RELAY_ON);
		vRight.Relay_Control(RELAY_OFF);
	}
	else {
		vLeft.Relay_Control(RELAY_OFF);
		vRight.Relay_Control(RELAY_ON);
	}
	this->vDir = dir;
	return this->vDir;
}

VALVE_DIR Valve::GetStatus(void) {
	return this->vDir;
}

//================ END OF FILE ==============================================//

