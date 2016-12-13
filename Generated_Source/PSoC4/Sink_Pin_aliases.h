/*******************************************************************************
* File Name: Sink_Pin.h  
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

#if !defined(CY_PINS_Sink_Pin_ALIASES_H) /* Pins Sink_Pin_ALIASES_H */
#define CY_PINS_Sink_Pin_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Sink_Pin_0			(Sink_Pin__0__PC)
#define Sink_Pin_0_PS		(Sink_Pin__0__PS)
#define Sink_Pin_0_PC		(Sink_Pin__0__PC)
#define Sink_Pin_0_DR		(Sink_Pin__0__DR)
#define Sink_Pin_0_SHIFT	(Sink_Pin__0__SHIFT)
#define Sink_Pin_0_INTR	((uint16)((uint16)0x0003u << (Sink_Pin__0__SHIFT*2u)))

#define Sink_Pin_INTR_ALL	 ((uint16)(Sink_Pin_0_INTR))


#endif /* End Pins Sink_Pin_ALIASES_H */


/* [] END OF FILE */
