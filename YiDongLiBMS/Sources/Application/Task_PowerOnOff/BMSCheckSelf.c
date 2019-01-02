/*=======================================================================
 *Subsystem:   裸机
 *File:        BMSCheckSelf.C
 *Author:      WenYuhao
 *Description: 通信：
               接口：
               波特率：
               只有自检成功后才能闭合响应的开关
/* ========================================================================
 * History:    修改历史记录列表；
 * 1. Date:    
      Author:  
      Modification: 
========================================================================*/
#include  "BMSCheckSelf.h"  
#include  "WorkModeJudge.h"
#include  "BattInfoConfig.h"
#include  "FltLevcfg.h"
#include  "Task_DataProcess.h"
#include  "Task_InsulDetect.h"
#include  "Task_UpMonitor.h"
#include  "Init_Sys.h"
#include  "PIT.h"
#include  "Task_VoltTempCollect.h"

Flt_BMSCheckSelf_T  g_Flt_BMSCheckSelf;
/*=======================================================================
 *函数名:      Checkself_BattState() 
 *功能:        BMS自检前的数据采集及处理
 *参数:        time:PIT中断周期       
 *返回：       无
 *说明：       任务时间到标志位给1，10ms执行一次
========================================================================*/
static
void Checkself_BattState(uint16 time)
{
  switch(time)
  {
    case 1:
      Task_OpenWireDetect();          //导线开路
    break;  
    
    case 6:
      Task_VoltCMDSend();             //电压寄存器清零
    break;
    
    case 8:
      Task_VoltCollect();             //电压读取函数
    break;
    
    case 10:
      Task_TempCMDSend();             //温度清零函数
    break;
    
    case 12:
      Task_TempCollect();             //温度采集函数
    break;

    case 14:
      Task_InsulationDetect();        //绝缘检测判断
    break;
    
    case 16:
      Task_DataProcess();
      memset(&PIT_TimePeriod, 0x00, sizeof(PIT_TimePeriod_T));
    break;
  }
}

/*=======================================================================
 *函数名:      放电自检判断项 
 *功能:        初始化后进行自检,自检只要一次通过则闭合继电器
 *参数:        参照各个函数       
 *返回：       参照各个函数
 *说明：       
========================================================================*/
/************************放电系统总压过低********************************/
static
uint32 CheckSelf_SysVoltLow_DischagTemp(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Sys_Low2*1000);
  }
  else //低温
  {
    return (g_BMSMonitor_New_LT.Voll_Sys_Low1_LT*1000);
  }
}
static
uint8  CheckSelf_SysVoltLow_DisCharge(uint32 totalvolt, uint8 temp)
{
   static uint8 cnt=0;
   if(totalvolt > CheckSelf_SysVoltLow_DischagTemp(temp))
   {
      cnt = 0;
      return 0;  //自检成功
      
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电单体电压过低********************************/
static
uint16 CheckSelf_CellVoltLow_Temp(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Cell_Low2);//可换成标定变量(分辨率：0.001V/位)
  }
  else //低温
  {
    return (g_BMSMonitor_New_LT.Volt_Cell_Low2_LT);//可换成标定变量(分辨率：0.001V/位)
  }
}
static
uint8  CheckSelf_CellVoltLow_DisCharge(uint16 Voltmin, uint8 temp)
{
   static uint8 cnt=0;
   if(Voltmin > CheckSelf_CellVoltLow_Temp(temp))
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电单体温度过高********************************/
static
uint8  CheckSelf_CellTempHigh_DisCharge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp < g_BMSMonitor_Temp.Temp_DisCharge_High2)
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电单体温度过低********************************/
static
uint8  CheckSelf_CellTempLow_DisCharge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp > g_BMSMonitor_Temp.Temp_DisCharge_Low2)
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电电流过过高********************************/
static
uint8  CheckSelf_CurrentOver_DisCharge(float curr)
{
   static uint8 cnt=0;
   if(curr < (g_BMSMonitor_Curr.Current_DisCharge_High2*0.1-750))
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}

/************************放电绝缘故障********************************/
static
uint8  CheckSelf_InsulDetect_DisCharge(uint8 insul)
{
   static uint8 cnt=0;
   if(insul > (g_BMSMonitor_Insul.Insulation_Resis2*0.1)) //0.1kΩ/V
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}

