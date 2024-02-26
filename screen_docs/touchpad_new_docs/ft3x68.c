#include "ft3x68.h"
//#include "TP.h"
//#include "CTP_IIC.h"
//#include "usart.h"
//#include "delay.h" 
//#include "string.h" 
//#include "DM.h"
//#include "Focaltech_FW_Download.h"
 

const u8 FT_Size_Buffer[]={"12345678"}; 

#define FT_Size sizeof(FT_Size_Buffer)	 
	
//向FT3X68写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
u8 FT3X68_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(FT3X68_CMD_WR);	//发送写地址 
	CT_IIC_Wait_Ack(); 
	
	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    CT_IIC_Send_Byte(buf[i]);  	//发数据
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//产生一个停止条件	    
	return ret; 
}
//从FT3X68读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void FT3X68_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(FT3X68_CMD_WR);   	//发送写命令 	 
	CT_IIC_Wait_Ack(); 	 	
	
 	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(FT3X68_CMD_RD);   	//发送读命令		   
	CT_IIC_Wait_Ack();	 
  
	for(i=0;i<len;i++)
	{	   
  buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    CT_IIC_Stop();//产生一个停止条件     
} 

int fts_read(uint8_t addr, uint8_t *data, uint16_t len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(FT3X68_CMD_WR);   	//发送写命令 	 
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(addr&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(FT3X68_CMD_RD);   	//发送读命令		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
  data[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    CT_IIC_Stop();//产生一个停止条件     
} 



//初始化FT3X68触摸屏
//返回值:0,初始化成功;1,初始化失败 
u8 FT3X68_Init(void)
{
	CT_IIC_Init();      	//初始化电容屏的I2C总线  
	
	FT3X68_RST=1;				//复位
	delay_ms(1);	
	FT3X68_RST=0;				//复位
	delay_ms(20);
 	FT3X68_RST=1;				//释放复位		    
	delay_ms(50);  	

	return 1;
}

const u16 FT3X68_TPX_TBL[5]={FT3X68_TP1_REG,FT3X68_TP2_REG,FT3X68_TP3_REG,FT3X68_TP4_REG,FT3X68_TP5_REG};

//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u16 FT3X68_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u16 res=0;
	u8 temp,temp0;
	static u8 t=0;//控制查询间隔,从而降低CPU占用率   
	t++;
	
	if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
	{
		FT3X68_RD_Reg(FT3X68_TD_STATUS,&mode,1);//读取触摸点的状态  
		//printf("mode: %2x\r\n",((mode>>4)&0X0F));//mode);
        //printf("mode:0x%04x\r\n",mode);
		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);//将点的个数转换为1的位数,匹配tp_dev.sta定义 
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//触摸有效?
				{
					FT3X68_RD_Reg(FT3X68_TPX_TBL[i],buf,4);	//读取XY坐标值 
						
					tp_dev.x[i]=COL- (((u16)(buf[0]&0X0F)<<8)+buf[1])-1;  //触摸点 X坐标
					tp_dev.y[i]=ROW-(((u16)(buf[2]&0X0F)<<8)+buf[3])-1;   ////触摸点 Y坐标

                    //printf("y[%d]:%d\r\n",i,tp_dev.y[i]);
						
					if((buf[0]&0XF0)!=0X80)tp_dev.x[i]=tp_dev.y[i]=0;//必须是contact事件，才认为有效
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//读到的数据都是0,则忽略此次数据
			//if(tp_dev.y[0]==0xfff ||(tp_dev.y[0] & 0xfff)== 0xfff ){tp_dev.y[0]=0;mode=0;}	//读到的数据都是0,则忽略此次数据	
			t=0;		//触发一次,则会最少连续监测10次,从而提高命中率
		}
	}
	if((mode&0X1F)==0)//无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
		}
		else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 	
	if(t>240)t=10;//重新从10开始计数
	return res;
}
 
u16 Get_FT3X68_ID(void)
{
	u8 temp[2]; 
	FT3X68_RD_Reg(ID_G_CIPHER_LOW,&temp[0],1);  
	//printf("FT3X68 vendor ID:0x%x\r\n",(u16)temp[0]);

	return temp[0];			
}














