/*
**
** File: configuration_data.h
**
**
**
**
*/

#ifndef _CONFIGURATION_DATA_H
#define _CONFIGURATIION_DATA_H
    
#include <project.h>

typedef enum
{
   NOT_POPULATED = 0,
   RESISTOR      = 1,
   WATERMARK     = 2
}RESISTIVE_SENSOR_TYPE;
    
    
typedef packed struct
{
   uint8  modbus_address;
   float  resistance_1_value;
   float  resistance_2_value;
   uint32 resistance_mask;
   uint32 capacitance_mask;
   uint8  resistance_type[16];

}CONFIGURATION_DATA;


void initialize_configuration_data( void );
void restore_default_configuration_data( void );
void store_configuration_data( void );
CONFIGURATION_DATA *get_configuration_data();



    
    
#endif


