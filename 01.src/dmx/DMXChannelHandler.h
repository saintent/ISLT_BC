//================ File Description =========================================//
//=== File name : DMXChannelHandler.h
//===========================================================================//

#ifndef DMXCHANNELHANDLER_H_
#define DMXCHANNELHANDLER_H_

//================ Include Header ===========================================//
#include "lpc_types.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
//
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
//
//================ EXTERN FUNCTION ==========================================//
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
//================ EXTERN FUNCTION POINTER ==================================//
//
//================ EXTERN VARIABLE ==========================================//
//
//================ EXTERN QUEUE =============================================//
//
//================ CLASS DECLARATION ========================================//
namespace dmx {

class DMXChannelHandler {
public:
	DMXChannelHandler(uint16_t channel, void* context);
	virtual ~DMXChannelHandler();

	uint16_t GetChannel(void);

	uint8_t SetValue(uint8_t value);
	uint8_t GetValue(void);

	uint8_t IsUpdate(void);

	void* context;

private:
	uint16_t channel;
	uint8_t value;
	uint8_t lastValue;
};

} /* namespace dmx */
//================ END OF FILE ==============================================//
#endif /* DMXCHANNELHANDLER_H_ */
