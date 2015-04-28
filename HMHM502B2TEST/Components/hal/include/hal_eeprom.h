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

extern void  I2C_TXBYTE(unsigned char DATA);  //I2C����һ���ֽ�  
extern void  I2C_START(void);                 //I2CͨѶ������־
extern void  I2C_STOP(void);                  //I2CͨѶ������־
extern void  I2C_SDA_HIGH(void);              //SDA������ߵ�ƽ
extern void  I2C_SDA_LOW(void);               //SDA������͵�ƽ
extern void  I2C_SCK_HIGH(void);              //SCK������ߵ�ƽ 
extern void  I2C_SCK_LOW(void);               //SCK������͵�ƽ

extern void EEPROM_init();
extern unsigned char  I2C_GETACK(void);       //���ACK�ź�
extern unsigned char  I2C_RXBYTE(void);       //I2C����һ���ֽ� 
extern unsigned char Write_SingleByte(unsigned char order,unsigned char addr,unsigned char value);//д��һ���ֽ� 
extern unsigned char Read_SingleByte(unsigned char order,unsigned char addr,unsigned char *value);//����һ���ֽ�
extern unsigned char crc8(unsigned char arr[],unsigned char len);//crc
extern unsigned char Read_SingleByte_L(unsigned char order,unsigned short addr,unsigned char *value);
extern unsigned char Write_SingleByte_L(unsigned char order,unsigned short addr,unsigned char value);
extern uint8 User_SetI2C_Data(uint16 addr, uint8 Len, uint8* value);
extern uint8 User_GetI2C_Data(uint16 addr, uint8 Len, uint8* value);
extern uint8 Write_Page(uint16 page, uint8 Len, uint8* value);
extern uint8 Read_Page(uint16 page, uint8 Len, uint8* value);



#endif
