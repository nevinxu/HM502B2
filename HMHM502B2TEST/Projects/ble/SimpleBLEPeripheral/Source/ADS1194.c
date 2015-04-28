/******************************************* Copyright (c) **********************************************
**             Zhejiang Helowin Medical technology  Co.,LTD.
**                   http://www.hellowin.cn
**Filename:    ADS1194.c
**Revised:    $Date: 2012-10-18 $
**Revision:    $Revision: V1.0.0 $
**Description:  This file contains the ADS1194 application.
********************************************************************************************************/

/**************************************************************************************************
 *                      INCLUDES
 **************************************************************************************************/
#include "ads1194.h"
#include "BLE_SEND.h"




/**************************************************************************************************
 *                                       FUNCTIONS - API
 **************************************************************************************************/



/**************************************************************************************************
 *                                       LOCAL FUNCTIONS 
 **************************************************************************************************/
static void ADS1194_WriteReg(uint8, uint8);
static void ADS1194_WriteCommand(uint8);
static uint8 ADS1194_ReadReg(uint8);
static uint8 ADS1194_ReadSingleLeadData(uint8);
static void ADS1194_ChipEnable ( void );
static void ADS1194_ChipDisable ( void );
static void ADS1194_DataReadyPoll ( void );
static void ADS1194_Init_StartUp( void );
static void ADS1194_Init_TestSignal( void );
static void ADS1194_Init_ShortInput( void );
//static void ADS1194_Init_Normal( void );
static void ADS1194_Init_Normal( unsigned char Rate,unsigned char  gain);

/********************************************************************************************************
* Function Name : ADS1194_WriteReg
* Description :
* Input     :
* Output     : 
* Return     :
********************************************************************************************************/
static void ADS1194_WriteReg(uint8 address, uint8 data)
{
  HalEcg_CSEnable();                              // CSʹ��
  HalEcg_WriteByte(SDATAC);                       // ֹͣRDATACģʽ
  HalEcg_WriteByte(WREG | address & 0x1F);        // �Ĵ�����ַ
  HalEcg_WriteByte(0x00);                         // д1���ֽ�
  HalEcg_WriteByte(data);                         // д����
  HalEcg_CSDisable();                             // 
}

/********************************************************************************************************
* Function Name : ADS1194_WriteCommand
* Description :
* Input     :
* Output     : 
* Return     :
********************************************************************************************************/
static void ADS1194_WriteCommand(uint8 command)
{
  HalEcg_CSEnable();                              // CSʹ��
  HalEcg_WriteByte(command);                      // д����
  //HalEcg_CSDisable();                           // 
}


/********************************************************************************************************
* Function Name : ADS1194_ReadReg
* Description :
* Input     :
* Output     : 
* Return     :
********************************************************************************************************/
static uint8 ADS1194_ReadReg(uint8 address)
{
  uint8 read;
  HalEcg_CSEnable();                             // CSʹ��
  HalEcg_WriteByte(SDATAC);                      // ֹͣRDATACģʽ
  HalEcg_WriteByte(RREG | address & 0x1F);       // �Ĵ�����ַ
  HalEcg_WriteByte(0x00);                        // ��1���ֽ�
  HalEcg_ReadByte(&read, 0xFF);                  // ������
  //HalEcg_CSDisable();                            //
  return read;
}

/********************************************************************************************************
* Function Name : ADS1194_ReadDataContinuous
* Description :
* Input     :
* Output     : 
* Return     :
********************************************************************************************************/

uint8 ADS1194_ReadDataContinuous(uint8 *ptr)
{
  ADS1194_ChipEnable();
  ADS1194_WriteCommand(SDATAC);
  ADS1194_WriteCommand(RDATAC);
  ADS1194_SoftStart();

  ADS1194_ChipDisable(); 
  return 1;    
}

