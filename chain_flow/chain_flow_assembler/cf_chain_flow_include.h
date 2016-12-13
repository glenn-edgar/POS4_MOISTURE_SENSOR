/*
** Build Time: 2016-12-01 16:43:04.708000  
*/
#ifndef _CF_ASSEMBLER_H_
#define _CF_ASSEMBLER_H_



#define CHAIN_NUM   12
#define LINK_NUM    83



#include <project.h>

#include "cf_chain_flow_support.h" 
#include "cf_runtime_functions.h" 
#include "cf_external_functions.h" 
char  chain_state[12];
char  link_state[83];
unsigned link_data[83];
const unsigned start_state[]={1,1,1,1,1,1,1,1,1,1,0,1};



const LINK_CELL CHAIN_initialization_LINKS[]= 
{
{ one_step_fn,(CF_FUN_AUX)restore_system_parameters,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)initialize_modbus_bus,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)set_modbus_reset_flag,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)set_modbus_watch_dog_flag,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)initialize_analog,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)find_one_wire_devices,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)start_ds18B20_temperature_measurement,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ wait_time_fn,(CF_FUN_AUX)NULL,(unsigned)1000,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)read_ds18B20_temperature_measurement,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)measure_air_temperature,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)enable_interrupts,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)init_moisture_processing,(unsigned)0,(unsigned)0,(unsigned)0},
{ terminate_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};
const LINK_CELL CHAIN_second_tick_chain_LINKS[]= 
{
{ wait_event_fn,(CF_FUN_AUX)NULL,(unsigned)CF_SECOND_TICK,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)set_modbus_watch_dog_flag,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)update_modbus_rtc_values,(unsigned)0,(unsigned)0,(unsigned)0},
{ reset_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};
const LINK_CELL CHAIN_minute_tick_chain_LINKS[]= 
{
{ wait_event_fn,(CF_FUN_AUX)NULL,(unsigned)CF_MINUTE_TICK,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)set_minute_rollover_flag,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)measure_air_temperature,(unsigned)0,(unsigned)0,(unsigned)0},
{ reset_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};
const LINK_CELL CHAIN_hour_tick_chain_LINKS[]= 
{
{ wait_event_fn,(CF_FUN_AUX)NULL,(unsigned)CF_HOUR_TICK,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)start_ds18B20_temperature_measurement,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ wait_time_fn,(CF_FUN_AUX)NULL,(unsigned)1000,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)read_ds18B20_temperature_measurement,(unsigned)0,(unsigned)0,(unsigned)0},
{ reset_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};
const LINK_CELL CHAIN_measure_channel_LINKS[]= 
{
{ wait_event_fn,(CF_FUN_AUX)NULL,(unsigned)CF_START_MOISTURE_READING,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)clear_new_moisture_measurement_flag,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)set_source_channel,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)make_source_measurement,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)remove_power,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)set_sink_channel,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)make_dummy_measurement,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)remove_power,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)update_new_measurement_available,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)make_capacitance_measurements,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)set_new_moisture_measurement_flag,(unsigned)0,(unsigned)0,(unsigned)0},
{ reset_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};
const LINK_CELL CHAIN_one_wire_presence_LINKS[]= 
{
{ wait_event_fn,(CF_FUN_AUX)NULL,(unsigned)CF_CHECK_ONE_WIRE_PRESENCE,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)find_one_wire_devices,(unsigned)0,(unsigned)0,(unsigned)0},
{ reset_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};
const LINK_CELL CHAIN_make_soil_measurement_LINKS[]= 
{
{ wait_event_fn,(CF_FUN_AUX)NULL,(unsigned)CF_MAKE_SOIL_TEMP_MEASUREMENT,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)start_ds18B20_temperature_measurement,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ wait_time_fn,(CF_FUN_AUX)NULL,(unsigned)1000,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)read_ds18B20_temperature_measurement,(unsigned)0,(unsigned)0,(unsigned)0},
{ reset_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};
const LINK_CELL CHAIN_make_air_temperature_LINKS[]= 
{
{ wait_event_fn,(CF_FUN_AUX)NULL,(unsigned)CF_MEASURE_AIR_TEMP_HUMIDITY,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)read_ds18B20_temperature_measurement,(unsigned)0,(unsigned)0,(unsigned)0},
{ reset_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};
const LINK_CELL CHAIN_flash_update_LINKS[]= 
{
{ wait_event_fn,(CF_FUN_AUX)NULL,(unsigned)CF_UPDATE_FLASH,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)program_flash,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)restore_system_parameters,(unsigned)0,(unsigned)0,(unsigned)0},
{ reset_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};
const LINK_CELL CHAIN_heart_beat_LINKS[]= 
{
{ wait_time_fn,(CF_FUN_AUX)NULL,(unsigned)1000,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX) toggle_heart_beat,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ reset_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};
const LINK_CELL CHAIN_commissioning_heart_beat_LINKS[]= 
{
{ wait_time_fn,(CF_FUN_AUX)NULL,(unsigned)100,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX) toggle_heart_beat,(unsigned)0,(unsigned)0,(unsigned)0},
{ one_step_fn,(CF_FUN_AUX)pat_watch_dog,(unsigned)0,(unsigned)0,(unsigned)0},
{ reset_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};
const LINK_CELL CHAIN_handle_commissioning_LINKS[]= 
{
{ one_step_fn,(CF_FUN_AUX)set_normal_modbus_address,(unsigned)0,(unsigned)0,(unsigned)0},
{ disable_chain_fn,(CF_FUN_AUX)NULL,(unsigned)commissioning_heart_beat,(unsigned)-1,(unsigned)-1},
{ enable_chain_fn,(CF_FUN_AUX)NULL,(unsigned)heart_beat,(unsigned)-1,(unsigned)-1},
{ wait_condition_fn,(CF_FUN_AUX)switch_off_fn,(unsigned)0,(unsigned)0,(unsigned)0},
{ wait_condition_fn,(CF_FUN_AUX)switch_on_fn,(unsigned)0,(unsigned)0,(unsigned)0},
{ wait_condition_fn,(CF_FUN_AUX)switch_off_fn,(unsigned)0,(unsigned)0,(unsigned)0},
{ disable_chain_fn,(CF_FUN_AUX)NULL,(unsigned)heart_beat,(unsigned)-1,(unsigned)-1},
{ enable_chain_fn,(CF_FUN_AUX)NULL,(unsigned)commissioning_heart_beat,(unsigned)-1,(unsigned)-1},
{ one_step_fn,(CF_FUN_AUX)set_commissiong_address,(unsigned)0,(unsigned)0,(unsigned)0},
{ verify_condition_fn,(CF_FUN_AUX)switch_off_fn,(unsigned)0,(unsigned)0,(unsigned)0},
{ wait_event_fn,(CF_FUN_AUX)NULL,(unsigned)CF_COMMISSIONING_DONE,(unsigned)300000,(unsigned)0},
{ reset_fn,(CF_FUN_AUX)NULL,(unsigned)0,(unsigned)0,(unsigned)0},
};



const CHAIN_LINK chain_control[] =
{
{ 0,0,16,CHAIN_initialization_LINKS},
{ 16,1,5,CHAIN_second_tick_chain_LINKS},
{ 21,2,4,CHAIN_minute_tick_chain_LINKS},
{ 25,3,8,CHAIN_hour_tick_chain_LINKS},
{ 33,4,12,CHAIN_measure_channel_LINKS},
{ 45,5,3,CHAIN_one_wire_presence_LINKS},
{ 48,6,8,CHAIN_make_soil_measurement_LINKS},
{ 56,7,3,CHAIN_make_air_temperature_LINKS},
{ 59,8,4,CHAIN_flash_update_LINKS},
{ 63,9,4,CHAIN_heart_beat_LINKS},
{ 67,10,4,CHAIN_commissioning_heart_beat_LINKS},
{ 71,11,12,CHAIN_handle_commissioning_LINKS},
};
#endif
