#include "pcf85.h"

//RTC Structure
typedef struct pcf85_s
{
  uint8_t   Address;
  uint8_t   TxBuf[PCF85_TX_RX_BUF_LENGTH];
  uint8_t   RxBuf[PCF85_TX_RX_BUF_LENGTH];
} pcf85_t, * pcf85_p;

static pcf85_t  pcf85              = {0};
//Register Masks - Seconds, Minutes, Hours, Days, WeekDays, Months, Years
const  uint8_t  pcf85_regmask[7]  = {0x7F, 0x7F, 0x3F, 0x3F, 0x07, 0x1F, 0xFF};

#if (TWI0_ENABLED == 1)
  const    nrf_drv_twi_t   twi    = NRF_DRV_TWI_INSTANCE(0);
#elif (TWI1_ENABLED == 1)
  const    nrf_drv_twi_t   twi    = NRF_DRV_TWI_INSTANCE(1);
#else
  #error "No TWI enabled"
#endif



bool PCF85_Init()
{
  //uint32_t error;
  uint8_t attempts = 200;
  //nrf_drv_twi_config_t cfg;

  //cfg.frequency =  TWI_FREQUENCY;
  //cfg.scl = TWI_SCL_PIN;
  //cfg.sda = TWI_SDA_PIN;
  //cfg.interrupt_priority = TWI_IRQ_PRIORITY;
  
  //error = nrf_drv_twi_init(&twi, &cfg, NULL);
  if(!twi_master_init()) return false;
  //if(error != NRF_SUCCESS) return false;
  
  //nrf_drv_twi_enable(&twi);
  
  pcf85.Address = PCF85_SLAVE_ADDRESS;
  
  //We should wait until RTC oscillator isn't stable
  while(attempts)
  {
    //Write Time Register Offset
    pcf85.TxBuf[0] = PCF85_DTIME_REGISTER_OFFSET;
    //error = nrf_drv_twi_tx(&twi, pcf85.Address, pcf85.TxBuf, 1, true);
    //if(error != NRF_SUCCESS) return false;
    if(!twi_master_transfer(pcf85.Address, pcf85.TxBuf, 1, TWI_DONT_ISSUE_STOP))
      return false;

    //Read Seconds and Oscillator Flag from RTC
    pcf85.RxBuf[0] = 0;
    //error = nrf_drv_twi_rx(&twi, pcf85.Address, pcf85.RxBuf, 1, false);
    //if(error != NRF_SUCCESS) return false;
    if(!twi_master_transfer((pcf85.Address | TWI_READ_BIT), pcf85.RxBuf, 1, TWI_ISSUE_STOP))
      return false;

    //If OS (Oscillator Stop) Flag is cleared - Break waiting
    if((pcf85.RxBuf[0] & PCF85_DTIME_BIT_OS) == 0) break;
    
    //Else write Time Register Offset
    pcf85.TxBuf[0] = PCF85_DTIME_REGISTER_OFFSET;
    //Clear OS Flag
    pcf85.TxBuf[1] = pcf85.RxBuf[0] & (~PCF85_DTIME_BIT_OS);
    //error = nrf_drv_twi_tx(&twi, pcf85.Address, pcf85.TxBuf, 1, false);
    //if(error != NRF_SUCCESS) return false;
    if(!twi_master_transfer(pcf85.Address, pcf85.TxBuf, 2, TWI_ISSUE_STOP))
      return false;
    
    nrf_delay_ms(3);
    attempts--;
  }
  
  return true;
}



bool PCF85_GetTime(pcf85time_p Time)
{
  //uint32_t error;
  uint8_t i, * ui8Time = (uint8_t *)Time;
  
  //Write Time Register Offset
  pcf85.TxBuf[0] = PCF85_DTIME_REGISTER_OFFSET;
  //error = nrf_drv_twi_tx(&twi, pcf85.Address, pcf85.TxBuf, 1, true);
  //if(error != NRF_SUCCESS) return false;
  if(!twi_master_transfer(pcf85.Address, pcf85.TxBuf, 1, TWI_DONT_ISSUE_STOP))
    return false;

  //Read Date Time from RTC
  for(i = 0; i < 7; i++) pcf85.RxBuf[i] = 0;
  //error = nrf_drv_twi_rx(&twi, pcf85.Address, pcf85.RxBuf, 7, false);
  //if(error != NRF_SUCCESS) return false;
  if(!twi_master_transfer((pcf85.Address | TWI_READ_BIT), pcf85.RxBuf, 7, TWI_ISSUE_STOP))
    return false;
 
  Time->Years = 0;
  for(i = 0; i < 7; i++)
  {
    pcf85.RxBuf[i] &= pcf85_regmask[i];
    *ui8Time = ((pcf85.RxBuf[i] >> 4) * 10) + (pcf85.RxBuf[i] & 0x0F);
    ui8Time++;
  }
  Time->Years += 2000;

  return true;
}



bool PCF85_SetTime(pcf85time_p Time)
{
  //uint32_t error;
  uint8_t i, * ui8Time = (uint8_t *)Time;
  
  pcf85.TxBuf[0] = PCF85_DTIME_REGISTER_OFFSET;
  for(i = 0; i < 7; i++)
  {
    if(i == 6)
      pcf85.TxBuf[i + 1] = (((Time->Years - 2000) / 10) << 4) + ((Time->Years - 2000) % 10);
    else
      pcf85.TxBuf[i + 1] = ((*ui8Time / 10) << 4) + (*ui8Time % 10);
    ui8Time++;
  }
  
  //Write Time Register Offset and Time
  //error = nrf_drv_twi_tx(&twi, pcf85.Address, pcf85.TxBuf, 8, false);
  //if(error != NRF_SUCCESS) return false;
  if(!twi_master_transfer(pcf85.Address, pcf85.TxBuf, 8, TWI_ISSUE_STOP))
    return false;
  
  return true;
}