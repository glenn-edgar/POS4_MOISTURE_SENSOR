/*
**
** File: one_wire.c
**
**
**
*/

#include <project.h>
#include "one_wire.h"
#include "cf_external_functions.h"
#include "DS18B20.h"
#include "process_modbus_message.h"

#define DEFAULT_TEMPERATURE 60.
static uint16 one_wire_device_found;

#define NUMBER_OF_ONE_WIRE_DEVICES 1

const ONE_WIRE_IO one_wire_io[ NUMBER_OF_ONE_WIRE_DEVICES] = 
{
    { One_Wire_Pin_Read, One_Wire_Pin_Write },
    
};

ONE_WIRE_ADDRESS one_wire_addresses[ NUMBER_OF_ONE_WIRE_DEVICES];




int find_one_wire_devices(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    if( param_1 >= NUMBER_OF_ONE_WIRE_DEVICES )
    {
        param_1 = NUMBER_OF_ONE_WIRE_DEVICES -1;
    }
    one_wire_device_found = DS1820_FindFirstDevice(&one_wire_io[param_1], &one_wire_addresses[param_1]);
    store_modbus_data_registers( ONE_WIRE_DEVICE_FOUND , 1, &one_wire_device_found );
     
    return CF_DISABLE;
}

int start_ds18B20_temperature_measurement(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    if( param_1 >= NUMBER_OF_ONE_WIRE_DEVICES )
    {
        param_1 = NUMBER_OF_ONE_WIRE_DEVICES -1;
    }

    if( one_wire_device_found != 0)
    {
       DS1820_StartTemp_Reading(&one_wire_io[param_1], &one_wire_addresses[param_1]);
    }
    return CF_DISABLE;
}

int read_ds18B20_temperature_measurement(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    float temperature;
    if( param_1 >= NUMBER_OF_ONE_WIRE_DEVICES )
    {
        param_1 = NUMBER_OF_ONE_WIRE_DEVICES -1;
    }

    if( one_wire_device_found != 0)
    {
       temperature = DS1820_GetTempRaw(&one_wire_io[param_1], &one_wire_addresses[param_1]);
       temperature = (((temperature/256.)*9.)/5.0)+32;
       store_modbus_data_registers( MOISTURE_SOIL_TEMP_FLOAT , 2,(uint16*) &temperature );
    }
    else
    {
       temperature = DEFAULT_TEMPERATURE;
       store_modbus_data_registers( MOISTURE_SOIL_TEMP_FLOAT , 2,(uint16*) &temperature );
    }
    return CF_DISABLE;
}
    