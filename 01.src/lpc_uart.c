//================ File Despciption =========================================//
//=== File name : lpc_uart.c
//===========================================================================//



//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "lpc12xx.h"
#include "lpc_uart.h"
#include "lpc12xx_iocon.h"
#include "lpc12xx_sysctrl.h"
#include "lpc12xx_uart.h"
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
DataCallback_t* datacb[UART_MAX];
SendCmp_t* sndCmpcb[UART_MAX];
ChTimeout_t* chTmcb[UART_MAX];
//================ PRIVATE DEFINE ===========================================//
extern void UART0_IRQHandler(void);
extern void UART1_IRQHandler(void);
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//

void SetupUART_Location(Uart_type portNum, uint32_t location)
{
  if ( portNum == 0 )
  {
	if ( location == 0 )
	{
	  LPC_IOCON->PIO0_1 &= ~0x07;    /* UART0 I/O config */
	  LPC_IOCON->PIO0_1 |= 0x02;     /* UART0 RXD LOC0 */
	  LPC_IOCON->PIO0_2 &= ~0x07;
	  LPC_IOCON->PIO0_2 |= 0x02;     /* UART0 TXD LOC0 */
	}
	else if ( location == 1 )
	{
	  LPC_IOCON->PIO2_1 &= ~0x07;    /* UART0 I/O config */
	  LPC_IOCON->PIO2_1 |= 0x04;     /* UART0 RXD LOC1 */
	  LPC_IOCON->PIO2_2 &= ~0x07;
	  LPC_IOCON->PIO2_2 |= 0x04;     /* UART0 TXD LOC1 */
	}
  }
  else if ( portNum == 1 )
  {
	if ( location == 0 )
	{
	  LPC_IOCON->PIO0_8 &= ~0x07;    /* UART1 I/O config */
	  LPC_IOCON->PIO0_8 |= 0x02;     /* UART1 RXD LOC0 */
	  LPC_IOCON->PIO0_9 &= ~0x07;
	  LPC_IOCON->PIO0_9 |= 0x02;     /* UART1 TXD LOC0 */
	}
	else if ( location == 1 )
	{
	  LPC_IOCON->PIO2_11 &= ~0x07;    /* UART1 I/O config */
	  LPC_IOCON->PIO2_11 |= 0x05;     /* UART RXD LOC1 */
	  LPC_IOCON->PIO2_10 &= ~0x07;
	  LPC_IOCON->PIO2_10 |= 0x05;     /* UART TXD LOC1 */
	}
	else
	{
	  LPC_IOCON->PIO2_12 &= ~0x07;    /* UART1 I/O config */
	  LPC_IOCON->PIO2_12 |= 0x03;     /* UART RXD LOC1 */
	  LPC_IOCON->PIO2_13 &= ~0x07;
	  LPC_IOCON->PIO2_13 |= 0x03;     /* UART TXD LOC1 */
	}
  }
  else
  {
	while ( 1 );
  }
  return;
}

void UARTInit(Uart_type portNum, uint32_t baudrate, uint8_t loc) {
	LPC_UART0_TypeDef* uptr;
	IRQn_Type type;
	uint32_t Fdiv;
	uint32_t clkDiv;
	// Getting memory access of uart
	if(portNum == UART0) {
		uptr = (LPC_UART0_TypeDef*)LPC_UART0_BASE;
		SYS_ResetPeripheral(SYS_PRESETCTRL_UART0_RST, DISABLE);
		SYS_ConfigAHBCLK(SYS_AHBCLKCTRL_UART0, ENABLE);
		SYS_SetUART0ClockDiv(1);
		clkDiv = LPC_SYSCON->UART0CLKDIV;
		type = UART0_IRQn;
	}
	else if (portNum == UART1) {
		uptr = (LPC_UART0_TypeDef*)LPC_UART1_BASE;
		SYS_ResetPeripheral(SYS_PRESETCTRL_UART1_RST, DISABLE);
		SYS_ConfigAHBCLK(SYS_AHBCLKCTRL_UART1, ENABLE);
		SYS_SetUART1ClockDiv(1);
		clkDiv = LPC_SYSCON->UART1CLKDIV;
		type = UART1_IRQn;

	}
	SetupUART_Location(portNum, loc);
	uptr->LCR = UART_CFG_DATABIT_8
			| UART_CFG_STOPBIT_1
			| UART_CFG_PARITY_NONE
			| UART_CFG_TXDBREAK_DISABLE
			| UART_DLAB_BIT;
	Fdiv = ((SystemCoreClock/clkDiv)/16)/baudrate ;	/*baud rate */

	uptr->DLM = Fdiv / 256;
	uptr->DLL = Fdiv % 256;
	uptr->LCR &= ~UART_DLAB_BIT; 		// Clear DLAB bit

	// Interrupt Enable RX and TX
	uptr->IER = UART_INTCFG_RBR
			//| UART_INTCFG_THRE
			| UART_INTCFG_RLS;

	/* Enable the UART Interrupt */
	NVIC_EnableIRQ(type);
}

