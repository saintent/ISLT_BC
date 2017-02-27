//================ File Desciption =========================================//
//=== File name : DMXChannelHandler.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "DMXChannelHandler.h"
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
namespace dmx {

DMXChannelHandler::DMXChannelHandler(uint16_t channel, void* context) {
	// TODO Auto-generated constructor stub
	this->channel = channel;
	this->context = context;
}

DMXChannelHandler::~DMXChannelHandler() {
	// TODO Auto-generated destructor stub
}

uint16_t DMXChannelHandler::GetChannel(void) {
	return this->channel;
}

uint8_t DMXChannelHandler::SetValue(uint8_t value) {
	this->value = value;
	return 0;
}

uint8_t DMXChannelHandler::GetValue(void) {
	return this->value;
}

uint8_t DMXChannelHandler::IsUpdate(void) {
	uint8_t result;
	if (this->lastValue != this->value) {
		result = 1;

	} else {
		result = 0;
	}
	this->lastValue = this->value;
	return result;
}

} /* namespace dmx */
//================ END OF FILE ==============================================//

