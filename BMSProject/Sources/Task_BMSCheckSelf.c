/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_BMSCheckSelf.C
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
#include "Task_BMSCheckSelf.h"  

BMSCheckSelf_T  BMSCheckSelf;
/*=======================================================================
 *函数名:      Checkself_BattState() 
 *功能:        BMS自检前的数据采集及处理
 *参数:        time:PIT中断周期       
 *返回：       无
 *说明：       任务时间到标志位给1，10ms执行一次
========================================================================*/
void Checkself_BattState(uint16 *time)
{
  switch(*time)
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
      *time = 0;
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
  if(Temp>=0+40)//常温(>=0℃)
  { 
    return ((uint32)(2850)*(SYS_SERIES_YiDongLi));//可换成标定变量(分辨率：0.001V/位)
  }
  else //低温
  {
    return ((uint32)(2700)*(SYS_SERIES_YiDongLi));//可换成标定变量(分辨率：0.001V/位)
  }
}
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
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电单体电压过低********************************/
static
uint32 CheckSelf_CellVoltLow_Temp(uint8 Temp)
{
  if(Temp>=0+40)//常温(>=0℃)
  { 
    return ((uint32)(2800));//可换成标定变量(分辨率：0.001V/位)
  }
  else //低温
  {
    return ((uint32)(2500));//可换成标定变量(分辨率：0.001V/位)
  }
}

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
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电单体温度过高********************************/
uint8  CheckSelf_CellTempHigh_DisCharge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp < 60+40)
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电单体温度过低********************************/
uint8  CheckSelf_CellTempLow_DisCharge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp > (-30+40))
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1; //自检故障
      }
      
   }
   return 2;
}

/************************放电电流过过高********************************/
uint8  CheckSelf_CurrentOver_DisCharge(float curr)
{
   static uint8 cnt=0;
   if(curr > 650)
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}

/************************放电绝缘故障********************************/
uint8  CheckSelf_InsulDetect_DisCharge(uint8 insul)
{
   static uint8 cnt=0;
   if(insul < 0.1*SYS_SERIES_YiDongLi*3.65) //0.1kΩ/V
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
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
uint8  CheckSelf_SysVoltHigh_Charge(uint32 totalvolt)
{
   static uint8 cnt=0;
   if(totalvolt < 37000*SYS_SERIES_YiDongLi)//移植时注意分辨率和uint16是否合适
   {
      cnt = 0;
      return 0;  //自检成功
      
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}
/************************充电单体电压过高********************************/
uint8  CheckSelf_CellVoltHigh_Charge(uint16 Voltmax)
{
   static uint8 cnt=0;
   if(Voltmax<37000)     //问题同上
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;//自检故障
      }
      
   }
   return 2;
}
/********************************************************************/
uint8  CheckSelf_CellTempHigh_Charge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp < 55+40)  //注意偏移量
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;//自检故障
      }
   }
   return 2;
}
/********************************************************************/
uint8  CheckSelf_CellTempLow_Charge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp > (-5+40)) //注意偏移量
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;
        //自检故障
      }
      
   }
   return 2;
}

/********************************************************************/
uint8  CheckSelf_InsulDetect_Charge(uint8 insul)
{
   static uint8 cnt=0;
   if(insul < 0.1*SYS_SERIES_YiDongLi*3.65) //0.1kΩ/V
   {
      cnt = 0;
      return 0;  //自检成功
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
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
uint8 CheckSelf_Discharge(void)
{
   uint8 state,temp;
   temp  = CheckSelf_SysVoltLow_DisCharge(DataColletInfo.SysVolt_Total, TempInfo.CellTemp_Ave);
   state = state|temp;
   temp  = CheckSelf_CellVoltLow_DisCharge(VoltInfo.CellVolt_Min, TempInfo.CellTemp_Ave);
   state = state|temp;
   temp  = CheckSelf_CellTempHigh_DisCharge(TempInfo.CellTemp_Max);
   state = state|temp;
   temp  = CheckSelf_CellTempLow_DisCharge(TempInfo.CellTemp_Min);
   state = state|temp;
   temp  = CheckSelf_CurrentOver_DisCharge(DataColletInfo.DataCollet_Current_Filter);
   state = state|temp;
   temp  = CheckSelf_InsulDetect_DisCharge(IsoDetect.insulation_resist);
   state = state|temp;
   temp  = CheckSelf_OpenWireDetect(OpenWireInfo.OpenWire_Status);
   state = state|temp;
   return state;
}

/*=======================================================================
 *函数名:      CheckSelf_Charge() 
 *功能:        充电时自检的状态判断
 *参数:        无      
 *返回：       uint8:是否存在自检故障,0:正常;1:故障
 *说明：       初始化作为自检使用
========================================================================*/
uint8 CheckSelf_Charge(void)
{
   uint8 state,temp;
   temp  = CheckSelf_SysVoltHigh_Charge(DataColletInfo.SysVolt_Total);
   state = state|temp;
   temp  = CheckSelf_CellVoltHigh_Charge(VoltInfo.CellVolt_Max);
   state = state|temp;
   temp  = CheckSelf_CellTempHigh_Charge(TempInfo.CellTemp_Max);
   state = state|temp;
   temp  = CheckSelf_CellTempLow_Charge(TempInfo.CellTemp_Min);
   state = state|temp;
   temp  = CheckSelf_InsulDetect_Charge(IsoDetect.insulation_resist);
   state = state|temp;
   temp  = CheckSelf_OpenWireDetect(OpenWireInfo.OpenWire_Status);
   state = state|temp;
   return state;
}
/*=======================================================================
 *函数名:      BMS_CheckSelf() 
 *功能:        自检总函数
 *参数:        无      
 *返回：       uint8:是否存在自检故障,0:正常;1:故障
 *说明：       初始化中,若自检成功则执行任务,否则一直自检
========================================================================*/
uint8 BMS_CheckSelf(uint16 *time, uint8 workmode)
{
   Checkself_BattState(time);//中断时间参数
   switch(workmode)
   {
     case MODE_DISCHARGE :
       if((CheckSelf_Discharge() == 0) && (System_InitState.AllInit_State == 0)) //故障判断与初始化状态0:正常
       {
          
          *time = 0;//时间变为0,可以进行后续任务执行
          return 0;
       }
     break; 
     
     case MODE_CHARGE:
       if((CheckSelf_Charge() == 0) && (System_InitState.AllInit_State == 0)) //故障判断与初始化状态0:正常
       {
          *time = 0;//时间变为0,可以进行后续任务执行
          return 0;
       }
     break;                                                                 
   }
   return 1; 
}

