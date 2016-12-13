/*
**
** File: cf_dth22.c
**
**
**
*/



#include <project.h>
#include "process_modbus_message.h"
#include "cf_external_functions.h"
#include "dht22.h"


#define NUMBER_OF_DHT22_DEVICES 1

const DHT22_SENSOR dht22_sensor[ NUMBER_OF_DHT22_DEVICES] = 
{
    { temp_humidity_pin_Read, temp_humidity_pin_Write },
    
};

int measure_air_temperature(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    uint32 device_found;
    uint32 step_number;
    float temperature;
    float humidity;
    
    if( param_1 >= NUMBER_OF_DHT22_DEVICES )
    {
        param_1 = NUMBER_OF_DHT22_DEVICES -1;
    }
   

    DHT22_get_data( &dht22_sensor[param_1], &device_found, &step_number, &temperature, &humidity );    
    store_modbus_data_registers(AIR_HUMIDITY_FLOAT, 2, (uint16 *)&humidity);
    store_modbus_data_registers( AIR_TEMP_FLOAT , 2, (uint16 *)&temperature);
    return CF_DISABLE;
}
 