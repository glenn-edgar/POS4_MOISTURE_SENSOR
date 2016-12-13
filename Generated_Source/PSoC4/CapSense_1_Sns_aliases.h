/*******************************************************************************
* File Name: CapSense_1_Sns.h  
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

#if !defined(CY_PINS_CapSense_1_Sns_ALIASES_H) /* Pins CapSense_1_Sns_ALIASES_H */
#define CY_PINS_CapSense_1_Sns_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define CapSense_1_Sns_0			(CapSense_1_Sns__0__PC)
#define CapSense_1_Sns_0_PS		(CapSense_1_Sns__0__PS)
#define CapSense_1_Sns_0_PC		(CapSense_1_Sns__0__PC)
#define CapSense_1_Sns_0_DR		(CapSense_1_Sns__0__DR)
#define CapSense_1_Sns_0_SHIFT	(CapSense_1_Sns__0__SHIFT)
#define CapSense_1_Sns_0_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__0__SHIFT*2u)))

#define CapSense_1_Sns_1			(CapSense_1_Sns__1__PC)
#define CapSense_1_Sns_1_PS		(CapSense_1_Sns__1__PS)
#define CapSense_1_Sns_1_PC		(CapSense_1_Sns__1__PC)
#define CapSense_1_Sns_1_DR		(CapSense_1_Sns__1__DR)
#define CapSense_1_Sns_1_SHIFT	(CapSense_1_Sns__1__SHIFT)
#define CapSense_1_Sns_1_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__1__SHIFT*2u)))

#define CapSense_1_Sns_2			(CapSense_1_Sns__2__PC)
#define CapSense_1_Sns_2_PS		(CapSense_1_Sns__2__PS)
#define CapSense_1_Sns_2_PC		(CapSense_1_Sns__2__PC)
#define CapSense_1_Sns_2_DR		(CapSense_1_Sns__2__DR)
#define CapSense_1_Sns_2_SHIFT	(CapSense_1_Sns__2__SHIFT)
#define CapSense_1_Sns_2_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__2__SHIFT*2u)))

#define CapSense_1_Sns_3			(CapSense_1_Sns__3__PC)
#define CapSense_1_Sns_3_PS		(CapSense_1_Sns__3__PS)
#define CapSense_1_Sns_3_PC		(CapSense_1_Sns__3__PC)
#define CapSense_1_Sns_3_DR		(CapSense_1_Sns__3__DR)
#define CapSense_1_Sns_3_SHIFT	(CapSense_1_Sns__3__SHIFT)
#define CapSense_1_Sns_3_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__3__SHIFT*2u)))

#define CapSense_1_Sns_4			(CapSense_1_Sns__4__PC)
#define CapSense_1_Sns_4_PS		(CapSense_1_Sns__4__PS)
#define CapSense_1_Sns_4_PC		(CapSense_1_Sns__4__PC)
#define CapSense_1_Sns_4_DR		(CapSense_1_Sns__4__DR)
#define CapSense_1_Sns_4_SHIFT	(CapSense_1_Sns__4__SHIFT)
#define CapSense_1_Sns_4_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__4__SHIFT*2u)))

#define CapSense_1_Sns_5			(CapSense_1_Sns__5__PC)
#define CapSense_1_Sns_5_PS		(CapSense_1_Sns__5__PS)
#define CapSense_1_Sns_5_PC		(CapSense_1_Sns__5__PC)
#define CapSense_1_Sns_5_DR		(CapSense_1_Sns__5__DR)
#define CapSense_1_Sns_5_SHIFT	(CapSense_1_Sns__5__SHIFT)
#define CapSense_1_Sns_5_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__5__SHIFT*2u)))

