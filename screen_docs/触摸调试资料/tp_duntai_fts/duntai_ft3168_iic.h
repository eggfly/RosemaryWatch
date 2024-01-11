


#ifndef  _DUNTAI_FT3168_IIC_H
#define  _DUNTAI_FT3168_IIC_H

#include "../hardware_task/hardware_task_config.h"

#if Choose_Tp_Stytle==TP_ID_DUNTAI_FT3168

#include "osal.h"
#include "stdint.h"


#define TP_HRS_DEVICE_I2C_ADDRESS             0x15
#define TP_HRS_DEVICE_I2C_ADDRESS_UPDATE      0x6A
#define FT3168_DEVICE_I2C_ADDRESS             0x38

#define kal_int8        int8_t
#define kal_uint8       uint8_t
#define kal_bool        uint8_t
#define kal_uint16      uint16_t
#define kal_uint32      uint32_t
#define KAL_TRUE        1
#define KAL_FALSE       0
kal_bool TP_HRS_ReadBytes(kal_uint8* Data, kal_uint8 RegAddr);
kal_bool TP_HRS_WriteBytes(kal_uint8 RegAddr, kal_uint8 Data);
kal_bool ft3168_ReadBytes(kal_uint8* Data, kal_uint8 RegAddr,kal_uint32 len);
kal_bool ft3168_WriteBytes(kal_uint8 * Data,kal_uint32 len);
kal_bool ft3168_ReadBytes_16(kal_uint8* Data, kal_uint16 RegAddr,kal_uint32 len);
kal_bool ft3168_ReadBytes_32(kal_uint8* Data, kal_uint32 RegAddr,kal_uint32 len);
//kal_bool TP_HRS_WriteBytes_updata(uint8_t device_addr,uint16_t RegAddr, uint8_t *Data,uint8_t len,uint8_t lenth);
kal_bool TP_HRS_WriteBytes_updata(uint8_t device_addr,uint16_t RegAddr, uint8_t *Data,uint16_t len,uint8_t lenth);
kal_bool TP_HRS_read_updata(uint8_t device_addr,uint16_t RegAddr, uint8_t *Data,uint8_t len,uint8_t lenth);
void tp_816_init(void);
void CaptouchInterruptHandle_1(void);

#endif

#endif
