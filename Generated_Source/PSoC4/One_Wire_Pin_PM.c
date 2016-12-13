/*******************************************************************************
* File Name: One_Wire_Pin.c  
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
#include "One_Wire_Pin.h"

static One_Wire_Pin_BACKUP_STRUCT  One_Wire_Pin_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: One_Wire_Pin_Sleep
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
*  \snippet One_Wire_Pin_SUT.c usage_One_Wire_Pin_Sleep_Wakeup
*******************************************************************************/
void One_Wire_Pin_Sleep(void)
{
    #if defined(One_Wire_Pin__PC)
        One_Wire_Pin_backup.pcState = One_Wire_Pin_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            One_Wire_Pin_backup.usbState = One_Wire_Pin_CR1_REG;
            One_Wire_Pin_USB_POWER_REG |= One_Wire_Pin_USBIO_ENTER_SLEEP;
            One_Wire_Pin_CR1_REG &= One_Wire_Pin_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(One_Wire_Pin__SIO)
        One_Wire_Pin_backup.sioState = One_Wire_Pin_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        One_Wire_Pin_SIO_REG &= (uint32)(~One_Wire_Pin_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: One_Wire_Pin_Wakeup
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
*  Refer to One_Wire_Pin_Sleep() for an example usage.
*******************************************************************************/
void One_Wire_Pin_Wakeup(void)
{
    #if defined(One_Wire_Pin__PC)
        One_Wire_Pin_PC = One_Wire_Pin_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            One_Wire_Pin_USB_POWER_REG &= One_Wire_Pin_USBIO_EXIT_SLEEP_PH1;
            One_Wire_Pin_CR1_REG = One_Wire_Pin_backup.usbState;
            One_Wire_Pin_USB_POWER_REG &= One_Wire_Pin_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(One_Wire_Pin__SIO)
        One_Wire_Pin_SIO_REG = One_Wire_Pin_backup.sioState;
    #endif
}


/* [] END OF FILE */
