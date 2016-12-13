/*
**
**
** File Name: rtc_functions.c
**
**
*/

#include <project.h>
#include "rtc_functions.h"
#include "cf_external_functions.h"
#include "cf_chain_flow_support.h"
#include "cf_events.h"
#define CF_HOUR_TICK           7


static uint32 current_time;
static uint32 ref_time;

void rtc_check_time( void )
{
    
    current_time = RTC_1_GetTime();
    cf_send_interrupt_event( CF_SECOND_TICK, 1);

    if( RTC_1_GetMinutes( current_time) != RTC_1_GetMinutes(ref_time ) )
    {
        cf_send_interrupt_event( CF_MINUTE_TICK, 1);
    }
    if( RTC_1_GetHours( current_time) != RTC_1_GetHours(ref_time ) )
    {
        cf_send_interrupt_event( CF_HOUR_TICK, 1);
    }
    ref_time = current_time;
      
    
    
    
    
    
}

int init_rtc(void)
{
    
    RTC_1_Start();
    RTC_1_SetPeriod( 1, 1 );
    ref_time = RTC_1_GetTime();
    return CF_DISABLE;
}


