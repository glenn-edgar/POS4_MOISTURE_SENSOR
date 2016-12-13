/*
**
** File cf_status_bit.c
**
**
**
*/
#include <project.h>
#include "cf_status_bit.h"


static uint32 cf_status_register;



void cf_initialize_status_manager( void )
{
   cf_status_register = 0;
}

void  cf_status_clear_bit( uint32 status_bit)
{
  uint8  criticalState;
    criticalState = CyEnterCriticalSection();

    cf_status_register = cf_status_register &(~status_bit);
    CyExitCriticalSection(criticalState);
}

void cf_clear_status(void )
{
      
   uint8  criticalState;
    criticalState = CyEnterCriticalSection();

    cf_status_register = 0;
    CyExitCriticalSection(criticalState);
}

void cf_set_interrupt_status_bit( uint32 status_bit )
{
      cf_status_register |= status_bit;

}

void cf_set_status_bit( uint32 status_bit )
{
    
    uint8  criticalState;
    criticalState = CyEnterCriticalSection();

   cf_status_register |= status_bit;
   CyExitCriticalSection(criticalState);
}


uint32 cf_get_status( )
{
   int return_value;
   uint8  criticalState;
    criticalState = CyEnterCriticalSection();

   // add interrupt protoection
   return_value = cf_status_register;
   cf_status_register = 0;
   CyExitCriticalSection(criticalState);
   return return_value;
}
             
  



