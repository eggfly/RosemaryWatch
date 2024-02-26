#ifndef __FT3X68_H
#define __FT3X68_H	
#include "sys.h"	


//与电容触摸屏连接的芯片引脚(未包含IIC引脚) 
//IO操作函数	 
#define FT3X68_RST    		PBout(3)	//复位引脚
#define FT3X68_INT    		PCin(3)		//中断引脚	

//I2C读写命令	
#define FT3X68_CMD_WR 		0X70    //写命令
#define FT3X68_CMD_RD 		0X71		//读命令

//FT3X68 部分寄存器定义 
#define FT3X68_DEVIDE_MODE 0x00   //模式控制寄存器
#define FT3X68_TD_STATUS   0x02		//触摸状态寄存器

#define FT3X68_TP1_REG 		 0X03	  //第一个触摸点数据地址
#define FT3X68_TP2_REG 		 0X09		//第二个触摸点数据地址
#define FT3X68_TP3_REG 		 0X0F		//第三个触摸点数据地址
#define FT3X68_TP4_REG 		 0X15		//第四个触摸点数据地址
#define FT3X68_TP5_REG 		 0X1B		//第五个触摸点数据地址  
#define ID_G_CIPHER_HIGH     0xA3
#define ID_G_CIPHER_MID      0x9F
#define ID_G_CIPHER_LOW      0xA0
 
#define	FT3X68_ID_G_LIB_VERSION		0xA1		//版本		
#define	FT3X68_Chip_vendor_ID		  0xA3		//Chip vendor ID	
#define FT3X68_ID_G_MODE 			    0xA4   	//中断模式控制寄存器
#define FT3X68_ID_G_THGROUP			  0x80   	//触摸有效值设置寄存器
#define FT3X68_ID_G_PERIODACTIVE	0x88   	//激活状态周期设置寄存器

#define FTS_CHIP_IDH    0x64
#define FTS_CHIP_IDL    0x56

#define FTS_FW_VER       0x02
#define FTS_CMD_READ_ID  0x90

#define FTS_UPGRADE_AA   0xAA
#define FTS_UPGRADE_55   0x55

#define FTS_REG_FW_VER    0xA6
#define FTS_REG_UPGRADE   0xFC

#define FTS_RETRIES_CHECK_ID                        20
#define FTS_DELAY_UPGRADE_AA                        10
#define FTS_DELAY_UPGRADE_RESET                     400
#define FTS_DELAY_READ_ID                           20
#define FTS_DELAY_UPGRADE                           80
#define FTS_RETRIES_UPGRADE                         2

#define FTS_CMD_RESET                               0x07
#define FTS_CMD_FLASH_MODE                          0x09
#define FTS_FLASH_MODE_UPGRADE_VALUE                0x0B
#define FTS_CMD_FLASH_STATUS                        0x6A
#define FTS_CMD_ERASE_APP                           0x61

#define FTS_CMD_START_DELAY                 		12

/* register address */
#define FTS_REG_WORKMODE                    		0x00
#define FTS_REG_WORKMODE_FACTORY_VALUE      		0x40
#define FTS_REG_WORKMODE_SCAN_VALUE				    	0xC0
#define FTS_REG_FLOW_WORK_CNT               		0x91
#define FTS_REG_POWER_MODE                  		0xA5
#define FTS_REG_GESTURE_EN                  		0xD0
#define FTS_REG_GESTURE_ENABLE              		0x01
#define FTS_REG_GESTURE_OUTPUT_ADDRESS      		0xD3



u8 FT3X68_WR_Reg(u16 reg,u8 *buf,u8 len);
void FT3X68_RD_Reg(u16 reg,u8 *buf,u8 len);
u8 FT3X68_Init(void);
u16 FT3X68_Scan(u8 mode);
int Get_FT3X68_FW_Ver(void);
void FT3X68_Enter_Factory_Mode(void);
u16 Get_FT3X68_ID(void);
void FT3X68_FW_Upgrade(void);
void FT3X68_Channel_Num_Test(void);

#endif

















