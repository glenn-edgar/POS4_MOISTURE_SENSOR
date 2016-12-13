/*******************************************************************************
* File Name: One_Wire_Pin.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_One_Wire_Pin_H) /* Pins One_Wire_Pin_H */
#define CY_PINS_One_Wire_Pin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "One_Wire_Pin_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} One_Wire_Pin_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   One_Wire_Pin_Read(void);
void    One_Wire_Pin_Write(uint8 value);
uint8   One_Wire_Pin_ReadDataReg(void);
#if defined(One_Wire_Pin__PC) || (CY_PSOC4_4200L) 
    void    One_Wire_Pin_SetDriveMode(uint8 mode);
#endif
void    One_Wire_Pin_SetInterruptMode(uint16 position, uint16 mode);
uint8   One_Wire_Pin_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void One_Wire_Pin_Sleep(void); 
void One_Wire_Pin_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(One_Wire_Pin__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define One_Wire_Pin_DRIVE_MODE_BITS        (3)
    #define One_Wire_Pin_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - One_Wire_Pin_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the One_Wire_Pin_SetDriveMode() function.
         *  @{
         */
        #define One_Wire_Pin_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define One_Wire_Pin_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define One_Wire_Pin_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define One_Wire_Pin_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define One_Wire_Pin_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define One_Wire_Pin_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define One_Wire_Pin_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define One_Wire_Pin_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define One_Wire_Pin_MASK               One_Wire_Pin__MASK
#define One_Wire_Pin_SHIFT              One_Wire_Pin__SHIFT
#define One_Wire_Pin_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in One_Wire_Pin_SetInterruptMode() function.
     *  @{
     */
        #define One_Wire_Pin_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define One_Wire_Pin_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define One_Wire_Pin_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define One_Wire_Pin_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(One_Wire_Pin__SIO)
    #define One_Wire_Pin_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(One_Wire_Pin__PC) && (CY_PSOC4_4200L)
    #define One_Wire_Pin_USBIO_ENABLE               ((uint32)0x80000000u)
    #define One_Wire_Pin_USBIO_DISABLE              ((uint32)(~One_Wire_Pin_USBIO_ENABLE))
    #define One_Wire_Pin_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define One_Wire_Pin_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define One_Wire_Pin_USBIO_ENTER_SLEEP          ((uint32)((1u << One_Wire_Pin_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << One_Wire_Pin_USBIO_SUSPEND_DEL_SHIFT)))
    #define One_Wire_Pin_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << One_Wire_Pin_USBIO_SUSPEND_SHIFT)))
    #define One_Wire_Pin_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << One_Wire_Pin_USBIO_SUSPEND_DEL_SHIFT)))
    #define One_Wire_Pin_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(One_Wire_Pin__PC)
    /* Port Configuration */
    #define One_Wire_Pin_PC                 (* (reg32 *) One_Wire_Pin__PC)
#endif
/* Pin State */
#define One_Wire_Pin_PS                     (* (reg32 *) One_Wire_Pin__PS)
/* Data Register */
#define One_Wire_Pin_DR                     (* (reg32 *) One_Wire_Pin__DR)
/* Input Buffer Disable Override */
#define One_Wire_Pin_INP_DIS                (* (reg32 *) One_Wire_Pin__PC2)

/* Interrupt configuration Registers */
#define One_Wire_Pin_INTCFG                 (* (reg32 *) One_Wire_Pin__INTCFG)
#define One_Wire_Pin_INTSTAT                (* (reg32 *) One_Wire_Pin__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define One_Wire_Pin_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(One_Wire_Pin__SIO)
    #define One_Wire_Pin_SIO_REG            (* (reg32 *) One_Wire_Pin__SIO)
#endif /* (One_Wire_Pin__SIO_CFG) */

/* USBIO registers */
#if !defined(One_Wire_Pin__PC) && (CY_PSOC4_4200L)
    #define One_Wire_Pin_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define One_Wire_Pin_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define One_Wire_Pin_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define One_Wire_Pin_DRIVE_MODE_SHIFT       (0x00u)
#define One_Wire_Pin_DRIVE_MODE_MASK        (0x07u << One_Wire_Pin_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins One_Wire_Pin_H */


/* [] END OF FILE */
