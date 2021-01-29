/*
 * @Author: Lemon
 * @Date: 2021-01-22 10:40:04
 * @LastEditTime: 2021-01-29 15:10:33
 * @LastEditors: Lemon
 * @Description: 
 * @FilePath: \nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\user_saadc\user_adc.c
 */
 #include "project_base_include.h"
#include "user_adc.h"
#include "app_scheduler.h"
#include "bluetooth.h"
#include "ble_data_process.h"

static nrf_saadc_value_t samples[SAADC_CHANNEL_TOTAL_COUNT];

//Channel 0 用于测试VDD电压
//channel 1 用于测试低温传感器温度
//channel 2 用于测量高温传感器温度
static nrfx_saadc_channel_t channels[SAADC_CHANNEL_TOTAL_COUNT] = {NRFX_SAADC_DEFAULT_CHANNEL_SE(NRF_SAADC_INPUT_VDD, 0),
                                                             NRFX_SAADC_DEFAULT_CHANNEL_SE(NRF_SAADC_INPUT_AIN3, 1),
                                                            NRFX_SAADC_DEFAULT_CHANNEL_SE(NRF_SAADC_INPUT_AIN1, 2)};
//vdd电压，单位mv
static uint32_t vdd_volatage_in_mv;
//低温传感器温度值
static int8_t current_low_temperature = 0;
//由于需要查表及一定的算法获取温度值,时间消耗比较大，因此只记录adc的原始值，在发送的时候进行计算温度。
static int16_t current_high_temperature_adc_value = 0;

//传感器数据，1字节VDD电压，1字节低温传感器温度，2字节高温传感器温度，数据类型为int8
static int8_t sensor_data[4]; 

/**
 * @description: adc采样值转化vdd电压值
 * @param {nrf_saadc_value_t} adc_value
 * @return {*}
 */
uint32_t adc_value_convert_to_volatage_in_mv(nrf_saadc_value_t adc_value)
{
	return (uint32_t)(adc_value*100*6*6/4095);
}

/**
 * @description: adc采样值转化为低温传感器温度值，参考9700A文档
 * @param {nrf_saadc_value_t} adc_value
 * @return {*}int8_t
 */
int8_t adc_value_convert_to_temperature_low(nrf_saadc_value_t adc_value)
{
	uint16_t temp_mv = 0;
	temp_mv = adc_value*100*3*6/4095;
	return (temp_mv - MCP9700A_VOLATAGE_IN_MV_0_TEMPERATURE)/MCP9700A_VOLATAGE_IN_MV_LINEARITY;
} 
/**
 * @description: adc采样值转化为高温传感器温度值
 * @param {nrf_saadc_value_t} adc_value
 * @return {*}int
 */
int16_t adc_value_convert_to_temperature_high(nrf_saadc_value_t adc_value)
{
	return -10; 
//	return current_high_temperature_adc_value;
}

/**
 * @description: 将传感器数据进行编码，发送给蓝牙主机
 * @param {*}
 * @return {*}
 */
static void sensor_data_encoded_and_send(void * p_event_data, uint16_t event_size)
{
	if(jh_state_mechine.connect_statue)
	{
		int16_t temp = 0;
		sensor_data[0] = vdd_volatage_in_mv/VDD_VOLTAGE_UNIT;
		sensor_data[1] = current_low_temperature;
		temp = adc_value_convert_to_temperature_high(current_high_temperature_adc_value);
		sensor_data[2] = (temp&0xFF00)>>8;
		sensor_data[3] = (int8_t)(temp&0x00FF);

		//NRF_LOG_INFO("send data to app");
		//NRF_LOG_HEXDUMP_INFO(sensor_data,4);
		data_encoded_and_send(JH_CMD_START_TRANS,sensor_data);	
	}
}
/**
 * @description: 通过schedule调用发送接口，防止在中断中阻塞
 * @param {*}
 * @return {*}
 */
