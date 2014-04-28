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
	SSP_init.ClockRate = 250000;
	SSP_Init(&SSP_init);

	//NVIC_EnableIRQ(SSP_IRQn);

	SSP_Cmd (ENABLE);
}

uint8_t SSPSend(uint8_t data) {
	uint8_t dataReturn;

	/* Clear Rx FIFO */
	while (SSP_GetStatus(SSP_STATUS_RNE ) == SET)
		dataReturn = SSP_ReceiveData();

	/* Wait if Tx FIFO is not empty */
	while (SSP_GetStatus(SSP_STATUS_TFE ) == RESET);

	SSP_SendData(data);
	while (SSP_GetStatus(SSP_STATUS_BSY ) == SET);

	/* Wait to receive a byte */
	while (SSP_GetStatus(SSP_STATUS_RNE ) == RESET);

	/* Return the byte read from the SSP bus */
	dataReturn = (uint8_t) SSP_ReceiveData();

	return (dataReturn);
}

void SSPRecvBlockFIFO (uint8_t *pBuffer, uint32_t BufferLength)
{
	uint32_t startcnt, i;
    uint8_t recvByte;

	if ( BufferLength < 4 ) startcnt = BufferLength;
    else                       startcnt = 4;

    /* fill TX FIFO, prepare clk for receive */
	for ( i = startcnt; i; i-- )
        SSP_SendData (0x00);

    i = 0;
    do {
        while (SSP_GetStatus(SSP_STATUS_RNE) == RESET);
        recvByte = (uint8_t)SSP_ReceiveData ();

        /* fill TX FIFO, prepare clk for receive */
        if ( i < ( BufferLength - startcnt ) )
            SSP_SendData (0x00);

        *pBuffer++ = recvByte;
        i++;
    } while ( i < BufferLength );

}

/*void SSP_IRQHandler(void)
{
	uint32_t i, int_status_reg;

	int_status_reg = SSP_GetIntStatusReg ();

     Receive overrun interrupt
    if (int_status_reg & SSP_INT_RXOVERRUN)
    {
        SSP_ClearIntPendingBit (SSP_INT_RXOVERRUN);
    }

     Receive timeout interrupt
    if (int_status_reg & SSP_INT_RXTIMEOUT)
    {
        SSP_ClearIntPendingBit (SSP_INT_RXTIMEOUT);
    }

     Receive halffull interrupt
	if (int_status_reg & SSP_INT_RXHALFFULL) {

	}

	 Receive timeout interrupt
	if (int_status_reg & SSP_INT_TXHALFEMPTY) {

	}
}*/
//================ END OF FILE ==============================================//
