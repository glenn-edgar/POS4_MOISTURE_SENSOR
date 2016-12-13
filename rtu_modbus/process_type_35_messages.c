/*
**
** File: process_type_35_message.c
**
**
Query
Slave Address 11
Function 10
Starting Address Hi 00  EEPROM OFFSET
Starting Address Lo 01  EEPROM OFFSET
No. of Registers Hi 00  NUMBER OF REGISTERS
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




#include <device.h>
#include "process_type_35_messages.h" 
#include "modbus_utilities.h"




int process_type_35_message( int in_length, uint8 *in, int out_length, uint8 *out )
{

    unsigned working_offset;
    unsigned number_of_registers;
    unsigned count;
    unsigned i;

    unsigned eeprom_count;
    unsigned eeprom_offset;
    uint8    temp_1;
    uint8    temp_2;
    
    
    
    *out++           = *in;
    working_offset = (*in++)*256;
    *out++           = *in;
    working_offset += (*in++);
    *out++           = *in;
    number_of_registers = (*in++)*256;
    *out++           = *in;
    number_of_registers += (*in++);
    count       = 4;
    in++;   // skip byte count;
    eeprom_offset = working_offset *2;
    eeprom_count  = number_of_registers*2;
    if( (eeprom_offset +eeprom_count) >= EEPROM_SIZE )
    {
        return 0;  // request is bigger that eeprom size
    }
    for( i = eeprom_offset; i <  eeprom_offset + (eeprom_count); i+=2 )
    {
        temp_1 = *in++;
        temp_2 = *in++;
        EEPROM_1_WriteByte(temp_2,i);
        EEPROM_1_WriteByte(temp_1,i + 1);
    }
    

        
    return  count;
    
     
}    
 