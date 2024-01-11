    	COL=368; //	COL---DM横向像素(列数)
		ROW=448; // ROW----DM纵向像素(行数)	
		
		
/*---------------------------------------------------------------------------
此函数用于设置DM“窗口”---GRAM
---------------------------------------------------------------------------*/
void DM_block_write(u16 Xstart,u16 Xend,u16 Ystart,u16 Yend)
{	
	
SPI_CS=0;
	SPI_WriteComm(0x2a);   
	SPI_WriteData(Xstart>>8);
	SPI_WriteData(Xstart&0xff);
	SPI_WriteData(Xend>>8);
	SPI_WriteData(Xend&0xff);
SPI_CS=1;
	
SPI_CS=0;
	SPI_WriteComm(0x2b);   
	SPI_WriteData(Ystart>>8);
	SPI_WriteData(Ystart&0xff);
	SPI_WriteData(Yend>>8);
	SPI_WriteData(Yend&0xff);
SPI_CS=1;

SPI_CS=0;	
	SPI_WriteComm(0x2c);
SPI_CS=1;	

}



void SPI_WriteComm(u16 regval)
{ 
		SPI_1L_SendData(0x02);
		SPI_1L_SendData(0x00);
		SPI_1L_SendData(regval);
		SPI_1L_SendData(0x00);//delay_us(2);
}

void SPI_ReadComm(u16 regval)
{    
		SPI_1L_SendData(0x03);//
		SPI_1L_SendData(0x00);
		SPI_1L_SendData(regval);
		SPI_1L_SendData(0x00);//PAM
}


void SPI_WriteComm_QSPI(u16 regval)
{ 

		SPI_1L_SendData(0x32);//32  
		SPI_1L_SendData(0x00);
		SPI_1L_SendData(regval);
		SPI_1L_SendData(0x00);//delay_us(2);
}

void SPI_WriteData(u16 val)
{   
 unsigned char n;	
   for(n=0; n<8; n++)			
   {  
	if(val&0x80) SPI_SDA=1;/*SPI_SDA 为写进去的值*/
	else         SPI_SDA=0;
	 
	val<<= 1;

	SPI_SCL=0;//delay_us(2);
	SPI_SCL=1;
   }


}		


void Write_Disp_Data(u32 val)
{ 
	#ifdef Disp_Data_24bits
	  QSPI_WriteData(val>>16);
	#endif

	QSPI_WriteData(val>>8);
	QSPI_WriteData(val);
}

void QSPI_WriteData(u16 val)
{
		//SPI_CS=0; 			
		if(val&0x80) SPI_IO3=1;
		else  			 SPI_IO3=0;

		if(val&0x40) SPI_IO2=1;
		else  			 SPI_IO2=0;

		if(val&0x20) SPI_IO1=1;
		else   			 SPI_IO1=0;

		if(val&0x10) SPI_IO0=1;
		else  			 SPI_IO0=0;

		SPI_SCL=0;//delay_us(2);
		SPI_SCL=1;//delay_us(2);


		if(val&0x08) SPI_IO3=1;else SPI_IO3=0;

		if(val&0x04) SPI_IO2=1;else SPI_IO2=0;

		if(val&0x02) SPI_IO1=1;else SPI_IO1=0;

		if(val&0x01) SPI_IO0=1;else SPI_IO0=0;

		SPI_SCL=0;//delay_us(2);
		SPI_SCL=1;//delay_us(1);
}


void SPI_4wire_data_1wire_Addr(u16 First_Byte,u16 Addr)
{   
	SPI_1L_SendData(First_Byte);//
	SPI_1L_SendData(0x00);
	SPI_1L_SendData(Addr);
	SPI_1L_SendData(0x00);//PA
}


void SPI_4wire_data_4wire_Addr(u16 First_Byte,u16 Addr)
{
	SPI_1L_SendData(First_Byte);//
	QSPI_WriteData(0x00);
	QSPI_WriteData(Addr);
	QSPI_WriteData(0x00);//PA
}


