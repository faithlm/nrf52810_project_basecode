#ifndef USER_ADC_H
#define USER_ADC_H
#include "nrfx_saadc.h"
#define LOWPOWER_THREHOLD_VALUE_MV  3200
#define LOWPOWER_THREHOLD_VALUE_MV_OUTSIDE  3650
void sample_timer_handler(void);
void saadc_configure_init_and_sample(void);
#endif
