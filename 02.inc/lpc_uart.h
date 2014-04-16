//================ File Despciption =========================================//
//=== File name : lpc_uart.h
//===========================================================================//


#ifndef LPC_UART_H_
#define LPC_UART_H_


//================ Include Header ===========================================//
#include "lpc12xx.h"
#include "lpc_types.h"
//================ PULBIC DEFINE ============================================//
#define UART_DLAB_BIT			((uint8_t)(1 << 7))	 /* UART 5 bit data mode */
// UART_databit_type definitions
#define UART_CFG_DATABIT_5   	((uint8_t)(0))       /* UART 5 bit data mode */
#define UART_CFG_DATABIT_6   	((uint8_t)(1<<0))    /* UART 6 bit data mode */
#define UART_CFG_DATABIT_7   	((uint8_t)(2<<0))    /* UART 7 bit data mode */
#define UART_CFG_DATABIT_8   	((uint8_t)(3<<0))    /* UART 8 bit data mode */
// UART_stopbit_type definitions
#define UART_CFG_STOPBIT_1   	((uint8_t)(0))      /* UART 1 Stop Bits Select */
#define UART_CFG_STOPBIT_2   	((uint8_t)(1<<2))   /* UART Two Stop Bits Select */
// UART_parity_type definitions
#define UART_CFG_PARITY_NONE   ((uint8_t)(0))     /* No parity */
#define UART_CFG_PARITY_ODD    ((uint8_t)(1<<3))  /* Odd parity */
#define UART_CFG_PARITY_EVEN   ((uint8_t)(3<<3))  /* Even parity */
#define UART_CFG_PARITY_MARK   ((uint8_t)(5<<3))  /* Forced "1" stick parity */
#define UART_CFG_PARITY_SPACE  ((uint8_t)(7<<3))  /* Forced "0" stick parity */
// FIFO_level_type definitions
#define UART_CFG_FIFOTRG_1   ((uint8_t)(0))    /* UART FIFO trigger level 0: 1 char */
#define UART_CFG_FIFOTRG_4   ((uint8_t)(1<<6)) /* UART FIFO trigger level 1: 4 char */
#define UART_CFG_FIFOTRG_8   ((uint8_t)(2<<6)) /* UART FIFO trigger level 2: 8 char */
#define UART_CFG_FIFOTRG_14  ((uint8_t)(3<<6)) /* UART FIFO trigger level 3: 14 char */
// FIFO_dmamode_type definitions
#define UART_CFG_DMAMODE_DISABLE  ((uint8_t)(0))     /* UART DMA mode disable */
#define UART_CFG_DMAMODE_ENABLE   ((uint8_t)(1<<3))  /* UART DMA mode enable  */
// TXD_break_forced_type definitions
#define UART_CFG_TXDBREAK_DISABLE  ((uint8_t)(0))      /* TXD Break disable */
#define UART_CFG_TXDBREAK_ENABLE   ((uint8_t)(1<<6))   /* TXD Break enable */
// UART_interrupt_type definitions
#define UART_INTCFG_RBR  ((uint16_t)(1<<0)) /* RBR Interrupt enable*/
#define UART_INTCFG_THRE ((uint16_t)(1<<1)) /* THR Interrupt enable*/
#define UART_INTCFG_RLS  ((uint16_t)(1<<2)) /* RX line status interrupt enable*/
#define UART_MSINT_EN    ((uint32_t)(1<<3)) /*!< Modem status interrupt enable */
#define UART_CTSINT_EN   ((uint32_t)(1<<7)) /*!< CTS1 signal transition interrupt enable */
#define UART_INTCFG_ABEO ((uint16_t)(1<<8)) /* the end of auto-baud interrupt */
#define UART_INTCFG_ABTO ((uint16_t)(1<<9)) /* the auto-baud time-out interrupt */
// UART_int_status_type definitions
#define UART_INTSTAT_PEND              ((uint16_t)(1<<0))  /* Active low */
#define UART_INTSTAT_RX_LINE_STAT      ((uint16_t)(3<<1))  /* Receive line status*/
#define UART_INTSTAT_RX_DATA_AVAILABLE ((uint16_t)(2<<1))  /* Receive data available*/
#define UART_INTSTAT_RX_TIMEOUT        ((uint16_t)(6<<1))  /* Character time-out indicator*/
#define UART_INTSTAT_TX_READY          ((uint16_t)(1<<1))  /* THRE interrupt*/
#define UART_INTSTAT_MODEM             ((uint16_t)(0<<1))  /* Modem interrupt*/
#define UART_INTSTAT_AUTO_END          ((uint16_t)(1<<8))  /* End of auto-baud interrupt */
#define UART_INTSTAT_AUTO_TIMOUT       ((uint16_t)(1<<9))  /* Autobaud time-out interrupt */
// UART_line_status_type definitions
#define UART_LS_RX_DATA_READY ((uint8_t)(1<<0)) /*Receive data ready*/
#define UART_LS_RX_FIFO_FULL  ((uint8_t)(1<<1)) /*Overrun error: RX fifo overflow*/
#define UART_LS_PARITY_ERROR  ((uint8_t)(1<<2)) /*Parity error*/
#define UART_LS_FRAMING_ERROR ((uint8_t)(1<<3)) /*Framing error*/
#define UART_LS_BREAK_INT     ((uint8_t)(1<<4)) /*Break interrupt*/
#define UART_LS_TX_EMPTY      ((uint8_t)(1<<5)) /*Transmit holding register THR empty*/
#define UART_LS_TX_EMPTY_ALL  ((uint8_t)(1<<6)) /*Transmitter THR and TSR all empty*/
#define UART_LS_RX_FIFO_ERROR ((uint8_t)(1<<7)) /*Error in RX FIFO or others*/
// UART_auto_baudrate_mode_type definitions
#define UART_CFG_AUTOBAUD_MODE0  ((uint8_t)(0))     /* UART Auto baudrate Mode 0 */
#define UART_CFG_AUTOBAUD_MODE1  ((uint8_t)(1<<1)) /* UART Auto baudrate Mode 1 */
// UART_auto_baudrate_restart_type definitions
#define UART_CFG_ABRESTART_DISABLE  ((uint8_t)(0))     /* Auto baudrate disable */
#define UART_CFG_ABRESTART_ENABLE   ((uint8_t)(1<<2)) /* Auto baudrate enable */
// UART_modem_status_type definitions
#define UART_MODEM_STAT_DELTA_CTS  ((uint8_t)(1<<0))   /* State change of CTS */
#define UART_MODEM_STAT_DELTA_DSR  ((uint8_t)(1<<1))   /* State change of DSR */
#define UART_MODEM_STAT_LO2HI_RI   ((uint8_t)(1<<2))   /* Low to high transition of RI */
#define UART_MODEM_STAT_DELTA_DCD  ((uint8_t)(1<<3))   /* State change of input DCD */
#define UART_MODEM_STAT_CTS        ((uint8_t)(1<<4))   /* Clear To Send State */
#define UART_MODEM_STAT_DSR        ((uint8_t)(1<<5))   /* Data Set Ready State */
#define UART_MODEM_STAT_RI         ((uint8_t)(1<<6))   /* Ring Indicator State */
#define UART_MODEM_STAT_DCD        ((uint8_t)(1<<7))   /* Data Carrier Detect State */
// UART_modem_DTR_pin_type definitions
#define UART_MODEM_PIN_DTR_DISABLE ((uint8_t)(0))
#define UART_MODEM_PIN_DTR_ENABLE  ((uint8_t)(1<<0)) /* Modem output pin DTR */
// UART_modem_RTS_pin_type definitions
#define UART_MODEM_PIN_RTS_DISABLE ((uint8_t)(0))
#define UART_MODEM_PIN_RTS_ENABLE  ((uint8_t)(1<<1)) /* Modem output pin RTS */
// UART_modem_loopback_type definitions
#define UART_MODEM_MODE_LOOPBACK_DISABLE ((uint8_t)(0))
#define UART_MODEM_MODE_LOOPBACK_ENABLE  ((uint8_t)(1<<4))  /* Loop back mode select */
// UART_modem_autoRTS_type definitions
#define UART_MODEM_MODE_AUTO_RTS_DISABLE ((uint8_t)(0))
#define UART_MODEM_MODE_AUTO_RTS_ENABLE  ((uint8_t)(1<<6)) /* Enable AutoRTS flow-control*/
// UART_modem_autoCTS_type definitions
#define UART_MODEM_MODE_AUTO_CTS_DISABLE ((uint8_t)(0))
#define UART_MODEM_MODE_AUTO_CTS_ENABLE  ((uint8_t)(1<<7)) /* Enable AutoCTS flow-control*/
// UART_485_nmm_type definitions
#define UART_RS485_NORMAL_MULTIDROP_DISABLE ((uint8_t)(0))
#define UART_RS485_NORMAL_MULTIDROP_ENABLE  ((uint8_t)(1<<0))  /* Normal Multi-drop Mode */
// UART_485_receiver_type definitions
#define UART_RS485_RECEIVER_ENABLE   ((uint8_t)(0))
#define UART_RS485_RECEIVER_DISABLE  ((uint8_t)(1<<1))  /* The receiver is disabled */
// UART_485_add_type definitions
#define UART_RS485_AUTO_ADDRRESS_DISABLE ((uint8_t)(0))
#define UART_RS485_AUTO_ADDRRESS_ENABLE  ((uint8_t)(1<<2))  /* Auto Address Detect */
// UART_485_dirpin_ctrl_type definitions
#define UART_RS485_DIRPIN_RTS  ((uint8_t)(0))
#define UART_RS485_DIRPIN_DTR  ((uint8_t)(1<<3))  /* Direction control */
// UART_485_autodir_type definitions
#define UART_RS485_AUTO_DIRECTION_DISABLE ((uint8_t)(0))
#define UART_RS485_AUTO_DIRECTION_ENABLE  ((uint8_t)(1<<4)) /* Auto Direction Control */
// UART_485_dirpin_level_type definitions
#define UART_RS485_DIRPIN_LOW   ((uint8_t)(0))
#define UART_RS485_DIRPIN_HIGH  ((uint8_t)(1<<5))  /* Reverses the polarity */
// UART_irda_mode_type definitions
#define UART_IRDA_DISABLE       ((uint8_t)(0))    /* IrDA mode disable */
#define UART_IRDA_ENABLE        ((uint8_t)(1<<0)) /* IrDA mode enable */
// UART_irda_input_invert_type definitions
#define UART_IRDA_INPUTINVERT_NOT   ((uint8_t)(0))    /* IrDA serial not input inverted */
#define UART_IRDA_INPUTINVERTED     ((uint8_t)(1<<1)) /* IrDA serial input inverted */
// UART_irda_fixed_pulse_type definitions
#define UART_IRDA_PULSEDIV_NORMAL     ((uint8_t)(0))
#define UART_IRDA_PULSEDIV_2          ((uint8_t)(0x1<<2))
#define UART_IRDA_PULSEDIV_4          ((uint8_t)(0x3<<2))
#define UART_IRDA_PULSEDIV_8          ((uint8_t)(0x5<<2))
#define UART_IRDA_PULSEDIV_16         ((uint8_t)(0x7<<2))
#define UART_IRDA_PULSEDIV_32         ((uint8_t)(0x9<<2))
#define UART_IRDA_PULSEDIV_64         ((uint8_t)(0xB<<2))
#define UART_IRDA_PULSEDIV_128        ((uint8_t)(0xD<<2))
#define UART_IRDA_PULSEDIV_256        ((uint8_t)(0xF<<2))
// UART_process_time_delay definitions
#define UART_BLOKING_TIMEOUT          (0xFFFFFFFFUL)
#define UART_NO_TIMEOUT               (0x1UL)

