/*
 * @Author: Lemon
 * @Date: 2021-01-22 10:39:55
 * @LastEditTime: 2021-01-29 15:28:37
 * @LastEditors: Lemon
 * @Description: 定时器功能模块
 * @FilePath: \nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\user_timer\user_timer.c
 */
#include "project_base_include.h"
#include "user_timer.h"
#include "ble_data_process.h"
#include "user_adc.h"
#include "bluetooth.h"

APP_TIMER_DEF(m_1s_timer);
static uint32_t now_second = 0;

uint32_t get_now_second(void)
{
	return now_second;
}

/**
 * @description: 1s中断处理函数，进行状态判断及处理
 * @param {*}
 * @return {*}
 */
static void timer_event_handler(void * p_event_data, uint16_t event_size)
{
	//链接状态，且开始传输以后，才进行adc采集
	if(jh_state_mechine.start_trans_statue && jh_state_mechine.connect_statue)
	{
		//超过20分钟没有重新计时时，自动停止传输
		if(now_second - jh_state_mechine.last_start_trans_time >= jh_state_mechine.trans_time)
		{
			jh_state_mechine.start_trans_statue = false;
		}
		if(now_second - jh_state_mechine.last_trans_time>=jh_state_mechine.trans_interval)
		{
			saadc_configure_init_and_sample_schedule();
			jh_state_mechine.last_trans_time = now_second;
		}
		if(jh_state_mechine.wait_ack_statue)
		{
			//应答超时，重发传感器数据
			if(now_second - jh_state_mechine.last_trans_time >= jh_state_mechine.wait_ack_interval)
			{
				NRF_LOG_INFO("Resend sensor data");
				ble_send_sensor_data_to_master_schedule();
			}
		}
	}
}
/**
 * @description: 通过schedue来调用状态处理函数，防止阻塞
 * @param {*}
 * @return {*}
 */
static void timer_event_handler_schedule(void)
{
	app_sched_event_put(NULL, NULL, timer_event_handler);
}

/**
 * @description: 1s定时器中断函数，里面调用schedule来进行防阻塞处理
 * @param {*}
 * @return {*}
 */
static void m_1s_timer_timeout_handler(void * p_context)
{
    now_second++;
	timer_event_handler_schedule();
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
/**
 * @description: 启动1s的定时器
 * @param {*}
 * @return {*}
 */
void m_1s_timer_start(void)
{
	ret_code_t err_code = app_timer_start(m_1s_timer,M_1S_TIMER_TIMEOUT,NULL);
	APP_ERROR_CHECK(err_code);
}

/**
 * @description: 时钟初始化
 * @param {*}
 * @return {*}
 */
static void clock_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
}

/**
 * @description: timer模块初始化
 * @param {*}
 * @return {*}
 */
void user_timer_init(void)
{
	clock_init();
	timers_init();
    m_1s_timer_start();
}
