/*
**
**
** File:  analog_temperature_measurements.c
**
**
**
*/

#include <project.h>

#include "analog_temperature_measurements.h"

// temperature calibration in degrees F




float measure_tmp36_sensor( void )
{
    float temperature;

    
    CyDelay(1);
    AMux_2_FastSelect( AMux_2_CHANNELS-2 );

    ADC_SAR_Seq_1_StartConvert();
    CyDelay(1); 

    temperature = ADC_SAR_Seq_1_CountsTo_mVolts(0 ,  ADC_SAR_Seq_1_GetResult16(0));

 

    temperature = ( (float)temperature - 500.)/10. ; // offset -500 mv  10mv/deg c
    temperature = (temperature*9./5.)+32;
  
    return temperature;
    
  }
