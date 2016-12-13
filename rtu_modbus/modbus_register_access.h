/*
**
** File: modbus_register_access.h
**
**
**
*/

#ifndef MODBUS_REGISTER_ACCESS_H
#define MODBUS_REGISTER_ACCESS_H

#include <project.h>

void initialize_modbus_registers( void );

uint16 mod_get_register_16( int modbus_index );

void mod_store_register_16( int modbus_index, uint16 value );

float mod_get_register_float( int modbus_index );

void mod_store_register_float( int modbus_index, float value );



#endif