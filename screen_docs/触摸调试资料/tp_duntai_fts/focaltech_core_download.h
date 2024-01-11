

#ifndef _focaltech_core_download_h
#define _focaltech_core_download_h
/********************************************************************************
 *                                                                           	*
 *      			            Focaltech Systems (R)                    		*
 *                                                                           	*
 *                               All Rights Reserved                     		*
 *                                                                        		*
 *  THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS      	*
 *  THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS       	*
 *  SUBJECT TO LICENSE TERMS.                               				    *
 *                                                                           	*
 *******************************************************************************/

typedef unsigned char	FTS_BYTE;
typedef unsigned short  FTS_WORD;
typedef unsigned int    FTS_DWRD;
typedef signed int      FTS_BOOL;

/*fts 26bytes protocol,support 5 points*/
#define CTPM_SUPPORT_POINTS_MAX	     2//0x05


#define FTS_NULL 	0x0
#define FTS_TRUE 	0x01
#define FTS_FALSE 	0x0

#define FTS_TOUCH_POINT_INFO_Headaddress   0x00
#define FTS_TOUCH_POINT_INFO_LEN           (3+6*CTPM_SUPPORT_POINTS_MAX)


#define POINTER_CHECK(p)	if((p)==FTS_NULL){return FTS_FALSE;}

/*Note: please modify this MACRO if the slave address changed*/
#define I2C_CTPM_ADDRESS 	0x70



typedef enum
{
	ERR_OK,
	ERR_MODE,
	ERR_READID,
	ERR_ERASE,
	ERR_STATUS,
	ERR_ECC
}E_UPGRADE_ERR_TYPE;

 /* 
 Error status codes:
 */
#define CTPM_NOERROR			(0x01 << 0)
#define CTPM_ERR_PARAMETER		(0x01 << 1)
#define CTPM_ERR_PROTOCOL		(0x01 << 2)
#define CTPM_ERR_ECC			(0x01 << 3)
#define CTPM_ERR_MODE			(0x01 << 4)
#define CTPM_ERR_I2C			(0x01 << 5)
#define CTPM_ERR_SCAN			(0x01 << 6)



/*the information of one touch point */
typedef struct
{
	/*x coordinate*/
	FTS_WORD	w_tp_x;

	/*y coordinate*/
	FTS_WORD 	w_tp_y;

	/*point id: start from 0  to  0x0f*/
	FTS_BYTE 	bt_tp_id;

	/*0 means press down; 1 means put up ;2 means contact*/
	FTS_BYTE 	bt_tp_event;
	/*touch area:the size  of touch */
    FTS_BYTE 	bt_tp_area;
	/*the strength of the press*/
	FTS_BYTE 	w_tp_pressure;
}ST_TOUCH_POINT, *PST_TOUCH_POINT;

typedef enum
{
	TYPE_Z00M_IN,
	TYPE_Z00M_OUT,
	TYPE_INVALIDE
}E_GESTURE_TYPE;

/*the information of one touch */
typedef struct
{
	/*the number of touch points*/
	FTS_BYTE 			bt_tp_num;

	/*touch gesture*/
	E_GESTURE_TYPE		bt_gesture;

	/*point to a list which stored 1 to 5 touch points information*/
	ST_TOUCH_POINT* 	pst_point_info;
}ST_TOUCH_INFO, *PST_TOUCH_INFO;

/*
[function]: 
	write a value to register.
[parameters]:
	e_reg_name[in]	:register name;
	pbt_buf[in]		:the returned register value;
[return]:
	FTS_TRUE	:success;
	FTS_FALSE	:io fail;
*/
extern FTS_BYTE fts_register_read(FTS_BYTE e_reg_name, FTS_BYTE* pbt_buf, FTS_BYTE bt_len);

/*
[function]: 
	read out the register value.
[parameters]:
	e_reg_name[in]	:register name;
	pbt_buf[out]	:the returned register value;
	bt_len[in]		:length of pbt_buf, should be set to 2;		
[return]:
	FTS_TRUE	:success;
	FTS_FALSE	:io fail;
*/
extern FTS_BOOL fts_register_write(FTS_BYTE write_reg_address, FTS_BYTE* writebuf,FTS_BYTE write_len);

/*
[function]: 
	get all the information of one touch.
[parameters]:
	pst_touch_info[out]	:stored all the information of one touch;	
[return]:
	CTPM_NOERROR		:success;
	CTPM_ERR_I2C		:io fail;
	CTPM_ERR_PROTOCOL	:protocol data error;
	CTPM_ERR_ECC		:ecc error.
*/
extern FTS_BYTE fts_ctpm_get_touch_info(ST_TOUCH_INFO* pst_touch_info);

/*
[function]: 
	burn the FW to ctpm.
[parameters]:(ref. SPEC)
	pbt_buf[in]	:point to Head+FW ;
	dw_lenth[in]:the length of the FW + 6(the Head length);	
	bt_ecc[in]	:the ECC of the FW
[return]:
	ERR_OK		:no error;
	ERR_MODE	:fail to switch to UPDATE mode;
	ERR_READID	:read id fail;
	ERR_ERASE	:erase chip fail;
	ERR_STATUS	:status error;
	ERR_ECC		:ecc error.
*/
extern E_UPGRADE_ERR_TYPE fts_ctpm_fw_upgrade(FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth, FTS_BYTE bt_ecc);

#ifdef __cplusplus
}
#endif
#endif