/********************************************************************************************************
* Function Name : ADS1194_ReadSingleData
* Description :
* Input     :
* Output     : 
* Return     :
********************************************************************************************************/
void ADS1194_ReadSingleData(uint8 *pLoff, uint8 *pLeadI, uint8 *pLeadII, uint8 *pLeadC1)
{
  uint8 regdata[11];
  uint8 index;
  ADS1194_ChipEnable();
  ADS1194_WriteCommand(SDATAC);
  ADS1194_WriteCommand(RDATA);
  ADS1194_WriteCommand(START);
  ADS1194_DataReadyPoll();

  for(index=0; index<11; index++)                                 // 88Bit(24+4*16)����11��Byte������
  {
    HalEcg_ReadByte(&regdata[index], 0xFF);
  }
  *pLoff       = ((regdata[1]&0xf0)|((regdata[2]&0xf0)>>4));
  *pLeadI      = regdata[4];                                      //16bit������ֻȡ��8λ��Ч���ݣ�
  *pLeadII     = regdata[6];                                      //16bit������ֻȡ��8λ��Ч���ݣ�
  *pLeadC1     = regdata[8];                                      //16bit������ֻȡ��8λ��Ч���ݣ�
}

/********************************************************************************************************
* Function Name : ADS1194_ReadSingleLeadData
* Description :
* Input     :
* Output     : 
* Return     :
********************************************************************************************************/
static uint8 ADS1194_ReadSingleLeadData(uint8 leadparam)
{
  uint8 regdata[11];
  uint8 index;
  uint8 leaddata;
  ADS1194_ChipEnable();
  ADS1194_WriteCommand(SDATAC);   //ֹͣ�����ɼ�ģʽ
  ADS1194_WriteCommand(START);
  ADS1194_WriteCommand(RDATA);
  ADS1194_DataReadyPoll();

  for(index=0; index<11; index++)                                 // 88Bit(24+4*16)����11��Byte������
  {
    HalEcg_ReadByte(&regdata[index], 0xFF);
  }
  
  switch(leadparam)
  {
    case LOFF_DATA:
      leaddata = ((regdata[1]&0xf0)|((regdata[2]&0xf0)>>4));
      break;
      
    case LEADI_DATA:
      leaddata = regdata[4];           //16bit������ֻȡ��8λ��Ч���ݣ�
      break;  
      
    case LEADII_DATA:
      leaddata = regdata[6];           //16bit������ֻȡ��8λ��Ч���ݣ�
      break; 
      
    case LEADIII_DATA:
      leaddata = regdata[8];           //16bit������ֻȡ��8λ��Ч���ݣ�
      break; 

    default:  
      leaddata = 0xFF;                //Ĭ������Ϊ0xFF��
      break; 
  }
  
  return leaddata;
}


/********************************************************************************************************
* Function Name : ADS1194_ReadData
* Description :
* Input     :
* Output     : 
* Return     :
********************************************************************************************************/
void ADS1194_ReadData(uint8 *ptr)
{
  uint8 regdata[11];
  uint8 index;
  ADS1194_ChipEnable();
  ADS1194_WriteCommand(SDATAC);
  ADS1194_WriteCommand(START);
  ADS1194_WriteCommand(RDATA);
  ADS1194_DataReadyPoll();

  for(index=0; index<11; index++)             // 88Bit(24+4*16)����11��Byte������
  {
    HalEcg_ReadByte(&regdata[index], 0xFF);
    *(ptr+index) = regdata[index];
  }
  //ADS1194_ChipDisable(); 
}

/********************************************************************************************************
* Function Name : ADS1194_WriteData
* Description :
* Input     :
* Output     : 
* Return     :
********************************************************************************************************/
void ADS1194_WriteData(uint8 address, uint8 data)
{
  
}

/**********************************************************************************************************
*ADS1194����
**********************************************************************************************************/

//ADS1194����
void ADS1194_WakeUp( void )
{
  ADS1194_WriteCommand(WAKEUP);
}

//ADS1194����
void ADS1194_Sleep( void )
{
  ADS1194_WriteCommand(STANDBY);
}

