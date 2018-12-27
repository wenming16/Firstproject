#ifndef SPI_H_
#define SPI_H_

  #include  "MC9S12XEP100.h"
  #include  "TypeDefinition.h"

  //SPI1通道的片选IO口宏定义
  #define  SS2                PTP_PTP3
  #define  SS2_Dir            DDRP_DDRP3

  //SPI1物理层初始化
  uint8  SPI1_Init(void);
  void  SPI1_Write(uint8 data); 
  uint8 SPI1_read(void);

  //SPI1控制的器件唤醒与休眠
  void wakeup_idle(void);
  void wakeup_sleep(void);
  //延时函数
  void delay_time(uint16 t);
  
#endif