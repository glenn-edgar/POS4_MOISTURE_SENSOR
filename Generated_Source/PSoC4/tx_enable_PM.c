/*******************************************************************************
* File Name: tx_enable.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "tx_enable.h"

static tx_enable_BACKUP_STRUCT  tx_enable_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: tx_enable_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function must be called for SIO and USBIO
*  pins. It is not essential if using GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet tx_enable_SUT.c usage_tx_enable_Sleep_Wakeup
*******************************************************************************/
void tx_enable_Sleep(void)
{
    #if defined(tx_enable__PC)
        tx_enable_backup.pcState = tx_enable_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            tx_enable_backup.usbState = tx_enable_CR1_REG;
            tx_enable_USB_POWER_REG |= tx_enable_USBIO_ENTER_SLEEP;
            tx_enable_CR1_REG &= tx_enable_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(tx_enable__SIO)
        tx_enable_backup.sioState = tx_enable_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        tx_enable_SIO_REG &= (uint32)(~tx_enable_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: tx_enable_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep().
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to tx_enable_Sleep() for an example usage.
*******************************************************************************/
void tx_enable_Wakeup(void)
{
    #if defined(tx_enable__PC)
        tx_enable_PC = tx_enable_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            tx_enable_USB_POWER_REG &= tx_enable_USBIO_EXIT_SLEEP_PH1;
            tx_enable_CR1_REG = tx_enable_backup.usbState;
            tx_enable_USB_POWER_REG &= tx_enable_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(tx_enable__SIO)
        tx_enable_SIO_REG = tx_enable_backup.sioState;
    #endif
}


/* [] END OF FILE */