//ADS1194�����λ����
void ADS1194_SoftReset( void )
{
  ADS1194_WriteCommand(RESET);
}

//ADS1194�����ʼADCת��
void ADS1194_SoftStart( void )
{ 
  ADS1194_WriteCommand(START);
}

//ADS1194���ֹͣADCת��
void ADS1194_SoftStop( void )
{
  ADS1194_WriteCommand(STOP);
}

//ADS1194��ʼ����������ģʽ
void ADS1194_StartReadDataContinuous( void )
{
  ADS1194_ChipEnable();
  ADS1194_WriteCommand(SDATAC);
  ADS1194_SoftStart();
  ADS1194_WriteCommand(RDATAC);
  // ADS1194_DataReadyPoll();     //ͨ���ж϶�ȡECG���ݣ�����Ҫ��ѯECG_DRDY��ƽ
}

//ADS1194ֹͣ����������ģʽ
void ADS1194_StopReadDataContinuous( void )
{
  ADS1194_WriteCommand(SDATAC);
  ADS1194_ChipDisable();
}

//��ADS1194�豸ID
uint8 ADS1194_ID( void )
{   
  return(ADS1194_ReadReg(ID));
}

//дADS1194 GPIO��
void ADS1194_WriteGPIO(uint8 num, uint8 val)
{
  uint8 i=0;

  i=ADS1194_ReadReg(GPIO);
  ADS1194_WriteReg(GPIO,i&(~(1<<num)));
  if ( val>0 ) 
  {
    val=i|(1<<(num+4));
  } 
  else 
  {
    val=i&(~(1<<(num+4)));
  }
  ADS1194_WriteReg(GPIO,val);
}

//ADS1194��GPIO�Ĵ���ֵ
uint8 ADS1194_ReadGPIO(uint8 num)
{
  uint8 i=0;

  i=ADS1194_ReadReg(GPIO);
  ADS1194_WriteReg(GPIO,i|(1<<num));
  i=ADS1194_ReadReg(GPIO);
  i&=(1<<num);
  i>>=num;
  return(i);
}

//


/**********************************************************************************************************
* Device SPI Chip Select Manipulation                                                                 *
**********************************************************************************************************/
static void ADS1194_ChipEnable ( void )    // ADS1194 module uses GPIO as the SPI CS
{
  HalEcg_CSEnable(); 
}

static void ADS1194_ChipDisable ( void )   // ADS1194 uses GPIO for SPI CS
{
  HalEcg_CSDisable(); 
}

/**********************************************************************************************************
* ADS1194 Functions                                                                        *
**********************************************************************************************************/
static void ADS1194_DataReadyPoll ( void )
{
  while(HalEcg_DrdyStatus());      // ADS1194�ȴ�DRDY�źű�ͣ��ɼ�����ת����ɣ����Զ�ȡ����
 // HalEcg_WaitMs(5);
}

/**********************************************************************************************************
                ADS1194 Functions                                                                        *
**********************************************************************************************************/
void ADS1194_PowerDown ( void )
{
  HalEcg_PowerDisable();
}

