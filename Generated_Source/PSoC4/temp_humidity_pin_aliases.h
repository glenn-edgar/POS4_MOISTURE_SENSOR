/*******************************************************************************
* File Name: temp_humidity_pin.h  
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

#if !defined(CY_PINS_temp_humidity_pin_ALIASES_H) /* Pins temp_humidity_pin_ALIASES_H */
#define CY_PINS_temp_humidity_pin_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define temp_humidity_pin_0			(temp_humidity_pin__0__PC)
#define temp_humidity_pin_0_PS		(temp_humidity_pin__0__PS)
#define temp_humidity_pin_0_PC		(temp_humidity_pin__0__PC)
#define temp_humidity_pin_0_DR		(temp_humidity_pin__0__DR)
#define temp_humidity_pin_0_SHIFT	(temp_humidity_pin__0__SHIFT)
#define temp_humidity_pin_0_INTR	((uint16)((uint16)0x0003u << (temp_humidity_pin__0__SHIFT*2u)))

#define temp_humidity_pin_INTR_ALL	 ((uint16)(temp_humidity_pin_0_INTR))


#endif /* End Pins temp_humidity_pin_ALIASES_H */


/* [] END OF FILE */