//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	UART0 = 0,
	UART1,
	UART_MAX
}Uart_type;

typedef enum {
	UART_LOC_0 = 0,
	UART_LOC_1
}UART_LOC_T;


//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
typedef void DataCallback_t(Uart_type t, uint8_t data);
typedef void SendCmp_t(Uart_type t);
typedef void ChTimeout_t(Uart_type t);
//================ EXTERN FUNCTION ==========================================//
#ifdef __cplusplus
extern "C"
{
#endif

void SetupUART_Location(Uart_type portNum, uint32_t location);
void UARTInit(Uart_type portNum, uint32_t baudrate, uint8_t loc);
void UART_RS485Init(void);
void UARTRegDataCb(Uart_type portNum, DataCallback_t* cb);
void UARTRegSendCmp(Uart_type portNum, SendCmp_t* cb);
void UARTRegChTimeOut(Uart_type portNum, ChTimeout_t* cb);
void UARTUnRegDataCb(Uart_type portNum);
void UARTUnRegSendCmpCb(Uart_type portNum);
void UARTUnRegChTimeOutCb(Uart_type portNum);
void UARTSetupDMA(Uart_type portNum, FunctionalState state);

void UART_StartSent(Uart_type portNumber);
void UART_StopSent(Uart_type portNumber);
Status UARTSendCh(Uart_type portNum, uint8_t ch);

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


//================ END OF FILE ==============================================//
#endif /* LPC_UART_H_ */