void ADS1194_PowerUp ( void )
{   
  HalEcg_PowerEnable();
}
// ADS1194Ĭ�ϳ�ʼ������:��������ģʽ
static void ADS1194_Init_PowerDown()
{
  HalEcg_CSEnable();                // CSʹ��
  HalEcg_WriteByte(SDATAC);         // ֹͣRDATACģʽ
  HalEcg_WriteByte(WREG | CONFIG1); // ��ʼ�Ĵ�����ַ��CONFIG1
  HalEcg_WriteByte(0x10);           // ����д17���Ĵ���
  HalEcg_WriteByte(0x00);           // CONFIG1  
  
  HalEcg_WriteByte(0x00);           // CONFIG2      
  HalEcg_WriteByte(0x00);           // CONFIG3      
  HalEcg_WriteByte(0x0);           // LOFF//  

  HalEcg_WriteByte(0x81);           // CH1SET      
  HalEcg_WriteByte(0x81);           // CH2SET      
  HalEcg_WriteByte(0x81);           // CH3SET      
  
  HalEcg_WriteByte(0x81);           // CH4SET      
  HalEcg_WriteByte(0x81);           // CH5SET      
  HalEcg_WriteByte(0x81);           // CH6SET      
  HalEcg_WriteByte(0x81);           // CH7SET      
  HalEcg_WriteByte(0x81);           // CH8SET      
  HalEcg_WriteByte(0x00);           // RLD_SENSP    
  HalEcg_WriteByte(0x00);           // RLD_SENSN    
  HalEcg_WriteByte(0x00);           // LOFF_SENSP  
  HalEcg_WriteByte(0x00);           // LOFF_SENSN  
  HalEcg_WriteByte(0x00);           // LOFF_FLIP    

  HalEcg_CSDisable(); 
  HalEcg_WaitUs(5);
  HalEcg_CSEnable(); 
  HalEcg_WriteByte(WREG | GPIO);    // ��ʼ�Ĵ�����ַ:GPIO
  HalEcg_WriteByte(0x05);           // ����д6���Ĵ���
  HalEcg_WriteByte(0x00);           // GPIO        
  HalEcg_WriteByte(0x00);           // PACE        
  HalEcg_WriteByte(0x00);           // RESERVED        
  HalEcg_WriteByte(0x00);           // CONFIG4      
  HalEcg_WriteByte(0x00);           // WCT1        
  HalEcg_WriteByte(0x00);           // WCT2
  HalEcg_CSDisable();  
}

/**********************************************************************************************************
                ADS1194 Initialization
**********************************************************************************************************/
// ADS1194��ʼ��
void ADS1194_Init( unsigned char RateMode,unsigned  char  gain)
{
  HalEcgInit();
  ADS1194_Init_StartUp();
  Reset_ADS1194_Mode(RateMode,gain);
}

// ADS1194��ʼ������
static void ADS1194_Init_StartUp( void )
{  
  ADS1194_PowerUp();                  // �ϵ���������AVDD��Դ
  ADS1194_ChipEnable();               // CSʹ��
  ADS1194_SoftReset();                // �����λ
}

// ADS1194Ĭ�ϳ�ʼ������:2Hz���������ź�
static void ADS1194_Init_TestSignal( void )
{
  ADS1194_ChipEnable();             // CSʹ��
  ADS1194_WriteReg(CONFIG2,0x30);   // CONFIG2
  ADS1194_WriteReg(CH1SET,0x05);    // CH1SET
  ADS1194_WriteReg(CH2SET,0x05);    // CH2SET
  ADS1194_WriteReg(CH3SET,0x05);    // CH3SET
  ADS1194_ChipDisable(); 
}

// ADS1194Ĭ�ϳ�ʼ������:����ͨ���̽�
static void ADS1194_Init_ShortInput( void )
{
  ADS1194_ChipEnable();             // CSʹ��
  ADS1194_WriteReg(CH1SET,0x01);    // CH1SET
  ADS1194_WriteReg(CH2SET,0x01);    // CH2SET
  ADS1194_WriteReg(CH3SET,0x01);    // CH3SET
  ADS1194_ChipDisable(); 
}


