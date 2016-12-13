/*******************************************************************************
* File Name: timer_tick.h
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
#if !defined(CY_ISR_timer_tick_H)
#define CY_ISR_timer_tick_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void timer_tick_Start(void);
void timer_tick_StartEx(cyisraddress address);
void timer_tick_Stop(void);

CY_ISR_PROTO(timer_tick_Interrupt);

void timer_tick_SetVector(cyisraddress address);
cyisraddress timer_tick_GetVector(void);

void timer_tick_SetPriority(uint8 priority);
uint8 timer_tick_GetPriority(void);

void timer_tick_Enable(void);
uint8 timer_tick_GetState(void);
void timer_tick_Disable(void);

void timer_tick_SetPending(void);
void timer_tick_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the timer_tick ISR. */
#define timer_tick_INTC_VECTOR            ((reg32 *) timer_tick__INTC_VECT)

/* Address of the timer_tick ISR priority. */
#define timer_tick_INTC_PRIOR             ((reg32 *) timer_tick__INTC_PRIOR_REG)

/* Priority of the timer_tick interrupt. */
#define timer_tick_INTC_PRIOR_NUMBER      timer_tick__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable timer_tick interrupt. */
#define timer_tick_INTC_SET_EN            ((reg32 *) timer_tick__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the timer_tick interrupt. */
#define timer_tick_INTC_CLR_EN            ((reg32 *) timer_tick__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the timer_tick interrupt state to pending. */
#define timer_tick_INTC_SET_PD            ((reg32 *) timer_tick__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the timer_tick interrupt. */
#define timer_tick_INTC_CLR_PD            ((reg32 *) timer_tick__INTC_CLR_PD_REG)



#endif /* CY_ISR_timer_tick_H */


/* [] END OF FILE */
