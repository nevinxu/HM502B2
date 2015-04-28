#ifndef I2_C_H
#define I2_C_H


/*---SCL at P1.3---*/
#define HAL_IO_SCL_PORT   P1
#define HAL_IO_SCL_BIT    BV(3)
#define HAL_IO_SCL_SEL    P1SEL
#define HAL_IO_SCL_DIR    P1DIR 
#define HAL_IO_SCL_P1INP  P1INP //pullup / pulldown
/*---SDA at P1.2---*/
#define HAL_IO_SDA_PORT   P1
#define HAL_IO_SDA_BIT    BV(2)
#define HAL_IO_SDA_SEL    P1SEL
#define HAL_IO_SDA_DIR    P1DIR 
#define HAL_IO_SDA_P1INP  P1INP //pullup / pulldown

/*I2C addr*/
#define FM24LC64ADDR  0xA0
#define PAGESIZE        64

extern void  I2C_TXBYTE(unsigned char DATA);  //I2C发送一个字节  
extern void  I2C_START(void);                 //I2C通讯启动标志
extern void  I2C_STOP(void);                  //I2C通讯结束标志
extern void  I2C_SDA_HIGH(void);              //SDA脚输出高电平
extern void  I2C_SDA_LOW(void);               //SDA脚输出低电平
extern void  I2C_SCK_HIGH(void);              //SCK脚输出高电平 
extern void  I2C_SCK_LOW(void);               //SCK脚输出低电平

extern void EEPROM_init();
extern unsigned char  I2C_GETACK(void);       //获得ACK信号
extern unsigned char  I2C_RXBYTE(void);       //I2C接收一个字节 
extern unsigned char Write_SingleByte(unsigned char order,unsigned char addr,unsigned char value);//写入一个字节 
extern unsigned char Read_SingleByte(unsigned char order,unsigned char addr,unsigned char *value);//读出一个字节
extern unsigned char crc8(unsigned char arr[],unsigned char len);//crc
extern unsigned char Read_SingleByte_L(unsigned char order,unsigned short addr,unsigned char *value);
extern unsigned char Write_SingleByte_L(unsigned char order,unsigned short addr,unsigned char value);
extern uint8 User_SetI2C_Data(uint16 addr, uint8 Len, uint8* value);
extern uint8 User_GetI2C_Data(uint16 addr, uint8 Len, uint8* value);
extern uint8 Write_Page(uint16 page, uint8 Len, uint8* value);
extern uint8 Read_Page(uint16 page, uint8 Len, uint8* value);



#endif
