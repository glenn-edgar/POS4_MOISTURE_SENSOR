/*******************************************************************************
* File Name: modbus_485_interface.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the UART component
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "modbus_485_interface.h"
#if (modbus_485_interface_INTERNAL_CLOCK_USED)
    #include "modbus_485_interface_IntClock.h"
#endif /* End modbus_485_interface_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 modbus_485_interface_initVar = 0u;

#if (modbus_485_interface_TX_INTERRUPT_ENABLED && modbus_485_interface_TX_ENABLED)
    volatile uint8 modbus_485_interface_txBuffer[modbus_485_interface_TX_BUFFER_SIZE];
    volatile uint8 modbus_485_interface_txBufferRead = 0u;
    uint8 modbus_485_interface_txBufferWrite = 0u;
#endif /* (modbus_485_interface_TX_INTERRUPT_ENABLED && modbus_485_interface_TX_ENABLED) */

#if (modbus_485_interface_RX_INTERRUPT_ENABLED && (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED))
    uint8 modbus_485_interface_errorStatus = 0u;
    volatile uint8 modbus_485_interface_rxBuffer[modbus_485_interface_RX_BUFFER_SIZE];
    volatile uint8 modbus_485_interface_rxBufferRead  = 0u;
    volatile uint8 modbus_485_interface_rxBufferWrite = 0u;
    volatile uint8 modbus_485_interface_rxBufferLoopDetect = 0u;
    volatile uint8 modbus_485_interface_rxBufferOverflow   = 0u;
    #if (modbus_485_interface_RXHW_ADDRESS_ENABLED)
        volatile uint8 modbus_485_interface_rxAddressMode = modbus_485_interface_RX_ADDRESS_MODE;
        volatile uint8 modbus_485_interface_rxAddressDetected = 0u;
    #endif /* (modbus_485_interface_RXHW_ADDRESS_ENABLED) */
#endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED && (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED)) */


