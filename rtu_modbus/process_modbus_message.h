
/*
**
** File: process_modbus_message.h
**
**/

#ifndef _PROCESS_MODBUS_MESSAGE_H_
#define _PROCESS_MODBUS_MESSAGE_H_
    
#include <project.h>

    
#define MODBUS_REGISTER_LENGTH 256

/*
**
** Modbus Register Definitions
**
*/ 


    
// System Definitions
    
#define MOD_UNIT_ID                     0

#define MOD_RTU_WATCH_DOG_FLAG          1
#define MOD_CONTROLLER_WATCH_DOG_FLAG   2
#define MOD_COMMISSIONING_FLAG          3
#define MOD_POWER_UP_EVENT              4
#define MOD_RESET_REASON                5
#define MOD_MINUTE_ROLLOVER             6

// contains 4 modbus registers 64 bit.
#define MOD_RTU_UNIX_TIME               8


#define ONE_WIRE_DEVICE_FOUND              13  
#define NEW_MOISTURE_DATA_FLAG             14
#define AIR_HUMIDITY_FLOAT                 15        
#define AIR_TEMP_FLOAT                     17
#define MOISTURE_SOIL_TEMP_FLOAT           19
#define RESISTOR_FLOAT                     21

  
    
#define RESISTIVE_SENSOR_1_FLOAT         30
#define RESISTIVE_SENSOR_2_FLOAT         32
#define RESISTIVE_SENSOR_3_FLOAT         34
#define RESISTIVE_SENSOR_4_FLOAT         36
#define RESISTIVE_SENSOR_5_FLOAT         38
#define RESISTIVE_SENSOR_6_FLOAT         40
#define RESISTIVE_SENSOR_7_FLOAT         42
#define RESISTIVE_SENSOR_8_FLOAT         44   
#define RESISTIVE_SENSOR_9_FLOAT         46
#define RESISTIVE_SENSOR_10_FLOAT        48
#define RESISTIVE_SENSOR_11_FLOAT        50
#define RESISTIVE_SENSOR_12_FLOAT        52
#define RESISTIVE_SENSOR_13_FLOAT        54
#define RESISTIVE_SENSOR_14_FLOAT        56
#define RESISTIVE_SENSOR_15_FLOAT        58
#define RESISTIVE_SENSOR_16_FLOAT        60   

#define RESISTIVE_VALUE_1_FLOAT         70
#define RESISTIVE_VALUE_2_FLOAT         72
#define RESISTIVE_VALUE_3_FLOAT         74
#define RESISTIVE_VALUE_4_FLOAT         76
#define RESISTIVE_VALUE_5_FLOAT         78
#define RESISTIVE_VALUE_6_FLOAT         80
#define RESISTIVE_VALUE_7_FLOAT         82
#define RESISTIVE_VALUE_8_FLOAT         84   
#define RESISTIVE_VALUE_9_FLOAT         86
#define RESISTIVE_VALUE_10_FLOAT        88
#define RESISTIVE_VALUE_11_FLOAT        90
#define RESISTIVE_VALUE_12_FLOAT        92
#define RESISTIVE_VALUE_13_FLOAT        94
#define RESISTIVE_VALUE_14_FLOAT        96
#define RESISTIVE_VALUE_15_FLOAT        98
#define RESISTIVE_VALUER_16_FLOAT      100   

    
   
    

// CONFIGURATION DATA
#define RESISTIVE_SENSOR_1_CONFIGURATION    110
#define RESISTIVE_SENSOR_2_CONFIGURATION    111
#define RESISTIVE_SENSOR_3_CONFIGURATION    112 
#define RESISTIVE_SENSOR_4_CONFIGURATION    113
#define RESISTIVE_SENSOR_5_CONFIGURATION    114
#define RESISTIVE_SENSOR_6_CONFIGURATION    115
#define RESISTIVE_SENSOR_7_CONFIGURATION    116
#define RESISTIVE_SENSOR_8_CONFIGURATION    117
#define RESISTIVE_SENSOR_9_CONFIGURATION    118
#define RESISTIVE_SENSOR_10_CONFIGURATION   119
#define RESISTIVE_SENSOR_11_CONFIGURATION   120 
#define RESISTIVE_SENSOR_12_CONFIGURATION   121
#define RESISTIVE_SENSOR_13_CONFIGURATION   122
#define RESISTIVE_SENSOR_14_CONFIGURATION   123
#define RESISTIVE_SENSOR_15_CONFIGURATION   124
#define RESISTIVE_SENSOR_16_CONFIGURATION   125


    



#define MODBUS_ARRAY_LENGTH  sizeof( modbus_address_data)/sizeof(modbus_address_data)


extern uint16 modbus_address_data[2];

static inline uint16 get_modbus_address( void )
{
    return modbus_address_data[0];
}

static inline uint16 get_commissioning_address( void )
{
    return modbus_address_data[1];
}



void initialize_modbus_registers( void );
// return out message length or 0 if error 
int process_modbus_message( int in_message_length, uint8 *in_message, int out_buffer_length, uint8 *out_buffer );

int store_modbus_data_registers( unsigned index, unsigned length, uint16 *new_data);
int get_modbus_data_registers( unsigned index, unsigned length, uint16 *buffer );
uint16 *get_modbus_register_address();

    
    
#endif