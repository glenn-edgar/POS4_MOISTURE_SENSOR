/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include "cf_chain_flow_support.h"

#include "cf_events.h"
#include "cf_status_bit.h"
#include "rtc_functions.h"
#include "configuration_data.h"
#include "modbus_serial_ctrl.h"
#include "event_fifo.h"

CY_ISR(WdtIsrHandler);
#define ENABLE_WATCH_DOG 0
static void setup_one_second_timer_watch_dog( void );
static void SysTickInit( void );

static inline void  process_status_data( unsigned status_data)
{

    
    if( status_data &  CF_PROCESS_TIMER_TICK )
    {
        cf_process_cf_event( CF_TIME_TICK_EVENT, TICK_INTERVAL );
        cf_status_clear_bit(  CF_PROCESS_TIMER_TICK );
    }
    if( status_data & CF_PROCESS_MODBUS )
    {
      process_rx_modbus_rtu_message( );
      cf_status_clear_bit(  CF_PROCESS_MODBUS );
    }
    
}

// static varibles to save stack space
static int event_number;
static unsigned event_id;
static unsigned event_data;
static unsigned status_data;
//CY_SYS_WDT_MODE_RESET,CY_SYS_WDT_MODE_INT 
// CySysGetResetReason() 

CY_ISR_PROTO(watch_dog_isr );




int main()

{

    CyGlobalIntDisable;  // Chain Global Interrupts  initialization chain will enable interrupts 
    CySysGetResetReason(CY_SYS_RESET_WDT); // record reset cause indirectlr
    CySysClkWcoStart();   


    cf_initialize_status_manager();             
    cf_initialize_event_manager();
    initialize_cf_system(  );
    initialize_configuration_data(); 
    EF_initialize();
    EF_enqueue_elemement( 0, EVENTS_SYSTEM_REBOOT,0 );  // power up event

    
    CyDelay(1000); 
    /* Switch LFCLK source from ILO to WCO. */
    CySysClkSetLfclkSource(CY_SYS_CLK_LFCLK_SRC_WCO);

    setup_one_second_timer_watch_dog();
   SysTickInit(  );
    
    
    
    
    cf_process_cf_event( CF_SYSTEM_INIT , 0);
    for(;;)
    {
        status_data = cf_get_status();
        if( status_data != 0 )
        {
            process_status_data( status_data);
             
        }
        else
        {
           
           event_number = cf_rx_event( &event_id, &event_data );
           if( event_number > 0 )
           {
            
             
              cf_process_cf_event( event_id, event_data);
           }
           else
           {
             ; // add sleep for low power apps
           }
        }  
    }   
}



/*
**
** Watch Dog interrupt 
**
**
**
**
*/


// setup 1 second interrupt
#define WDT_COUNT0_MATCH 0x8000
// setup 2 second watchdog timeout
#define WDT_COUNT1_MATCH 4




CY_ISR(WdtIsrHandler)
{

    /* Clear interrupts state */
	
    RTC_1_Update();
    rtc_check_time();
    CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
    WdtIsr_ClearPending();
    
  
     
}




static void setup_one_second_timer_watch_dog( void )
{
	/* Setup ISR for interrupts at WDT counter 0 events. */
    WdtIsr_StartEx(WdtIsrHandler);

    /* Enable global interrupts. */
    CyGlobalIntEnable;
	
	/* Set WDT counter 0 to generate interrupt on match */
	CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
	CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WDT_COUNT0_MATCH);
	CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, 1u);
#if ENABLE_WATCH_DOG	
	/* Enable WDT counters 0 and 1 cascade */
	CySysWdtWriteCascade(CY_SYS_WDT_CASCADE_01);
    
	/* Set WDT counter 1 to generate reset on match */
	CySysWdtWriteMatch(CY_SYS_WDT_COUNTER1, WDT_COUNT1_MATCH);
	CySysWdtWriteMode(CY_SYS_WDT_COUNTER1, CY_SYS_WDT_MODE_RESET);
    CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER1, 1u);
	

    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK | CY_SYS_WDT_COUNTER1_MASK);
#else
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK );
#endif

    
	/* Lock WDT registers and try to disable WDT counters 0 and 1 */
	CySysWdtLock();
    init_rtc();


}

/*
**
** SysTick Support
**
**
**
*/

#define Thousand_Hz_Time    33
#define Hundred_Hz_Divide   10
#define DEGLITCH_MASK      0x7


volatile uint32 switch_on;
static volatile uint32 sub_divide_count;
static volatile uint32 deglitch_register;

static inline void process_switch_functions( void )
{
    deglitch_register = deglitch_register << 1;
    if( Switch_Pin_Read() == 1 )
    {
       deglitch_register = deglitch_register | 1;
    }
    deglitch_register = deglitch_register & DEGLITCH_MASK;
    if( deglitch_register == DEGLITCH_MASK )
    {
        switch_on = 1;
    }
    if( deglitch_register == 0 )
    {
        switch_on = 0;
    }
}
    
    

static inline void SysTickCallBackHandler( void )
{
    sub_divide_count += 1;
    if( sub_divide_count >= Hundred_Hz_Divide )
    {
        cf_set_interrupt_status_bit( CF_PROCESS_TIMER_TICK  );
        process_switch_functions();
        
        sub_divide_count = 0;
    }
    // process_modbus time interval interrupt
    modbus_timer_interrupt( );
}


/*
**
** Sys Tick is setup to 1 KHZ Interrupt
**
*/
static void SysTickInit( void )
{
    switch_on = 0;
    deglitch_register = 0;
    CySysTickStart();
    CySysTickSetClockSource(CY_SYS_SYST_CSR_CLK_SRC_LFCLK);
    CySysTickSetCallback(0, SysTickCallBackHandler );
    CySysTickSetReload(Thousand_Hz_Time);
    
    CySysTickEnableInterrupt();
}