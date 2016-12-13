/*******************************************************************************
* File Name: modbus_485_interface.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_modbus_485_interface_H)
#define CY_UART_modbus_485_interface_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

#define modbus_485_interface_RX_ENABLED                     (1u)
#define modbus_485_interface_TX_ENABLED                     (1u)
#define modbus_485_interface_HD_ENABLED                     (0u)
#define modbus_485_interface_RX_INTERRUPT_ENABLED           (1u)
#define modbus_485_interface_TX_INTERRUPT_ENABLED           (1u)
#define modbus_485_interface_INTERNAL_CLOCK_USED            (1u)
#define modbus_485_interface_RXHW_ADDRESS_ENABLED           (0u)
#define modbus_485_interface_OVER_SAMPLE_COUNT              (16u)
#define modbus_485_interface_PARITY_TYPE                    (0u)
#define modbus_485_interface_PARITY_TYPE_SW                 (0u)
#define modbus_485_interface_BREAK_DETECT                   (0u)
#define modbus_485_interface_BREAK_BITS_TX                  (13u)
#define modbus_485_interface_BREAK_BITS_RX                  (13u)
#define modbus_485_interface_TXCLKGEN_DP                    (1u)
#define modbus_485_interface_USE23POLLING                   (1u)
#define modbus_485_interface_FLOW_CONTROL                   (0u)
#define modbus_485_interface_CLK_FREQ                       (0u)
#define modbus_485_interface_TX_BUFFER_SIZE                 (16u)
#define modbus_485_interface_RX_BUFFER_SIZE                 (16u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(modbus_485_interface_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define modbus_485_interface_CONTROL_REG_REMOVED            (0u)
#else
    #define modbus_485_interface_CONTROL_REG_REMOVED            (1u)
#endif /* End modbus_485_interface_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct modbus_485_interface_backupStruct_
{
    uint8 enableState;

    #if(modbus_485_interface_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End modbus_485_interface_CONTROL_REG_REMOVED */

} modbus_485_interface_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void modbus_485_interface_Start(void) ;
void modbus_485_interface_Stop(void) ;
uint8 modbus_485_interface_ReadControlRegister(void) ;
void modbus_485_interface_WriteControlRegister(uint8 control) ;

void modbus_485_interface_Init(void) ;
void modbus_485_interface_Enable(void) ;
void modbus_485_interface_SaveConfig(void) ;
void modbus_485_interface_RestoreConfig(void) ;
void modbus_485_interface_Sleep(void) ;
void modbus_485_interface_Wakeup(void) ;

/* Only if RX is enabled */
#if( (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) )

    #if (modbus_485_interface_RX_INTERRUPT_ENABLED)
        #define modbus_485_interface_EnableRxInt()  CyIntEnable (modbus_485_interface_RX_VECT_NUM)
        #define modbus_485_interface_DisableRxInt() CyIntDisable(modbus_485_interface_RX_VECT_NUM)
        CY_ISR_PROTO(modbus_485_interface_RXISR);
    #endif /* modbus_485_interface_RX_INTERRUPT_ENABLED */

    void modbus_485_interface_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void modbus_485_interface_SetRxAddress1(uint8 address) ;
    void modbus_485_interface_SetRxAddress2(uint8 address) ;

    void  modbus_485_interface_SetRxInterruptMode(uint8 intSrc) ;
    uint8 modbus_485_interface_ReadRxData(void) ;
    uint8 modbus_485_interface_ReadRxStatus(void) ;
    uint8 modbus_485_interface_GetChar(void) ;
    uint16 modbus_485_interface_GetByte(void) ;
    uint8 modbus_485_interface_GetRxBufferSize(void)
                                                            ;
    void modbus_485_interface_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define modbus_485_interface_GetRxInterruptSource   modbus_485_interface_ReadRxStatus

#endif /* End (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) */

