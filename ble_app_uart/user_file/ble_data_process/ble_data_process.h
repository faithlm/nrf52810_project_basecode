/*
 * @Author: Lemon
 * @Date: 2021-01-25 14:02:13
 * @LastEditTime: 2021-01-25 15:55:43
 * @LastEditors: Lemon
 * @Description: 
 * @FilePath: \code\nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_uart\user_file\ble_data_process\ble_data_process.h
 */
#ifndef  BLE_DATA_PROCESS_H
#define	 BLE_DATA_PROCESS_H
#include "project_base_include.h"
#include "bluetooth.h"

//指令码定义
#define  JH_CMD_START_SAMPLE    0x01
#define  JH_CMD_STOP_SAMPLE     0x02
#define  JH_CMD_RESTART_SAMPLE  0x03
#define  JH_CMD_START_TRANS     0x04

//应答码定义,指令码&0x80
#define  JH_ACK_START_SAMPLE    0x81
#define  JH_ACK_STOP_SAMPLE     0x82
#define  JH_ACK_RESTART_SAMPLE  0x83
#define  JH_ACK_START_TRANS     0x84

//字段属性定义
#define PROTOCOL_HEADER_HIGH_IDX        0x00
#define PROTOCOL_HEADER_LOW_IDX         0x01
#define PROTOCOL_VERSION_IDX            0x02
#define PROTOCOL_OPCODE_IDX             0x03
#define PROTOCOL_PAYLOAD_LENGTH_IDX     0x04
#define PROTOCOL_PAYLOAD_CONTEXT_IDX    0x05

#define PROTOCOL_HEADER_HIGH    0x4A
#define PROTOCOL_HEADER_LOW     0x48

#define NRF_ERROR_INVALID_CMD  0x01
#define NRF_ERROR_CRC_ERROR    0x02

#define MAX_PAYLOAD_CONTEXT  20
typedef struct {
    uint16_t frame_header;
    uint8_t  frame_protocol_version;
    uint8_t  frame_opcode;
    uint8_t  frame_payload_length;
    uint8_t  frame_payload_context[MAX_PAYLOAD_CONTEXT];
    uint8_t  frame_crc;
}jh_frame_t;

void frame_prase_schedule(uint8_t * p_data,uint16_t length);
#endif
