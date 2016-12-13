/*
**
** File cf_events.h
**
*/

#ifndef _CF_EVENTS_H_
#define _CF_EVENTS_H_

#define TICK_INTERVAL          16
    
#define CF_SYSTEM_INIT         0
#define CF_INIT_EVENT          1
#define CF_TIME_TICK_EVENT     2
#define CF_WATCH_DOG_EVENT     3

#define CF_SECOND_TICK         5
#define CF_MINUTE_TICK         6
#define CF_HOUR_TICK           7
#define CF_COMMISSIONING_DONE  8
#define CF_START_MOISTURE_READING    9
#define CF_120HZ_TICK          10
#define CF_MEASURE_MOISTURE_CHANNEL 12
#define CF_MOISTURE_CHANNEL_DONE    13
#define CF_MOISTURE_START_CHANNELS  14
#define CF_UPDATE_FLASH             15
#define CF_CHECK_ONE_WIRE_PRESENCE  16
#define CF_MAKE_SOIL_TEMP_MEASUREMENT 17
#define CF_MEASURE_AIR_TEMP_HUMIDITY  18

#define CF_MEASURE_RESISTIVE_ELEMENTS  20
#define CF_RESISTIVE_ELEMENTS_DONE     21
#define CF_MEASURE_CAPACITIVE_ELEMENTS 22
#define CF_CAPACITACNE_ELEMENETS_DONE  23
    

void cf_initialize_event_manager( void );
void cf_send_interrupt_event( unsigned event, unsigned data);
void cf_send_event( unsigned event, unsigned data );
int cf_rx_event( unsigned *event, unsigned *event_data );

#endif
