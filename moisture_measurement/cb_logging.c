/*
** File:  cb_logging.c
** Handles EEPROM Moisture Logging
**
*/
#include <device.h>
#include "cb_logging.h"
#include "process_modbus_message.h"

/*
Layout of EEPROM Format  in 4 bytes value
position 0 -- Initialization Flag  0xa5a55a5a
position 4 -- Number of 4 byte entries per entry 
position 8 -- Time in Minutes Between Logging Interval
position 12 --  Current Logged Number
position 16 --  start of Logging Data
position 16 + number of log elements * 4  -- start of Second Logs


 */
 




const float reference_resistance_array [ MOISTURE_SENSOR_NUMBER ] = 
{
  9930.,  // 0
  9930.,  // 1
  9930.,  // 2 
  9930.,  // 3
  9930.,  // 4
  9930.,  // 5
  9930.,   // 6
  9930.,   // 7
  9930.   // 8
  
};



const uint16 moisture_configuration_data[ MOISTURE_SENSOR_NUMBER ] =
{
   WATER_MARK,
   WATER_MARK,
   RESISTIVE_ELEMENT,
   RESISTIVE_ELEMENT,
   RESISTIVE_ELEMENT,
   RESISTIVE_ELEMENT,
   RESISTIVE_ELEMENT,
   RESISTIVE_ELEMENT,
   RESISTIVE_ELEMENT,
    
};

#define CALIBRATION_NUMBER 1
    
#define CALIBRATION_ARRAY_LENGTH  CALIBRATION_NUMBER *2



float get_reference_resistor( uint32 channel )
{
    uint16 return_value;
    if( channel >= MOISTURE_SENSOR_NUMBER )
    {
        CYASSERT(0);
    }
    else
    {
        return_value =reference_resistance_array[ channel];
    }
    return return_value;
}

uint16 get_moisture_sensor_configuration( unsigned sensor_id )
{
    uint16 return_value;
    if( sensor_id >= MOISTURE_SENSOR_NUMBER )
    {
        CYASSERT(0);
    }
    else
    {
        return_value = moisture_configuration_data[ sensor_id];
    }
    return return_value;
}


int store_default_resistor_calibration_values_to_modbus_registers(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    store_modbus_data_registers( REFERENCE_RESISTOR_1_FLOAT, MOISTURE_SENSOR_NUMBER *2 , (uint16*)reference_resistance_array);
    return 0;
}








int update_moisture_configuration(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    const uint16 *modbus_address;
    uint8 *buffer;
    CyGlobalIntDisable
    modbus_address = get_modbus_register_address();
    buffer = (uint8 *)&modbus_address[MOISTURE_SENSOR_1_CONFIGURATION];
    //cystatus Em_EEPROM_1_Write(const uint8 srcBuf[], const uint8 eepromPtr[], uint32 byteCount);
    Em_EEPROM_1_Write((uint8 *) moisture_configuration_data,buffer, sizeof(moisture_configuration_data));
    CYGlobalIntEnable
    return 1;
}


