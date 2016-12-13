/*******************************************************************************
* File Name: debounce_clock.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_debounce_clock_H) /* CY_CONTROL_REG_debounce_clock_H */
#define CY_CONTROL_REG_debounce_clock_H

#include "cytypes.h"

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} debounce_clock_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    debounce_clock_Write(uint8 control) ;
uint8   debounce_clock_Read(void) ;

void debounce_clock_SaveConfig(void) ;
void debounce_clock_RestoreConfig(void) ;
void debounce_clock_Sleep(void) ; 
void debounce_clock_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define debounce_clock_Control        (* (reg8 *) debounce_clock_Sync_ctrl_reg__CONTROL_REG )
#define debounce_clock_Control_PTR    (  (reg8 *) debounce_clock_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_debounce_clock_H */


/* [] END OF FILE */
