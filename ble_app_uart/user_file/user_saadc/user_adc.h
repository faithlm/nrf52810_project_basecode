#ifndef USER_ADC_H
#define USER_ADC_H
#include "nrf_drv_saadc.h"
#include "user_log.h"
#include "user_bsp.h"
#include "bluetooth.h"
#define LOWPOWER_THREHOLD_VALUE_MV  3200
#define LOWPOWER_THREHOLD_VALUE_MV_OUTSIDE  3650
void saadc_configure_init_and_sample(void);
uint16_t get_volate_value(void);
uint16_t get_temp_value(void);
bool get_low_power_statue(void);
#endif
