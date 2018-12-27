/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_DataCollection.c
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:  
========================================================================*/

#include  "TypeDefinition.h"
#include  "BattInfoParaStructure.h"
#include  "ADC.h"
#include  "Task_DataProcess.h"
#include  "Filter_Function.h"
#include  "CAN.h"
#include  "DataFromCSSU.h"

DataColletInfo_T DataColletInfo;

static void DataCollection_Volt(void);
static void DataCollection_Temp(void);
/*=======================================================================
 *函数名:      Task_DataProcess(void) 
 *功能:        采集电流、电压、温度等信息
 *参数:        无       
 *返回：       无
 *说明：       计算SOC时，所用的电流值,还需要考虑霍尔传感器是否是一样的
========================================================================*/
void Task_DataProcess(void)                       
{
  
  
}

/*=======================================================================
 *函数名:      DataCollection_Volt()  
 *功能:        采集电压信息
 *参数:        无       
 *返回：       无
 
 *说明：        
========================================================================*/
static
void DataCollection_Volt(void)
{
  uint8   i;
  uint16  max_v=0,min_v=0xFFFFFFFF;
  uint32  total_v=0;
  uint8   max_vnub=0,min_vnub=0;
  
  for(i=0; i<SYS_NUMBER_BOX_MODULE; i++)
  {
    if (Module_VoltInfo[i].Max_VoltC > max_v) 
    {
      max_v = Module_VoltInfo[i].Max_VoltC;
      max_vnub = Module_VoltInfo[i].Max_VoltC_Node;
    } 
    if (Module_VoltInfo[i].Min_VoltC < min_v) 
    {
      min_v = Module_VoltInfo[i].Min_VoltC;
      min_vnub = Module_VoltInfo[i].Min_VoltC_Node;
    }      
    total_v += DataColletInfo.ModuleVolt_Total[i] ; //两个模组的电压之和
    total_v *= 0.5;                                 //系统总压
  }
  
  VoltInfo.CellVolt_Max         = max_v;
  VoltInfo.CellVolt_Min         = min_v; 
  VoltInfo.CellVolt_MaxNode     = max_vnub;
  VoltInfo.CellVolt_MinNode     = min_vnub;
  DataColletInfo.SysVolt_Total  = total_v;
  VoltInfo.CellVolt_Diff        = max_v - min_v;
  VoltInfo.CellVolt_Ave         = DataColletInfo.SysVolt_Total / SYS_SERIES;
} 

/*=======================================================================
 *函数名:      DataCollection_Temp()  
 *功能:        采集电压信息
 *参数:        无       
 *返回：       无
 
 *说明：        
========================================================================*/
static
void DataCollection_Temp(void) 
{
  uint8   i;
  uint16  max_t=0,min_t=0xFFFFFFFF;
  uint32  ave_t;
  uint8   max_tnub=0,min_tnub=0;
  
  for(i=0; i<SYS_NUMBER_BOX_MODULE; i++)
  {
    if (Module_TempInfo[i].Max_Temp > max_t) 
    {
      max_t = Module_TempInfo[i].Max_Temp;
      max_tnub = Module_TempInfo[i].Max_TempNode;
    } 
    if (Module_TempInfo[i].Min_Temp < min_t) 
    {
      min_t = Module_TempInfo[i].Min_Temp;
      min_tnub = Module_TempInfo[i].Min_TempNode;
    }      
    ave_t   += Module_TempInfo[i].Ave_Temp ; //模组的平均温度
    ave_t   *= 0.5;                                 //系统平均温度
  }
  
  TempInfo.CellTemp_Max         = max_t;
  TempInfo.CellTemp_Min         = min_t; 
  TempInfo.CellTemp_MaxNode     = max_tnub;
  TempInfo.CellTemp_MinNode     = min_tnub;
  TempInfo.CellTemp_Ave         = ave_t;
  TempInfo.CellTemp_Diff        = max_t - min_t;
}