/* Only if TX is enabled */
#if(modbus_485_interface_TX_ENABLED || modbus_485_interface_HD_ENABLED)

    #if(modbus_485_interface_TX_INTERRUPT_ENABLED)
        #define modbus_485_interface_EnableTxInt()  CyIntEnable (modbus_485_interface_TX_VECT_NUM)
        #define modbus_485_interface_DisableTxInt() CyIntDisable(modbus_485_interface_TX_VECT_NUM)
        #define modbus_485_interface_SetPendingTxInt() CyIntSetPending(modbus_485_interface_TX_VECT_NUM)
        #define modbus_485_interface_ClearPendingTxInt() CyIntClearPending(modbus_485_interface_TX_VECT_NUM)
        CY_ISR_PROTO(modbus_485_interface_TXISR);
    #endif /* modbus_485_interface_TX_INTERRUPT_ENABLED */

    void modbus_485_interface_SetTxInterruptMode(uint8 intSrc) ;
    void modbus_485_interface_WriteTxData(uint8 txDataByte) ;
    uint8 modbus_485_interface_ReadTxStatus(void) ;
    void modbus_485_interface_PutChar(uint8 txDataByte) ;
    void modbus_485_interface_PutString(const char8 string[]) ;
    void modbus_485_interface_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void modbus_485_interface_PutCRLF(uint8 txDataByte) ;
    void modbus_485_interface_ClearTxBuffer(void) ;
    void modbus_485_interface_SetTxAddressMode(uint8 addressMode) ;
    void modbus_485_interface_SendBreak(uint8 retMode) ;
    uint8 modbus_485_interface_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define modbus_485_interface_PutStringConst         modbus_485_interface_PutString
    #define modbus_485_interface_PutArrayConst          modbus_485_interface_PutArray
    #define modbus_485_interface_GetTxInterruptSource   modbus_485_interface_ReadTxStatus

#endif /* End modbus_485_interface_TX_ENABLED || modbus_485_interface_HD_ENABLED */

#if(modbus_485_interface_HD_ENABLED)
    void modbus_485_interface_LoadRxConfig(void) ;
    void modbus_485_interface_LoadTxConfig(void) ;
#endif /* End modbus_485_interface_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_modbus_485_interface) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    modbus_485_interface_CyBtldrCommStart(void) CYSMALL ;
    void    modbus_485_interface_CyBtldrCommStop(void) CYSMALL ;
    void    modbus_485_interface_CyBtldrCommReset(void) CYSMALL ;
    cystatus modbus_485_interface_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus modbus_485_interface_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_modbus_485_interface)
        #define CyBtldrCommStart    modbus_485_interface_CyBtldrCommStart
        #define CyBtldrCommStop     modbus_485_interface_CyBtldrCommStop
        #define CyBtldrCommReset    modbus_485_interface_CyBtldrCommReset
        #define CyBtldrCommWrite    modbus_485_interface_CyBtldrCommWrite
        #define CyBtldrCommRead     modbus_485_interface_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_modbus_485_interface) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define modbus_485_interface_BYTE2BYTE_TIME_OUT (25u)
    #define modbus_485_interface_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define modbus_485_interface_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define modbus_485_interface_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define modbus_485_interface_SET_SPACE      (0x00u)
#define modbus_485_interface_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (modbus_485_interface_TX_ENABLED) || (modbus_485_interface_HD_ENABLED) )
    #if(modbus_485_interface_TX_INTERRUPT_ENABLED)
        #define modbus_485_interface_TX_VECT_NUM            (uint8)modbus_485_interface_TXInternalInterrupt__INTC_NUMBER
        #define modbus_485_interface_TX_PRIOR_NUM           (uint8)modbus_485_interface_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* modbus_485_interface_TX_INTERRUPT_ENABLED */

    #define modbus_485_interface_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define modbus_485_interface_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define modbus_485_interface_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(modbus_485_interface_TX_ENABLED)
        #define modbus_485_interface_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (modbus_485_interface_HD_ENABLED) */
        #define modbus_485_interface_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (modbus_485_interface_TX_ENABLED) */

    #define modbus_485_interface_TX_STS_COMPLETE            (uint8)(0x01u << modbus_485_interface_TX_STS_COMPLETE_SHIFT)
    #define modbus_485_interface_TX_STS_FIFO_EMPTY          (uint8)(0x01u << modbus_485_interface_TX_STS_FIFO_EMPTY_SHIFT)
    #define modbus_485_interface_TX_STS_FIFO_FULL           (uint8)(0x01u << modbus_485_interface_TX_STS_FIFO_FULL_SHIFT)
    #define modbus_485_interface_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << modbus_485_interface_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (modbus_485_interface_TX_ENABLED) || (modbus_485_interface_HD_ENABLED)*/

