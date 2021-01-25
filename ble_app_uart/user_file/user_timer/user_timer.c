/*
 * @Author: Lemon
 * @Date: 2021-01-22 10:39:55
 * @LastEditTime: 2021-01-25 16:17:41
 * @LastEditors: Lemon
 * @Description: 定时器功能模块
 * @FilePath: \code\nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\user_timer\user_timer.c
 */
#include "project_base_include.h"
#include "user_timer.h"
#include "ble_data_process.h"
#include "user_adc.h"
APP_TIMER_DEF(m_1s_timer);
static uint32_t now_second = 0;
static void m_1s_timer_timeout_handler(void * p_context)
{
    now_second++;
    if(0 == now_second%10)
	NRF_LOG_INFO("now second = %d",now_second);
	sample_timer_handler();
}
/**@brief Function for initializing the timer module.
 */
static void timers_init(void)
{
    // Initialize timer module.
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_1s_timer,APP_TIMER_MODE_REPEATED,m_1s_timer_timeout_handler);
	APP_ERROR_CHECK(err_code);
}

void m_50ms_timer_start(void)
{
	ret_code_t err_code = app_timer_start(m_1s_timer,M_1S_TIMER_TIMEOUT,NULL);
	APP_ERROR_CHECK(err_code);
}
static void clock_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
}
void user_timer_init(void)
{
	clock_init();
	timers_init();
    m_50ms_timer_start();
}
