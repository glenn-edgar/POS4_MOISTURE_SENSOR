/*******************************************************************************
* File Name: modbus_485_interface_IntClock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_modbus_485_interface_IntClock_H)
#define CY_CLOCK_modbus_485_interface_IntClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void modbus_485_interface_IntClock_StartEx(uint32 alignClkDiv);
#define modbus_485_interface_IntClock_Start() \
    modbus_485_interface_IntClock_StartEx(modbus_485_interface_IntClock__PA_DIV_ID)

#else

void modbus_485_interface_IntClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void modbus_485_interface_IntClock_Stop(void);

void modbus_485_interface_IntClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 modbus_485_interface_IntClock_GetDividerRegister(void);
uint8  modbus_485_interface_IntClock_GetFractionalDividerRegister(void);

#define modbus_485_interface_IntClock_Enable()                         modbus_485_interface_IntClock_Start()
#define modbus_485_interface_IntClock_Disable()                        modbus_485_interface_IntClock_Stop()
#define modbus_485_interface_IntClock_SetDividerRegister(clkDivider, reset)  \
    modbus_485_interface_IntClock_SetFractionalDividerRegister((clkDivider), 0u)
#define modbus_485_interface_IntClock_SetDivider(clkDivider)           modbus_485_interface_IntClock_SetDividerRegister((clkDivider), 1u)
#define modbus_485_interface_IntClock_SetDividerValue(clkDivider)      modbus_485_interface_IntClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define modbus_485_interface_IntClock_DIV_ID     modbus_485_interface_IntClock__DIV_ID

#define modbus_485_interface_IntClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define modbus_485_interface_IntClock_CTRL_REG   (*(reg32 *)modbus_485_interface_IntClock__CTRL_REGISTER)
#define modbus_485_interface_IntClock_DIV_REG    (*(reg32 *)modbus_485_interface_IntClock__DIV_REGISTER)

#define modbus_485_interface_IntClock_CMD_DIV_SHIFT          (0u)
#define modbus_485_interface_IntClock_CMD_PA_DIV_SHIFT       (8u)
#define modbus_485_interface_IntClock_CMD_DISABLE_SHIFT      (30u)
#define modbus_485_interface_IntClock_CMD_ENABLE_SHIFT       (31u)

#define modbus_485_interface_IntClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << modbus_485_interface_IntClock_CMD_DISABLE_SHIFT))
#define modbus_485_interface_IntClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << modbus_485_interface_IntClock_CMD_ENABLE_SHIFT))

#define modbus_485_interface_IntClock_DIV_FRAC_MASK  (0x000000F8u)
#define modbus_485_interface_IntClock_DIV_FRAC_SHIFT (3u)
#define modbus_485_interface_IntClock_DIV_INT_MASK   (0xFFFFFF00u)
#define modbus_485_interface_IntClock_DIV_INT_SHIFT  (8u)

#else 

#define modbus_485_interface_IntClock_DIV_REG        (*(reg32 *)modbus_485_interface_IntClock__REGISTER)
#define modbus_485_interface_IntClock_ENABLE_REG     modbus_485_interface_IntClock_DIV_REG
#define modbus_485_interface_IntClock_DIV_FRAC_MASK  modbus_485_interface_IntClock__FRAC_MASK
#define modbus_485_interface_IntClock_DIV_FRAC_SHIFT (16u)
#define modbus_485_interface_IntClock_DIV_INT_MASK   modbus_485_interface_IntClock__DIVIDER_MASK
#define modbus_485_interface_IntClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_modbus_485_interface_IntClock_H) */

/* [] END OF FILE */
