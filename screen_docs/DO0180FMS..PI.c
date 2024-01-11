//DO0180FMST07示例代码 
//SPI_3Wire 4lane（QSPI）

//驱动IC 寄存器初始配置
// **********************
//深圳达沃科技有限公司
// DWO LIMITED
// www.dwo.net.cn
// mail: alan@dwo.net.cn
// Tel: 86-755-8258 0810
// AMOLED design, driver board development, PCBA, provide one-stop service.
//************************

/*Recommended Operating Sequence */
/*STEP 1---[ Power On ] */
/*STEP 2---[ Activate Reset ] Driver IC */
/*STEP 3---[ Configure AMOLED Register] */ 	


//Power ON sequence
/*  STEP 1---[ Power On ]
（1）turn-ON VBAT  (+3.8VDC)
（2）turn-ON IOVCC  (+1.8VDC)	
（3）turn-ON VCI  (Controled by Power_IC_EN)

Wait 10ms
STEP 2---[ Activate Reset ] 
STEP 3---[ SPI_3Wire/QSPI Configure AMOLED Register] 
STEP 4---[ QSPI Send AMOLED Display Data]
*/

void AMOLED_Init(void) // www.dwo.net.cn
{	 	
SPI_CS=0;
	SPI_WriteComm(0x11); //Sleep Out
SPI_CS=1;

delay_ms(120);

SPI_CS=0;
	SPI_WriteComm(0x44); 
	SPI_WriteData(0x01);
	SPI_WriteData(0x66);
SPI_CS=1;

SPI_CS=0;
	SPI_WriteComm(0x35); //TE ON
	SPI_WriteData(0x00);
SPI_CS=1;

SPI_CS=0;
	SPI_WriteComm(0x3A); 
	SPI_WriteData(0x55); //Interface Pixel Format	16bit/pixel
	//SPI_WriteData(0x66); //Interface Pixel Format	18bit/pixel
	//SPI_WriteData(0x77); //Interface Pixel Format	24bit/pixel
SPI_CS=1;

SPI_CS=0;
	SPI_WriteComm(0x53); 
	SPI_WriteData(0x20);
SPI_CS=1;

delay_ms(10);

SPI_CS=0;
	SPI_WriteComm(0x51); //Write Display Brightness	MAX_VAL=0XFF
	SPI_WriteData(0x00);
SPI_CS=1;

delay_ms(10);

SPI_CS=0;
	SPI_WriteComm(0x29); //Display on  
SPI_CS=1;

delay_ms(10);

SPI_CS=0;
	SPI_WriteComm(0x51); //Write Display Brightness	MAX_VAL=0XFF
	SPI_WriteData(0xFF);
SPI_CS=1;
}  //end of AMOLED_Init


///*---------------------------------------------------------------------------
//此函数用于设置AMOLED“窗口”---GRAM
//---------------------------------------------------------------------------*/
void AMOLED_Block_Write(u16 Xstart,u16 Xend,u16 Ystart,u16 Yend)
{	

SPI_CS=0;
	SPI_WriteComm(0x2a); //Set Column Start Address   
	SPI_WriteData(Xstart>>8);
	SPI_WriteData(Xstart&0xff);
	SPI_WriteData(Xend>>8);
	SPI_WriteData(Xend&0xff);
SPI_CS=1;
	
SPI_CS=0;
	SPI_WriteComm(0x2b); //Set Row Start Address    
	SPI_WriteData(Ystart>>8);
	SPI_WriteData(Ystart&0xff);
	SPI_WriteData(Yend>>8);
	SPI_WriteData(Yend&0xff);
SPI_CS=1;

SPI_CS=0;	
	SPI_WriteComm(0x2c);//Memory Write
SPI_CS=1;	

}
}

//SPI写数据
//----------------------------------------------------------------------
void  SPI_1L_SendData(u16 dat)
{  
  unsigned char i;

   for(i=0; i<8; i++)			
   {   
      if( (dat&0x80)!=0 ) SPI_SDA = 1;
      else                SPI_SDA = 0;

		dat  <<= 1;

	  SPI_SCL = 0;//delay_us(2);
    SPI_SCL = 1;	
		 
   }
	 
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


void Write_Disp_Data(u16 val) //16BIT
{ 
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



void SPI_4W_DATA_4W_ADDR_START(void )
{
SPI_4wire_data_4wire_Addr(0x12,0x2c);
}
	
	
	
void SPI_4W_DATA_1W_ADDR_START(void )
{
SPI_4wire_data_1wire_Addr(0x32,0x2C);
}

void SPI_4W_DATA_1W_ADDR_END(void )
{	
SPI_CS=0;	
SPI_4wire_data_1wire_Addr(0x32,0x00);
SPI_CS=1;	
}

//清屏函数   
//全屏显示单色
void DM_Clear(u16 color)
{  	
	unsigned int i,j;  
	
	DM_block_write(0,COL-1,0,ROW-1);
	
	SPI_CS=0;
	SPI_4wire_data_1wire_Addr(0x32,0x2C);

	for(i=0;i<COL;i++)
		for(j=0;j<ROW;j++)
			{
			Write_Disp_Data(color);
			}
			
		SPI_CS=1;
			
SPI_4W_DATA_1W_ADDR_END();
}

