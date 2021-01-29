/*
 * @Author: Lemon
 * @Date: 2021-01-22 10:39:49
 * @LastEditTime: 2021-01-29 15:13:09
 * @LastEditors: Lemon
 * @Description: bsp相关代码
 * @FilePath: \nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\user_bsp\user_bsp.c
 */
#include "user_bsp.h"

static bsp_indication_t last_bsp_statue = BSP_INDICATE_FIRST;

static void bsp_event_handler(bsp_event_t event)
{
    switch (event)
    {
        default:
            break;
    }
}

/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
static void buttons_leds_init(void)
{
    bsp_event_t startup_event;

    uint32_t err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(err_code);

    err_code = bsp_btn_ble_init(NULL, &startup_event);
    APP_ERROR_CHECK(err_code);

}

/**
 * @description: bsp模块初始化
 * @param {*}
 * @return {*}
 */
uint32_t user_bsp_init(void)
{
    buttons_leds_init();
}



