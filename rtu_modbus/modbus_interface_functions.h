/*
**
** File: modbus_interface_functions.h
**
**
**
**
**
*/

#ifndef _MODBUS_INTERFACE_FUNCTIONS_H_
#define _MODBUS_INTERFACE_FUNCTIONS_H_

#include <device.h>


#define EEPROM_NUMBER 10   
   
uint16  mod_access_read_temp(  uint8 *buffer );
uint16  mod_read_current_time( uint8 *buffer );

uint16  mod_store_current_time( uint8 *buffer );
uint16  mod_eeprom_access( uint8 *buffer );    

uint16  test_float_read( uint8 *buffer );
uint16 mod_store_long( uint8 *buffer );
uint16 mod_store_float( uint8 *buffer );

#endif