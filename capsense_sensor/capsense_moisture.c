/*
**
** File:  capsense_moisture.c
**
**
**
*/


#include <project.h>
#include "capsense_moisture.h"
#include "cf_external_functions.h"
#include "configuration_data.h"
#include "process_modbus_message.h"


static float cap_array[ CAPSENSE_SENSORS];
int initialize_capacitance_measurements(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    
    
    
   CapSense_1_Start();
   
   
    return 0;
}
    


int make_capacitance_measurements(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    unsigned i;
    unsigned mask;
    CONFIGURATION_DATA *ptr; 
    float temp;
    

    mask = 1;
    ptr = get_configuration_data();
    
  
    for( i = 0; i < CAPSENSE_SENSORS; i++ )
    {
        if( (ptr->capacitance_mask & mask) != 0 )
        {
           CapSense_1_ScanSensor(i);
           while( CapSense_1_IsBusy() != 0 )
           {
                ;
           }
           temp = CapSense_1_ReadSensorRaw(i); 
          
        }
        else
        { 
            temp = 0;
        }
        cap_array[i] = temp;
        store_modbus_data_registers( CAPACITIVE_SENSOR_1_FLOAT + (2*i), 2, (uint16 *)&temp );
        mask = mask << 1;
    }
    
    return 0;
}



