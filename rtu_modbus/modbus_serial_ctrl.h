/*
**
** File modbus_serial_ctrl.h
**
**
*/

#ifndef _MODBUS_SERIAL_CTRL_H_
#define _MODBUS_SERIAL_CTRL_H_

#include "cf_chain_flow_support.h"
#include "cf_external_functions.h"
#include <project.h>

int initialize_modbus_rtu(void);
void modbus_timer_interrupt( void );
int process_rx_modbus_rtu_message(void );   
    
#endif 