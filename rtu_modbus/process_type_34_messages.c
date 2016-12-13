/*
**
** File: process_type_34_message.c

Type 34 message
Query
Field Name (Hex)
Slave Address 11
Function 03
Starting Address Hi 00  Offset in EEPROM
Starting Address Lo 6B  
No. of Points Hi 00      Number of words  
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
 

#include <device.h>
#include "EEPROM_1.h"
#include "modbus_utilities.h"
#include "process_type_34_messages.h" 





#define TABLE_LENGTH 

int process_type_34_message( int in_length, uint8 *in, int out_length, uint8 *out )
{

    unsigned working_offset;  // offset of eeprom
    unsigned number_of_registers;
    unsigned i;
    unsigned eeprom_count;
    unsigned eeprom_offset;
    
    
    
    working_offset = (*in++)*256;
    working_offset += (*in++);
    number_of_registers = (*in++)*256;
    number_of_registers += (*in++);
    eeprom_offset = working_offset *2;
    eeprom_count  = number_of_registers*2;
    if( (eeprom_offset +eeprom_count) >= EEPROM_SIZE )
    {
        return 0;  // request is bigger that eeprom size
    }
    if( (eeprom_count + 1 ) > (unsigned)out_length )
    {
        return 0; // request exceeds size of eeprom buffer
    }
        
    
    *out++ = eeprom_count;

    for( i = eeprom_offset; i < eeprom_offset + (eeprom_count) ; i+=2)
    {
        *out++ = EEPROM_1_ReadByte(i+1) ;
        *out++ = EEPROM_1_ReadByte(i);
    }
    return eeprom_count +1;
}

 
