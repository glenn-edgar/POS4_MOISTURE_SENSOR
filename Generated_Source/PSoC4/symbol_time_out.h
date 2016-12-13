/*******************************************************************************
* File Name: symbol_time_out.h
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
#if !defined(CY_ISR_symbol_time_out_H)
#define CY_ISR_symbol_time_out_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void symbol_time_out_Start(void);
void symbol_time_out_StartEx(cyisraddress address);
void symbol_time_out_Stop(void);

CY_ISR_PROTO(symbol_time_out_Interrupt);

void symbol_time_out_SetVector(cyisraddress address);
cyisraddress symbol_time_out_GetVector(void);

void symbol_time_out_SetPriority(uint8 priority);
uint8 symbol_time_out_GetPriority(void);

void symbol_time_out_Enable(void);
uint8 symbol_time_out_GetState(void);
void symbol_time_out_Disable(void);

void symbol_time_out_SetPending(void);
void symbol_time_out_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the symbol_time_out ISR. */
#define symbol_time_out_INTC_VECTOR            ((reg32 *) symbol_time_out__INTC_VECT)

/* Address of the symbol_time_out ISR priority. */
#define symbol_time_out_INTC_PRIOR             ((reg32 *) symbol_time_out__INTC_PRIOR_REG)

/* Priority of the symbol_time_out interrupt. */
#define symbol_time_out_INTC_PRIOR_NUMBER      symbol_time_out__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable symbol_time_out interrupt. */
#define symbol_time_out_INTC_SET_EN            ((reg32 *) symbol_time_out__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the symbol_time_out interrupt. */
#define symbol_time_out_INTC_CLR_EN            ((reg32 *) symbol_time_out__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the symbol_time_out interrupt state to pending. */
#define symbol_time_out_INTC_SET_PD            ((reg32 *) symbol_time_out__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the symbol_time_out interrupt. */
#define symbol_time_out_INTC_CLR_PD            ((reg32 *) symbol_time_out__INTC_CLR_PD_REG)



#endif /* CY_ISR_symbol_time_out_H */


/* [] END OF FILE */
