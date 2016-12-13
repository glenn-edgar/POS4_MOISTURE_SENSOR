/*******************************************************************************
* File Name: RS_485_TX_ENBABLE.h  
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

#if !defined(CY_PINS_RS_485_TX_ENBABLE_ALIASES_H) /* Pins RS_485_TX_ENBABLE_ALIASES_H */
#define CY_PINS_RS_485_TX_ENBABLE_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define RS_485_TX_ENBABLE_0			(RS_485_TX_ENBABLE__0__PC)
#define RS_485_TX_ENBABLE_0_PS		(RS_485_TX_ENBABLE__0__PS)
#define RS_485_TX_ENBABLE_0_PC		(RS_485_TX_ENBABLE__0__PC)
#define RS_485_TX_ENBABLE_0_DR		(RS_485_TX_ENBABLE__0__DR)
#define RS_485_TX_ENBABLE_0_SHIFT	(RS_485_TX_ENBABLE__0__SHIFT)
#define RS_485_TX_ENBABLE_0_INTR	((uint16)((uint16)0x0003u << (RS_485_TX_ENBABLE__0__SHIFT*2u)))

#define RS_485_TX_ENBABLE_INTR_ALL	 ((uint16)(RS_485_TX_ENBABLE_0_INTR))


#endif /* End Pins RS_485_TX_ENBABLE_ALIASES_H */


/* [] END OF FILE */