/*=======================================================================
 *函数名:      充电自检判断项 
 *功能:        初始化后进行自检,自检只要一次通过则闭合继电器
 *参数:        参照各个函数       
 *返回：       参照各个函数
 *说明：       
========================================================================*/
/************************充电系统总压过高********************************/
static
uint8  CheckSelf_SysVoltHigh_Charge(uint32 totalvolt)
{
   static uint8 cnt=0;
   if(totalvolt < g_BMSMonitor_Volt.Volt_Sys_High2*1000)//移植时注意分辨率和uint16是否合适
   {
      cnt = 0;
      return 0;  //自检成功
      
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}
/************************充电单体电压过高********************************/
static
uint8  CheckSelf_CellVoltHigh_Charge(uint16 Voltmax)
{
   static uint8 cnt=0;
   if(Voltmax< g_BMSMonitor_Volt.Volt_Cell_High2)     //问题同上
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}
/********************************************************************/
static
uint8  CheckSelf_CellTempHigh_Charge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp < g_BMSMonitor_Temp.Temp_Charge_High2)  //注意偏移量
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;//自检故障
      }
   }
   return 2;
}
/********************************************************************/
static
uint8  CheckSelf_CellTempLow_Charge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp > g_BMSMonitor_Temp.Temp_Charge_Low2) //注意偏移量
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;
        //自检故障
      }
      
   }
   return 2;
}

/********************************************************************/
static
uint8  CheckSelf_InsulDetect_Charge(uint8 insul)
{
   static uint8 cnt=0;
   if(insul > g_BMSMonitor_Insul.Insulation_Resis2*0.1) 
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt >= 2)
      {
        cnt = 2;
      }
      if(++cnt>=2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}
/*=======================================================================
 *函数名:      CheckSelf_OpenWireDetect 
 *功能:        绝缘检测的状态判断
 *参数:        state:绝缘检测输入状态       
 *返回：       uint8:是否存在绝缘故障,0:正常;1:故障
 *说明：       初始化作为自检使用
========================================================================*/
static
uint8  CheckSelf_OpenWireDetect(uint8 state)
{
  if(state == 0)
  {
     return 0;  //自检成功
  }
  return 1;
}

/*=======================================================================
 *函数名:      CheckSelf_Discharge() 
 *功能:        放电时自检的状态判断
 *参数:        无      
 *返回：       uint8:是否存在自检故障,0:正常;1:故障
 *说明：       初始化作为自检使用
========================================================================*/
static
uint8 CheckSelf_Discharge(Flt_BMSCheckSelf_T*ptr)
{
   uint8 state=0;
   memset(ptr, 0x00, sizeof(Flt_BMSCheckSelf_T));
   
   ptr->SysVolt_Low    = CheckSelf_SysVoltLow_DisCharge(g_VoltInfo.SysVolt_Total, g_TempInfo.CellTemp_Ave);
   state = state|ptr->SysVolt_Low;
   
   ptr->CellVolt_Low   = CheckSelf_CellVoltLow_DisCharge(g_VoltInfo.CellVolt_Min, g_TempInfo.CellTemp_Ave);
   state = state|ptr->CellVolt_Low;
   
   ptr->CellTemp_Over  = CheckSelf_CellTempHigh_DisCharge(g_TempInfo.CellTemp_Max);
   state = state|ptr->CellTemp_Over;
   
   ptr->CellTemp_Low   = CheckSelf_CellTempLow_DisCharge(g_TempInfo.CellTemp_Min);
   state = state|ptr->CellTemp_Low;
   
   ptr->SysCurr_Over   = CheckSelf_CurrentOver_DisCharge(g_DataColletInfo.DataCollet_Current_Filter);
   state = state|ptr->SysCurr_Over;
   
   ptr->SysInsul_Flt   = CheckSelf_InsulDetect_DisCharge(g_IsoDetect.insulation_resist);
   state = state|ptr->SysInsul_Flt;
   
   ptr->OpenWire_Flt  = CheckSelf_OpenWireDetect(g_OpenWireInfo.OpenWire_Status);
   state = state|ptr->OpenWire_Flt;
   return state;
}

