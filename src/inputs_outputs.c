#include "inputs_outputs.h"
#include "nrf_drv_gpiote.h"


input_data_t input_data_Y;

void inY_event_handler (nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if((pin==IN_Y))
    {  
      input_data_Y.new_state = true;
    }
}


void inputs_init (void)
{
  //nrf_gpio_cfg_input(29,NRF_GPIO_PIN_PULLUP);
  
   if(!nrf_drv_gpiote_is_init())
   {
      nrf_drv_gpiote_init();
   }
  
   #ifdef YuriyPCB
   nrf_drv_gpiote_in_config_t config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
   config.pull = NRF_GPIO_PIN_PULLUP;
   nrf_drv_gpiote_in_init(IN_Y, &config, inY_event_handler);
   nrf_drv_gpiote_in_event_enable(IN_Y, true); 
   #endif 
}




