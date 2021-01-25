/*
 * @Author: Lemon
 * @Date: 2021-01-22 09:20:14
 * @LastEditTime: 2021-01-22 11:46:11
 * @LastEditors: Lemon
 * @Description: main.c
 * @FilePath: \nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\main.c
 */
#include "project_base_include.h"
#include "user_hal.h"
#include "user_timer.h"
#include "bluetooth.h"
#include "user_adc.h"
/**
 * @description: 系统初始化
 * @param {*}
 * @return {*}
 */
void system_init(void)
{
    user_hal_init();
    user_timer_init();
    user_bluetooth_init();
	saadc_configure_init_and_sample();
}

int main(void)
{
    system_init();
    // Enter main loop.
    for (;;)
    {
        idle_state_handle();
    }
}