/*=======================================================================
 *函数名:      CheckSelf_Charge() 
 *功能:        充电时自检的状态判断
 *参数:        无      
 *返回：       uint8:是否存在自检故障,0:正常;1:故障
 *说明：       初始化作为自检使用
========================================================================*/
static
uint8 CheckSelf_Charge(Flt_BMSCheckSelf_T*ptr)
{
   uint8 state=0;
   memset(ptr, 0x00, sizeof(Flt_BMSCheckSelf_T));
   
   ptr->SysVolt_Over   = CheckSelf_SysVoltHigh_Charge(g_VoltInfo.SysVolt_Total);
   state = state|ptr->SysVolt_Over;
   
   ptr->CellVolt_Over  = CheckSelf_CellVoltHigh_Charge(g_VoltInfo.CellVolt_Max);
   state = state|ptr->CellVolt_Over;
   
   ptr->CellTemp_Over  = CheckSelf_CellTempHigh_Charge(g_TempInfo.CellTemp_Max);
   state = state|ptr->CellTemp_Over;
   
   ptr->CellTemp_Low   = CheckSelf_CellTempLow_Charge(g_TempInfo.CellTemp_Min);
   state = state|ptr->CellTemp_Low;
   
   ptr->SysInsul_Flt   = CheckSelf_InsulDetect_Charge(g_IsoDetect.insulation_resist);
   state = state|ptr->SysInsul_Flt;
   
   ptr->OpenWire_Flt  = CheckSelf_OpenWireDetect(g_OpenWireInfo.OpenWire_Status);
   state = state|ptr->OpenWire_Flt;
   return state;
}
/*=======================================================================
 *函数名:      BMS_CheckSelf() 
 *功能:        自检总函数
 *参数:        无      
 *返回：       uint8:是否存在自检故障,0:正常;1:故障
 *说明：       初始化中,若自检成功则执行任务,否则一直自检
========================================================================*/
static
uint8 CheckSelf_Process(uint16 time, uint8 workmode, uint8 sysinitstate)
{
   Checkself_BattState(time);    //中断时间参数
   
   switch(workmode)
   {
     case MODE_DISCHARGE :
       if((CheckSelf_Discharge(&g_Flt_BMSCheckSelf) == 0) && (sysinitstate == 0)) //故障判断与初始化状态0:正常
       {
          memset(&PIT_TimePeriod, 0x00, sizeof(PIT_TimePeriod_T));
          return 0;
       }
     break; 
     
     case MODE_CHARGE:
       if((CheckSelf_Charge(&g_Flt_BMSCheckSelf) == 0) && (sysinitstate == 0)) //故障判断与初始化状态0:正常
       {
          memset(&PIT_TimePeriod, 0x00, sizeof(PIT_TimePeriod_T));
          return 0;
       }
     break;                                                                 
   }
   return 1; 
}
/*=======================================================================
 *函数名:      CheckSelf_DelayTime() 
 *功能:        自检函数
 *参数:        time:激励时钟
               mode:工作模式
 *返回：       无
 *说明：       BMS自检过程中，只要BMS工作状态改变那么保证自检只进行1次
========================================================================*/
 static
 void CheckSelf_DelayTime(uint16 ts)
 {
    uint16 i,j;
    for(i=0; i<100; i++)
    {
      for(j=0; j<ts; j++);
    }
 }

/*=======================================================================
 *函数名:      BMS_CheckSelf() 
 *功能:        自检函数
 *参数:        time:激励时钟
               mode:工作模式
 *返回：       无
 *说明：       BMS自检过程中，只要BMS工作状态改变那么保证自检只进行1次
========================================================================*/
static
void BMS_CheckselfOnce(uint16 time, uint8 mode)
{
   static uint8 workmode = 1;
   if(workmode != mode)
   {
      while(CheckSelf_Process(time, g_WorkStateJudge.WorkState, g_SysInitState.AllInit_State)!=0)
      {
         BMSCheckself_UpMonitor(&g_SysInitState, &g_Flt_BMSCheckSelf);
         CheckSelf_DelayTime(10);
      }
      workmode = mode; 
   }
}
/*=======================================================================
 *函数名:      Task_BMSWorkModeCheckself() 
 *功能:        自检及工作状态的判断
 *参数:        无      
 *返回：       无
 *说明：       BMS自检过程中，只要BMS工作状态改变那么保证自检只进行1次
========================================================================*/
void BMS_WorkModeCheckself(void)
{
  BMS_CheckselfOnce(PIT_TimePeriod.T500ms, g_WorkStateJudge.WorkState);
  g_WorkStateJudge.WorkState = WokeModeJudgment();
}

