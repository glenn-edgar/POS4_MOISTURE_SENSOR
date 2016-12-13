/*
**
** File: process_modbus_message.c
** This file processes the modbus message from
** either the usb or serial handler
**
*/


#include <project.h>
#include <assert.h>

#include "process_crc.h"
#include "process_type_3_message.h"
#include "process_type_16_message.h"
#include "process_type_24_message.h"

#include "process_modbus_message.h"



// logging interval is in minutes
// default sampling Interval is one hour

#define SAMPLING_INTERVAL   60 

static uint16 modbus_registers[ MODBUS_REGISTER_LENGTH ];





uint16 modbus_address_data[2];


uint16 *get_modbus_register_address( void )
{
    return modbus_registers;
}

void initialize_modbus_registers( void )
{
    uint16 temp;

 
    memset( modbus_registers, 0, sizeof(modbus_registers));

    temp = 1;
    store_modbus_data_registers( MOD_RTU_WATCH_DOG_FLAG, 1, &temp);
    
    
}
  


int store_modbus_data_registers( unsigned index, unsigned length, uint16 *buffer)
{
    int return_value;
    if( (index + length ) < MODBUS_REGISTER_LENGTH )
    {
        memcpy(  modbus_registers+index,buffer, length*sizeof(uint16));
        return_value = 1;
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}
    
    
int get_modbus_data_registers( unsigned index, unsigned length, uint16 *buffer )
{
    int return_value;
    if( (index + length ) < MODBUS_REGISTER_LENGTH )
    {
        memcpy( buffer, modbus_registers+index, length*sizeof(uint16));
        return_value = 1;
        
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}



int process_modbus_message( int in_length, uint8 *in_buffer, 
                            int out_buffer_length, uint8 *out_buffer )
{
	
	int function_code    = 0;
    int crc_ok           = 0;
    int out_length       = 0;

    if( in_length < 5 ){ return 0; }
    
	
    crc_ok =  compute_crc( in_length, in_buffer);

	if( crc_ok != 0)
	{
		function_code = in_buffer[1];
        out_buffer[0] = in_buffer[0];  // returning slave address
        out_buffer[1] = in_buffer[1]; // returning function code
        out_buffer_length -=2;
        in_length         -=4;  // removing slave address, function code and 16 bit crc
		// extract function code
		switch( function_code )
		{
            case 3:
                    out_length = process_type_3_message(in_length, &in_buffer[2], out_buffer_length-2,
                                                     &out_buffer[2]);
                    break;
            
            case 16:
                    out_length = process_type_16_message( in_length, &in_buffer[2],out_buffer_length-2,
                                                     &out_buffer[2]);
                    
                    break;
            
            case 24:
                    out_length = process_type_24_message( in_length, &in_buffer[2], out_buffer_length, 
                                   &out_buffer[2]);
                    break;
                    
                     
            default:
                    out_length = 0; // unrecognized function code
     
           }         
        
        }
        else
        {
            out_length = 0; // bad crc
        }
        if( out_length > 200 )
         {
            CYASSERT(0);
         }

        if(  out_length > 0 )
        {
            out_length += 2; // putting back slave and function id into count
            append_crc( out_length,out_buffer);
            out_length += 2; // adding crc
        }
        
        
    

         return out_length;
    
}    