#define CapSense_1_Sns_6			(CapSense_1_Sns__6__PC)
#define CapSense_1_Sns_6_PS		(CapSense_1_Sns__6__PS)
#define CapSense_1_Sns_6_PC		(CapSense_1_Sns__6__PC)
#define CapSense_1_Sns_6_DR		(CapSense_1_Sns__6__DR)
#define CapSense_1_Sns_6_SHIFT	(CapSense_1_Sns__6__SHIFT)
#define CapSense_1_Sns_6_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__6__SHIFT*2u)))

#define CapSense_1_Sns_7			(CapSense_1_Sns__7__PC)
#define CapSense_1_Sns_7_PS		(CapSense_1_Sns__7__PS)
#define CapSense_1_Sns_7_PC		(CapSense_1_Sns__7__PC)
#define CapSense_1_Sns_7_DR		(CapSense_1_Sns__7__DR)
#define CapSense_1_Sns_7_SHIFT	(CapSense_1_Sns__7__SHIFT)
#define CapSense_1_Sns_7_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__7__SHIFT*2u)))

#define CapSense_1_Sns_INTR_ALL	 ((uint16)(CapSense_1_Sns_0_INTR| CapSense_1_Sns_1_INTR| CapSense_1_Sns_2_INTR| CapSense_1_Sns_3_INTR| CapSense_1_Sns_4_INTR| CapSense_1_Sns_5_INTR| CapSense_1_Sns_6_INTR| CapSense_1_Sns_7_INTR))
#define CapSense_1_Sns_Generic0_0__GEN			(CapSense_1_Sns__Generic0_0__GEN__PC)
#define CapSense_1_Sns_Generic0_0__GEN_PS		(CapSense_1_Sns__Generic0_0__GEN__PS)
#define CapSense_1_Sns_Generic0_0__GEN_PC		(CapSense_1_Sns__Generic0_0__GEN__PC)
#define CapSense_1_Sns_Generic0_0__GEN_DR		(CapSense_1_Sns__Generic0_0__GEN__DR)
#define CapSense_1_Sns_Generic0_0__GEN_SHIFT	(CapSense_1_Sns__Generic0_0__GEN__SHIFT)
#define CapSense_1_Sns_Generic0_0__GEN_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__0__SHIFT*2u)))

#define CapSense_1_Sns_Generic1_0__GEN			(CapSense_1_Sns__Generic1_0__GEN__PC)
#define CapSense_1_Sns_Generic1_0__GEN_PS		(CapSense_1_Sns__Generic1_0__GEN__PS)
#define CapSense_1_Sns_Generic1_0__GEN_PC		(CapSense_1_Sns__Generic1_0__GEN__PC)
#define CapSense_1_Sns_Generic1_0__GEN_DR		(CapSense_1_Sns__Generic1_0__GEN__DR)
#define CapSense_1_Sns_Generic1_0__GEN_SHIFT	(CapSense_1_Sns__Generic1_0__GEN__SHIFT)
#define CapSense_1_Sns_Generic1_0__GEN_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__1__SHIFT*2u)))

#define CapSense_1_Sns_Generic2_0__GEN			(CapSense_1_Sns__Generic2_0__GEN__PC)
#define CapSense_1_Sns_Generic2_0__GEN_PS		(CapSense_1_Sns__Generic2_0__GEN__PS)
#define CapSense_1_Sns_Generic2_0__GEN_PC		(CapSense_1_Sns__Generic2_0__GEN__PC)
#define CapSense_1_Sns_Generic2_0__GEN_DR		(CapSense_1_Sns__Generic2_0__GEN__DR)
#define CapSense_1_Sns_Generic2_0__GEN_SHIFT	(CapSense_1_Sns__Generic2_0__GEN__SHIFT)
#define CapSense_1_Sns_Generic2_0__GEN_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__2__SHIFT*2u)))

