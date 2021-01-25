/*
 * @Author: Lemon
 * @Date: 2021-01-22 10:41:40
 * @LastEditTime: 2021-01-25 14:42:04
 * @LastEditors: Lemon
 * @Description: HAL 头文件
 * @FilePath: \code\nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\user_hal\user_hal.h
 */
#ifndef USER_HAL_H
#define USER_HAL_H
#include "project_base_include.h"

#define SCHED_MAX_EVENT_DATA_SIZE       50                                      /**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE               10                                      /**< Maximum number of events in the scheduler queue. */

void idle_state_handle(void);
uint32_t user_hal_init(void);
#endif
