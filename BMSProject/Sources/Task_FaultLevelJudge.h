/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_FaultLevelJudge.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_FAULTLEVELJUDGE_H_
#define _TASK_FAULTLEVELJUDGE_H_

/*======================================================================
                             故障判断结构体
========================================================================*/
  //==========================充电故障判断============================
  typedef struct
  {
    //-------------------故障等级：按照设定要求-----------------------
    //单体电压
    uint8 Level_Volt_Cell_High;               
    uint8 Level_Volt_Cell_Low; 
    uint8 Level_Volt_Cell_Diff_High;
    //系统电压    
    uint8 Level_Volt_Sys_High; 
    
    //温度     
    uint8 Level_Temp_High;    
    uint8 Level_Temp_Low;     
    uint8 Level_Temp_Diff_High;
    
    //充电电流   
    uint8 Level_Current_Charge_High; 
 
    uint8 Level_Charge_SwitchOff_flag;
    
    uint8 Level_Charge_BalanceOff_Flag;
          
  }Fault_Charge_T;
  extern Fault_Charge_T Fault_Charge;             //充电故障判断
  
  //==========================放电故障判断===========================
  typedef struct
  {
    //-------------------故障等级：按照设定要求-----------------------
    //单体电压
    uint8 Level_Volt_Cell_Low; 
    uint8 Level_Volt_Cell_Diff_High;
    //系统电压    
    uint8 Level_Volt_Sys_Low;
    
    //温度     
    uint8 Level_Temp_High;    
    uint8 Level_Temp_Low;     
    uint8 Level_Temp_Diff_High;
    
    //放电电流   
    uint8 Level_Current_DisCharge_High; 
    
    uint8 Level_DisCharge_SwitchOff_flag;
    
    uint8 Level_DisCharge_BalanceOff_Flag;
    
  }Fault_DisCharge_T;
  extern Fault_DisCharge_T Fault_DisCharge;       //放电故障判断
  
  //==============================掉线信号==============================
  typedef struct
  {
    //---------------------故障状态：0正常；1故障----------------------
    uint8 VCU;     
    uint8 HVU;
    
    uint8 CSSU1;
    #if(SYS_NUMBER_MODULE>=2)
      uint8 CSSU2;
    #endif
    #if(SYS_NUMBER_MODULE>=3)
      uint8 CSSU3;
    #endif
    #if(SYS_NUMBER_MODULE>=4)
      uint8 CSSU4;
    #endif
    #if(SYS_NUMBER_MODULE>=5)
      uint8 CSSU5;
    #endif
    #if(SYS_NUMBER_MODULE>=6)
      uint8 CSSU6;
    #endif
          
  }State_Offline_T;
  extern State_Offline_T State_Offline;             //充电故障判断 
  
  //==============================心跳信号==============================
  typedef struct
  {
    //CSSU
    uint8 HeartBeat_CSSU1;
    #if(SYS_NUMBER_MODULE>=2)
      uint8 HeartBeat_CSSU2;
    #endif
    #if(SYS_NUMBER_MODULE>=3)
      uint8 HeartBeat_CSSU3;
    #endif
    #if(SYS_NUMBER_MODULE>=4)
      uint8 HeartBeat_CSSU4;
    #endif
    #if(SYS_NUMBER_MODULE>=5)
      uint8 HeartBeat_CSSU5;
    #endif
    #if(SYS_NUMBER_MODULE>=6)
      uint8 HeartBeat_CSSU6;
    #endif
    //VCU
    uint8 HeartBeat_VCU;
    //HVU
    uint8 HeartBeat_HVU;
  }HeartBeat_T;
  extern HeartBeat_T HeartBeat;              //心跳信号
  
  //=========================放电故障阈值================================
  typedef struct
  {
    //放电总压低
    uint32 F2_DisChg_VoltSL_NT;
    uint32 F1_DisChg_VoltSL_NT;
    
    uint32 F2_DisChg_VoltSL_LT;
    uint32 F1_DisChg_VoltSL_LT;
    //放电单体低
    uint16 F2_DisChg_VoltCL_NT;
    uint16 F1_DisChg_VoltCL_NT;
    
    uint16 F2_DisChg_VoltCL_LT;
    uint16 F1_DisChg_VoltCL_LT;
    //放电单体压差
    uint16 F1_DisChg_VoltCD;
    //放电高温
    uint8  F2_DisChg_TempH;
    uint8  F1_DisChg_TempH;
    //放电低温
    uint8  F2_DisChg_TempL;
    uint8  F1_DisChg_TempL;
    //放电温差
    uint8  F1_DisChg_TempD;
    //放电过流
    float  F2_DisChg_CurrH;
    float  F1_DisChg_CurrH;
    //绝缘故障
    uint16 F2_DisChg_Insul;
    
  }FltThreshold_DisChg_T;
  extern  FltThreshold_DisChg_T FltThreshold_DisChg;

  //=========================充电故障阈值================================  
  typedef struct
  {
    //充电总压高
    uint32 F2_Charge_VoltSL;
    uint32 F1_Charge_VoltSL;
    
    //充电单体高
    uint16 F2_Charge_VoltCL;
    uint16 F1_Charge_VoltCL;
    
    //充电单体压差
    uint16 F1_Charge_VoltCD;
    //充电高温
    uint8  F2_Charge_TempH;
    uint8  F1_Charge_TempH;
    //充电低温
    uint8  F2_Charge_TempL;
    uint8  F1_Charge_TempL;
    //充电温差
    uint8  F1_Charge_TempD;
    //充电过流
    float  F2_Charge_CurrH;
    float  F1_Charge_CurrH;
    //绝缘故障
    uint16 F2_Charge_Insul;
    
  }FltThreshold_Charge_T;
  extern  FltThreshold_Charge_T FltThreshold_Charge;
/*-======================================================================
                              函数声明
=========================================================================*/
 void Task_FaultLevelJudge(uint8 workstate);

#endif