#define CapSense_1_Sns_Generic3_0__GEN			(CapSense_1_Sns__Generic3_0__GEN__PC)
#define CapSense_1_Sns_Generic3_0__GEN_PS		(CapSense_1_Sns__Generic3_0__GEN__PS)
#define CapSense_1_Sns_Generic3_0__GEN_PC		(CapSense_1_Sns__Generic3_0__GEN__PC)
#define CapSense_1_Sns_Generic3_0__GEN_DR		(CapSense_1_Sns__Generic3_0__GEN__DR)
#define CapSense_1_Sns_Generic3_0__GEN_SHIFT	(CapSense_1_Sns__Generic3_0__GEN__SHIFT)
#define CapSense_1_Sns_Generic3_0__GEN_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__3__SHIFT*2u)))

#define CapSense_1_Sns_Generic4_0__GEN			(CapSense_1_Sns__Generic4_0__GEN__PC)
#define CapSense_1_Sns_Generic4_0__GEN_PS		(CapSense_1_Sns__Generic4_0__GEN__PS)
#define CapSense_1_Sns_Generic4_0__GEN_PC		(CapSense_1_Sns__Generic4_0__GEN__PC)
#define CapSense_1_Sns_Generic4_0__GEN_DR		(CapSense_1_Sns__Generic4_0__GEN__DR)
#define CapSense_1_Sns_Generic4_0__GEN_SHIFT	(CapSense_1_Sns__Generic4_0__GEN__SHIFT)
#define CapSense_1_Sns_Generic4_0__GEN_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__4__SHIFT*2u)))

#define CapSense_1_Sns_Generic5_0__GEN			(CapSense_1_Sns__Generic5_0__GEN__PC)
#define CapSense_1_Sns_Generic5_0__GEN_PS		(CapSense_1_Sns__Generic5_0__GEN__PS)
#define CapSense_1_Sns_Generic5_0__GEN_PC		(CapSense_1_Sns__Generic5_0__GEN__PC)
#define CapSense_1_Sns_Generic5_0__GEN_DR		(CapSense_1_Sns__Generic5_0__GEN__DR)
#define CapSense_1_Sns_Generic5_0__GEN_SHIFT	(CapSense_1_Sns__Generic5_0__GEN__SHIFT)
#define CapSense_1_Sns_Generic5_0__GEN_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__5__SHIFT*2u)))

#define CapSense_1_Sns_Generic6_0__GEN			(CapSense_1_Sns__Generic6_0__GEN__PC)
#define CapSense_1_Sns_Generic6_0__GEN_PS		(CapSense_1_Sns__Generic6_0__GEN__PS)
#define CapSense_1_Sns_Generic6_0__GEN_PC		(CapSense_1_Sns__Generic6_0__GEN__PC)
#define CapSense_1_Sns_Generic6_0__GEN_DR		(CapSense_1_Sns__Generic6_0__GEN__DR)
#define CapSense_1_Sns_Generic6_0__GEN_SHIFT	(CapSense_1_Sns__Generic6_0__GEN__SHIFT)
#define CapSense_1_Sns_Generic6_0__GEN_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__6__SHIFT*2u)))

#define CapSense_1_Sns_Generic7_0__GEN			(CapSense_1_Sns__Generic7_0__GEN__PC)
#define CapSense_1_Sns_Generic7_0__GEN_PS		(CapSense_1_Sns__Generic7_0__GEN__PS)
#define CapSense_1_Sns_Generic7_0__GEN_PC		(CapSense_1_Sns__Generic7_0__GEN__PC)
#define CapSense_1_Sns_Generic7_0__GEN_DR		(CapSense_1_Sns__Generic7_0__GEN__DR)
#define CapSense_1_Sns_Generic7_0__GEN_SHIFT	(CapSense_1_Sns__Generic7_0__GEN__SHIFT)
#define CapSense_1_Sns_Generic7_0__GEN_INTR	((uint16)((uint16)0x0003u << (CapSense_1_Sns__7__SHIFT*2u)))


#endif /* End Pins CapSense_1_Sns_ALIASES_H */


/* [] END OF FILE */
