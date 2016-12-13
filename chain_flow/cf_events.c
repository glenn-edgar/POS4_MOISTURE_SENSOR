/*
**
** File: cf_events.c
**
**
**
**
*/
#include <project.h>
#include "cf_events.h"


#define EVENT_SIZE 256
#define EVENT_MASK 255

static unsigned rx_index;
static unsigned tx_index;
static unsigned event_number;

typedef struct
{
   unsigned  event;
   unsigned  data;
}EVENT_DATA;

static EVENT_DATA event_data[ EVENT_SIZE ];

void cf_initialize_event_manager( void )
{
   rx_index     = 0;
   tx_index     = 0;
   event_number = 0;
}

void cf_send_interrupt_event( unsigned event, unsigned data)
{
  
   if( event_number < EVENT_SIZE )
   {
     event_data[rx_index].event      = event;
     event_data[rx_index].data       = data;
     event_number++;
     rx_index = ( rx_index +1 )&EVENT_MASK;
   }
   
}

void cf_send_event( unsigned event, unsigned data )
{
   uint8  criticalState;
    criticalState = CyEnterCriticalSection();
   if( event_number < EVENT_SIZE )
   {
     event_data[rx_index].event      = event;
     event_data[rx_index].data       = data;
     event_number++;
     rx_index = ( rx_index +1 )&EVENT_MASK;
   }
   CyExitCriticalSection(criticalState);
}


int cf_rx_event( unsigned *event, unsigned *data )
{
   int return_value;
   uint8  criticalState;
    criticalState = CyEnterCriticalSection();
   // add interrupt protoection
   if( event_number > 0 )
   {
      *event          = event_data[tx_index].event;
      *data     = event_data[tx_index].data;
      return_value    = 1;
      event_number--;
      tx_index = ( tx_index +1 )&EVENT_MASK;

   }
   else
   {
     return_value = 0;
   }
   CyExitCriticalSection(criticalState);
   return return_value;
}
             
  
