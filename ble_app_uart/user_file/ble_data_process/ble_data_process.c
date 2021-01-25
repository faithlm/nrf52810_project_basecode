/*
 * @Author: Lemon
 * @Date: 2021-01-25 14:02:13
 * @LastEditTime: 2021-01-25 16:35:29
 * @LastEditors: Lemon
 * @Description: 
 * @FilePath: \code\nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\ble_data_process\ble_data_process.c
 */
#include "ble_data_process.h"
static jh_frame_t jh_frame;

/**
 * @description: 检查数据帧CRC是否符合规则
 * @param {uint8_t *} p_data
 * @param {uint16_t} length
 * @return {bool}
 */
bool crc_check(uint8_t * p_data,uint16_t length)
{
    uint8_t crc_value=0;
    ASSERT(length<50);

    for(uint8_t i=0;i<length-1;i++)
    {
        crc_value+=p_data[i];
    }
    if(p_data[length-1] == crc_value)
    {
        return true;
    }
    else
    {
        NRF_LOG_INFO("CRC check error,should be 0x%x,now is 0x%x",crc_value,p_data[length-1]);
        return false;
    }
}
uint32_t frame_process(jh_frame_t m_jh_frame)
{
    switch(m_jh_frame.frame_opcode)
    {
        case JH_CMD_START_SAMPLE:
            NRF_LOG_INFO("JH_CMD_START_SAMPLE");
            break;
        case JH_CMD_STOP_SAMPLE:
            NRF_LOG_INFO("JH_CMD_START_SAMPLE");
            break;
        case JH_CMD_RESTART_SAMPLE:
            NRF_LOG_INFO("JH_CMD_START_SAMPLE");
            break;
        case JH_CMD_START_TRANS:
            NRF_LOG_INFO("JH_CMD_START_SAMPLE");
            break;
        default:
            NRF_LOG_INFO("Error cmd 0x%x",m_jh_frame.frame_opcode);
            break;
    }
}
/**
 * @description: 将蓝牙数据转换为对应的数据帧
 * @param {uint8_t} *p_data
 * @param {uint16_t} length
 * @return {*}
 */
uint32_t frame_prase(uint8_t * p_data,uint16_t length)
{
    NRF_LOG_INFO("frame_prase");
    if((PROTOCOL_HEADER_HIGH != p_data[PROTOCOL_HEADER_HIGH_IDX])||(PROTOCOL_HEADER_LOW != p_data[PROTOCOL_HEADER_LOW_IDX]))
    {
        NRF_LOG_INFO("NRF_ERROR_INVALID_CMD");
        return NRF_ERROR_INVALID_CMD;
    }
    if(!crc_check(p_data,length))
    return NRF_ERROR_CRC_ERROR;
    memset(&jh_frame,0,sizeof(jh_frame_t));
    jh_frame.frame_header           = 0x4A48;
    jh_frame.frame_protocol_version = p_data[PROTOCOL_VERSION_IDX]; 
    jh_frame.frame_opcode           = p_data[PROTOCOL_OPCODE_IDX]; 
    jh_frame.frame_payload_length   = p_data[PROTOCOL_PAYLOAD_LENGTH_IDX]; 
    memcpy(jh_frame.frame_payload_context,&p_data[PROTOCOL_PAYLOAD_CONTEXT_IDX],p_data[PROTOCOL_PAYLOAD_LENGTH_IDX]);
    jh_frame.frame_crc              = p_data[p_data[PROTOCOL_PAYLOAD_LENGTH_IDX]+5];

}
void frame_prase_schedule(uint8_t * p_data,uint16_t length)
{
    APP_ERROR_CHECK(app_sched_event_put(p_data, length, frame_prase));
}