// ADS1194Ĭ�ϳ�ʼ������:��������ģʽ
static void ADS1194_Init_Normal( unsigned char Rate,unsigned char  gain)
{
  HalEcg_CSEnable();                // CSʹ��
  HalEcg_WriteByte(SDATAC);         // ֹͣRDATACģʽ
  HalEcg_WriteByte(WREG | CONFIG1); // ��ʼ�Ĵ�����ַ��CONFIG1
  HalEcg_WriteByte(0x10);           // ����д17���Ĵ���
  HalEcg_WriteByte(0x40|Rate);           // CONFIG1  
  
  HalEcg_WriteByte(0x30);           // CONFIG2      
  HalEcg_WriteByte(0xCC);           // CONFIG3      
  HalEcg_WriteByte(0xE3);           // LOFF//  

  HalEcg_WriteByte(gain);           // CH1SET      
  HalEcg_WriteByte(gain);           // CH2SET      
  HalEcg_WriteByte(gain);           // CH3SET      
  
  HalEcg_WriteByte(0x81);           // CH4SET      
  HalEcg_WriteByte(0x81);           // CH5SET      
  HalEcg_WriteByte(0x81);           // CH6SET      
  HalEcg_WriteByte(0x81);           // CH7SET      
  HalEcg_WriteByte(0x81);           // CH8SET      
  HalEcg_WriteByte(0x03);           // RLD_SENSP    
  HalEcg_WriteByte(0x03);           // RLD_SENSN    
  HalEcg_WriteByte(0x07);           // LOFF_SENSP  
  HalEcg_WriteByte(0x07);           // LOFF_SENSN  
  HalEcg_WriteByte(0x00);           // LOFF_FLIP    

  HalEcg_CSDisable(); 
  HalEcg_WaitUs(5);
  HalEcg_CSEnable(); 
  HalEcg_WriteByte(WREG | GPIO);    // ��ʼ�Ĵ�����ַ:GPIO
  HalEcg_WriteByte(0x05);           // ����д6���Ĵ���
  HalEcg_WriteByte(0x0F);           // GPIO        
  HalEcg_WriteByte(0x00);           // PACE        
  HalEcg_WriteByte(0x00);           // RESERVED        
  HalEcg_WriteByte(0x02);           // CONFIG4      
  HalEcg_WriteByte(0x09);           // WCT1        
  HalEcg_WriteByte(0xC2);           // WCT2
  HalEcg_CSDisable();  
}



void  Reset_ADS1194_Mode(unsigned char RateMode,unsigned  char  gain)
{
  ADS1194_StopReadDataContinuous();
  ADS1194_SoftReset();                // �����λ
  
  if(gain == 5)
  {
    gain = 0x60;
  }
  else if(gain == 0x01)  //
  {
    gain = 0x20;
  }
  else if(gain ==  0x02)
  {
    gain = 0x0;
  }
  else if(gain == 0x04)
  {
    gain = 0x40;
  }
  
  if(RateMode == 1)    //125
  {
  ADS1194_Init_Normal(0x06,gain);
  }
  else if (RateMode == 2)
  {
      ADS1194_Init_Normal(0x05,gain);   //250
  }
  else if (RateMode == 3)      //500
  {
      ADS1194_Init_Normal(0x04,gain);
  }
  else if (RateMode == 4)     //1k
  {
      ADS1194_Init_Normal(0x03,gain);
  }
  else if (RateMode == 5)   //2k
  {
      ADS1194_Init_Normal(0x02,gain);
  }
  ADS1194_StartReadDataContinuous();
}
/**********************************************************************************************************
                ADS1194 Calibration
**********************************************************************************************************/
void ADS1194_Calibration(uint8* caldatum)
{ 
//  uint8 cal_0mv[11],cal_1mv[11];
  uint8 LeadII[100]={0}; 
  uint16 LeadData = 0;
  uint8 i=0;
  //0mVУ׼
  ADS1194_Init_ShortInput();
  HalEcg_WaitMs(5);
  for(i=0; i<100; i++);
  {
    LeadII[i] = ADS1194_ReadSingleLeadData(LEADII_DATA);
  }

  for(i=0; i<100; i++);
  {
    LeadData += LeadII[i];
  }
  *caldatum = LeadData/100;

 // ADS1194_ReadData(cal_0mv);
//  *caldatum = cal_0mv[4];             //
  
  //1mVУ׼
 // ADS1194_Init_TestSignal();
 // ADS1194_ReadData(LEADII_DATA);
//  *caloffset = cal_1mv[4];            //
}

/*****************************************  http://www.hellowin.cn  ************************************/
