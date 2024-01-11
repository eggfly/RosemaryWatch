#include "../hardware_task/hardware_task_config.h"
#include "duntai_ft3168_iic.h"
#include "platform_devices.h"
#include "ad_i2c.h"
#include "peripheral_gpio_config.h"
#include "hw_wkup_da1469x.h"
#include "sys_watchdog.h"
#include "../application_task/gui_config_page/gui_config.h"

#if Choose_Tp_Stytle==TP_ID_DUNTAI_FT3168



kal_bool duntai_ft3168_ReadBytes(kal_uint8* Data, kal_uint8 RegAddr,kal_uint32 len)
{
    int ret;

    ad_i2c_handle_t hdl;
#if dg_configUSE_TP
    hdl = ad_i2c_open(&ctp_i2c_cfg);
    ret = ad_i2c_write(hdl, &RegAddr, 1, HW_I2C_F_ADD_STOP);
    //DriverDelay_DelayUs(50);
    if (ret == 0)
        {
        ret=ad_i2c_read(hdl, Data, len, HW_I2C_F_ADD_STOP | HW_I2C_F_ADD_RESTART);
    }
    ad_i2c_close(hdl,false);
#endif

    return ret;

}
/******************************************
         write bytes
*******************************************/
kal_bool duntai_ft3168_WriteBytes(kal_uint8 * Data,kal_uint32 len)
{
        int ret;
#if dg_configUSE_TP
        ad_i2c_handle_t hdl;
        hdl = ad_i2c_open(&ctp_i2c_cfg);
        ret = ad_i2c_write(hdl,Data,len,HW_I2C_F_ADD_STOP);
        ad_i2c_close(hdl,false);
#endif
        return ret;
}
kal_bool duntai_ft3168_WriteBytes_reg(kal_uint8 reg,kal_uint8 * Data,kal_uint32 len)
{



        int ret;
#if dg_configUSE_TP
        ad_i2c_handle_t hdl;
        hdl = ad_i2c_open(&ctp_i2c_cfg);
        ret = ad_i2c_write(hdl,&reg,1,HW_I2C_F_ADD_STOP);
        DriverDelay_DelayUs(50);
        ret = ad_i2c_write(hdl,Data,len,HW_I2C_F_ADD_STOP);
        ad_i2c_close(hdl,false);
#endif
        return ret;
}

//kal_bool duntai_ft3168_ReadBytes_16(uint8_t device_addr,uint16_t RegAddr, uint8_t *Data,uint8_t len,uint8_t lenth)
kal_bool duntai_ft3168_ReadBytes_16(kal_uint8* Data, kal_uint16 RegAddr,kal_uint32 len)
{
    int ret;
    uint8_t reg_buf[2];
    reg_buf[0] = RegAddr>>8;
    reg_buf[1] = RegAddr;

#if dg_configUSE_TP
    ad_i2c_handle_t hdl;

    hdl = ad_i2c_open(&ctp_i2c_cfg);
    ret = ad_i2c_write(hdl, reg_buf, 2, HW_I2C_F_ADD_STOP);
    if (ret == 0)
        {
        ret=ad_i2c_read(hdl, Data, len, HW_I2C_F_ADD_STOP | HW_I2C_F_ADD_RESTART);
    }
    ad_i2c_close(hdl,false);
#endif
    return ret;
}

kal_bool duntai_ft3168_ReadBytes_32(kal_uint8* Data, kal_uint32 RegAddr,kal_uint32 len)
{
    int ret;
    uint8_t reg_buf[4];
    reg_buf[0] = RegAddr>>24;
    reg_buf[1] = RegAddr>>16;
    reg_buf[2] = RegAddr>>8;
    reg_buf[3] = RegAddr;

#if dg_configUSE_TP
    ad_i2c_handle_t hdl;

    hdl = ad_i2c_open(&ctp_i2c_cfg);
    ret = ad_i2c_write(hdl, reg_buf, 4, HW_I2C_F_ADD_STOP);
    if (ret == 0)
        {
        ret=ad_i2c_read(hdl, Data, len, HW_I2C_F_ADD_STOP | HW_I2C_F_ADD_RESTART);
    }
    ad_i2c_close(hdl,false);
#endif
    return ret;
}