/*******************************************************************************
* Function Name: modbus_485_interface_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  modbus_485_interface_Start() sets the initVar variable, calls the
*  modbus_485_interface_Init() function, and then calls the
*  modbus_485_interface_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The modbus_485_interface_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time modbus_485_interface_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the modbus_485_interface_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void modbus_485_interface_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(modbus_485_interface_initVar == 0u)
    {
        modbus_485_interface_Init();
        modbus_485_interface_initVar = 1u;
    }

    modbus_485_interface_Enable();
}


/*******************************************************************************
* Function Name: modbus_485_interface_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call modbus_485_interface_Init() because
*  the modbus_485_interface_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void modbus_485_interface_Init(void) 
{
    #if(modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED)

        #if (modbus_485_interface_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(modbus_485_interface_RX_VECT_NUM, &modbus_485_interface_RXISR);
            CyIntSetPriority(modbus_485_interface_RX_VECT_NUM, modbus_485_interface_RX_PRIOR_NUM);
            modbus_485_interface_errorStatus = 0u;
        #endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED) */

        #if (modbus_485_interface_RXHW_ADDRESS_ENABLED)
            modbus_485_interface_SetRxAddressMode(modbus_485_interface_RX_ADDRESS_MODE);
            modbus_485_interface_SetRxAddress1(modbus_485_interface_RX_HW_ADDRESS1);
            modbus_485_interface_SetRxAddress2(modbus_485_interface_RX_HW_ADDRESS2);
        #endif /* End modbus_485_interface_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        modbus_485_interface_RXBITCTR_PERIOD_REG = modbus_485_interface_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        modbus_485_interface_RXSTATUS_MASK_REG  = modbus_485_interface_INIT_RX_INTERRUPTS_MASK;
    #endif /* End modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED*/

    #if(modbus_485_interface_TX_ENABLED)
        #if (modbus_485_interface_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(modbus_485_interface_TX_VECT_NUM, &modbus_485_interface_TXISR);
            CyIntSetPriority(modbus_485_interface_TX_VECT_NUM, modbus_485_interface_TX_PRIOR_NUM);
        #endif /* (modbus_485_interface_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (modbus_485_interface_TXCLKGEN_DP)
            modbus_485_interface_TXBITCLKGEN_CTR_REG = modbus_485_interface_BIT_CENTER;
            modbus_485_interface_TXBITCLKTX_COMPLETE_REG = ((modbus_485_interface_NUMBER_OF_DATA_BITS +
                        modbus_485_interface_NUMBER_OF_START_BIT) * modbus_485_interface_OVER_SAMPLE_COUNT) - 1u;
        #else
            modbus_485_interface_TXBITCTR_PERIOD_REG = ((modbus_485_interface_NUMBER_OF_DATA_BITS +
                        modbus_485_interface_NUMBER_OF_START_BIT) * modbus_485_interface_OVER_SAMPLE_8) - 1u;
        #endif /* End modbus_485_interface_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (modbus_485_interface_TX_INTERRUPT_ENABLED)
            modbus_485_interface_TXSTATUS_MASK_REG = modbus_485_interface_TX_STS_FIFO_EMPTY;
        #else
            modbus_485_interface_TXSTATUS_MASK_REG = modbus_485_interface_INIT_TX_INTERRUPTS_MASK;
        #endif /*End modbus_485_interface_TX_INTERRUPT_ENABLED*/

    #endif /* End modbus_485_interface_TX_ENABLED */

    #if(modbus_485_interface_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        modbus_485_interface_WriteControlRegister( \
            (modbus_485_interface_ReadControlRegister() & (uint8)~modbus_485_interface_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(modbus_485_interface_PARITY_TYPE << modbus_485_interface_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End modbus_485_interface_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: modbus_485_interface_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call modbus_485_interface_Enable() because the modbus_485_interface_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  modbus_485_interface_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void modbus_485_interface_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        modbus_485_interface_RXBITCTR_CONTROL_REG |= modbus_485_interface_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        modbus_485_interface_RXSTATUS_ACTL_REG  |= modbus_485_interface_INT_ENABLE;

        #if (modbus_485_interface_RX_INTERRUPT_ENABLED)
            modbus_485_interface_EnableRxInt();

            #if (modbus_485_interface_RXHW_ADDRESS_ENABLED)
                modbus_485_interface_rxAddressDetected = 0u;
            #endif /* (modbus_485_interface_RXHW_ADDRESS_ENABLED) */
        #endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED) */
    #endif /* (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED) */

    #if(modbus_485_interface_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!modbus_485_interface_TXCLKGEN_DP)
            modbus_485_interface_TXBITCTR_CONTROL_REG |= modbus_485_interface_CNTR_ENABLE;
        #endif /* End modbus_485_interface_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        modbus_485_interface_TXSTATUS_ACTL_REG |= modbus_485_interface_INT_ENABLE;
        #if (modbus_485_interface_TX_INTERRUPT_ENABLED)
            modbus_485_interface_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            modbus_485_interface_EnableTxInt();
        #endif /* (modbus_485_interface_TX_INTERRUPT_ENABLED) */
     #endif /* (modbus_485_interface_TX_INTERRUPT_ENABLED) */

    #if (modbus_485_interface_INTERNAL_CLOCK_USED)
        modbus_485_interface_IntClock_Start();  /* Enable the clock */
    #endif /* (modbus_485_interface_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: modbus_485_interface_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void modbus_485_interface_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED)
        modbus_485_interface_RXBITCTR_CONTROL_REG &= (uint8) ~modbus_485_interface_CNTR_ENABLE;
    #endif /* (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED) */

    #if (modbus_485_interface_TX_ENABLED)
        #if(!modbus_485_interface_TXCLKGEN_DP)
            modbus_485_interface_TXBITCTR_CONTROL_REG &= (uint8) ~modbus_485_interface_CNTR_ENABLE;
        #endif /* (!modbus_485_interface_TXCLKGEN_DP) */
    #endif /* (modbus_485_interface_TX_ENABLED) */

    #if (modbus_485_interface_INTERNAL_CLOCK_USED)
        modbus_485_interface_IntClock_Stop();   /* Disable the clock */
    #endif /* (modbus_485_interface_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED)
        modbus_485_interface_RXSTATUS_ACTL_REG  &= (uint8) ~modbus_485_interface_INT_ENABLE;

        #if (modbus_485_interface_RX_INTERRUPT_ENABLED)
            modbus_485_interface_DisableRxInt();
        #endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED) */
    #endif /* (modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED) */

    #if (modbus_485_interface_TX_ENABLED)
        modbus_485_interface_TXSTATUS_ACTL_REG &= (uint8) ~modbus_485_interface_INT_ENABLE;

        #if (modbus_485_interface_TX_INTERRUPT_ENABLED)
            modbus_485_interface_DisableTxInt();
        #endif /* (modbus_485_interface_TX_INTERRUPT_ENABLED) */
    #endif /* (modbus_485_interface_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: modbus_485_interface_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8 modbus_485_interface_ReadControlRegister(void) 
{
    #if (modbus_485_interface_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(modbus_485_interface_CONTROL_REG);
    #endif /* (modbus_485_interface_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: modbus_485_interface_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  modbus_485_interface_WriteControlRegister(uint8 control) 
{
    #if (modbus_485_interface_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       modbus_485_interface_CONTROL_REG = control;
    #endif /* (modbus_485_interface_CONTROL_REG_REMOVED) */
}


#if(modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED)
    /*******************************************************************************
    * Function Name: modbus_485_interface_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      modbus_485_interface_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      modbus_485_interface_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      modbus_485_interface_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      modbus_485_interface_RX_STS_BREAK            Interrupt on break.
    *      modbus_485_interface_RX_STS_OVERRUN          Interrupt on overrun error.
    *      modbus_485_interface_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      modbus_485_interface_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void modbus_485_interface_SetRxInterruptMode(uint8 intSrc) 
    {
        modbus_485_interface_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  modbus_485_interface_rxBuffer - RAM buffer pointer for save received data.
    *  modbus_485_interface_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  modbus_485_interface_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  modbus_485_interface_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 modbus_485_interface_ReadRxData(void) 
    {
        uint8 rxData;

    #if (modbus_485_interface_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        modbus_485_interface_DisableRxInt();

        locRxBufferRead  = modbus_485_interface_rxBufferRead;
        locRxBufferWrite = modbus_485_interface_rxBufferWrite;

        if( (modbus_485_interface_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = modbus_485_interface_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= modbus_485_interface_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            modbus_485_interface_rxBufferRead = locRxBufferRead;

            if(modbus_485_interface_rxBufferLoopDetect != 0u)
            {
                modbus_485_interface_rxBufferLoopDetect = 0u;
                #if ((modbus_485_interface_RX_INTERRUPT_ENABLED) && (modbus_485_interface_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( modbus_485_interface_HD_ENABLED )
                        if((modbus_485_interface_CONTROL_REG & modbus_485_interface_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            modbus_485_interface_RXSTATUS_MASK_REG  |= modbus_485_interface_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        modbus_485_interface_RXSTATUS_MASK_REG  |= modbus_485_interface_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end modbus_485_interface_HD_ENABLED */
                #endif /* ((modbus_485_interface_RX_INTERRUPT_ENABLED) && (modbus_485_interface_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = modbus_485_interface_RXDATA_REG;
        }

        modbus_485_interface_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = modbus_485_interface_RXDATA_REG;

    #endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  modbus_485_interface_RX_STS_FIFO_NOTEMPTY.
    *  modbus_485_interface_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  modbus_485_interface_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   modbus_485_interface_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   modbus_485_interface_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 modbus_485_interface_ReadRxStatus(void) 
    {
        uint8 status;

        status = modbus_485_interface_RXSTATUS_REG & modbus_485_interface_RX_HW_MASK;

    #if (modbus_485_interface_RX_INTERRUPT_ENABLED)
        if(modbus_485_interface_rxBufferOverflow != 0u)
        {
            status |= modbus_485_interface_RX_STS_SOFT_BUFF_OVER;
            modbus_485_interface_rxBufferOverflow = 0u;
        }
    #endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. modbus_485_interface_GetChar() is
    *  designed for ASCII characters and returns a uint8 where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  modbus_485_interface_rxBuffer - RAM buffer pointer for save received data.
    *  modbus_485_interface_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  modbus_485_interface_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  modbus_485_interface_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 modbus_485_interface_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (modbus_485_interface_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        modbus_485_interface_DisableRxInt();

        locRxBufferRead  = modbus_485_interface_rxBufferRead;
        locRxBufferWrite = modbus_485_interface_rxBufferWrite;

        if( (modbus_485_interface_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = modbus_485_interface_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= modbus_485_interface_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            modbus_485_interface_rxBufferRead = locRxBufferRead;

            if(modbus_485_interface_rxBufferLoopDetect != 0u)
            {
                modbus_485_interface_rxBufferLoopDetect = 0u;
                #if( (modbus_485_interface_RX_INTERRUPT_ENABLED) && (modbus_485_interface_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( modbus_485_interface_HD_ENABLED )
                        if((modbus_485_interface_CONTROL_REG & modbus_485_interface_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            modbus_485_interface_RXSTATUS_MASK_REG |= modbus_485_interface_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        modbus_485_interface_RXSTATUS_MASK_REG |= modbus_485_interface_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end modbus_485_interface_HD_ENABLED */
                #endif /* modbus_485_interface_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = modbus_485_interface_RXSTATUS_REG;
            if((rxStatus & modbus_485_interface_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = modbus_485_interface_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (modbus_485_interface_RX_STS_BREAK | modbus_485_interface_RX_STS_PAR_ERROR |
                                modbus_485_interface_RX_STS_STOP_ERROR | modbus_485_interface_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        modbus_485_interface_EnableRxInt();

    #else

        rxStatus =modbus_485_interface_RXSTATUS_REG;
        if((rxStatus & modbus_485_interface_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = modbus_485_interface_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (modbus_485_interface_RX_STS_BREAK | modbus_485_interface_RX_STS_PAR_ERROR |
                            modbus_485_interface_RX_STS_STOP_ERROR | modbus_485_interface_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 modbus_485_interface_GetByte(void) 
    {
        
    #if (modbus_485_interface_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        modbus_485_interface_DisableRxInt();
        locErrorStatus = (uint16)modbus_485_interface_errorStatus;
        modbus_485_interface_errorStatus = 0u;
        modbus_485_interface_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | modbus_485_interface_ReadRxData() );
    #else
        return ( ((uint16)modbus_485_interface_ReadRxStatus() << 8u) | modbus_485_interface_ReadRxData() );
    #endif /* modbus_485_interface_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  modbus_485_interface_rxBufferWrite - used to calculate left bytes.
    *  modbus_485_interface_rxBufferRead - used to calculate left bytes.
    *  modbus_485_interface_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 modbus_485_interface_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (modbus_485_interface_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        modbus_485_interface_DisableRxInt();

        if(modbus_485_interface_rxBufferRead == modbus_485_interface_rxBufferWrite)
        {
            if(modbus_485_interface_rxBufferLoopDetect != 0u)
            {
                size = modbus_485_interface_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(modbus_485_interface_rxBufferRead < modbus_485_interface_rxBufferWrite)
        {
            size = (modbus_485_interface_rxBufferWrite - modbus_485_interface_rxBufferRead);
        }
        else
        {
            size = (modbus_485_interface_RX_BUFFER_SIZE - modbus_485_interface_rxBufferRead) + modbus_485_interface_rxBufferWrite;
        }

        modbus_485_interface_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((modbus_485_interface_RXSTATUS_REG & modbus_485_interface_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  modbus_485_interface_rxBufferWrite - cleared to zero.
    *  modbus_485_interface_rxBufferRead - cleared to zero.
    *  modbus_485_interface_rxBufferLoopDetect - cleared to zero.
    *  modbus_485_interface_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void modbus_485_interface_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        modbus_485_interface_RXDATA_AUX_CTL_REG |= (uint8)  modbus_485_interface_RX_FIFO_CLR;
        modbus_485_interface_RXDATA_AUX_CTL_REG &= (uint8) ~modbus_485_interface_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (modbus_485_interface_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        modbus_485_interface_DisableRxInt();

        modbus_485_interface_rxBufferRead = 0u;
        modbus_485_interface_rxBufferWrite = 0u;
        modbus_485_interface_rxBufferLoopDetect = 0u;
        modbus_485_interface_rxBufferOverflow = 0u;

        modbus_485_interface_EnableRxInt();

    #endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  modbus_485_interface__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  modbus_485_interface__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  modbus_485_interface__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  modbus_485_interface__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  modbus_485_interface__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  modbus_485_interface_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  modbus_485_interface_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void modbus_485_interface_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(modbus_485_interface_RXHW_ADDRESS_ENABLED)
            #if(modbus_485_interface_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* modbus_485_interface_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = modbus_485_interface_CONTROL_REG & (uint8)~modbus_485_interface_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << modbus_485_interface_CTRL_RXADDR_MODE0_SHIFT);
                modbus_485_interface_CONTROL_REG = tmpCtrl;

                #if(modbus_485_interface_RX_INTERRUPT_ENABLED && \
                   (modbus_485_interface_RXBUFFERSIZE > modbus_485_interface_FIFO_LENGTH) )
                    modbus_485_interface_rxAddressMode = addressMode;
                    modbus_485_interface_rxAddressDetected = 0u;
                #endif /* End modbus_485_interface_RXBUFFERSIZE > modbus_485_interface_FIFO_LENGTH*/
            #endif /* End modbus_485_interface_CONTROL_REG_REMOVED */
        #else /* modbus_485_interface_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End modbus_485_interface_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void modbus_485_interface_SetRxAddress1(uint8 address) 
    {
        modbus_485_interface_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void modbus_485_interface_SetRxAddress2(uint8 address) 
    {
        modbus_485_interface_RXADDRESS2_REG = address;
    }

#endif  /* modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED*/


#if( (modbus_485_interface_TX_ENABLED) || (modbus_485_interface_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: modbus_485_interface_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   modbus_485_interface_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   modbus_485_interface_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   modbus_485_interface_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   modbus_485_interface_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void modbus_485_interface_SetTxInterruptMode(uint8 intSrc) 
    {
        modbus_485_interface_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  modbus_485_interface_txBuffer - RAM buffer pointer for save data for transmission
    *  modbus_485_interface_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  modbus_485_interface_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  modbus_485_interface_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void modbus_485_interface_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(modbus_485_interface_initVar != 0u)
        {
        #if (modbus_485_interface_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            modbus_485_interface_DisableTxInt();

            if( (modbus_485_interface_txBufferRead == modbus_485_interface_txBufferWrite) &&
                ((modbus_485_interface_TXSTATUS_REG & modbus_485_interface_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                modbus_485_interface_TXDATA_REG = txDataByte;
            }
            else
            {
                if(modbus_485_interface_txBufferWrite >= modbus_485_interface_TX_BUFFER_SIZE)
                {
                    modbus_485_interface_txBufferWrite = 0u;
                }

                modbus_485_interface_txBuffer[modbus_485_interface_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                modbus_485_interface_txBufferWrite++;
            }

            modbus_485_interface_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            modbus_485_interface_TXDATA_REG = txDataByte;

        #endif /*(modbus_485_interface_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8 modbus_485_interface_ReadTxStatus(void) 
    {
        return(modbus_485_interface_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  modbus_485_interface_txBuffer - RAM buffer pointer for save data for transmission
    *  modbus_485_interface_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  modbus_485_interface_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  modbus_485_interface_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void modbus_485_interface_PutChar(uint8 txDataByte) 
    {
    #if (modbus_485_interface_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((modbus_485_interface_TX_BUFFER_SIZE > modbus_485_interface_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            modbus_485_interface_DisableTxInt();
        #endif /* (modbus_485_interface_TX_BUFFER_SIZE > modbus_485_interface_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = modbus_485_interface_txBufferWrite;
            locTxBufferRead  = modbus_485_interface_txBufferRead;

        #if ((modbus_485_interface_TX_BUFFER_SIZE > modbus_485_interface_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            modbus_485_interface_EnableTxInt();
        #endif /* (modbus_485_interface_TX_BUFFER_SIZE > modbus_485_interface_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(modbus_485_interface_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((modbus_485_interface_TXSTATUS_REG & modbus_485_interface_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            modbus_485_interface_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= modbus_485_interface_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            modbus_485_interface_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((modbus_485_interface_TX_BUFFER_SIZE > modbus_485_interface_MAX_BYTE_VALUE) && (CY_PSOC3))
            modbus_485_interface_DisableTxInt();
        #endif /* (modbus_485_interface_TX_BUFFER_SIZE > modbus_485_interface_MAX_BYTE_VALUE) && (CY_PSOC3) */

            modbus_485_interface_txBufferWrite = locTxBufferWrite;

        #if ((modbus_485_interface_TX_BUFFER_SIZE > modbus_485_interface_MAX_BYTE_VALUE) && (CY_PSOC3))
            modbus_485_interface_EnableTxInt();
        #endif /* (modbus_485_interface_TX_BUFFER_SIZE > modbus_485_interface_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (modbus_485_interface_TXSTATUS_REG & modbus_485_interface_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                modbus_485_interface_SetPendingTxInt();
            }
        }

    #else

        while((modbus_485_interface_TXSTATUS_REG & modbus_485_interface_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        modbus_485_interface_TXDATA_REG = txDataByte;

    #endif /* modbus_485_interface_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  modbus_485_interface_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void modbus_485_interface_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(modbus_485_interface_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                modbus_485_interface_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  modbus_485_interface_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void modbus_485_interface_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(modbus_485_interface_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                modbus_485_interface_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  modbus_485_interface_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void modbus_485_interface_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(modbus_485_interface_initVar != 0u)
        {
            modbus_485_interface_PutChar(txDataByte);
            modbus_485_interface_PutChar(0x0Du);
            modbus_485_interface_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  modbus_485_interface_txBufferWrite - used to calculate left space.
    *  modbus_485_interface_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 modbus_485_interface_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (modbus_485_interface_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        modbus_485_interface_DisableTxInt();

        if(modbus_485_interface_txBufferRead == modbus_485_interface_txBufferWrite)
        {
            size = 0u;
        }
        else if(modbus_485_interface_txBufferRead < modbus_485_interface_txBufferWrite)
        {
            size = (modbus_485_interface_txBufferWrite - modbus_485_interface_txBufferRead);
        }
        else
        {
            size = (modbus_485_interface_TX_BUFFER_SIZE - modbus_485_interface_txBufferRead) +
                    modbus_485_interface_txBufferWrite;
        }

        modbus_485_interface_EnableTxInt();

    #else

        size = modbus_485_interface_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & modbus_485_interface_TX_STS_FIFO_FULL) != 0u)
        {
            size = modbus_485_interface_FIFO_LENGTH;
        }
        else if((size & modbus_485_interface_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (modbus_485_interface_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  modbus_485_interface_txBufferWrite - cleared to zero.
    *  modbus_485_interface_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void modbus_485_interface_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        modbus_485_interface_TXDATA_AUX_CTL_REG |= (uint8)  modbus_485_interface_TX_FIFO_CLR;
        modbus_485_interface_TXDATA_AUX_CTL_REG &= (uint8) ~modbus_485_interface_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (modbus_485_interface_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        modbus_485_interface_DisableTxInt();

        modbus_485_interface_txBufferRead = 0u;
        modbus_485_interface_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        modbus_485_interface_EnableTxInt();

    #endif /* (modbus_485_interface_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   modbus_485_interface_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   modbus_485_interface_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   modbus_485_interface_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   modbus_485_interface_SEND_WAIT_REINIT - Performs both options: 
    *      modbus_485_interface_SEND_BREAK and modbus_485_interface_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  modbus_485_interface_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with modbus_485_interface_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The modbus_485_interface_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void modbus_485_interface_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(modbus_485_interface_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(modbus_485_interface_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == modbus_485_interface_SEND_BREAK) ||
                (retMode == modbus_485_interface_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                modbus_485_interface_WriteControlRegister(modbus_485_interface_ReadControlRegister() |
                                                      modbus_485_interface_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                modbus_485_interface_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = modbus_485_interface_TXSTATUS_REG;
                }
                while((tmpStat & modbus_485_interface_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == modbus_485_interface_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == modbus_485_interface_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = modbus_485_interface_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & modbus_485_interface_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == modbus_485_interface_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == modbus_485_interface_REINIT) ||
                (retMode == modbus_485_interface_SEND_WAIT_REINIT) )
            {
                modbus_485_interface_WriteControlRegister(modbus_485_interface_ReadControlRegister() &
                                              (uint8)~modbus_485_interface_CTRL_HD_SEND_BREAK);
            }

        #else /* modbus_485_interface_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == modbus_485_interface_SEND_BREAK) ||
                (retMode == modbus_485_interface_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (modbus_485_interface_PARITY_TYPE != modbus_485_interface__B_UART__NONE_REVB) || \
                                    (modbus_485_interface_PARITY_TYPE_SW != 0u) )
                    modbus_485_interface_WriteControlRegister(modbus_485_interface_ReadControlRegister() |
                                                          modbus_485_interface_CTRL_HD_SEND_BREAK);
                #endif /* End modbus_485_interface_PARITY_TYPE != modbus_485_interface__B_UART__NONE_REVB  */

                #if(modbus_485_interface_TXCLKGEN_DP)
                    txPeriod = modbus_485_interface_TXBITCLKTX_COMPLETE_REG;
                    modbus_485_interface_TXBITCLKTX_COMPLETE_REG = modbus_485_interface_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = modbus_485_interface_TXBITCTR_PERIOD_REG;
                    modbus_485_interface_TXBITCTR_PERIOD_REG = modbus_485_interface_TXBITCTR_BREAKBITS8X;
                #endif /* End modbus_485_interface_TXCLKGEN_DP */

                /* Send zeros */
                modbus_485_interface_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = modbus_485_interface_TXSTATUS_REG;
                }
                while((tmpStat & modbus_485_interface_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == modbus_485_interface_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == modbus_485_interface_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = modbus_485_interface_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & modbus_485_interface_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == modbus_485_interface_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == modbus_485_interface_REINIT) ||
                (retMode == modbus_485_interface_SEND_WAIT_REINIT) )
            {

            #if(modbus_485_interface_TXCLKGEN_DP)
                modbus_485_interface_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                modbus_485_interface_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End modbus_485_interface_TXCLKGEN_DP */

            #if( (modbus_485_interface_PARITY_TYPE != modbus_485_interface__B_UART__NONE_REVB) || \
                 (modbus_485_interface_PARITY_TYPE_SW != 0u) )
                modbus_485_interface_WriteControlRegister(modbus_485_interface_ReadControlRegister() &
                                                      (uint8) ~modbus_485_interface_CTRL_HD_SEND_BREAK);
            #endif /* End modbus_485_interface_PARITY_TYPE != NONE */
            }
        #endif    /* End modbus_485_interface_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       modbus_485_interface_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       modbus_485_interface_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears modbus_485_interface_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void modbus_485_interface_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( modbus_485_interface_CONTROL_REG_REMOVED == 0u )
            modbus_485_interface_WriteControlRegister(modbus_485_interface_ReadControlRegister() |
                                                  modbus_485_interface_CTRL_MARK);
        #endif /* End modbus_485_interface_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( modbus_485_interface_CONTROL_REG_REMOVED == 0u )
            modbus_485_interface_WriteControlRegister(modbus_485_interface_ReadControlRegister() &
                                                  (uint8) ~modbus_485_interface_CTRL_MARK);
        #endif /* End modbus_485_interface_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* Endmodbus_485_interface_TX_ENABLED */

#if(modbus_485_interface_HD_ENABLED)


    /*******************************************************************************
    * Function Name: modbus_485_interface_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void modbus_485_interface_LoadRxConfig(void) 
    {
        modbus_485_interface_WriteControlRegister(modbus_485_interface_ReadControlRegister() &
                                                (uint8)~modbus_485_interface_CTRL_HD_SEND);
        modbus_485_interface_RXBITCTR_PERIOD_REG = modbus_485_interface_HD_RXBITCTR_INIT;

    #if (modbus_485_interface_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        modbus_485_interface_SetRxInterruptMode(modbus_485_interface_INIT_RX_INTERRUPTS_MASK);
    #endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: modbus_485_interface_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void modbus_485_interface_LoadTxConfig(void) 
    {
    #if (modbus_485_interface_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        modbus_485_interface_SetRxInterruptMode(0u);
    #endif /* (modbus_485_interface_RX_INTERRUPT_ENABLED) */

        modbus_485_interface_WriteControlRegister(modbus_485_interface_ReadControlRegister() | modbus_485_interface_CTRL_HD_SEND);
        modbus_485_interface_RXBITCTR_PERIOD_REG = modbus_485_interface_HD_TXBITCTR_INIT;
    }

#endif  /* modbus_485_interface_HD_ENABLED */


/* [] END OF FILE */