#if( (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) )
    #if(modbus_485_interface_RX_INTERRUPT_ENABLED)
        #define modbus_485_interface_RX_VECT_NUM            (uint8)modbus_485_interface_RXInternalInterrupt__INTC_NUMBER
        #define modbus_485_interface_RX_PRIOR_NUM           (uint8)modbus_485_interface_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* modbus_485_interface_RX_INTERRUPT_ENABLED */
    #define modbus_485_interface_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define modbus_485_interface_RX_STS_BREAK_SHIFT             (0x01u)
    #define modbus_485_interface_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define modbus_485_interface_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define modbus_485_interface_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define modbus_485_interface_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define modbus_485_interface_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define modbus_485_interface_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define modbus_485_interface_RX_STS_MRKSPC           (uint8)(0x01u << modbus_485_interface_RX_STS_MRKSPC_SHIFT)
    #define modbus_485_interface_RX_STS_BREAK            (uint8)(0x01u << modbus_485_interface_RX_STS_BREAK_SHIFT)
    #define modbus_485_interface_RX_STS_PAR_ERROR        (uint8)(0x01u << modbus_485_interface_RX_STS_PAR_ERROR_SHIFT)
    #define modbus_485_interface_RX_STS_STOP_ERROR       (uint8)(0x01u << modbus_485_interface_RX_STS_STOP_ERROR_SHIFT)
    #define modbus_485_interface_RX_STS_OVERRUN          (uint8)(0x01u << modbus_485_interface_RX_STS_OVERRUN_SHIFT)
    #define modbus_485_interface_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << modbus_485_interface_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define modbus_485_interface_RX_STS_ADDR_MATCH       (uint8)(0x01u << modbus_485_interface_RX_STS_ADDR_MATCH_SHIFT)
    #define modbus_485_interface_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << modbus_485_interface_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define modbus_485_interface_RX_HW_MASK                     (0x7Fu)
#endif /* End (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) */

/* Control Register definitions */
#define modbus_485_interface_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define modbus_485_interface_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define modbus_485_interface_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define modbus_485_interface_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define modbus_485_interface_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define modbus_485_interface_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define modbus_485_interface_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define modbus_485_interface_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define modbus_485_interface_CTRL_HD_SEND               (uint8)(0x01u << modbus_485_interface_CTRL_HD_SEND_SHIFT)
#define modbus_485_interface_CTRL_HD_SEND_BREAK         (uint8)(0x01u << modbus_485_interface_CTRL_HD_SEND_BREAK_SHIFT)
#define modbus_485_interface_CTRL_MARK                  (uint8)(0x01u << modbus_485_interface_CTRL_MARK_SHIFT)
#define modbus_485_interface_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << modbus_485_interface_CTRL_PARITY_TYPE0_SHIFT)
#define modbus_485_interface_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << modbus_485_interface_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define modbus_485_interface_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define modbus_485_interface_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define modbus_485_interface_SEND_BREAK                         (0x00u)
#define modbus_485_interface_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define modbus_485_interface_REINIT                             (0x02u)
#define modbus_485_interface_SEND_WAIT_REINIT                   (0x03u)

