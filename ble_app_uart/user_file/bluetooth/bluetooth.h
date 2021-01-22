/*
 * @Author: Lemon
 * @Date: 2021-01-22 09:38:06
 * @LastEditTime: 2021-01-22 11:02:36
 * @LastEditors: Lemon
 * @Description: In User Settings Edit
 * @FilePath: \nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\bluetooth\bluetooth.h
 */
#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"

#include "ble_config.h"
#include "ble_nus.h"


uint32_t user_bluetooth_init(void);

#endif
