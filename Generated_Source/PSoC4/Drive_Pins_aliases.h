/*******************************************************************************
* File Name: Drive_Pins.h  
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

#if !defined(CY_PINS_Drive_Pins_ALIASES_H) /* Pins Drive_Pins_ALIASES_H */
#define CY_PINS_Drive_Pins_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Drive_Pins_0			(Drive_Pins__0__PC)
#define Drive_Pins_0_PS		(Drive_Pins__0__PS)
#define Drive_Pins_0_PC		(Drive_Pins__0__PC)
#define Drive_Pins_0_DR		(Drive_Pins__0__DR)
#define Drive_Pins_0_SHIFT	(Drive_Pins__0__SHIFT)
#define Drive_Pins_0_INTR	((uint16)((uint16)0x0003u << (Drive_Pins__0__SHIFT*2u)))

#define Drive_Pins_1			(Drive_Pins__1__PC)
#define Drive_Pins_1_PS		(Drive_Pins__1__PS)
#define Drive_Pins_1_PC		(Drive_Pins__1__PC)
#define Drive_Pins_1_DR		(Drive_Pins__1__DR)
#define Drive_Pins_1_SHIFT	(Drive_Pins__1__SHIFT)
#define Drive_Pins_1_INTR	((uint16)((uint16)0x0003u << (Drive_Pins__1__SHIFT*2u)))

#define Drive_Pins_INTR_ALL	 ((uint16)(Drive_Pins_0_INTR| Drive_Pins_1_INTR))


#endif /* End Pins Drive_Pins_ALIASES_H */


/* [] END OF FILE */
