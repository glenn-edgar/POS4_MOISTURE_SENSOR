
/*
**   File: "process_type_24_message.h"
**
**
Type 24 Query FIFO
    
Query
Slave Address 11
Function 18
FIFO Pointer Address Hi 04
FIFO Pointer Address Lo DE
Error Check (LRC or CRC) ––

Response
Slave Address 11
Function 18
Byte Count Hi 00
Byte Count Lo 08
FIFO Count Hi 00
FIFO Count Lo 03
FIFO Data Reg 1 Hi 01
FIFO Data Reg 1 Lo B8
FIFO Data Reg 2 Hi 12
FIFO Data Reg 2 Lo 84
FIFO Data Reg 3 Hi 13
FIFO Data Reg 3 Lo 22
Error Check (LRC or CRC) ––
*/




#include <project.h>
#include "process_type_24_message.h" 
#include "modbus_utilities.h"
#include "event_fifo.h"

#define MAX_FIFO_NUMBER 10

int process_type_24_message( int in_length, uint8 *in, int out_length, uint8 *out )
{
    unsigned fifo_number;
    uint16 event_data;
    unsigned i;
    uint16 event_id;
    unsigned queue_index;
    uint16   *temp_ptr;
    
    temp_ptr = (uint16 *)out;
    queue_index  = (*in++)*256;
    queue_index  += (*in);
    if( queue_index >= EF_number_of_queues() )
    {
        return 0;
    }
    
    fifo_number = EF_elements_in_queue( queue_index );
    if( fifo_number > MAX_FIFO_NUMBER )
    {
        fifo_number = MAX_FIFO_NUMBER;
    }
    /*
    ** Write out byte count
    **
    */
    *temp_ptr = fifo_number *4;
    swap_16( (uint8 *)temp_ptr++ );
    /*
    ** Write out fifo number 
    */
    *temp_ptr = fifo_number *2;
    swap_16( (uint8 *)temp_ptr++ );
    

 
    for( i= 0; i < fifo_number; i ++)
    {
        EF_dequeue_element( queue_index, &event_id, &event_data );
        *temp_ptr = event_id ;
        swap_16( (uint8 *)temp_ptr++ );
        *temp_ptr = event_data;
         swap_16( (uint8 *)temp_ptr++ );

    }
    return fifo_number*4+4;
}    
    