void ble_send_sensor_data_to_master_schedule(void)
{
	app_sched_event_put(NULL, NULL, sensor_data_encoded_and_send);
}
/**
 * @description: saadc中断函数
 * @param {nrfx_saadc_evt_tconst} *
 * @return {*}
 */
void saadc_event_handler(nrfx_saadc_evt_t const * p_event)
{
	uint32_t err_code;
    if (p_event->type == NRFX_SAADC_EVT_DONE)
    {
		vdd_volatage_in_mv = adc_value_convert_to_volatage_in_mv(p_event->data.done.p_buffer[SAADC_CHANNEL_VDD]);
		NRF_LOG_INFO("VDD volatage = %dmv",vdd_volatage_in_mv);
		current_low_temperature = adc_value_convert_to_temperature_low(p_event->data.done.p_buffer[SAADC_CHANNEL_LOW_TEMP]);
		NRF_LOG_INFO("Current low_temperature = %d C",current_low_temperature);
		current_high_temperature_adc_value = p_event->data.done.p_buffer[SAADC_CHANNEL_HIGH_TEMP];
    }
	ble_send_sensor_data_to_master_schedule();
    nrfx_saadc_uninit();
    /* Applying workaround from Errata 212, otherwise current is stuck at 4-500uA during sleep after first sample. */
    volatile uint32_t temp1;
    volatile uint32_t temp2;
    volatile uint32_t temp3;

    temp1 = *(volatile uint32_t *)0x40007640ul;
    temp2 = *(volatile uint32_t *)0x40007644ul;
    temp3 = *(volatile uint32_t *)0x40007648ul;

    *(volatile uint32_t *)0x40007FFCul = 0ul; 
    *(volatile uint32_t *)0x40007FFCul; 
    *(volatile uint32_t *)0x40007FFCul = 1ul;

    *(volatile uint32_t *)0x40007640ul = temp1;
    *(volatile uint32_t *)0x40007644ul = temp2;
    *(volatile uint32_t *)0x40007648ul = temp3;
}

/**
 * @description: 配置初始化saadc，采样完以后进行关闭，降低功耗
 * @param {*}
 * @return {*}
 */
static void saadc_configure_init_and_sample(void * p_event_data, uint16_t event_size)
{
	uint32_t err_code;
    err_code = nrfx_saadc_init(NRFX_SAADC_CONFIG_IRQ_PRIORITY);
    APP_ERROR_CHECK(err_code);


	//低温传感器，电压范围 100mv -- 1.75V,使用1/3的分压
	channels[SAADC_CHANNEL_LOW_TEMP].channel_config.gain = NRF_SAADC_GAIN1_3;

	//高温传感器，由于电压VCC是变化的，选择1/4 vcc也就是1/4 VDD作为参考电压,从而保证采样参考值为固定值
	channels[SAADC_CHANNEL_HIGH_TEMP].channel_config.reference = NRF_SAADC_REFERENCE_VDD4;
	channels[SAADC_CHANNEL_HIGH_TEMP].channel_config.gain = NRF_SAADC_GAIN1_4;

    err_code = nrfx_saadc_channels_config(channels, SAADC_CHANNEL_TOTAL_COUNT);
    APP_ERROR_CHECK(err_code);

	err_code = nrfx_saadc_simple_mode_set((1<<0|1<<1|1<<2),
                                              NRF_SAADC_RESOLUTION_12BIT,
                                              NRF_SAADC_OVERSAMPLE_4X,
                                              saadc_event_handler);
	APP_ERROR_CHECK(err_code);
	
	err_code = nrfx_saadc_buffer_set(samples, SAADC_CHANNEL_TOTAL_COUNT);
	APP_ERROR_CHECK(err_code);

	err_code = nrfx_saadc_mode_trigger();
	APP_ERROR_CHECK(err_code);

}
/**
 * @description: 通过schedule调用adc配置初始化，防止在中断中阻塞
 * @param {*}
 * @return {*}
 */
void saadc_configure_init_and_sample_schedule(void)
{
    app_sched_event_put(NULL, NULL, saadc_configure_init_and_sample);
}


