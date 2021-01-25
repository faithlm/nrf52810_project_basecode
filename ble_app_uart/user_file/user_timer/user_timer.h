/*
 * @Author: Lemon
 * @Date: 2021-01-22 10:39:55
 * @LastEditTime: 2021-01-25 14:53:28
 * @LastEditors: Lemon
 * @Description: 定时器模块头文件
 * @FilePath: \code\nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\user_timer\user_timer.h
 */
#ifndef USER_TIMER_H
#define USER_TIMER_H

#include "nrf_drv_clock.h"
#include "app_timer.h"

#define TIMER_1MS_COUNT         50
#define M_1S_TIMER_TIMEOUT   APP_TIMER_TICKS(1000) 

void user_timer_init(void);
#endif
