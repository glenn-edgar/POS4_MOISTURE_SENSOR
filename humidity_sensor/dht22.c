/*
**
** File DHT22.c
**
**
*/

#include <project.h>
#include "dht22.h"

static int look_for_dht22_response( DHT22_SENSOR *dht22_sensor )
{
    uint32 i;
    // look for trans to zero
    // look for trans to one
    // look for trans to zero
    for( i = 0; i < 60; i++)
    {
        if( dht22_sensor->pin_read() == 0 )
        {
            break;
        }
        CyDelayUs(1);
    }
    if( i > 50 )
    {
        return 0;
    }
    for( i = 0; i < 100; i++)
    {
        if( dht22_sensor->pin_read() != 0 )
        {
            break;
        }
        CyDelayUs(1);
    }    
    if(  i > 90 ) 
    {
        
        return 0;
    }
    
   for( i = 0; i < 100; i++)
    {
        if( dht22_sensor->pin_read() == 0 )
        {
            break;
        }
        CyDelayUs(1);
    }    
    if(  i > 90) 
    {
        
        return 0;
    }
    return 1;
}


static uint32 read_bit( DHT22_SENSOR *dht22_sensor, uint32 *status_flag )
{
    int i;
    *status_flag = 1;
    for( i = 0; i < 12; i++)
    {
        if( dht22_sensor->pin_read() != 0 )
        {
            break;
        }
        CyDelayUs(10);
    }
    for( i = 0; i < 11; i++)
    {
        if( dht22_sensor->pin_read() == 0 )
        {
            break;
        }
        CyDelayUs(10);
    }    
    if(  i < 4  )
    {
        
        return 0;
    }
    
    
    return 1;
 }
            
   
        
        
    

void DHT22_get_data( DHT22_SENSOR *dht22_sensor, uint32  *status_flag, uint32 *step_number, float *temperature, float *humidity )
{
   uint8  data[5];
   uint32  i; 
   uint32  j;
   uint64  mask;
   uint32  bit;
   int16  temp_1;
   int16 temp_2;
   uint16 checksum;
   
   uint8  criticalState;
   criticalState = CyEnterCriticalSection();
   
   dht22_sensor->pin_write(0);
   CyDelayUs(1500);
   dht22_sensor->pin_write(1);
   if( look_for_dht22_response( dht22_sensor ) == 0 )
   {
      *status_flag = 0;
      *step_number = 0;
      return;
   }
   for( i = 0; i < 5; i++ )
   {
     data[i] = 0;
     mask = 0x80;
     for( j = 0; j < 8 ; j++ )
     {
      bit = read_bit( dht22_sensor,  status_flag );
      if( *status_flag == 0 )
      {
        *step_number = i+1;
        return;
      }
      if( bit != 0 )
      {
        data[i] = data[i] | mask;
      }
      mask = mask >> 1 ;
    }
  }
  CyExitCriticalSection(criticalState);
  checksum = ( data[0] +data[1] +data[2]+data[3] )&0xff;
  if( checksum == data[4] )
  {
     temp_1 = data[0] <<8 | data[1];
     temp_2 = data[2] << 8 | data[3];
     *humidity = (float) temp_1/10.;
     *temperature = (((( float) temp_2)/10.)*9/5)+32;
  }
  else
  {
     *humidity = -1.;
     *temperature = -1.;
  }
   
}