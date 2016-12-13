/*******************************************************************************
* File Name: modbus_485_interfaceINT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "modbus_485_interface.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (modbus_485_interface_RX_INTERRUPT_ENABLED && (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED))
    /*******************************************************************************
    * Function Name: modbus_485_interface_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  modbus_485_interface_rxBuffer - RAM buffer pointer for save received data.
    *  modbus_485_interface_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  modbus_485_interface_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  modbus_485_interface_rxBufferOverflow - software overflow flag. Set to one
    *     when modbus_485_interface_rxBufferWrite index overtakes
    *     modbus_485_interface_rxBufferRead index.
    *  modbus_485_interface_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when modbus_485_interface_rxBufferWrite is equal to
    *    modbus_485_interface_rxBufferRead
    *  modbus_485_interface_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  modbus_485_interface_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(modbus_485_interface_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef modbus_485_interface_RXISR_ENTRY_CALLBACK
        modbus_485_interface_RXISR_EntryCallback();
    #endif /* modbus_485_interface_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START modbus_485_interface_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = modbus_485_interface_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in modbus_485_interface_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (modbus_485_interface_RX_STS_BREAK | 
                            modbus_485_interface_RX_STS_PAR_ERROR |
                            modbus_485_interface_RX_STS_STOP_ERROR | 
                            modbus_485_interface_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                modbus_485_interface_errorStatus |= readStatus & ( modbus_485_interface_RX_STS_BREAK | 
                                                            modbus_485_interface_RX_STS_PAR_ERROR | 
                                                            modbus_485_interface_RX_STS_STOP_ERROR | 
                                                            modbus_485_interface_RX_STS_OVERRUN);
                /* `#START modbus_485_interface_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef modbus_485_interface_RXISR_ERROR_CALLBACK
                modbus_485_interface_RXISR_ERROR_Callback();
            #endif /* modbus_485_interface_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & modbus_485_interface_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = modbus_485_interface_RXDATA_REG;
            #if (modbus_485_interface_RXHW_ADDRESS_ENABLED)
                if(modbus_485_interface_rxAddressMode == (uint8)modbus_485_interface__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & modbus_485_interface_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & modbus_485_interface_RX_STS_ADDR_MATCH) != 0u)
                        {
                            modbus_485_interface_rxAddressDetected = 1u;
                        }
                        else
                        {
                            modbus_485_interface_rxAddressDetected = 0u;
                        }
                    }
                    if(modbus_485_interface_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        modbus_485_interface_rxBuffer[modbus_485_interface_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    modbus_485_interface_rxBuffer[modbus_485_interface_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                modbus_485_interface_rxBuffer[modbus_485_interface_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (modbus_485_interface_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(modbus_485_interface_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        modbus_485_interface_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    modbus_485_interface_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(modbus_485_interface_rxBufferWrite >= modbus_485_interface_RX_BUFFER_SIZE)
                    {
                        modbus_485_interface_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(modbus_485_interface_rxBufferWrite == modbus_485_interface_rxBufferRead)
                    {
                        modbus_485_interface_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (modbus_485_interface_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            modbus_485_interface_RXSTATUS_MASK_REG  &= (uint8)~modbus_485_interface_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(modbus_485_interface_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (modbus_485_interface_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & modbus_485_interface_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START modbus_485_interface_RXISR_END` */

        /* `#END` */

    #ifdef modbus_485_interface_RXISR_EXIT_CALLBACK
        modbus_485_interface_RXISR_ExitCallback();
    #endif /* modbus_485_interface_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED && (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED)) */


#if (modbus_485_interface_TX_INTERRUPT_ENABLED && modbus_485_interface_TX_ENABLED)
    /*******************************************************************************
    * Function Name: modbus_485_interface_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  modbus_485_interface_txBuffer - RAM buffer pointer for transmit data from.
    *  modbus_485_interface_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  modbus_485_interface_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(modbus_485_interface_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef modbus_485_interface_TXISR_ENTRY_CALLBACK
        modbus_485_interface_TXISR_EntryCallback();
    #endif /* modbus_485_interface_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START modbus_485_interface_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((modbus_485_interface_txBufferRead != modbus_485_interface_txBufferWrite) &&
             ((modbus_485_interface_TXSTATUS_REG & modbus_485_interface_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(modbus_485_interface_txBufferRead >= modbus_485_interface_TX_BUFFER_SIZE)
            {
                modbus_485_interface_txBufferRead = 0u;
            }

            modbus_485_interface_TXDATA_REG = modbus_485_interface_txBuffer[modbus_485_interface_txBufferRead];

            /* Set next pointer */
            modbus_485_interface_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START modbus_485_interface_TXISR_END` */

        /* `#END` */

    #ifdef modbus_485_interface_TXISR_EXIT_CALLBACK
        modbus_485_interface_TXISR_ExitCallback();
    #endif /* modbus_485_interface_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (modbus_485_interface_TX_INTERRUPT_ENABLED && modbus_485_interface_TX_ENABLED) */


/* [] END OF FILE */
