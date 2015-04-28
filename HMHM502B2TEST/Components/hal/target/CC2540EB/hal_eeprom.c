 /*************************************************************/
 /*                     EEPROM��������                        */
#include "osal.h"
#include "ioCC2540.h"
#include "Hal_drivers.h"
#include "hal_eeprom.h"


#define  _NOP(); asm("nop");
#define DELAY10NOP(); asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");\
                     asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
#define DELAY20NOP(); asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");

 /**********************************************************/
 /*                   �ڲ���������                         */
 /**********************************************************/
void  I2C_TXBYTE(unsigned char num);  //I2C����һ���ֽ�  
void  I2C_START(void);                 //I2CͨѶ������־
void  I2C_STOP(void);                  //I2CͨѶ������־
void  I2C_SETACK(void);                //���ACK�ź� 
void  I2C_SETNCK(void);                //���NACK�ź�
unsigned char  I2C_RXBYTE(void);       //I2C����һ���ֽ� 
unsigned char  I2C_GETACK(void);       //���ACK�ź�
unsigned char Write_SingleByte(unsigned char order,unsigned char addr,unsigned char value);//д��һ���ֽ� 
unsigned char Read_SingleByte(unsigned char order,unsigned char addr,unsigned char *value);//����һ���ֽ�
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
Description:  I2C����һ���ֽ�  
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
Description:  I2C����һ���ֽ�  
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
Description:  I2CͨѶ������־  
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
Description:  I2CͨѶ������־  
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
Description:  ���ACK�ź� 
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
Description:  ���ACK�ź�  
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
Description:  ���NCK�ź�  
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
Description:  д��EEROMһ���ֽ�  
Input:  addrh,addrl,value
output: none
Return: temp
**********************************************************/
unsigned char Write_SingleByte(unsigned char order,unsigned char addr,unsigned char value)
{
    unsigned char temp;
    //I2C_WP_LOW();          //�ر���
   
     DELAY20NOP();
    I2C_START();           //������������
    
    I2C_TXBYTE(order);     //���Ϳ����ֽ�
    temp = I2C_GETACK();   //�ȴ�ACK
    if(temp/*&temp2*/)
    {
      return 0;
    }
    
    I2C_TXBYTE(addr);     //���͵�ַ�ֽ�
    temp = I2C_GETACK();  //�ȴ�ACK
    if(temp /*& temp2*/)  
    {
      return 0;
    }
    
    
    I2C_TXBYTE(value);    //���������ֽ�
    temp = I2C_GETACK();  //�ȴ�ACK
    if(temp /*& temp2*/) 
    {
      return 0;
    }
    
    I2C_STOP();          //����ֹͣ�ź�
   
    return 1;
}

/**********************************************************
Function:   Read_SingleByte(char addr,char *value)
Description:  ����EEROMһ���ֽ�  
Input:  addrh,addrl,*value
output: none
Return: temp
**********************************************************/
unsigned char Read_SingleByte(unsigned char order,unsigned char addr,unsigned char *value)
{
    unsigned char temp,temp2;
    
    I2C_START();                //������������
    I2C_TXBYTE(order&0xfe);     //���Ϳ����ֽ�
    temp = I2C_GETACK();        //�ȴ�ACK
    temp2=I2C_GET_SDA();
    if(temp & temp2) 
    {
      return 0;
    }
    
    I2C_TXBYTE(addr);          //���͵�ַ�ֽ�
    temp = I2C_GETACK();       //�ȴ�ACK
    temp2=I2C_GET_SDA();
    if(temp & temp2) 
    {
      return 0;
    }
        
    I2C_START();              //������������             
    I2C_TXBYTE(order);        //���Ϳ����ֽ�
    temp = I2C_GETACK();      //�ȴ�ACK
    temp2=I2C_GET_SDA();
    if(temp & temp2) 
    {
      return 0;
    }
   
    *value = I2C_RXBYTE();   //��ȡ����
   
    I2C_STOP();              //����ֹͣ�ź�
    return 1;
}

/**********************************************************
Function:   crc8(unsigned char arr[],unsigned char len) 
Description:  CRCУ��
Input:  arr[],len
output: none
Return: crc
**********************************************************/
unsigned char crc8(unsigned char arr[],unsigned char len)  //ptrΪ����ָ�룬lenΪ���ݳ���
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
    unsigned char temp = addr;  //д���� 
    static unsigned char Haddr,Laddr;
    Laddr=addr&0x0ff;
    Haddr=(addr&(0xff00))/0x100;
   
    I2C_START();           //������������
    
    I2C_TXBYTE(order);     //���Ϳ����ֽ�
    temp = I2C_GETACK();   //�ȴ�ACK
    if(temp ) 
    {
      return 0;
    }
    
    I2C_TXBYTE(Haddr);     //���͵�ַ�ֽ�
    temp = I2C_GETACK();  //�ȴ�ACK
    if(temp) 
    {
      return 0;
    }
    
    I2C_TXBYTE(Laddr);     //���͵�ַ�ֽ�
    temp = I2C_GETACK();  //�ȴ�ACK
    if(temp ) 
    {
      return 0;
    }
    
    I2C_TXBYTE(value);    //���������ֽ�
    temp = I2C_GETACK();  //�ȴ�ACK
    if(temp) 
    {
      return 0;
    }
    
    I2C_STOP();          //����ֹͣ�ź�
      
    return 1;
  
}


