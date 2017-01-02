from helper_functions import  Helper_Functions
from assembler          import  CF_Assembler

cf = CF_Assembler()
hf = Helper_Functions( cf )


cf.define_chain("initialization",True)


hf.one_step("restore_system_parameters")
hf.one_step("initialize_modbus_bus")
hf.one_step("set_modbus_reset_flag")
hf.one_step("set_modbus_watch_dog_flag")

hf.one_step("initialize_analog")


hf.one_step("find_one_wire_devices",0)
hf.one_step("pat_watch_dog")
hf.one_step("start_ds18B20_temperature_measurement",0)
hf.one_step("pat_watch_dog")
hf.wait_time(1000) 
hf.one_step("pat_watch_dog")
hf.one_step("read_ds18B20_temperature_measurement",0)
hf.one_step("measure_air_temperature")
hf.one_step("enable_interrupts")
hf.one_step("init_moisture_processing")
hf.terminate()  #initialization is done now disable the chain
cf.end_chain()






#These chains are for actions every second 
cf.define_chain("second_tick_chain", True )
hf.wait_event("CF_SECOND_TICK")
hf.one_step("set_modbus_watch_dog_flag")
hf.one_step("pat_watch_dog")

hf.one_step("update_modbus_rtc_values")
hf.reset()
cf.end_chain()

#These chains are for actions every minute
cf.define_chain("minute_tick_chain", True )
hf.wait_event("CF_MINUTE_TICK")
hf.one_step("set_minute_rollover_flag")
hf.one_step("measure_air_temperature")
hf.reset()
cf.end_chain()



#These chains are for actions every hour
cf.define_chain("hour_tick_chain", True )
hf.wait_event("CF_HOUR_TICK")
hf.one_step("pat_watch_dog")
hf.one_step("start_ds18B20_temperature_measurement",0)
hf.one_step("pat_watch_dog")
hf.wait_time(1000) 
hf.one_step("pat_watch_dog")
hf.one_step("read_ds18B20_temperature_measurement",0)

hf.reset()
cf.end_chain()




cf.define_chain("measure_channel",True)
hf.wait_event("CF_START_MOISTURE_READING")
hf.one_step("clear_new_moisture_measurement_flag")
hf.one_step("set_source_channel")
hf.one_step("make_source_measurement")
hf.one_step("remove_power")
hf.one_step("set_sink_channel")
hf.one_step("make_dummy_measurement")
hf.one_step("remove_power")
hf.one_step("update_new_measurement_available")
hf.one_step("set_new_moisture_measurement_flag")
hf.reset()
cf.end_chain()



cf.define_chain("one_wire_presence",True)
hf.wait_event("CF_CHECK_ONE_WIRE_PRESENCE")
hf.one_step("find_one_wire_devices")
hf.reset()
cf.end_chain()


cf.define_chain("make_soil_measurement",True)
hf.wait_event("CF_MAKE_SOIL_TEMP_MEASUREMENT")
hf.one_step("pat_watch_dog")
hf.one_step("start_ds18B20_temperature_measurement",0)
hf.one_step("pat_watch_dog")
hf.wait_time(1000) 
hf.one_step("pat_watch_dog")
hf.one_step("read_ds18B20_temperature_measurement",0)

hf.reset()
cf.end_chain()


cf.define_chain("make_air_temperature",True)
hf.wait_event("CF_MEASURE_AIR_TEMP_HUMIDITY")
hf.one_step("read_ds18B20_temperature_measurement",0)
hf.reset()
cf.end_chain()





cf.define_chain("flash_update",True)
hf.wait_event("CF_UPDATE_FLASH")
hf.one_step("program_flash")
hf.one_step("restore_system_parameters")
hf.reset()
cf.end_chain()

cf.define_chain( "heart_beat",True)
hf.wait_time(1000)  #1000 ms
hf.one_step(" toggle_heart_beat")
hf.one_step("pat_watch_dog")
hf.reset()
cf.end_chain()

cf.define_chain( "commissioning_heart_beat",False)
hf.wait_time(100)  # 100 ms
hf.one_step(" toggle_heart_beat")
hf.one_step("pat_watch_dog")
hf.reset()
cf.end_chain()

#this chain is for commisisioning activities for modbus serial
cf.define_chain("handle_commissioning",True)
hf.one_step("set_normal_modbus_address")
hf.disable_chain("commissioning_heart_beat")
hf.enable_chain("heart_beat")
hf.wait(  "switch_off_fn" )
hf.wait(  "switch_on_fn" )
hf.wait(  "switch_off_fn" )
hf.disable_chain("heart_beat")
hf.enable_chain("commissioning_heart_beat")
hf.one_step("set_commissiong_address")
hf.verify( "switch_off_fn")
hf.wait_event("CF_COMMISSIONING_DONE",300000) # 5 minutes
hf.reset()
cf.end_chain()

cf.generate_c_header()


