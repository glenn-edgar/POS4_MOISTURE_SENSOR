/*
**
** File: moisture_measurements.c
**
**
**
*/

#include <project.h>
#include "cf_external_functions.h"
#include "moisture_measurements.h"
#include "process_modbus_message.h"
#include "configuration_data.h"
#include "process_modbus_message.h"




#define SOURCE_CHANNEL   AMux_2_CHANNELS - 2
#define SINK_CHANNEL     AMux_2_CHANNELS - 1
#define DELTA_LIMIT      5
#define COMPARISON_OP    >

static float sink_measurement;
static float source_measurement;
static volatile float voltage_measurements[ AMux_2_CHANNELS ]; 
static volatile float resistance_array[ AMux_2_CHANNELS ];
static float make_measurement( uint32 channel );

static float calculate_resistor_current( uint32 channel, CONFIGURATION_DATA *ptr )
{
         float resistor_current;
         float delta_voltage;
    
         delta_voltage =      (source_measurement -  voltage_measurements[channel]);
         if( delta_voltage < DELTA_LIMIT )
         {
            delta_voltage = DELTA_LIMIT;
         }
           resistor_current =  delta_voltage/ptr->resistance_value;

          return resistor_current;
}
    
static float make_measurement( uint32 channel )
{
    AMux_2_FastSelect( channel );
    
    ADC_SAR_Seq_1_StartConvert();
    CyDelay(2);
    
    return (float )ADC_SAR_Seq_1_GetResult16(0);
}


int init_moisture_processing(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   remove_power(link_id, param_1,param_2,param_3,event,data);
   clear_new_moisture_measurement_flag(link_id, param_1,param_2,param_3,event,data);
  
   return CF_DISABLE;
}


int clear_new_moisture_measurement_flag(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    uint16 temp;
    temp = 0;
    store_modbus_data_registers( NEW_MOISTURE_DATA_FLAG , 1, &temp);
    return CF_DISABLE;

}

int set_new_moisture_measurement_flag(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    uint16 temp;
    temp = 1;
    store_modbus_data_registers( NEW_MOISTURE_DATA_FLAG , 1, &temp);
    return CF_DISABLE;

}

int set_source_channel(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{

   Source_Control_Write(3);
   Sink_Control_Write(0);

   return CF_DISABLE;
}


int make_source_measurement(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   int i;
   CONFIGURATION_DATA *ptr;
   uint16             *resistor_ptr;

   ptr = get_configuration_data(); 
   resistor_ptr = ptr->resistance_type;
   memset( (void *)voltage_measurements,0 , sizeof( voltage_measurements));
   for( i = 0; i <RESISTIVE_SENSOR_NUMBER; i++)
   {
      if(  *resistor_ptr COMPARISON_OP NOT_POPULATED )
      {

        voltage_measurements[i] =  make_measurement(i);
      }
      resistor_ptr++;
   }

   sink_measurement = make_measurement( SINK_CHANNEL );
   source_measurement = make_measurement( SOURCE_CHANNEL );

   return CF_DISABLE;
}

int set_sink_channel(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   Source_Control_Write(0);
   Sink_Control_Write(3);


   return CF_DISABLE;
}


int make_dummy_measurement(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   int i;
   CONFIGURATION_DATA *ptr;
   uint16             *resistor_ptr;

   ptr = get_configuration_data(); 
   resistor_ptr = ptr->resistance_type;
   for( i = 0; i <RESISTIVE_SENSOR_NUMBER; i++)
   {

      if(  *resistor_ptr COMPARISON_OP NOT_POPULATED )
      {

        make_measurement(i);
      }
      resistor_ptr++;
   }

   make_measurement( SINK_CHANNEL );
   make_measurement( SOURCE_CHANNEL );


   return CF_DISABLE;
}


int remove_power(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   Source_Control_Write(0);
   Sink_Control_Write(0);


   return CF_DISABLE;
}


int update_new_measurement_available(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   int i;
   float              temp_value;
   uint16             *resistor_ptr;
   float              resistor_current;
   float              soil_temperature;
    
   CONFIGURATION_DATA *ptr;
   ptr = get_configuration_data(); 
   resistor_ptr = ptr->resistance_type;
   for( i = 0; i < RESISTIVE_SENSOR_NUMBER; i++)
   {
      if( (  *resistor_ptr > NOT_POPULATED ) != 0 )
    
      {
         resistor_current = calculate_resistor_current(i, ptr );
         resistance_array[i] =  ( voltage_measurements[i] - sink_measurement )/ resistor_current; 
        
  
      }
      else
      {
        resistance_array[i] = 0.;
      }
       store_modbus_data_registers( RESISTIVE_VALUE_1_FLOAT +(2*i), 2, (uint16 *) &resistance_array[i]);
      resistor_ptr++;

   }
   resistor_ptr = ptr->resistance_type;
   for( i = 0; i< 16; i++)
   {
      switch( *resistor_ptr )
      {
         case NOT_POPULATED:
             temp_value = 0;
             break;
         case RESISTOR:
             temp_value = resistance_array[i];
             break;
         case WATERMARK:
            get_modbus_data_registers( MOISTURE_SOIL_TEMP_FLOAT , 2, (uint16*) &soil_temperature );
            soil_temperature = ( soil_temperature-32)*5./9.;
            temp_value = resistance_array[i]/1000.;
            if( temp_value > 75. )
            {
                temp_value = 75.;  // formula not accurate for large values of R  around 102 term results in divide by zero
            }
            //kPa=(4.093+3.213*R)/(1-0.009733*R-0.01205*T)
            temp_value = ( 4.093 + 3.213* temp_value  )/( 1. - .009733*temp_value - .01205* soil_temperature );
            break;
      }
      store_modbus_data_registers( RESISTIVE_SENSOR_1_FLOAT +(2*i), 2, (uint16 *) &temp_value);
      resistor_ptr++;
  }

   // compute high level functions
   return CF_DISABLE;
}