void UART_RS485Init(void) {
	LPC_IOCON->PIO0_0 &= ~0x07;
	LPC_IOCON->PIO0_0 |= 0x02;
	LPC_UART0->RS485CTRL |= UART_RS485_AUTO_DIRECTION_ENABLE
			| UART_RS485_DIRPIN_HIGH;
}

void UARTRegDataCb(Uart_type portNum, DataCallback_t* cb) {
	datacb[portNum] = cb;
}
void UARTRegSendCmp(Uart_type portNum, SendCmp_t* cb) {
	sndCmpcb[portNum] = cb;
}
void UARTRegChTimeOut(Uart_type portNum, ChTimeout_t* cb) {
	chTmcb[portNum] = cb;
}
void UARTUnRegDataCb(Uart_type portNum) {
	datacb[portNum] = 0;
}
void UARTUnRegSendCmpCb(Uart_type portNum) {
	sndCmpcb[portNum] = 0;
}
void UARTUnRegChTimeOutCb(Uart_type portNum) {
	chTmcb[portNum] = 0;
}

void UARTSetupDMA(Uart_type portNum, FunctionalState state) {
	LPC_UART0_TypeDef* uptr;
	// Getting memory access of uart
	if (portNum == UART0) {
		uptr = (LPC_UART0_TypeDef*) LPC_UART0_BASE;
	} else if (portNum == UART1) {
		uptr = (LPC_UART0_TypeDef*) LPC_UART1_BASE;
	}
	if(state == ENABLE) {
		uptr->FCR |= UART_CFG_DMAMODE_ENABLE;
	}
	else {
		uptr->FCR &= ~UART_CFG_DMAMODE_DISABLE;
	}
}

void UART_StartSent(Uart_type portNumber) {
	LPC_UART0_TypeDef* uptr;
	if (portNumber == UART0) {
		uptr = (LPC_UART0_TypeDef*) LPC_UART0_BASE;
	}
	else {
		uptr = (LPC_UART0_TypeDef*) LPC_UART1_BASE;
	}
	uptr->IER |= UART_INTCFG_THRE;

}
void UART_StopSent(Uart_type portNumber) {
	LPC_UART0_TypeDef* uptr;
	if (portNumber == UART0) {
		uptr = (LPC_UART0_TypeDef*) LPC_UART0_BASE;
	} else {
		uptr = (LPC_UART0_TypeDef*) LPC_UART1_BASE;
	}
	uptr->IER &= ~(UART_INTCFG_THRE);
}

Status UARTSendCh(Uart_type portNum, uint8_t ch) {
	Status st = ERROR;
	if(LPC_UART0->LSR & UART_LS_TX_EMPTY) {
		LPC_UART0->THR = ch;
		st = SUCCESS;
	}
	return st;
}

void UART0_IRQHandler(void) {
	uint8_t IIRValue;
	uint8_t Dummy = Dummy;

	IIRValue = LPC_UART0->IIR;
	switch (IIRValue & 0x0E) {
		case UART_INTSTAT_RX_LINE_STAT :
			Dummy = LPC_UART1->RBR;
		break;
		case UART_INTSTAT_RX_DATA_AVAILABLE:
			if (datacb[UART0] != 0) {
				datacb[UART0](UART0, LPC_UART0 ->RBR);
			}
		break;
		case UART_INTSTAT_RX_TIMEOUT:
			if (chTmcb[UART0] != 0) {
				chTmcb[UART0](UART0);
			}
		break;
		case UART_INTSTAT_TX_READY :
			if (sndCmpcb[UART0] !=0) {
				sndCmpcb[UART0](UART0);
			}
		break;
	}

}
void UART1_IRQHandler(void) {
	uint8_t IIRValue;
	uint8_t Dummy = Dummy;

	IIRValue = LPC_UART1->IIR;
	switch (IIRValue & 0x0E) {
		case UART_INTSTAT_RX_LINE_STAT :
			Dummy = LPC_UART1->RBR;
		break;
		case UART_INTSTAT_RX_DATA_AVAILABLE:
			if (datacb[UART1] != 0) {
				datacb[UART1](UART1, LPC_UART1 ->RBR);
			}
		break;
		case UART_INTSTAT_RX_TIMEOUT:
			if (chTmcb[UART1] != 0) {
				chTmcb[UART1](UART1);
			}
		break;
		case UART_INTSTAT_TX_READY :
			if (sndCmpcb[UART1] !=0) {
				sndCmpcb[UART1](UART1);
			}
		break;
		}
}
//================ END OF FILE ==============================================//
