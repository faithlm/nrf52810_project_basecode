/*
 * @Author: Lemon
 * @Date: 2021-01-22 10:40:04
 * @LastEditTime: 2021-01-29 15:25:08
 * @LastEditors: Lemon
 * @Description: 
 * @FilePath: \nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\user_saadc\user_adc.h
 */
#ifndef USER_ADC_H
#define USER_ADC_H
#include "nrfx_saadc.h"

#define LOWPOWER_THREHOLD_VALUE_MV  3200

#define SAADC_CHANNEL_TOTAL_COUNT   3
#define SAADC_CHANNEL_VDD			0
#define SAADC_CHANNEL_LOW_TEMP		1
#define SAADC_CHANNEL_HIGH_TEMP		2

#define MCP9700A_VOLATAGE_IN_MV_0_TEMPERATURE	500
#define MCP9700A_VOLATAGE_IN_MV_LINEARITY	    10

#define VDD_VOLTAGE_UNIT            30

void saadc_configure_init_and_sample_schedule(void);
void ble_send_sensor_data_to_master_schedule(void);
#endif
