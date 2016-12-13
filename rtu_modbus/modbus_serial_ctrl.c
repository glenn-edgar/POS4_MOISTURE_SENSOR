/*
** File: modbus_srial_ctrl.c
**
**
*/

#include <project.h>
#include "modbus_serial_ctrl.h"
#include "process_modbus_message.h"
#include "cf_events.h"
#include "cf_status_bit.h"



#define UART_RX_IDLE           0
#define UART_RX_ACTIVE_RECEIVE 1
#define UART_RX_BAD_ADDRESS    2
#define UART_RX_DUMP_STATE     3

#define UART_TX_STATE_IDLE              0
#define UART_TX_STATE_TRANSMITTING      1
#define UART_TX_STATE_WAIT_FOR_EOT      2


#define UART_TIMER_IDLE 0
#define UART_TIMER_BAD_ADDRESS 1
#define UART_TIMER_RX  2

#define RX_FIFO_SIZE 16
#define TX_FIFO_SIZE 16
#define MODBUS_BUFFER_SIZE 256

static int uart_timer_state;
static int uart_rx_state;
static int uart_tx_state;
// process_rtu_message waits on CF_USB_RTU_EVENT event sent by rx interrupt handler

static int rx_interval_count;


static uint16 modbus_address;
static int modbus_rx_index;
static uint8 modbus_in_buffer[MODBUS_BUFFER_SIZE];

static int modbus_tx_index;
static int modbus_tx_message_count;
static uint8 modbus_out_buffer[MODBUS_BUFFER_SIZE];

static void init_rx_state( void );
static void init_processing_state( void );
static void init_tx_state( int tx_count );
static void abort_rx_state( void );
static volatile int read_status_value;

static inline int empty_rx_uart( void )
{
    int i;
    int return_value;
    
    return_value = 0;
    for( i = 0; i < RX_FIFO_SIZE; i++ )
    {
        read_status_value = UART_1_ReadRxStatus();
        if(  read_status_value & UART_1_RX_STS_FIFO_NOTEMPTY )
        {
            return_value += 1;
            modbus_in_buffer[ modbus_rx_index ] = UART_1_ReadRxData();
            if( modbus_rx_index > MODBUS_BUFFER_SIZE )
            {
                abort_rx_state();
            }
            else
            {
                modbus_rx_index += 1;
            }
        
        }
    }
    return return_value;
}

static inline void dump_rx_uart( void )
{
    int i;
   
    for( i = 0; i < RX_FIFO_SIZE; i++ )
    {
        read_status_value = UART_1_ReadRxStatus();
        if(  read_status_value & UART_1_RX_STS_FIFO_NOTEMPTY )
        {
             UART_1_ReadRxData();
         
        }
        else
        {
            break;
        }
    }
    
}

static inline int check_modbus_address( void )
{
    int return_value;
    if( modbus_in_buffer[0] == modbus_address )
    {
        return_value = 1;
    }
    else
    {
        return_value = 0;
    }
    return return_value;
    
}


CY_ISR_PROTO( rx_interrupt )
{
   
    switch( uart_rx_state )
    {
        case UART_RX_IDLE:
           rx_interval_count = empty_rx_uart();
           if( rx_interval_count > 0 )
           {
              
              if( check_modbus_address() > 0 )
              {
                 uart_rx_state     = UART_RX_ACTIVE_RECEIVE;
                 uart_timer_state  = UART_TIMER_RX;
              }
              else
              {
                uart_rx_state = UART_RX_BAD_ADDRESS;
                uart_timer_state = UART_TIMER_BAD_ADDRESS;
               
              }
           }     
           break;
        
        case UART_RX_ACTIVE_RECEIVE:
            rx_interval_count = empty_rx_uart();
            break;
        
        case UART_RX_BAD_ADDRESS:
            rx_interval_count = empty_rx_uart();
            
            modbus_rx_index = 0;
            break;
            
       case UART_RX_DUMP_STATE:
            dump_rx_uart();
            break;
            
    }

    
}

static inline int fill_transmitter( void )
{ 
    while( ( UART_1_ReadTxStatus() & UART_1_TX_STS_FIFO_NOT_FULL ) && ( modbus_tx_message_count > 0 ))
    {
        UART_1_WriteTxData(modbus_out_buffer[modbus_tx_index]);
        modbus_tx_index +=1;
        modbus_tx_message_count -= 1;
    }
    return modbus_tx_message_count;
}

