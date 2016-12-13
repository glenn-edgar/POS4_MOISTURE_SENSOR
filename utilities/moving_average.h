/*
**
** File: moving_average.h
**
**
**
*/

#ifndef _MOVING_AVERAGE_H_
#define _MOVING_AVERAGE_H_
#include <device.h>
typedef struct
{
   float alpha;
   float minus_alpha;
   float average;
}MOVING_AVERAGE_STRUCT;

void ma_initialize( MOVING_AVERAGE_STRUCT *ma, float alpha, float init_average);
float ma_update( MOVING_AVERAGE_STRUCT *moving_average, float new_sample);
float ma_get_value( MOVING_AVERAGE_STRUCT *moving_average );
void  ma_store_value( MOVING_AVERAGE_STRUCT *moving_average, float new_sample );
#endif