kal_bool TP_ft3168ReadBytes(kal_uint8* Data, kal_uint32 len)
{
        int ret;
#if dg_configUSE_TP
    ad_i2c_handle_t hdl;

    hdl = ad_i2c_open(&ctp_i2c_cfg);
    //ret = ad_i2c_write(hdl, &RegAddr, 1, HW_I2C_F_ADD_STOP);
   // if (ret == 0)
        {
        ret=ad_i2c_read(hdl, Data, len, HW_I2C_F_ADD_STOP | HW_I2C_F_ADD_RESTART);
    }
    ad_i2c_close(hdl,false);
#endif
    return ret;

}


kal_bool TP_HRS_ReadBytes(kal_uint8* Data, kal_uint8 RegAddr)
{
        int ret;
#if dg_configUSE_TP
    ad_i2c_handle_t hdl;

    hdl = ad_i2c_open(&ctp_i2c_cfg);
    ret = ad_i2c_write(hdl, &RegAddr, 1, HW_I2C_F_ADD_STOP);
    if (ret == 0)
        {
        ret=ad_i2c_read(hdl, Data, 1, HW_I2C_F_ADD_STOP | HW_I2C_F_ADD_RESTART);
    }
    ad_i2c_close(hdl,false);
#endif
    return ret;

}
/******************************************
         write bytes
*******************************************/
kal_bool TP_HRS_WriteBytes(kal_uint8 RegAddr, kal_uint8 Data)
{
        uint8_t data[2];
        data[0]=RegAddr;
        data[1]=Data;
        int ret;
#if dg_configUSE_TP
        ad_i2c_handle_t hdl;
        uint8_t buf[2];
        buf[0]=RegAddr;
        buf[1]=data;
        hdl = ad_i2c_open(&ctp_i2c_cfg);
        ret = ad_i2c_write(hdl,buf,2,HW_I2C_F_ADD_STOP);
        ad_i2c_close(hdl,false);
#endif
        return ret;
}


void test_gpio_config(HW_GPIO_PORT port, HW_GPIO_PIN pin, HW_GPIO_MODE mode,HW_GPIO_FUNC function, bool high, HW_GPIO_POWER power)
{
        hw_sys_pd_com_enable();
        hw_gpio_configure_pin(port, pin, mode, function, high);
        hw_gpio_configure_pin_power(port, pin, power);
        hw_gpio_pad_latch_enable(port, pin);
        hw_gpio_pad_latch_disable(port, pin);
        hw_sys_pd_com_disable();
}
void duntai_ft3168e_reset(void)
{
        test_gpio_config(TP_RST_PORT,TP_RST_PIN,HW_GPIO_MODE_OUTPUT_PUSH_PULL,HW_GPIO_FUNC_GPIO,true,HW_GPIO_POWER_VDD1V8P);
        test_gpio_config(TP_RST_PORT,TP_RST_PIN,HW_GPIO_MODE_OUTPUT_PUSH_PULL,HW_GPIO_FUNC_GPIO,false,HW_GPIO_POWER_VDD1V8P);
        OS_DELAY_MS(5);
        test_gpio_config(TP_RST_PORT,TP_RST_PIN,HW_GPIO_MODE_OUTPUT_PUSH_PULL,HW_GPIO_FUNC_GPIO,true,HW_GPIO_POWER_VDD1V8P);
        OS_DELAY_MS(300);


}
kal_bool duntai_ft3168_get_id(void)
{
        uint8_t duntai_ft3168_id[2] = {0};

        duntai_ft3168_ReadBytes(&duntai_ft3168_id[0], 0xA3, 1);

        printf("\n duntai_ft3168_id=%x\n",duntai_ft3168_id[0]);

        if(duntai_ft3168_id[0]==0x64)
        {
//
//                //fts_ctpm_Upgrade_process();
//                ft3168_fire_download_process();
        }

}


void tp_init(void)
{
        duntai_ft3168e_reset();
        duntai_ft3168_get_id();
}

void tp_wake_up(void)
{
  duntai_ft3168e_reset();
}
void tp_sleep_in(void)
{
        TP_HRS_WriteBytes(0xA5,3);
}

uint32_t last_tick_count = 0;

