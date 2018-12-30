/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_UpMonitor.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_UP_MONITOR_H_
#define _TASK_UP_MONITOR_H_  

  #include  "TypeDefinition.h"  
  #include  "CAN.h"
  /********BMS to Up Monitor information CSSU*******/

  #define    BMS_Send_Information1   (0x18FF9700+NUM_pack)
  #define    BMS_Send_Information2   (0x18FF9710+NUM_pack)
  #define    BMS_Send_Information3   (0x18FF9700+NUM_pack)
  #define    BMS_Send_Information4   (0x18FF9810+NUM_pack)
  #define    BMS_Send_Information5   (0x18FF9900+NUM_pack)
  #define    BMS_Send_Information6   (0x19FF9900+NUM_pack)



 //上位机数据标定
 //电池阀值标定
  //#define    Monitor_BMS_Volt   0x1810C0F4     //电池电压阈值信息        
  typedef struct 
  { //00
    uint16  Volt_Cell_High1;  //单体电压过压一级阀值， 单位 0.0001V/Bit
    uint16  Volt_Cell_High2;  //单体电压过压二级阀值， 单位 0.0001V/Bit
    //uint16  Volt_Cell_High3;  //单体电压过压三级阀值， 单位 0.0001V/Bit
    //01
    uint16  Volt_Cell_Low1; //单体电压欠压一级阀值， 单位 0.0001V/Bit
    uint16  Volt_Cell_Low2; //单体电压欠压二级阀值， 单位 0.0001V/Bit
    //uint16  Volt_Cell_Low3; //单体电压欠压三级阀值， 单位 0.0001V/Bit
    //02
    uint16  Volt_Sys_High1;  //电池组过压阀值1级
    uint16  Volt_Sys_High2;  //电池组过压阀值2级
    //uint16  Volt_Sys_High3;  //电池组过压阀值3级
    //03
    uint16  Volt_Sys_Low1; //电池组欠压阀值1级
    uint16  Volt_Sys_Low2; //电池组欠压阀值2级
    //uint16  Volt_Sys_Low3; //电池组欠压阀值3级
    //04
    uint16  Volt_Cell_Diff1;  //单体压差一级阀值， 单位 0.0001V/Bit
    //uint16  Volt_Cell_Diff1;  //单体压差二级阀值， 单位 0.0001V/Bit
    //uint16  Volt_Cell_Diff1;  //单体压差三级阀值， 单位 0.0001V/Bit
    //05
    //uint16  System_Diff1_Volt;  //系统压差一级阀值， 单位 0.1V/Bit
    //uint16  System_Diff2_Volt;  //系统压差二级阀值， 单位 0.1V/Bit
    //uint16  System_Diff3_Volt;  //系统压差三级阀值， 单位 0.1V/Bit
  }BMSMonitor_Volt_T;
  extern BMSMonitor_Volt_T g_BMSMonitor_Volt;
  
  //#define    Monitor_BMS_Temp   0x1811C0F4     //温度阈值信息 
  typedef struct 
  { //00
    uint8  Temp_Charge_High1;   //单体充电温度过温一级阀值， 单位 1℃/Bit  偏移量：-40
    uint8  Temp_Charge_High2;   //单体充电温度过温二级阀值， 单位 1℃/Bit  偏移量：-40
    //uint8  Temp_Charge_High3;   //单体充电温度过温三级阀值， 单位 1℃/Bit  偏移量：-40
    
    uint8  Temp_Charge_Low1;  //单体充电温度低温一级阀值， 单位 1℃/Bit  偏移量：-40
    uint8  Temp_Charge_Low2;  //单体充电温度低温二级阀值， 单位 1℃/Bit  偏移量：-40
    //uint8  Temp_Charge_Low3;  //单体充电温度低温三级阀值， 单位 1℃/Bit  偏移量：-40
    //01
    uint8  Temp_DisCharge_High1;    //单体放电温度过温一级阀值， 单位 1℃/Bit   偏移量：-40
    uint8  Temp_DisCharge_High2;    //单体放电温度过温二级阀值， 单位 1℃/Bit   偏移量：-40
    //uint8  Temp_DisCharge_High3;    //单体放电温度过温三级阀值， 单位 1℃/Bit   偏移量：-40
    
    uint8  Temp_DisCharge_Low1;   //单体放电温度低温一级阀值， 单位 1℃/Bit   偏移量：-40
    uint8  Temp_DisCharge_Low2;   //单体放电温度低温二级阀值， 单位 1℃/Bit   偏移量：-40
    //uint8  Temp_DisCharge_Low3;   //单体放电温度低温三级阀值， 单位 1℃/Bit   偏移量：-40
    //02
    uint8  Temp_Charge_Diff1;     //单体充电温差一级阀值， 单位 1℃/Bit    偏移量：-40
    //uint8  Charge_Diff2_Temp;     //单体充电温差二级阀值， 单位 1℃/Bit    偏移量：-40
    //uint8  Charge_Diff3_Temp;     //单体充电温差三级阀值， 单位 1℃/Bit    偏移量：-40
    
    uint8  Temp_DisCharge_Diff1;  //单体放电温差一级阀值， 单位 1℃/Bit    偏移量：-40
    //uint8  DisCharge_Diff2_Temp;  //单体放电温差二级阀值， 单位 1℃/Bit    偏移量：-40
    //uint8  DisCharge_Diff3_Temp;  //单体放电温差三级阀值， 单位 1℃/Bit    偏移量：-40
   
   /* //03
    uint8  ChargeGun_Over1_Temp; //充电枪高温一级阀值， 单位 1℃/Bit    偏移量：-40
    uint8  ChargeGun_Over2_Temp; //充电枪高温二级阀值， 单位 1℃/Bit    偏移量：-40
    uint8  ChargeGun_Over3_Temp; //充电枪高温三级阀值， 单位 1℃/Bit    偏移量：-40
    
    uint8  Chip_Over1_Temp; //BMS芯片高温一级阀值， 单位 1℃/Bit    偏移量：-40
    uint8  Chip_Over2_Temp; //BMS芯片高温二级阀值， 单位 1℃/Bit    偏移量：-40
    uint8  Chip_Over3_Temp; //BMS芯片高温三级阀值， 单位 1℃/Bit    偏移量：-40
    */
  }BMSMonitor_Temp_T;
  extern BMSMonitor_Temp_T g_BMSMonitor_Temp;
  
 
  //#define    Monitor_BMS_Current   0x1812C0F4     //电流阈值及SOC阈值信息 
  typedef struct 
  { //00
    uint16  Current_DisCharge_High1;  //放电过流阈值1级，0.1A/位     偏移量:-750
    uint16  Current_DisCharge_High2;  //放电过流阈值2级，0.1A/位     偏移量:-750
    //uint16  Current_DisCharge_High3;  //放电过流阈值3级，0.1A/位     偏移量:-750
    //01
    uint16  Current_Charge_High1;     //充电过流阈值1级，0.1A/位      偏移量:-750
    uint16  Current_Charge_High2;     //充电过流阈值2级，0.1A/位      偏移量:-750
    //uint16  Current_Charge_High3;     //充电过流阈值3级，0.1A/位      偏移量:-750
    /*//02
    uint8   SOC_Low1;   //低SOC1级     1%/位
    uint8   SOC_Low2;   //低SOC2级
    uint8   SOC_Low3;   //低SOC3级
    
    uint8   SOC_High1;  //高SOC1级
    uint8   SOC_High2;  //高SOC2级
    uint8   SOC_High3;  //高SOC3级
    */
  }BMSMonitor_Curr_T;
  extern BMSMonitor_Curr_T g_BMSMonitor_Curr;
  
  //#define    Monitor_BMS_Insulation   0x1813C0F4     //绝缘故障阈值信息 
  typedef struct 
  { //00
    uint16 Insulation_Resis1;     // 绝缘电阻一级阀值， 单位 0.1KΩ/Bit;
    uint16 Insulation_Resis2;     // 绝缘电阻二级阀值， 单位 0.1KΩ/Bit;
    uint16 Insulation_Resis3;     // 绝缘电阻三级阀值， 单位 0.1KΩ/Bit;  
  
  }BMSMonitor_Insu_T;
  extern BMSMonitor_Insu_T g_BMSMonitor_Insul; 
  
  //发送至上位机的SOC初始化/查表标记
  typedef struct 
  { 
    uint8   SOC_t; //放在发送至上位机的头文件中  
  
  }BMSMonitor_SOC_T;
  extern BMSMonitor_SOC_T g_BMSMonitor_SOC; 
  
  //发送至上位机的SOH值
  typedef struct 
  { 
    uint8   SOH; //放在发送至上位机的头文件中  
  
  }BMSMonitor_SOH_T;
  extern BMSMonitor_SOH_T g_BMSMonitor_SOH; 
  
  typedef struct
  {
     uint16 Voll_Sys_Low1_LT;
     uint16 Voll_Sys_Low2_LT;
     uint16  Volt_Cell_Low1_LT; //单体电压欠压一级阀值， 单位 0.0001V/Bit
     uint16  Volt_Cell_Low2_LT; //单体电压欠压二级阀值， 单位 0.0001V/Bit
  
  }BMSMonitor_New_LT_T;
  extern BMSMonitor_New_LT_T g_BMSMonitor_New_LT;

