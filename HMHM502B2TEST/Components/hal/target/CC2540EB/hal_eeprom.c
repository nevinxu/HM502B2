 /*************************************************************/
 /*                     EEPROM驱动程序                        */
#include "osal.h"
#include "ioCC2540.h"
#include "Hal_drivers.h"
#include "hal_eeprom.h"


#define  _NOP(); asm("nop");
#define DELAY10NOP(); asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");\
                     asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
#define DELAY20NOP(); asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");

 /**********************************************************/
 /*                   内部函数声明                         */
 /**********************************************************/
void  I2C_TXBYTE(unsigned char num);  //I2C发送一个字节  
void  I2C_START(void);                 //I2C通讯启动标志
void  I2C_STOP(void);                  //I2C通讯结束标志
void  I2C_SETACK(void);                //输出ACK信号 
void  I2C_SETNCK(void);                //输出NACK信号
unsigned char  I2C_RXBYTE(void);       //I2C接收一个字节 
unsigned char  I2C_GETACK(void);       //获得ACK信号
unsigned char Write_SingleByte(unsigned char order,unsigned char addr,unsigned char value);//写入一个字节 
unsigned char Read_SingleByte(unsigned char order,unsigned char addr,unsigned char *value);//读出一个字节
unsigned char crc8(unsigned char arr[],unsigned char len);//crc



/******************************************************************************
*  function about I2C
******************************************************************************/
void I2C_SCK_HIGH(void)
{
   HAL_IO_SCL_PORT |= HAL_IO_SCL_BIT;        /*init io is high*/
}
void I2C_SCK_LOW(void)
{
  HAL_IO_SCL_PORT &=~ HAL_IO_SCL_BIT;        /*init io is low*/
}

void I2C_SCL_OUT(void)
{
  HAL_IO_SCL_DIR |= (HAL_IO_SCL_BIT);      /* Set pin direction to Output */
}
void I2C_SCL_IN(void)
{
  HAL_IO_SCL_DIR &= ~(HAL_IO_SCL_BIT);     /* Set pin direction to Input */
}

void I2C_SDA_HIGH(void)
{
   HAL_IO_SDA_PORT |= HAL_IO_SDA_BIT;        /*init io is high*/
}
void I2C_SDA_LOW(void)
{
  HAL_IO_SDA_PORT &=~ HAL_IO_SDA_BIT;        /*init io is low*/
}
void I2C_SDA_IN(void)
{
   HAL_IO_SDA_DIR &= ~(HAL_IO_SDA_BIT);     /* Set pin direction to Input */
}
void I2C_SDA_OUT(void)
{
   HAL_IO_SDA_DIR |= (HAL_IO_SDA_BIT);     /* Set pin direction to out */
}
uint8 I2C_GET_SDA(void)
{
  return (HAL_IO_SDA_PORT&HAL_IO_SDA_BIT);
}


void EEPROM_init()
{
  HAL_IO_SCL_SEL &= ~ (HAL_IO_SCL_BIT);    /* Set pin function to GPIO */
  HAL_IO_SCL_DIR |=   (HAL_IO_SCL_BIT);    /* Set pin direction to output */
  HAL_IO_SCL_P1INP |=  (HAL_IO_SCL_BIT);   /* set pullup-pulldown*/
  HAL_IO_SDA_SEL &= ~ (HAL_IO_SDA_BIT);    /* Set pin function to GPIO */
  HAL_IO_SDA_DIR |= (HAL_IO_SDA_BIT);      /* Set pin direction to outnput */
  HAL_IO_SDA_P1INP |= (HAL_IO_SDA_BIT);    /* set pullup-pulldown*/

  HAL_IO_SCL_PORT |= HAL_IO_SCL_BIT;       /*init io is high*/
  HAL_IO_SDA_PORT |= HAL_IO_SDA_BIT;
}

/**********************************************************
Function:  I2C_RXBYTE(void)
Description:  I2C接收一个字节  
Input:  none
output: none
Return: temp
**********************************************************/
unsigned char  I2C_RXBYTE(void)
{
     unsigned char ntemp=0;
     unsigned char i;
	 
     I2C_SDA_HIGH();
     I2C_SDA_IN();
     DELAY20NOP();
	 
     for(i=8;i>0;i--)
     {
       I2C_SCK_HIGH(); 
       if(I2C_GET_SDA())
       {ntemp|=(1<<(i-1));}
       I2C_SCK_LOW();
       DELAY20NOP();
     }
     return (ntemp);
}

