#include "user_adc.h"

static nrf_saadc_value_t adc_buf[2];
static uint16_t  batt_lvl_in_milli_volts = 0;
static bool low_power_statue = 0;
static uint16_t  temp_value = 0;
void saadc_event_handler(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        nrf_saadc_value_t adc_result;

        uint32_t          err_code;

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, 1);
        APP_ERROR_CHECK(err_code);
        adc_result = p_event->data.done.p_buffer[0];
		batt_lvl_in_milli_volts = (uint16_t)(adc_result*14);//(adc_result/1024*(1/6)*0.6*4)	
		if(get_work_mode()!=WORK_MODE_OUTSIDE)
		{
			if(batt_lvl_in_milli_volts<=LOWPOWER_THREHOLD_VALUE_MV)
			{
					low_power_statue = true;
			}
			else
			{
					low_power_statue = false;
			}
		}
		else
		{
			if(batt_lvl_in_milli_volts<=LOWPOWER_THREHOLD_VALUE_MV_OUTSIDE)
			{
					low_power_statue = true;
			}
			else
			{
					low_power_statue = false;
			}
		}
		NRF_LOG_INFO("Volate is %d",batt_lvl_in_milli_volts);

//    show_adv_mode();
        
    }
	nrf_drv_saadc_uninit();
	NRF_SAADC->INTENCLR=(SAADC_INTENCLR_END_Clear << SAADC_INTENCLR_END_Pos);
	NVIC_ClearPendingIRQ(SAADC_IRQn);
}
uint16_t get_volate_value(void)
{
	return batt_lvl_in_milli_volts;
}
uint16_t get_temp_value(void)
{
	return temp_value;
}
bool get_low_power_statue(void)
{
	return low_power_statue;
}
void saadc_configure_init_and_sample(void)
{
    ret_code_t err_code = nrf_drv_saadc_init(NULL, saadc_event_handler);
    APP_ERROR_CHECK(err_code);

    nrf_saadc_channel_config_t config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN4);
    err_code = nrf_drv_saadc_channel_init(0,&config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[0], 1);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[1], 1);
    APP_ERROR_CHECK(err_code);
    
	err_code = nrf_drv_saadc_sample();
	APP_ERROR_CHECK(err_code);	
}
