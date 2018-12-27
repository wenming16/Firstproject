 /*=======================================================================
 *Subsystem:   裸机
 *File:        Task_DataCollection.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef  _TASK_DATA_COLLECTION_H_
#define  _TASK_DATA_COLLECTION_H_

  #include "TypeDefinition.h"
  #include "BattInfoConfig.h"
  
  //extern uint8 CSSU_BMS_Data[8];   
  /*
  //单体电压和温度结构体
  typedef  struct 
  {
    uint16 single_voltage[SysSeries];
    uint8 single_temperature[SysTempNum];
    uint8 Openwire_Status[SysModuleNum];  
  }Single_VoltTemp_T;
  extern Single_VoltTemp_T  Single_VoltTemp;
  */ 
  //接收内网数据结构体
  typedef  struct 
  {
    uint16  Max_VoltC;
    uint16  Min_VoltC;
    uint8   Max_VoltC_Node;
    uint8   Min_VoltC_Node;
    uint32  Total_VoltModule;
  }Module_VoltInfo_T;  //模组电压参数结构体
  extern Module_VoltInfo_T Module_VoltInfo[SYS_NUMBER_MODULE];

  typedef  struct 
  {
    uint8   Max_Temp;
    uint8   Min_Temp;
    uint8   Max_TempNode;
    uint8   Min_TempNode;
    uint16  Total_Temp;
    
    uint8   Ave_Temp;
    
  }Module_TempInfo_T;  //模组温度参数结构体
  extern Module_TempInfo_T Module_TempInfo[SYS_NUMBER_MODULE];

  typedef  struct 
  {
    uint8   CSSU_TempH; 
    uint8   OpenWire_Status;
    uint32  ModuleVolt_Total;           //采集的模组总压
    uint32  InsulVolt_Total;           //采集的模组总压
   
  }Module_StateInfo_T;  //模组状态信息
  extern Module_StateInfo_T Module_StateInfo[SYS_NUMBER_MODULE];

 
  void Task_DataProcess(void);

#endif
