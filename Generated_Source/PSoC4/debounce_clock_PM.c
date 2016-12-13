/*******************************************************************************
* File Name: debounce_clock_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "debounce_clock.h"

/* Check for removal by optimization */
#if !defined(debounce_clock_Sync_ctrl_reg__REMOVED)

static debounce_clock_BACKUP_STRUCT  debounce_clock_backup = {0u};

    
/*******************************************************************************
* Function Name: debounce_clock_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void debounce_clock_SaveConfig(void) 
{
    debounce_clock_backup.controlState = debounce_clock_Control;
}


/*******************************************************************************
* Function Name: debounce_clock_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void debounce_clock_RestoreConfig(void) 
{
     debounce_clock_Control = debounce_clock_backup.controlState;
}


/*******************************************************************************
* Function Name: debounce_clock_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void debounce_clock_Sleep(void) 
{
    debounce_clock_SaveConfig();
}


/*******************************************************************************
* Function Name: debounce_clock_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void debounce_clock_Wakeup(void)  
{
    debounce_clock_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
