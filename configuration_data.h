/*
**
** File: configuration_data.h
**
**
**
**
*/

#ifndef _CONFIGURATION_DATA_H
#define _CONFIGURATION_DATA_H
    
#include <project.h>

#define RESISTIVE_SENSOR_NUMBER  8
    
typedef enum
{
   NOT_POPULATED = 0,
   RESISTOR      = 1,
   WATERMARK     = 2
}RESISTIVE_SENSOR_TYPE;
    
    
typedef struct
{
   uint16  uint_id;
   uint16  modbus_address[2];
   float  resistance_value;
   uint16 capacitance_mask;
   uint16  resistance_type[RESISTIVE_SENSOR_NUMBER];

}CONFIGURATION_DATA;

CONFIGURATION_DATA *get_configuration_data(void);
void initialize_configuration_data( void );
void restore_default_configuration_data( void );
void store_configuration_data( void );




    
    
#endif


