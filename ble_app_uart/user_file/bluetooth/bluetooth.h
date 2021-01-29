/*
 * @Author: Lemon
 * @Date: 2021-01-22 09:38:06
 * @LastEditTime: 2021-01-29 15:16:29
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

#define DEFAULT_TRANS_INTERVAL  5
#define DEFAULT_TRANS_TIME		20*60
#define	DEFAULT_WAOT_ACK_TIME	3

typedef struct {
	uint32_t trans_time;//工作时间，如果没有收到停止信号，超时以后停止数据发送
	uint32_t last_start_trans_time;
	uint32_t last_trans_time;
	uint32_t last_cmd_time;
	uint8_t trans_interval;//传输间隔，每隔多久发送一次数据。
	uint8_t wait_ack_interval;//等待应答超时时间
	uint8_t wait_ack_cmd;
	bool start_trans_statue;
	bool connect_statue;
	bool low_power_statue;
	bool wait_ack_statue;

}jh_state_mechine_t;

extern jh_state_mechine_t jh_state_mechine;
uint32_t user_bluetooth_init(void);
uint32_t ble_send_data_to_master(uint8_t *p_data,uint16_t length);
#endif