void SPI_4W_DATA_4W_ADDR_START(void)
{
   SPI_4wire_data_4wire_Addr(0x12,0x2c);
}
		
	
void SPI_4W_DATA_1W_ADDR_START(void)
{
   SPI_4wire_data_1wire_Addr(0x32,0x2C);
}

void SPI_4W_DATA_1W_ADDR_END(void)
{	
	SPI_CS=0;	
	SPI_4wire_data_1wire_Addr(0x32,0x00);
	SPI_CS=1;	
}


//清屏函数   
//全屏显示单色
void DM_Clear(u32 color)
{  	
	unsigned int i,j;  
	
	DM_block_write(0,COL-1,0,ROW-1);
	
	SPI_CS=0;
	if(QSPI_DT==SPI_4W_DATA_1W_ADD){SPI_4wire_data_1wire_Addr(0x32,0x2C);}   
	if(QSPI_DT==SPI_4W_DATA_4W_ADD){SPI_4wire_data_4wire_Addr(0x12,0x2C);}//SH8601Z	

	for(i=0;i<COL;i++)
		for(j=0;j<ROW;j++)
			{
			Write_Disp_Data(color);
			}
			
		SPI_CS=1;
			
		if(QSPI_DT==SPI_4W_DATA_1W_ADD){SPI_4W_DATA_1W_ADDR_END();}
}


//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void DM_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{ 
  unsigned int i,j; 
	
	DM_block_write(xsta,xend,ysta,yend);

	SPI_CS=0;

	if(QSPI_DT==SPI_4W_DATA_1W_ADD){SPI_4wire_data_1wire_Addr(0x32,0x2C);}   
	if(QSPI_DT==SPI_4W_DATA_4W_ADD){SPI_4wire_data_4wire_Addr(0x12,0x2C);}//SH8601Z	   	

	for(i=xsta;i<xend;i++)
	for(j=ysta;j<yend;j++)
	Write_Disp_Data(color); 

	SPI_CS=1;
	
  if(QSPI_DT==SPI_4W_DATA_1W_ADD){SPI_4W_DATA_1W_ADDR_END();}
} 



void DM_DrawPoint(u16 x,u16 y,u16 frontcolor)//画点-----PEN_COLOR:此点的颜色
{
	if(x>=COL)
	   x=COL-1;
  if(y>=ROW)
     y=ROW-1;
	
	DM_block_write(x,x,y,y);
			
	SPI_CS=0;
	if(QSPI_DT==SPI_4W_DATA_1W_ADD){SPI_4wire_data_1wire_Addr(0x32,0x2C);}   
	if(QSPI_DT==SPI_4W_DATA_4W_ADD){SPI_4wire_data_4wire_Addr(0x12,0x2C);}//SH8601Z	
	
	Write_Disp_Data(frontcolor);	
							
	SPI_CS=1;
  if(QSPI_DT==SPI_4W_DATA_1W_ADD){SPI_4W_DATA_1W_ADDR_END();}
}  

void DM_Drawbigpoint(u16 x,u16 y)//画一个大点(2*2的点)
{
  DM_DrawPoint(x,y,PEN_COLOR);//中心点 
	DM_DrawPoint(x+1,y,PEN_COLOR);
	DM_DrawPoint(x,y+1,PEN_COLOR);
	DM_DrawPoint(x+1,y+1,PEN_COLOR);	
  	
}


