#ifndef _EEDFLASH_H_
#define _EEDFLASH_H_

  #include  "TypeDefinition.h"

  #define EEprom_Ptr      0x0E00  //数据地址标记位起始地址
  #define EEprom_Baseadrr 0x0E10  //变量存储的基地址
  #define EEprom_Length   16      //存储轮询长度
  #define Elem_Num 26             //定义存储变量地址的长度
  
  typedef struct 
  {
    uint8   pEErom_base;
    uint8   EE_Value;       //EEPROM储存正确性判断
    uint16  Charge_Times;    //存放在EEPROM中的时间
    uint32  BmsRunTime_min;  //存放在EEPROM中的时间  
    uint32  SOC_Counter_Time;  //存放在EEPROM中的时间
    
  }EEprom_Data_T;
  extern EEprom_Data_T EEprom_Data; 

  void Get_EEprom_Value(void);
  void EEprom_Init(void);
  void EEprom_read(uint16 , uint8);
  void Task_EEpromWrite(uint16); 

#endif