CY_ISR_PROTO( tx_interrupt )
{
    int status;
    switch( uart_tx_state )
    {
        case UART_TX_STATE_IDLE:
            UART_1_SetTxInterruptMode( 0   );
            break;
        
        case UART_TX_STATE_TRANSMITTING:
            status = fill_transmitter();
            if( status == 0 )
            {
                uart_tx_state = UART_TX_STATE_WAIT_FOR_EOT;
                 UART_1_SetTxInterruptMode( UART_1_TX_STS_COMPLETE   );
                UART_1_SetTxInterruptMode(UART_1_TX_STS_COMPLETE );
            }
            break;
        
        case UART_TX_STATE_WAIT_FOR_EOT:
              UART_1_Stop();
              UART_1_Start();
             UART_1_SetTxInterruptMode( 0   );
              init_rx_state();
              break;
    }
}

void modbus_timer_interrupt( void )
{
    
    switch( uart_timer_state )
    {
       case UART_TIMER_IDLE:
           break; 
        
       case UART_TIMER_BAD_ADDRESS:  // wait for link to become idle and then reenable modbus rx
            if( rx_interval_count == 0 )
            {
                init_rx_state();
            }
            else
            {
                rx_interval_count = 0;
            }
            break;
            
       case UART_TIMER_RX:
            if( rx_interval_count == 0 )
            {
                init_processing_state();
            }
            else
            {
                rx_interval_count = 0;
            }
            break;
    }
}

static inline void abort_rx_state( void )
{
     uart_rx_state     =  UART_RX_BAD_ADDRESS;
     uart_timer_state  =  UART_RX_IDLE;
     modbus_rx_index   = 0;
     uart_tx_state     =  UART_TX_STATE_IDLE;
}

static inline void init_rx_state( void )
{
    
   rx_interval_count  = 0;
   modbus_rx_index    = 0;
   uart_rx_state      =  UART_RX_IDLE;
   uart_timer_state   =  UART_TIMER_IDLE;
   uart_tx_state      =  UART_TX_STATE_IDLE;
   UART_1_SetTxInterruptMode(  0 );
   UART_1_ClearTxBuffer(); 
  
   
}

static inline void init_processing_state( void )
{
    cf_set_interrupt_status_bit( CF_PROCESS_MODBUS );
    uart_rx_state      =  UART_RX_DUMP_STATE;
    uart_timer_state   = UART_TIMER_IDLE;
}


static inline void init_tx_state( int out_count )
{
   if( out_count > 200)
   {
     CYASSERT(0);  // this should not happen
   }
   modbus_tx_index  = 0;
   modbus_tx_message_count = out_count;
   uart_timer_state   = UART_TIMER_IDLE;
   uart_tx_state     =  UART_TX_STATE_TRANSMITTING;
   UART_1_SetTxInterruptMode(  UART_1_TX_STS_FIFO_NOT_FULL  );
   
   fill_transmitter();
}



int initialize_modbus_rtu(void)
{
    uart_timer_state = UART_TIMER_IDLE;
    uart_rx_state    = UART_RX_IDLE;
    uart_timer_state = UART_TIMER_IDLE;
    UART_1_Start();
    UART_1_SetRxInterruptMode(UART_1_RX_STS_FIFO_NOTEMPTY );
 

    modbus_rx_interrupt_StartEx(rx_interrupt); 
    UART_1_SetTxInterruptMode(  0 );
    modbus_tx_interrupt_StartEx(tx_interrupt); 
  
 
    // find modbus address
    modbus_address = get_modbus_address();

    init_rx_state();
      
    return CF_DISABLE; // TBD Right now
}

int process_rx_modbus_rtu_message(void )
{
    int out_count;
    
    out_count =  process_modbus_message( modbus_rx_index, &modbus_in_buffer[0], sizeof(modbus_out_buffer), &modbus_out_buffer[0] ); 
                           
    
    if( out_count > 0 )
    {
        init_tx_state( out_count );
    }
    else
    {
        init_rx_state();
    }
    return CF_TERMINATE; // TBD Right now
}




int set_normal_modbus_address(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
      uint16 temp;
      temp = 0;
    
      store_modbus_data_registers( MOD_COMMISSIONING_FLAG, 1 , &temp );
      modbus_address = get_modbus_address();
      return 0;
}

int set_commissiong_address(unsigned link_id, unsigned param_1,
  unsigned param_2, unsigned param_3, unsigned event, unsigned data)
{
      uint16 temp;
      temp = 1;
    
       store_modbus_data_registers( MOD_COMMISSIONING_FLAG, 1 , &temp );
       modbus_address = get_commissioning_address();
      
      return 0;
}