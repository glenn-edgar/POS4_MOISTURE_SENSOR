/*******************************************************************************
* File Name: CapSense_2_Sns.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_CapSense_2_Sns_ALIASES_H) /* Pins CapSense_2_Sns_ALIASES_H */
#define CY_PINS_CapSense_2_Sns_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define CapSense_2_Sns_0			(CapSense_2_Sns__0__PC)
#define CapSense_2_Sns_0_PS		(CapSense_2_Sns__0__PS)
#define CapSense_2_Sns_0_PC		(CapSense_2_Sns__0__PC)
#define CapSense_2_Sns_0_DR		(CapSense_2_Sns__0__DR)
#define CapSense_2_Sns_0_SHIFT	(CapSense_2_Sns__0__SHIFT)
#define CapSense_2_Sns_0_INTR	((uint16)((uint16)0x0003u << (CapSense_2_Sns__0__SHIFT*2u)))

#define CapSense_2_Sns_INTR_ALL	 ((uint16)(CapSense_2_Sns_0_INTR))
#define CapSense_2_Sns_Generic0_0__GEN			(CapSense_2_Sns__Generic0_0__GEN__PC)
#define CapSense_2_Sns_Generic0_0__GEN_PS		(CapSense_2_Sns__Generic0_0__GEN__PS)
#define CapSense_2_Sns_Generic0_0__GEN_PC		(CapSense_2_Sns__Generic0_0__GEN__PC)
#define CapSense_2_Sns_Generic0_0__GEN_DR		(CapSense_2_Sns__Generic0_0__GEN__DR)
#define CapSense_2_Sns_Generic0_0__GEN_SHIFT	(CapSense_2_Sns__Generic0_0__GEN__SHIFT)
#define CapSense_2_Sns_Generic0_0__GEN_INTR	((uint16)((uint16)0x0003u << (CapSense_2_Sns__0__SHIFT*2u)))


#endif /* End Pins CapSense_2_Sns_ALIASES_H */


/* [] END OF FILE */
