/*
**
** File: modbus_interface_functions.c
**
**
**
**
*/
#include <device.h>
#include "modbus_interface_functions.h"
#include "die_temp.h"
#include "sigma_mux.h"
#include "a_to_d_functions.h"



/*
**
**  Type 3 modbus messages
**
*/


uint16  mod_access_read_temp( uint8 *buffer )
{
    uint16 *temp_ptr;
    temp_ptr = (uint16 *)buffer;
    *temp_ptr = die_get_temperature();
    
    return 2;
}    
 
/*
**
** This is test function only
**
**
*/
uint16 test_float_read( uint8 *buffer )
{
    float *temp_ptr;
    temp_ptr = (float *)buffer;
    *temp_ptr++ = 4.5;
    *temp_ptr++ = -4.5;
    return 2 * sizeof(float);
}

uint16 mod_read_current_time( uint8 *buffer )
{
    uint16 *temp_ptr;
    
    
    *buffer++ =  RTC_1_ReadSecond();
    *buffer++ =  RTC_1_ReadMinute();
    *buffer++ =  RTC_1_ReadHour();
    *buffer++ =   RTC_1_ReadMonth();
    temp_ptr =     (uint16 *)buffer;
    *temp_ptr = RTC_1_ReadYear();
    return  6;
}  


/*
**
**  Type 16 messages
**
*/

uint16  mod_store_current_time( uint8 *buffer )
{
    uint16 *temp_ptr;
    
    RTC_1_WriteSecond(*buffer++);   
    RTC_1_WriteMinute(*buffer++);  
    RTC_1_WriteHour( *buffer++ );
    RTC_1_WriteMonth(*buffer++);
    temp_ptr =     (uint16 *)buffer;
    RTC_1_WriteYear( *temp_ptr );

    return 0;
}

/*
**
** Store
**
*/


static uint32 long_store;
static float float_store;

uint16 mod_store_long( uint8 *buffer )
{
   uint32 *temp_ptr;
   temp_ptr = (uint32 *)buffer;
   long_store = *temp_ptr;
   return 0;
}


uint16 mod_store_float( uint8 *buffer )
{
    float *temp_ptr;
    temp_ptr = (float *)buffer;
    float_store = *temp_ptr;
    return 0;
}
