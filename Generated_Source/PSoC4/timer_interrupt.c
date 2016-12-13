/*******************************************************************************
* File Name: timer_interrupt.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <timer_interrupt.h>
#include "cyapicallbacks.h"

#if !defined(timer_interrupt__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START timer_interrupt_intc` */

/* `#END` */

extern cyisraddress CyRamVectors[CYINT_IRQ_BASE + CY_NUM_INTERRUPTS];

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: timer_interrupt_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it. This function disables the interrupt, 
*  sets the default interrupt vector, sets the priority from the value in the
*  Design Wide Resources Interrupt Editor, then enables the interrupt to the 
*  interrupt controller.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void timer_interrupt_Start(void)
{
    /* For all we know the interrupt is active. */
    timer_interrupt_Disable();

    /* Set the ISR to point to the timer_interrupt Interrupt. */
    timer_interrupt_SetVector(&timer_interrupt_Interrupt);

    /* Set the priority. */
    timer_interrupt_SetPriority((uint8)timer_interrupt_INTC_PRIOR_NUMBER);

    /* Enable it. */
    timer_interrupt_Enable();
}


/*******************************************************************************
* Function Name: timer_interrupt_StartEx
********************************************************************************
*
* Summary:
*  Sets up the interrupt and enables it. This function disables the interrupt,
*  sets the interrupt vector based on the address passed in, sets the priority 
*  from the value in the Design Wide Resources Interrupt Editor, then enables 
*  the interrupt to the interrupt controller.
*  
*  When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*  used to provide consistent definition across compilers:
*  
*  Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*   Function prototype example:
*   CY_ISR_PROTO(MyISR);
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void timer_interrupt_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    timer_interrupt_Disable();

    /* Set the ISR to point to the timer_interrupt Interrupt. */
    timer_interrupt_SetVector(address);

    /* Set the priority. */
    timer_interrupt_SetPriority((uint8)timer_interrupt_INTC_PRIOR_NUMBER);

    /* Enable it. */
    timer_interrupt_Enable();
}


/*******************************************************************************
* Function Name: timer_interrupt_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void timer_interrupt_Stop(void)
{
    /* Disable this interrupt. */
    timer_interrupt_Disable();

    /* Set the ISR to point to the passive one. */
    timer_interrupt_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: timer_interrupt_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for timer_interrupt.
*
*   Add custom code between the START and END comments to keep the next version
*   of this file from over-writing your code.
*
*   Note You may use either the default ISR by using this API, or you may define
*   your own separate ISR through ISR_StartEx().
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(timer_interrupt_Interrupt)
{
    #ifdef timer_interrupt_INTERRUPT_INTERRUPT_CALLBACK
        timer_interrupt_Interrupt_InterruptCallback();
    #endif /* timer_interrupt_INTERRUPT_INTERRUPT_CALLBACK */ 

    /*  Place your Interrupt code here. */
    /* `#START timer_interrupt_Interrupt` */

    /* `#END` */
}


/*******************************************************************************
* Function Name: timer_interrupt_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling timer_interrupt_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use timer_interrupt_StartEx instead.
* 
*   When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*   used to provide consistent definition across compilers:
*
*   Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*
*   Function prototype example:
*     CY_ISR_PROTO(MyISR);
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void timer_interrupt_SetVector(cyisraddress address)
{
    CyRamVectors[CYINT_IRQ_BASE + timer_interrupt__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: timer_interrupt_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress timer_interrupt_GetVector(void)
{
    return CyRamVectors[CYINT_IRQ_BASE + timer_interrupt__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: timer_interrupt_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. 
*
*   Note calling timer_interrupt_Start or timer_interrupt_StartEx will 
*   override any effect this API would have had. This API should only be called
*   after timer_interrupt_Start or timer_interrupt_StartEx has been called. 
*   To set the initial priority for the component, use the Design-Wide Resources
*   Interrupt Editor.
*
*   Note This API has no effect on Non-maskable interrupt NMI).
*
* Parameters:
*   priority: Priority of the interrupt, 0 being the highest priority
*             PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*             PSoC 4: Priority is from 0 to 3.
*
* Return:
*   None
*
*******************************************************************************/
void timer_interrupt_SetPriority(uint8 priority)
{
	uint8 interruptState;
    uint32 priorityOffset = ((timer_interrupt__INTC_NUMBER % 4u) * 8u) + 6u;
    
	interruptState = CyEnterCriticalSection();
    *timer_interrupt_INTC_PRIOR = (*timer_interrupt_INTC_PRIOR & (uint32)(~timer_interrupt__INTC_PRIOR_MASK)) |
                                    ((uint32)priority << priorityOffset);
	CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: timer_interrupt_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt, 0 being the highest priority
*    PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*    PSoC 4: Priority is from 0 to 3.
*
*******************************************************************************/
uint8 timer_interrupt_GetPriority(void)
{
    uint32 priority;
	uint32 priorityOffset = ((timer_interrupt__INTC_NUMBER % 4u) * 8u) + 6u;

    priority = (*timer_interrupt_INTC_PRIOR & timer_interrupt__INTC_PRIOR_MASK) >> priorityOffset;

    return (uint8)priority;
}


/*******************************************************************************
* Function Name: timer_interrupt_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt to the interrupt controller. Do not call this function
*   unless ISR_Start() has been called or the functionality of the ISR_Start() 
*   function, which sets the vector and the priority, has been called.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void timer_interrupt_Enable(void)
{
    /* Enable the general interrupt. */
    *timer_interrupt_INTC_SET_EN = timer_interrupt__INTC_MASK;
}


/*******************************************************************************
* Function Name: timer_interrupt_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 timer_interrupt_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*timer_interrupt_INTC_SET_EN & (uint32)timer_interrupt__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: timer_interrupt_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt in the interrupt controller.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void timer_interrupt_Disable(void)
{
    /* Disable the general interrupt. */
    *timer_interrupt_INTC_CLR_EN = timer_interrupt__INTC_MASK;
}


/*******************************************************************************
* Function Name: timer_interrupt_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
* Side Effects:
*   If interrupts are enabled and the interrupt is set up properly, the ISR is
*   entered (depending on the priority of this interrupt and other pending 
*   interrupts).
*
*******************************************************************************/
void timer_interrupt_SetPending(void)
{
    *timer_interrupt_INTC_SET_PD = timer_interrupt__INTC_MASK;
}


/*******************************************************************************
* Function Name: timer_interrupt_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt in the interrupt controller.
*
*   Note Some interrupt sources are clear-on-read and require the block 
*   interrupt/status register to be read/cleared with the appropriate block API 
*   (GPIO, UART, and so on). Otherwise the ISR will continue to remain in 
*   pending state even though the interrupt itself is cleared using this API.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void timer_interrupt_ClearPending(void)
{
    *timer_interrupt_INTC_CLR_PD = timer_interrupt__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