#define modbus_485_interface_OVER_SAMPLE_8                      (8u)
#define modbus_485_interface_OVER_SAMPLE_16                     (16u)

#define modbus_485_interface_BIT_CENTER                         (modbus_485_interface_OVER_SAMPLE_COUNT - 2u)

#define modbus_485_interface_FIFO_LENGTH                        (4u)
#define modbus_485_interface_NUMBER_OF_START_BIT                (1u)
#define modbus_485_interface_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define modbus_485_interface_TXBITCTR_BREAKBITS8X   ((modbus_485_interface_BREAK_BITS_TX * modbus_485_interface_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define modbus_485_interface_TXBITCTR_BREAKBITS ((modbus_485_interface_BREAK_BITS_TX * modbus_485_interface_OVER_SAMPLE_COUNT) - 1u)

#define modbus_485_interface_HALF_BIT_COUNT   \
                            (((modbus_485_interface_OVER_SAMPLE_COUNT / 2u) + (modbus_485_interface_USE23POLLING * 1u)) - 2u)
#if (modbus_485_interface_OVER_SAMPLE_COUNT == modbus_485_interface_OVER_SAMPLE_8)
    #define modbus_485_interface_HD_TXBITCTR_INIT   (((modbus_485_interface_BREAK_BITS_TX + \
                            modbus_485_interface_NUMBER_OF_START_BIT) * modbus_485_interface_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define modbus_485_interface_RXBITCTR_INIT  ((((modbus_485_interface_BREAK_BITS_RX + modbus_485_interface_NUMBER_OF_START_BIT) \
                            * modbus_485_interface_OVER_SAMPLE_COUNT) + modbus_485_interface_HALF_BIT_COUNT) - 1u)

#else /* modbus_485_interface_OVER_SAMPLE_COUNT == modbus_485_interface_OVER_SAMPLE_16 */
    #define modbus_485_interface_HD_TXBITCTR_INIT   ((8u * modbus_485_interface_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define modbus_485_interface_RXBITCTR_INIT      (((7u * modbus_485_interface_OVER_SAMPLE_COUNT) - 1u) + \
                                                      modbus_485_interface_HALF_BIT_COUNT)
#endif /* End modbus_485_interface_OVER_SAMPLE_COUNT */

#define modbus_485_interface_HD_RXBITCTR_INIT                   modbus_485_interface_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 modbus_485_interface_initVar;
#if (modbus_485_interface_TX_INTERRUPT_ENABLED && modbus_485_interface_TX_ENABLED)
    extern volatile uint8 modbus_485_interface_txBuffer[modbus_485_interface_TX_BUFFER_SIZE];
    extern volatile uint8 modbus_485_interface_txBufferRead;
    extern uint8 modbus_485_interface_txBufferWrite;
#endif /* (modbus_485_interface_TX_INTERRUPT_ENABLED && modbus_485_interface_TX_ENABLED) */
#if (modbus_485_interface_RX_INTERRUPT_ENABLED && (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED))
    extern uint8 modbus_485_interface_errorStatus;
    extern volatile uint8 modbus_485_interface_rxBuffer[modbus_485_interface_RX_BUFFER_SIZE];
    extern volatile uint8 modbus_485_interface_rxBufferRead;
    extern volatile uint8 modbus_485_interface_rxBufferWrite;
    extern volatile uint8 modbus_485_interface_rxBufferLoopDetect;
    extern volatile uint8 modbus_485_interface_rxBufferOverflow;
    #if (modbus_485_interface_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 modbus_485_interface_rxAddressMode;
        extern volatile uint8 modbus_485_interface_rxAddressDetected;
    #endif /* (modbus_485_interface_RXHW_ADDRESS_ENABLED) */
#endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED && (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define modbus_485_interface__B_UART__AM_SW_BYTE_BYTE 1
#define modbus_485_interface__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define modbus_485_interface__B_UART__AM_HW_BYTE_BY_BYTE 3
#define modbus_485_interface__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define modbus_485_interface__B_UART__AM_NONE 0

