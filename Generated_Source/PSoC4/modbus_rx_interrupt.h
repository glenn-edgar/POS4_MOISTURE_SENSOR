/*******************************************************************************
* File Name: modbus_rx_interrupt.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_modbus_rx_interrupt_H)
#define CY_ISR_modbus_rx_interrupt_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void modbus_rx_interrupt_Start(void);
void modbus_rx_interrupt_StartEx(cyisraddress address);
void modbus_rx_interrupt_Stop(void);

CY_ISR_PROTO(modbus_rx_interrupt_Interrupt);

void modbus_rx_interrupt_SetVector(cyisraddress address);
cyisraddress modbus_rx_interrupt_GetVector(void);

void modbus_rx_interrupt_SetPriority(uint8 priority);
uint8 modbus_rx_interrupt_GetPriority(void);

void modbus_rx_interrupt_Enable(void);
uint8 modbus_rx_interrupt_GetState(void);
void modbus_rx_interrupt_Disable(void);

void modbus_rx_interrupt_SetPending(void);
void modbus_rx_interrupt_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the modbus_rx_interrupt ISR. */
#define modbus_rx_interrupt_INTC_VECTOR            ((reg32 *) modbus_rx_interrupt__INTC_VECT)

/* Address of the modbus_rx_interrupt ISR priority. */
#define modbus_rx_interrupt_INTC_PRIOR             ((reg32 *) modbus_rx_interrupt__INTC_PRIOR_REG)

/* Priority of the modbus_rx_interrupt interrupt. */
#define modbus_rx_interrupt_INTC_PRIOR_NUMBER      modbus_rx_interrupt__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable modbus_rx_interrupt interrupt. */
#define modbus_rx_interrupt_INTC_SET_EN            ((reg32 *) modbus_rx_interrupt__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the modbus_rx_interrupt interrupt. */
#define modbus_rx_interrupt_INTC_CLR_EN            ((reg32 *) modbus_rx_interrupt__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the modbus_rx_interrupt interrupt state to pending. */
#define modbus_rx_interrupt_INTC_SET_PD            ((reg32 *) modbus_rx_interrupt__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the modbus_rx_interrupt interrupt. */
#define modbus_rx_interrupt_INTC_CLR_PD            ((reg32 *) modbus_rx_interrupt__INTC_CLR_PD_REG)



#endif /* CY_ISR_modbus_rx_interrupt_H */


/* [] END OF FILE */
