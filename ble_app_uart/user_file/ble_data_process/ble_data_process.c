/*
 * @Author: Lemon
 * @Date: 2021-01-25 14:02:13
 * @LastEditTime: 2021-01-29 15:20:26
 * @LastEditors: Lemon
 * @Description: 
 * @FilePath: \nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\ble_data_process\ble_data_process.c
 */
#include "ble_data_process.h"
#include "user_timer.h"
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

/**
 * @description: 计算指令CRC值
 * @param {*}
 * @return {*}
 */
static uint8_t crc_calc(uint8_t * p_data,uint16_t length)
{
	uint8_t crc_value=0;
    ASSERT(length<50);

    for(uint8_t i=0;i<length;i++)
    {
        crc_value+=p_data[i];
    }
	return crc_value;
} 

/**
 * @description: 对指令数据进行组包发送
 * @param {uint8_t} op_code
 * @param {uint8_t} *p_data
 * @return {*}
 */
void data_encoded_and_send(uint8_t op_code,uint8_t *p_data)
{
	uint32_t err_code;
	uint8_t data_encoded[20]={0x4A,0x48,0x01};

	data_encoded[PROTOCOL_OPCODE_IDX] = op_code;
	if(op_code != 0x04)
	{
		data_encoded[PROTOCOL_PAYLOAD_LENGTH_IDX] = 0x01;
	}
	else
	{
		data_encoded[PROTOCOL_PAYLOAD_LENGTH_IDX] = 0x04;
	}
	memcpy(&data_encoded[PROTOCOL_PAYLOAD_CONTEXT_IDX],p_data,data_encoded[PROTOCOL_PAYLOAD_LENGTH_IDX]);
	data_encoded[5+data_encoded[PROTOCOL_PAYLOAD_LENGTH_IDX]] = crc_calc(data_encoded,data_encoded[PROTOCOL_PAYLOAD_LENGTH_IDX]+5);
	err_code = ble_send_data_to_master(data_encoded,6+data_encoded[PROTOCOL_PAYLOAD_LENGTH_IDX]);
	APP_ERROR_CHECK(err_code);
    jh_state_mechine.wait_ack_cmd = op_code|0x80;
    jh_state_mechine.wait_ack_statue = true;
}

/**
 * @description: 对指令进行应答
 * @param {uint8_t} op_code
 * @param {uint8_t} result_code
 * @return {*}
 */
void cmd_ack_send(uint8_t op_code,uint8_t result_code)
{
	uint32_t err_code;
	uint8_t data_encoded[20]={0x4A,0x48,0x01};

	data_encoded[PROTOCOL_OPCODE_IDX] = op_code|0x80;
	data_encoded[PROTOCOL_PAYLOAD_LENGTH_IDX] = 0x01;
	data_encoded[PROTOCOL_PAYLOAD_CONTEXT_IDX] = result_code;
	data_encoded[5+data_encoded[PROTOCOL_PAYLOAD_LENGTH_IDX]] = crc_calc(data_encoded,data_encoded[PROTOCOL_PAYLOAD_LENGTH_IDX]+5);
	err_code = ble_send_data_to_master(data_encoded,6+data_encoded[PROTOCOL_PAYLOAD_LENGTH_IDX]);
	APP_ERROR_CHECK(err_code);
}

/**
 * @description: 帧命令解析
 * @param {jh_frame_t} m_jh_frame
 * @return {*}
 */
uint32_t frame_process(jh_frame_t m_jh_frame)
{
    if(jh_state_mechine.wait_ack_statue)
    {
        if(jh_state_mechine.wait_ack_cmd == m_jh_frame.frame_opcode)
        {
            jh_state_mechine.wait_ack_statue = false;
            jh_state_mechine.wait_ack_cmd    = 0x00;
            return NRF_SUCCESS;
        }
    }
    switch(m_jh_frame.frame_opcode)
    {
        case JH_CMD_START_SAMPLE:
            NRF_LOG_INFO("JH_CMD_START_SAMPLE");
			cmd_ack_send(jh_frame.frame_opcode,NRF_JH_SUCCESS);
			jh_state_mechine.start_trans_statue = true;
			jh_state_mechine.last_start_trans_time = get_now_second();
            break;
        case JH_CMD_STOP_SAMPLE:
            NRF_LOG_INFO("JH_CMD_STOP_SAMPLE");
			cmd_ack_send(jh_frame.frame_opcode,NRF_JH_SUCCESS);
			jh_state_mechine.start_trans_statue = false;
            break;
        case JH_CMD_RESTART_SAMPLE:
            NRF_LOG_INFO("JH_CMD_RESTART_SAMPLE");
			cmd_ack_send(jh_frame.frame_opcode,NRF_JH_SUCCESS);
            break;
        case JH_CMD_START_TRANS:
            NRF_LOG_INFO("JH_CMD_START_TRANS");
			cmd_ack_send(jh_frame.frame_opcode,NRF_JH_SUCCESS);
			jh_state_mechine.last_start_trans_time = get_now_second();
            break;
        default:
            NRF_LOG_INFO("Error cmd 0x%x",m_jh_frame.frame_opcode);
			cmd_ack_send(jh_frame.frame_opcode,NRF_ERROR_INVALID_CMD);
            break;
    }
	return NRF_SUCCESS;
}
/**
 * @description: 将蓝牙数据转换为对应的数据帧
 * @param {uint8_t} *p_data
 * @param {uint16_t} length
 * @return {*}
 */
void frame_prase(void * p_event_data, uint16_t length)
{
    NRF_LOG_INFO("frame_prase");
	uint8_t *p_data = p_event_data;
    if((PROTOCOL_HEADER_HIGH != p_data[PROTOCOL_HEADER_HIGH_IDX])||(PROTOCOL_HEADER_LOW != p_data[PROTOCOL_HEADER_LOW_IDX]))
    {
        NRF_LOG_INFO("NRF_ERROR_INVALID_CMD");
		return;
    }
    memset(&jh_frame,0,sizeof(jh_frame_t));
    jh_frame.frame_header           = ((PROTOCOL_HEADER_HIGH<<8)+PROTOCOL_HEADER_LOW);
    jh_frame.frame_protocol_version = p_data[PROTOCOL_VERSION_IDX]; 
    jh_frame.frame_opcode           = p_data[PROTOCOL_OPCODE_IDX]; 
    jh_frame.frame_payload_length   = p_data[PROTOCOL_PAYLOAD_LENGTH_IDX]; 
    memcpy(jh_frame.frame_payload_context,&p_data[PROTOCOL_PAYLOAD_CONTEXT_IDX],p_data[PROTOCOL_PAYLOAD_LENGTH_IDX]);
    jh_frame.frame_crc              = p_data[p_data[PROTOCOL_PAYLOAD_LENGTH_IDX]+5];
	if(length != jh_frame.frame_payload_length + 6)
	{
		//指令包长度异常
		cmd_ack_send(jh_frame.frame_opcode,NRF_ERROR_INVALID_CMD_LENGTH);
		NRF_LOG_INFO("Invalid length");
		return ;
	}
	if(!crc_check(p_data,length))
	{
		//crc错误
		cmd_ack_send(jh_frame.frame_opcode,NRF_ERROR_CRC_ERROR);
		return ;
	}
	frame_process(jh_frame);
}
void frame_prase_schedule(const uint8_t * p_data,uint16_t length)
{
    app_sched_event_put((void *)p_data, length, frame_prase);
}