#define modbus_485_interface__B_UART__NONE_REVB 0
#define modbus_485_interface__B_UART__EVEN_REVB 1
#define modbus_485_interface__B_UART__ODD_REVB 2
#define modbus_485_interface__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define modbus_485_interface_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define modbus_485_interface_NUMBER_OF_STOP_BITS    (1u)

#if (modbus_485_interface_RXHW_ADDRESS_ENABLED)
    #define modbus_485_interface_RX_ADDRESS_MODE    (0u)
    #define modbus_485_interface_RX_HW_ADDRESS1     (0u)
    #define modbus_485_interface_RX_HW_ADDRESS2     (0u)
#endif /* (modbus_485_interface_RXHW_ADDRESS_ENABLED) */

#define modbus_485_interface_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << modbus_485_interface_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << modbus_485_interface_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << modbus_485_interface_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << modbus_485_interface_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << modbus_485_interface_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << modbus_485_interface_RX_STS_BREAK_SHIFT) \
                                        | (0 << modbus_485_interface_RX_STS_OVERRUN_SHIFT))

#define modbus_485_interface_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << modbus_485_interface_TX_STS_COMPLETE_SHIFT) \
                                        | (1 << modbus_485_interface_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (1 << modbus_485_interface_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << modbus_485_interface_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef modbus_485_interface_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define modbus_485_interface_CONTROL_REG \
                            (* (reg8 *) modbus_485_interface_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define modbus_485_interface_CONTROL_PTR \
                            (  (reg8 *) modbus_485_interface_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End modbus_485_interface_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(modbus_485_interface_TX_ENABLED)
    #define modbus_485_interface_TXDATA_REG          (* (reg8 *) modbus_485_interface_BUART_sTX_TxShifter_u0__F0_REG)
    #define modbus_485_interface_TXDATA_PTR          (  (reg8 *) modbus_485_interface_BUART_sTX_TxShifter_u0__F0_REG)
    #define modbus_485_interface_TXDATA_AUX_CTL_REG  (* (reg8 *) modbus_485_interface_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define modbus_485_interface_TXDATA_AUX_CTL_PTR  (  (reg8 *) modbus_485_interface_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define modbus_485_interface_TXSTATUS_REG        (* (reg8 *) modbus_485_interface_BUART_sTX_TxSts__STATUS_REG)
    #define modbus_485_interface_TXSTATUS_PTR        (  (reg8 *) modbus_485_interface_BUART_sTX_TxSts__STATUS_REG)
    #define modbus_485_interface_TXSTATUS_MASK_REG   (* (reg8 *) modbus_485_interface_BUART_sTX_TxSts__MASK_REG)
    #define modbus_485_interface_TXSTATUS_MASK_PTR   (  (reg8 *) modbus_485_interface_BUART_sTX_TxSts__MASK_REG)
    #define modbus_485_interface_TXSTATUS_ACTL_REG   (* (reg8 *) modbus_485_interface_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define modbus_485_interface_TXSTATUS_ACTL_PTR   (  (reg8 *) modbus_485_interface_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(modbus_485_interface_TXCLKGEN_DP)
        #define modbus_485_interface_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) modbus_485_interface_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define modbus_485_interface_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) modbus_485_interface_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define modbus_485_interface_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) modbus_485_interface_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define modbus_485_interface_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) modbus_485_interface_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define modbus_485_interface_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) modbus_485_interface_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define modbus_485_interface_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) modbus_485_interface_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define modbus_485_interface_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) modbus_485_interface_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define modbus_485_interface_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) modbus_485_interface_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define modbus_485_interface_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) modbus_485_interface_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define modbus_485_interface_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) modbus_485_interface_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* modbus_485_interface_TXCLKGEN_DP */

#endif /* End modbus_485_interface_TX_ENABLED */

#if(modbus_485_interface_HD_ENABLED)

    #define modbus_485_interface_TXDATA_REG             (* (reg8 *) modbus_485_interface_BUART_sRX_RxShifter_u0__F1_REG )
    #define modbus_485_interface_TXDATA_PTR             (  (reg8 *) modbus_485_interface_BUART_sRX_RxShifter_u0__F1_REG )
    #define modbus_485_interface_TXDATA_AUX_CTL_REG     (* (reg8 *) modbus_485_interface_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define modbus_485_interface_TXDATA_AUX_CTL_PTR     (  (reg8 *) modbus_485_interface_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define modbus_485_interface_TXSTATUS_REG           (* (reg8 *) modbus_485_interface_BUART_sRX_RxSts__STATUS_REG )
    #define modbus_485_interface_TXSTATUS_PTR           (  (reg8 *) modbus_485_interface_BUART_sRX_RxSts__STATUS_REG )
    #define modbus_485_interface_TXSTATUS_MASK_REG      (* (reg8 *) modbus_485_interface_BUART_sRX_RxSts__MASK_REG )
    #define modbus_485_interface_TXSTATUS_MASK_PTR      (  (reg8 *) modbus_485_interface_BUART_sRX_RxSts__MASK_REG )
    #define modbus_485_interface_TXSTATUS_ACTL_REG      (* (reg8 *) modbus_485_interface_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define modbus_485_interface_TXSTATUS_ACTL_PTR      (  (reg8 *) modbus_485_interface_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End modbus_485_interface_HD_ENABLED */

#if( (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) )
    #define modbus_485_interface_RXDATA_REG             (* (reg8 *) modbus_485_interface_BUART_sRX_RxShifter_u0__F0_REG )
    #define modbus_485_interface_RXDATA_PTR             (  (reg8 *) modbus_485_interface_BUART_sRX_RxShifter_u0__F0_REG )
    #define modbus_485_interface_RXADDRESS1_REG         (* (reg8 *) modbus_485_interface_BUART_sRX_RxShifter_u0__D0_REG )
    #define modbus_485_interface_RXADDRESS1_PTR         (  (reg8 *) modbus_485_interface_BUART_sRX_RxShifter_u0__D0_REG )
    #define modbus_485_interface_RXADDRESS2_REG         (* (reg8 *) modbus_485_interface_BUART_sRX_RxShifter_u0__D1_REG )
    #define modbus_485_interface_RXADDRESS2_PTR         (  (reg8 *) modbus_485_interface_BUART_sRX_RxShifter_u0__D1_REG )
    #define modbus_485_interface_RXDATA_AUX_CTL_REG     (* (reg8 *) modbus_485_interface_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define modbus_485_interface_RXBITCTR_PERIOD_REG    (* (reg8 *) modbus_485_interface_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define modbus_485_interface_RXBITCTR_PERIOD_PTR    (  (reg8 *) modbus_485_interface_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define modbus_485_interface_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) modbus_485_interface_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define modbus_485_interface_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) modbus_485_interface_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define modbus_485_interface_RXBITCTR_COUNTER_REG   (* (reg8 *) modbus_485_interface_BUART_sRX_RxBitCounter__COUNT_REG )
    #define modbus_485_interface_RXBITCTR_COUNTER_PTR   (  (reg8 *) modbus_485_interface_BUART_sRX_RxBitCounter__COUNT_REG )

    #define modbus_485_interface_RXSTATUS_REG           (* (reg8 *) modbus_485_interface_BUART_sRX_RxSts__STATUS_REG )
    #define modbus_485_interface_RXSTATUS_PTR           (  (reg8 *) modbus_485_interface_BUART_sRX_RxSts__STATUS_REG )
    #define modbus_485_interface_RXSTATUS_MASK_REG      (* (reg8 *) modbus_485_interface_BUART_sRX_RxSts__MASK_REG )
    #define modbus_485_interface_RXSTATUS_MASK_PTR      (  (reg8 *) modbus_485_interface_BUART_sRX_RxSts__MASK_REG )
    #define modbus_485_interface_RXSTATUS_ACTL_REG      (* (reg8 *) modbus_485_interface_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define modbus_485_interface_RXSTATUS_ACTL_PTR      (  (reg8 *) modbus_485_interface_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) */

#if(modbus_485_interface_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define modbus_485_interface_INTCLOCK_CLKEN_REG     (* (reg8 *) modbus_485_interface_IntClock__PM_ACT_CFG)
    #define modbus_485_interface_INTCLOCK_CLKEN_PTR     (  (reg8 *) modbus_485_interface_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define modbus_485_interface_INTCLOCK_CLKEN_MASK    modbus_485_interface_IntClock__PM_ACT_MSK
#endif /* End modbus_485_interface_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(modbus_485_interface_TX_ENABLED)
    #define modbus_485_interface_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End modbus_485_interface_TX_ENABLED */

#if(modbus_485_interface_HD_ENABLED)
    #define modbus_485_interface_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End modbus_485_interface_HD_ENABLED */

#if( (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) )
    #define modbus_485_interface_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define modbus_485_interface_WAIT_1_MS      modbus_485_interface_BL_CHK_DELAY_MS   

#define modbus_485_interface_TXBUFFERSIZE   modbus_485_interface_TX_BUFFER_SIZE
#define modbus_485_interface_RXBUFFERSIZE   modbus_485_interface_RX_BUFFER_SIZE

#if (modbus_485_interface_RXHW_ADDRESS_ENABLED)
    #define modbus_485_interface_RXADDRESSMODE  modbus_485_interface_RX_ADDRESS_MODE
    #define modbus_485_interface_RXHWADDRESS1   modbus_485_interface_RX_HW_ADDRESS1
    #define modbus_485_interface_RXHWADDRESS2   modbus_485_interface_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define modbus_485_interface_RXAddressMode  modbus_485_interface_RXADDRESSMODE
#endif /* (modbus_485_interface_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define modbus_485_interface_initvar                    modbus_485_interface_initVar

#define modbus_485_interface_RX_Enabled                 modbus_485_interface_RX_ENABLED
#define modbus_485_interface_TX_Enabled                 modbus_485_interface_TX_ENABLED
#define modbus_485_interface_HD_Enabled                 modbus_485_interface_HD_ENABLED
#define modbus_485_interface_RX_IntInterruptEnabled     modbus_485_interface_RX_INTERRUPT_ENABLED
#define modbus_485_interface_TX_IntInterruptEnabled     modbus_485_interface_TX_INTERRUPT_ENABLED
#define modbus_485_interface_InternalClockUsed          modbus_485_interface_INTERNAL_CLOCK_USED
#define modbus_485_interface_RXHW_Address_Enabled       modbus_485_interface_RXHW_ADDRESS_ENABLED
#define modbus_485_interface_OverSampleCount            modbus_485_interface_OVER_SAMPLE_COUNT
#define modbus_485_interface_ParityType                 modbus_485_interface_PARITY_TYPE

#if( modbus_485_interface_TX_ENABLED && (modbus_485_interface_TXBUFFERSIZE > modbus_485_interface_FIFO_LENGTH))
    #define modbus_485_interface_TXBUFFER               modbus_485_interface_txBuffer
    #define modbus_485_interface_TXBUFFERREAD           modbus_485_interface_txBufferRead
    #define modbus_485_interface_TXBUFFERWRITE          modbus_485_interface_txBufferWrite
#endif /* End modbus_485_interface_TX_ENABLED */
#if( ( modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED ) && \
     (modbus_485_interface_RXBUFFERSIZE > modbus_485_interface_FIFO_LENGTH) )
    #define modbus_485_interface_RXBUFFER               modbus_485_interface_rxBuffer
    #define modbus_485_interface_RXBUFFERREAD           modbus_485_interface_rxBufferRead
    #define modbus_485_interface_RXBUFFERWRITE          modbus_485_interface_rxBufferWrite
    #define modbus_485_interface_RXBUFFERLOOPDETECT     modbus_485_interface_rxBufferLoopDetect
    #define modbus_485_interface_RXBUFFER_OVERFLOW      modbus_485_interface_rxBufferOverflow
#endif /* End modbus_485_interface_RX_ENABLED */

#ifdef modbus_485_interface_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define modbus_485_interface_CONTROL                modbus_485_interface_CONTROL_REG
#endif /* End modbus_485_interface_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(modbus_485_interface_TX_ENABLED)
    #define modbus_485_interface_TXDATA                 modbus_485_interface_TXDATA_REG
    #define modbus_485_interface_TXSTATUS               modbus_485_interface_TXSTATUS_REG
    #define modbus_485_interface_TXSTATUS_MASK          modbus_485_interface_TXSTATUS_MASK_REG
    #define modbus_485_interface_TXSTATUS_ACTL          modbus_485_interface_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(modbus_485_interface_TXCLKGEN_DP)
        #define modbus_485_interface_TXBITCLKGEN_CTR        modbus_485_interface_TXBITCLKGEN_CTR_REG
        #define modbus_485_interface_TXBITCLKTX_COMPLETE    modbus_485_interface_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define modbus_485_interface_TXBITCTR_PERIOD        modbus_485_interface_TXBITCTR_PERIOD_REG
        #define modbus_485_interface_TXBITCTR_CONTROL       modbus_485_interface_TXBITCTR_CONTROL_REG
        #define modbus_485_interface_TXBITCTR_COUNTER       modbus_485_interface_TXBITCTR_COUNTER_REG
    #endif /* modbus_485_interface_TXCLKGEN_DP */
#endif /* End modbus_485_interface_TX_ENABLED */

#if(modbus_485_interface_HD_ENABLED)
    #define modbus_485_interface_TXDATA                 modbus_485_interface_TXDATA_REG
    #define modbus_485_interface_TXSTATUS               modbus_485_interface_TXSTATUS_REG
    #define modbus_485_interface_TXSTATUS_MASK          modbus_485_interface_TXSTATUS_MASK_REG
    #define modbus_485_interface_TXSTATUS_ACTL          modbus_485_interface_TXSTATUS_ACTL_REG
#endif /* End modbus_485_interface_HD_ENABLED */

#if( (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) )
    #define modbus_485_interface_RXDATA                 modbus_485_interface_RXDATA_REG
    #define modbus_485_interface_RXADDRESS1             modbus_485_interface_RXADDRESS1_REG
    #define modbus_485_interface_RXADDRESS2             modbus_485_interface_RXADDRESS2_REG
    #define modbus_485_interface_RXBITCTR_PERIOD        modbus_485_interface_RXBITCTR_PERIOD_REG
    #define modbus_485_interface_RXBITCTR_CONTROL       modbus_485_interface_RXBITCTR_CONTROL_REG
    #define modbus_485_interface_RXBITCTR_COUNTER       modbus_485_interface_RXBITCTR_COUNTER_REG
    #define modbus_485_interface_RXSTATUS               modbus_485_interface_RXSTATUS_REG
    #define modbus_485_interface_RXSTATUS_MASK          modbus_485_interface_RXSTATUS_MASK_REG
    #define modbus_485_interface_RXSTATUS_ACTL          modbus_485_interface_RXSTATUS_ACTL_REG
#endif /* End  (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) */

#if(modbus_485_interface_INTERNAL_CLOCK_USED)
    #define modbus_485_interface_INTCLOCK_CLKEN         modbus_485_interface_INTCLOCK_CLKEN_REG
#endif /* End modbus_485_interface_INTERNAL_CLOCK_USED */

#define modbus_485_interface_WAIT_FOR_COMLETE_REINIT    modbus_485_interface_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_modbus_485_interface_H */


/* [] END OF FILE */
