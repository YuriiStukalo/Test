#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "s25fl.h"
#include "pcf85.h"
#include "app_gpiote.h"
#include "nrf_drv_gpiote.h"
#include "inputs_outputs.h"
/*
 * This example uses only one instance of the SPI master.
 * Please make sure that only one instance of the SPI master is enabled in config file.
 */

static uint8_t WrBuf[1024];
static uint8_t RdBuf[1024];

static app_gpiote_user_id_t   m_example_user_id;
uint32_t  low_to_high_bitmask = 0x1FFFFFFF; // Bitmask to be notified of transition from low to high for GPIO 0-29
uint32_t  high_to_low_bitmask = 0x1FFFFFFF; // Bitmask to be notified of transition from high to low for GPIO 0-29
uint32_t retval;
static uint8_t tstBtn;

static bool CompareBuffers(uint8_t * pBufA, uint8_t * pBufB, uint32_t len)
{
  uint32_t i;

  for(i = 0; i < len; i++)
  {
    if(pBufA[i] != pBufB[i]) return false;
  }

  return true;
}


static void InitBuffers(uint8_t * pBufA, uint8_t * pBufB, uint32_t len)
{
  uint32_t i;

  for(i = 0; i < len; i++)
  {
    pBufA[i] = i & 0xFF;
    pBufB[i] = 0;
  }
}


/**@brief Function for initializing bsp module.
 */
//void bsp_configuration()
//{
//    uint32_t err_code = NRF_SUCCESS;

//    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
//    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
//    NRF_CLOCK->TASKS_LFCLKSTART    = 1;

//    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
//    {
        // Do nothing.
//    }

//    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, NULL);
        
//    err_code = bsp_init(BSP_INIT_LED, APP_TIMER_TICKS(100, APP_TIMER_PRESCALER), NULL);
//    APP_ERROR_CHECK(err_code);
//}

#define TIME_STAMP 28
#define EVENT_BUF  256
char writeEventData[EVENT_BUF];
unsigned char len;
unsigned char i;

int main(void)
{
  volatile bool S25FL_WriteComplete = false;
  volatile uint32_t S25FL_ReadedID = 0;
  volatile bool PCF85_Present = false;
  
  //inputs_init ();
  
  
  
  if(S25FL_Init())
  {
    S25FL_ReadBuffer(0x00000000, RdBuf, 512);
    S25FL_ReadBuffer(0x00010000, RdBuf, 512);
    
    //WARNING! Sector Address MUST be aligned to Sector Size
    if(S25FL_SectorErase(0x00000000))
    {
      S25FL_ReadBuffer(0x00000000, RdBuf, 1024);
      
      InitBuffers(WrBuf, RdBuf, 1024);
      
      //WARNING! Address MUST be aligned to Page Size
      // Random Writing Memory Driver is difficult and It requires a large RAM for Full Sector Saving
      // And thats why the Flash MUST be written Page by Page
      // In addition Page Write Command gives the highest Write Speed
      if(S25FL_WriteBuffer(0x00000000, WrBuf, 753) == 753)
      {
        if(S25FL_ReadBuffer(0x00000000, RdBuf, 1024) == 1024)
        {
          S25FL_WriteComplete = CompareBuffers(WrBuf, RdBuf, 753);
        }
      }
    }
  }
  //APP_ERROR_CHECK(err_code);
  
  
  PCF85_Present = PCF85_Init();

  pcf85time_t Time;

//  Time.Seconds = 13;
//  Time.Minutes = 36;
//  Time.Hours = 23;
//  Time.Days = 10;
//  Time.WeekDays = 4;
//  Time.Months = 7;
//  Time.Years = 2015;
//  PCF85_SetTime(&Time);
  
  while(1)
  {
    #ifdef YuriyPCB
//    if(input_data_Y.new_state)
//    {  
//       PCF85_GetTime(&Time);
//       input_data_Y.action = nrf_gpio_pin_read(IN_Y);
//       input_data_Y.new_state = false; 
//       len = strlen(writeEventData);
//       if(EVENT_BUF-len > TIME_STAMP)
//       {  
//          sprintf((char*)&writeEventData[len], "%s %02d %02d %04d %02d %02d %02d %s", "IN_Y:", Time.Days, ( Time.Months + 1), (Time.Years), Time.Hours, Time.Minutes, Time.Seconds, "\r\n");
//       }  
//       if(strlen(writeEventData)== EVENT_BUF)
//       {
//          //write data to memomory
//         // memeset((char*)&writeEventData, 0x00, EVENT_BUF);
//       }
//    }
    #endif
    PCF85_GetTime(&Time);
    nrf_delay_ms(10);
  }
  
  //while(1);
}


/** @} */
