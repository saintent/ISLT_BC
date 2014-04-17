//================ File Description =========================================//
//=== File name : ssp.c
//===========================================================================//



//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "lpc12xx.h"
#include "ssp.h"
#include "lpc12xx_iocon.h"
#include "lpc12xx_sysctrl.h"
#include "lpc12xx_ssp.h"
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
void SSPInit(void) {
	IOCON_PIO_CFG_Type PIO_config;
	SSP_InitTypeDef	SSP_init;

	IOCON_StructInit(&PIO_config);
	PIO_config.type = IOCON_SSP_SCK;
	IOCON_SetFunc(&PIO_config);
	PIO_config.type = IOCON_SSP_SSEL;
	IOCON_SetFunc(&PIO_config);
	PIO_config.type = IOCON_SSP_MISO;
	IOCON_SetFunc(&PIO_config);
	PIO_config.type = IOCON_SSP_MOSI;
	IOCON_SetFunc(&PIO_config);

	SSP_init.DataSize = SSP_DATASIZE_8;
	SSP_init.FrameFormat = SSP_FRAMEFORMAT_SPI;
	SSP_init.CPOL = SSP_CPOL_HIGH;
	SSP_init.CPHA = SSP_CPHA_SECOND;
	SSP_init.Mode = SSP_MODE_MASTER;
	SSP_init.ClockRate = 1000000;
	SSP_Init(&SSP_init);

	SSP_Cmd (ENABLE);
}
//================ END OF FILE ==============================================//
