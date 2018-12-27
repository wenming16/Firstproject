 /*=======================================================================
 *Subsystem:   裸机
 *File:        Task_SOC.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef  _TASK_SOC_H_
#define  _TASK_SOC_H_

  #define SOC_Period      100          /* SOC的计算周期是100ms */
  
//SOC信息结构体
typedef  struct
{
  uint16  SOC_Init;                   //SOC初始化
  float   SOC_LowestVoltGet;          //最低电压对应的SOC值（0-1）
  float   SOC_HighestVoltGet;         //最高电压对应的SOC值（0-1）  
  uint16  SOC_CalTime;                //SOC计时 1s自增一次
  float   SOC_ValueRead;              //SOC读取值：单位为1；0-1,读取的值
  float   SOC_ValueVoltGet;           //SOC计算值：单位为1；0-1
  float   SOC_ValueInitDiff;         //SOC初值误差，读取值-计算值
  float   SOC_ValueRealtimeDiff;     //SOC实时误差，逐渐递减
  uint16  SOC_CheckTable_StartTime;   //查表计时的初值，用于常电下的SOC时长计算
  uint8   SOC_CheckTable_Flag;        //常电状态下，对是否进行查表进行标记
  uint32  SOC_StandingTime;           //SOC静置时间,本次开启时间-上次关闭时间
}SOCInfo_T;
extern    SOCInfo_T    SOCInfo;

typedef  struct
{
  float   Energy_Once_DisCharge;           //单次放电量
  float   Energy_Total_Charge;             //总充电量 
  float   Energy_Total_DisCharge;          //总放电量
   
}EnergyInfo_T;
extern    EnergyInfo_T    EnergyInfo;

/*
//未配置的表格
 typedef struct
  {
    uint8   Start_Run_Time;  //存放在系统初始化的时间
    
    
    

  }SOC_LEP_DATA_T;
 */
#endif