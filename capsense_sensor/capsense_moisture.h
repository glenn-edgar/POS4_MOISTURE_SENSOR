/*
**
**
** File: capsense_moisture.h
**
**
**
**/

#ifndef CAPSENSE_MOISTURE_H_
#define CAPSENSE_MOISTURE_H_

#define CAPSENSE_SENSORS  8
int initialize_capacitance_measurements(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data);
 


int make_capacitance_measurements(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data);

    
    
    
#endif