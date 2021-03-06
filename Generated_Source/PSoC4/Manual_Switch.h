/*******************************************************************************
* File Name: Manual_Switch.h  
* Version 1.90
*
* Description:
*  This file containts Status Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_STATUS_REG_Manual_Switch_H) /* CY_STATUS_REG_Manual_Switch_H */
#define CY_STATUS_REG_Manual_Switch_H

#include "cytypes.h"
#include "CyLib.h"

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 statusState;

} Manual_Switch_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

uint8 Manual_Switch_Read(void) ;
void Manual_Switch_InterruptEnable(void) ;
void Manual_Switch_InterruptDisable(void) ;
void Manual_Switch_WriteMask(uint8 mask) ;
uint8 Manual_Switch_ReadMask(void) ;


/***************************************
*           API Constants
***************************************/

#define Manual_Switch_STATUS_INTR_ENBL    0x10u


/***************************************
*         Parameter Constants
***************************************/

/* Status Register Inputs */
#define Manual_Switch_INPUTS              1


/***************************************
*             Registers
***************************************/

/* Status Register */
#define Manual_Switch_Status             (* (reg8 *) Manual_Switch_sts_sts_reg__STATUS_REG )
#define Manual_Switch_Status_PTR         (  (reg8 *) Manual_Switch_sts_sts_reg__STATUS_REG )
#define Manual_Switch_Status_Mask        (* (reg8 *) Manual_Switch_sts_sts_reg__MASK_REG )
#define Manual_Switch_Status_Aux_Ctrl    (* (reg8 *) Manual_Switch_sts_sts_reg__STATUS_AUX_CTL_REG )

#endif /* End CY_STATUS_REG_Manual_Switch_H */


/* [] END OF FILE */