uint8 Write_Page(uint16 page, uint8 Len, uint8* value)
{
   unsigned char temp = 0;  //д���� 
   unsigned char Haddr,Laddr;
   Haddr= page>>6;
   Laddr= page<<6;
   
  if(Len <= PAGESIZE)
  {

  restart:   I2C_START();           //������������
    
    I2C_TXBYTE(FM24LC64ADDR);     //���Ϳ����ֽ�
    temp = I2C_GETACK();   //�ȴ�ACK
    if(temp ) 
    {
      goto restart;
    }
    
    I2C_TXBYTE(Haddr);     //���͵�ַ�ֽ�
    temp = I2C_GETACK();  //�ȴ�ACK
    if(temp) 
    {
      return 0;
    }
    
    I2C_TXBYTE(Laddr);     //���͵�ַ�ֽ�
    temp = I2C_GETACK();  //�ȴ�ACK
    if(temp ) 
    {
      return 0;
    }
    while(Len--)
    {
      I2C_TXBYTE(*value);    //���������ֽ�
      temp = I2C_GETACK();  //�ȴ�ACK
      if(temp) 
      {
        return 0;
      }
      value++;
    }
    
    I2C_STOP();          //����ֹͣ�ź�
      

  }
      return 1;
}

uint8 Read_Page(uint16 page, uint8 Len, uint8* value)
{
    unsigned char temp = 0;  //д���� 
    unsigned char Haddr,Laddr;
    Laddr=page>>6;
    Haddr=page<<6;
    
restart3:    I2C_START();                //������������
    I2C_TXBYTE(FM24LC64ADDR);          //���Ϳ����ֽ�
    temp = I2C_GETACK();        //�ȴ�ACK
    if(temp ) 
    {
      goto restart3;
    }
	    
    I2C_TXBYTE(Haddr);          //���͵�ַ�ֽ�
    temp = I2C_GETACK();       //�ȴ�ACK
    if(temp ) 
    {
      return 0;
    }
    
    I2C_TXBYTE(Laddr);          //���͵�ַ�ֽ�
    temp = I2C_GETACK();       //�ȴ�ACK
    if(temp) 
    {
      return 0;
    }
		
restart4:    I2C_START();              //������������             
    I2C_TXBYTE(FM24LC64ADDR+1);        //���Ϳ����ֽ�
    temp = I2C_GETACK();      //�ȴ�ACK
    if(temp) 
    {
      goto restart4;
    }
    while(Len--)
    {
      *value = I2C_RXBYTE();   //��ȡ����
      if(Len > 0)
      {
        I2C_SETACK();
      }
      value++;
    }
   
    I2C_STOP();              //����ֹͣ�ź�
    return 1;
}



unsigned char Read_SingleByte_L(unsigned char order,unsigned short addr,unsigned char *value)
{
    //unsigned char temp2;
    unsigned char temp = addr;  //д���� 
    unsigned char Haddr,Laddr;
    Laddr=addr&0x0ff;
    Haddr=(addr&(0xff00))/0x100;
    
restart2:    I2C_START();                //������������
    I2C_TXBYTE(order);          //���Ϳ����ֽ�
    temp = I2C_GETACK();        //�ȴ�ACK
    if(temp ) 
    {
      goto restart2;
    }
	    
    I2C_TXBYTE(Haddr);          //���͵�ַ�ֽ�
    temp = I2C_GETACK();       //�ȴ�ACK
    if(temp ) 
    {
      return 0;
    }
    
    I2C_TXBYTE(Laddr);          //���͵�ַ�ֽ�
    temp = I2C_GETACK();       //�ȴ�ACK
    if(temp) 
    {
      return 0;
    }
		
    I2C_START();              //������������             
    I2C_TXBYTE(order+1);        //���Ϳ����ֽ�
    temp = I2C_GETACK();      //�ȴ�ACK
    if(temp) 
    {
      return 0;
    }
   
    *value = I2C_RXBYTE();   //��ȡ����
   
    I2C_STOP();              //����ֹͣ�ź�
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
