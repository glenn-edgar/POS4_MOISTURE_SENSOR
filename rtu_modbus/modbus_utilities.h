/*
**
** File: modbus_utilities.h
** File for shuffling bytes for network ordering
**
**
**
*/

#ifndef _MODBUS_UTILITES_H_
#define _MODBUS_UTILITES_H_

#include <project.h>
  


    
static inline void swap_16( uint8 *input )
{
    uint8 temp;
    uint8 *swap_ptr;
    
    swap_ptr = input+1;
    temp     = *input;
    *input   = *swap_ptr;
    *swap_ptr = temp;

      
   
}
// probably is buggy
static inline void swap_32( uint8 *input )
{
    uint32 *temp_ptr;
    uint32 temp;
    
    temp_ptr = (uint32*)input;
    temp = *temp_ptr;   
  
    temp =  ((temp&0xff000000)>>24)|((temp&0x00ff0000)>>8)|((temp&0xff00)<<8)|((temp&0xff)<<24 );
     *temp_ptr = temp;
}

    
#endif