void DM_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)//画线---x1,y1:起点坐标---x2,y2:终点坐标  
{
  unsigned int x, y, t;
	
	if((x1==x2)&&(y1==y2))DM_DrawPoint(x1, y1,PEN_COLOR);
	else if(abs(y2-y1)>abs(x2-x1))//斜率大于1 
	{
		if(y1>y2) 
		{
			t=y1;
			y1=y2;
			y2=t; 
			t=x1;
			x1=x2;
			x2=t; 
		}
		for(y=y1;y<y2;y++)//以y轴为基准 
		{
			x=(unsigned long)(y-y1)*(x2-x1)/(y2-y1)+x1;
			DM_DrawPoint(x, y,PEN_COLOR);  
		}
	}
	else     //斜率小于等于1 
	{
		if(x1>x2)
			{
				t=y1;
				y1=y2;
				y2=t;
				t=x1;
				x1=x2;
				x2=t;
			}   
		for(x=x1;x<=x2;x++)//以x轴为基准 
		{
			y =(unsigned long)(x-x1)*(y2-y1)/(x2-x1)+y1;
			DM_DrawPoint(x,y,PEN_COLOR); 
		}
	} 
}


//在指定位置显示一个字符
//x:0~COL-6
//y:0~ROW-12
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void DM_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{   
  u8 temp,t1,t;
	u8 pos;    
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	for(t=0;t<csize;t++)
	{   
			if     (size==12)temp=DM_asc_1206[num][t];//调用1206字体
			else if(size==16)temp=DM_asc_1608[num][t];//调用1608字体
			else if(size==24)temp=DM_asc_2412[num][t];//调用2412字体
            else if(size==32)temp=DM_asc_3216[num][t];//调用3216字体		
			else if(size==48)temp=DM_asc_4824[num][t];//调用4824字体
		    else return;								//没有的字库
		
		for(t1=0;t1<8;t1++)
		{			    
			if     (temp&0x80) DM_DrawPoint(x,y,PEN_COLOR);
			else if(mode==0)   DM_DrawPoint(x,y,PAPER_COLOR);
			temp<<=1;
			y++;
			if(y>=(ROW-1))return;		//超区域了
			if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=(COL-1))return;	//超区域了
					break;
				}
		}  	 
	}  

}

//显示2个数字
//x,y:起点坐标
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~99);
void DM_ShowNum(u16 x,u16 y,u16 num,u16 size,u16 mode)
{         							   
    DM_ShowChar(x,y,(num/10)%10+'0',size,mode); 
    DM_ShowChar(x+size/2,y,num%10+'0',size,mode); 
}

//显示4个数字
//x,y:起点坐标
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~9999);
void DM_Show4Num(u16 x,u16 y,u16 num,u16 size,u16 mode)
{ 
  
DM_ShowChar(x,y,(num/1000)+'0',size,mode);
DM_ShowChar(x+size/2,y,(num/100)%10+'0',size,mode);      							   
DM_ShowChar(x+size,y,(num/10)%10+'0',size,mode); 
DM_ShowChar(x+size+size/2,y,num%10+'0',size,mode); 
}



//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//str  :字符串
//size :字体大小 默认为8*16
//mode:0,非叠加方式;1,叠加方式    		   
void DM_Disp_Str(u16 x,u16 y,u16 size,u8*str)
{
		unsigned char bHz=0;     //字符或者中文  	 
		//unsigned char size=16;
		unsigned char mode=1;
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
                if(x>(COL-size/2))//换行
				{				   
					y+=size;
					x=0;	   
				}							    
		        if(y>(ROW-size))break;//越界返回      
		        if(*str==13)//换行符号
		        {         
		            y+=size;
					x=0;
		            str++; 
		        }  
		        else DM_ShowChar(x,y,*str,size,mode);//有效部分写入 
				str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }					 
    }  
		
		
} 


	PEN_COLOR=RED;
	DM_Clear(BLACK);//显示单色画面

	PEN_COLOR=RED;
	DM_Disp_Str(120,60,24,"DWO");	  
	PEN_COLOR=WHITE;    
	DM_Disp_Str(0,90,48,"1.8 inch AMOLED");	
	PEN_COLOR=GREEN;	
	DM_Disp_Str(120,136,32,"368x448");		
	PEN_COLOR=BLUE;
	DM_Disp_Str(120,180,48,"QSPI");
	PEN_COLOR=GBLUE;
	DM_Disp_Str(10,240,48,"www.dwo.net.cn");
