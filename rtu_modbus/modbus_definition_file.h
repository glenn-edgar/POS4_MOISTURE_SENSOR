/*
**  File: modbus_register_definitions.h
**
**
**
**
**
*/

#ifndef MODBUS_REGISTER_DEFINITIONS_H
#define MODBUS_REGISTER_DEFINITIONS

#define MODBUS_MAX_NUMBER  128

// System Stuff

#define MOD_UNIT_ID           0
#define MOD_UNIT_TEMP         1
#define MOD_WATCH_DOG_FLAG    2
#define MOD_COMMISSIONING_FLAG    3
#define MOD_POWER_ONE_EVENT       4
#define MOD_MINUTE_ROLLOVER       5
#define MOD_DISCRETE_IO_CHANGE    6


// Digitial IO
#define MOD_FLOW_METER   20
#define DISCRETE_INPUT_1 22
#define DISCRETE_INPUT_2 23
#define DISCRETE_INPUT_3 24


// Analog IO
#define SUPPLY_CURRENT_PLC_DC  50
#define VALVE_CURRENT_PLC_DC   60
#define SUPPLY_WELL_CURRENT_DC 70
#define DRIVE_WELL_CURRENT_DC  80
#define WELL_PRESSURE_DC       90
#define FILTER_PRESSURE_DC    100


#endif