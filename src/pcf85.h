#ifndef PCF85_H
#define PCF85_H

#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "nrf_error.h"
#include "nrf_delay.h"
#include "nrf51_bitfields.h"
#include "nrf_drv_twi.h"
#include "twi_master.h"

//Define RTC model (must be 85063 or 8523)
#define PCF85_MODEL               85063
//#define PCF85_MODEL               8523
#define PCF85_TWI                 1
#define PCF85_TX_RX_BUF_LENGTH    20

#if (PCF85_TWI == 0)
  #if (TWI0_ENABLED == 1)
    #define TWI_ID                0
    #define TWI_FREQUENCY         TWI_CONFIG_FREQUENCY(0)
    #define TWI_SCL_PIN           TWI_CONFIG_SCL(0)
    #define TWI_SDA_PIN           TWI_CONFIG_SDA(0)
    #define TWI_IRQ_PRIORITY      TWI_CONFIG_IRQ_PRIORITY(0)
  #else
    #error "TWI0 using by PCF85 DISABLED!"
  #endif
#elif (PCF85_TWI == 1)
  #if (TWI1_ENABLED == 1)
    #define TWI_ID                1
    #define TWI_FREQUENCY         TWI_CONFIG_FREQUENCY(1)
    #define TWI_SCL_PIN           TWI_CONFIG_SCL(1)
    #define TWI_SDA_PIN           TWI_CONFIG_SDA(1)
    #define TWI_IRQ_PRIORITY      TWI_CONFIG_IRQ_PRIORITY(1)
  #else
    #error "TWI1 using by PCF85 DISABLED!"
  #endif
#else
  #error "No TWI enabled for PCF85"
#endif

#if (PCF85_MODEL == 85063)
  #define PCF85_SLAVE_ADDRESS           (0x51 << 1)
  #define PCF85_CTRL1_REGISTER_OFFSET   0x00
  #define   PCF85_CTRL1_BIT_24H         0x02
  #define PCF85_DTIME_REGISTER_OFFSET   0x04
  #define   PCF85_DTIME_BIT_OS          0x80
#elif (PCF85_MODEL == 8523)
  #define PCF85_SLAVE_ADDRESS           (0x68 << 1)
  #define PCF85_CTRL1_REGISTER_OFFSET   0x00
  #define   PCF85_CTRL1_BIT_24H         0x08
  #define PCF85_DTIME_REGISTER_OFFSET   0x03
  #define   PCF85_DTIME_BIT_OS          0x80
#else
  #error "No RTC Model Selected"
#endif

//RTC Structure
typedef struct pcf85time_s
{
  uint8_t   Seconds;
  uint8_t   Minutes;
  uint8_t   Hours;
  uint8_t   Days;
  uint8_t   WeekDays;
  uint8_t   Months;
  uint16_t  Years;
} pcf85time_t, * pcf85time_p;

bool PCF85_Init(void);
bool PCF85_GetTime(pcf85time_p Time);
bool PCF85_SetTime(pcf85time_p Time);


#endif //S25FL_H