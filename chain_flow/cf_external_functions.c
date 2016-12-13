/*
**
** File: cf_external_functions.c
** This file is manually generated
** 
**
*/
#include <project.h>
#include "cf_events.h"
#include "cf_status_bit.h"

#include "cf_external_functions.h"

#include "rtc_functions.h"
#include "process_modbus_message.h"
#include "modbus_serial_ctrl.h"
#include "configuration_data.h"
#include "analog_temperature_measurements.h"


extern uint16 modbus_address_data[2];

/*
**
** Initialize Function
**
**
*/
CY_ISR_PROTO( sleep_timer_interrupt )
{
   
    cf_set_interrupt_status_bit( CF_PROCESS_TIMER_TICK );  
    
}

/*
** Initialization Functions
**
**
**
**
**
*/


int enable_interrupts(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   CyGlobalIntEnable;
   return CF_DISABLE;
}



int initialize_analog(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   ADC_SAR_Seq_1_Start();

   return 0;
}


/*
**
** Toggle heart_beat
**
**
*/
int  toggle_heart_beat(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
	
           
   Heart_Beat_Pin_Write( !Heart_Beat_Pin_Read() );
   return CF_DISABLE;       

}



/*
**
**  One Second Interrupt
**
*/

int pat_watch_dog(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    uint8  criticalState;
    criticalState = CyEnterCriticalSection();
    CySysWdtUnlock();
    CySysWatchdogFeed(CY_SYS_WDT_COUNTER1);
    CySysWdtLock();
    CyExitCriticalSection(criticalState);
    return CF_DISABLE;
}






/*
**
**  modbus functions
**
**
**
*/


int initialize_modbus_bus(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    
    initialize_modbus_rtu();
    return 0;
}

/*
**
** One Minute Interrupt
**
*/
int set_minute_rollover_flag(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    uint16 temp;
    temp = 1;
    store_modbus_data_registers(MOD_MINUTE_ROLLOVER , 1, &temp);
    return 0;
}

int set_modbus_watch_dog_flag(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    uint16 temp;
    temp = 1;
    store_modbus_data_registers( MOD_RTU_WATCH_DOG_FLAG, 1, &temp);
    return 0;
}

int update_modbus_rtc_values(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    uint64 time;

    time = RTC_1_GetUnixTime();
    store_modbus_data_registers( MOD_RTU_UNIX_TIME, 4, (uint16 *)&time);
   
    return 0;
}

int restore_system_parameters(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   CONFIGURATION_DATA *ptr;
   initialize_modbus_registers();
   ptr = get_configuration_data();
   memcpy( modbus_address_data, ptr->modbus_address, sizeof(modbus_address_data) );
   store_modbus_data_registers( RESISTOR_FLOAT, 2, (uint16*)&ptr->resistance_value);

   store_modbus_data_registers( CAPACITANCE_SENSOR_MASK, 1, (uint16*)&ptr->capacitance_mask);
   store_modbus_data_registers( RESISTIVE_SENSOR_1_CONFIGURATION, RESISTIVE_SENSOR_NUMBER, (uint16*)&ptr->resistance_type);
   store_modbus_data_registers( MOD_UNIT_ID, 1,(uint16 *) &ptr->uint_id ); 
    return 0;
}






/*
    Reset reason values;
 
   CY_SYS_RESET_WDT       - WDT caused a reset
   CY_SYS_RESET_PROTFAULT - Occured protection violation that requires reset
   CY_SYS_RESET_SW        - Cortex-M0 requested a system reset.

*/



int set_modbus_reset_flag(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   uint16 temp;
   temp = 1;
   store_modbus_data_registers( MOD_POWER_UP_EVENT, 1, &temp);
   temp = CY_SYS_RES_CAUSE_REG;
   store_modbus_data_registers( MOD_RESET_REASON, 1, &temp);
   return 0;
}

/*
**
** Modbus Commissioning Functions
**
**
**
*/
extern  volatile uint32 switch_on;

int switch_on_fn(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   int return_value;
    
   if( switch_on == 0 )
   {
      return_value = 1;  // switch is active low
   }
   else
   {
      return_value = 0;
   }
   return return_value;
}


int switch_off_fn(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
   int return_value;
    
   if( switch_on == 0 )
   {
      return_value = 0; // switch is active low
   }
   else
   {
      return_value = 1;
   }
   return return_value;
}

int program_flash(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
    store_configuration_data(  );
    return 0;
}