void CaptouchInterruptHandle_1(void)
{
        static uint32_t last_tp_buffer_first_num=0;
        static uint32_t time_press_tick=0;
        static uint32_t press_status=0;
        uint8_t tp_point_buf[14]={0};
        uint32_t tmp_x = 0;
        uint32_t tmp_y = 0;

        duntai_ft3168_ReadBytes(tp_point_buf, 0x02,5);
        tmp_x=(((uint16_t)((tp_point_buf[1] & 0x0F)<<8))+(uint16_t)tp_point_buf[2]);
        tmp_y=(((uint16_t)((tp_point_buf[3] & 0x0F)<<8))+(uint16_t)tp_point_buf[4]);

        // printf("\r\n tp_point_buf11=0x%04x,0x%04x,tick = %d ,0x%02x,0x%02x,0x%02x,0x%02x\r\n",tmp_x,tmp_y,(unsigned int)OS_GET_TICK_COUNT(),tp_point_buf[1],tp_point_buf[2],tp_point_buf[3],tp_point_buf[4]);

       if(((tp_point_buf[1]>>4)&0x0f==0x04)||(last_tp_buffer_first_num>0&&tp_point_buf[0]==0))
        {
              // printf("\r\n TP_STATE__LIFT--x0=%d,y0 = %d \r\n",tmp_x ,tmp_y );
          UserTpA_Calculate(TP_STATE__LIFT, tmp_x, tmp_y);
          last_tp_buffer_first_num=0;
          if(press_status==1)
          {
              press_status=0;
          }
         // Tp_Lift_To_Slide_Screen(time_press_tick);
        }
       else
       {
              // printf("\r\n TP_STATE__PRESS--x0=%d,y0 = %d \r\n",tmp_x ,tmp_y );
           UserTpA_Calculate(TP_STATE__PRESS, tmp_x, tmp_y);
          if(press_status==0)
          {
               press_status=1;
              time_press_tick=OS_TICKS_2_MS(OS_GET_TICK_COUNT());
          }
       }


       last_tp_buffer_first_num=tp_point_buf[0];
//        it7259_ReadBytes(tp_point_buf, 0x80,1);
//          if(tp_point_buf[0]&0x80)  //有手指信息
//          {
//                  it7259_ReadBytes(tp_point_buf, 0xE0,5);
//
//                  tmp_x=(((uint16_t)(tp_point_buf[3] & 0x0F)<<8)+(uint16_t)tp_point_buf[2]);
//                  tmp_y=(((uint16_t)(tp_point_buf[3] & 0xF0)<<4)+(uint16_t)tp_point_buf[4]);
//                  printf("\r\n tp_point_buf11=%x,%x,tick = %d \r\n",tmp_x,tmp_y,(unsigned int)OS_GET_TICK_COUNT());
//                  if(tmp_x>320)tmp_x=320;
//                  if(tmp_y>320)tmp_y=320;
//                  if(tmp_x==0&&tmp_y==0)
//                  {
//                     UserTpA_Calculate(TP_STATE__LIFT, tmp_x, tmp_y);
//                  }
//                  else
//                  {
//
//
//                    UserTpA_Calculate(TP_STATE__PRESS, 320-tmp_x, 320-tmp_y);
//                  }
//
//          }
//          else
//          {
//                  it7259_ReadBytes(tp_point_buf, 0xE0,5);
//
//                  tmp_x=(((uint16_t)(tp_point_buf[3] & 0x0F)<<8)+(uint16_t)tp_point_buf[2]);
//                  tmp_y=(((uint16_t)(tp_point_buf[3] & 0xF0)<<4)+(uint16_t)tp_point_buf[4]);
//                  printf("\r\n tp_point_buf22=%x,%x,tick = %d \r\n",tmp_x,tmp_y,(unsigned int)OS_GET_TICK_COUNT());
//                  UserTpA_Calculate(TP_STATE__LIFT, tmp_x, tmp_y);
//
//          }

//#if 0
//                uint8_t TP_type;
//                uint16_t pdwSampleX, pdwSampleY;
//                uint8_t ucQueryResponse = 0;//ite7259_wait_command_done();
//                uint8_t tp_temp[10];
//
//                uint32_t tmp_x = 0;
//                uint32_t tmp_y = 0;
//#if 0
//   test_buf[test_index][0] = OS_GET_TICK_COUNT();
//#endif
//                TP_HRS_ReadBytes(&tp_temp[0],0x01);  //手势
//                TP_HRS_ReadBytes(&tp_temp[1],0x02);  // 触摸状态   1按下  0 抬起
//                TP_HRS_ReadBytes(&tp_temp[2],0x03);  //x 高位4
//                TP_HRS_ReadBytes(&tp_temp[3],0x04);  //x 地位
//                TP_HRS_ReadBytes(&tp_temp[4],0x05);  //y 高位4
//                TP_HRS_ReadBytes(&tp_temp[5],0x06);  //y 地位
//#if 0
//         test_buf[test_index][1] = OS_GET_TICK_COUNT();
//         test_index ++ ;
//         if(test_index == 100)
//           {
//              test_index =0 ;
//            }
//#endif
//                tmp_x = ((tp_temp[2] & 0x0f)<<8) | tp_temp[3];
//                tmp_y = ((tp_temp[4] & 0x0f)<<8) | tp_temp[5];
//   printf("\n tp_temp=%d,%d,%d,%d tick = %x \n",tp_temp[0],tp_temp[1],tmp_x,tmp_y,(unsigned int)OS_GET_TICK_COUNT());
//
//                if(tp_temp[1] == 0x01)
//                {
//                        UserTpA_Calculate(TP_STATE__PRESS, tmp_x, tmp_y);
//                }
//                else if(tp_temp[1] == 0x00)
//                {
//                        UserTpA_Calculate(TP_STATE__LIFT, tmp_x, tmp_y);
//                }
//
//#endif
//#if 0
//        uint8_t tp_temp[10];
//        uint32_t tmp_x = 0;
//        uint32_t tmp_y = 0;
//        test_buf[test_index][0] = OS_GET_TICK_COUNT();
//        TP_HRS_ReadBytes(&tp_temp[0],0x01);  //手势
//        TP_HRS_ReadBytes(&tp_temp[1],0x02);  // 触摸状态   1按下  0 抬起
//        TP_HRS_ReadBytes(&tp_temp[2],0x03);  //x 高位4
//        TP_HRS_ReadBytes(&tp_temp[3],0x04);  //x 地位
//        TP_HRS_ReadBytes(&tp_temp[4],0x05);  //y 高位4
//        TP_HRS_ReadBytes(&tp_temp[5],0x06);  //y 地位
//
//        test_buf[test_index][1] = OS_GET_TICK_COUNT();
//        test_index ++ ;
//        if(test_index == 100)
//        {
//                test_index =0 ;
//        }
//        tmp_x = ((tp_temp[2] & 0x0f)<<8) | tp_temp[3];
//        tmp_y = ((tp_temp[4] & 0x0f)<<8) | tp_temp[5];
//        if(tp_temp[1] == 0x01)
//        {
//                //UserTpA_Calculate(TP_STATE__PRESS, tmp_x, tmp_y);
//        }
//        else if(tp_temp[1] == 0x00)
//        {
//                //UserTpA_Calculate(TP_STATE__LIFT, tmp_x, tmp_y);
//        }
//        //LOG(LEVEL_INFO,"tp_temp=%x,%x,%x,%x\n",tp_temp[0],tp_temp[1],tp_temp[2],tp_temp[3]);
//        //printf("\n tp_temp=%d,%d,%d,%d tick = %x \n",tp_temp[0],tp_temp[1],tmp_x,tmp_y,(unsigned int)OS_GET_TICK_COUNT());
//
//        uint32_t now_tick_count = OS_GET_TICK_COUNT();
//
//        if(last_tick_count > now_tick_count)
//        {
//                if(last_tick_count - Interrupt_tick > now_tick_count)
//                {
//                        //两次中断触发超过1000ms,发送数据到上层
//                        middleware_Touch_Event();
//                }
//                last_tick_count = now_tick_count;
//        }
//        else
//        {
//
//                if(now_tick_count - Interrupt_tick > last_tick_count)
//                {
//                        //两次中断触发超过1000ms,发送数据到上层
//                        middleware_Touch_Event();
//                        last_tick_count = now_tick_count;
//                }
//        }
//#endif
}





#endif
