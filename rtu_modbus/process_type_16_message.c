/*
**
** File: process_type_16_message.c
**
**
Query
Slave Address 11
Function 10
Starting Address Hi 00
Starting Address Lo 01
No. of Registers Hi 00
No. of Registers Lo 02
Byte Count 04
Data Hi 00
Data Lo 0A
Data Hi 01
Data Lo 02
Error Check (LRC or CRC) ––    
    
Response
Slave Address 11
Function 10
Starting Address Hi 00
Starting Address Lo 01
No. of Registers Hi 00
No. of Registers Lo 02
Error Check (LRC or CRC) ––    
    




**
**
*/




#include <project.h>
#include "process_modbus_message.h"
#include "process_type_16_message.h" 

#include "modbus_utilities.h"
#include "configuration_data.h"

#include "cf_events.h"



#define SWAP_16 0
#define SWAP_32 1


typedef uint16 (*FN_16)( uint8 *buffer );

typedef struct
{
    unsigned start_register;
    unsigned register_count;
    unsigned swap_type;
    FN_16   access;
    
}TYPE_16_STRUCT;      


static uint16 check_one_wire_presence( uint8 *buffer)
{
    
    cf_send_event( CF_CHECK_ONE_WIRE_PRESENCE, *buffer);
    return 1;
}


static uint16 make_soil_temperature( uint8 *buffer )
{
    cf_send_event( CF_MAKE_SOIL_TEMP_MEASUREMENT, *buffer  );
    return 1;
}


static uint16 make_air_temp_humidity( uint8 *buffer )
{
    cf_send_event( CF_MEASURE_AIR_TEMP_HUMIDITY, *buffer );
    return 1;
}






static uint16 clear_watch_dog_flags( uint8 * buffer )
{
    uint16 temp;
    
    temp =  0;
    store_modbus_data_registers( MOD_RTU_WATCH_DOG_FLAG, 1 , &temp);

    return 1;
}





static uint16 clear_power_up_event( uint8 *buffer )
{
    uint16 temp;
    
    temp = 0;
    store_modbus_data_registers( MOD_POWER_UP_EVENT, 1 , &temp);
    return 1;
}


static uint16 clear_minute_rollover( uint8 *buffer )
{
    uint16 temp;
    
    temp = 0;
    store_modbus_data_registers( MOD_MINUTE_ROLLOVER, 1 , &temp);
    return 1;
}

static uint16 set_controller_watch_dog_flags( uint8 * buffer )
{
    uint16 temp;
    
    temp =  1;
    store_modbus_data_registers( MOD_CONTROLLER_WATCH_DOG_FLAG, 1 , &temp);

    return 1;
}




static uint16 change_time( uint8 *buffer )
{
    int     i;
    uint8*  temp_ptr;
    uint64 time;
   
    temp_ptr = (uint8 *)&time;
    for( i = 0; i < 8; i++)
    {
        *temp_ptr++ = *buffer++;
    }
    
    RTC_1_SetUnixTime(time );
    store_modbus_data_registers( MOD_RTU_UNIX_TIME, 4 ,(uint16*) &time);
    return 1;
}




static uint16 update_moisture_sensor_configuration( uint8 *buffer )
{

   
    CONFIGURATION_DATA *ptr;
    
    ptr = get_configuration_data();

    store_modbus_data_registers( RESISTIVE_SENSOR_1_CONFIGURATION , RESISTIVE_SENSOR_NUMBER,(uint16 *) buffer );
    memcpy( ptr->resistance_type, buffer, sizeof(ptr->resistance_type));
    return 1;

}



static uint16 force_moisture_reading( uint8 *buffer )
{
    cf_send_event( CF_START_MOISTURE_READING, 0 );
    return 1;
}

static uint16 update_flash( uint8 *buffer )
{
    cf_send_event( CF_UPDATE_FLASH, 0 );
    return 1;
}

static uint16 change_modbus_address( uint8 *buffer )
{
    uint16 mod_commission_flag;
    uint16 return_value;
    CONFIGURATION_DATA *ptr;
    
    get_modbus_data_registers( MOD_COMMISSIONING_FLAG, 1, &mod_commission_flag );
    
    if( mod_commission_flag != 0  )
    {
        return_value = 1;
        ptr = get_configuration_data();
        memcpy( &ptr->modbus_address, buffer, sizeof(ptr->modbus_address));
        memcpy( modbus_address_data , buffer,  sizeof(modbus_address_data));
        cf_send_event( CF_UPDATE_FLASH, 0 );
        cf_send_event(  CF_COMMISSIONING_DONE, 0 );
    }
    else
    {
        return_value = 0;
    }    
    return return_value;
}

