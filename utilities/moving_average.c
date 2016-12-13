/*
**
** File: moving_average.c
**
**
*/

#include "moving_average.h"



void ma_initialize( MOVING_AVERAGE_STRUCT *ma, float alpha , float init_average )
{
	ma->alpha = alpha;
    ma->minus_alpha = 1.0 - alpha;
    ma->average     = init_average;
}

float ma_update( MOVING_AVERAGE_STRUCT *ma, float new_sample)
{
    ma->average *= ma->minus_alpha;
    ma->average += ((float) new_sample)*ma->alpha;
    return ma->average;
 
}

float ma_get_value( MOVING_AVERAGE_STRUCT *ma )
{
    return ma->average;
}

void  ma_store_value( MOVING_AVERAGE_STRUCT *ma, float new_sample )
{
    ma->average = new_sample;
}