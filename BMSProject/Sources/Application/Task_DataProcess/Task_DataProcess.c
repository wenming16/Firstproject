/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_DataProcess.c
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:  
========================================================================*/

#include  "Task_DataProcess.h"

VoltInfo_T        g_VoltInfo;
TempInfo_T        g_TempInfo;
DataColletInfo_T  g_DataColletInfo; 
OpenWireInfo_T    g_OpenWireInfo;
InsulInfo_T       g_InsulInfo;

static void DataProcess_Volt(void);
static void DataProcess_Temp(void);
static void DataProcess_OpenWire(void);

/*=======================================================================
 *函数名:      Init_TaskDataProcess(void) 
 *功能:        采集电流、电压、温度等信息
 *参数:        无       
 *返回：       无
 *说明：       计算SOC时，所用的电流值,还需要考虑霍尔传感器是否是一样的
========================================================================*/
void Init_TaskDataProcess(void)                       
{
  //电池信息变量清零
  memset(&g_VoltInfo,   0, sizeof(VoltInfo_T));                  //电池电压数据清零
  memset(&g_TempInfo,   0, sizeof(TempInfo_T));                  //电池温度数据清零
  memset(&g_InsulInfo,     0, sizeof(InsulInfo_T));        //绝缘信息清零
  memset(&g_OpenWireInfo,    0, sizeof(OpenWireInfo_T));  //导线开路信息清零
  memset(&g_DataColletInfo,    0, sizeof(DataColletInfo_T));  //采集数据清零
  
}


/*=======================================================================
 *函数名:      Task_DataProcess(void) 
 *功能:        采集电流、电压、温度等信息
 *参数:        无       
 *返回：       无
 *说明：       计算SOC时，所用的电流值,还需要考虑霍尔传感器是否是一样的
========================================================================*/
void Task_DataProcess(void)                       
{
  DataProcess_Volt();
  DataProcess_Temp();
  DataProcess_OpenWire();
}

/*=======================================================================
 *函数名:      DataProcess_Volt()  
 *功能:        采集电压信息
 *参数:        无       
 *返回：       无
 
 *说明：        
========================================================================*/
static
void DataProcess_Volt(void)
{
  uint8   i;
  uint16  max_v=0,min_v=0xFFFFFFFF;
  uint32  total_v=0;
  uint8   max_vnub=0,min_vnub=0;
  
  //读取0~24节电池的电压
  for(i=0;i<NUM_Battery;i++) 
  {
    g_VoltInfo.CellVolt[i] = g_LTC6811_VoltInfo.CellVolt[i];  
  }
  
  //读取最大电压以及所在位置
  if (g_LTC6811_VoltInfo.CellVolt_Max > max_v) 
  {
    max_v    = g_LTC6811_VoltInfo.CellVolt_Max;
    max_vnub = g_LTC6811_VoltInfo.CellVolt_MaxNode;
  } 
  if (g_LTC6811_VoltInfo.CellVolt_Max > max_v) 
  {
    max_v    = g_VoltInfo.CellVolt_Max;
    max_vnub = g_VoltInfo.CellVolt_MaxNode;
  } 
  
  //读取最小电压以及所在位置
  if (g_LTC6811_VoltInfo.CellVolt_Min < min_v) 
  {
    min_v     = g_LTC6811_VoltInfo.CellVolt_Min;
    min_vnub  = g_LTC6811_VoltInfo.CellVolt_MinNode;
  }  
  if (g_LTC6811_VoltInfo.CellVolt_Min < min_v) 
  {
    min_v     = g_VoltInfo.CellVolt_Min;
    min_vnub  = g_VoltInfo.CellVolt_MinNode;
  }  
      
  total_v = 0.5*(g_LTC6811_VoltInfo.CellVolt_Total + g_DataColletInfo.SysVolt_Total);                                 //系统总压

  
  g_VoltInfo.CellVolt_Max         = max_v;
  g_VoltInfo.CellVolt_Min         = min_v; 
  g_VoltInfo.CellVolt_MaxNode     = max_vnub;
  g_VoltInfo.CellVolt_MinNode     = min_vnub;
  g_DataColletInfo.SysVolt_Total  = total_v;
  g_VoltInfo.CellVolt_Diff        = max_v - min_v;
  g_VoltInfo.CellVolt_Ave         = g_DataColletInfo.SysVolt_Total / SYS_SERIES_YiDongLi;
} 

/*=======================================================================
 *函数名:      DataProcess_Temp(void)  
 *功能:        采集电压信息
 *参数:        无       
 *返回：       无
 
 *说明：        
========================================================================*/
static
void DataProcess_Temp(void) 
{
  uint8   i;
  uint16  max_t=0,min_t=0xFFFFFFFF;
  uint32  ave_t;
  uint8   max_tnub=0,min_tnub=0;
  
  //读取0~24节电池温度信息
  for(i=0;i<NUM_Battery;i++)
  {
    g_TempInfo.CellTemp[i] = g_LTC6811_TempInfo.CellTemp[i];
  }
  
  //读取最大温度及位置
  if (g_LTC6811_TempInfo.CellTemp_Max > max_t) 
  {
    max_t     = g_LTC6811_TempInfo.CellTemp_Max;
    max_tnub  = g_LTC6811_TempInfo.CellTemp_MaxNode;
  } 
  if (g_TempInfo.CellTemp_Max > max_t) 
  {
    max_t     = g_TempInfo.CellTemp_Max;
    max_tnub  = g_TempInfo.CellTemp_MaxNode;
  }
  
  //读取最小温度及位置
  if (g_LTC6811_TempInfo.CellTemp_Min < min_t) 
  {
    min_t     = g_LTC6811_TempInfo.CellTemp_Min;
    min_tnub  = g_LTC6811_TempInfo.CellTemp_MinNode;
  } 
  if (g_TempInfo.CellTemp_Min < min_t) 
  {
    min_t     = g_TempInfo.CellTemp_Min;
    min_tnub  = g_TempInfo.CellTemp_MinNode;
  }    
       
  ave_t = 0.5*(g_LTC6811_TempInfo.CellTemp_Ave + g_TempInfo.CellTemp_Ave);                                 //系统平均温度

  
  g_TempInfo.CellTemp_Max         = max_t;
  g_TempInfo.CellTemp_Min         = min_t; 
  g_TempInfo.CellTemp_MaxNode     = max_tnub;
  g_TempInfo.CellTemp_MinNode     = min_tnub;
  g_TempInfo.CellTemp_Ave         = ave_t;
  g_TempInfo.CellTemp_Diff        = max_t - min_t;
}

/*=======================================================================
 *函数名:      DataProcess_OpenWire(void)  
 *功能:        采集电压信息
 *参数:        无       
 *返回：       无
 
 *说明：        
========================================================================*/
static
void DataProcess_OpenWire(void) 
{
  uint8   i;
  
  //读取主板采集的导线开路信息
  for(i=0;i<NUM_IC;i++) 
  {
    g_OpenWireInfo.OpenWire_Node[i] = g_LTC6811_OpwireInfo.OpenwireLocation[i];
    
  }
  for(i=0;i<NUM_OPENWIRE;i++)
  {
    if(g_OpenWireInfo.OpenWire_Node[i] != 0)
    {
      g_OpenWireInfo.OpenWire_Status = 1;  //导线开路故障
      break;
    }
    else
    {
      g_OpenWireInfo.OpenWire_Status = 0;
    }
  }
  
}





