/*******************************************************************************
* File Name: modbus_485_interface_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
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


/***************************************
* Local data allocation
***************************************/

static modbus_485_interface_BACKUP_STRUCT  modbus_485_interface_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: modbus_485_interface_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the modbus_485_interface_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  modbus_485_interface_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void modbus_485_interface_SaveConfig(void)
{
    #if(modbus_485_interface_CONTROL_REG_REMOVED == 0u)
        modbus_485_interface_backup.cr = modbus_485_interface_CONTROL_REG;
    #endif /* End modbus_485_interface_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: modbus_485_interface_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  modbus_485_interface_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling modbus_485_interface_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void modbus_485_interface_RestoreConfig(void)
{
    #if(modbus_485_interface_CONTROL_REG_REMOVED == 0u)
        modbus_485_interface_CONTROL_REG = modbus_485_interface_backup.cr;
    #endif /* End modbus_485_interface_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: modbus_485_interface_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The modbus_485_interface_Sleep() API saves the current component state. Then it
*  calls the modbus_485_interface_Stop() function and calls 
*  modbus_485_interface_SaveConfig() to save the hardware configuration.
*  Call the modbus_485_interface_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  modbus_485_interface_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void modbus_485_interface_Sleep(void)
{
    #if(modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED)
        if((modbus_485_interface_RXSTATUS_ACTL_REG  & modbus_485_interface_INT_ENABLE) != 0u)
        {
            modbus_485_interface_backup.enableState = 1u;
        }
        else
        {
            modbus_485_interface_backup.enableState = 0u;
        }
    #else
        if((modbus_485_interface_TXSTATUS_ACTL_REG  & modbus_485_interface_INT_ENABLE) !=0u)
        {
            modbus_485_interface_backup.enableState = 1u;
        }
        else
        {
            modbus_485_interface_backup.enableState = 0u;
        }
    #endif /* End modbus_485_interface_RX_ENABLED || modbus_485_interface_HD_ENABLED*/

    modbus_485_interface_Stop();
    modbus_485_interface_SaveConfig();
}


/*******************************************************************************
* Function Name: modbus_485_interface_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  modbus_485_interface_Sleep() was called. The modbus_485_interface_Wakeup() function
*  calls the modbus_485_interface_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  modbus_485_interface_Sleep() function was called, the modbus_485_interface_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  modbus_485_interface_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void modbus_485_interface_Wakeup(void)
{
    modbus_485_interface_RestoreConfig();
    #if( (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) )
        modbus_485_interface_ClearRxBuffer();
    #endif /* End (modbus_485_interface_RX_ENABLED) || (modbus_485_interface_HD_ENABLED) */
    #if(modbus_485_interface_TX_ENABLED || modbus_485_interface_HD_ENABLED)
        modbus_485_interface_ClearTxBuffer();
    #endif /* End modbus_485_interface_TX_ENABLED || modbus_485_interface_HD_ENABLED */

    if(modbus_485_interface_backup.enableState != 0u)
    {
        modbus_485_interface_Enable();
    }
}


/* [] END OF FILE */