static uint16  clear_moisture_flag( uint8 *buffer )
{
    uint16 new_moisture_data_flag;
    
    new_moisture_data_flag = 0;
    store_modbus_data_registers( NEW_MOISTURE_DATA_FLAG, 1, &new_moisture_data_flag );
    return 1;
}    

#define TABLE_LENGTH 14
static const TYPE_16_STRUCT type_16_access_functions[TABLE_LENGTH] = 
{
    // System Level Writes
    { 20,    4,                                                 SWAP_16,  change_time                    },       
    { 21,    2,                                                 SWAP_16,  change_modbus_address          },            
    { 22,    1,                                                 SWAP_16,  clear_watch_dog_flags          },
    { 23,    1,                                                 SWAP_16,  clear_power_up_event           } ,
    { 25,    1,                                                 SWAP_16,  clear_minute_rollover          },
    { 26,    1,                                                 SWAP_16,  set_controller_watch_dog_flags  },
    { 27,    1,                                                 SWAP_16,  check_one_wire_presence         },
    { 28,    1,                                                 SWAP_16,  make_soil_temperature           },
    { 29,    1,                                                 SWAP_16,  make_air_temp_humidity          },

    { 30,    1,                                                 SWAP_16, force_moisture_reading       },
    { 31,    RESISTIVE_SENSOR_NUMBER,                           SWAP_16,  update_moisture_sensor_configuration          },
    
    { 33,    1,                                                 SWAP_16,  update_flash   },
    { 34,    1,                                                 SWAP_16,  clear_moisture_flag }
   
    
   
};
        


static  const TYPE_16_STRUCT *find_access_function( unsigned start_register, unsigned number );


int process_type_16_message( int in_length, uint8 *in, int out_length, uint8 *out )
{
    int       return_value;
    unsigned working_register;
    unsigned number_of_registers;
    unsigned count;
    unsigned i;
    unsigned swap_inc;
    unsigned swap_type;
    const TYPE_16_STRUCT     *acc_struct;
    uint8                    *ref_pointer;

    
    
    return_value = 0;
    *out++           = *in;
    working_register = (*in++)*256;
    *out++           = *in;
    working_register += (*in++);
    *out++           = *in;
    number_of_registers = (*in++)*256;
    *out++           = *in;
    number_of_registers += (*in++);
    count       = 4;
    in++;   // skip byte count;

    ref_pointer =  in;
    acc_struct = find_access_function( working_register, number_of_registers );
    
    if( acc_struct != NULL )
    {
        swap_type = acc_struct->swap_type;
    }
    else
    {
        swap_type = SWAP_16;
    }
        
    switch( swap_type)
    {
            case SWAP_16:
                swap_inc = 2;
                break;
            case SWAP_32:
                swap_inc = 4;
                break;
            default:
                // should not happen
                count = 0;
                return 0;
    }
    for( i = 0; i< number_of_registers*2; i+= swap_inc )
    {
            switch( swap_type)
            {
               case SWAP_16:
                  swap_16( in );
                  break;
               case SWAP_32:
                 swap_32( in);
                 break;
            default:
                // should not happen
                count = 0;
                return 0;
             }
             in += swap_inc;
     }
         
     if( acc_struct != NULL )
     {
        if( acc_struct->access( ref_pointer ) )
        {
            return_value = count;
        }
        else
        {
           return_value = 0;
        }
    }
    else
    {
        return_value = 0;
 #if 0
      if( store_modbus_data_registers( working_register, number_of_registers, (uint16 *) ref_pointer) != 0 )
      {
        return_value = count;
      }
      else
      {
        return_value = 0;
      }
#endif
    }
    if( return_value > 200 )
    {
        CYASSERT(0);
    }
    return return_value;
   
}    
                                                    
const TYPE_16_STRUCT * find_access_function( unsigned start_register, unsigned number )
{
   unsigned i;
   const TYPE_16_STRUCT *access_ptr;
   
   access_ptr = &type_16_access_functions[0];
   for( i = 0; i < TABLE_LENGTH; i++ )
   {
       if( (  access_ptr->start_register == start_register ) && (access_ptr->register_count == number))
       {
          return access_ptr;
       }
       else
       {
          access_ptr++;
       }
    }
    return NULL;
    
    
} 