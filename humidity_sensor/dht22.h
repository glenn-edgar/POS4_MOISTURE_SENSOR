/*
**
**
** File: dht22.h
**
**
**
**
*/

#ifndef DHT22_H_
#define DHT22_H_
  
    
#include <project.h>

typedef uint8 ( *PIN_READ )(void );
typedef void  ( *PIN_WRITE )( uint8 value );    
    
typedef const struct 
{
    PIN_READ pin_read;
    PIN_WRITE pin_write;
}DHT22_SENSOR;
        
void DHT22_get_data( DHT22_SENSOR *dht22_sensor, uint32  *device_found, uint32 *step_number, float *temperature, float *humiditiy );    
    
#endif