//---------------------------------------------------------------------------------------------------- 
  
  //#define    Monitor_BMS_SOC   0x1915F4C0         //特殊类信息标定说明
  typedef struct 
  { //00
    uint16  SOC_Init;             //SOC初始值， 单位 1%/Bit；
    union
    {
      
     /* uint8  Relay1_Station;       //继电器开关变量
      struct 
      {
        uint8 Relay_Positive:2;    //主正继电器
        uint8 Relay_Negtive:2;     //主负继电器
        uint8 Relay_Precharge:2;   //预充继电器
        uint8 Relay_Charge:2;      //充电继电器
        
      }Monitor_BMS_DelayBit; */
      
     int  Relay1_Station;       //继电器开关变量
      struct 
      {
        int Relay_Positive:2;    //主正继电器
        int Relay_Negtive:2;     //主负继电器
        int Relay_Precharge:2;   //预充继电器
        int Relay_Charge:2;      //充电继电器
        
      }Monitor_BMS_DelayBit;
    }Monitor_BMS_Delay_T;
    
    uint16 Ref_Volt;               //参考电压标定(0~65536)
    uint16 Ref_Current;            //电流传感器的斜率
    //01
    uint16 Volt_Resis;             //电压检测中分压电阻的标定
    uint32 Pack_Total_Capacity;    //标定SOH中的总容量(2000*额定容量)
    //02
    uint16 BMS_Running_Time;       //BMS运行时长
      
  }MonitorBMS_SOC_T;
  extern MonitorBMS_SOC_T g_MonitorBMS_SOC;
  
   
  //#define    Monitor_to_BMSFF   0x19FFF4C0
  typedef struct 
  {
    uint8 Communication;   //BMS与上位机通信信号，0xAA： 通信开始； 0x00： 不进行通信；
    uint8 Bootloader_Time;      //数据更新 1~5； （1 代表 500ms）  
    uint8 CellMessageSend_judge;   //电池类信息发送判断标记      
    uint16 None2;
    uint16 None3;
  }MonitorBMS_Start_T;
  extern MonitorBMS_Start_T g_MonitorBMS_Start; 
 
  uint8 CAN_ToUpMonitor(pCANFRAME);
  uint8 CAN_UpMonitorInit(void);
  void Task_BMSToUpMonitor(void);
  void Task_BMUToUpMonitor(void);
  
  void UpMonitor_to_Bms(pCANFRAME);

#endif