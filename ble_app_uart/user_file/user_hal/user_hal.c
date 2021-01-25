/*
 * @Author: Lemon
 * @Date: 2021-01-22 10:41:40
 * @LastEditTime: 2021-01-25 14:36:32
 * @LastEditors: Lemon
 * @Description: hal代码
 * @FilePath: \code\nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\user_hal\user_hal.c
 */
#include "user_hal.h"
#include "project_base_include.h"
#include "nrf_pwr_mgmt.h"

/**@brief Function for the Event Scheduler initialization.
 */
static void scheduler_init(void) { APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE); }

//电源管理模块

/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
void idle_state_handle(void)
{
    app_sched_execute();
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}
//看门狗模块
 void wdt_init(void)
{
    NRF_WDT->CONFIG = (WDT_CONFIG_HALT_Pause << WDT_CONFIG_HALT_Pos) | ( WDT_CONFIG_SLEEP_Run << WDT_CONFIG_SLEEP_Pos);
    NRF_WDT->CRV = 10*32768; // 15 sec. timout
    NRF_WDT->RREN |= WDT_RREN_RR0_Msk; //Enable reload register 0
    NRF_WDT->TASKS_START = 1;
}
void feed_wdt(void)
{
    NRF_WDT->RR[0] = WDT_RR_RR_Reload;
}

//log模块
/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

uint32_t user_hal_init(void)
{
    log_init();
    scheduler_init();
	power_management_init();
    NRF_LOG_INFO("HAL init finish");
}