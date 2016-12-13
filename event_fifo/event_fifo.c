/*
**
**
** File: event_fifo.c
**
**
**
**
*/

#include <assert.h>
#include <project.h>


#include "event_fifo.h"


#define NUMBER_OF_QUEUES  1
// QUEUE number must be a multiple of 2
#define QUEUE_NUMBER 16  
#define QUEUE_MASK   15

typedef struct
{
    uint16 event;
    uint16 data;
    
}QUEUE_DATA;

typedef struct
{
    int number;
    int read_index;
    int write_index;
    QUEUE_DATA queue_data[ QUEUE_NUMBER ];
}QUEUE_STRUCTURE;

QUEUE_STRUCTURE queue_structure[NUMBER_OF_QUEUES];

void EF_initialize( void )
{
    memset( queue_structure, 0, sizeof(queue_structure ));
    // add in start up event
    EF_enqueue_elemement( 0, EVENTS_SYSTEM_REBOOT, 0 );
}

unsigned EF_number_of_queues( void )
{
    return NUMBER_OF_QUEUES;
}

int EF_elements_in_queue( unsigned queue_id )
{
    if( queue_id < NUMBER_OF_QUEUES )
    {
        return queue_structure[queue_id].number;
    }
    return -1;  
}
    
// returns number of elements in queue -1 means bad queue_id
int  EF_enqueue_elemement( unsigned queue_id, uint16 event_id, uint16 event_data )
{
    QUEUE_STRUCTURE *queue_str;
    if( queue_id > NUMBER_OF_QUEUES )
    {
        return -1;
    }
    queue_str = &queue_structure[ queue_id ];

    queue_str->queue_data[queue_str->write_index].event = event_id;
    queue_str->queue_data[queue_str->write_index].data = event_data;
    
    queue_str->write_index +=1;
    queue_str->write_index &= QUEUE_MASK;
    
    if( queue_str->number >= QUEUE_NUMBER)
    {
         
         queue_str->read_index +=1;
         queue_str->read_index &= QUEUE_MASK;
    }
    else
    {
        queue_str->number += 1;
    }
    return queue_str->number;
}
       

// return number left in queue after dequeue -- return -1 if attempt was made on empty queue
int  EF_dequeue_element( unsigned queue_id, uint16 *event_id, uint16 *event_data )
{
    QUEUE_STRUCTURE *queue_str;
    if( queue_id > NUMBER_OF_QUEUES )
    {
        return -1;
    }
    
    queue_str = &queue_structure[ queue_id ];
    if( queue_str->number == 0 )
    {
        return -1;
    }
   

    *event_id   = queue_str->queue_data[queue_str->read_index].event;
    *event_data = queue_str->queue_data[queue_str->read_index].data;
    
    queue_str->read_index +=1;
    queue_str->read_index &= QUEUE_MASK;
    queue_str->number -= 1;
  
    return queue_str->number;
}