/**********************************************************
Function:  I2C_TXBYTE(int DATA)
Description:  I2C发送一个字节  
Input:  DATA
output: none
Return: none
**********************************************************/
void  I2C_TXBYTE(unsigned char num)
{
     
    unsigned char i;
	
    I2C_SDA_OUT();
    DELAY20NOP();
	
    for(i=0;i<8;i++)
    {
      if(num&0x80)
      {
        I2C_SDA_HIGH();
      }
      else
      {
        I2C_SDA_LOW();
      }
      I2C_SCK_HIGH();
      DELAY20NOP();
        
      num<<=1;
      
      I2C_SCK_LOW();
      DELAY20NOP();
    }
}

/**********************************************************
Function:  I2C_START(void)
Description:  I2C通讯启动标志  
Input:  none
output: none
Return: none
**********************************************************/
void  I2C_START(void)
{
  I2C_SDA_OUT();
  I2C_SCL_OUT();
  DELAY20NOP();
  I2C_SDA_HIGH();
  DELAY20NOP();
  DELAY20NOP();
  I2C_SCK_HIGH();
  DELAY20NOP();
  DELAY20NOP();
  I2C_SDA_LOW();
  DELAY20NOP();
  DELAY20NOP();
  I2C_SCK_LOW();
  DELAY20NOP();
  DELAY20NOP();



}

/**********************************************************
Function:  I2C_STOP(void)
Description:  I2C通讯结束标志  
Input:  none
output: none
Return: none
**********************************************************/
void  I2C_STOP(void)
{  
  I2C_SDA_OUT();
  I2C_SCL_OUT();
  DELAY20NOP();
  I2C_SDA_LOW();
  DELAY20NOP();
  DELAY20NOP();
  I2C_SCK_HIGH();
  DELAY20NOP();
  I2C_SDA_HIGH(); 
  DELAY20NOP();
  DELAY20NOP();
  I2C_SCK_LOW();
  DELAY20NOP();
  DELAY20NOP();


 }

/**********************************************************
Function:  I2C_GETACK(void)
Description:  获得ACK信号 
Input:  none
output: none
Return: temp
**********************************************************/
unsigned char  I2C_GETACK(void)
{
  
  unsigned char ntemp=0;
  unsigned char lp=200;

  I2C_SDA_IN();

  I2C_SCK_HIGH();
 // DELAY20NOP();
  
  wait:
  ntemp=(I2C_GET_SDA());
  if((ntemp!=0)&&((lp--)!=0))
  {
    goto wait;
  }
  I2C_SCK_LOW();
   DELAY20NOP();
  return(ntemp);    
}


/**********************************************************
Function:  I2C_SETACK(void)
Description:  输出ACK信号  
Input:  none
output: none
Return: none
**********************************************************/
void  I2C_SETACK(void)
{
  I2C_SCK_LOW();
  DELAY20NOP();
      
  I2C_SDA_LOW();
  DELAY10NOP();
      
  I2C_SCK_HIGH();
  DELAY20NOP();
      
}
/**********************************************************
Function:  I2C_SETNCK(void)
Description:  输出NCK信号  
Input:  none
output: none
Return: none
**********************************************************/
void  I2C_SETNCK(void)
{ 
  I2C_SCK_LOW();
  DELAY20NOP();
  
  I2C_SDA_HIGH();
  DELAY10NOP();
  
  I2C_SCK_HIGH();
  DELAY20NOP();
  
  I2C_SCK_LOW();
  DELAY20NOP();
}

