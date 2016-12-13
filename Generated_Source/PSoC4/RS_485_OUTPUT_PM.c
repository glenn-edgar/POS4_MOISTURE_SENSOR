/*******************************************************************************
* File Name: RS_485_OUTPUT.c  
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
#include "RS_485_OUTPUT.h"

static RS_485_OUTPUT_BACKUP_STRUCT  RS_485_OUTPUT_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: RS_485_OUTPUT_Sleep
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
*  \snippet RS_485_OUTPUT_SUT.c usage_RS_485_OUTPUT_Sleep_Wakeup
*******************************************************************************/
void RS_485_OUTPUT_Sleep(void)
{
    #if defined(RS_485_OUTPUT__PC)
        RS_485_OUTPUT_backup.pcState = RS_485_OUTPUT_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            RS_485_OUTPUT_backup.usbState = RS_485_OUTPUT_CR1_REG;
            RS_485_OUTPUT_USB_POWER_REG |= RS_485_OUTPUT_USBIO_ENTER_SLEEP;
            RS_485_OUTPUT_CR1_REG &= RS_485_OUTPUT_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(RS_485_OUTPUT__SIO)
        RS_485_OUTPUT_backup.sioState = RS_485_OUTPUT_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        RS_485_OUTPUT_SIO_REG &= (uint32)(~RS_485_OUTPUT_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: RS_485_OUTPUT_Wakeup
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
*  Refer to RS_485_OUTPUT_Sleep() for an example usage.
*******************************************************************************/
void RS_485_OUTPUT_Wakeup(void)
{
    #if defined(RS_485_OUTPUT__PC)
        RS_485_OUTPUT_PC = RS_485_OUTPUT_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            RS_485_OUTPUT_USB_POWER_REG &= RS_485_OUTPUT_USBIO_EXIT_SLEEP_PH1;
            RS_485_OUTPUT_CR1_REG = RS_485_OUTPUT_backup.usbState;
            RS_485_OUTPUT_USB_POWER_REG &= RS_485_OUTPUT_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(RS_485_OUTPUT__SIO)
        RS_485_OUTPUT_SIO_REG = RS_485_OUTPUT_backup.sioState;
    #endif
}


/* [] END OF FILE */
