/*
**
** File: event_fifo.h
**
**
**
**
*/

#ifndef _EVENT_FIFO_H_
#define _EVENT_FIFO_H_
   
#include <project.h>

/*
**
**  System Events
**
**
*/
#define EVENTS_SYSTEM_REBOOT 0

    
    
    
void EF_initialize( void );

unsigned EF_number_of_queues( void );
int EF_elements_in_queue( unsigned queue_id );
// returns number of elements in queue
int  EF_enqueue_elemement( unsigned queue_id, uint16 event_id, uint16 event_data );
// return number left in queue after dequeue -- return -1 if attempt was made on empty queue
int  EF_dequeue_element( unsigned queue_id, uint16 *event_id, uint16 *event_data );

    
    
#endif