/**********************************************************
Function:   Write_SingleByte(unsigned char addrh,unsigned char addrl,unsigned char value)
Description:  写入EEROM一个字节  
Input:  addrh,addrl,value
output: none
Return: temp
**********************************************************/
unsigned char Write_SingleByte(unsigned char order,unsigned char addr,unsigned char value)
{
    unsigned char temp;
    //I2C_WP_LOW();          //关保护
   
     DELAY20NOP();
    I2C_START();           //启动数据总线
    
    I2C_TXBYTE(order);     //发送控制字节
    temp = I2C_GETACK();   //等待ACK
    if(temp/*&temp2*/)
    {
      return 0;
    }
    
    I2C_TXBYTE(addr);     //发送地址字节
    temp = I2C_GETACK();  //等待ACK
    if(temp /*& temp2*/)  
    {
      return 0;
    }
    
    
    I2C_TXBYTE(value);    //发送数据字节
    temp = I2C_GETACK();  //等待ACK
    if(temp /*& temp2*/) 
    {
      return 0;
    }
    
    I2C_STOP();          //发送停止信号
   
    return 1;
}

/**********************************************************
Function:   Read_SingleByte(char addr,char *value)
Description:  读出EEROM一个字节  
Input:  addrh,addrl,*value
output: none
Return: temp
**********************************************************/
unsigned char Read_SingleByte(unsigned char order,unsigned char addr,unsigned char *value)
{
    unsigned char temp,temp2;
    
    I2C_START();                //启动数据总线
    I2C_TXBYTE(order&0xfe);     //发送控制字节
    temp = I2C_GETACK();        //等待ACK
    temp2=I2C_GET_SDA();
    if(temp & temp2) 
    {
      return 0;
    }
    
    I2C_TXBYTE(addr);          //发送地址字节
    temp = I2C_GETACK();       //等待ACK
    temp2=I2C_GET_SDA();
    if(temp & temp2) 
    {
      return 0;
    }
        
    I2C_START();              //启动数据总线             
    I2C_TXBYTE(order);        //发送控制字节
    temp = I2C_GETACK();      //等待ACK
    temp2=I2C_GET_SDA();
    if(temp & temp2) 
    {
      return 0;
    }
   
    *value = I2C_RXBYTE();   //读取数据
   
    I2C_STOP();              //发送停止信号
    return 1;
}

/**********************************************************
Function:   crc8(unsigned char arr[],unsigned char len) 
Description:  CRC校验
Input:  arr[],len
output: none
Return: crc
**********************************************************/
unsigned char crc8(unsigned char arr[],unsigned char len)  //ptr为数据指针，len为数据长度
{
   unsigned char ci;
   unsigned char cj=0;
   unsigned char crc=0;
   while(len--)
   {
      for(ci=0x80; ci!=0; ci>>=1)
      {
        if((crc&0x80)!=0) 
        {
          crc<<=1; 
	  crc^=0x31;
	}
        else 
	{
          crc<<=1;
        }
        if((arr[cj]&ci)!=0) 
	{
          crc^=0x31;
        }
      }
      cj++;
   }
   return(crc);
}

unsigned char Write_SingleByte_L(unsigned char order,unsigned short addr,unsigned char value)
{
    unsigned char temp = addr;  //写命令 
    static unsigned char Haddr,Laddr;
    Laddr=addr&0x0ff;
    Haddr=(addr&(0xff00))/0x100;
   
    I2C_START();           //启动数据总线
    
    I2C_TXBYTE(order);     //发送控制字节
    temp = I2C_GETACK();   //等待ACK
    if(temp ) 
    {
      return 0;
    }
    
    I2C_TXBYTE(Haddr);     //发送地址字节
    temp = I2C_GETACK();  //等待ACK
    if(temp) 
    {
      return 0;
    }
    
    I2C_TXBYTE(Laddr);     //发送地址字节
    temp = I2C_GETACK();  //等待ACK
    if(temp ) 
    {
      return 0;
    }
    
    I2C_TXBYTE(value);    //发送数据字节
    temp = I2C_GETACK();  //等待ACK
    if(temp) 
    {
      return 0;
    }
    
    I2C_STOP();          //发送停止信号
      
    return 1;
  
}


