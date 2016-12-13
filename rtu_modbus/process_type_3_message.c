/*
**
** File: process_type_3_message.c

Type 3 message
Query
Field Name (Hex)
Slave Address 11
Function 03
Starting Address Hi 00
Starting Address Lo 6B
No. of Points Hi 00
No. of Points Lo 03
Error Check (LRC or CRC) ––

    
Response
Slave Address 11
Function 03
Byte Count 06
Data Hi (Register 40108) 02
Data Lo (Register 40108) 2B
Data Hi (Register 40109) 00
Data Lo (Register 40109) 00
Data Hi (Register 40110) 00
Data Lo (Register 40110) 64
Error Check (LRC or CRC) ––

**
**
*/
 
#include <project.h>
#include "process_type_3_message.h" 

#include "modbus_utilities.h"
#include "process_modbus_message.h"



int process_type_3_message( int in_length, uint8 *in, int out_length, uint8 *out )
{

    unsigned starting_register;
    int number;
    int i;
 
    
    
    
    
    starting_register = (*in++)*256;
    starting_register += (*in++);
    number = (*in++)*256;
    number += (*in++);
    // check for storeage
    if( out_length < (int)(number *2))
    {
        return 0;
    }
    *out++ = number*2;
    if( get_modbus_data_registers( starting_register, number, (uint16 *) out ) == 0 )
    {
        return 0;
    }
    // now swap bytes
    for( i = 0; i< number; i++)
    {
        swap_16(out);
        out += 2;
    }
    return   (number*2) +1;  
    
}
     
    
                                                    
