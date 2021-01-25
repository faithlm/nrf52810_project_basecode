/*
 * @Author: Lemon
 * @Date: 2021-01-22 10:40:04
 * @LastEditTime: 2021-01-25 16:37:35
 * @LastEditors: Lemon
 * @Description: 
 * @FilePath: \code\nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\user_saadc\user_adc.c
 */
 #include "project_base_include.h"
#include "user_adc.h"

#define SAADC_CHANNEL_COUNT   4
#define SAADC_SAMPLE_INTERVAL_MS 250
static volatile bool is_ready = true;
static nrf_saadc_value_t samples[SAADC_CHANNEL_COUNT];
static nrfx_saadc_channel_t channels[SAADC_CHANNEL_COUNT] = {NRFX_SAADC_DEFAULT_CHANNEL_SE(NRF_SAADC_INPUT_AIN0, 0),
                                                             NRFX_SAADC_DEFAULT_CHANNEL_SE(NRF_SAADC_INPUT_AIN1, 1),
                                                             NRFX_SAADC_DEFAULT_CHANNEL_SE(NRF_SAADC_INPUT_AIN2, 2),
                                                             NRFX_SAADC_DEFAULT_CHANNEL_SE(NRF_SAADC_INPUT_AIN3, 3)};

void saadc_event_handler(nrfx_saadc_evt_t const * p_event)
{
    if (p_event->type == NRFX_SAADC_EVT_DONE)
    {
        for(int i = 0; i < p_event->data.done.size; i++)
        {
            NRF_LOG_INFO("CH%d: %d", i, p_event->data.done.p_buffer[i]);
        }

        is_ready = true;
    }

    /* Applying workaround from Errata 212, otherwise current is stuck at 4-500uA during sleep after first sample. */
    volatile uint32_t temp1;
    volatile uint32_t temp2;
    volatile uint32_t temp3;

    temp1 = *(volatile uint32_t *)0x40007640ul;
    temp2 = *(volatile uint32_t *)0x40007644ul;
    temp3 = *(volatile uint32_t *)0x40007648ul;

    *(volatile uint32_t *)0x40007FFCul = 0ul; 
    *(volatile uint32_t *)0x40007FFCul; 
    *(volatile uint32_t *)0x40007FFCul = 1ul;

    *(volatile uint32_t *)0x40007640ul = temp1;
    *(volatile uint32_t *)0x40007644ul = temp2;
    *(volatile uint32_t *)0x40007648ul = temp3;
}
void sample_timer_handler(void)
{
    if(is_ready)
    {
        ret_code_t err_code;

        err_code = nrfx_saadc_simple_mode_set((1<<0|1<<1|1<<2|1<<3),
                                              NRF_SAADC_RESOLUTION_12BIT,
                                              NRF_SAADC_OVERSAMPLE_4X,
                                              saadc_event_handler);
        APP_ERROR_CHECK(err_code);
        
        err_code = nrfx_saadc_buffer_set(samples, SAADC_CHANNEL_COUNT);
        APP_ERROR_CHECK(err_code);

        err_code = nrfx_saadc_mode_trigger();
        APP_ERROR_CHECK(err_code);

        is_ready = false;
    }
    
}
void saadc_configure_init_and_sample(void)
{
	uint32_t err_code;
    err_code = nrfx_saadc_init(NRFX_SAADC_CONFIG_IRQ_PRIORITY);
    APP_ERROR_CHECK(err_code);

    err_code = nrfx_saadc_channels_config(channels, SAADC_CHANNEL_COUNT);
    APP_ERROR_CHECK(err_code);
}