uint8 Write_Page(uint16 page, uint8 Len, uint8* value)
{
   unsigned char temp = 0;  //写命令 
   unsigned char Haddr,Laddr;
   Haddr= page>>6;
   Laddr= page<<6;
   
  if(Len <= PAGESIZE)
  {

  restart:   I2C_START();           //启动数据总线
    
    I2C_TXBYTE(FM24LC64ADDR);     //发送控制字节
    temp = I2C_GETACK();   //等待ACK
    if(temp ) 
    {
      goto restart;
    }
    
    I2C_TXBYTE(Haddr);     //发送地址字节
    temp = I2C_GETACK();  //等待ACK
    if(temp) 
    {
      return 0;
    }
    
    I2C_TXBYTE(Laddr);     //发送地址字节
    temp = I2C_GETACK();  //等待ACK
    if(temp ) 
    {
      return 0;
    }
    while(Len--)
    {
      I2C_TXBYTE(*value);    //发送数据字节
      temp = I2C_GETACK();  //等待ACK
      if(temp) 
      {
        return 0;
      }
      value++;
    }
    
    I2C_STOP();          //发送停止信号
      

  }
      return 1;
}

uint8 Read_Page(uint16 page, uint8 Len, uint8* value)
{
    unsigned char temp = 0;  //写命令 
    unsigned char Haddr,Laddr;
    Laddr=page>>6;
    Haddr=page<<6;
    
restart3:    I2C_START();                //启动数据总线
    I2C_TXBYTE(FM24LC64ADDR);          //发送控制字节
    temp = I2C_GETACK();        //等待ACK
    if(temp ) 
    {
      goto restart3;
    }
	    
    I2C_TXBYTE(Haddr);          //发送地址字节
    temp = I2C_GETACK();       //等待ACK
    if(temp ) 
    {
      return 0;
    }
    
    I2C_TXBYTE(Laddr);          //发送地址字节
    temp = I2C_GETACK();       //等待ACK
    if(temp) 
    {
      return 0;
    }
		
restart4:    I2C_START();              //启动数据总线             
    I2C_TXBYTE(FM24LC64ADDR+1);        //发送控制字节
    temp = I2C_GETACK();      //等待ACK
    if(temp) 
    {
      goto restart4;
    }
    while(Len--)
    {
      *value = I2C_RXBYTE();   //读取数据
      if(Len > 0)
      {
        I2C_SETACK();
      }
      value++;
    }
   
    I2C_STOP();              //发送停止信号
    return 1;
}



unsigned char Read_SingleByte_L(unsigned char order,unsigned short addr,unsigned char *value)
{
    //unsigned char temp2;
    unsigned char temp = addr;  //写命令 
    unsigned char Haddr,Laddr;
    Laddr=addr&0x0ff;
    Haddr=(addr&(0xff00))/0x100;
    
restart2:    I2C_START();                //启动数据总线
    I2C_TXBYTE(order);          //发送控制字节
    temp = I2C_GETACK();        //等待ACK
    if(temp ) 
    {
      goto restart2;
    }
	    
    I2C_TXBYTE(Haddr);          //发送地址字节
    temp = I2C_GETACK();       //等待ACK
    if(temp ) 
    {
      return 0;
    }
    
    I2C_TXBYTE(Laddr);          //发送地址字节
    temp = I2C_GETACK();       //等待ACK
    if(temp) 
    {
      return 0;
    }
		
    I2C_START();              //启动数据总线             
    I2C_TXBYTE(order+1);        //发送控制字节
    temp = I2C_GETACK();      //等待ACK
    if(temp) 
    {
      return 0;
    }
   
    *value = I2C_RXBYTE();   //读取数据
   
    I2C_STOP();              //发送停止信号
    return 1;
}

uint8 User_SetI2C_Data(uint16 addr, uint8 Len, uint8* value)
{
	uint8 L, state, i;
	uint8* pchar;
        
	pchar = value;
	i = 0;
	for (L = 0; L < Len; L++)
	{
		state = Write_SingleByte_L(FM24LC64ADDR, addr, *pchar);
		if (state == 0)
		{
			i++;
			L--;
			if (i > 3)
				return state;
		}
		addr++;
		pchar++;
	}
	
	return state;
}
uint8 User_GetI2C_Data(uint16 addr, uint8 Len, uint8* value)
{
	uint8 L, state, i;
	uint8* pchar;
	
	pchar = value;
	
	i = 0;
	
	for (L = 0; L < Len; L++)
	{
		state = Read_SingleByte_L(FM24LC64ADDR, addr, pchar);
		if (state == 0)
		{
			i++;
			L--;
			if (i > 3)
				return state;
		}
		addr++;
		pchar++;

	}
	I2C_SDA_IN();//lance
	I2C_SCL_IN();
	return state;
}
